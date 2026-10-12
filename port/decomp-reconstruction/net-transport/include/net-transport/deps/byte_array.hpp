// Stand-in for the engine's ByteArray (owned by game-app, ByteArray::SetBuffer at 0x36ddac).
// Original layout (32-bit offsets, kept as comments): +0x00 data pointer, +0x04 size (int).
// Only what the net-transport byte-array struct needs is here. Replace with the real type
// when the game-app category lands.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>

// _Z10CustomFreePv: releases memory obtained from the engine allocator.
inline void CustomFree(void* p) { std::free(p); }

struct ByteArray {
    void* data = nullptr;       // +0x00
    std::int32_t size = 0;      // +0x04

    // ByteArray::SetBuffer(void const*, int), 0x36ddac.
    // Reuses the block when the size is unchanged, otherwise frees it and allocates size bytes.
    // Zero-fills then copies. Returns the data pointer (null when nothing was allocated).
    void* SetBuffer(const void* src, std::size_t count) {
        void* result = nullptr;
        if (size == static_cast<std::int32_t>(count)) {
            result = data;
        } else {
            if (data) {
                CustomFree(data);
                data = nullptr;
            }
            size = static_cast<std::int32_t>(count);
            result = new (std::nothrow) std::uint8_t[count];  // operator new[](n, 2) in the original
            data = result;
        }
        if (result && size > 0) {
            std::memset(data, 0, static_cast<std::size_t>(size));
            std::memcpy(data, src, static_cast<std::size_t>(size));
        }
        return result;
    }
};
