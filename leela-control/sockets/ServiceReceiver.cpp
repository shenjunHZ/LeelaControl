//
// Created by junshen on 1/25/19.
//
#include <iostream>
#include <zmq_addon.hpp>
#include "ServiceReceiver.hpp"

namespace sockets
{
    ServiceReceiver::ServiceReceiver(ZmqReceiver& zmqReceiver,
                                     zmq::context_t& context,
                                     const std::string& serviceAddress,
                                     applications::UserApp& userApp)
        : m_serviceSocket{context, zmq::socket_type::pull}
        , m_zmqReceiver{zmqReceiver}
        , m_serviceAddress{serviceAddress}
        , m_userApp{userApp}
    {
        std::cout << "Info: " << "service receiver listening on " << m_serviceAddress << std::endl;

        m_serviceSocket.setsockopt(ZMQ_IPV6, 1);
        m_serviceSocket.bind(m_serviceAddress);
        m_zmqReceiver.registerReceiver(m_serviceSocket, [&]()
        {
            receiveEvent(m_serviceSocket);
        });
    }

    void ServiceReceiver::receiveEvent(zmq::socket_t& socket)
    {
        std::cout << "Debug: " << "process event" << std::endl;
        zmq::multipart_t multipartMsg(socket);
        auto clientIdentity = multipartMsg.popstr();
        auto msg = multipartMsg.pop();

        if(0 == msg.size())
        {
            std::cout << "Debug: " << "receive service event: invalid message." << std::endl;
            return;
        }
        processEvent(msg, clientIdentity);
    }

    void ServiceReceiver::processEvent(const zmq::message_t &message, const std::string &zmqIdentity)
    {
        std::string strMessage((char*)(message.data()));

        // to do judgement protocol, is need not ignore it
        // can use proto serial
        // onGtpMessage or onProtocolMessage
        m_userApp.onMessage({strMessage, zmqIdentity});
    }
} // namespace sockets