// Batch v1-vendor-gameswf-pack0-157e56-0 (vendor-gameswf, gameswf r1714 with Gameloft changes).
//
// Compiled by default:
//   matrix::concatenate, get_x_scale, set_lerp, transform(rect*), set_inverse  (DH2 bodies; the r1714
//       definitions in upstream/gameswf-r1714/gameswf/gameswf_types.cpp are wrapped in #if 0, and
//       set_lerp and set_inverse return bool in gameswf.h)
//   smart_ptr_proxy::operator=(weak_proxy*)              (include/vendor-gameswf/smart_ptr_proxy.hpp)
//   permanent_string::~permanent_string()                (include/vendor-gameswf/permanent_string.hpp)
//   filter_texture_cache::get_region_size_requirement_cellsize (include/vendor-gameswf/filter_texture_cache.hpp)
//
// Behind VENDOR_GAMESWF_DH2_CORE_HEADERS (PARTIAL, not compiled by check_tu.sh): gameswf_character.h
//   and gameswf_dlist.h do not compile in the tree (base/tu_gc_singlethreaded_refcount.h:120, as_object
//   is not a gc_object_collector_base). character::custom ctor and dtor, display_list::remove_display_object.
//
// No body (PARTIAL, layout or type missing; see the notes in the ledger):
//   display_list::construct, display_list::~display_list, hash<stringi_pointer,...>::set_raw_capacity,
//   weak_ptr<character>::get_ptr, filter_texture_cache::get_character_region.
//
// Skipped (compiler clones, body identical to a function in this batch or in another batch):
//   0x43c404, 0x7c1330 smart_ptr_proxy::operator=(weak_proxy*) [clone] (argument folded to nullptr)
//   0x421f74, 0x42da98 weak_ptr<character>::operator=(character*) [clone] (argument folded to nullptr)

#include <cfloat>
#include <cmath>
#include <cstdint>

#include "gameswf/gameswf.h"
#include "vendor-gameswf/filter_texture_cache.hpp"
#include "vendor-gameswf/gameswf_alloc.hpp"
#include "vendor-gameswf/permanent_string.hpp"
#include "vendor-gameswf/smart_ptr_proxy.hpp"

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_dlist.h"
#endif

