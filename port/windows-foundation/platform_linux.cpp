#include "platform_win32.hpp"
#include "platform_key_codes.hpp"

#ifndef DH_PLATFORM_SDL2
#error "The Linux window adapter requires SDL2."
#endif

#include <SDL.h>
#include <GL/gl.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include "platform_sleep.hpp"

namespace dh::foundation {

struct Window::Impl {
    SDL_Window* window{};
    SDL_GLContext context{};
    int width{}, height{};
    bool close{true}, minimized{};
    bool focused{};
    std::string error;

    void update_dimensions() noexcept {
        if (window) SDL_GetWindowSize(window, &width, &height);
    }

    void cleanup() noexcept {
        close = true;
        if (context) {
            if (SDL_GL_GetCurrentContext() == context) SDL_GL_MakeCurrent(nullptr, nullptr);
            SDL_GL_DeleteContext(context);
            context = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        width = height = 0;
        minimized = false;
        focused = false;
    }
};

Window::Window() : impl_(std::make_unique<Impl>()) {}
Window::~Window() { impl_->cleanup(); }

bool Window::open(const char* title, int width, int height) {
    auto& state = *impl_;
    state.cleanup();
    state.error.clear();
    if (width <= 0 || height <= 0) {
        state.error = "Window dimensions must be positive.";
        return false;
    }
    if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        state.error = std::string("SDL video initialization failed: ") + SDL_GetError();
        return false;
    }

    SDL_GL_ResetAttributes();
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    const char* safe_title = title && *title ? title : "DH-SC Foundation";
    state.window = SDL_CreateWindow(safe_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if (!state.window) {
        state.error = std::string("SDL window creation failed: ") + SDL_GetError();
        state.cleanup();
        return false;
    }
    state.context = SDL_GL_CreateContext(state.window);
    if (!state.context) {
        state.error = std::string("SDL OpenGL context creation failed: ") + SDL_GetError();
        state.cleanup();
        return false;
    }
    if (SDL_GL_MakeCurrent(state.window, state.context) != 0) {
        state.error = std::string("SDL could not make OpenGL context current: ") + SDL_GetError();
        state.cleanup();
        return false;
    }
    { const char* v = std::getenv("DH_VSYNC"); (void)set_swap_interval(v && *v ? std::atoi(v) : 1); } // B066
    // Focus changes are delivered through SDL's event pump. The Win32 host
    // queries GetFocus immediately after ShowWindow; publish the corresponding
    // SDL focus state before FrontendRuntime checks its focused-window gate.
    SDL_PumpEvents();
    state.update_dimensions();
    state.close = false;
    state.focused = (SDL_GetWindowFlags(state.window) & SDL_WINDOW_INPUT_FOCUS) != 0;
    state.minimized = false;
    return true;
}

bool Window::resize(int width, int height) {
    auto& state = *impl_;
    if (!state.window || state.close || width <= 0 || height <= 0) {
        state.error = "Resize requires an open window and positive dimensions.";
        return false;
    }
    SDL_SetWindowSize(state.window, width, height);
    state.error.clear();
    return true;
}

void Window::poll() {
    auto& state = *impl_;
    SDL_Event event{};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_WINDOWEVENT && event.window.windowID == SDL_GetWindowID(state.window) &&
             event.window.event == SDL_WINDOWEVENT_CLOSE)) {
            state.close = true;
        } else if (event.type == SDL_WINDOWEVENT && event.window.windowID == SDL_GetWindowID(state.window)) {
            switch (event.window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                state.width = event.window.data1;
                state.height = event.window.data2;
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                state.minimized = true;
                break;
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_MAXIMIZED:
                state.minimized = false;
                state.update_dimensions();
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                state.focused = true;
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                state.focused = false;
                break;
            default:
                break;
            }
        }
    }
}

void Window::swap() {
    if (impl_->window && !impl_->minimized) SDL_GL_SwapWindow(impl_->window);
}

int Window::width() const noexcept { return impl_->width; }
int Window::height() const noexcept { return impl_->height; }
bool Window::should_close() const noexcept { return impl_->close; }
bool Window::minimized() const noexcept { return impl_->minimized; }
const std::string& Window::error() const noexcept { return impl_->error; }

bool Window::key_down(int key) const noexcept {
    if (key == platform_key::mouse_left) return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    const auto* state = SDL_GetKeyboardState(nullptr);
    SDL_Scancode code = SDL_SCANCODE_UNKNOWN;
    switch (key) {
    case 'A': case 'a': code = SDL_SCANCODE_A; break;
    case 'D': case 'd': code = SDL_SCANCODE_D; break;
    case 'S': case 's': code = SDL_SCANCODE_S; break;
    case 'W': case 'w': code = SDL_SCANCODE_W; break;
    case platform_key::tab: code = SDL_SCANCODE_TAB; break;
    case platform_key::escape: code = SDL_SCANCODE_ESCAPE; break;
    case platform_key::shift: code = SDL_SCANCODE_LSHIFT; break;
    case platform_key::space: code = SDL_SCANCODE_SPACE; break;
    case platform_key::left: code = SDL_SCANCODE_LEFT; break;
    case platform_key::up: code = SDL_SCANCODE_UP; break;
    case platform_key::right: code = SDL_SCANCODE_RIGHT; break;
    case platform_key::down: code = SDL_SCANCODE_DOWN; break;
    case platform_key::f5: code = SDL_SCANCODE_F5; break;
    case platform_key::f9: code = SDL_SCANCODE_F9; break;
    default: break;
    }
    if (code == SDL_SCANCODE_LSHIFT) return state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT];
    return code != SDL_SCANCODE_UNKNOWN && state[code] != 0;
}

bool Window::focused() const noexcept { return impl_->focused; }

bool Window::cursor_position(float& x, float& y) const noexcept {
    if (!impl_->window) return false;
    int client_x{}, client_y{};
    SDL_GetMouseState(&client_x, &client_y);
    x = static_cast<float>(client_x);
    y = static_cast<float>(client_y);
    return true;
}

double Window::seconds() noexcept {
    using clock = std::chrono::steady_clock;
    static const auto epoch = clock::now();
    return std::chrono::duration<double>(clock::now() - epoch).count();
}

} // namespace dh::foundation

namespace dh::foundation {
// B066: see platform_sleep.hpp (Linux sleeps are already accurate).
void platform_enable_precise_timers() noexcept {}

void* Window::gl_proc(const char* name) noexcept { return SDL_GL_GetProcAddress(name); }

bool Window::set_swap_interval(int interval) noexcept {
    if (interval < 0) return SDL_GL_SetSwapInterval(-1) == 0;
    return SDL_GL_SetSwapInterval(interval) == 0;
}

int Window::swap_interval() const noexcept { return SDL_GL_GetSwapInterval(); }
} // namespace dh::foundation
