#include "AppInstance.hpp"
#include "games/ManagementJob.hpp"
#include "DefineView.hpp"

namespace
{
    std::string getLeelaPath(const Config::AppConfiguration& config)
    {
        if (config.find(Config::leelaPath) != config.end())
        {
            return config[Config::leelaPath].as<std::string>();
        }
        return "./";
    }

    bool isEnableLeelaLog(const Config::AppConfiguration& config)
    {
        if(config.find(Config::enableLeelaLog) != config.end())
        {
            return config[Config::enableLeelaLog].as<bool>();
        }
        return false;
    }
}
namespace App
{
    using namespace Define;
    AppInstance::AppInstance(const Config::AppConfiguration& config, const Config::AppConfiguration& address)
        : m_ptrManagement{nullptr}
    {
        initService(config);
    }

    AppInstance::~AppInstance()
    {

    }

    void AppInstance::initService(const Config::AppConfiguration& config)
    {
        std::string leelaPath = getLeelaPath(config);
        m_ptrManagement = std::make_unique<Game::ManagementJob>(leelaPath, isEnableLeelaLog(config));

        // to do 
        m_ptrManagement->createJobLeela(leelaStarLevel::STAR_LEVEL_4);

    }

}