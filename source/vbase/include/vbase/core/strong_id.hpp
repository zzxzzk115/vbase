#pragma once
#include <cstdint>

namespace vbase
{
    template<class Tag, class T = uint32_t>
    struct StrongID
    {
        T value {0};

        constexpr bool     valid() const noexcept { return value != 0; }
        constexpr explicit operator bool() const noexcept { return valid(); }

        friend constexpr bool operator==(StrongID a, StrongID b) noexcept { return a.value == b.value; }
        friend constexpr bool operator!=(StrongID a, StrongID b) noexcept { return a.value != b.value; }
        friend constexpr bool operator<(StrongID a, StrongID b) noexcept { return a.value < b.value; }
    };
} // namespace vbase
