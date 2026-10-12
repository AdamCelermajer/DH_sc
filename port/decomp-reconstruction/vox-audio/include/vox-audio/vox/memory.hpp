// Allocation entry points (Utils/memory.cpp in the original).
#pragma once

#include <cstddef>

namespace vox {

enum VoxMemHint : int {
    kVoxMemHintDefault = 0,
};

// Receives the "Out of memory:" report that VoxAlloc makes before returning null.
// The original prints memory statistics and asserts depending on gAssertLevel;
// the port routes that through this hook instead.
using OutOfMemoryHook = void (*)(const char* what);
void SetOutOfMemoryHook(OutOfMemoryHook hook);

}  // namespace vox

void* VoxAlloc(std::size_t size);
void* VoxAlloc(std::size_t size, vox::VoxMemHint hint);
void VoxFree(void* ptr);
