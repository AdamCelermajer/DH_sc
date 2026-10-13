// Batch v1-vendor-gameswf-gameswf_shape_character_def-7e2b97-0 (vendor-gameswf, owner gameswf::shape_character_def).
//
// Eight binary functions: the constructor (C1 0x77b878, C2 0x77b900), the destructor (D2 0x77c570,
// D1 0x77c754), sort_and_clean_meshes (0x779fd0), point_test_local (0x77a570), the five-argument
// display (0x77a990) and create_character_instance (0x77be00). Per-function status and notes are in
// ledger.d/v1-vendor-gameswf-gameswf_shape_character_def-7e2b97-0.csv.
//
// Upstream, no body here: sort_and_clean_meshes and point_test_local. They match gameswf r1714
// gameswf_shape.cpp. The sort calls sort_by_decreasing_error, which fddef6-2 defines (upstream declares it
// just before shape_character_def::sort_and_clean_meshes). point_test_local is rect::point_test on m_bound,
// then the path loop.
//
// DH2 bodies in this file, ledgered partial: the constructor, the destructor, display (five arguments,
// no blend mode) and create_character_instance. They are written but not compiled. The upstream gameswf
// headers do not build in this tree (as_value.hpp and gameswf.h DECLARE_GC_TYPES), so these bodies sit
// behind VENDOR_GAMESWF_DH2_CORE_HEADERS, as in fddef6-1. Their arithmetic and selection rules are in
// vendor-gameswf/gameswf_shape_dh2_helpers.hpp and tested by tests/v1-vendor-gameswf-gameswf_shape_character_def-7e2b97-0_test.cpp.
//
// Where the DH2 bodies differ from upstream gameswf_shape.cpp:
//   - constructor: the binary stores a zero byte at +0x0c of each of its four arrays (fill, line, path and
//     cached meshes), so the array layout is 16 bytes, not the 12 of the upstream container.h (layout request).
//     It also does not write m_bound or m_edge_bounds, where upstream rect() zeroes them.
//   - destructor: each cached mesh is freed with gameswf::free_internal, as the binary does. Upstream uses delete.
//   - display: no bitmap_blend_mode argument. mesh_set::display is the four-argument DH2 overload, and the
//     new mesh is allocated with operator new(size, MemHint) (binary: operator new(0x14, 0)).
//   - create_character_instance: the player makes the instance through its pool, not with new.
//
// The upstream definitions this file replaces (constructor and destructor) are wrapped in #if 0 in
// upstream/gameswf-r1714/gameswf/gameswf_shape.cpp.

#include <new>

#include "vendor-gameswf/gameswf_alloc.hpp"            // gameswf::free_internal
#include "vendor-gameswf/gameswf_shape_dh2_helpers.hpp"  // selection and scale arithmetic

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include "core-platform/deps/swf_types.hpp"            // swf::MemHint
#include "core-platform/memory.hpp"                    // operator new(std::size_t, swf::MemHint)
#include "gameswf/gameswf.h"                           // get_curve_max_pixel_error()
#include "gameswf/gameswf_impl.h"                      // generic_character
#include "gameswf/gameswf_player.h"                    // player::create_generic_character
#include "gameswf/gameswf_shape.h"

