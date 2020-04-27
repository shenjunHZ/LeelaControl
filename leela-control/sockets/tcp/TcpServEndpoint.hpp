#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/detail/endpoint.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <map>
#include <memory>
#include "types/IpVersion.hpp"
#include "types/TcpMessageEnvelope.hpp"
#include "sockets/ZmqReceiver.hpp"
#include "configurations/DefineView.hpp"
#include "applications/UserApp.hpp"

namespace sockets
{
    using IpEndpoint = boost::asio::ip::detail::endpoint;
    using TcpEndpoint = boost::asio::ip::tcp::endpoint;
    using IpAddress = boost::asio::ip::address;
    class ITcpSysCall;
    class TcpSysCallFactory;

    class TcpServEndpoint final
    {
    public:
        TcpServEndpoint::TcpServEndpoint(TcpSysCallFactory& tcpSysCallFactory, ZmqReceiver& zmqReceiver, 
            const configurations::AppAddresses& appAddresses, applications::UserApp& userApp);
        ~TcpServEndpoint();

    private:
        void createTcpSocket(const types::IpVersion& ipVersion);
        void startServer(const std::string& localIp, const std::uint32_t port);
        void closeAndUnregister(const types::IpVersion& ipVersion);
        types::IpVersion getIpVersion(const std::string& ipAddress);
        void createTcpSysCallBaseIpVersion(const types::IpVersion& ipVersion);
        void createBlockingModeSocket(const types::IpVersion& ipVersion, const std::string& localIp);
        void createNonblockingModeSocket(const types::IpVersion& ipVersion, const std::string& localIp);
        void setSocketKeepAlive(const types::IpVersion& ipVersion);
        void receiveConnectFromSocket(const types::IpVersion& ipVersion);
        void receiveDataFromSocket(const types::IpVersion& ipVersion, const int& clientFd);
        TcpEndpoint createServAddr(unsigned port, const std::string& ipAddress);

    private:
        const TcpSysCallFactory& m_tcpSysCallFactory;
        ZmqReceiver& m_zmqReceiver;
        applications::UserApp& m_userApp;
        std::map<types::IpVersion, std::unique_ptr<ITcpSysCall>> m_tcpSockets;
        std::map<int, sockaddr_in> m_clientSockets;

        const configurations::AppAddresses& m_appAddresses;
        std::atomic<bool> m_tcpSocketClosed;
    };
} // end sockets