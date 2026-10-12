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

// 0x3104f8 _Z8VoxAllocj (partial: thunk to CustomAlloc(uint) at 0x310454, which is not ported.
// The missing parts are its ShowMemoryStats("Out of memory:") call and the gAssertLevel 1/2
// branches; the port reports through vox::SetOutOfMemoryHook instead.)
void* VoxAlloc(std::size_t size) { return AllocOrReport(size); }

// 0x310648 _Z8VoxAllocjN3vox10VoxMemHintE (partial: a hint > 0 forwards to CustomMemalign(hint, size)
// at 0x3105b4. That function ignores its first argument and returns CustomAlloc(size), so the port's
// malloc path matches. Missing: the gAssertLevel 1/2 branches of CustomMemalign and of CustomAlloc (0x310454),
// which are not modelled because gAssertLevel is not in the port, and ShowMemoryStats.)
void* VoxAlloc(std::size_t size, vox::VoxMemHint hint) {
    (void)hint;
    return AllocOrReport(size);
}

// 0x310664 _Z8VoxAllocjN3vox10VoxMemHintEPKcS2_i (partial: a hint > 0 forwards to CustomMemalign, which
// ignores the hint and returns CustomAlloc(size). Missing: the gAssertLevel branches of CustomMemalign and
// CustomAlloc (0x310454) and ShowMemoryStats. File, function and line are not used by the listing.)
void* VoxAlloc(std::size_t size, vox::VoxMemHint hint, const char* file, const char* function, int line) {
    (void)hint;
    (void)file;
    (void)function;
    (void)line;
    return AllocOrReport(size);
}

// 0x31068c _Z8VoxAllocjPKcS0_i (partial: the thunk CustomAlloc(uint,char const*,uint,char const*) at 0x310660
// forwards to CustomAlloc(uint) at 0x310454. Missing: the gAssertLevel branches and ShowMemoryStats of that
// function, which the port replaces with the out-of-memory hook. File, function and line are not used.)
void* VoxAlloc(std::size_t size, const char* file, const char* function, int line) {
    (void)file;
    (void)function;
    (void)line;
    return AllocOrReport(size);
}

// 0x310444 _Z7VoxFreePv
void VoxFree(void* ptr) { std::free(ptr); }

// 0x889490 VoxFree_c
extern "C" void VoxFree_c(void* ptr) { std::free(ptr); }
