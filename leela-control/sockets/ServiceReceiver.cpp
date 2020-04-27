//
// Created by junshen on 1/25/19.
//
#include <iostream>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include "ServiceReceiver.hpp"
#include "logger/Logger.hpp"
#include "SocketMonitorNotifier.hpp"

namespace
{
    constexpr auto inprocSocketMonitor = "inproc://socketMonitor";
}
namespace sockets
{
    ServiceReceiver::ServiceReceiver(ZmqReceiver& zmqReceiver,
                                     zmq::context_t& context,
                                     const std::string& serviceAddress,
                                     applications::UserApp& userApp)
        : m_serviceSocket{context, zmq::socket_type::router} // need zmq connect identity
        , m_socketMonitorNotifier{m_serviceSocket, context, inprocSocketMonitor}
        , m_zmqReceiver{zmqReceiver}
        , m_serviceAddress{serviceAddress}
        , m_context{context}
        , m_userApp{userApp}
    {
        std::cout << "Info: " << "service zmq listening on: " << m_serviceAddress << std::endl;

//         m_serviceSocket.setsockopt(ZMQ_LINGER, 0);
//         m_serviceSocket.bind(m_serviceAddress);
//         m_zmqReceiver.registerReceiver(m_serviceSocket, [&]()
//         {
//             receiveEvent(m_serviceSocket);
//         });

        /*start a thread for socket monitor event
        * ZMQ_EVENT_CONNECTED：链接已建立
        * ZMQ_EVENT_CONNECT_DELAYED：同步连接失败，仍在进行重试
        * ZMQ_EVENT_CONNECT_RETRIED：尝试异步连接/重连
        * ZMQ_EVENT_LISTENING：socket已经绑定了某个地址，准备好接受连接请求
        * ZMQ_EVENT_BIND_FAILED：socket无法绑定在这个地址上
        * ZMQ_EVENT_ACCEPTED：连接请求被接受
        * ZMQ_EVENT_ACCEPT_FAILED：无法接受客户端的连接请求
        * ZMQ_EVENT_CLOSED：连接关闭
        * ZMQ_EVENT_CLOSE_FAILED：连接无法被关闭
        * ZMQ_EVENT_DISCONNECTED：会话被破坏
        */
 //       std::thread(&ServiceReceiver::startSocketMonitor, this);
    }

    void ServiceReceiver::receiveEvent(zmq::socket_t& socket)
    {
        LOG_DEBUG_MSG("process receive event.");
        zmq::multipart_t multipartMsg(socket);
        auto clientIdentity = multipartMsg.popstr();
        auto msg = multipartMsg.pop();

        if(0 == msg.size())
        {
            LOG_DEBUG_MSG("Receive service event: invalid message.");
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
        m_userApp.onMessage({zmqIdentity, strMessage});
    }

    void ServiceReceiver::startSocketMonitor()
    {
        m_socketMonitorNotifier.monitor(m_serviceSocket, inprocSocketMonitor);
    }

} // namespace sockets