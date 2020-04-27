#pragma once

#include <zmq.hpp>
#include "ISocketObserver.hpp"

namespace sockets
{
    class SocketMonitorNotifier : public ISocketObserver, public zmq::monitor_t
    {
    public:
        SocketMonitorNotifier(zmq::socket_t& socket, zmq::context_t& context, const std::string& address);

        void socketMonitor() override;

        void on_event_connected(const zmq_event_t &event_, const char* addr_) override;

    private:
        //zmq::socket_t m_socket;
    };
} // namespace sockets