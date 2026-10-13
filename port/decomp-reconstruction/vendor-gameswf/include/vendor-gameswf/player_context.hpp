// gameswf::player_context as the DH2 binary has it (Gameloft; upstream GameSWF r1714 has no such class).
//
// The context holds the glyph provider, the bitmap glyph provider and an array of players. The layout is
// taken from the constructor (0x76ce38, 0x76ce90) and the destructor (0x76cee8, 0x76d4cc). render_handler_glitch.hpp
// and texture_cache.hpp forward-declare the type; that declaration stays valid.
//
// Layout, 32-bit offsets (host pointers are 8 bytes, so the offsets are comments only):
//   +0x00 vptr and ref_counted base (DH2 ref_counted.hpp, 12 bytes)
//   +0x0c glyph_provider* m_glyph_provider       owned: the destructor runs ~glyph_provider and frees it
//   +0x10 bitmap_glyph_provider* m_bitmap_glyph_provider   the type is absent from the tree
//   +0x14 array<player*> unk_14                  12 bytes: data +0x14, size +0x18, buffer size +0x1c
//   +0x20 uint8_t unk_20                         cleared by the constructor; no batch reads it yet
//   +0x24 int32_t unk_24                         cleared by the constructor
//   +0x28 int32_t unk_28                         cleared by the constructor
//
// Virtual slots (vptr offsets): +0x00 D1 and +0x04 D0. Only the destructor is virtual.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack6-a8bd9d-0.cpp

#pragma once

#include <cstdint>

#include "base/container.h"   // array<T> (upstream; the template is global in base/)
#include "gameswf/gameswf.h"  // player, glyph_provider is forward-declared below
#include "vendor-gameswf/ref_counted.hpp"  // DH2 ref_counted (12 bytes, vptr +0x00)

namespace gameswf {

struct glyph_provider;         // vendor-gameswf/glyph_provider.hpp (DH2)
struct bitmap_glyph_provider;  // absent from the tree; only a pointer is held here

struct player_context : public ref_counted {
    player_context();                 // 0x76ce38 (C1), 0x76ce90 (C2)
    ~player_context() override;       // 0x76cee8 (D2), 0x76d4cc (D1); D0 0x76d578 is compiler-generated

    glyph_provider* m_glyph_provider = nullptr;                 // +0x0c
    bitmap_glyph_provider* m_bitmap_glyph_provider = nullptr;   // +0x10
    array<player*> unk_14;                                      // +0x14
    std::uint8_t unk_20 = 0;                                    // +0x20
    std::int32_t unk_24 = 0;                                    // +0x24
    std::int32_t unk_28 = 0;                                    // +0x28
};

}  // namespace gameswf
