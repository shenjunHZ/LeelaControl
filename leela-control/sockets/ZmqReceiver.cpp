//
// Created by junshen on 1/24/19.
//

#include "ZmqReceiver.hpp"
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>

namespace
{
    constexpr decltype(zmq_pollitem_t::fd) emptyFd{0};
    constexpr decltype(zmq_pollitem_t::socket) emptyZmqDescriptor{nullptr};

    bool isReadyToRead(const zmq_pollitem_t& item)
    {
        return (item.revents & ZMQ_POLLIN) != 0;
    }
}

namespace sockets
{
    void ZmqReceiver::registerFileDescriptor(const types::FileDescriptor descriptor, std::function<void()> callback)
    {
        addPollEntry({emptyZmqDescriptor, descriptor, ZMQ_POLLIN, 0}, std::move(callback));
    }

    void ZmqReceiver::unregisterFileDescriptor(types::FileDescriptor descriptor)
    {
        removePollEntry([descriptor](const auto& item)
        {
            return item.item.fd == descriptor;
        });
    }

    void ZmqReceiver::registerReceiver(zmq::socket_t&& socket, std::function<void(const zmq::message_t&)> messageHandler)
    {
        auto socketPtr = std::make_unique<zmq::socket_t>(std::move(socket));
        auto& addedSocket = *socketPtr;

        std::function<void()> callback =
        [&addedSocket, handler = std::move(messageHandler), &received = this->m_received, &wrapper = this->m_wrapper]()
        {
            wrapper.recv(addedSocket, received);
            handler(received);
        };

        addPollEntry({addedSocket, emptyFd, ZMQ_POLLIN, 0}, std::move(callback), std::move(socketPtr));
    }

    void ZmqReceiver::registerReceiver(zmq::socket_t& socket, std::function<void()> callback)
    {
        addPollEntry({socket, emptyFd, ZMQ_POLLIN, 0}, std::move(callback));
    }

    void ZmqReceiver::unregisterReceiver(const zmq::socket_t& socket)
    {
        removePollEntry([&socket](const auto& item) { return item.item.socket == socket; });
    }

    bool ZmqReceiver::isServiceStopped() const
    {
        return m_state == State::stopped;
    }

    void ZmqReceiver::setState(const State newState)
    {
        if (not isServiceStopped())
        {
            m_state = newState;
        }
    }

    void ZmqReceiver::shutdown()
    {
        setState(State::stopped);
    }

    void ZmqReceiver::addPollEntry(zmq_pollitem_t pollItem,
        std::function<void()> callback,
        std::unique_ptr<zmq::socket_t> socket)
    {
        m_pollEntries.push_back({pollItem, std::move(callback), std::move(socket)});
        setState(State::updateRequired);
    }

    void ZmqReceiver::removePollEntry(std::function<bool(const PollEntry& item)> matcher)
    {
        const auto it = std::remove_if(m_pollEntries.begin(), m_pollEntries.end(), std::move(matcher));
        if (it != m_pollEntries.end())
        {
            m_pollEntries.erase(it, m_pollEntries.end());
            setState(State::updateRequired);
        }
    }

    void ZmqReceiver::refreshPollCollections(
        std::vector<zmq_pollitem_t>& pollItems,
        std::vector<std::function<void()>>& callbacks) const
    {
        pollItems.clear();
        callbacks.clear();
        for (const auto& entry : m_pollEntries)
        {
            pollItems.push_back(entry.item);
            callbacks.push_back(entry.callback);
        }
    }

    void ZmqReceiver::receiveLoop() try
    {
        std::vector<zmq_pollitem_t> pollItems;
        std::vector<std::function<void()>> callbacks;

        m_state = State::updateRequired;
        while (not isServiceStopped())
        {
            if (m_state == State::updateRequired)
            {
                refreshPollCollections(pollItems, callbacks);
                setState(State::working);
            }
            m_wrapper.poll(pollItems);
            for (auto i = 0u; i < pollItems.size() and m_state == State::working; ++i)
            {
                if (isReadyToRead(pollItems.at(i)))
                {
                    callbacks.at(i)();
                }
            }
        }
        std::cout << "Debug: "<< "ZmqReceiver received stop message, exiting from receiveLoop" << std::endl;
    }
    catch (...)
    {
        std::cout << "Error: " << "Unhandled exception in ZmqReceiver loop: "
                  << boost::current_exception_diagnostic_information() << std::endl;
        throw;
    }
} // namespace sockets