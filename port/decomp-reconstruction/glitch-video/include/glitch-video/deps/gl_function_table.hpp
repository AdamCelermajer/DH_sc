// Minimal GL function table. GL calls in the reconstructed code are routed
// through this table, so tests need no GL context: a test fills the table with
// fakes, and the Windows/Android build fills it with the real entry points.
// Only the calls the reconstructed state code needs are listed.
#pragma once

#include <cstdint>

namespace glitch::video::deps {

struct GLFunctionTable {
    void (*enable)(uint32_t cap) = nullptr;
    void (*disable)(uint32_t cap) = nullptr;
    void (*blendFunc)(uint32_t src, uint32_t dst) = nullptr;
    void (*depthFunc)(uint32_t func) = nullptr;
    void (*depthMask)(uint8_t flag) = nullptr;
    void (*clear)(uint32_t mask) = nullptr;
};

// Active table. Defaults to empty; a test or platform layer assigns entries.
GLFunctionTable& activeGLFunctionTable();

}  // namespace glitch::video::deps
