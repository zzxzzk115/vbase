#pragma once

#include "vbase/core/assert.hpp"
#include "vbase/core/noncopyable.hpp"

namespace vbase
{
    template<class T>
    class Singleton : NonCopyable
    {
    public:
        static T& instance()
        {
            VBASE_ASSERT(s_Instance != nullptr);
            return *s_Instance;
        }

    protected:
        Singleton()
        {
            VBASE_ASSERT(s_Instance == nullptr);
            s_Instance = static_cast<T*>(this);
        }

        ~Singleton()
        {
            VBASE_ASSERT(s_Instance == this);
            s_Instance = nullptr;
        }

    private:
        static inline T* s_Instance = nullptr;
    };
} // namespace vbase
