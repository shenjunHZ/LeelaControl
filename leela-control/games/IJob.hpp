//
// Created by root on 1/20/19.
//

#pragma once

#include "configurations/DefineView.hpp"

namespace games
{
    using namespace configurations;

    class IJob
    {
    public:
        virtual ~IJob() = default;
        virtual void createJobParameter(const leelaStarLevel& level) = 0;
        virtual void startGameLeela() = 0;
    };
};
