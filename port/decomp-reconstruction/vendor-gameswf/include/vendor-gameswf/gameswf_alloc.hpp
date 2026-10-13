// gameswf allocator internals as the DH2 binary has them (Gameloft build). Not in gameswf r1714.
//
// The bodies are in vendor-gameswf/src/v1-vendor-gameswf-gameswf-fddef6-0.cpp (0x752b2c..0x752bac).
// init/close/dump are empty. free_internal and malloc_internal are forwards to the core-platform
// allocator (SwfFree, SwfAlloc). realloc_internal allocates, copies min(old, new) bytes and frees.
//
// The binary ignores the MemHint argument of malloc_internal and passes 0 to SwfAlloc. The second
// argument of free_internal is ignored by the binary (realloc passes the old size, close passes 0).

#pragma once

#include "core-platform/deps/swf_types.hpp"  // swf::MemHint

namespace gameswf {

void init_memory_allocator();   // 0x752b2c, empty
void close_memory_allocator();  // 0x752b30, empty
void dump_memory_allocator();   // 0x752b34, empty

void free_internal(void* p, unsigned int size);  // 0x752b38, forwards to SwfFree(p)

void* malloc_internal(unsigned int size, swf::MemHint hint, const char* file, int line);  // 0x752b3c
void* malloc_internal(unsigned int size, swf::MemHint hint);                              // 0x752b9c

// 0x752b4c: old_size is the 3rd argument, new_size the 2nd. The copy is min(old_size, new_size).
void* realloc_internal(void* p, unsigned int new_size, unsigned int old_size, swf::MemHint hint,
                       const char* file, int line);  // 0x752b4c
void* realloc_internal(void* p, unsigned int new_size, unsigned int old_size,
                       swf::MemHint hint);  // 0x752bac

}  // namespace gameswf
