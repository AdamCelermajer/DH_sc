// Animation: one animation entry of an AnimationSet. It holds the collada database of the animation file
// (CColladaDatabase, built from the file name at +0x14) and is destroyed by freeing its own block.
// The destructor (0x364ad4) destroys an embedded CColladaDatabase at this + 0x18 (ADD R0, R0, #0x18 in the
// assembly) and then releases the string block at this + 0x00. Neither member is declared here yet: the
// engine's CColladaDatabase is a partial record, and the string base layout is not resolved (layout request).
// Original 32-bit size 0x30 (48): AnimationSet::s_invalidAnim is 48 bytes. The +0x20..+0x2c words are the
// clip id, use count, last-use stamp and counter read by the AnimationSet cache (see the members below).
// No vtable.
// Reconstructed from libDungeonHunter2.so pseudocode (dtor 0x364ad4).
#pragma once

#include <cstdint>

class Animation {
public:
    ~Animation();                                                            // 0x364ad4

    // +0x00 .. +0x07 unresolved
    uint8_t pad_00[8];                                                       // +0x00
    int m_clipId;                                                            // +0x08 (misplaced, see unk_20: 0x364ad4 puts the string block over +0x00..+0x17, and GetClipId reads +0x20)
    uint8_t pad_0c[8];                                                       // +0x0c
    const char* m_fileName;                                                  // +0x14 collada file name, passed to CColladaDatabase by AnimSetManager::AddTemplateAnim
    uint8_t pad_18[8];                                                       // +0x18 engine CColladaDatabase (8 bytes at 32-bit), destroyed by ~Animation; not declared here
    int32_t unk_20;                                                          // +0x20 clip id, -1 = none. AnimSetManager::GetClipId (0x47559c) returns it; AnimationSet::GetAnimationBySetIndex (0x364bf4) matches it. name: m_clipId, once the +0x08 member is retired
    int32_t m_useCount;                                                      // +0x24 use count; > 0 means in use (AnimationSet::_FindCacheCandidate 0x3649e0, UnloadAnimation 0x3650b4)
    uint32_t m_lastUseTime;                                                  // +0x28 glitch::os::Timer::getRealTime stamp on each hit (AnimationSet::GetAnimation 0x3660f4, GetAnimationBySetIndex 0x364bf4); LRU key in _FindCacheCandidate
    int32_t unk_2c;                                                          // +0x2c counter, compared by AnimationSet::_FindCacheCandidate (0x3649e0); incremented by setCurrentAnimation (not checked)
};
