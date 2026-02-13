#pragma once

#include "vbase/core/assert.hpp"
#include "vbase/core/span.hpp"
#include "vbase/io/endian.hpp"

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>

namespace vbase
{
#if __cplusplus >= 201703L
    class BinaryReader
    {
    public:
        explicit BinaryReader(ConstByteSpan data) : m_Data(data), m_Off(0) {}

        size_t remaining() const noexcept { return m_Data.size() - m_Off; }
        size_t offset() const noexcept { return m_Off; }

        void seek(size_t off)
        {
            VBASE_ASSERT(off <= m_Data.size());
            m_Off = off;
        }

        template<class T>
        T read()
        {
            VBASE_ASSERT(remaining() >= sizeof(T));
            T v {};
            std::memcpy(&v, m_Data.data() + m_Off, sizeof(T));
            m_Off += sizeof(T);
            if constexpr (std::is_integral_v<T> && sizeof(T) <= 8)
                v = from_little_endian(v);
            return v;
        }

        ConstByteSpan readBytes(size_t n)
        {
            VBASE_ASSERT(remaining() >= n);
            auto s = m_Data.subspan(m_Off, n);
            m_Off += n;
            return s;
        }

        std::vector<uint8_t> readBlob()
        {
            uint32_t             n = read<uint32_t>();
            auto                 b = readBytes(n);
            std::vector<uint8_t> out(n);
            for (uint32_t i = 0; i < n; ++i)
                out[i] = static_cast<uint8_t>(b[i]);
            return out;
        }

    private:
        ConstByteSpan m_Data;
        size_t        m_Off;
    };
#endif
} // namespace vbase
