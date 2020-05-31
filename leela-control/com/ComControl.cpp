#include "applications/ServiceContext.hpp"
#include "applications/UserApp.hpp"
#include "configurations/DefineView.hpp"
#include "configurations/types/TcpMessageEnvelope.hpp"
#include "com/include/ComControl.hpp"
#include "com/include/ComSerial.hpp"
#include "helps/ParseHelp.hpp"

namespace
{
    std::atomic_bool keep_running(true);
    std::string getPortCom(const configurations::AppConfiguration& config)
    {
        if (config.find(configurations::portCom) != config.end())
        {
            return config[configurations::portCom].as<std::string>();
        }
        return "/dev/ttyUSB0";
    }

    int getBaudRate(const configurations::AppConfiguration& config)
    {
        if (config.find(configurations::baudRate) != config.end())
        {
            return config[configurations::baudRate].as<int>();
        }
        return 115200;
    }

    int getDataBits(const configurations::AppConfiguration& config)
    {
        if (config.find(configurations::dataBits) != config.end())
        {
            return config[configurations::dataBits].as<int>();
        }
        return 8;
    }

    int getStopBits(const configurations::AppConfiguration& config)
    {
        if (config.find(configurations::stopBits) != config.end())
        {
            return config[configurations::stopBits].as<int>();
        }
        return 8;
    }

    std::string getDataPaity(const configurations::AppConfiguration& config)
    {
        if (config.find(configurations::dataPaity) != config.end())
        {
            return config[configurations::dataPaity].as<std::string>();
        }
        return "NONE";
    }
}  // namespace

namespace com
{
 ComControl::ComControl(applications::ServiceContext& serviceContext, applications::UserApp& userApp)
    : m_logger{ serviceContext.m_logger }
    , m_comSerial{ std::make_unique<com::ComSerial>(getPortCom(serviceContext.m_configParams), 
         configurations::comConfigParams{ getDataBits(serviceContext.m_configParams), getDataBits(serviceContext.m_configParams),
         getStopBits(serviceContext.m_configParams), getDataPaity(serviceContext.m_configParams) }, userApp) }
    , m_serviceContext{ serviceContext }
    , m_userApp{ userApp }
{

}

ComControl::~ComControl()
{
    if (m_threadCom.joinable())
    {
        m_threadCom.join();
    }
}

void ComControl::startPortCom()
{
    int fd = m_comSerial->openCom();
    if (fd > 0)
    {
        LOG_INFO_MSG(m_logger, "start port {} com to command in file description {}.", getPortCom(m_serviceContext.m_configParams), fd);

        m_threadCom = std::thread([this, &fd]() {
            this->startPortComCommand(fd);
            });
    }
}

void ComControl::startPortComCommand(int fd)
{
    while (keep_running)
    {
        std::string comData;
        m_comSerial->readCom(comData);
        if (not comData.empty())
        {
            std::vector<std::string> commands = helps::parseCommands(comData);
            LOG_DEBUG_MSG("port com: {}, received message: {}.", fd, comData);

            for (const auto& command : commands)
            {
                m_userApp.onMessage(configurations::types::TcpMessageEnvelope{ command, fd });
            }
        }
    }
}
}  // namespace com