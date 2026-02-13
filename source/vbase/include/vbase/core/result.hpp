#pragma once

#include "vbase/core/assert.hpp"

#include <utility>

namespace vbase
{
    template<class T, class E>
    class Result
    {
    public:
        using value_type = T;
        using error_type = E;

        static Result ok(T v) { return Result(std::move(v)); }
        static Result err(E e) { return Result(std::move(e), TagErr {}); }

        explicit operator bool() const noexcept { return m_Ok; }

        T& value() &
        {
            VBASE_ASSERT(m_Ok);
            return m_V;
        }
        const T& value() const&
        {
            VBASE_ASSERT(m_Ok);
            return m_V;
        }
        T&& value() &&
        {
            VBASE_ASSERT(m_Ok);
            return std::move(m_V);
        }

        E& error() &
        {
            VBASE_ASSERT(!m_Ok);
            return m_E;
        }
        const E& error() const&
        {
            VBASE_ASSERT(!m_Ok);
            return m_E;
        }
        E&& error() &&
        {
            VBASE_ASSERT(!m_Ok);
            return std::move(m_E);
        }

    private:
        struct TagErr
        {};

        explicit Result(T v) : m_Ok(true), m_V(std::move(v)), m_E() {}
        explicit Result(E e, TagErr) : m_Ok(false), m_V(), m_E(std::move(e)) {}

        bool m_Ok {false};
        T    m_V {};
        E    m_E {};
    };

    template<class E>
    class Result<void, E>
    {
    public:
        static Result ok() { return Result(true, E {}); }
        static Result err(E e) { return Result(false, std::move(e)); }

        explicit operator bool() const noexcept { return m_Ok; }

        void value() const noexcept { VBASE_ASSERT(m_Ok); }

        E& error() &
        {
            VBASE_ASSERT(!m_Ok);
            return m_E;
        }
        const E& error() const&
        {
            VBASE_ASSERT(!m_Ok);
            return m_E;
        }

    private:
        Result(bool ok, E e) : m_Ok(ok), m_E(std::move(e)) {}
        bool m_Ok {false};
        E    m_E {};
    };
} // namespace vbase
