#pragma once

namespace sockets
{
namespace types
{
    struct TcpMessageEnvelope
    {
        std::string payload;
        int socketFd;
    };
} // namespace types
} // namespace sockets