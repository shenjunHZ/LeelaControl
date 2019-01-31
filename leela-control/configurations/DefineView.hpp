#pragma once

namespace configurations
{
    enum class errorInfo
    {
        ERROR_NO_LEELAZ = 1,
        ERROR_LAUNCH,
        ERROR_PROCESS,
        ERROR_GTP
    };

    enum class leelaStarLevel
    {
        STAR_LEVEL_1 = 0,
        STAR_LEVEL_2,
        STAR_LEVEL_3,
        STAR_LEVEL_4,
        STAR_LEVEL_5,
        STAR_LEVEL_6,
        STAR_LEVEL_7,
        STAR_LEVEL_8,
        STAR_LEVEL_9
    };

    struct AppAddresses
    {
        std::string serviceAddress;
    };

    struct messageData
    {
        std::string zmqIdentity;
        std::string message;
    };

namespace keys
{
    const std::string serviceAddress = "addresses" ".serviceAddress";
} // namespace keys
} // namespace configurations
