#pragma once

#include "vbase/core/assert.hpp"
#include "vbase/core/hash.hpp"
#include "vbase/service/service_concept.hpp"

#include <cstdint>
#include <unordered_map>

namespace vbase
{
#define SERVICE_REGISTRY_CONCEPT_CHECK(T) static_assert(ServiceType<T>, "T must satisfy ServiceType concept")
#define SERVICE_REGISTER(T) \
public: \
    static const char* serviceName() { return #T; }

    class ServiceRegistry
    {
    public:
        template<ServiceType T>
        void provide(T* instance)
        {
            SERVICE_REGISTRY_CONCEPT_CHECK(T);
            VBASE_ASSERT(instance != nullptr);

            const uint64_t key = hashString(T::serviceName());

            auto [it, inserted] = m_Map.emplace(key, instance);

            VBASE_ASSERT(inserted && "Service already registered");
        }

        template<ServiceType T>
        T* tryGet() const
        {
            SERVICE_REGISTRY_CONCEPT_CHECK(T);
            const uint64_t key = hashString(T::serviceName());

            auto it = m_Map.find(key);

            if (it == m_Map.end())
                return nullptr;

            return static_cast<T*>(it->second);
        }

        template<ServiceType T>
        T& require() const
        {
            T* p = tryGet<T>();

            VBASE_ASSERT(p != nullptr);

            return *p;
        }

    private:
        std::unordered_map<uint64_t, void*> m_Map;
    };
} // namespace vbase
