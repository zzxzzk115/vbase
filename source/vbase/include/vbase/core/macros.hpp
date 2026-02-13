#pragma once

#include "vbase/config.hpp"

#if VBASE_COMPILER_MSVC
  #define VBASE_FORCEINLINE __forceinline
  #define VBASE_NOINLINE    __declspec(noinline)
  #define VBASE_DEBUGBREAK() __debugbreak()
#elif VBASE_COMPILER_CLANG || VBASE_COMPILER_GCC
  #define VBASE_FORCEINLINE inline __attribute__((always_inline))
  #define VBASE_NOINLINE    __attribute__((noinline))
  #define VBASE_DEBUGBREAK() __builtin_trap()
#else
  #define VBASE_FORCEINLINE inline
  #define VBASE_NOINLINE
  #define VBASE_DEBUGBREAK() ((void)0)
#endif

#if __cplusplus >= 201703L
  #define VBASE_NODISCARD [[nodiscard]]
#else
  #define VBASE_NODISCARD
#endif

#if __cplusplus >= 202002L
  #define VBASE_LIKELY(x)   (x) [[likely]]
  #define VBASE_UNLIKELY(x) (x) [[unlikely]]
#else
  #define VBASE_LIKELY(x)   (x)
  #define VBASE_UNLIKELY(x) (x)
#endif

#if VBASE_ENABLE_UNREACHABLE_TRAP
  #if VBASE_COMPILER_MSVC
    #define VBASE_UNREACHABLE() __assume(0)
  #else
    #define VBASE_UNREACHABLE() __builtin_unreachable()
  #endif
#else
  #define VBASE_UNREACHABLE() ((void)0)
#endif
