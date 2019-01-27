//
// Created by root on 1/23/19.
//

#pragma once
#include <zmq.hpp>
#include "configurations/AppConfiguration.hpp"

namespace applications
{
class ServiceContext
{
public:
    ServiceContext(const configurations::ApplicationConfiguration& config,
        const configurations::AppAddress& addresses)
        : m_configParams{config}
        , m_addresses{addresses}
    {

    }

    zmq::context_t m_zmqContext;
    socket::ZmqReceiver m_zmqReceiver;
    const configuration::AppAddress& m_addresses;
    const configuration::ApplicationConfiguration& m_configParams;
};
} // namespace applications
