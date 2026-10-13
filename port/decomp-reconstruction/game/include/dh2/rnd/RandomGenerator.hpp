// rnd::RandomGenerator: owns one random-level generation: the tile grid, the block and list-rule maps,
// the room pools, the root rule and the level configuration it writes. Original 32-bit size 0x190 (400),
// matching the operator new(0x190) in the level code (Level.hpp, m_randomGenerator).
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x487e84 / 0x487fdc, dtors 0x488504 /
// 0x488704, UnloadTiles 0x4843a0, UnloadRules 0x488300, UnloadRoomPools 0x485a34, LoadRoomPools 0x485cdc,
// LoadListRules 0x4865f0, LoadBlocks 0x4892ec, LoadRuleFile 0x489508, GetBlock 0x48e184, GetList 0x484438,
// ValidList 0x484554, ValidBlock 0x4845d0, PrintMap 0x484820, Generate 0x488134, GetFiles 0x488904,
// SaveModularLevelToStream 0x488b84, SaveModularLevelXml 0x488e50).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "core-platform/array2d.hpp"
#include "dh2/rnd/RootRule.hpp"

class LevelConfig;
class StreamBuffer;
class TiXmlNode;

namespace rnd {

class Tile;
class Block;
class ListRule;

class RandomGenerator {
public:
    RandomGenerator();                                              // 0x487e84 / 0x487fdc
    ~RandomGenerator();                                             // 0x488504 / 0x488704

    int NextInt();                                                  // 0x483a94
    int GetInt(int lo, int hi);                                     // 0x483ac8
    unsigned int operator()(unsigned int n);                        // 0x483af8
    unsigned int Hash(unsigned char* data);                         // 0x483b14

    void UnloadTiles();                                             // 0x4843a0
    void UnloadRules();                                             // 0x488300
    void UnloadRoomPools();                                         // 0x485a34

    ListRule* GetList(const char* name) const;                      // 0x484438
    bool ValidList(const char* name);                               // 0x484554
    bool ValidBlock(const char* name);                              // 0x4845d0
    Block* GetBlock(const char* name) const;                        // 0x48e184

    void PrintMap();                                                // 0x484820
    void LoadRoomPools(TiXmlNode* node);                            // 0x485cdc
    void LoadListRules(TiXmlNode* node);                            // 0x4865f0
    void LoadBlocks();                                              // 0x4892ec
    void LoadRuleFile(const char* path);                            // 0x489508
    void Generate(int count);                                       // 0x488134
    void GetFiles(const char* directory, std::vector<std::string>& files);  // 0x488904
    void SaveModularLevelToStream(StreamBuffer& stream);            // 0x488b84
    void SaveModularLevelXml();                                     // 0x488e50

    // Original layout. Offsets are the 32-bit offsets of the original object (no vptr).
    uint8_t pad_00[0x08];                                           // +0x00 .. +0x07 not written by the ctor
    Array2d<Tile*> m_tiles;                                         // +0x08 tile grid (Array2d; 0x34 bytes in the original)
    uint8_t m_blocks[0x14];                                         // +0x3c std::map<const char*, Block*, lstr>, header node at +0x3c (type not declared, see open_questions)
    uint8_t pad_50[0x04];                                           // +0x50 .. +0x53
    uint8_t m_listRules[0x14];                                      // +0x54 std::map<const char*, ListRule*, lstr>, header node at +0x54
    uint8_t pad_68[0x04];                                           // +0x68 .. +0x6b
    RootRule m_rootRule;                                            // +0x6c the root rule (0x90 bytes)
    std::string unk_fc;                                             // +0xfc
    uint8_t pad_114[0x18];                                          // +0x114 .. +0x12b
    std::string unk_12c;                                            // +0x12c
    std::string unk_144;                                            // +0x144
    std::string m_sceneDir;                                         // +0x15c initialized to "data/scene/" by the ctor
    std::string unk_174;                                            // +0x174
    LevelConfig* m_levelConfig;                                     // +0x18c operator new(0x318), constructed with LevelConfig(4)
};

}  // namespace rnd
