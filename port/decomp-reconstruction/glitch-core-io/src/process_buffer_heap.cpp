#include "glitch-core-io/core/process_buffer.hpp"

#include <cstdint>

namespace glitch::core {

namespace {

// The heap object (20 bytes in the original's .bss): the word at +0x00 is the start,
// +0x04 is the bound the size is measured to, +0x08 is the top of the used part, and
// the excess flag is the byte at +0x10. setSize and the allocator, which move these,
// are not reconstructed, so the values stay at their zero defaults.
struct ProcessBufferHeapState {
    std::uintptr_t start = 0;     // +0x00
    std::uintptr_t limit = 0;     // +0x04
    std::uintptr_t top = 0;       // +0x08
    bool excessEnabled = false;   // +0x10
};

ProcessBufferHeapState heap;

}  // namespace

// 0x534218 _ZN6glitch4core24getProcessBufferHeapSizeEv
int getProcessBufferHeapSize() { return static_cast<int>(heap.limit - heap.start); }

// 0x534234 _ZN6glitch4core35getProcessBufferHeapAvailableMemoryEv
int getProcessBufferHeapAvailableMemory() {
    const auto free = static_cast<std::uint32_t>(heap.limit - heap.top) & 0xfffffffcu;
    return static_cast<int>(free) - 8;
}

// 0x534254 _ZN6glitch4core32isProcessBufferHeapExcessEnabledEv
bool isProcessBufferHeapExcessEnabled() { return heap.excessEnabled; }

// 0x534268 _ZN6glitch4core33setProcessBufferHeapExcessEnabledEb
void setProcessBufferHeapExcessEnabled(bool enabled) { heap.excessEnabled = enabled; }

}  // namespace glitch::core
