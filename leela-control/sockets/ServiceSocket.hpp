//
// Created by root on 1/23/19.
//

#pragma once

#include "ISocket.hpp"

namespace endpoints
{
class ServiceSocket final : public ISocket
{
public:
    ServiceSocket(
            ISocketSysCall& sysCall,
            Config::TcpConfiguration& tcpConfiguration,
            applications::IDataListener& dataListener,
            const Config::AppConfiguration& config
    );

    ~ServiceSocket();
};
} // namespace endpoints
