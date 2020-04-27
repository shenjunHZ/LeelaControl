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
        * ZMQ_EVENT_CONNECTED�������ѽ���
        * ZMQ_EVENT_CONNECT_DELAYED��ͬ������ʧ�ܣ����ڽ�������
        * ZMQ_EVENT_CONNECT_RETRIED�������첽����/����
        * ZMQ_EVENT_LISTENING��socket�Ѿ�����ĳ����ַ��׼���ý�����������
        * ZMQ_EVENT_BIND_FAILED��socket�޷����������ַ��
        * ZMQ_EVENT_ACCEPTED���������󱻽���
        * ZMQ_EVENT_ACCEPT_FAILED���޷����ܿͻ��˵���������
        * ZMQ_EVENT_CLOSED�����ӹر�
        * ZMQ_EVENT_CLOSE_FAILED�������޷����ر�
        * ZMQ_EVENT_DISCONNECTED���Ự���ƻ�
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