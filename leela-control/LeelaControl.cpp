// LeelaControl.cpp: 定义应用程序的入口点。
/* 
* Copyright (C) jun shen
*/
#include "LeelaControl.hpp"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("Leela-Control");
    constexpr int LEELA_CONTROL_VERSION = 1.0;
    app.setApplicationVersion(QString("V: %1").arg(LEELA_CONTROL_VERSION));

    boost::program_options::variables_map cmdParams;
    helps::parseProgramOptions(argc, argv, cmdParams);

    try
    {
        configurations::AppConfiguration configParams =
            configurations::loadFromIniFile(cmdParams["config-file"].as<std::string>());
        const configurations::AppAddresses addresses = helps::getAppAddresses(configParams);

        spdlog::logger& logger = logger::getLogger();
        LOG_INFO_MSG(logger, "Leela Control Program Start up.");
        applications::ServiceContext serviceContext{configParams, addresses, logger};

        applications::AppInstance appInstance(serviceContext);
        serviceContext.m_zmqReceiver.receiveLoop();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR_MSG("{}", boost::diagnostic_information(e));
        return EXIT_FAILURE;
    }

	return app.exec();
}
