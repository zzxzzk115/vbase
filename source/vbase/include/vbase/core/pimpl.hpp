#pragma once

#include <memory>
#include <utility>

namespace vbase
{
    template<class T>
    class Pimpl
    {
    public:
        Pimpl() = default;

        template<class... Args>
        explicit Pimpl(std::in_place_t, Args&&... args) : m_Impl(std::make_unique<T>(std::forward<Args>(args)...))
        {}

        T*       get() noexcept { return m_Impl.get(); }
        const T* get() const noexcept { return m_Impl.get(); }

        T&       operator*() noexcept { return *m_Impl; }
        const T& operator*() const noexcept { return *m_Impl; }
        T*       operator->() noexcept { return m_Impl.get(); }
        const T* operator->() const noexcept { return m_Impl.get(); }

        explicit operator bool() const noexcept { return static_cast<bool>(m_Impl); }

    private:
        std::unique_ptr<T> m_Impl;
    };
} // namespace vbase
