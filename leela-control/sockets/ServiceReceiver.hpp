//
// Created by junshen on 1/25/19.
//

#pragma once

#include <zmq.hpp>
#include <string>
#include "applications/UserApp.hpp"
#include "ZmqReceiver.hpp"
#include "configurations/DefineView.hpp"

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

private:
    zmq::socket_t m_serviceSocket;
    std::string m_serviceAddress;
    zmq::context_t& m_context;
    ZmqReceiver& m_zmqReceiver;
	applications::UserApp& m_userApp;
};
}// namespace sockets
