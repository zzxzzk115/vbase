#pragma once

#include "vbase/core/assert.hpp"

#include <typeindex>
#include <unordered_map>

namespace vbase
{
    class ServiceRegistry
    {
    public:
        template<class T>
        void provide(T* instance)
        {
            VBASE_ASSERT(instance != nullptr);
            m_Map[std::type_index(typeid(T))] = instance;
        }

        template<class T>
        T* tryGet() const
        {
            auto it = m_Map.find(std::type_index(typeid(T)));
            if (it == m_Map.end())
                return nullptr;
            return reinterpret_cast<T*>(it->second);
        }

        template<class T>
        T& require() const
        {
            T* p = tryGet<T>();
            VBASE_ASSERT(p != nullptr);
            return *p;
        }

    private:
        std::unordered_map<std::type_index, void*> m_Map;
    };
} // namespace vbase
