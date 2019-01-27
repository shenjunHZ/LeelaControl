//
// Created by junshen on 1/24/19.
//

#pragma once
#include <zmq.hpp>
#include <memory>
#include "DescriptorPoller.hpp"

namespace sockets
{
    enum class State
    {
        working,
        updateRequired,
        stopped
    };

	struct PollEntry
	{
		zmq_pollitem_t item;
		std::function<void()> callback;
		std::unique_ptr<zmq::socket_t> socket; // unique_ptr because socket_t doesn't move well on vector reallocations
	};

class ZmqReceiver : public DescriptorPoller
{
public:
    struct ZmqWrapper
    {
        using PollType = std::function<void(std::vector<zmq_pollitem_t>&)>;
        using RecvType = std::function<void(zmq::socket_t&, zmq::message_t&)>;

        PollType poll = [](auto& items)
        {
            zmq::poll(items);
        };

        RecvType recv = [](auto& source, auto& message)
        {
            source.recv(&message);
        };
    };

    void registerFileDescriptor(types::FileDescriptor, std::function<void()> callback) final;
    void unregisterFileDescriptor(types::FileDescriptor) final;

    void registerReceiver(zmq::socket_t&&, std::function<void(const zmq::message_t&)> messageHandler);
    void registerReceiver(zmq::socket_t&, std::function<void()> callback);

    void unregisterReceiver(const zmq::socket_t&);

    void receiveLoop();
    void shutdown();

    ZmqReceiver() = default;
    explicit ZmqReceiver(ZmqWrapper zmqWrapper)
        : m_wrapper(std::move(zmqWrapper))
    {

    }
private:
    void refreshPollCollections(std::vector<zmq_pollitem_t>&, std::vector<std::function<void()>>&) const;
    bool isServiceStopped() const;
    void setState(State);
    void addPollEntry(zmq_pollitem_t, std::function<void()>, std::unique_ptr<zmq::socket_t> socket = nullptr);
    void removePollEntry(std::function<bool(const PollEntry&)>);

private:
    State m_state{State::updateRequired};
    std::vector<PollEntry> m_pollEntries;
    zmq::message_t m_received;
    ZmqWrapper m_wrapper;
};
} // namespace sockets