namespace gameswf {

// 0x77b878 gameswf::shape_character_def::shape_character_def(gameswf::player*)   [C1, complete object]
// 0x77b900 gameswf::shape_character_def::shape_character_def(gameswf::player*)   [C2, base object]
// PARTIAL: not compiled (see the file header). Member initialisation matches upstream. The binary also
// writes a zero byte at +0x0c of each array (no member declared for it here; layout request) and leaves
// m_bound and m_edge_bounds unwritten. The rect default constructor used here does write them.
// Binary order (for reference): character_def(player), the byte at +0x84, the vptrs at +0 and +0x20,
// then the arrays at +0x24, +0x34, +0x44 and +0x78 cleared, and the flags at +0x74 and +0x75.
shape_character_def::shape_character_def(player* p)
	: character_def(p),
	  m_uses_nonscaling_strokes(false),
	  m_uses_scaling_strokes(false)
{
}

// 0x77c570 gameswf::shape_character_def::~shape_character_def()   [D2, base object]
// 0x77c754 gameswf::shape_character_def::~shape_character_def()   [D1, complete object]
// PARTIAL: not compiled (see the file header). The explicit part frees each cached mesh_set with
// ~mesh_set and gameswf::free_internal (the binary passes one argument; the size is ignored, so 0 is passed).
// The compiler then destroys the members in reverse declaration order, as the binary does:
// m_cached_meshes (size 0, buffer freed), m_paths (resize 0 each path, buffer freed), m_line_styles,
// m_fill_styles. Then the tesselating_shape subobject, then character_def::~character_def.
// The binary also has a branch for a negative size. A size is never negative, so it is not reproduced.
shape_character_def::~shape_character_def()
{
	for (int i = 0; i < m_cached_meshes.size(); ++i)
	{
		mesh_set* v = m_cached_meshes[i];
		if (v != nullptr)
		{
			v->~mesh_set();
			free_internal(v, 0);
		}
	}
}

// 0x77a990 gameswf::shape_character_def::display(gameswf::matrix const&, gameswf::cxform const&, float,
//          gameswf::array<gameswf::fill_style> const&, gameswf::array<gameswf::line_style> const&) const
// PARTIAL: not compiled (see the file header). A DH2 overload; the six-argument upstream display stays.
// Binary steps: scale from the basis (inlined get_max_scale), stop if |scale| < 1e-6, the error
// ((20 / scale) / pixel_scale) * curve error, reuse a cached mesh_set if one fits (display it and return),
// else build one at 0.75 of the error, add it to the cache, display it and sort the cache.
void shape_character_def::display(const matrix& mat, const cxform& cx, float pixel_scale,
                                  const array<fill_style>& fill_styles,
                                  const array<line_style>& line_styles) const
{
	const float max_scale =
		dh2::shape_matrix_max_scale(mat.m_[0][0], mat.m_[0][1], mat.m_[1][0], mat.m_[1][1]);
	if (!dh2::shape_scale_is_drawable(max_scale))
	{
		return;
	}

	const float object_space_error =
		dh2::shape_object_space_error(max_scale, pixel_scale, get_curve_max_pixel_error());
	const int hit = dh2::shape_select_cached_mesh(
		m_cached_meshes.size(), object_space_error,
		[this](int i) { return m_cached_meshes[i]->get_error_tolerance(); });
	if (hit >= 0)
	{
		m_cached_meshes[hit]->display(mat, cx, fill_styles, line_styles);
		return;
	}

	// The binary calls operator new(0x14, 0) and then the constructor, with no null check.
	void* mem = operator new(sizeof(mesh_set), static_cast<swf::MemHint>(0));
	mesh_set* m = new (mem) mesh_set(this, dh2::shape_new_mesh_tolerance(object_space_error));
	m_cached_meshes.push_back(m);
	m->display(mat, cx, fill_styles, line_styles);
	sort_and_clean_meshes();
}

// 0x77be00 gameswf::shape_character_def::create_character_instance(gameswf::character*, int)
// PARTIAL: not compiled (see the file header). Overrides character_def::create_character_instance, whose
// upstream body is new generic_character. Binary: the inlined weak-pointer read of m_player (a dead proxy
// is dropped, the pointer and proxy are cleared, NULL is returned), then the result of
// player::create_generic_character(def, parent, id) on that player is returned. The binary makes the call even when
// the player is NULL, and so does this.
character* shape_character_def::create_character_instance(character* parent, int id)
{
	return get_player()->create_generic_character(this, parent, id);
}

}  // namespace gameswf

#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS

// 0x779fd0 gameswf::shape_character_def::sort_and_clean_meshes() const
//   upstream gameswf_shape.cpp, shape_character_def::sort_and_clean_meshes. Binary: if the cache is not empty (size > 0), qsort it with
//   sort_by_decreasing_error, four bytes per element. The upstream debug-only assert loop is not in the binary.
//
// 0x77a570 gameswf::shape_character_def::point_test_local(float, float)
//   upstream gameswf_shape.cpp, shape_character_def::point_test_local. Binary: the m_bound early-out (rect::point_test, inlined), then
//   path::point_test over m_paths (40-byte stride), return 1 on the first hit. An empty path list returns 0
//   before the loop, which is the same result as the upstream loop.
