#pragma once

// ======================================================
// vbase build configuration (macro-driven)
// - The build system MUST define VBASE_DEBUG (0/1).
// - Other macros have defaults.
// ======================================================

#ifndef VBASE_DEBUG
  #error "VBASE_DEBUG must be defined by the build system (0 or 1)."
#endif

#ifndef VBASE_USE_EXCEPTIONS
  #define VBASE_USE_EXCEPTIONS 0
#endif

#ifndef VBASE_ENABLE_RTTI
  #define VBASE_ENABLE_RTTI 0
#endif

#ifndef VBASE_ASSERT_LEVEL
  // 0: off, 1: fatal only, 2: assert+verify+fatal
  #define VBASE_ASSERT_LEVEL 2
#endif

#ifndef VBASE_ENABLE_UNREACHABLE_TRAP
  #define VBASE_ENABLE_UNREACHABLE_TRAP 1
#endif

#ifndef VBASE_DISABLE_DEPRECATED_WARNINGS
  #define VBASE_DISABLE_DEPRECATED_WARNINGS 1
#endif

// Compiler detection
#if defined(_MSC_VER)
  #define VBASE_COMPILER_MSVC 1
#else
  #define VBASE_COMPILER_MSVC 0
#endif

#if defined(__clang__)
  #define VBASE_COMPILER_CLANG 1
#else
  #define VBASE_COMPILER_CLANG 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
  #define VBASE_COMPILER_GCC 1
#else
  #define VBASE_COMPILER_GCC 0
#endif

// Platform
#if defined(_WIN32) || defined(__CYGWIN__)
  #define VBASE_PLATFORM_WINDOWS 1
#else
  #define VBASE_PLATFORM_WINDOWS 0
#endif

#if VBASE_DISABLE_DEPRECATED_WARNINGS
  #if VBASE_COMPILER_MSVC
    #define VBASE_DISABLE_DEPRECATED_BEGIN __pragma(warning(push)) __pragma(warning(disable : 4996))
    #define VBASE_DISABLE_DEPRECATED_END   __pragma(warning(pop))
  #elif VBASE_COMPILER_CLANG
    #define VBASE_DISABLE_DEPRECATED_BEGIN _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
    #define VBASE_DISABLE_DEPRECATED_END   _Pragma("clang diagnostic pop")
  #elif VBASE_COMPILER_GCC
    #define VBASE_DISABLE_DEPRECATED_BEGIN _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
    #define VBASE_DISABLE_DEPRECATED_END   _Pragma("GCC diagnostic pop")
  #else
    #define VBASE_DISABLE_DEPRECATED_BEGIN
    #define VBASE_DISABLE_DEPRECATED_END
  #endif
#else
  #define VBASE_DISABLE_DEPRECATED_BEGIN
  #define VBASE_DISABLE_DEPRECATED_END
#endif
