#pragma once

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <QtCore/QTextStream>
#include <stdlib.h>
#include <functional>
#include <string>
#include <regex>
#include <stdexcept>
#include "configurations/DefineView.hpp"

namespace helps
{
    void parseProgramOptions(int argc, char* argv[], boost::program_options::variables_map& cmdParams);

    configurations::AppAddresses getAppAddresses(const boost::program_options::variables_map& cmdParams);

	std::string boostAsioBasedHostNameResolver(const std::string& address);

    std::string createBindableAddress(
            const std::string& zmqAddress,
            const std::function<std::string(const std::string&)>& hostNameResolver = boostAsioBasedHostNameResolver);

    std::vector<std::string> parseCommands(const std::string& datas);
} // namespace helps