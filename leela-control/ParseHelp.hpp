#pragma once
#include <boost/program_options.hpp>
#include <QtCore/QTextStream>

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
                QTextStream(stdout) << "parseProgramOptions:lexical_cast Failed in  default_value";
                QTextStream(stdout) << e.what() << endl;
            }

            try
            {
                po::store(po::parse_command_line(argc, argv, optDescr), cmdParams);
                po::notify(cmdParams);
            }
            catch (const po::error& e)
            {
                QTextStream(stdout) << e.what() << endl;
                std::exit(EXIT_FAILURE);
            }

            if (cmdParams.count("help") != 0)
            {
                std::cout << optDescr << std::endl;
                std::exit(EXIT_SUCCESS);
            }
        }
    }
}