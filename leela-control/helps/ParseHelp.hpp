#pragma once
#include <boost/program_options.hpp>
#include <QtCore/QTextStream>
#include <stdlib.h>
#include "configurations/DefineView.hpp"

namespace Help
{
    void parseProgramOptions(int argc, char* argv[], boost::program_options::variables_map& cmdParams)
    {
        namespace po = boost::program_options;

        po::options_description optDescr("");
        {
            try
            {
                auto defaultConfigFilePath = po::value<std::string>()->default_value("configuration.ini");

                optDescr.add_options()("config-file,c", defaultConfigFilePath, "Configuration file path")(
                    "help,h", "Prints this help message");
            }
            catch (boost::bad_lexical_cast& e)
            {
                std::cout << "Error: " << "parseProgramOptions:lexical_cast Failed in  default_value";
                std::cout << e.what() << std::endl;
            }

            try
            {
                po::store(po::parse_command_line(argc, argv, optDescr), cmdParams);
                po::notify(cmdParams);
            }
            catch (const po::error& e)
            {
                std::cout << "Error: " << e.what() << std::endl;
                exit(EXIT_FAILURE);
            }

            if (cmdParams.count("help") != 0)
            {
                std::cout << optDescr << std::endl;
                exit(EXIT_SUCCESS);
            }
        }
    }

    configurations::AppAddresses getAppAddresses(const boost::program_options::variables_map& cmdParams)
    {
        using namespace configurations::keys;
        return configurations::AppAddresses{cmdParams[serviceAddress].as<std::string>()};
    }
}