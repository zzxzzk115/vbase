#pragma once
namespace vbase
{
    struct IModule
    {
        virtual ~IModule()               = default;
        virtual const char* name() const = 0;
        virtual bool        init()       = 0;
        virtual void        shutdown()   = 0;
    };
} // namespace vbase
