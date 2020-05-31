#include "ManagementUser.hpp"

namespace applications
{

    ManagementUser::ManagementUser()
    {
        m_cQsPtl.SetParamter(true, "000000", m_receiveBuffer.data(), bufferSize); // use common qs protocol
    }

    void ManagementUser::onTcpMessage(const configurations::types::TcpMessageEnvelope& data, const int socketFd)
    {

    }

    void ManagementUser::onTcpMessage(const std::vector<std::uint8_t>& receiveData, const int& recvSize)
    {
        SPtlValue sPtlValue;
        m_receiveBuffer.clear();
        if ( CMD_DECODE_OK == m_cQsPtl.decode(receiveData.data(), receiveData, &sPtlValue) )
        {
            std::string deviceID = "";
            for (int i = 0; i < CMD_ID_LEN; i++)
            {
                deviceID += std::to_string(sPtlValue.id[i]);
            }
        }
    }

} // namespace applications