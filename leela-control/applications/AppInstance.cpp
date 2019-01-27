#include <bitset>
#include "AppInstance.hpp"
#include "games/ManagementJob.hpp"
#include "configurations/DefineView.hpp"
#include "sockets/ServiceReceiver.hpp"
#include "helps/ParseHelp.hpp"

namespace
{
    std::string getLeelaPath(const configurations::AppConfiguration& config)
    {
        if (config.find(configurations::leelaPath) != config.end())
        {
            return config[configurations::leelaPath].as<std::string>();
        }
        return "./";
    }

    bool isEnableLeelaLog(const configurations::AppConfiguration& config)
    {
        if(config.find(configurations::enableLeelaLog) != config.end())
        {
            return config[configurations::enableLeelaLog].as<bool>();
        }
        return false;
    }

    std::bitset<9> getLeelaLevel(const configurations::AppConfiguration& config)
    {
        std::bitset<9> bitSet;
        if(config.find(configurations::enableLeelaLevel_1) != config.end())
        {
            if(config[configurations::enableLeelaLevel_1].as<bool>())
            {
                bitSet.set(0);
            }
        }
        if(config.find(configurations::enableLeelaLevel_2) != config.end())
        {
            if(config[configurations::enableLeelaLevel_2].as<bool>())
            {
                bitSet.set(1);
            }
        }
        if(config.find(configurations::enableLeelaLevel_3) != config.end())
        {
            if(config[configurations::enableLeelaLevel_3].as<bool>())
            {
                bitSet.set(2);
            }
        }
        if(config.find(configurations::enableLeelaLevel_4) != config.end())
        {
            if(config[configurations::enableLeelaLevel_4].as<bool>())
            {
                bitSet.set(3);
            }
        }
        if(config.find(configurations::enableLeelaLevel_5) != config.end())
        {
            if(config[configurations::enableLeelaLevel_5].as<bool>())
            {
                bitSet.set(4);
            }
        }
        if(config.find(configurations::enableLeelaLevel_6) != config.end())
        {
            if(config[configurations::enableLeelaLevel_6].as<bool>())
            {
                bitSet.set(5);
            }
        }
        if(config.find(configurations::enableLeelaLevel_7) != config.end())
        {
            if(config[configurations::enableLeelaLevel_7].as<bool>())
            {
                bitSet.set(6);
            }
        }
        if(config.find(configurations::enableLeelaLevel_8) != config.end())
        {
            if(config[configurations::enableLeelaLevel_8].as<bool>())
            {
                bitSet.set(7);
            }
        }
        if(config.find(configurations::enableLeelaLevel_9) != config.end())
        {
            if(config[configurations::enableLeelaLevel_9].as<bool>())
            {
                bitSet.set(8);
            }
        }
        return bitSet;
    }
}
namespace applications
{
    using namespace configurations;

    AppInstance::AppInstance(const configurations::AppConfiguration& config,
        const configurations::AppAddresses& address)
        : m_zmqContext{}
        , m_zmqReceiver{}
        , m_Management{getLeelaPath(config), isEnableLeelaLog(config)}
        , m_userApp{m_Management}
        , m_ServiceReceiver{m_zmqReceiver, m_zmqContext, helps::createBindableAddress(address.serviceAddress), m_userApp}
    {
        initService(config);
    }

    void AppInstance::initService(const configurations::AppConfiguration& config)
    {
        std::bitset<9> bitSet = getLeelaLevel(config);
        for(int index = 0; index < 9; index++)
        {
            if(bitSet.test(index))
            {
                m_Management.createJobLeela(static_cast<leelaStarLevel>(index));
            }
        }

        m_zmqReceiver.receiveLoop();
    }

}