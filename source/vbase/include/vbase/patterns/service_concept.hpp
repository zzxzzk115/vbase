#pragma once

#include <concepts>
#include <string_view>

namespace vbase
{
    template<typename T>
    concept ServiceType = requires {
        { T::serviceName() } -> std::convertible_to<std::string_view>;
    };
} // namespace vbase
