// Stand-in for the engine's ByteArray (owned by game-app, ByteArray::SetBuffer at 0x36ddac).
// Original layout (32-bit offsets, kept as comments): +0x00 data pointer, +0x04 size (int).
// Only what the net-transport byte-array struct needs is here. Replace with the real type
// when the game-app category lands.
//
// Ownership (integrator decision 4): the object owns its block. The block comes from ByteArrayAllocate
// (the engine allocator call `operator new[](n, 2)` in the original) and goes back through
// CustomFree. Both are the C allocator here, so the pair always matches, whichever C++ runtime
// the program is linked with (a new[] block handed to free() corrupted the heap on mixed runtimes).
// The destructor frees the block and the copy operations deep-copy it, so
// NetStructMemberType<ByteArray> (destructors 0x36ff88 and 0x370778) releases the buffer exactly
// once. The original releases temporaries with an explicit `if (p) CustomFree(p)`; the ported
// functions let the destructor do that.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utility>

// _Z10CustomFreePv: releases memory obtained from the engine allocator.
inline void CustomFree(void* p) { std::free(p); }
// operator new[](unsigned int, int): allocates from the engine allocator (memory hint 2). Named
// apart from core-platform's CustomAlloc so a translation unit that sees both keeps the engine one.
inline void* ByteArrayAllocate(std::size_t n) { return std::malloc(n); }

struct ByteArray {
    void* data = nullptr;       // +0x00
    std::int32_t size = 0;      // +0x04

    ByteArray() = default;
    // Deep copy: the new object owns its own block (the original has no copy operations; the
    // code that copies a ByteArray does it with SetBuffer, which this reproduces).
    ByteArray(const ByteArray& other) { SetBuffer(other.data, static_cast<std::size_t>(other.size)); }
    ByteArray(ByteArray&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }
    ByteArray& operator=(const ByteArray& other) {
        if (this != &other) {
            SetBuffer(other.data, static_cast<std::size_t>(other.size));
        }
        return *this;
    }
    ByteArray& operator=(ByteArray&& other) noexcept {
        if (this != &other) {
            Free();
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
    // Frees the block (the `if (data) CustomFree(data)` the original runs on its temporaries and
    // in the value destructors 0x36ff88 and 0x370778).
    ~ByteArray() { Free(); }

    // Releases the block and clears the pointer. The size is left alone, as the original does.
    void Free() {
        if (data) {
            CustomFree(data);
            data = nullptr;
        }
    }

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
            result = ByteArrayAllocate(count);  // operator new[](n, 2) in the original
            data = result;
        }
        if (result && size > 0) {
            std::memset(data, 0, static_cast<std::size_t>(size));
            std::memcpy(data, src, static_cast<std::size_t>(size));
        }
        return result;
    }
};
