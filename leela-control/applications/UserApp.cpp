//
// Created by junshen on 1/26/19.
//
#ifndef WIN_ENV_RUN
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <iostream>
#include <atomic>
#include "UserApp.hpp"
namespace
{
	std::atomic_bool keep_running(true);
    using resultCallback = std::function<void(const std::string&)>;
}

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

    void UserApp::onMessage(const sockets::types::TcpMessageEnvelope& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_messageDatas.push(data);

        std::cout << "Debug: " << "socket fd: " << data.socketFd 
            << " data message: " << data.payload << std::endl;
        m_condition.notify_one();
    }

    void UserApp::onGTPMessage(const std::string& id, const std::string& command)
    {
        auto it = m_socketToLeela.find(id);
        if (it != m_socketToLeela.end())
        {
            auto& leelaJob = m_managementJob.getJob(it->second);
            resultCallback callback;
            leelaJob.inputGTPCommand(id, command, callback);
        }
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
