#pragma once
#include "IComSerial.hpp"

namespace com
{
class IComControl
{
public:
    virtual void startPortCom() = 0;
    virtual ~IComControl() = default;
};
}  // namespace com