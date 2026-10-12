// Reconstruction slice global_03. Functions are rebuilt from Hex-Rays output and
// checked against the ARM listing where the pseudocode was ambiguous.
#include "core-platform/w7_global_03.hpp"

#include <cfloat>
#include <cmath>
#include <cstdio>
#include <new>

namespace gameswf {
// Only the layout the call site needs; the real class lives in the gameswf tree.
class tu_file {
public:
    tu_file(const char* name, const char* mode);
};
int get_verbose_parse();
}  // namespace gameswf

using w7g3::ClipVertex;
using w7g3::Vec2;

namespace {

// Normalizes v in place and returns its length. Stands in for b2Vec2::Normalize,
// whose body is outside this slice.
float NormalizeVec(Vec2& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len < FLT_EPSILON) {
        return len;
    }
    float inv = 1.0f / len;
    v.x *= inv;
    v.y *= inv;
    return len;
}

}  // namespace

// 0x7a7c30 _ZL11fs_callbackPN7gameswf9characterEPKcS3_
void fs_callback(gameswf::character* a1, const char* a2, const char* a3) {
    (void)a1;
    (void)a2;
    (void)a3;
}

// 0x7a8420 _ZL11message_logPKc
// Partial: the stream is taken as stdout. The listing addresses it as __sF + 0x54,
// which is stdout in the Android libc FILE layout; the Windows mapping is not checked.
int message_log(const char* msg) {
    int verbose = gameswf::get_verbose_parse();
    if (verbose) {
        fputs(msg, stdout);
        return fflush(stdout);
    }
    return verbose;
}

// 0x7a846c _ZL12log_callbackbPKc
// Partial: the second stream is taken as stderr (__sF + 0xA8 in the listing); see message_log.
int log_callback(bool to_stderr, const char* msg) {
    if (!to_stderr) {
        return message_log(msg);
    }
    message_log(msg);
    fputs(msg, stderr);
    return fflush(stderr);
}

// 0x7a9954 _ZL11file_openerPKc
// Partial: the listing calls operator new(0x28, swf::MemHint); that MemHint overload is
// not reproduced, and plain ::operator new is used with the same 0x28 size.
gameswf::tu_file* file_opener(const char* name) {
    void* mem = ::operator new(0x28);
    return new (mem) gameswf::tu_file(name, "rb");
}

// 0x7dcb6c _ZL8LongAxisPd
// Reconstructed: the Hex-Rays bit-key compares are |x| comparisons. Differs only for NaN,
// where the original compares bit patterns.
int LongAxis(double* a) {
    int v7 = std::fabs(a[1]) > std::fabs(a[0]) ? 1 : 0;
    if (std::fabs(a[2]) > std::fabs(a[v7])) {
        return 2;
    }
    return v7;
}

// 0x7f5038 _ZL17ClipSegmentToLineP10ClipVertexS0_RK6b2Vec2f
// Partial: the segment clip matches Hex-Rays: vertex copies, the crossing point, and the
// id of the crossing point (v1.id when d0 <= 0, else v0.id). Uses the w7g3::ClipVertex and
// w7g3::Vec2 stand-ins for ClipVertex and b2Vec2, which are defined outside this slice.
int ClipSegmentToLine(ClipVertex* out, ClipVertex* in, const Vec2& normal, float offset) {
    const ClipVertex& v0 = in[0];
    const ClipVertex& v1 = in[1];
    float d0 = (normal.x * v0.v.x + normal.y * v0.v.y) - offset;
    float d1 = (normal.x * v1.v.x + normal.y * v1.v.y) - offset;
    int n = 0;
    if (d0 <= 0.0f) {
        out[n++] = v0;
    }
    if (d1 <= 0.0f) {
        out[n++] = v1;
    }
    if (d0 * d1 < 0.0f) {
        float t = d0 / (d0 - d1);
        out[n].v.x = v0.v.x + t * (v1.v.x - v0.v.x);
        out[n].v.y = v0.v.y + t * (v1.v.y - v0.v.y);
        out[n].id = (d0 <= 0.0f) ? v1.id : v0.id;
        ++n;
    }
    return n;
}

// 0x7f881c _ZL8InPointsRK6b2Vec2PS0_i
// Reconstructed: the sign-flip tricks are fabs; the tolerance uses the same
// float(double) rounding as the listing.
int InPoints(const Vec2& p, const Vec2* pts, int count) {
    if (count <= 0) {
        return 0;
    }
    for (int i = 0; i < count; ++i) {
        float dx = std::fabs(p.x - pts[i].x);
        float dy = std::fabs(p.y - pts[i].y);
        float mx = std::fmax(std::fabs(p.x), std::fabs(pts[i].x));
        float my = std::fmax(std::fabs(p.y), std::fabs(pts[i].y));
        float tx = static_cast<float>(static_cast<float>(mx + 1.0) * 0.000011921);
        float ty = static_cast<float>(static_cast<float>(my + 1.0) * 0.000011921);
        if (tx > dx && ty > dy) {
            return 1;
        }
    }
    return 0;
}

