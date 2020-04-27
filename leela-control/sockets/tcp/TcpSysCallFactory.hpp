#pragma once
#include <memory>

namespace sockets
{
class ITcpSysCall;

class TcpSysCallFactory
{
public:
    TcpSysCallFactory() = default;
    ~TcpSysCallFactory() = default;
    std::unique_ptr<ITcpSysCall> createTcpSysCall() const;
};
}// namespace sockets