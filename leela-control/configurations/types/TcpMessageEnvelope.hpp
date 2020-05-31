#pragma once
#include <functional>

namespace configurations
{
namespace types
{
    using RespCallback = std::function<void(const std::string&)>;
    struct TcpMessageEnvelope
    {
        std::string payload;
        int socketFd;
        RespCallback callback;
    };
} // namespace types
} // namespace sockets