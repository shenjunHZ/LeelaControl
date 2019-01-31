//
// Created by root on 1/23/19.
//

#pragma once
#include <zmq.hpp>
#include "configurations/AppConfiguration.hpp"
#include "configurations/DefineView.hpp"
#include "sockets/ZmqReceiver.hpp"
#include "logger/Logger.hpp"

namespace applications
{
struct ServiceContext
{
public:
    ServiceContext(const configurations::AppConfiguration& config,
        const configurations::AppAddresses& addresses,
                   spdlog::logger& logger)
        : m_configParams{config}
        , m_addresses{addresses}
        , m_logger{logger}
    {

    }

    zmq::context_t m_zmqContext;
    sockets::ZmqReceiver m_zmqReceiver;
    const configurations::AppAddresses& m_addresses;
    const configurations::AppConfiguration& m_configParams;
    spdlog::logger& m_logger;
};
} // namespace applications
