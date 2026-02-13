#pragma once

#include "vbase/core/span.hpp"

#include <cstddef>
#include <cstdint>

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
#endif
} // namespace vbase
