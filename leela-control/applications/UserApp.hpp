//
// Created by junshen on 1/26/19.
//

#pragma once
#include "configurations/AppConfiguration.hpp"
#include "configurations/DefineView.hpp"
#include "games/ManagementJob.hpp"

namespace applications
{
class UserApp
{
public:
	UserApp(games::IManagementJob& managementJob);
	~UserApp();
	void onMessage(const configurations::messageData& data);

private:
	void startProcess();
	void processMessage();

private:
	std::queue<configurations::messageData> m_messageDatas;
	games::IManagementJob& m_managementJob;
	mutable std::mutex m_mutex;
	std::condition_variable m_condition;
	boost::thread m_threadProcess;
};
} // namespace applications
