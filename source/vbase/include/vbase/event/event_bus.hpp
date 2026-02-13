#pragma once

#include "vbase/core/noncopyable.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace vbase
{
    class Subscription : NonCopyable
    {
    public:
        Subscription() = default;

        Subscription(std::function<void(uint64_t)> unsubscribeFn, uint64_t id) :
            m_Unsubscribe(std::move(unsubscribeFn)), m_Id(id)
        {}

        Subscription(Subscription&& o) noexcept : m_Unsubscribe(std::move(o.m_Unsubscribe)), m_Id(o.m_Id)
        {
            o.m_Id = 0;
        }

        Subscription& operator=(Subscription&& o) noexcept
        {
            if (this == &o)
                return *this;
            reset();
            m_Unsubscribe = std::move(o.m_Unsubscribe);
            m_Id          = o.m_Id;
            o.m_Id        = 0;
            return *this;
        }

        ~Subscription() { reset(); }

        void reset()
        {
            if (m_Id != 0 && m_Unsubscribe)
            {
                m_Unsubscribe(m_Id);
                m_Id = 0;
            }
        }

        explicit operator bool() const noexcept { return m_Id != 0; }

    private:
        std::function<void(uint64_t)> m_Unsubscribe;
        uint64_t                      m_Id {0};
    };

    class EventBus : NonCopyable
    {
    public:
        template<class E>
        using Fn = std::function<void(const E&)>;

        template<class E>
        Subscription subscribe(Fn<E> fn)
        {
            auto&          chan = channel<E>();
            const uint64_t id   = ++chan.nextId;
            chan.handlers.push_back({id, [f = std::move(fn)](const void* p) { f(*reinterpret_cast<const E*>(p)); }});
            return Subscription([this, tid = std::type_index(typeid(E))](uint64_t cid) { this->unsubscribe(tid, cid); },
                                id);
        }

        template<class E>
        void publish(const E& e)
        {
            auto it = m_Channels.find(std::type_index(typeid(E)));
            if (it == m_Channels.end())
                return;

            auto handlers = it->second.handlers;
            for (auto& h : handlers)
                h.fn(&e);
        }

        void clear() { m_Channels.clear(); }

    private:
        struct Handler
        {
            uint64_t                         id;
            std::function<void(const void*)> fn;
        };
        struct Channel
        {
            uint64_t             nextId {0};
            std::vector<Handler> handlers;
        };

        template<class E>
        Channel& channel()
        {
            return m_Channels[std::type_index(typeid(E))];
        }

        void unsubscribe(std::type_index tid, uint64_t id)
        {
            auto it = m_Channels.find(tid);
            if (it == m_Channels.end())
                return;

            auto& vec = it->second.handlers;
            vec.erase(std::remove_if(vec.begin(), vec.end(), [&](const Handler& h) { return h.id == id; }), vec.end());
        }

        std::unordered_map<std::type_index, Channel> m_Channels;
    };
} // namespace vbase
