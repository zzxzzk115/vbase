#pragma once

#include "vbase/core/noncopyable.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

namespace vbase
{
    class Connection : NonCopyable
    {
    public:
        Connection() = default;

        Connection(std::function<void(uint64_t)> disconnectFn, uint64_t id) :
            m_Disconnect(std::move(disconnectFn)), m_Id(id)
        {}

        Connection(Connection&& o) noexcept : m_Disconnect(std::move(o.m_Disconnect)), m_Id(o.m_Id) { o.m_Id = 0; }

        Connection& operator=(Connection&& o) noexcept
        {
            if (this == &o)
                return *this;
            disconnect();
            m_Disconnect = std::move(o.m_Disconnect);
            m_Id         = o.m_Id;
            o.m_Id       = 0;
            return *this;
        }

        ~Connection() { disconnect(); }

        void disconnect()
        {
            if (m_Id != 0 && m_Disconnect)
            {
                m_Disconnect(m_Id);
                m_Id = 0;
            }
        }

        explicit operator bool() const noexcept { return m_Id != 0; }

    private:
        std::function<void(uint64_t)> m_Disconnect;
        uint64_t                      m_Id {0};
    };

    template<class... Args>
    class Signal
    {
    public:
        using Fn = std::function<void(Args...)>;

        Connection connect(Fn fn)
        {
            const uint64_t id = ++m_NextId;
            m_Slots.push_back({id, std::move(fn)});
            return Connection([this](uint64_t cid) { this->disconnect(cid); }, id);
        }

        void emit(Args... args)
        {
            auto slots = m_Slots; // tolerate modifications
            for (auto& s : slots)
            {
                if (s.fn)
                    s.fn(args...);
            }
        }

        void clear() { m_Slots.clear(); }

    private:
        struct Slot
        {
            uint64_t id;
            Fn       fn;
        };

        void disconnect(uint64_t id)
        {
            m_Slots.erase(std::remove_if(m_Slots.begin(), m_Slots.end(), [&](const Slot& s) { return s.id == id; }),
                          m_Slots.end());
        }

        uint64_t          m_NextId {0};
        std::vector<Slot> m_Slots;
    };
} // namespace vbase
