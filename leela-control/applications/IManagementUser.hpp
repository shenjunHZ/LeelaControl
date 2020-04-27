#pragma once
#include "IUser.hpp"

namespace applications
{
    class IManagementUser
    {
    public:
        ~IManagementUser() = default;
        virtual void createUser(const std::string& chassBoardID) = 0;
        virtual void deleteUser(const std::string& chassBoardID) = 0;
        virtual const IUser& getUser(const std::string& chassBoardID) = 0;
    };
} // namespace applications