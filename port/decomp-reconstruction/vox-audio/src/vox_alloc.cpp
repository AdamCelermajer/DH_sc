#include "vox-audio/vox/memory.hpp"

#include <cstdlib>

namespace {
vox::OutOfMemoryHook g_out_of_memory_hook = nullptr;

void* AllocOrReport(std::size_t size) {
    void* p = std::malloc(size);
    if (p == nullptr && g_out_of_memory_hook != nullptr) {
        g_out_of_memory_hook("Out of memory:");
    }
    return p;
}
}  // namespace

namespace vox {

void SetOutOfMemoryHook(OutOfMemoryHook hook) { g_out_of_memory_hook = hook; }

}  // namespace vox

// 0x3104f8 _Z8VoxAllocj
// Original also prints memory statistics and asserts at gAssertLevel 1/2 on
// failure; the port reports through vox::SetOutOfMemoryHook instead.
void* VoxAlloc(std::size_t size) { return AllocOrReport(size); }

// 0x310648 _Z8VoxAllocjN3vox10VoxMemHintE
// Hint > 0 selects the original's CustomMemalign path. Without the custom
// allocator the port only has malloc, so the hint is ignored.
void* VoxAlloc(std::size_t size, vox::VoxMemHint hint) {
    (void)hint;
    return AllocOrReport(size);
}

// 0x310444 _Z7VoxFreePv
void VoxFree(void* ptr) { std::free(ptr); }

// 0x889490 VoxFree_c
extern "C" void VoxFree_c(void* ptr) { std::free(ptr); }
