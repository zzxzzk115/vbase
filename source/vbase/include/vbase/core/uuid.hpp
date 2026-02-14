#pragma once

#include "vbase/core/hash.hpp"
#include "vbase/core/string_view.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <random>
#include <string>

namespace vbase
{
    struct UUID
    {
        uint64_t hi {0};
        uint64_t lo {0};

        constexpr bool     valid() const noexcept { return (hi | lo) != 0; }
        constexpr explicit operator bool() const noexcept { return valid(); }

        friend constexpr bool operator==(UUID a, UUID b) noexcept { return a.hi == b.hi && a.lo == b.lo; }

        friend constexpr bool operator!=(UUID a, UUID b) noexcept { return !(a == b); }
    };

    inline std::string to_string(UUID u)
    {
        static constexpr char hex[] = "0123456789abcdef";
        std::string           s;
        s.resize(32);

        auto write64 = [&](uint64_t v, int offset) {
            for (int i = 0; i < 16; ++i)
            {
                const int shift = (15 - i) * 4;
                s[offset + i]   = hex[(v >> shift) & 0xF];
            }
        };

        write64(u.hi, 0);
        write64(u.lo, 16);
        return s;
    }

    inline bool try_parse_uuid(const char* str, UUID& out)
    {
        if (!str)
            return false;

        const size_t n = std::strlen(str);
        if (n != 32)
            return false;

        auto hexval = [](char c) -> int {
            if (c >= '0' && c <= '9')
                return c - '0';
            if (c >= 'a' && c <= 'f')
                return 10 + (c - 'a');
            if (c >= 'A' && c <= 'F')
                return 10 + (c - 'A');
            return -1;
        };

        uint64_t hi = 0, lo = 0;

        for (size_t i = 0; i < 16; ++i)
        {
            int a = hexval(str[i * 2 + 0]);
            int b = hexval(str[i * 2 + 1]);
            if (a < 0 || b < 0)
                return false;

            uint8_t byte = static_cast<uint8_t>((a << 4) | b);

            if (i < 8)
                hi = (hi << 8) | byte;
            else
                lo = (lo << 8) | byte;
        }

        out.hi = hi;
        out.lo = lo;
        return true;
    }

#if __cplusplus >= 201703L

    inline UUID uuid_from_bytes(ConstByteSpan bytes)
    {
        const uint64_t h1 = hash64(bytes);

        // Mix with constant seed to decorrelate hi/lo
        uint64_t                 seed = 0x9e3779b97f4a7c15ull;
        std::array<std::byte, 8> s {};
        std::memcpy(s.data(), &seed, 8);

        uint64_t h2 = hash64(ConstByteSpan {s.data(), s.size()});
        h2 ^= (h1 + 0x85ebca6bull + (h2 << 6) + (h2 >> 2));

        return UUID {h1, h2};
    }

    inline UUID uuid_from_string_key(StringView key)
    {
        ConstByteSpan bytes {reinterpret_cast<const std::byte*>(key.data()), key.size()};
        return uuid_from_bytes(bytes);
    }

#endif

    inline UUID uuid_random()
    {
        static thread_local std::mt19937_64 rng {std::random_device {}()};

        UUID id;
        id.hi = rng();
        id.lo = rng();

        // Avoid accidental nil UUID
        if (!id.valid())
        {
            id.lo = 1;
        }

        return id;
    }
} // namespace vbase

namespace std
{
    template<>
    struct hash<vbase::UUID>
    {
        size_t operator()(const vbase::UUID& id) const noexcept
        {
            return std::hash<std::string> {}(vbase::to_string(id));
        }
    };
} // namespace std