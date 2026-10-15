// Batch r1-vox-audio-pack0-5b0f48-0. Only the function that had no body anywhere lives here.
// The other functions of the batch were completed in place in their existing sources (see
// vox_alloc.cpp, voxengine.cpp, voxengine-facade/engine_instance.cpp, engine-internal-data/,
// engine-internal-emitter/, decoder-mswav-mpc8/ and w10-parsefile/).

#include <cstddef>

#include "vox-audio/vox/memory.hpp"

// 0x889494 VoxAlloc_c (partial). Verified from the listing: the whole body is the single ARM
// instruction `B _Z8VoxAllocjPKcS0_i`, so this C-linkage entry point tail-branches to
// VoxAlloc(uint, char const*, char const*, int) at 0x31068c, and the forward above reproduces it
// exactly (the four arguments pass through unchanged, the unsigned line becoming the int the
// symbol declares). The row's status is the target's: VoxAlloc(0x31068c) cannot reproduce the
// gAssertLevel 1/2 branches of CustomAlloc (0x310454), because those and the global they read are
// core-platform's and vox_alloc.cpp has to stay linkable without core-platform (this batch tried
// forwarding and it breaks `check_tu.sh --test`, which links only the listed sources). See
// vox_alloc.cpp.
extern "C" void* VoxAlloc_c(std::size_t size, const char* file, const char* function, unsigned int line) {
    return VoxAlloc(size, file, function, static_cast<int>(line));
}
