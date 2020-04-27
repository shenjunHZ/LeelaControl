//
// Created by junshen on 1/24/19.
//

#pragma once
#include <functional>
#ifdef WIN_ENV_RUN
#include <winsock.h>
#endif // WIN_ENV_RUN

namespace sockets
{
namespace types
{
#ifndef WIN_ENV_RUN
        using FileDescriptor = int;
#else
        using FileDescriptor = SOCKET;
#endif
} // namespace types

class DescriptorPoller
{
public:
    virtual void registerFileDescriptor(types::FileDescriptor, std::function<void()>) = 0;
    virtual void unregisterFileDescriptor(types::FileDescriptor) = 0;
    virtual ~DescriptorPoller() = default;
};
} // namespace sockets
