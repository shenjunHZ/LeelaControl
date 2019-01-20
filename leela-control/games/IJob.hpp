//
// Created by root on 1/20/19.
//

#pragma once

#include "DefineView.hpp"

namespace Game
{
    using namespace Define;

    class IJob
    {
    public:
        virtual ~IJob() = default;
        virtual void createGameLeela(const leelaStarLevel& level) = 0;
        virtual void startGameLeela() = 0;
    };
};
