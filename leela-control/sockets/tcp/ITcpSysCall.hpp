#pragma once
//#include <arpa/inet.h>
#ifdef WIN_ENV_RUN
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif // WIN_ENV_RUN

namespace sockets
{
    class ITcpSysCall
    {
    public:
        virtual ~ITcpSysCall() = default;
        // create socket file descriptor
        virtual int wrapperSocket(int domain, int type, int protocol) = 0;
        // binding port
        virtual int wrapperSocketBind(struct sockaddr* servAddr) = 0;
        // start listen
        virtual bool wrapperSocketListen(int backlog) = 0;
        // accept link
        virtual int wrapperSocketAccept(sockaddr_in& clientAddr, int size) = 0;
        // send message
        virtual int wrapperSocketSend(int fd, const char* data, int size, int flags = 0) = 0;
        // receive message
        virtual int wrapperSocketRecv(int fd, char* data, int size, int flags = 0) = 0;
        // connect to server
        virtual int wrapperSocketConnect(int fd, const sockaddr_in servAddr, int size) = 0;
        // close socket
        virtual void wrapperSocketClose() const = 0;
        // set socket mode
        virtual void setNonblockingMode() const = 0;
        // get socket file descriptor
        virtual int getFileDescriptor() const = 0;
        // get socket listening status
        virtual bool isServerListeningOnSocket() const = 0;
        // close socket
        virtual void closeSocket(int socketFd) const = 0;
        // set socket params
        virtual int wrapperSetsockopt(int fd, int level, int optionName, const void* optionVal, int optionLen) const = 0;
    };
}