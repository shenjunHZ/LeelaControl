#include <iostream>
#include "TcpServEndpoint.hpp"
#include "TcpSysCall.hpp"
#include "TcpSysCallFactory.hpp"
#include "configurations/AppConfiguration.hpp"

namespace sockets
{

    constexpr int bufferSize{ 128 * 1024 };
    constexpr int keepAlive = 1;    // open keepalive.
    constexpr int keepIdle = 60 *5; // if no date in 60 * 5 seconds begint to detect.
    constexpr int keepInterval = 5; // detection of interval 5 seconds.
    constexpr int keepCount = 3;    // detection times. if received will not send again.

    TcpServEndpoint::TcpServEndpoint(TcpSysCallFactory& tcpSysCallFactory, ZmqReceiver& zmqReceiver, 
        const configurations::AppAddresses& appAddresses, applications::UserApp& userApp)
        : m_tcpSysCallFactory{ tcpSysCallFactory }
        , m_zmqReceiver{ zmqReceiver }
        , m_userApp{ userApp }
        , m_appAddresses{ appAddresses }
        , m_tcpSocketClosed{ true }
    {
        startServer(appAddresses.tcpParams.tcpServiceIp, appAddresses.tcpParams.tcpServicePort);
    }

    TcpServEndpoint::~TcpServEndpoint()
    {
        std::cout << "debug: " << "Destroying tcp endpoint." << std::endl;
        while (! m_tcpSockets.empty())
        {
            closeAndUnregister(m_tcpSockets.begin()->first);
        }
        while (! m_clientSockets.empty())
        {
            m_zmqReceiver.unregisterFileDescriptor(m_clientSockets.begin()->first);
            m_clientSockets.erase(m_clientSockets.begin()->first);
        }
    }

    TcpEndpoint TcpServEndpoint::createServAddr(unsigned port, const std::string& ipAddress)
    {
        IpAddress address = IpAddress::from_string(ipAddress);
        return TcpEndpoint(address, port);
    }

    void TcpServEndpoint::startServer(const std::string& localIp, const std::uint32_t port)
    {
        std::cout << "info: " << "start server tcp ip: " << localIp << " port: " << port << std::endl;
        types::IpVersion ipVersion = getIpVersion(localIp);
        createBlockingModeSocket(ipVersion, localIp);
        setSocketKeepAlive(ipVersion);

        TcpEndpoint ipAddress{ createServAddr(port, localIp) };
        
        if ( (m_tcpSockets.at(ipVersion)->wrapperSocketBind(ipAddress.data()/*reinterpret_cast<struct sockaddr*>(&servAddr)*/)) &&
            (!m_tcpSockets.at(ipVersion)->isServerListeningOnSocket()) )
        {
            m_tcpSockets.at(ipVersion)->wrapperSocketListen(100);
        }
    }

    void TcpServEndpoint::createBlockingModeSocket(const types::IpVersion& ipVersion, const std::string& localIp)
    {
        createTcpSysCallBaseIpVersion(ipVersion);
        auto it = m_tcpSockets.find(ipVersion);
        createTcpSocket(it->first);
    }

    void TcpServEndpoint::createNonblockingModeSocket(const types::IpVersion& ipVersion, const std::string& localIp)
    {
        createTcpSysCallBaseIpVersion(ipVersion);
        createTcpSocket(ipVersion);
        m_tcpSockets.at(ipVersion)->setNonblockingMode();
    }

