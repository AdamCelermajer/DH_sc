#pragma once

#include <memory>
#include <string>

namespace dh::foundation {

// Small Win32/WGL boundary. Keys are Windows virtual-key codes (ASCII for A-Z).
class Window final {
public:
    Window();
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    bool open(const char* title, int width, int height);
    // Resize the client drawing area; returns false on invalid dimensions or OS failure.
    bool resize(int width, int height);
    void poll();
    void swap();
    // B066: explicit vsync control (WGL_EXT_swap_control / SDL_GL_SetSwapInterval). Returns false when unsupported.
    bool set_swap_interval(int interval) noexcept;
    // Current driver swap interval, or -1 when it cannot be queried.
    int swap_interval() const noexcept;
    // OpenGL entry point lookup for the current context (wglGetProcAddress / SDL_GL_GetProcAddress); null when absent.
    static void* gl_proc(const char* name) noexcept;
    int width() const noexcept;
    int height() const noexcept;
    bool should_close() const noexcept;
    bool key_down(int virtual_key) const noexcept;
    bool cursor_position(float& client_x,float& client_y) const noexcept;
    bool focused() const noexcept;
    bool minimized() const noexcept;
    const std::string& error() const noexcept;

    // Monotonic, high-resolution seconds, suitable for frame/timeline deltas.
    static double seconds() noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace dh::foundation
