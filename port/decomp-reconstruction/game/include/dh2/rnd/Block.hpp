// rnd::Block: a placeable room piece of a random level. Has a name, gameplay and visual asset, a
// cell size (width, height) and eight per-side records. Abstract: MgxBlock is the concrete block.
// Original 32-bit size 0x9c0 (2496), taken from the constructor's writes up to the end of the side table.
// Reconstructed from libDungeonHunter2.so pseudocode (ctor 0x48ab34, dtors 0x489f58 / 0x489fbc /
// 0x489fd8, IsStraight 0x489988, GetFlag 0x4899e4, LinkToOtherBlocks 0x489aec).
#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "core-platform/array2d.hpp"

struct lstr;                                                     // global string comparator (name-keyed maps)

namespace rnd {

class Tile;

class Block {
public:
    // One per-side record of the 8-entry table at +0x60. Original size 300 (0x12c) bytes.
    struct Side {
        uint8_t pad_00[0x14];                                       // +0x00 .. +0x13
        const char* unk_14;                                         // +0x14 ctor stores &unk_956B24: a data object (word 4, then a pointer), not a C string
        uint8_t pad_18[0x04];                                       // +0x18 .. +0x1b
        void* unk_1c;                                               // +0x1c zeroed by the ctor (vector-like)
        void* unk_20;                                               // +0x20 zeroed by the ctor
        void* unk_24;                                               // +0x24 zeroed by the ctor
        uint8_t pad_28[0x104];                                      // +0x28 .. +0x12b
    };

    Block();                                                        // 0x48ab34 / 0x48ac24
    virtual ~Block();                                               // 0x489f58, 0x489fbc, 0x489fd8

    // Pure virtuals in vtable slots 2..4. MgxBlock provides them.
    virtual bool PlaceInMap(Array2d<Tile*>& map, Tile* tile, int x, int y) = 0;     // slot 0x08 (returns 1 in the original)
    virtual bool RemoveFromMap(Array2d<Tile*>& map, Tile* tile, int x, int y) = 0;  // slot 0x0c (returns 1 in the original)
    virtual bool FitsInMap(Array2d<Tile*>& map, int x, int y) = 0;                  // slot 0x10

    bool IsStraight() const;                                        // 0x489988
    int GetFlag() const;                                            // 0x4899e4 (bit set of 1 << direction id over the sides)
    void LinkToOtherBlocks(const std::map<const char*, Block*, lstr>& blocks);  // 0x489aec

    // Original layout. Offsets are the 32-bit offsets of the original object; the vptr at +0x00
    // is implicit.
    std::string m_name;                                             // +0x04 "name"
    std::string m_gameplay;                                         // +0x1c "gameplay"
    std::string m_visual;                                           // +0x34 "visual"
    float unk_4c;                                                   // +0x4c initialized to 2960.0f by the ctor
    float unk_50;                                                   // +0x50 initialized to 2960.0f by the ctor
    int32_t m_width;                                                // +0x54 initialized to 1 (1x1 blocks, Track A)
    int32_t m_height;                                               // +0x58 initialized to 1
    int32_t unk_5c;                                                 // +0x5c initialized to 0 (count-like, Track A)
    Side m_sides[8];                                                // +0x60 .. +0x9bf, eight 300-byte records
};

}  // namespace rnd
