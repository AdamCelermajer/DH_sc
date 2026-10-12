// Declarations for the global_00 reconstruction slice (w7_global_00.cpp).
#pragma once

#include <istream>
#include <vector>

#include "core-platform/deps/game_types.hpp"

// Three-float vector used by the operator/ overload below. The layout (X, Y, Z
// at +0, +4, +8) is taken from the component stores in the listing.
struct Vector3D {
    float X, Y, Z;
};

// 0x310750 _ZdvRK8Vector3Df
Vector3D operator/(const Vector3D& v, float divisor);

// 0x313498 _ZrsRSiRSt6vectorI7Point3DIfESaIS2_EE
std::istream& operator>>(std::istream& is, std::vector<Point3D<float>>& points);

// Subtitle queue state used by ClearSubtitles. Element and deque layouts follow
// the offsets the listing uses; the entry fields before +0x8 were not recovered.
struct SubtitleEntry {
    void* unknown_00;
    void* unknown_04;
    void* text_08;  // separately allocated buffer, freed before the entry
};

// Deque of SubtitleEntry* with 32-slot nodes. The five words are contiguous in
// the original (the first one is the global g_SubtitlesQueue).
struct SubtitleDeque {
    SubtitleEntry** cur;     // +0x00 start.cur
    SubtitleEntry** first;   // +0x04 start.first (node base)
    SubtitleEntry** last;    // +0x08 start.last (node end)
    SubtitleEntry*** node;   // +0x0C start.node (map slot)
    SubtitleEntry** finish;  // +0x10 finish.cur
};

extern SubtitleDeque g_SubtitlesQueue;
extern SubtitleEntry* g_CurrentSubtitle;

// 0x33971c ClearSubtitles
void ClearSubtitles();
