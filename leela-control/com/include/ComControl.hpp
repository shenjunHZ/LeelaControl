#pragma once
#include <memory>
#include <thread>
#include "IComControl.hpp"
#include "logger/Logger.hpp"

namespace applications
{
    struct ServiceContext;
}
namespace applications
{
    class UserApp;
}

namespace com
{
class IComSerial;

class ComControl : public IComControl
{
public:
    ComControl(applications::ServiceContext& serviceContext, applications::UserApp& userApp);
    ~ComControl();
    void startPortCom();

private:
    void startPortComCommand(int fd);

private:
    spdlog::logger&                  m_logger;
    std::unique_ptr<IComSerial>      m_comSerial;
    std::thread                      m_threadCom;
    applications::ServiceContext&    m_serviceContext;
    applications::UserApp&           m_userApp;
};
}  // namespace com