// Minimal GL function table. GL calls in the reconstructed code are routed
// through this table, so tests need no GL context: a test replaces entries with
// recorders, and the Windows/Android build fills it with the real entry points.
// Only the calls the reconstructed render-state code needs are listed. Every
// entry defaults to a no-op so that an unset entry is harmless.
#pragma once

#include <cstdint>

namespace glitch::video::deps {

struct GLFunctionTable {
    void (*enable)(uint32_t cap) = nullptr;
    void (*disable)(uint32_t cap) = nullptr;
    void (*blendFunc)(uint32_t src, uint32_t dst) = nullptr;
    void (*blendEquation)(uint32_t mode) = nullptr;
    void (*blendColor)(float r, float g, float b, float a) = nullptr;
    void (*clearColor)(float r, float g, float b, float a) = nullptr;
    uint32_t (*createProgram)() = nullptr;
    void (*deleteProgram)(uint32_t program) = nullptr;
    void (*useProgram)(uint32_t program) = nullptr;
    void (*cullFace)(uint32_t mode) = nullptr;
    void (*depthFunc)(uint32_t func) = nullptr;
    void (*depthMask)(uint8_t flag) = nullptr;
    void (*depthRangef)(float nearValue, float farValue) = nullptr;
    void (*clearDepthf)(float depth) = nullptr;
    void (*frontFace)(uint32_t mode) = nullptr;
    void (*lineWidth)(float width) = nullptr;
    void (*polygonOffset)(float factor, float units) = nullptr;
    void (*sampleCoverage)(float value, uint8_t invert) = nullptr;
    void (*stencilFunc)(uint32_t func, int32_t ref, uint32_t mask) = nullptr;
    void (*stencilMask)(uint32_t mask) = nullptr;
    void (*stencilOp)(uint32_t fail, uint32_t zfail, uint32_t zpass) = nullptr;
    void (*clearStencil)(int32_t s) = nullptr;
    void (*colorMask)(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = nullptr;
    void (*scissor)(int32_t x, int32_t y, int32_t width, int32_t height) = nullptr;
    void (*flush)() = nullptr;
    void (*finish)() = nullptr;
    void (*clear)(uint32_t mask) = nullptr;
};

// Active table. Entries that are still null are skipped by the callers.
GLFunctionTable& activeGLFunctionTable();

}  // namespace glitch::video::deps
