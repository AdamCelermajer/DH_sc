// Batch v1-vendor-gameswf-gameswf_font-a42405-0: gameswf::font (DH2 bodies).
//
// Batch functions and where each one lives:
//   0x7ce3d0 output_cached_data, 0x7ce3d4 input_cached_data   upstream (gameswf_font.cpp, empty body; the DH2 binary is bx lr)
//   0x7ce628 copy_from                                        DH2 addition; defined below
//   0x7cf228 ~font (D2), 0x7cf314 ~font (D1)                  upstream (gameswf_font.cpp); member destructors match
//   0x7cf41c get_units_per_em, 0x7cf520 get_height            DH2 additions; partial (lookups stubbed)
//   0x7cf8d8 font(player*) (C1), 0x7cf984 font(player*) (C2)  DH2 constructor; defined below (upstream one is #if 0)
//
// DH2 layout of gameswf::font (32-bit offsets, from the constructor and copy_from):
//   +0x00 vptr; the character_def base ends at +0x20 (m_player weak pointer proxy at +0x18, pointer at +0x1c, from get_units_per_em)
//   +0x20 m_glyphs (array: buffer, size, buffer size)        upstream member
//   +0x2c unk_2c (byte, zeroed by the constructor)           DH2 addition, in gameswf_font.h
//   +0x30 m_fontname (tu_string, 20 bytes)                   upstream member
//   +0x44 m_owning_movie                                     upstream member
//   +0x48 m_has_layout (not set by the constructor)          upstream member
//   +0x49 m_unicode_chars, +0x4a m_shift_jis_chars, +0x4b m_ansi_chars,
//   +0x4c m_is_italic, +0x4d m_is_bold, +0x4e m_wide_codes   upstream members
//   +0x50 m_code_table (hash: one pointer)                   upstream member
//   +0x54 m_ascent, +0x58 m_descent, +0x5c m_leading         upstream members
//   +0x60 m_advance_table (array<float>)                     upstream member
//   +0x6c unk_6c (byte, zeroed by the constructor)           DH2 addition, in gameswf_font.h
//   +0x70 m_kerning_pairs (hash: one pointer)                upstream member
//   +0x74 m_table_hint (byte)                                upstream member
//   +0x78 m_zone_table (array<zone_record>)                  upstream member
//   +0x84 unk_84 (byte, zeroed by the constructor)           DH2 addition, in gameswf_font.h
//
// Known difference, kept on purpose: the binary instantiates m_code_table as hash<unsigned short, short>. The
// header keeps upstream's hash<Uint16, int>, because upstream gameswf_font.cpp uses it as int (get_glyph).
// The hash is one pointer either way, so the layout is the same and the bodies below do not depend on the value type.
//
// BUILD STATUS (2026-10-13): check_tu.sh FAILS on this file, as it does on every TU that includes gameswf/gameswf.h
// (the vendor-gameswf/src/v1-vendor-gameswf-gameswf_character_def batch fails the same way). The 12 errors are
// all in base/tu_gc_singlethreaded_refcount.h:120,123,136,139 (gc_ptr<as_object>, gc_ptr<character>, ... cannot
// convert to gc_object_collector_base). None are in this file. Fixing them means changing engine code in the
// gc header, so they are left alone. Checked in a scratch copy of upstream with those four calls cast
// (reinterpret_cast), this file and the edited upstream gameswf_font.cpp compile with no errors under -Wall.
// Copy_from and the two constructors are partial until the gc header builds; get_units_per_em and get_height
// are partial for their stubbed lookups.

#include "gameswf/gameswf_font.h"

