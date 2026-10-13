// rnd::Tile: one placed block instance on the random-level grid. Tiles form a tree through their
// parent and child links, and carry a copy of the ListElem that produced them. Original 32-bit size
// 0x84 (132) as far as the constructor writes go; the tail is not recovered.
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x4917f8 / 0x491850, dtors 0x4914e8 /
// 0x491500, AddChild 0x491428, RemoveChild 0x491440, TryPlaceTile 0x4913d0, PlaceTile 0x4913d8,
// Unspawn 0x4918a8, RemoveNeighbors 0x491928, NewRoot 0x49196c, Spawn 0x4919b0, TrySpawn 0x491ab0).
#pragma once

#include <cstdint>

#include "dh2/rnd/ListElem.hpp"

class TiXmlElement;
class Module;

namespace rnd {

class RandomGenerator;
class Block;
class Exit;

class Tile {
public:
    Tile(RandomGenerator& generator, Block& block, Tile* parent, ListElem& elem);  // 0x4917f8 / 0x491850
    ~Tile();                                                        // 0x4914e8 / 0x491500

    bool TryPlaceTile(int x, int y);                                // 0x4913d0
    void PlaceTile(int x, int y, float z);                          // 0x4913d8
    void AddChild(Tile* child);                                     // 0x491428
    void RemoveChild(Tile* child);                                  // 0x491440
    void Print();                                                   // 0x4914ac
    void SetModuleMVXProperties(Module* module);                    // 0x491518
    void Unspawn();                                                 // 0x4918a8
    void RemoveNeighbors();                                         // 0x491928
    void SaveAsModuleXML(TiXmlElement* parent);                     // 0x491d90

    // Signatures for NewRoot, Spawn and TrySpawn are inferred from the call sites (see open_questions).
    static Tile* NewRoot(RandomGenerator& generator, Block* block, ListElem& elem);        // 0x49196c
    Tile* Spawn(Block& block, const Exit* from, ListElem& elem);    // 0x4919b0
    bool TrySpawn(const Exit* from, Block& block, ListElem& elem);  // 0x491ab0

    // Original layout. Offsets are the 32-bit offsets of the original object.
    void* unk_00;                                                   // +0x00 copy of the block's +0x18 pointer (ctor)
    RandomGenerator* m_generator;                                   // +0x04
    Tile* m_parent;                                                 // +0x08
    int32_t m_childCount;                                           // +0x0c incremented by AddChild
    Tile* m_children[8];                                            // +0x10 .. +0x2f child slots (count at +0x0c)
    Block* m_block;                                                 // +0x30
    ListElem m_listElem;                                            // +0x34 copy of the spawning ListElem
};

}  // namespace rnd
