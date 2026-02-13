#pragma once
// vbase provides generic DLL export/import helpers for the ecosystem.
// vbase itself is typically static/header-only.

#if defined(_WIN32) || defined(__CYGWIN__)
  #define VBASE_DLL_EXPORT __declspec(dllexport)
  #define VBASE_DLL_IMPORT __declspec(dllimport)
#else
  #define VBASE_DLL_EXPORT __attribute__((visibility("default")))
  #define VBASE_DLL_IMPORT __attribute__((visibility("default")))
#endif
