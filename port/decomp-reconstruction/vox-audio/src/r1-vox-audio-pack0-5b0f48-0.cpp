// Batch r1-vox-audio-pack0-5b0f48-0. Only the function that had no body anywhere lives here.
// The other functions of the batch were completed in place in their existing sources (see
// vox_alloc.cpp, voxengine.cpp, voxengine-facade/engine_instance.cpp, engine-internal-data/,
// engine-internal-emitter/, decoder-mswav-mpc8/ and w10-parsefile/).

#include <cstddef>

#include "vox-audio/vox/memory.hpp"

// 0x889494 VoxAlloc_c (partial: a C-linkage thunk. The asm is a plain branch to 0x31068c, which is
// VoxAlloc(uint, char const*, char const*, int) and is itself partial: the gAssertLevel branches and
// ShowMemoryStats of CustomAlloc (0x310454) are not modelled, see vox_alloc.cpp. The forward is
// complete, so the missing parts are the target's.)
extern "C" void* VoxAlloc_c(std::size_t size, const char* file, const char* function, unsigned int line) {
    return VoxAlloc(size, file, function, static_cast<int>(line));
}
