#pragma once

#include "vbase/core/span.hpp"

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace vbase
{
#if __cplusplus >= 201703L
    inline uint64_t hash64(ConstByteSpan bytes) noexcept
    {
        constexpr uint64_t FNV_OFFSET = 1469598103934665603ull;
        constexpr uint64_t FNV_PRIME  = 1099511628211ull;

        uint64_t h = FNV_OFFSET;
        for (auto byte : bytes)
        {
            h ^= static_cast<uint64_t>(byte);
            h *= FNV_PRIME;
        }
        return h;
    }

    // runtime version
    inline uint64_t hashString(std::string_view str) noexcept
    {
        return hash64(ConstByteSpan(reinterpret_cast<const std::byte*>(str.data()), str.size()));
    }
#endif

    // compile-time version (C++17 constexpr)
    constexpr uint64_t hashStringConst(const char* str, size_t len) noexcept
    {
        constexpr uint64_t FNV_OFFSET = 1469598103934665603ull;
        constexpr uint64_t FNV_PRIME  = 1099511628211ull;

        uint64_t h = FNV_OFFSET;

        for (size_t i = 0; i < len; ++i)
        {
            h ^= static_cast<uint64_t>(str[i]);
            h *= FNV_PRIME;
        }

        return h;
    }

    // helper for string literal
    template<size_t N>
    constexpr uint64_t hashLiteral(const char (&str)[N]) noexcept
    {
        return hashStringConst(str, N - 1);
    }
} // namespace vbase
