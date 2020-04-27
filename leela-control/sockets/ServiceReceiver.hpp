//
// Created by junshen on 1/25/19.
//

#pragma once

#include <zmq.hpp>
#include <string>
#include "applications/UserApp.hpp"
#include "ZmqReceiver.hpp"
#include "configurations/DefineView.hpp"
#include "SocketMonitorNotifier.hpp"

namespace sockets
{
class ServiceReceiver
{
public:
    ServiceReceiver(ZmqReceiver& zmqReceiver,
        	zmq::context_t& context,
        	const std::string& serviceAddress,
			applications::UserApp& userApp);

private:
    void receiveEvent(zmq::socket_t& socket);
    void processEvent(const zmq::message_t&, const std::string&);

    void startSocketMonitor();

private:
    zmq::socket_t  m_serviceSocket;
    SocketMonitorNotifier m_socketMonitorNotifier;
    std::string    m_serviceAddress;
    ZmqReceiver&   m_zmqReceiver;
    zmq::context_t& m_context;
	applications::UserApp& m_userApp;
};
}// namespace sockets
