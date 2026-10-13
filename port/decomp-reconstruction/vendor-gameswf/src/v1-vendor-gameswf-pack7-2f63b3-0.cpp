// Batch v1-vendor-gameswf-pack7-2f63b3-0: gameswf::edge, gameswf::mesh, gameswf::mesh_set and
// gameswf::path (gameswf r1714 with the DH2 changes).
//
// Upstream rows, no body here (the binary matches the gameswf r1714 source under upstream/gameswf-r1714):
//   0x773d34 tu_mutex::~tu_mutex             gameswf_mutex.h, single-thread branch (TU_CONFIG_LINK_TO_THREAD == 0)
//   0x779034 tesselating_shape::~tesselating_shape   gameswf_shape.h, inline virtual destructor
//   0x7790a0, 0x7790b8 edge::edge()          gameswf_shape.cpp
//   0x7790d0, 0x7791d8 edge::edge(float, float, float, float)  gameswf_shape.cpp (upstream clamp, same compares)
//   0x779388, 0x7793c0 mesh::mesh()          gameswf_shape.cpp (the arrays default-construct to zero)
//   0x77956c, 0x77958c mesh_set::mesh_set()  gameswf_shape.cpp
//   0x77c230, 0x77c544 mesh_set::~mesh_set() gameswf_shape.cpp (implicit member destruction clears m_layers)
//   0x779ff4 path::point_test(float, float)  gameswf_shape.cpp (same algorithm, same float operations)
// Skipped: 0x77a948 (deleting destructor) and 0x779a64 (path copy constructor), both compiler-generated.
//
// Upstream definitions this file replaces (wrapped in #if 0 in gameswf_shape.cpp):
//   mesh::add_triangle, mesh_set::mesh_set(tesselating_shape const*, float).
// edge::edge(float, float, float, float) is not replaced: its binary body matches upstream, so the upstream
// definition stays active and this file does not define it.
//
// The bodies below need the gameswf core headers, which do not build in this tree yet (gameswf_action.h and
// tu_gc_singlethreaded_refcount.h fail on untouched code), so they are written behind VENDOR_GAMESWF_DH2_CORE_HEADERS
// and are not compiled by the default check. No part of this batch runs on its own, so there is no test.
//
// Layouts (32-bit offsets). The DH2 gameswf::array<T> is 16 bytes: buffer +0, size +4, capacity +8, and a flag
// byte at +0x0c that reserve() checks (binary 0x7616b0). The vendored container.h array is 12 bytes with no
// flag, so the flag is not modelled. mesh: m_triangle_strip +0x00, m_triangle_list +0x10 (float, DH2),
// m_triangle_indices +0x20 (DH2). mesh_set: m_error_tolerance +0x00, m_layers +0x04. path: upstream member order.

#ifdef VENDOR_GAMESWF_DH2_CORE_HEADERS
#include "gameswf/gameswf.h"
#include "vendor-gameswf/counted_buffer.hpp"  // DH2 replacement for the #if 0 counted_buffer in gameswf_action.h
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_shape.h"

namespace gameswf {

// 0x7790d0 gameswf::edge::edge(float, float, float, float)  [C2]  upstream, see gameswf_shape.cpp
// 0x7791d8 gameswf::edge::edge(float, float, float, float)  [C1]  upstream, see gameswf_shape.cpp
// Binary per component: __aeabi_fcmpge(v, 0xFF7FFFFF) (v >= -FLT_MAX) and __aeabi_fcmple(v, 0x7F7FFFFF)
// (v <= FLT_MAX), both single precision. The value is stored only when both hold, else 0.0f. FLT_MAX and -FLT_MAX
// are kept; NaN and +/-inf become 0. This is the upstream expression, so no body is written here.

// 0x77b988 gameswf::mesh::add_triangle(float const*)
// PARTIAL: the binary appends six floats to m_triangle_list (+0x10) with the array growth rule inlined: it
// reserves new + new/2 only when new != 0 and new > capacity, and never shrinks. append() goes through resize(),
// which also reallocates when new <= capacity/2. Capacity is private in container.h, so this rule cannot be
// reproduced here. The contents match: six zero-filled slots, then the six floats.
void mesh::add_triangle(const float pts[6]) {
    m_triangle_list.append(pts, 6);
}

// 0x77bf20 gameswf::mesh::~mesh()
// PARTIAL: the binary also has a branch for a negative array size (loops at 0x77bf84, 0x77bfa8 and 0x77bfcc) that
// zeroes elements below the buffer start. Array sizes are never negative, so that branch is not reproduced. The
// rest matches: m_triangle_indices (+0x20), m_triangle_list (+0x10) and m_triangle_strip (+0x00) are each set to
// size 0 and their buffers released, in that order. resize(0) does both.
mesh::~mesh() {
    m_triangle_indices.resize(0);
    m_triangle_list.resize(0);
    m_triangle_strip.resize(0);
}

// 0x77ca1c gameswf::mesh::set_triangles(float const*, int, unsigned short const*, int)
// PARTIAL, as add_triangle: verts are appended to m_triangle_list (+0x10) and indices to m_triangle_indices (+0x20),
// each only when its count is above zero. Contents match (zero-filled slots, then the copy). Only the capacity
// rule differs (see add_triangle). The binary returns its last loop register; the declaration is void.
void mesh::set_triangles(const float* verts, int float_count, const unsigned short* indices, int index_count) {
    if (float_count > 0) {
        m_triangle_list.append(verts, float_count);
    }
    if (index_count > 0) {
        m_triangle_indices.append(indices, index_count);
    }
}

// 0x77a95c gameswf::mesh_set::mesh_set(gameswf::tesselate::tesselating_shape const*, float)  [C1]
// 0x77abcc gameswf::mesh_set::mesh_set(gameswf::tesselate::tesselating_shape const*, float)  [C2]
// PARTIAL: the binary stores the error tolerance, default-constructs m_layers and calls
// gameswf::execute_tesselate(this, sh, err, 0.0), where sh is the shape_character_def that the tesselating_shape
// subobject belongs to (a null argument stays null; the binary subtracts 32 from a non-null one). The static_cast
// below makes the same adjustment. execute_tesselate (0x789d98) is queued in vendor-gameswf but is not in this
// tree yet, so its effect is missing. The binary's fourth argument, 0.0, is not in the callee's three-parameter
// signature and is dropped.
mesh_set::mesh_set(const tesselate::tesselating_shape* sh, float error_tolerance)
    : m_error_tolerance(error_tolerance) {
    execute_tesselate(this, static_cast<const shape_character_def*>(sh), error_tolerance);
}

}  // namespace gameswf

#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS
