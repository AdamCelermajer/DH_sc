// gameswf::render_cache as the DH2 binary has it (Gameloft; upstream GameSWF r1714 has no such class).
//
// The cache stamps the clocks it was built against. Two pairs of 32-bit words hold them: the texture cache's
// m_last_clock (+0x00 and +0x04) and a second clock (+0x08 and +0x0c) that comes from the bitmap glyph provider.
// The destructor (0x78ba28) releases its two arrays and two owned blocks.
//
// Layout, 32-bit offsets (host pointers are 8 bytes, so the offsets are comments only):
//   +0x00 int32_t m_texture_clock[2]    texture_cache::m_last_clock as (low, high) words
//   +0x08 int32_t m_bitmap_clock[2]     second clock, (low, high) words
//   +0x10 array<entry> unk_10           12 bytes; entry is 24 bytes (six 32-bit words)
//   +0x1c uint8_t pad_1c[4]
//   +0x20 owned_block unk_20            16 bytes
//   +0x30 owned_block unk_30            16 bytes
//   +0x40 array<uint16_t> unk_40        12 bytes
//
// The clock comparison (0x7738a4) is dh2::render_cache_clocks_valid in render_cache_clocks.hpp (no dependencies).
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack6-a8bd9d-0.cpp

#pragma once

#include <cstdint>

#include "base/container.h"                    // array<T> (upstream; global in base/)
#include "vendor-gameswf/render_cache_clocks.hpp"  // dh2::render_cache_clocks_valid

namespace gameswf {

struct character;  // gameswf.h

struct render_cache {
    // Element of unk_10. The destructor zeroes six 32-bit words per element when the size is negative.
    struct entry {
        std::int32_t unk_00 = 0;  // +0x00
        std::int32_t unk_04 = 0;  // +0x04
        std::int32_t unk_08 = 0;  // +0x08
        std::int32_t unk_0c = 0;  // +0x0c
        std::int32_t unk_10 = 0;  // +0x10
        std::int32_t unk_14 = 0;  // +0x14
    };

    // A 16-byte block owned by the cache when flag_0c is zero. The destructor frees unk_00 in that case.
    struct owned_block {
        void* unk_00 = nullptr;     // +0x00
        std::int32_t unk_04 = 0;    // +0x04: cleared by the destructor
        std::int32_t unk_08 = 0;    // +0x08: cleared by the destructor when the block is owned
        std::uint8_t flag_0c = 0;   // +0x0c: zero means owned (freed by the destructor)
    };

    std::int32_t m_texture_clock[2] = {0, 0};  // +0x00
    std::int32_t m_bitmap_clock[2] = {0, 0};   // +0x08
    array<entry> unk_10;                       // +0x10
    std::uint8_t pad_1c[4] = {};               // +0x1c
    owned_block unk_20;                        // +0x20
    owned_block unk_30;                        // +0x30
    array<std::uint16_t> unk_40;               // +0x40

    ~render_cache();                           // 0x78ba28 (partial)

    // 0x7738a4. PARTIAL: the player and the bitmap glyph provider are not in the tree, so this returns false.
    bool is_valid(character* ch);
};

}  // namespace gameswf
