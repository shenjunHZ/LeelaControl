//
// Created by root on 1/20/19.
//

#pragma once

#include "configurations/DefineView.hpp"

namespace games
{
    using namespace configurations;
    using resultCallback = std::function<void(const std::string&)>;

    class IJob
    {
    public:
        virtual ~IJob() = default;
        virtual void createJobParameter(const leelaStarLevel& level) = 0;
        virtual void startGameLeela(const resultCallback&) = 0;
        virtual void inputGTPCommand(const std::string& chassBoardID, const std::string& command,
            const resultCallback&) = 0;
        virtual void stopGameLeela(const resultCallback&) = 0;
    };
};
