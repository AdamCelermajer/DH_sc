#include "platform_context_identity.hpp"

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/gl.h>
#elif defined(DH_PLATFORM_SDL2)
#include <SDL.h>
#include <sys/syscall.h>
#include <unistd.h>
#else
#error "Select a supported platform context identity backend."
#endif

namespace dh::foundation {

std::uintptr_t current_render_context_identity() noexcept {
#if defined(_WIN32)
    return reinterpret_cast<std::uintptr_t>(wglGetCurrentContext());
#else
    return reinterpret_cast<std::uintptr_t>(SDL_GL_GetCurrentContext());
#endif
}

std::uintptr_t current_render_drawable_identity() noexcept {
#if defined(_WIN32)
    return reinterpret_cast<std::uintptr_t>(wglGetCurrentDC());
#else
    return reinterpret_cast<std::uintptr_t>(SDL_GL_GetCurrentWindow());
#endif
}

std::uint64_t current_render_thread_identity() noexcept {
#if defined(_WIN32)
    return static_cast<std::uint64_t>(GetCurrentThreadId());
#else
    return static_cast<std::uint64_t>(static_cast<std::uint32_t>(syscall(SYS_gettid)));
#endif
}

} // namespace dh::foundation
