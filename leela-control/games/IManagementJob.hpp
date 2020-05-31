#pragma once
#include <boost/optional.hpp>
#include "Job.hpp"
#include "configurations/types/TcpMessageEnvelope.hpp"

namespace games
{
    class IManagementJob
    {
    public:
        virtual ~IManagementJob() = default;
        virtual void createJobLeela(const leelaStarLevel&) = 0;
        virtual void startJobLeela(const leelaStarLevel&, configurations::types::RespCallback) = 0;
        virtual void stopGameLeela(const leelaStarLevel&) = 0;
        virtual  boost::optional<IJob&> getJob(const leelaStarLevel&) = 0;
    };
} // namespace games