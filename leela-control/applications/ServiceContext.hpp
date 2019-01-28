//
// Created by root on 1/23/19.
//

#pragma once
#include <zmq.hpp>
#include "configurations/AppConfiguration.hpp"
#include "configurations/DefineView.hpp"
#include "sockets/ZmqReceiver.hpp"

namespace applications
{
struct ServiceContext
{
public:
    ServiceContext(const configurations::AppConfiguration& config,
        const configurations::AppAddresses& addresses)
        : m_configParams{config}
        , m_addresses{addresses}
    {

    }

    zmq::context_t m_zmqContext;
    sockets::ZmqReceiver m_zmqReceiver;
    const configurations::AppAddresses& m_addresses;
    const configurations::AppConfiguration& m_configParams;
};
} // namespace applications
