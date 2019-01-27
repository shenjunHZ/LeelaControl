#pragma once
#include <QtCore/QObject>

namespace Game
{
    class ILeelaCommand 
    {
    public:
        virtual ~ILeelaCommand() = default;
    };
}