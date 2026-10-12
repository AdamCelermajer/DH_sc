// global_00 slice: reconstructed functions. Rows without a body here are
// recorded in the ledger as skipped or partial with the reason.
// 0x30ef50 ObjToStr(vector3d<float>&) is not repeated here: it is already defined
// in core-platform/src/obj_str.cpp with the same "%f,%f,%f" body.

#include "core-platform/w7_global_00.hpp"

#include <istream>
#include <vector>

#include "core-platform/deps/game_types.hpp"
#include "core-platform/memory.hpp"

namespace {
// The original node holds 0x80 bytes of 4-byte pointers, i.e. 32 slots.
constexpr int kSubtitleNodeSlots = 32;
}  // namespace

// Storage for the subtitle queue. Only the ClearSubtitles side is recovered here.
SubtitleDeque g_SubtitlesQueue{};
SubtitleEntry* g_CurrentSubtitle = nullptr;

// 0x310750 _ZdvRK8Vector3Df
// Divides each component by the scalar and returns the result by value.
Vector3D operator/(const Vector3D& v, float divisor) {
    return Vector3D{v.X / divisor, v.Y / divisor, v.Z / divisor};
}

// 0x313498 _ZrsRSiRSt6vectorI7Point3DIfESaIS2_EE
// Partial: the element reader is read as X, sep, Y, sep, Z (the same pattern as
// the vector3d<float> reader at 0x310380). The separator is read as a single
// character and the loop continues while it is ';' or ' ' as the listing
// compares. The ';' element separator matches operator<< at 0x3133e0. The ' '
// case and the exact char-read call (_M_formatted_get argument) are not confirmed.
std::istream& operator>>(std::istream& is, std::vector<Point3D<float>>& points) {
    char sep = 0;
    do {
        sep = 'a';
        Point3D<float> p{0.0f, 0.0f, 0.0f};
        is >> p.X;
        is >> sep;
        is >> p.Y;
        is >> sep;
        is >> p.Z;
        points.push_back(p);
        is >> sep;
    } while (sep == ';' || sep == ' ');
    return is;
}

// 0x33971c ClearSubtitles
// Partial: the queue is a deque of SubtitleEntry* in 32-slot nodes. Each entry
// frees its text buffer (+0x8) and then itself. The node release at the end of
// a node calls CustomFree where the original calls the SGI node allocator
// (__node_alloc::_M_deallocate, 0x80 bytes); the allocation side is not recovered,
// so the allocator that matches it is not yet confirmed.
void ClearSubtitles() {
    SubtitleDeque& q = g_SubtitlesQueue;
    while (q.cur != q.finish) {
        SubtitleEntry* entry = *q.cur;
        if (q.cur == q.last - 1) {
            if (q.first != nullptr) {
                CustomFree(q.first);
            }
            ++q.node;
            q.cur = *q.node;
            q.first = q.cur;
            q.last = q.cur + kSubtitleNodeSlots;
        } else {
            ++q.cur;
        }

        if (entry->text_08 != nullptr) {
            CustomFree(entry->text_08);
            entry->text_08 = nullptr;
        }
        CustomFree(entry);
    }

    if (g_CurrentSubtitle != nullptr) {
        if (g_CurrentSubtitle->text_08 != nullptr) {
            CustomFree(g_CurrentSubtitle->text_08);
            g_CurrentSubtitle->text_08 = nullptr;
        }
        CustomFree(g_CurrentSubtitle);
        g_CurrentSubtitle = nullptr;
    }
}
