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

// 0x3104f8 _Z8VoxAllocj (partial). The listing is one `B` to CustomAlloc(uint) at 0x310454.
// Verified from that listing: CustomAlloc mallocs, and on failure calls
// ShowMemoryStats("Out of memory:") and then, with gAssertLevel == 2, writes 0 through a null
// pointer, or, with gAssertLevel == 1, fprintfs "ASSERT(%s) FAILED: %s:%d\n" with "ptr" and
// memory.cpp:58 to stderr. The port reproduces the message through vox::SetOutOfMemoryHook over
// malloc (core-platform's ShowMemoryStats body is empty), but it cannot reproduce either
// assert-level branch here: those branches and the global they read (gAssertLevel) are
// core-platform's, and this file must stay linkable on its own. Forwarding to the real
// CustomAlloc/CustomMemalign was tried in this batch: it links inside the full build (every target
// links the whole LINK_GROUP:RESCAN group), but it breaks the project's own single-batch test
// command `tools/trackb/check_tu.sh --test <test> <sources>`, which links only the listed sources -
// vox-audio/tests/r1-vox-audio-pack0-5b0f48-0_test.cpp and decoder_stb_vorbis_test.cpp both list
// src/vox_alloc.cpp and then fail with "undefined reference to CustomAlloc(unsigned int)". The
// missing symbol is therefore kept out of the build and the row stays partial.
void* VoxAlloc(std::size_t size) { return AllocOrReport(size); }

// 0x310648 _Z8VoxAllocjN3vox10VoxMemHintE (partial). Verified from the listing: the hint is
// tested (SUBS R3,R1,#0); a hint above 0 branches to CustomMemalign(hint, size) at 0x3105b4 and
// anything else to CustomAlloc(size) at 0x310454. CustomMemalign's own listing ignores the
// alignment and tail-calls CustomAlloc(size), so its malloc and message path is the same, but
// CustomMemalign also runs its own gAssertLevel branches before that tail call (report "0" at
// memory.cpp:80, or the null write at level 2). The same missing item applies, and for the same
// reason: those branches are core-platform's and this file has to stay independently linkable.
void* VoxAlloc(std::size_t size, vox::VoxMemHint hint) {
    (void)hint;
    return AllocOrReport(size);
}

// 0x310664 _Z8VoxAllocjN3vox10VoxMemHintEPKcS2_i (partial). Verified from the listing: with a
// hint above 0 it tail-calls CustomMemalign(hint, size); otherwise it tail-calls
// CustomAlloc(size, file, line, nullptr) at 0x310660, which itself only forwards to
// CustomAlloc(uint). The file, function and line arguments are passed through and dropped by
// that thunk, so their registers are not read. The same missing item applies: the gAssertLevel
// 1/2 branches and ShowMemoryStats are core-platform's (see the note on 0x3104f8).
void* VoxAlloc(std::size_t size, vox::VoxMemHint hint, const char* file, const char* function, int line) {
    (void)hint;
    (void)file;
    (void)function;
    (void)line;
    return AllocOrReport(size);
}

// 0x31068c _Z8VoxAllocjPKcS0_i (partial). Verified from the listing: it moves the line argument
// into the third slot and passes nullptr as the fourth, then tail-calls
// CustomAlloc(uint, char const*, uint, char const*) at 0x310660, a one-instruction `B` to
// CustomAlloc(uint) at 0x310454. So the path is malloc plus the out-of-memory report, with the
// file, function and line ignored. The same missing item applies: the gAssertLevel 1/2 branches
// of CustomAlloc are core-platform's (see the note on 0x3104f8).
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
