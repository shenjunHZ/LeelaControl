// LeelaControl.cpp: 定义应用程序的入口点。
/* 
* Copyright (C) jun shen
*/

#include <QtCore/QCoreApplication>
#include <boost/exception/diagnostic_information.hpp>
#include "configurations/AppConfiguration.hpp"
#include "applications/AppInstance.hpp"
#include "LeelaControl.hpp"
#include "ParseHelp.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("Leela-Control");
    constexpr int LEELA_CONTROL_VERSION = 1.0;
    app.setApplicationVersion(QString("V: ").arg(LEELA_CONTROL_VERSION));

    boost::program_options::variables_map cmdParams;
    Help::parseProgramOptions(argc, argv, cmdParams);

    try
    {
        Config::AppConfiguration configParams =
            Config::loadFromIniFile(cmdParams["config-file"].as<std::string>());
        const Config::AppConfiguration addresses;
       //Config::runApp(configParams, addresses);
        App::AppInstance appInstance(configParams, addresses);
    }
    catch (const std::exception& e)
    {
        QTextStream(stdout) << QString::fromStdString(boost::diagnostic_information(e)) << endl;
        return EXIT_FAILURE;
    }

	return app.exec();
}