namespace gameswf
{

// 0x7ce628 gameswf::font::copy_from(gameswf::font*)   (_ZN7gameswf4font9copy_fromEPS0_)
// Binary: tu_string::operator= (m_fontname), then byte copies of +0x49..+0x4e, dword copies of +0x54, +0x58,
// +0x5c, and a byte copy of +0x74. The binary leaves tu_string::operator='s result in r0. Hex-Rays shows a
// pointer return for that reason; a void function compiles to the same code, so this is void.
void font::copy_from(font* src)
{
	m_fontname = src->m_fontname;	// +0x30 tu_string
	m_unicode_chars = src->m_unicode_chars;	// +0x49
	m_shift_jis_chars = src->m_shift_jis_chars;	// +0x4a
	m_ansi_chars = src->m_ansi_chars;	// +0x4b
	m_is_italic = src->m_is_italic;	// +0x4c
	m_is_bold = src->m_is_bold;	// +0x4d
	m_wide_codes = src->m_wide_codes;	// +0x4e
	m_ascent = src->m_ascent;	// +0x54
	m_descent = src->m_descent;	// +0x58
	m_leading = src->m_leading;	// +0x5c
	m_table_hint = src->m_table_hint;	// +0x74
}

// 0x7cf8d8 gameswf::font::font(gameswf::player*)   (_ZN7gameswf4fontC1EPNS_6playerE)
// 0x7cf984 gameswf::font::font(gameswf::player*)   (_ZN7gameswf4fontC2EPNS_6playerE)
// Binary: character_def(player); vptr; m_glyphs zeroed; unk_2c = 0; m_fontname = "Times New Roman" (the constant is
// in the tu_string(const char*) clone, which is the upstream default); m_has_layout is not written; the
// unicode, shift-jis, ansi (=1), italic, bold and wide-code bytes; m_owning_movie = NULL; m_code_table,
// m_ascent, m_descent, m_leading, m_advance_table, m_kerning_pairs, m_table_hint and m_zone_table zeroed;
// unk_6c and unk_84 = 0. Upstream sets the same members except the three DH2 bytes.
font::font(player* p) :
	character_def(p),
	m_fontname("Times New Roman"),	// default
	m_owning_movie(NULL),
	m_unicode_chars(false),
	m_shift_jis_chars(false),
	m_ansi_chars(true),
	m_is_italic(false),
	m_is_bold(false),
	m_wide_codes(false),
	m_ascent(0.0f),
	m_descent(0.0f),
	m_leading(0.0f),
	m_table_hint(0)
{
	unk_2c = 0;	// +0x2c
	unk_6c = 0;	// +0x6c
	unk_84 = 0;	// +0x84
}

// 0x7cf41c gameswf::font::get_units_per_em() const   (_ZNK7gameswf4font16get_units_per_emEv)
// PARTIAL: the lookups are stubbed, so the result is always 1.0f. Hex-Rays flow:
//   1. Inlined weak-pointer check on m_player (as character_def::get_player). A dead proxy is dropped
//      and cleared, and the binary then reads the render object without a NULL test. Not reproduced.
//   2. render = player+0x0ac; bgp = render+0x10 (bitmap_glyph_provider*). If
//      bgp->get_font_entity(m_fontname, m_is_bold, m_is_italic) is not NULL, return 1024.0f.
//   3. gp = render+0x0c (glyph_provider*). fe = gp->get_face_entity(m_fontname, m_is_bold, m_is_italic).
//      If fe is not NULL, return (float) units_per_EM, the u16 at +0x44 of the FT_Face that fe holds at +0x24.
//   4. Otherwise return 1.0f.
// Missing: the player+0x0ac render object layout, the bitmap_glyph_provider and glyph_provider::get_face_entity
// (Gameloft, not in the tree), and the face_entity layout. The FreeType offsets are the 32-bit FT_FaceRec
// layout and were not checked against a FreeType header.
float font::get_units_per_em() const
{
	return 1.0f;
}

// 0x7cf520 gameswf::font::get_height() const   (_ZNK7gameswf4font10get_heightEv)
// PARTIAL: the lookups are stubbed, so the result is always 0.0f. Hex-Rays flow, with the same two lookups:
//   1. bgp->get_font_entity(m_fontname, m_is_bold, m_is_italic) not NULL: return fe->float(+0x28) * 20.0f.
//   2. gp->get_face_entity(...) not NULL: return (float)(FT_Face ascender at +0x46 - FT_Face descender at +0x48).
//   3. Otherwise return 0.0f.
// Missing: the same types as get_units_per_em (see above).
float font::get_height() const
{
	return 0.0f;
}

// 0x7ce3d0 gameswf::font::output_cached_data(gameswf::tu_file*, gameswf::cache_options const&)
// 0x7ce3d4 gameswf::font::input_cached_data(gameswf::tu_file*)
// Both are upstream functions (gameswf_font.cpp). The upstream bodies are compiled out (#if 0, "@@ Disabled"),
// and the DH2 binary is bx lr for both, so they are not redefined here.

// The destructors (0x7cf228, 0x7cf314) are upstream: font::~font() { m_glyphs.resize(0); } plus the implicit
// member destructors, which run in the binary's order (zone table, kerning hash, advance table, code table,
// m_fontname, m_glyphs), then the base destructor. Not redefined here.

}	// end namespace gameswf
