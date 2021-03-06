#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <fstream>
#include <iostream>
#include "AppConfiguration.hpp"
#include "applications/AppInstance.hpp"

namespace
{
    boost::program_options::options_description createProgramOptionsDescription()
    {
        auto description = boost::program_options::options_description();
        using boost::program_options::value;

        description.add_options()
            (configurations::leelaPath, value<std::string>()->default_value("./"), "leela zero execute path")
            (configurations::enableLeelaLog, value<bool>()->default_value(false), "enable leela zero log")
            (configurations::serviceAddress, value<std::string>()->required(), "leela control service address")

            (configurations::tcpServiceIp, value<std::string>()->required(), "leela control tcp service ip address")
            (configurations::tcpServicePort, value<int>()->required(), "leela control tcp service port")
            (configurations::tcpBufferSize, value<int>()->required(), "leela control tcp service buffer size")
            (configurations::tcpKeepAlive, value<int>()->required(), "leela control tcp service open keeplive")
            (configurations::tcpKeepIdle, value<int>()->required(), "leela control tcp service idle to detect")
            (configurations::tcpKeepInterval, value<int>()->required(), "leela control tcp service detect interval")
            (configurations::tcpKeepCount, value<int>()->required(), "leela control tcp service detect times")

            (configurations::enableLeelaLevel_1, value<bool>()->default_value(false), "leela engine level 1")
            (configurations::enableLeelaLevel_2, value<bool>()->default_value(false), "leela engine level 2")
            (configurations::enableLeelaLevel_3, value<bool>()->default_value(false), "leela engine level 3")
            (configurations::enableLeelaLevel_4, value<bool>()->default_value(false), "leela engine level 4")
            (configurations::enableLeelaLevel_5, value<bool>()->default_value(false), "leela engine level 5")
            (configurations::enableLeelaLevel_6, value<bool>()->default_value(false), "leela engine level 6")
            (configurations::enableLeelaLevel_7, value<bool>()->default_value(false), "leela engine level 7")
            (configurations::enableLeelaLevel_8, value<bool>()->default_value(false), "leela engine level 8")
            (configurations::enableLeelaLevel_9, value<bool>()->default_value(false), "leela engine level 9")
            (configurations::waitLeelazStartTimes, value<int>()->default_value(10), "wait leela zero init weight file")
            (configurations::loggerLevel, value<int>()->default_value(1), "output logger level Debug=1")
            (configurations::maxFileSize, value<int>()->default_value(3 * 1024 *1024), "each rotate log file size")
            (configurations::maxRotateFiles, value<int>()->default_value(3), "max rotate files")
            
            (configurations::portCom,  value<std::string>()->required(), "leela command port com config")
            (configurations::baudRate, value<int>()->required(), "leela command com baud rate config")
            (configurations::dataBits, value<int>()->required(), "leela command com data bits config")
            (configurations::stopBits, value<int>()->required(), "leela command com stop bits config")
            (configurations::dataPaity, value<std::string>()->required(), "leela command com data paity config")
            
            (configurations::enableGTPEngine, value<bool>()->default_value(false), "start leela with gpu");

        return description;
    }

    boost::program_options::variables_map parseProgramOptions(std::ifstream& boostOptionsStream)
    {
        namespace po = boost::program_options;

        po::variables_map appConfig;
        po::store(po::parse_config_file(boostOptionsStream, createProgramOptionsDescription()), appConfig);
        try
        {
            po::notify(appConfig);
        }
        catch (const po::error& e)
        {
            std::cout << "Error: " << "Parsing the config file failed: " << e.what() << std::endl;
            throw;
        }
        return appConfig;
    }
} // namespace

namespace configurations
{

    configurations::AppConfiguration loadFromIniFile(const std::string& configFile)
    {
        std::ifstream configFileStream{ configFile };
        AppConfiguration configuration{ { parseProgramOptions(configFileStream) } };

        return configuration;
    }

}