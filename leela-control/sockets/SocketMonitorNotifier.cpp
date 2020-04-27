#include "SocketMonitorNotifier.hpp"
#include "logger/Logger.hpp"

namespace sockets
{
    SocketMonitorNotifier::SocketMonitorNotifier(zmq::socket_t& socket, zmq::context_t& context, const std::string& address)
    {
        //m_socket.setsockopt(ZMQ_LINGER, 0);
        //m_socket.connect(address);
        LOG_DEBUG_MSG("Socket monitor connect to {}", address);
    }

    void SocketMonitorNotifier::socketMonitor()
    {

    }

    void SocketMonitorNotifier::on_event_connected(const zmq_event_t &event_, const char* addr_)
    {
        LOG_DEBUG_MSG("Connected socket descriptor {}", event_.value);
        LOG_DEBUG_MSG("Connected socket address {}", addr_);
    }

} // namespace sockets