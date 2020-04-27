#pragma once
#include "ITcpSysCall.hpp"

namespace sockets
{
    class TcpSysCall final : public ITcpSysCall
    {
    public:
        TcpSysCall();
        int wrapperSocket(int domain, int type, int protocol) override;
        
        int wrapperSocketBind(struct sockaddr* servAddr) override;
        
        bool wrapperSocketListen(int backlog) override;
        
        int wrapperSocketAccept(sockaddr_in& clientAddr, int size) override;
        
        int wrapperSocketRecv(int fd, char* data, int size, int flags = 0) override;

        int wrapperSetsockopt(int fd, int level, int optionName, const void* optionVal, int optionLen) const override;

        int wrapperSocketSend(int fd, const char* data, int size, int flags = 0) override;
        
        int wrapperSocketConnect(int fd, const sockaddr_in servAddr, int size) override;

        void wrapperSocketClose() const override;

        void setNonblockingMode() const override;

        int getFileDescriptor() const override;

        bool isServerListeningOnSocket() const override;

        void closeSocket(int socketFd) const override;

    private:
        int m_socketFd;
        //struct sockaddr_in m_servAddr;
    };
}