#include "TcpSysCallFactory.hpp"
#include "TcpSysCall.hpp"

namespace sockets
{
    std::unique_ptr<ITcpSysCall> TcpSysCallFactory::createTcpSysCall() const
    {
        return std::make_unique<TcpSysCall>();
    }
}