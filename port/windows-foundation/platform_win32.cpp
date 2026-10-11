#include "platform_win32.hpp"

#ifndef _WIN32
#error "The Windows foundation platform requires Windows."
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <GL/gl.h>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "platform_sleep.hpp"

namespace dh::foundation {
namespace {
constexpr wchar_t window_class[] = L"DHSCWindowsFoundation";

std::wstring wide_title(const char* title) {
    if (!title || !*title) return L"DH-SC Foundation";
    const int size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, title, -1, nullptr, 0);
    if (!size) return L"DH-SC Foundation";
    std::wstring result(static_cast<std::size_t>(size), L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, title, -1, result.data(), size);
    result.resize(static_cast<std::size_t>(size - 1));
    return result;
}
} // namespace

struct Window::Impl {
    HWND hwnd = nullptr;
    HDC dc = nullptr;
    HGLRC context = nullptr;
    int width = 0;
    int height = 0;
    bool close = true;
    bool minimized = false;
    std::array<bool, 256> keys{};
    int wheel_units = 0; // P16 MAPFIX: WM_MOUSEWHEEL delta accumulated until take_wheel_notches()
    std::string error;

    static LRESULT CALLBACK procedure(HWND hwnd, UINT message, WPARAM wp, LPARAM lp) {
        auto* self = reinterpret_cast<Impl*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        if (message == WM_NCCREATE) {
            const auto* creation = reinterpret_cast<const CREATESTRUCTW*>(lp);
            self = static_cast<Impl*>(creation->lpCreateParams);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        }
        if (self) {
            switch (message) {
            case WM_CLOSE:
                self->close = true;
                return 0;
            case WM_SIZE:
                self->width = LOWORD(lp);
                self->height = HIWORD(lp);
                self->minimized = wp == SIZE_MINIMIZED;
                return 0;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                if (wp < self->keys.size()) self->keys[wp] = true;
                // Let Windows handle system keys, including Alt+F4.
                if (message == WM_KEYDOWN) return 0;
                break;
            case WM_KEYUP:
            case WM_SYSKEYUP:
                if (wp < self->keys.size()) self->keys[wp] = false;
                if (message == WM_KEYUP) return 0;
                break;
            case WM_KILLFOCUS:
                self->keys.fill(false);
                return 0;
            case WM_LBUTTONDOWN:
                self->keys[VK_LBUTTON]=true;SetCapture(hwnd);return 0;
            case WM_LBUTTONUP:
                self->keys[VK_LBUTTON]=false;if(GetCapture()==hwnd)ReleaseCapture();return 0;
            case WM_CAPTURECHANGED:
                self->keys[VK_LBUTTON]=false;return 0;
            case WM_MOUSEWHEEL: // P16 MAPFIX: consumed by the Map page zoom (take_wheel_notches)
                self->wheel_units+=GET_WHEEL_DELTA_WPARAM(wp);return 0;
            case WM_ERASEBKGND:
                return 1;
            case WM_DESTROY:
                self->close = true;
                return 0;
            case WM_NCDESTROY:
                SetWindowLongPtrW(hwnd, GWLP_USERDATA, 0);
                break;
            }
        }
        return DefWindowProcW(hwnd, message, wp, lp);
    }

    void cleanup() noexcept {
        close = true;
        keys.fill(false);
        if (context) {
            if (wglGetCurrentContext() == context) wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(context);
            context = nullptr;
        }
        if (dc && hwnd) ReleaseDC(hwnd, dc);
        dc = nullptr;
        if (hwnd && IsWindow(hwnd)) DestroyWindow(hwnd);
        hwnd = nullptr;
    }

