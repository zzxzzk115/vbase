#pragma once

#include <cstdint>

namespace vbase
{
    enum class Endian : uint8_t
    {
        eLittle,
        eBig
    };

    constexpr Endian native_endian() noexcept
    {
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        return Endian::eBig;
#else
        return Endian::eLittle;
#endif
    }

    template<class T>
    inline T bswap(T v) noexcept;

    template<>
    inline uint16_t bswap<uint16_t>(uint16_t v) noexcept
    {
        return static_cast<uint16_t>((v >> 8) | (v << 8));
    }

    template<>
    inline uint32_t bswap<uint32_t>(uint32_t v) noexcept
    {
        return ((v & 0x000000FFu) << 24) | ((v & 0x0000FF00u) << 8) | ((v & 0x00FF0000u) >> 8) |
               ((v & 0xFF000000u) >> 24);
    }

    template<>
    inline uint64_t bswap<uint64_t>(uint64_t v) noexcept
    {
        return ((v & 0x00000000000000FFull) << 56) | ((v & 0x000000000000FF00ull) << 40) |
               ((v & 0x0000000000FF0000ull) << 24) | ((v & 0x00000000FF000000ull) << 8) |
               ((v & 0x000000FF00000000ull) >> 8) | ((v & 0x0000FF0000000000ull) >> 24) |
               ((v & 0x00FF000000000000ull) >> 40) | ((v & 0xFF00000000000000ull) >> 56);
    }

    template<class T>
    inline T to_little_endian(T v) noexcept
    {
        if (native_endian() == Endian::eLittle)
            return v;
        if constexpr (sizeof(T) == 2)
            return static_cast<T>(bswap<uint16_t>(static_cast<uint16_t>(v)));
        if constexpr (sizeof(T) == 4)
            return static_cast<T>(bswap<uint32_t>(static_cast<uint32_t>(v)));
        if constexpr (sizeof(T) == 8)
            return static_cast<T>(bswap<uint64_t>(static_cast<uint64_t>(v)));
        return v;
    }

    template<class T>
    inline T from_little_endian(T v) noexcept
    {
        return to_little_endian(v);
    }
} // namespace vbase
