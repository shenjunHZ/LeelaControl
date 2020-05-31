#pragma once
#include "IManagementUser.hpp"
#include "Protocol/qsPtl.hpp"

namespace
{
    constexpr int bufferSize{ 128 * 1024 };
}

namespace applications
{
    class ManagementUser : public IManagementUser
    {
    public:
        ManagementUser();
        void onTcpMessage(const configurations::types::TcpMessageEnvelope& data, const int socketFd);

    private:
        std::map<std::string, int> m_;
        std::vector<std::uint8_t> m_receiveBuffer{ bufferSize };
        CQsPtl m_cQsPtl;
    };
} // namespace applications