    bool fail(const char* operation) {
        const DWORD code = GetLastError();
        error = std::string(operation) + " failed (Windows error " + std::to_string(code) + ").";
        cleanup();
        return false;
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

    const HINSTANCE instance = GetModuleHandleW(nullptr);
    WNDCLASSEXW cls{};
    cls.cbSize = sizeof(cls);
    cls.style = CS_OWNDC;
    cls.lpfnWndProc = Impl::procedure;
    cls.hInstance = instance;
    cls.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    cls.lpszClassName = window_class;
    if (!RegisterClassExW(&cls) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return state.fail("RegisterClassExW");

    constexpr DWORD style = WS_OVERLAPPEDWINDOW;
    RECT bounds{0, 0, width, height};
    if (!AdjustWindowRectEx(&bounds, style, FALSE, 0)) return state.fail("AdjustWindowRectEx");
    const auto caption = wide_title(title);
    state.hwnd = CreateWindowExW(0, window_class, caption.c_str(), style,
        CW_USEDEFAULT, CW_USEDEFAULT, bounds.right - bounds.left, bounds.bottom - bounds.top,
        nullptr, nullptr, instance, &state);
    if (!state.hwnd) return state.fail("CreateWindowExW");
    state.dc = GetDC(state.hwnd);
    if (!state.dc) return state.fail("GetDC");

    PIXELFORMATDESCRIPTOR format{};
    format.nSize = sizeof(format);
    format.nVersion = 1;
    format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    format.iPixelType = PFD_TYPE_RGBA;
    format.cColorBits = 32;
    format.cDepthBits = 24;
    format.cStencilBits = 8;
    format.iLayerType = PFD_MAIN_PLANE;
    const int selected = ChoosePixelFormat(state.dc, &format);
    if (!selected) return state.fail("ChoosePixelFormat");
    if (!SetPixelFormat(state.dc, selected, &format)) return state.fail("SetPixelFormat");
    state.context = wglCreateContext(state.dc);
    if (!state.context) return state.fail("wglCreateContext");
    if (!wglMakeCurrent(state.dc, state.context)) return state.fail("wglMakeCurrent");
    // B066: make the swap interval explicit (vsync on unless DH_VSYNC=0) instead of inheriting the driver default.
    { const char* v = std::getenv("DH_VSYNC"); const int want = v && *v ? std::atoi(v) : 1; const bool ok = set_swap_interval(want); std::cout << "Swap interval requested=" << want << " supported=" << ok << " effective=" << swap_interval() << std::endl; }

    RECT client{};
    GetClientRect(state.hwnd, &client);
    state.width = client.right;
    state.height = client.bottom;
    state.minimized = false;
    state.close = false;
    ShowWindow(state.hwnd, SW_SHOW);
    UpdateWindow(state.hwnd);
    return true;
}

bool Window::resize(int width, int height) {
    auto& state = *impl_;
    if (!state.hwnd || state.close || width <= 0 || height <= 0) {
        state.error = "Resize requires an open window and positive dimensions.";
        return false;
    }
    RECT bounds{0, 0, width, height};
    const auto style = static_cast<DWORD>(GetWindowLongPtrW(state.hwnd, GWL_STYLE));
    const auto extended = static_cast<DWORD>(GetWindowLongPtrW(state.hwnd, GWL_EXSTYLE));
    if (!AdjustWindowRectEx(&bounds, style, GetMenu(state.hwnd) != nullptr, extended)) {
        state.error = "AdjustWindowRectEx failed during resize (Windows error " +
            std::to_string(GetLastError()) + ").";
        return false;
    }
    if (!SetWindowPos(state.hwnd, nullptr, 0, 0, bounds.right - bounds.left,
                      bounds.bottom - bounds.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE)) {
        state.error = "SetWindowPos failed during resize (Windows error " +
            std::to_string(GetLastError()) + ").";
        return false;
    }
    state.error.clear();
    return true;
}

void Window::poll() {
    MSG message{};
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) impl_->close = true;
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

void Window::swap() {
    if (impl_->dc && !impl_->minimized) SwapBuffers(impl_->dc);
}

int Window::width() const noexcept { return impl_->width; }
int Window::height() const noexcept { return impl_->height; }
bool Window::should_close() const noexcept { return impl_->close; }
bool Window::minimized() const noexcept { return impl_->minimized; }
const std::string& Window::error() const noexcept { return impl_->error; }
bool Window::key_down(int key) const noexcept {
    return key >= 0 && key < static_cast<int>(impl_->keys.size()) && impl_->keys[key];
}
bool Window::focused() const noexcept{return impl_->hwnd&&GetFocus()==impl_->hwnd;}
bool Window::cursor_position(float& x,float& y) const noexcept {
    POINT point{};if(!impl_->hwnd||!GetCursorPos(&point)||!ScreenToClient(impl_->hwnd,&point))return false;
    x=float(point.x);y=float(point.y);return true;
}
int Window::take_wheel_notches() noexcept {
    const int notches=impl_->wheel_units/WHEEL_DELTA;
    impl_->wheel_units-=notches*WHEEL_DELTA;
    return notches;
}

double Window::seconds() noexcept {
    static const double frequency = [] {
        LARGE_INTEGER value{};
        QueryPerformanceFrequency(&value);
        return static_cast<double>(value.QuadPart);
    }();
    LARGE_INTEGER now{};
    QueryPerformanceCounter(&now);
    return static_cast<double>(now.QuadPart) / frequency;
}

} // namespace dh::foundation

namespace dh::foundation {
// B066: see platform_sleep.hpp.
void platform_enable_precise_timers() noexcept {
    static const bool once = [] {
        if (HMODULE winmm = LoadLibraryW(L"winmm.dll")) {
            using Begin = UINT(WINAPI*)(UINT);
            if (auto begin = reinterpret_cast<Begin>(reinterpret_cast<void*>(GetProcAddress(winmm, "timeBeginPeriod")))) begin(1);
        }
        return true;
    }();
    (void)once;
}

void* Window::gl_proc(const char* name) noexcept {
    void* address = reinterpret_cast<void*>(wglGetProcAddress(name));
    // Some drivers return small sentinel values instead of null for unsupported names.
    const auto value = reinterpret_cast<std::uintptr_t>(address);
    if (value <= 3 || value == static_cast<std::uintptr_t>(-1)) return nullptr;
    return address;
}

bool Window::set_swap_interval(int interval) noexcept {
    using Set = BOOL(WINAPI*)(int);
    auto set = reinterpret_cast<Set>(gl_proc("wglSwapIntervalEXT"));
    return set && set(interval) != FALSE;
}

int Window::swap_interval() const noexcept {
    using Get = int(WINAPI*)();
    auto get = reinterpret_cast<Get>(gl_proc("wglGetSwapIntervalEXT"));
    return get ? get() : -1;
}
} // namespace dh::foundation
