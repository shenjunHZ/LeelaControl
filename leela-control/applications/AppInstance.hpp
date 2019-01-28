#pragma once

#include <zmq.hpp>
#include "UserApp.hpp"
#include "configurations/AppConfiguration.hpp"
#include "configurations/DefineView.hpp"
#include "sockets/ZmqReceiver.hpp"
#include "sockets/ServiceReceiver.hpp"
#include "games/ManagementJob.hpp"
#include "ServiceContext.hpp"
namespace applications
{
    class AppInstance
    {
    public:
        AppInstance(applications::ServiceContext& serviceContext);
        ~AppInstance() = default;

    private:
        void initService(const configurations::AppConfiguration& config);

    private:
        games::ManagementJob     m_Management;
        UserApp                  m_userApp;
        sockets::ServiceReceiver m_ServiceReceiver;
    };
}