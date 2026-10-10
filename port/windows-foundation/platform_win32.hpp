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
    int width() const noexcept;
    int height() const noexcept;
    bool should_close() const noexcept;
    bool key_down(int virtual_key) const noexcept;
    bool cursor_position(float& client_x,float& client_y) const noexcept;
    // P16 MAPFIX: mouse-wheel notches (120 units each) since the last call; the sub-notch remainder is kept.
    int take_wheel_notches() noexcept;
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
