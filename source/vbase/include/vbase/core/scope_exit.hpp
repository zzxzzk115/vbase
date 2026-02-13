#pragma once

#include <utility>

namespace vbase
{
    template<class F>
    class ScopeExit
    {
    public:
        explicit ScopeExit(F&& f) noexcept : m_F(std::forward<F>(f)), m_Active(true) {}
        ScopeExit(ScopeExit&& o) noexcept : m_F(std::move(o.m_F)), m_Active(o.m_Active) { o.m_Active = false; }

        ScopeExit(const ScopeExit&)            = delete;
        ScopeExit& operator=(const ScopeExit&) = delete;
        ScopeExit& operator=(ScopeExit&&)      = delete;

        ~ScopeExit() noexcept
        {
            if (m_Active)
                m_F();
        }

        void release() noexcept { m_Active = false; }

    private:
        F    m_F;
        bool m_Active;
    };

    template<class F>
    inline ScopeExit<F> scope_exit(F&& f) noexcept
    {
        return ScopeExit<F>(std::forward<F>(f));
    }
} // namespace vbase
