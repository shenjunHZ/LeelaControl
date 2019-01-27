//
// Created by junshen on 1/27/19.
//

#include "ParseHelp.hpp"

namespace
{
	constexpr auto tcpPrefix = "tcp://";
	const std::regex tcpAddressRegex(R"(tcp://\[*([^\]]*)\]*:(\d+))");
	const std::regex addressWithProtocolRegex(R"(^[a-zA-Z]{3,6}://[\-\w/\.]+(?::\d+)?$)");

	bool isIPAddressValid(const std::string& address)
	{
		boost::system::error_code ec;
		const auto addr = boost::asio::ip::address::from_string(address, ec);
		return not ec and not addr.is_unspecified();
	}
}// namespace

namespace helps
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

    //using namespace std::string_literals;
    std::string boostAsioBasedHostNameResolver(const std::string& address) try
    {
	    using namespace boost::asio::ip;
        tcp::resolver::query query(address, "");

        boost::asio::io_service ioService;
        const auto resolvedIterator = tcp::resolver(ioService).resolve(query);
        if (resolvedIterator != tcp::resolver::iterator())
        {
            tcp::endpoint endpoint = *resolvedIterator;
            return endpoint.address().to_string();
        }
        BOOST_THROW_EXCEPTION(std::runtime_error("Unable to resolve " + address + " with boost tcp resolver"));
	}
	catch (const boost::system::system_error& e)
	{
		BOOST_THROW_EXCEPTION(
		std::runtime_error("Error while trying to resolve " + address + " with boost tcp resolver: " + e.what()));
	}

    std::string createBindableAddress(
        const std::string& zmqAddress,
        const std::function<std::string(const std::string&)>& hostNameResolver /*= boostAsioBasedHostNameResolver*/)
    {
        std::cout << "Debug: " << "bind service address " << zmqAddress << std::endl;
        std::smatch matched;
        if (zmqAddress.find(tcpPrefix) != std::string::npos)
        {
            if (std::regex_match(zmqAddress, matched, tcpAddressRegex))
            {
                //Expects(matched.size() == 3);
                const auto& address = matched[1].str();
                if (isIPAddressValid(address))
                {
				    return zmqAddress;
                }

                const auto& resolvedAddress = hostNameResolver(address);
                const auto& ipPort = matched[2].str();
                return std::string(tcpPrefix).append(resolvedAddress).append(":").append(ipPort);
            }
            BOOST_THROW_EXCEPTION(std::invalid_argument("Tcp address " + zmqAddress + " without port"));
        }
        if (std::regex_match(zmqAddress, matched, addressWithProtocolRegex))
        {
            return zmqAddress;
        }
        BOOST_THROW_EXCEPTION(std::invalid_argument("Cannot convert " + zmqAddress + " to bindable adddress"));
    }
}// namespace helps