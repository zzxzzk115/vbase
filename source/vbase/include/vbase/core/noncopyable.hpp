#pragma once
namespace vbase
{
    struct NonCopyable
    {
        NonCopyable()                              = default;
        NonCopyable(const NonCopyable&)            = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&)                 = default;
        NonCopyable& operator=(NonCopyable&&)      = default;
        ~NonCopyable()                             = default;
    };
} // namespace vbase
