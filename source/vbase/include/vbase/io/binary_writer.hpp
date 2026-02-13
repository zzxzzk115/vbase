#pragma once

#include "vbase/core/span.hpp"
#include "vbase/io/endian.hpp"

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>

namespace vbase
{
#if __cplusplus >= 201703L
    class BinaryWriter
    {
    public:
        BinaryWriter() = default;

        const std::vector<std::byte>& bytes() const noexcept { return m_Data; }
        std::vector<std::byte>&       bytes() noexcept { return m_Data; }

        size_t size() const noexcept { return m_Data.size(); }

        void writeBytes(ConstByteSpan s)
        {
            const size_t old = m_Data.size();
            m_Data.resize(old + s.size());
            std::memcpy(m_Data.data() + old, s.data(), s.size());
        }

        template<class T>
        void write(T v)
        {
            if constexpr (std::is_integral_v<T> && sizeof(T) <= 8)
                v = to_little_endian(v);

            const size_t old = m_Data.size();
            m_Data.resize(old + sizeof(T));
            std::memcpy(m_Data.data() + old, &v, sizeof(T));
        }

        void writeBlob(const void* p, uint32_t n)
        {
            write<uint32_t>(n);
            const size_t old = m_Data.size();
            m_Data.resize(old + n);
            std::memcpy(m_Data.data() + old, p, n);
        }

    private:
        std::vector<std::byte> m_Data;
    };
#endif
} // namespace vbase
