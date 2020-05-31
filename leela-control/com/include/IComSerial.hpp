#pragma once
#include <string>

namespace com
{
class IComSerial
{
public:
    virtual int openCom() = 0;
    virtual int closeCom() = 0;
    virtual int writeCom(const std::string& data) = 0;
    virtual int readCom(std::string& data) = 0;
};
} // namespace com