    void TcpServEndpoint::receiveConnectFromSocket(const types::IpVersion& ipVersion) try
    {
        std::unique_ptr<ITcpSysCall>& tcpSysCall = m_tcpSockets.at(ipVersion);
        struct sockaddr_in clientAddr;
        int clientFd = tcpSysCall->wrapperSocketAccept(clientAddr, sizeof(clientAddr));
        if (clientFd >= 0)
        {
            auto it = m_clientSockets.find(clientFd);
            if (it != m_clientSockets.end())
            {
                std::cout << "warning: " << "socket file description have exist. " << clientFd << std::endl;
                return;
            }
            std::cout << "info: " << "socket connect fd: " << clientFd << std::endl;
            m_clientSockets.emplace(clientFd, clientAddr);
            auto itFd = m_clientSockets.find(clientFd);

            m_zmqReceiver.registerFileDescriptor(itFd->first, [this, &ipVersion, &clientFd = itFd->first]() 
                {
                    receiveDataFromSocket(ipVersion, clientFd); 
                });
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }

    void TcpServEndpoint::receiveDataFromSocket(const types::IpVersion& ipVersion, const int& clientFd) try
    {
        std::unique_ptr<ITcpSysCall>& tcpSysCall = m_tcpSockets.at(ipVersion);
        std::vector<std::uint8_t> receiveBuffer(bufferSize);
        int recvSize = tcpSysCall->wrapperSocketRecv(clientFd, (char*)(receiveBuffer.data()), receiveBuffer.size() - 1);
        if (recvSize <= 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
            {
                return;
            }
            tcpSysCall->closeSocket(clientFd);
            std::cout << "info: " << "socket close fd: " << clientFd << std::endl;
            m_zmqReceiver.unregisterFileDescriptor(clientFd);
            m_clientSockets.erase(clientFd);
            return;
        }
        LOG_DEBUG_MSG("received message: {}, from socket {}.",std::string{ receiveBuffer.cbegin(), receiveBuffer.cbegin() + recvSize }, clientFd);

        m_userApp.onMessage(configurations::types::TcpMessageEnvelope{ {receiveBuffer.cbegin(), receiveBuffer.cbegin() + recvSize}, 
            clientFd, [&tcpSysCall, &clientFd](const std::string& result)
                {
                    tcpSysCall->wrapperSocketSend(clientFd, result.c_str(), result.size());
                    LOG_DEBUG_MSG("Send result to client: {}", result);
                }
            });
    }
    catch (const std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
        return;
    }

    void TcpServEndpoint::setSocketKeepAlive(const types::IpVersion& ipVersion)
    {
        std::unique_ptr<ITcpSysCall>& tcpSysCall = m_tcpSockets.at(ipVersion);
        tcpSysCall->wrapperSetsockopt(tcpSysCall->getFileDescriptor(), SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(keepAlive));
        tcpSysCall->wrapperSetsockopt(tcpSysCall->getFileDescriptor(), IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(keepIdle));
        tcpSysCall->wrapperSetsockopt(tcpSysCall->getFileDescriptor(), IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(keepInterval));
        tcpSysCall->wrapperSetsockopt(tcpSysCall->getFileDescriptor(), IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(keepCount));
    }

    void TcpServEndpoint::closeAndUnregister(const types::IpVersion& ipVersion)
    {
        if (0 < m_tcpSockets.at(ipVersion)->getFileDescriptor())
        {
            std::unique_ptr<ITcpSysCall>& tcpSysCall = m_tcpSockets.at(ipVersion);
            m_zmqReceiver.unregisterFileDescriptor(tcpSysCall->getFileDescriptor());
            tcpSysCall->wrapperSocketClose();
        }
        m_tcpSockets.erase(ipVersion);
    }

    void TcpServEndpoint::createTcpSocket(const types::IpVersion& ipVersion)
    {
        int domain = ipVersion == types::IpVersion::ipV4 ? AF_INET : AF_INET6;
        std::unique_ptr<ITcpSysCall>& tcpSysCall = m_tcpSockets.at(ipVersion);

        tcpSysCall->wrapperSocket(domain, SOCK_STREAM, 0);
        m_zmqReceiver.registerFileDescriptor(tcpSysCall->getFileDescriptor(), [this, &ipVersion]() {receiveConnectFromSocket(ipVersion); });
    }

    types::IpVersion TcpServEndpoint::getIpVersion(const std::string& ipAddress) try
    {
        return ((boost::asio::ip::address::from_string(ipAddress).is_v4()) ? types::IpVersion::ipV4 : types::IpVersion::ipV6);
    }
    catch (const std::exception&)
    {
        std::cout << "error: " << "invalid ip address." << std::endl;
        return types::IpVersion::ipV4;
    }

    void TcpServEndpoint::createTcpSysCallBaseIpVersion(const types::IpVersion& ipVersion)
    {
        std::unique_ptr<ITcpSysCall> tcpSysCall = m_tcpSysCallFactory.createTcpSysCall();
        auto it = m_tcpSockets.find(ipVersion);
        if (it == m_tcpSockets.end())
        {
            m_tcpSockets.emplace(ipVersion, std::move(tcpSysCall));
        }
    }
}