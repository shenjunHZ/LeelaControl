#pragma once

namespace sockets
{
    class ISocketObserver
    {
    public:
        virtual void socketMonitor() = 0;
        virtual ~ISocketObserver() = default;
    };
} // namespace sockets