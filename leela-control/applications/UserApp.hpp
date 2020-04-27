//
// Created by junshen on 1/26/19.
//

#pragma once
#include "configurations/AppConfiguration.hpp"
#include "configurations/DefineView.hpp"
#include "games/ManagementJob.hpp"
#include "sockets/tcp/types/TcpMessageEnvelope.hpp"
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>

namespace applications
{
using variant = boost::variant<configurations::messageData, sockets::types::TcpMessageEnvelope>;

class UserApp
{
public:
	UserApp(games::IManagementJob& managementJob);
	~UserApp();
	void onMessage(const configurations::messageData& data);
    void onMessage(const sockets::types::TcpMessageEnvelope& data);

    void onGTPMessage(const std::string& id, const std::string& command);

private:
	void startProcess();
	void processMessage();

private:
	std::queue<variant> m_messageDatas;
	games::IManagementJob& m_managementJob;
	mutable std::mutex m_mutex;
	std::condition_variable m_condition;
	boost::thread m_threadProcess;
    // chessboard id to leela level
    std::map<std::string, configurations::leelaStarLevel> m_socketToLeela;
};

struct Handler : public boost::static_visitor <void>
{
    Handler(UserApp& userApp) : m_userApp{ userApp }
    {

    }
    void operator () (configurations::messageData)
    {
        std::cout << "Debug: " << "process zmq message. " << std::endl;
    }
    void operator () (sockets::types::TcpMessageEnvelope messageEnvolope)
    {
        std::cout << "Debug: " << "process tcp socket message. " << std::endl;

        onGTPMessage(messageEnvolope.socketFd, messageEnvolope.payload); // to do decode payload to command and chassBoardID
    }

    //    void UserApp::handleConnectionRequest()
    // 	{
    // 		std::cout << "Debug: " << "connection request " << std::endl;
    // 	}

    void onGTPMessage(const int& socketFd, const std::string& command)
    {
        try
        {
            m_userApp.onGTPMessage(boost::lexical_cast<std::string>(socketFd), command);
        }
        catch (boost::bad_lexical_cast & e)
        {
            std::cout << "error: " << "decode socket fd "<< socketFd << "failed: " << e.what() << std::endl;
        }
    }

    UserApp& m_userApp;
};
} // namespace applications
