// rnd::RPElem: one size entry of a RoomPool. FillSizes stores the two requested sizes and their
// maximum; the loader fills the rest. Original 32-bit size not recovered (at least 0x18).
// Reconstructed from libDungeonHunter2.so pseudocode (FillSizes 0x48bc00, LoadFromXml 0x48c48c).
#pragma once

#include <cstdint>

class TiXmlNode;

namespace rnd {

class RPElem {
public:
    void FillSizes(int a, int b);                                   // 0x48bc00: stores a, max(a, b)
    void LoadFromXml(TiXmlNode* node);                              // 0x48c48c

    uint8_t pad_00[0x04];                                           // +0x00 not written by the recovered code
    uint32_t unk_04;                                                // +0x04 key compared by RoomPool::Find (0x48bc20)
    uint8_t pad_08[0x04];                                           // +0x08 not written by the recovered code
    int32_t unk_0c;                                                 // +0x0c set to the first size by FillSizes
    int32_t unk_10;                                                 // +0x10 max of the two sizes (FillSizes)
    int32_t unk_14;                                                 // +0x14 set to the first size by FillSizes
};

}  // namespace rnd
