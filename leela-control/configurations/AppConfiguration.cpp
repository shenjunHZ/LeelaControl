#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <fstream>
#include "AppConfiguration.hpp"
#include "applications/AppInstance.hpp"

namespace
{
    boost::program_options::options_description createProgramOptionsDescription()
    {
        auto description = boost::program_options::options_description();
        using boost::program_options::value;

        description.add_options()
            (Config::leelaPath, value<std::string>()->default_value("./"), "leela zero execute path")
            (Config::enableLeelaLog, value<bool>()->default_value(false), "enable leela zero log");


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
            QTextStream(stdout) << "Parsing the config file failed: " << e.what() << endl;
            throw;
        }
        return appConfig;
    }
} // namespace

namespace Config
{

    Config::AppConfiguration loadFromIniFile(const std::string& configFile)
    {
        std::ifstream configFileStream{ configFile };
        AppConfiguration configuration{ { parseProgramOptions(configFileStream) } };

        return configuration;
    }

    void runApp(const AppConfiguration& configParams, const AppConfiguration& addresses)
    {
        QTextStream(stdout) << "start to run app" << endl;

        App::AppInstance appInstance(configParams, addresses);
    }

}