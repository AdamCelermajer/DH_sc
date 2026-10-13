// rnd::RoomPool: a set of RPElem size entries loaded from a <roompool> XML node. The "size" attribute
// is read into +0x00. Original 32-bit size not recovered (at least 0x10).
// Reconstructed from libDungeonHunter2.so pseudocode (dtor 0x485a0c, LoadFromXml 0x48cac0,
// Find 0x48bc20 / 0x48c3f8 / 0x4906bc, ComputeSizeOfRules 0x490904).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "dh2/rnd/RPElem.hpp"

class TiXmlNode;

namespace rnd {

class RandomGenerator;

class RoomPool {
public:
    ~RoomPool();                                                    // 0x485a0c (clears the element vector)

    RPElem* Find(unsigned long key);                                // 0x48bc20
    RPElem* Find(unsigned char* key);                               // 0x48c3f8
    RPElem* Find(const std::string& key);                           // 0x4906bc
    void LoadFromXml(TiXmlNode* node);                              // 0x48cac0
    void ComputeSizeOfRules();                                      // 0x490904

    int32_t m_size;                                                 // +0x00 "size" attribute
    std::vector<RPElem> m_elems;                                    // +0x04 elements ("elem" children)
    RandomGenerator* m_generator;                                   // +0x10 the owning generator (LoadRoomPools sets it before LoadFromXml)
};

}  // namespace rnd
