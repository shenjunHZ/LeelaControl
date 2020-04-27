#include <bitset>
#include "AppInstance.hpp"
#include "helps/ParseHelp.hpp"

namespace
{
    constexpr int leelaAgentCounters = 9;

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

    std::bitset<leelaAgentCounters> getLeelaLevel(const configurations::AppConfiguration& config)
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

    AppInstance::AppInstance(applications::ServiceContext& serviceContext)
        : m_Management{getLeelaPath(serviceContext.m_configParams),
                       isEnableLeelaLog(serviceContext.m_configParams),
                       serviceContext.m_logger}
        , m_userApp{m_Management}
        , m_serviceReceiver{serviceContext.m_zmqReceiver,
							serviceContext.m_zmqContext,
							helps::createBindableAddress(serviceContext.m_addresses.serviceAddress),
							m_userApp}
        , m_tcpSysCallFactory{}
        , m_tcpServEndpoint{ m_tcpSysCallFactory, serviceContext.m_zmqReceiver, serviceContext.m_addresses, m_userApp }
    {
        initService(serviceContext.m_configParams);
    }

    void AppInstance::initService(const configurations::AppConfiguration& config)
    {
        std::bitset<leelaAgentCounters> bitSet = getLeelaLevel(config);
        for(int index = 0; index < leelaAgentCounters; index++)
        {
            if(bitSet.test(index))
            {
                m_Management.createJobLeela(static_cast<leelaStarLevel>(index));
                m_Management.startJobLeela(static_cast<leelaStarLevel>(index));
            }
        }
    }

}