// 0x7f89e0 _ZL10ProcessTwoP6b2Vec2S0_S0_S0_S0_
// Partial: matches Hex-Rays. Uses the w7g3::Vec2 stand-in for b2Vec2 and NormalizeVec in
// place of b2Vec2::Normalize (not in this tree). The scale only matters when the length is
// at least FLT_EPSILON, the case where the normalized vector is used.
int ProcessTwo(Vec2* a1, Vec2* a2, Vec2* a3, Vec2* a4, Vec2* a5) {
    Vec2 d{a5[0].x - a5[1].x, a5[0].y - a5[1].y};
    float len = NormalizeVec(d);
    float v13 = (-a5[1].x) * d.x + (-a5[1].y) * d.y;
    if (v13 <= 0.0f || len < 0.00000011921f) {
        a1[0] = a3[1];
        a2[0] = a4[1];
        a3[0] = a3[1];
        a4[0] = a4[1];
        a5[0] = a5[1];
        return 1;
    }
    float t = v13 / len;
    a1->x = a3[1].x + t * (a3[0].x - a3[1].x);
    a1->y = a3[1].y + t * (a3[0].y - a3[1].y);
    a2->x = a4[1].x + t * (a4[0].x - a4[1].x);
    a2->y = a4[1].y + t * (a4[0].y - a4[1].y);
    return 2;
}

// 0x7f8018 _ZL12ProcessThreeP6b2Vec2S0_S0_S0_S0_
// Partial: control flow and expressions match Hex-Rays. The three return paths (1: vertex 2
// kept, 2: edge reduction, 3: barycentric) are present. Uses the w7g3::Vec2 stand-in for b2Vec2.
int ProcessThree(Vec2* a1, Vec2* a2, Vec2* a3, Vec2* a4, Vec2* a5) {
    const float x0 = a5[0].x, y0 = a5[0].y;
    const float x1 = a5[1].x, y1 = a5[1].y;
    const float x2 = a5[2].x, y2 = a5[2].y;
    const float e20x = x2 - x0;
    const float e20y = y2 - y0;
    const float dot2 = e20x * x2 + e20y * y2;
    const float dot12 = (x2 - x1) * x2 + (y2 - y1) * y2;

    if (dot2 <= 0.0f && dot12 <= 0.0f) {
        a1[0] = a3[2];
        a2[0] = a4[2];
        a3[0] = a3[2];
        a4[0] = a4[2];
        a5[0] = a5[2];
        return 1;
    }

    const float det = (x1 - x0) * e20y - (y1 - y0) * e20x;
    const float v40 = det * (x1 * y2 - y1 * x2);
    const float v33 = -((x2 - x1) * x1 + (y2 - y1) * y1);
    const float v39 = v33 + dot12;

    if (v40 > 0.0f || v33 < 0.0f || dot12 < 0.0f || v39 <= 0.0f) {
        const float v14 = det * (y0 * x2 - x0 * y2);
        const float v26 = -(x0 * e20x + y0 * e20y);
        const float v35 = v26 + dot2;
        if (v14 > 0.0f || v26 < 0.0f || dot2 < 0.0f || v35 <= 0.0f) {
            const float denom = (v40 + v14) + det * (x0 * y1 - y0 * x1);
            const float inv = 1.0f / denom;
            const float w0 = v40 * inv;
            const float w1 = v14 * inv;
            const float w2 = (1.0f - w0) - w1;
            a1->x = w0 * a3[0].x + w1 * a3[1].x + w2 * a3[2].x;
            a1->y = w0 * a3[0].y + w1 * a3[1].y + w2 * a3[2].y;
            a2->x = w0 * a4[0].x + w1 * a4[1].x + w2 * a4[2].x;
            a2->y = w0 * a4[0].y + w1 * a4[1].y + w2 * a4[2].y;
            return 3;
        }
        const float t = v26 / v35;
        a1->x = a3[0].x + t * (a3[2].x - a3[0].x);
        a1->y = a3[0].y + t * (a3[2].y - a3[0].y);
        a2->x = a4[0].x + t * (a4[2].x - a4[0].x);
        a2->y = a4[0].y + t * (a4[2].y - a4[0].y);
        a3[1] = a3[2];
        a4[1] = a4[2];
        a5[1] = a5[2];
        return 2;
    }

    const float t = v33 / v39;
    a1->x = a3[1].x + t * (a3[2].x - a3[1].x);
    a1->y = a3[1].y + t * (a3[2].y - a3[1].y);
    a2->x = a4[1].x + t * (a4[2].x - a4[1].x);
    a2->y = a4[1].y + t * (a4[2].y - a4[1].y);
    a3[0] = a3[2];
    a4[0] = a4[2];
    a5[0] = a5[2];
    return 2;
}