namespace gameswf {
namespace {

// The binary's range check on float results: __aeabi_fcmpge against -FLT_MAX (0xFF7FFFFF), then
// __aeabi_fcmple against +FLT_MAX (0x7F7FFFFF). A value outside [-FLT_MAX, +FLT_MAX] becomes 0. NaN also
// becomes 0, because the fcmpge is false for NaN. This is the test tu_float::operator= applies in gameswf.h.
inline bool in_range(float v)
{
	return v >= -FLT_MAX && v <= FLT_MAX;
}

inline float range_or_zero(float v)
{
	return in_range(v) ? v : 0.0f;
}

}  // namespace

// 0x4165b8 gameswf::matrix::concatenate(gameswf::matrix const&)
// Binary: m happens first, then *this. Each result is range-checked, then all six are stored.
// Result order in the binary: r00, r10, r01, r11, r02, r12.
void matrix::concatenate(const matrix& m)
{
	const float a00 = m_[0][0], a01 = m_[0][1], a02 = m_[0][2];
	const float a10 = m_[1][0], a11 = m_[1][1], a12 = m_[1][2];
	const float b00 = m.m_[0][0], b01 = m.m_[0][1], b02 = m.m_[0][2];
	const float b10 = m.m_[1][0], b11 = m.m_[1][1], b12 = m.m_[1][2];

	const float r00 = range_or_zero((a00 * b00) + (a01 * b10));
	const float r10 = range_or_zero((a10 * b00) + (a11 * b10));
	const float r01 = range_or_zero((a00 * b01) + (a01 * b11));
	const float r11 = range_or_zero((a10 * b01) + (a11 * b11));
	const float r02 = range_or_zero(((a00 * b02) + (a01 * b12)) + a02);
	const float r12 = range_or_zero(((a10 * b02) + (a11 * b12)) + a12);

	m_[0][0] = r00;
	m_[1][0] = r10;
	m_[0][1] = r01;
	m_[1][1] = r11;
	m_[0][2] = r02;
	m_[1][2] = r12;
}

// 0x75329c gameswf::matrix::get_x_scale() const
// Binary: sqrt(m00^2 + m01^2), the norm of row 0, negated when the determinant is below zero. The r1714
// source uses column 0 (m00, m10) and differs.
float matrix::get_x_scale() const
{
	const float m00 = m_[0][0], m01 = m_[0][1];
	const float m10 = m_[1][0], m11 = m_[1][1];

	float scale = std::sqrt((m00 * m00) + (m01 * m01));
	if (((m00 * m11) - (m10 * m01)) < 0.0f)
	{
		scale = -scale;
	}
	return scale;
}

// 0x7947b8 gameswf::matrix::set_lerp(gameswf::matrix const&, gameswf::matrix const&, float)
// Each element is a + t * (b - a), range-checked. Returns whether the last element (m12, before the
// check) is in range. Every element reads only its own index, so the reads happen before the stores.
bool matrix::set_lerp(const matrix& m1, const matrix& m2, float t)
{
	const float v00 = m1.m_[0][0] + (t * (m2.m_[0][0] - m1.m_[0][0]));
	const float v10 = m1.m_[1][0] + (t * (m2.m_[1][0] - m1.m_[1][0]));
	const float v01 = m1.m_[0][1] + (t * (m2.m_[0][1] - m1.m_[0][1]));
	const float v11 = m1.m_[1][1] + (t * (m2.m_[1][1] - m1.m_[1][1]));
	const float v02 = m1.m_[0][2] + (t * (m2.m_[0][2] - m1.m_[0][2]));
	const float v12 = m1.m_[1][2] + (t * (m2.m_[1][2] - m1.m_[1][2]));

	m_[0][0] = range_or_zero(v00);
	m_[1][0] = range_or_zero(v10);
	m_[0][1] = range_or_zero(v01);
	m_[1][1] = range_or_zero(v11);
	m_[0][2] = range_or_zero(v02);
	m_[1][2] = range_or_zero(v12);

	return in_range(v12);
}

// 0x794a14 gameswf::matrix::transform(gameswf::rect*) const
// Replaces the rect with the box of its four transformed corners. Corners in the binary's order: A (x0,y0),
// B (x1,y0), C (x0,y1), D (x1,y1). The binary's last compare (max y of A, B, D < y of C) is left in r0 and
// is not a designed return, so the function is void (as in the r1714 source).
void matrix::transform(rect* bound) const
{
	const float m00 = m_[0][0], m01 = m_[0][1], m02 = m_[0][2];
	const float m10 = m_[1][0], m11 = m_[1][1], m12 = m_[1][2];

	const float x0 = bound->m_x_min, x1 = bound->m_x_max;
	const float y0 = bound->m_y_min, y1 = bound->m_y_max;

	const float xA = ((x0 * m00) + (y0 * m01)) + m02;
	const float yA = ((x0 * m10) + (y0 * m11)) + m12;
	const float xB = ((x1 * m00) + (y0 * m01)) + m02;
	const float yB = ((x1 * m10) + (y0 * m11)) + m12;
	const float xC = ((x0 * m00) + (y1 * m01)) + m02;
	const float yC = ((x0 * m10) + (y1 * m11)) + m12;
	const float xD = ((x1 * m00) + (y1 * m01)) + m02;
	const float yD = ((x1 * m10) + (y1 * m11)) + m12;

	// Box of A and B.
	float mnx = (xA < xB) ? xA : xB;
	float mxx = (xA < xB) ? xB : xA;
	float mny = (yA < yB) ? yA : yB;
	float mxy = (yA < yB) ? yB : yA;

	// Fold in D, then C, with the binary's compare order.
	mnx = (mnx >= xD) ? xD : mnx;
	mny = (mny >= yD) ? yD : mny;
	mxx = (mxx >= xD) ? mxx : xD;
	mxy = (mxy >= yD) ? mxy : yD;
	mnx = (mnx >= xC) ? xC : mnx;
	mny = (mny >= yC) ? yC : mny;
	mxx = (mxx >= xC) ? mxx : xC;
	mxy = (mxy >= yC) ? mxy : yC;

	bound->m_x_min = mnx;
	bound->m_x_max = mxx;
	bound->m_y_min = mny;
	bound->m_y_max = mxy;
}

// 0x795adc gameswf::matrix::set_inverse(gameswf::matrix const&)
// Inverse of [[m00 m01 m02] [m10 m11 m12]]. A zero determinant does not give the identity: the result is
// [[1 0 -m02] [0 1 -m12]]. Otherwise the inverse is computed with the range-checked intermediates the
// binary uses. Returns whether the last element (before the check) is in range.
bool matrix::set_inverse(const matrix& m)
{
	const float m00 = m.m_[0][0], m01 = m.m_[0][1], m02 = m.m_[0][2];
	const float m10 = m.m_[1][0], m11 = m.m_[1][1], m12 = m.m_[1][2];

	const float det = (m11 * m00) - (m01 * m10);
	if (det == 0.0f)
	{
		const float v02 = -m02;
		const float v12 = -m12;
		m_[0][0] = 1.0f;
		m_[0][1] = 0.0f;
		m_[1][0] = 0.0f;
		m_[1][1] = 1.0f;
		m_[0][2] = range_or_zero(v02);
		m_[1][2] = range_or_zero(v12);
		return in_range(v12);
	}

	const float inv = 1.0f / det;
	const float v00 = range_or_zero(inv * m11);     // m11 / det
	const float v11 = range_or_zero(inv * m00);     // m00 / det
	const float v01 = range_or_zero((-m01) * inv);  // -m01 / det
	const float v10 = range_or_zero((-m10) * inv);  // -m10 / det
	const float v02 = range_or_zero(-((v00 * m02) + (v01 * m12)));
	const float v12 = -((v10 * m02) + (v11 * m12));

	m_[0][0] = v00;
	m_[1][1] = v11;
	m_[0][1] = v01;
	m_[1][0] = v10;
	m_[0][2] = v02;
	m_[1][2] = range_or_zero(v12);
	return in_range(v12);
}

// 0x41fe84 gameswf::smart_ptr_proxy::operator=(gameswf::weak_proxy*)
// The DH2 weak_proxy count is the int at +0. Release at zero is free_internal, not delete.
void smart_ptr_proxy::operator=(weak_proxy* p)
{
	weak_proxy* old = m_proxy;
	if (p != old)
	{
		if (old != nullptr)
		{
			if (old->m_ref_count-- == 1)
			{
				free_internal(old, 0);
			}
		}
		m_proxy = p;
		if (p != nullptr)
		{
			++p->m_ref_count;
		}
	}
}

// 0x752e64 gameswf::permanent_string::~permanent_string()
// Frees the buffer at +0x0c only when the flag byte at +0x00 is 0xff.
permanent_string::~permanent_string()
{
	if (unk_0x00 == -1)
	{
		free_internal(unk_0x0c, 0);
	}
}

// 0x756cdc gameswf::filter_texture_cache::get_region_size_requirement_cellsize(int&, int&)
// Both sizes are rounded up to whole 16-pixel cells, minimum 16. Returns the remainder of the original
// height (the binary computes it before the height is stored).
int filter_texture_cache::get_region_size_requirement_cellsize(int& width, int& height)
{
	int cells = width / 16;
	if (width % 16 > 0)
	{
		++cells;
	}
	int w = 16 * cells;
	if (w < 16)
	{
		w = 16;
	}
	width = w;

	const int remainder = height % 16;
	int cells_h = height / 16;
	if (remainder > 0)
	{
		++cells_h;
	}
	int h = 16 * cells_h;
	if (h < 16)
	{
		h = 16;
	}
	height = h;

	// The binary repeats the minimum clamp on both sizes. It is a no-op after the rounding above.
	if (width < 16)
	{
		width = 16;
	}
	if (height < 16)
	{
		height = 16;
	}
	return remainder;
}

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
// The bodies below need gameswf_character.h and gameswf_dlist.h, which do not compile in the tree
// yet. They are PARTIAL until those headers build.

// 0x412160 gameswf::character::custom::custom()
// PARTIAL: the identity colour transform and matrix are written as the binary does. The high byte of
// the word at +0x5c is not reproduced: the binary keeps the top byte of the old contents (masked with
// 0xfe into byte +0x5f), and a constructor cannot read it. The top byte is written as 0 here.
character::custom::custom()
{
	for (int i = 0; i < 8; i += 2)
	{
		unk_0x00[i] = 1.0f;
		unk_0x00[i + 1] = 0.0f;
	}
	m_matrix.m_[0][0] = 1.0f;
	m_matrix.m_[0][1] = 0.0f;
	m_matrix.m_[0][2] = 0.0f;
	m_matrix.m_[1][0] = 0.0f;
	m_matrix.m_[1][1] = 1.0f;
	m_matrix.m_[1][2] = 0.0f;
	unk_0x38 = 0;
	unk_0x3c = nullptr;
	unk_0x40 = 0;
	unk_0x44 = 0;
	unk_0x48 = 0;
	unk_0x4c = 1;
	unk_0x4d = 0;
	unk_0x5c = 0x00ffffffu;  // PARTIAL: top byte, see above
	unk_0x60 = 0;
	unk_0x64 = 0;
	unk_0x68 = 0;
}

// 0x7533c0 gameswf::character::custom::~custom()
// PARTIAL: the binary loops over the array<filter> at +0x3c when its count is negative (signed compare),
// zeroing 44-byte filter entries, then calls array<filter>::reserve(0). filter is not declared, so neither
// step is written here.
character::custom::~custom()
{
	if (unk_0x4c == 0xff)
	{
		free_internal(unk_0x58, 0);
	}
	unk_0x40 = 0;
}

// 0x756564 gameswf::display_list::remove_display_object(gameswf::character*)
// PARTIAL: the binary returns the index from get_character_by_ptr when it is negative (an int function
// in Hex-Rays), and otherwise the r0 of the tail call to remove(). Upstream remove is void, so the
// function is void here and the negative-index return value is dropped.
void display_list::remove_display_object(character* ch)
{
	const int index = get_character_by_ptr(ch);
	if (index >= 0)
	{
		remove(index);
	}
}
#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS

}  // namespace gameswf
