#pragma once

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>
#ifndef RENAME_NOREPLACE
#define RENAME_NOREPLACE (1 << 0)
#endif
#endif

namespace dh2::native::player_profile {

inline std::string publish_errno_message(const char* operation, int value) {
    return std::string(operation) + ": errno=" + std::to_string(value) + " (" +
           std::strerror(value) + ")";
}

// Publish a fully-written same-directory temporary file without replacing an
// existing campaign. renameat2 gives atomic no-replace semantics on Android
// API 24 kernels that implement it; hard-link/unlink is the older fallback.
inline bool publish_new_campaign_file(const std::filesystem::path& temporary,
                                      const std::filesystem::path& target,
                                      std::string& error) {
#ifdef _WIN32
    if (::MoveFileExW(temporary.c_str(), target.c_str(), MOVEFILE_WRITE_THROUGH))
        return true;
    const auto value = static_cast<int>(::GetLastError());
    error = "new campaign exclusive publish failed: Windows error=" +
            std::to_string(value);
    return false;
#else
    int rename_error = ENOSYS;
    // The system call avoids depending on a newer bionic wrapper: the NDK
    // exposes the kernel number even when targeting API 24. No replacing is
    // allowed, and a successful rename consumes the temporary path.
#if defined(__NR_renameat2)
    if (::syscall(__NR_renameat2, AT_FDCWD, temporary.c_str(), AT_FDCWD,
                  target.c_str(), RENAME_NOREPLACE) == 0)
        return true;
    rename_error = errno;
#endif
    // Older kernels can still publish atomically with a same-volume hard link.
    // Try it after any renameat2 failure: filesystems/security policies may
    // allow one no-replace primitive while rejecting the other.
    if (::link(temporary.c_str(), target.c_str()) != 0) {
        const int link_error = errno;
        error = publish_errno_message("new campaign hard-link publish failed",
                                      link_error);
#if defined(__NR_renameat2)
        error += "; renameat2=" + publish_errno_message("failed", rename_error);
#endif
        return false;
    }
    if (::unlink(temporary.c_str()) != 0) {
        const int unlink_error = errno;
        error = publish_errno_message("new campaign temporary unlink failed",
                                      unlink_error);
        return false;
    }
    return true;
#endif
}

} // namespace dh2::native::player_profile
