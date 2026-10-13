// Animation: one animation entry of an AnimationSet. It holds the collada database of the animation file
// (CColladaDatabase, built from the file name at +0x14) and is destroyed by freeing its own block.
// The destructor (0x364ad4) destroys an embedded CColladaDatabase at this + 0x18 (ADD R0, R0, #0x18 in the
// assembly) and then releases the string block at this + 0x00. Neither member is declared here yet: the
// engine's CColladaDatabase is a partial record, and the string base layout is not resolved (layout request).
// Original 32-bit size not recorded. No vtable.
// Reconstructed from libDungeonHunter2.so pseudocode (dtor 0x364ad4).
#pragma once

#include <cstdint>

class Animation {
public:
    ~Animation();                                                            // 0x364ad4

    // +0x00 .. +0x07 unresolved
    uint8_t pad_00[8];                                                       // +0x00
    int m_clipId;                                                            // +0x08 returned by AnimSetManager::GetClipId
    uint8_t pad_0c[8];                                                       // +0x0c
    const char* m_fileName;                                                  // +0x14 collada file name, passed to CColladaDatabase by AnimSetManager::AddTemplateAnim
};
