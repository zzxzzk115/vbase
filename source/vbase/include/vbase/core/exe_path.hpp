#pragma once

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

namespace vbase
{
    inline std::filesystem::path executable_path()
    {
#ifdef _WIN32
        wchar_t buffer[MAX_PATH];
        DWORD   len = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        return std::filesystem::path(buffer, buffer + len);
#elif defined(__APPLE__)
        uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);
        std::string buffer;
        buffer.resize(size);
        _NSGetExecutablePath(buffer.data(), &size);
        return std::filesystem::weakly_canonical(buffer);
#else
        char    buffer[4096];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer));
        return std::filesystem::path(buffer, buffer + len);
#endif
    }

    inline std::filesystem::path executable_dir() { return executable_path().parent_path(); }
} // namespace vbase