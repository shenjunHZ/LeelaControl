//
// Created by junshen on 1/26/19.
//
#ifndef WIN_ENV_RUN
#include <unistd.h>
#else
#include <windows.h>
#include <ciso646>
#endif
#include <iostream>
#include <atomic>
#include "UserApp.hpp"
namespace
{
	std::atomic_bool keep_running(true);

    const std::string LEELA{"leela"};
    const std::string STOP_AI{"quit"};
}  // namespace

using namespace configurations;
namespace applications
{
	UserApp::UserApp(games::IManagementJob& managementJob)
		: m_managementJob{managementJob}
	{
        startProcess();
        m_socketToLeela.clear();
	}

	UserApp::~UserApp()
	{
		if(m_threadProcess.joinable())
		{
			m_threadProcess.join();
		}
	}

	void UserApp::onMessage(const configurations::messageData& data)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_messageDatas.push(data);

		std::cout << "Debug: " << "identity: "<< data.zmqIdentity 
            << " data message: " << data.message << std::endl;
		m_condition.notify_one();
	}

    void UserApp::onMessage(const configurations::types::TcpMessageEnvelope& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_messageDatas.push(data);
        m_condition.notify_one();
    }

    void UserApp::onGTPMessage(const std::string& id, const std::string& command, 
        configurations::types::RespCallback callback)
    {
        auto it = m_socketToLeela.find(id);
        if (it != m_socketToLeela.end())
        {
            const auto& leelaJob = m_managementJob.getJob(it->second);
            if (leelaJob)
            {
                if (std::string::npos != command.find(STOP_AI))
                {
                    m_managementJob.stopGameLeela(it->second, callback);
                    m_socketToLeela.erase(id);
                }
                else
                {
                    leelaJob->inputGTPCommand(id, command, callback);
                }
            }
            else
            {
                LOG_WARNING_MSG("Cannot find leela to process this command {}", command);
            }
        }
        else
        {
            LOG_WARNING_MSG("Have not start leela for this link.");
        }
    }

    void UserApp::onControlMessage(const std::string& id, const std::string& command,
        configurations::types::RespCallback callback)
    {
        const auto leela = command.find(LEELA);
        if (std::string::npos == leela)
        {
            LOG_WARNING_MSG("Should set with leela AI.");
            return;
        }
        std::string level = command.substr(leela + std::strlen(LEELA.c_str()) + 1);
        if (level.empty())
        {
            LOG_WARNING_MSG("Should set with leela level.");
            return;
        }
        if ( std::atoi(level.c_str()) < static_cast<int>(leelaStarLevel::STAR_LEVEL_1) or 
                std::atoi(level.c_str()) > static_cast<int>(leelaStarLevel::STAR_LEVEL_9 ) )
        {
            LOG_WARNING_MSG("Excede leela level: {}", std::atoi(level.c_str()));
            return;
        }
        auto it = m_socketToLeela.find(id);
        if (it != m_socketToLeela.end())
        {
            LOG_WARNING_MSG("This link id have started leela: {}", static_cast<int>(it->second));
            return;
        }
        for (const auto& leela : m_socketToLeela)
        {
            if ( std::atoi(level.c_str()) 
                == static_cast<int>(leela.second) )
            {
                LOG_DEBUG_MSG("Leela level: {}, have been started.", std::atoi(level.c_str()));
                m_socketToLeela[id] = leela.second;
                return;
            }
        }
        m_managementJob.createJobLeela(static_cast<leelaStarLevel>(std::atoi(level.c_str())));
        m_managementJob.startJobLeela(static_cast<leelaStarLevel>(std::atoi(level.c_str())), callback);
        m_socketToLeela[id] = static_cast<leelaStarLevel>(std::atoi(level.c_str()));
    }

    void UserApp::processMessage()
	{
		while (keep_running)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_condition.wait(lock, [this]()
			{
				return !m_messageDatas.empty();
				// to do process message
				// create endponit entiry according identity
				// get leela job
				// send gtm message
				// or protocol message
			});

            Handler handler{*this};
            boost::apply_visitor(handler, m_messageDatas.front());

			m_messageDatas.pop();
			if(m_messageDatas.empty())
			{
#ifdef WIN_ENV_RUN
                Sleep(10);
#else
                usleep(1000 * 10);
#endif
			}
		}
	}

	void UserApp::startProcess()
	{
		boost::function<void()> fun = boost::bind(&UserApp::processMessage, this);
		m_threadProcess = boost::thread(fun);
	}
} // namespace applications
