// rnd::MgxBlock: a Block loaded from a .mgx level-piece file. Adds two ints after the Block base and
// places itself on the tile grid. Original 32-bit size 0x9c8 (2504).
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x48ad14 / 0x48b108, dtors 0x48a03c /
// 0x48a098 / 0x48a0b4, LoadFromXml 0x48a90c, LoadFromXmlStream 0x48aa84, FromFilename 0x48ad54,
// FitsInMap 0x48b85c, RemoveFromMap 0x48b8f0, PlaceInMap 0x48ba58).
#pragma once

#include <cstdint>

#include "core-platform/array2d.hpp"
#include "dh2/rnd/Block.hpp"

class IFileStream;
class TiXmlHandle;

namespace rnd {

class Tile;

class MgxBlock : public Block {
public:
    MgxBlock();                                                     // 0x48ad14 / 0x48b108
    ~MgxBlock() override;                                           // 0x48a03c, 0x48a098, 0x48a0b4

    void PlaceInMap(Array2d<Tile*>& map, Tile* tile, int x, int y) override;     // 0x48ba58, vtable slot 0x08
    void RemoveFromMap(Array2d<Tile*>& map, Tile* tile, int x, int y) override;  // 0x48b8f0, vtable slot 0x0c
    bool FitsInMap(Array2d<Tile*>& map, int x, int y) override;                  // 0x48b85c, vtable slot 0x10

    void LoadFromXml(TiXmlHandle& handle);                          // 0x48a90c
    void LoadFromXmlStream(IFileStream* stream);                    // 0x48aa84
    static MgxBlock* FromFilename(const char* a, const char* b, const char* c); // 0x48ad54

    // MgxBlock occupies +0x00 .. +0x9bf (Block base). Original 32-bit size 0x9c8.
    int32_t unk_9c0;                                                // +0x9c0 zeroed by the ctor
    int32_t unk_9c4;                                                // +0x9c4 zeroed by the ctor
};

}  // namespace rnd
