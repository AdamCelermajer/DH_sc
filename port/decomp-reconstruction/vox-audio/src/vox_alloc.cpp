#include "vox-audio/vox/memory.hpp"

#include <cstdio>
#include <cstdlib>

// 0x310454 CustomAlloc(uint) and 0x3105b4 CustomMemalign(uint,uint) live in core-platform, but
// their bodies are reached from here through the thunks below, so the port reproduces the whole
// chain. The assert level is core-platform's global (core-platform/src/memory.cpp defines it,
// value 0); vox-audio already references it from VoxSoundManager (-3a5f5c-1.cpp) through the
// declaration in vox-audio/vox/sound_manager.hpp, so the definition comes from the link.
// The reference here is weak so that this translation unit still links into a test target that
// does not carry the global: without it the level-dependent branches are skipped, which is the
// previous behaviour of this file.
extern int gAssertLevel __attribute__((weak));

namespace {
vox::OutOfMemoryHook g_out_of_memory_hook = nullptr;

// The report path of CustomAlloc(uint) at 0x310454, reached only when malloc fails:
// ShowMemoryStats("Out of memory:") and then the gAssertLevel branches. ShowMemoryStats' own body
// is empty in core-platform (0x31041c), so the port's hook carries the report.
constexpr const char kMemoryCppPath[] = "..\\..\\project_vs2005\\Game/..\\..\\sources\\Utils\\memory.cpp";

bool AssertLevelAvailable() { return &gAssertLevel != nullptr; }

void ReportOutOfMemory() {
    if (g_out_of_memory_hook != nullptr) {
        g_out_of_memory_hook("Out of memory:");
    }
    if (!AssertLevelAvailable()) {
        return;
    }
    if (gAssertLevel == 2) {
        // The listing writes 0 through a null pointer (STREQ R4,[R4]); the host aborts instead,
        // which stops the process the same way.
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "ptr", kMemoryCppPath, 0x3a);
    }
}

// The prologue of CustomMemalign(uint,uint) at 0x3105b4: it tests the level before it ignores the
// alignment and tail-calls CustomAlloc, so the report below happens even when the allocation then
// succeeds.
void ReportMemalign() {
    if (!AssertLevelAvailable()) {
        return;
    }
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "0", kMemoryCppPath, 0x50);
    }
}

void* AllocOrReport(std::size_t size) {
    void* p = std::malloc(size);
    if (p == nullptr) {
        ReportOutOfMemory();
    }
    return p;
}

// CustomMemalign(hint, size): the hint is taken as the alignment and dropped.
void* MemalignOrReport(std::size_t size) {
    ReportMemalign();
    return AllocOrReport(size);
}
}  // namespace

namespace vox {

void SetOutOfMemoryHook(OutOfMemoryHook hook) { g_out_of_memory_hook = hook; }

}  // namespace vox

// 0x3104f8 _Z8VoxAllocj
// Verified from the listing: the body is one `B _Z11CustomAllocj`, so this is CustomAlloc(uint) at
// 0x310454 - malloc, and on a null result ShowMemoryStats("Out of memory:") followed by
// gAssertLevel == 2 (write through address 0) or == 1 (the "ptr"/memory.cpp:58 report). Both
// branches are reproduced; the report goes through vox::SetOutOfMemoryHook because
// core-platform's ShowMemoryStats body is empty.
void* VoxAlloc(std::size_t size) { return AllocOrReport(size); }

// 0x310648 _Z8VoxAllocjN3vox10VoxMemHintE
// Verified from the listing: SUBS R3,R1,#0 tests the hint; above 0 it branches to
// CustomMemalign(hint, size) at 0x3105b4 and otherwise to CustomAlloc(size) at 0x310454.
// CustomMemalign's own listing ignores the alignment, runs its gAssertLevel branches ("0" and
// memory.cpp:80) and tail-calls CustomAlloc(size), whose failure path adds the "ptr" report; both
// are reproduced here.
void* VoxAlloc(std::size_t size, vox::VoxMemHint hint) {
    return hint > 0 ? MemalignOrReport(size) : AllocOrReport(size);
}

// 0x310664 _Z8VoxAllocjN3vox10VoxMemHintEPKcS2_i
// Verified from the listing: with a hint above 0 it tail-calls CustomMemalign(hint, size);
// otherwise it tail-calls CustomAlloc(uint, file, line, nullptr) at 0x310660, itself a one
// instruction `B` to CustomAlloc(uint) at 0x310454. The file, function and line arguments are
// passed through and dropped by that thunk, so their registers are never read.
void* VoxAlloc(std::size_t size, vox::VoxMemHint hint, const char* file, const char* function, int line) {
    (void)file;
    (void)function;
    (void)line;
    return hint > 0 ? MemalignOrReport(size) : AllocOrReport(size);
}

// 0x31068c _Z8VoxAllocjPKcS0_i
// Verified from the listing: it moves the line argument into the third slot and passes nullptr as
// the fourth, then tail-calls CustomAlloc(uint, char const*, uint, char const*) at 0x310660, a one
// instruction `B` to CustomAlloc(uint) at 0x310454. So the path is malloc plus the out-of-memory
// report and the gAssertLevel branches; the file, function and line are ignored.
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
