#pragma once

namespace configurations
{
    enum class errorInfo
    {
        ERROR_NO_LEELAZ = 1,
        ERROR_PROCESS_DIED,
        ERROR_LAUNCH,
        ERROR_PROCESS,
        ERROR_GTP
    };

    enum class leelaStarLevel
    {
        STAR_LEVEL_1 = 1,
        STAR_LEVEL_2,
        STAR_LEVEL_3,
        STAR_LEVEL_4,
        STAR_LEVEL_5,
        STAR_LEVEL_6,
        STAR_LEVEL_7,
        STAR_LEVEL_8,
        STAR_LEVEL_9
    };

    struct tcpServiceParams
    {
        std::string tcpServiceIp;
        int tcpServicePort;
        int tcpBufferSize;
        int tcpKeepAlive;
        int tcpKeepIdle;
        int tcpKeepInterval;
        int tcpKeepCount;
    };

    struct AppAddresses
    {
        std::string serviceAddress;
        std::string tcpServiceAddress;
        tcpServiceParams tcpParams;
    };

    struct messageData
    {
        std::string zmqIdentity;
        std::string message;
    };

    constexpr auto serviceSocketMonitor = "inproc://serviceSocketMonitor";

    struct comConfigParams
    {
        int baudRate;
        int dataBits;
        int stopBits;
        std::string dataPaity;
    };

namespace keys
{
    const std::string serviceAddress = "addresses" ".serviceAddress";
    const std::string tcpServiceIp = "tcp" ".tcpServiceIp";
    const std::string tcpServicePort = "tcp" ".tcpServicePort";
    const std::string tcpBufferSize = "tcp" ".tcpBufferSize";
    const std::string tcpKeepAlive = "tcp" ".tcpKeepAlive";
    const std::string tcpKeepCount = "tcp" ".tcpKeepCount";
    const std::string tcpKeepIdle = "tcp" ".tcpKeepIdle";
    const std::string tcpKeepInterval = "tcp" ".tcpKeepInterval";
} // namespace keys
} // namespace configurations
