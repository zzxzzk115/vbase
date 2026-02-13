#pragma once

#include "vbase/core/macros.hpp"

#include <cstdio>
#include <cstdlib>

namespace vbase
{
    using AssertHandler = void(*)(const char* expr,
                                  const char* msg,
                                  const char* file,
                                  int         line) noexcept;

    VBASE_NOINLINE inline void default_assert_handler(const char* expr,
                                                      const char* msg,
                                                      const char* file,
                                                      int         line) noexcept
    {
        // Minimal stderr print (no logger dependency).
        std::fprintf(stderr, "[vbase] ASSERT FAILED: %s\n", expr ? expr : "(null)");
        if (msg) std::fprintf(stderr, "  msg: %s\n", msg);
        if (file) std::fprintf(stderr, "  at: %s:%d\n", file, line);
        std::fflush(stderr);
#if VBASE_DEBUG
        VBASE_DEBUGBREAK();
#endif
        std::abort();
    }

    inline AssertHandler& assert_handler_storage() noexcept
    {
        static AssertHandler h = &default_assert_handler;
        return h;
    }

    inline void set_assert_handler(AssertHandler handler) noexcept
    {
        assert_handler_storage() = handler ? handler : &default_assert_handler;
    }

    VBASE_NOINLINE inline void panic_impl(const char* expr,
                                          const char* msg,
                                          const char* file,
                                          int         line) noexcept
    {
        assert_handler_storage()(expr, msg, file, line);
        // If handler returns, still terminate.
        std::abort();
    }
} // namespace vbase

#if VBASE_ASSERT_LEVEL >= 2
  #define VBASE_ASSERT(expr) \
      do { if (!(expr)) ::vbase::panic_impl(#expr, nullptr, __FILE__, (int)__LINE__); } while (0)
#else
  #define VBASE_ASSERT(expr) do { (void)sizeof(expr); } while (0)
#endif

#if VBASE_ASSERT_LEVEL >= 2
  #define VBASE_VERIFY(expr) \
      do { if (!(expr)) ::vbase::panic_impl(#expr, nullptr, __FILE__, (int)__LINE__); } while (0)
#else
  #define VBASE_VERIFY(expr) do { (void)(expr); } while (0)
#endif

#define VBASE_PANIC(msg) \
    do { ::vbase::panic_impl("VBASE_PANIC", (msg), __FILE__, (int)__LINE__); } while (0)
