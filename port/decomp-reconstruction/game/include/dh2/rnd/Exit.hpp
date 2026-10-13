// rnd::Exit: one open connection of a Block, facing a Direction. Holds the owning block, the grid
// cell it sits on, a float weight and the list of exit names it accepts. Original 32-bit size 0x2c (44).
// Reconstructed from libDungeonHunter2.so pseudocode (ctor 0x48a380; LoadFromXml 0x48a5d0;
// GetBlockUnitPosition 0x489ddc).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

class TiXmlHandle;
template <typename T> struct Point3D;

namespace rnd {

class Block;
class Direction;

class Exit {
public:
    // Ctor arguments in order: direction, block, x, y, float, int, names.
    Exit(const Direction& direction, Block* block, int x, int y, float weight, int flags,
         std::vector<std::string>& names);                          // 0x48a380 / 0x48a3d4

    bool LoadFromXml(TiXmlHandle handle, Block* block, int index);  // 0x48a5d0 (returns 1 when an exit was added)
    void GetBlockUnitPosition(Point3D<float>& position, Block* block);  // 0x489ddc (writes m_x and m_y; reads position X and Y)

    const char* unk_00;                                             // +0x00 the owning block's name pointer (Block::m_name c_str, ctor)
    Block* m_block;                                                 // +0x04
    int32_t m_x;                                                    // +0x08
    int32_t m_y;                                                    // +0x0c
    float unk_10;                                                   // +0x10 float ctor argument
    const Direction* m_direction;                                   // +0x14 Direction id source (Track A reads Exit+20)
    int32_t unk_18;                                                 // +0x18 int ctor argument
    std::vector<std::string> m_names;                               // +0x1c
    int32_t unk_28;                                                 // +0x28 zeroed by the ctor
};

}  // namespace rnd
