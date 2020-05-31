#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "TcpSysCall.hpp"

namespace sockets
{
    constexpr int optionValue = 1;

    TcpSysCall::TcpSysCall()
        : m_socketFd{-1}
    {
//         memset(&m_servAddr, 0, sizeof(m_servAddr));
//         m_servAddr.sin_family = AF_INET;
//         m_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//         m_servAddr.sin_port = htons(atoi(9090));
    }

    int TcpSysCall::wrapperSocket(int domain, int type, int protocol)
    {
        m_socketFd = socket(domain, type, protocol);
        if (-1 == m_socketFd)
        {
            std::cout << "error: " << "create socket error." << std::endl;
        }
        setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR, (char*)(&optionValue), sizeof(optionValue));

        return m_socketFd;
    }
    /*
    success: true
    */
    int TcpSysCall::wrapperSocketBind(struct sockaddr* servAddr)
    {
        if (0 != bind(m_socketFd, servAddr, sizeof(*servAddr)))
        {
            std::cout << "error: " << "socket bind error dress: " << std::string(servAddr->sa_data).c_str() << " family: " << servAddr->sa_family << std::endl;
            std::cout << "tcp socket bind error: " << strerror(errno) << std::endl;
            return false;
        }
        else
        {
            std::cout << "bind success." << std::endl;
        }

        return true;
    }

    bool TcpSysCall::wrapperSocketListen(int backlog)
    {
        if (0 != listen(m_socketFd, backlog))
        {
            std::cout << "error: " << "socket listen error." << std::endl;
            return false;
        }
        return true;
    }

    int TcpSysCall::wrapperSocketAccept(sockaddr_in& clientAddr, int size)
    {
#ifdef WIN_ENV_RUN
        int cliFd = accept(m_socketFd, (sockaddr*)(&clientAddr), &size);
#else
        int cliFd = accept(m_socketFd, (sockaddr*)(&clientAddr), (socklen_t*)(&size));
#endif
        if (cliFd >= 0)
        {
            std::cout << "info: " << "accept socket fd: " << cliFd << " client ip: " << inet_ntoa(clientAddr.sin_addr)
                << " port: " << ntohs(clientAddr.sin_port) << std::endl;
        }
        else
        {
            std::cout << "accept error: " << strerror(errno) << std::endl;
        }

        return cliFd;
    }

    void TcpSysCall::wrapperSocketClose() const
    {
#ifdef WIN_ENV_RUN
        closesocket(m_socketFd);
#else
        close(m_socketFd);
#endif
    }

    int TcpSysCall::wrapperSocketConnect(int cliFd, const sockaddr_in servAddr, int size)
    {
        if (! connect(cliFd, (sockaddr*)(&servAddr), size) )
        {
            std::cout << "error: " << "socket connect error." << std::endl;
            return false;
        }
        return true;
    }

    int TcpSysCall::wrapperSocketSend(int fd, const char* data, int size, int flags /* = 0*/)
    {
        return send(fd, data, size, flags);
    }

    int TcpSysCall::wrapperSocketRecv(int fd, char* data, int size, int flags /* = 0*/)
    {
        return recv(fd, data, size, flags);
    }

    int TcpSysCall::getFileDescriptor() const
    {
        return m_socketFd;
    }

    void TcpSysCall::setNonblockingMode() const
    {
#ifndef WIN_ENV_RUN
        int flags = fcntl(m_socketFd, F_GETFL);
        if (-1 == flags)
        {
            std::cout << "error: " << "Could not get socket mode." << std::endl;
            return;
        }
        flags |= O_NONBLOCK;
        flags = fcntl(m_socketFd, F_SETFL, flags);
        if (-1 == flags)
        {
            std::cout << "error: " << "Could not set socket mode." << std::endl;
        }
#else
        unsigned long ulEnable = 1;
        ioctlsocket(m_socketFd, FIONBIO, &ulEnable);
#endif // !WIN_ENV_RUN
    }

    bool TcpSysCall::isServerListeningOnSocket() const 
    {
        int isListening = 0;
        int sizeListening = sizeof(isListening);
#ifdef WIN_ENV_RUN
        const int result = getsockopt(m_socketFd, SOL_SOCKET, SO_ACCEPTCONN, (char*)(&isListening), &sizeListening);
#else
        const int result = getsockopt(m_socketFd, SOL_SOCKET, SO_ACCEPTCONN, (char*)(&isListening), (socklen_t*)(&sizeListening));
#endif
        if (-1 == result)
        {
            std::cout << "error: " << "get sock optional invalid." << std::endl;
        }
        std::cout << "info: " << "server listening on socket." << std::endl;
        return (1 == isListening);
    }

    void TcpSysCall::closeSocket(int socketFd) const
    {
#ifdef WIN_ENV_RUN
        closesocket(socketFd);
#else
        close(socketFd);
#endif
    }

    int TcpSysCall::wrapperSetsockopt(int fd, int level, int optionName, const void* optionVal, int optionLen) const
    {
        return setsockopt(fd, level, optionName, (char*)optionVal, optionLen);
    }

} // namespace sockets