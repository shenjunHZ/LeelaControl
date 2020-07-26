//
// Created by junshen on 1/26/19.
//
#pragma once
#include "configurations/AppConfiguration.hpp"
#include "configurations/DefineView.hpp"
#include "games/ManagementJob.hpp"
#include "configurations/types/TcpMessageEnvelope.hpp"
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>

namespace
{
    const std::string START_AI{"start_ai"};
}
namespace applications
{
using variant = boost::variant<configurations::messageData, configurations::types::TcpMessageEnvelope>;

class UserApp
{
public:
	UserApp(games::IManagementJob& managementJob);
	~UserApp();
	void onMessage(const configurations::messageData& data);
    void onMessage(const configurations::types::TcpMessageEnvelope& data);

    void onGTPMessage(const std::string& id, const std::string& command, 
        configurations::types::RespCallback callback);
    void onControlMessage(const std::string& id, const std::string& command,
        configurations::types::RespCallback callback);
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
   
    }
    void operator () (configurations::types::TcpMessageEnvelope messageEnvolope)
    {
        //LOG_DEBUG_MSG("Process tcp socket message.");
        if ( std::string::npos != messageEnvolope.payload.find(START_AI) )
        {
            onControlMessage(messageEnvolope.socketFd,
                messageEnvolope.payload, messageEnvolope.callback);
            return;
        }
        
        onGTPMessage(messageEnvolope.socketFd, 
            messageEnvolope.payload, messageEnvolope.callback); // to do decode payload to command and chassBoardID
    }

    void onGTPMessage(const int& socketFd, const std::string& command, 
        configurations::types::RespCallback callback)
    {
        try
        {
            m_userApp.onGTPMessage(boost::lexical_cast<std::string>(socketFd), 
                command, callback);
        }
        catch (boost::bad_lexical_cast & e)
        {
            LOG_ERROR_MSG("Decode socket fd {} failed: {}", socketFd, e.what());
        }
    }

    void onControlMessage(const int& socketFd, const std::string& command,
        configurations::types::RespCallback callback)
    {
        try
        {
            m_userApp.onControlMessage(boost::lexical_cast<std::string>(socketFd), command, callback);
        }
        catch (boost::bad_lexical_cast & e)
        {
            LOG_ERROR_MSG("Decode socket fd {} failed: {}", socketFd, e.what());
        }
    }

    UserApp& m_userApp;
};
} // namespace applications
