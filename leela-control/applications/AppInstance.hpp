#pragma once

#include <zmq.hpp>
#include "UserApp.hpp"
#include "configurations/AppConfiguration.hpp"
#include "configurations/DefineView.hpp"
#include "sockets/ZmqReceiver.hpp"
#include "sockets/ServiceReceiver.hpp"
#include "games/ManagementJob.hpp"

namespace applications
{
    class AppInstance
    {
    public:
        AppInstance(const configurations::AppConfiguration& config,
            const configurations::AppAddresses& address);
        ~AppInstance() = default;

    private:
        void initService(const configurations::AppConfiguration& config);

    private:
        zmq::context_t       m_zmqContext;
        sockets::ZmqReceiver m_zmqReceiver;
        games::ManagementJob m_Management;
        sockets::ServiceReceiver m_ServiceReceiver;
        UserApp m_userApp;
    };
}