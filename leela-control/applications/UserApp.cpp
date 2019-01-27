//
// Created by junshen on 1/26/19.
//
#include <iostream>
#include "UserApp.hpp"
namespace
{
	std::atomic_bool keep_running(true);
}

namespace applications
{
	UserApp::UserApp(games::IManagementJob& managementJob)
		: m_managementJob{managementJob}
	{

	}

	UserApp::~UserApp()
	{
		if(m_threadProcess.joinable())
		{
			m_threadProcess.join();
		}
	}

//	void UserApp::handleConnectionRequest()
//	{
//		std::cout << "Debug: " << "connection request " << std::endl;
//	}
//
//	void UserApp::onProtocolMessage()
//	{
//		handleConnectionRequest();
//		// to do others
//	}

	void UserApp::onMessage(const configurations::messageData& data)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_messageDatas.push(data);

		std::cout << "Debug: " << "identity: "<< data.zmqIdentity << std::endl;
		std::cout << "Debug: " << "data message: " << data.message << std::endl;
		m_condition.notify_one();
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
			configurations::messageData data = m_messageDatas.front();
			m_messageDatas.pop();
			if(data.message.empty())
			{
				usleep(1000 * 10);
			}
		}
	}

	void UserApp::startProcess()
	{
		boost::function<void()> fun = boost::bind(&UserApp::processMessage, this);
		m_threadProcess = boost::thread(fun);
	}
} // namespace applications