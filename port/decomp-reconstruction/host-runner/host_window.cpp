// Windowed host runner: a Win32 window, an EGL/GLES2 context from ANGLE (libEGL.dll / libGLESv2.dll loaded at run time, never linked or committed),
// the game's GL function table bound to the ANGLE entry points, mouse = touch, F12 / script "shot" = BMP screenshot.
#include <windows.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "glitch-shared/gl_function_table.hpp"
#include "host_runner.hpp"

extern "C" int appOnTouch(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

namespace dh2host {
namespace {

using deps_table = glitch::video::deps::GLFunctionTable;

HWND g_hwnd = nullptr;
bool g_closed = false;
bool g_mouseDown = false;
int g_width = 1280, g_height = 720;
bool g_wantShot = false;

typedef void* EGLDisplay;
typedef void* EGLSurface;
typedef void* EGLContext;
typedef void* EGLConfig;
HMODULE g_libEGL = nullptr, g_libGLES = nullptr;
EGLDisplay g_dpy = nullptr;
EGLSurface g_surf = nullptr;
EGLContext g_ctx = nullptr;
int (*p_eglInitialize)(EGLDisplay, int*, int*);
EGLDisplay (*p_eglGetDisplay)(void*);
int (*p_eglChooseConfig)(EGLDisplay, const int*, EGLConfig*, int, int*);
EGLSurface (*p_eglCreateWindowSurface)(EGLDisplay, EGLConfig, HWND, const int*);
EGLContext (*p_eglCreateContext)(EGLDisplay, EGLConfig, EGLContext, const int*);
int (*p_eglMakeCurrent)(EGLDisplay, EGLSurface, EGLSurface, EGLContext);
int (*p_eglSwapBuffers)(EGLDisplay, EGLSurface);
int (*p_eglSwapInterval)(EGLDisplay, int);
int (*p_eglBindAPI)(unsigned);
int (*p_eglGetError)();
void (*p_glReadPixels)(int, int, int, int, unsigned, unsigned, void*);

template <typename T>
bool load(HMODULE m, const char* name, T& out) {
    out = reinterpret_cast<T>(GetProcAddress(m, name));
    if (!out) std::fprintf(g_runnerOut, "missing %s\n", name);
    return out != nullptr;
}

LRESULT CALLBACK wndProc(HWND h, UINT msg, WPARAM w, LPARAM l) {
    switch (msg) {
        case WM_CLOSE:
            g_closed = true;
            return 0;
        case WM_DESTROY:
            g_closed = true;
            PostQuitMessage(0);
            return 0;
        case WM_LBUTTONDOWN:
            g_mouseDown = true;
            SetCapture(h);
            appOnTouch(1, (short)LOWORD(l), (short)HIWORD(l), 0, 1, 0, 0, 0);
            return 0;
        case WM_MOUSEMOVE:
            if (g_mouseDown) appOnTouch(2, (short)LOWORD(l), (short)HIWORD(l), 0, 1, 0, 0, 0);
            return 0;
        case WM_LBUTTONUP:
            if (g_mouseDown) {
                g_mouseDown = false;
                ReleaseCapture();
                appOnTouch(0, (short)LOWORD(l), (short)HIWORD(l), 0, 1, 0, 0, 0);
            }
            return 0;
        case WM_KEYDOWN:
            if (w == VK_ESCAPE) g_closed = true;
            if (w == VK_F12) g_wantShot = true;
            return 0;
    }
    return DefWindowProcA(h, msg, w, l);
}

void saveBmp(const char* path) {
    std::vector<unsigned char> px(static_cast<size_t>(g_width) * g_height * 4);
    p_glReadPixels(0, 0, g_width, g_height, 0x1908 /*GL_RGBA*/, 0x1401 /*GL_UNSIGNED_BYTE*/, px.data());
    for (size_t i = 0; i < px.size(); i += 4) std::swap(px[i], px[i + 2]);   // BGRA; rows are bottom-up as GL delivers them, as BMP stores them
    BITMAPFILEHEADER fh{};
    BITMAPINFOHEADER ih{};
    ih.biSize = sizeof(ih);
    ih.biWidth = g_width;
    ih.biHeight = g_height;
    ih.biPlanes = 1;
    ih.biBitCount = 32;
    ih.biCompression = BI_RGB;
    ih.biSizeImage = (DWORD)px.size();
    fh.bfType = 0x4d42;
    fh.bfOffBits = sizeof(fh) + sizeof(ih);
    fh.bfSize = fh.bfOffBits + ih.biSizeImage;
    FILE* f = std::fopen(path, "wb");
    if (!f) {
        std::fprintf(g_runnerOut, "cannot write %s\n", path);
        return;
    }
    std::fwrite(&fh, sizeof(fh), 1, f);
    std::fwrite(&ih, sizeof(ih), 1, f);
    std::fwrite(px.data(), 1, px.size(), f);
    std::fclose(f);
    std::fprintf(g_runnerOut, "[window] screenshot %s\n", path);
}

// Binds the game's GL function table to the ANGLE entry points: the same table the Android build binds to libGLESv2.
#define BIND(field, fn) t.field = reinterpret_cast<decltype(t.field)>(GetProcAddress(g_libGLES, fn))
void bindTable(deps_table& t) {
    BIND(enable, "glEnable"); BIND(disable, "glDisable"); BIND(blendFunc, "glBlendFunc"); BIND(blendEquation, "glBlendEquation");
    BIND(blendColor, "glBlendColor"); BIND(clearColor, "glClearColor"); BIND(createProgram, "glCreateProgram"); BIND(deleteProgram, "glDeleteProgram");
    BIND(useProgram, "glUseProgram"); BIND(cullFace, "glCullFace"); BIND(depthFunc, "glDepthFunc"); BIND(depthMask, "glDepthMask");
    BIND(depthRangef, "glDepthRangef"); BIND(clearDepthf, "glClearDepthf"); BIND(frontFace, "glFrontFace"); BIND(lineWidth, "glLineWidth");
    BIND(polygonOffset, "glPolygonOffset"); BIND(sampleCoverage, "glSampleCoverage"); BIND(stencilFunc, "glStencilFunc"); BIND(stencilMask, "glStencilMask");
    BIND(stencilOp, "glStencilOp"); BIND(clearStencil, "glClearStencil"); BIND(colorMask, "glColorMask"); BIND(scissor, "glScissor");
    BIND(flush, "glFlush"); BIND(finish, "glFinish"); BIND(clear, "glClear"); BIND(createShader, "glCreateShader"); BIND(shaderSource, "glShaderSource");
    BIND(compileShader, "glCompileShader"); BIND(deleteShader, "glDeleteShader"); BIND(getShaderiv, "glGetShaderiv"); BIND(getShaderInfoLog, "glGetShaderInfoLog");
    BIND(attachShader, "glAttachShader"); BIND(linkProgram, "glLinkProgram"); BIND(getProgramiv, "glGetProgramiv"); BIND(getProgramInfoLog, "glGetProgramInfoLog");
    BIND(getUniformLocation, "glGetUniformLocation"); BIND(getAttribLocation, "glGetAttribLocation"); BIND(getActiveUniform, "glGetActiveUniform");
    BIND(getActiveAttrib, "glGetActiveAttrib"); BIND(getIntegerv, "glGetIntegerv"); BIND(getFloatv, "glGetFloatv"); BIND(hint, "glHint");
    BIND(pixelStorei, "glPixelStorei"); BIND(getString, "glGetString"); BIND(getError, "glGetError"); BIND(bindBuffer, "glBindBuffer");
    BIND(genBuffers, "glGenBuffers"); BIND(deleteBuffers, "glDeleteBuffers"); BIND(vertexAttribPointer, "glVertexAttribPointer");
    BIND(vertexAttrib4f, "glVertexAttrib4f"); BIND(viewport, "glViewport"); BIND(activeTexture, "glActiveTexture"); BIND(bindTexture, "glBindTexture");
    BIND(readPixels, "glReadPixels"); BIND(bindFramebuffer, "glBindFramebuffer"); BIND(enableVertexAttribArray, "glEnableVertexAttribArray");
    BIND(disableVertexAttribArray, "glDisableVertexAttribArray"); BIND(drawElements, "glDrawElements"); BIND(drawArrays, "glDrawArrays");
    BIND(genRenderbuffers, "glGenRenderbuffers"); BIND(deleteRenderbuffers, "glDeleteRenderbuffers"); BIND(genTextures, "glGenTextures");
    BIND(deleteTextures, "glDeleteTextures"); BIND(texParameteri, "glTexParameteri"); BIND(texParameterf, "glTexParameterf");
    BIND(texSubImage2D, "glTexSubImage2D"); BIND(compressedTexImage2D, "glCompressedTexImage2D"); BIND(compressedTexSubImage2D, "glCompressedTexSubImage2D");
    BIND(generateMipmap, "glGenerateMipmap"); BIND(copyTexSubImage2D, "glCopyTexSubImage2D"); BIND(genFramebuffers, "glGenFramebuffers");
    BIND(framebufferTexture2D, "glFramebufferTexture2D"); BIND(framebufferRenderbuffer, "glFramebufferRenderbuffer");
    BIND(checkFramebufferStatus, "glCheckFramebufferStatus"); BIND(bindRenderbuffer, "glBindRenderbuffer"); BIND(renderbufferStorage, "glRenderbufferStorage");
    BIND(uniform1f, "glUniform1f"); BIND(uniform1i, "glUniform1i"); BIND(uniform1iv, "glUniform1iv"); BIND(uniform2iv, "glUniform2iv");
    BIND(uniform3iv, "glUniform3iv"); BIND(uniform4iv, "glUniform4iv"); BIND(uniform1fv, "glUniform1fv"); BIND(uniform2fv, "glUniform2fv");
    BIND(uniform3fv, "glUniform3fv"); BIND(uniform4fv, "glUniform4fv"); BIND(uniformMatrix4fv, "glUniformMatrix4fv");
    // The two whose size parameters are pointer sized in GLES2, and the BGRA upload (as gl_function_table.cpp does for Android).
    static void (*s_texImage2D)(uint32_t, int32_t, int32_t, int32_t, int32_t, int32_t, uint32_t, uint32_t, const void*);
    static void (*s_bufferData)(uint32_t, intptr_t, const void*, uint32_t);
    static void (*s_bufferSubData)(uint32_t, intptr_t, intptr_t, const void*);
    s_texImage2D = reinterpret_cast<decltype(s_texImage2D)>(GetProcAddress(g_libGLES, "glTexImage2D"));
    s_bufferData = reinterpret_cast<decltype(s_bufferData)>(GetProcAddress(g_libGLES, "glBufferData"));
    s_bufferSubData = reinterpret_cast<decltype(s_bufferSubData)>(GetProcAddress(g_libGLES, "glBufferSubData"));
    t.texImage2D = [](uint32_t target, int32_t level, int32_t internalFormat, int32_t w, int32_t h, int32_t border, uint32_t format, uint32_t type,
                      const void* px) {
        if (format == 0x80E1 && internalFormat == 0x1908) internalFormat = 0x80E1;
        s_texImage2D(target, level, internalFormat, w, h, border, format, type, px);
    };
    t.bufferData = [](uint32_t target, int32_t size, const void* data, uint32_t usage) { s_bufferData(target, size, data, usage); };
    t.bufferSubData = [](uint32_t target, int32_t offset, int32_t size, const void* data) { s_bufferSubData(target, offset, size, data); };
}
#undef BIND

std::string dllDir(const Options& opt) {
    std::string d = opt.angleDir;
    if (d.empty()) {
        char exe[MAX_PATH];
        GetModuleFileNameA(nullptr, exe, MAX_PATH);
        d = exe;
        d.resize(d.find_last_of("\\/") + 1);
    }
    if (!d.empty() && d.back() != '\\' && d.back() != '/') d += '\\';
    return d;
}

bool createWindowAndContext(const Options& opt) {
    g_width = opt.width;
    g_height = opt.height;
    SetProcessDPIAware();
    HINSTANCE inst = GetModuleHandleA(nullptr);
    WNDCLASSA wc{};
    wc.lpfnWndProc = wndProc;
    wc.hInstance = inst;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = "dh2_host_window";
    RegisterClassA(&wc);
    const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
    RECT r{0, 0, g_width, g_height};
    AdjustWindowRect(&r, style, FALSE);
    g_hwnd = CreateWindowA(wc.lpszClassName, "Dungeon Hunter 2 (host runner)", style, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top,
                           nullptr, nullptr, inst, nullptr);
    if (!g_hwnd) return false;

    const std::string dir = dllDir(opt);
    SetDllDirectoryA(dir.c_str());   // libEGL finds libGLESv2.dll and d3dcompiler_47.dll here
    g_libEGL = LoadLibraryA((dir + "libEGL.dll").c_str());
    g_libGLES = LoadLibraryA((dir + "libGLESv2.dll").c_str());
    if (!g_libEGL || !g_libGLES) {
        std::fprintf(g_runnerOut, "cannot load ANGLE libEGL.dll / libGLESv2.dll from %s (use --angle <dir>)\n", dir.c_str());
        return false;
    }
    bool ok = load(g_libEGL, "eglGetDisplay", p_eglGetDisplay) && load(g_libEGL, "eglInitialize", p_eglInitialize) &&
              load(g_libEGL, "eglChooseConfig", p_eglChooseConfig) && load(g_libEGL, "eglCreateWindowSurface", p_eglCreateWindowSurface) &&
              load(g_libEGL, "eglCreateContext", p_eglCreateContext) && load(g_libEGL, "eglMakeCurrent", p_eglMakeCurrent) &&
              load(g_libEGL, "eglSwapBuffers", p_eglSwapBuffers) && load(g_libEGL, "eglSwapInterval", p_eglSwapInterval) &&
              load(g_libEGL, "eglBindAPI", p_eglBindAPI) && load(g_libEGL, "eglGetError", p_eglGetError) && load(g_libGLES, "glReadPixels", p_glReadPixels);
    if (!ok) return false;
    g_dpy = p_eglGetDisplay(GetDC(g_hwnd));
    int major = 0, minor = 0;
    if (!p_eglInitialize(g_dpy, &major, &minor)) {
        std::fprintf(g_runnerOut, "eglInitialize failed 0x%x\n", p_eglGetError());
        return false;
    }
    p_eglBindAPI(0x30A0);   // EGL_OPENGL_ES_API
    const int cfgAttr[] = {0x3024, 8, 0x3023, 8, 0x3022, 8, 0x3021, 8, 0x3025, 24, 0x3026, 8, 0x3040, 4 /*ES2*/, 0x3033, 4 /*window*/, 0x3038};
    EGLConfig cfg = nullptr;
    int n = 0;
    if (!p_eglChooseConfig(g_dpy, cfgAttr, &cfg, 1, &n) || n < 1) {
        std::fprintf(g_runnerOut, "eglChooseConfig failed\n");
        return false;
    }
    g_surf = p_eglCreateWindowSurface(g_dpy, cfg, g_hwnd, nullptr);
    const int ctxAttr[] = {0x3098, 2, 0x3038};
    g_ctx = p_eglCreateContext(g_dpy, cfg, nullptr, ctxAttr);
    if (!g_surf || !g_ctx || !p_eglMakeCurrent(g_dpy, g_surf, g_surf, g_ctx)) {
        std::fprintf(g_runnerOut, "EGL surface/context failed 0x%x\n", p_eglGetError());
        return false;
    }
    p_eglSwapInterval(g_dpy, 0);
    bindTable(glitch::video::deps::activeGLFunctionTable());
    std::fprintf(g_runnerOut, "[window] EGL %d.%d, GL_RENDERER %s\n", major, minor,
                 (const char*)glitch::video::deps::activeGLFunctionTable().getString(0x1F01));
    return true;
}

}  // namespace

int runWindowLoop(const Options& opt) {
    g_runnerOut = splitConsole(opt);
    std::vector<ScriptEvent> script;
    if (!opt.scriptPath.empty() && !loadScript(opt.scriptPath, script)) {
        std::fprintf(g_runnerOut, "cannot read script %s\n", opt.scriptPath.c_str());
        return 2;
    }
    if (!createWindowAndContext(opt)) return 4;
    g_screenshotHook = [](const char* path) { saveBmp(path); };
    if (!bootGame(opt)) {
        std::fprintf(g_runnerOut, "boot failed\n");
        return 3;
    }
    std::fprintf(g_runnerOut, "[runner] booted (window %dx%d, %s clock)\n", opt.width, opt.height, opt.realtime ? "wall" : "fixed");
    std::fflush(g_runnerOut);
    bool quit = false;
    for (int f = 0; !quit && !g_closed && (opt.frames <= 0 || f < opt.frames); ++f) {
        MSG m;
        while (PeekMessageA(&m, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&m);
            DispatchMessageA(&m);
        }
        if (g_closed) break;
        scriptApplyFrame(script, f, opt, quit);
        if (!opt.realtime) clockAdvance(opt.dtMs);
        runFrame();
        if (g_wantShot) {
            g_wantShot = false;
            saveBmp((opt.outDir + "/shot_" + std::to_string(f) + ".bmp").c_str());
        }
        p_eglSwapBuffers(g_dpy, g_surf);
        if (opt.logEvery > 0 && f % opt.logEvery == 0) logState(f);
    }
    logState(currentFrame());
    std::fflush(g_runnerOut);
    return 0;
}

}  // namespace dh2host
