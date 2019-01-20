#pragma once
#include "Job.hpp"

namespace Game
{
    class IManagementJob
    {
    public:
        virtual ~IManagementJob() = default;
        virtual void createJobLeela(const leelaStarLevel& level) = 0;
        virtual const IJob& getJob(const leelaStarLevel& level) = 0;
    };
}