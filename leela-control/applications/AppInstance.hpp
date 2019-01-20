#pragma once
#include "configurations/AppConfiguration.hpp"

namespace Game
{
    class IManagementJob;
}

namespace App
{
    class AppInstance
    {
    public:
        AppInstance(const Config::AppConfiguration& config, const Config::AppConfiguration& address);
        ~AppInstance();

    private:
        void initService(const Config::AppConfiguration& config);

    private:
        std::unique_ptr<Game::IManagementJob> m_ptrManagement;
    };
}