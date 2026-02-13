#pragma once

#include <cstddef>
#include <type_traits>

namespace vbase
{
    template<class T>
    class Span
    {
    public:
        using element_type = T;
        using value_type   = std::remove_cv_t<T>;

        constexpr Span() noexcept : m_Ptr(nullptr), m_Len(0) {}
        constexpr Span(T* ptr, size_t len) noexcept : m_Ptr(ptr), m_Len(len) {}

        template<class U, class = std::enable_if_t<std::is_same_v<std::remove_cv_t<U>, value_type>>>
        constexpr Span(Span<U> other) noexcept : m_Ptr(other.data()), m_Len(other.size())
        {}

        constexpr T*     data() const noexcept { return m_Ptr; }
        constexpr size_t size() const noexcept { return m_Len; }
        constexpr bool   empty() const noexcept { return m_Len == 0; }

        constexpr T& operator[](size_t i) const noexcept { return m_Ptr[i]; }

        constexpr Span subspan(size_t offset, size_t count) const noexcept { return Span(m_Ptr + offset, count); }

        constexpr T* begin() const noexcept { return m_Ptr; }
        constexpr T* end() const noexcept { return m_Ptr + m_Len; }

    private:
        T*     m_Ptr;
        size_t m_Len;
    };

#if __cplusplus >= 201703L
    // std::byte exists from C++17
    using ByteSpan      = Span<std::byte>;
    using ConstByteSpan = Span<const std::byte>;
#endif
} // namespace vbase
