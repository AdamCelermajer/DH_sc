// Batch v1-vendor-gameswf-gameswf_edit_text_character-77beb0-0: gameswf::edit_text_character, 15 rows.
//
// Upstream rows (no body here): the upstream definitions in gameswf_text.cpp match the binary.
//   0x78a398 align_line      gameswf_text.cpp:1289
//   0x78b128 show_cursor     gameswf_text.cpp:823
//   0x78dde8 get_root        gameswf_text.cpp:818
//
// Gameloft rows defined below: 0x78a348 hit_test, 0x7914dc init, 0x7916a4 recycle, 0x78ae10 / 0x78ae30
// update_world_*, 0x78c328 / 0x78c410 ~edit_text_character (D1, D2). Only the destructor has an upstream
// definition; it is wrapped in #if 0 in gameswf_text.cpp. The others have no upstream definition in this tree.
//
// Partial rows with no body here: append_image, append_text, preload_glyphs(filter), preload_glyphs().
// They depend on the DH2 glyph and text record layouts, which differ from upstream (see the notes at
// each function and the layout requests in ledger.d). Nothing is guessed for them.
//
// Skipped row: 0x78c3f4, the deleting destructor. The compiler emits it from the class destructor.
//
// DH2 members used here are declared in gameswf_text.h and gameswf_character.h with // DH2: comments.
// Offsets below are the 32-bit offsets from the binary, kept as comments only.

#include "gameswf/gameswf_text.h"

namespace gameswf
{

// 0x78a348 _ZN7gameswf19edit_text_character8hit_testEff
// Override of character::hit_test (vtable +0x6c). The binary calls vtable slot +0x68 with the same
// two floats and returns its result as bool: that slot is edit_text_character::get_topmost_mouse_entity(float, float).
bool edit_text_character::hit_test(float x, float y)
{
	return get_topmost_mouse_entity(x, y);
}

// 0x7914dc _ZN7gameswf19edit_text_character4initEv
// Partial. Everything below follows the Hex-Rays order. Not reproduced: the binary also stores a flag
// byte at this+0x9d (the character base flag area, 157 decimal) equal to (length of get_var_name() > 1).
// No member of that name is declared in gameswf_character.h, so the store is left out.
// The Hex-Rays return value (the get_var_name() reference left in r0) is not a contract; this is void.
void edit_text_character::init()
{
	unk_16c = -1;                       // +0x16c (364), DH2
	unk_168 = 0;                        // +0x168 (360), DH2
	m_has_focus = false;                // +0x14c (332), byte
	m_cursor = 0;                       // +0x150 (336)
	m_xcursor = 0.0f;                   // +0x154 (340)
	m_ycursor = 0.0f;                   // +0x158 (344)
	m_x = 0.0f;                         // +0x15c (348)
	m_y = 0.0f;                         // +0x160 (352)
	m_last_line_start_record = 0;       // +0x164 (356), DH2
	m_color = m_def->m_color;           // +0x170 (368), definition +96
	m_text_height = m_def->m_text_height;   // +0x174 (372), definition +92
	m_font = m_def->m_font;             // +0x178 (376), smart pointer set_ref
	m_alignment = m_def->m_alignment;   // +0x17c (380), definition +104
	m_left_margin = m_def->m_left_margin;   // +0x180 (384), definition +108
	m_right_margin = m_def->m_right_margin; // +0x184 (388), definition +112
	m_indent = m_def->m_indent;         // +0x188 (392), definition +116
	m_leading = m_def->m_leading;       // +0x18c (396), definition +120
	unk_190 = 0.0f;                     // +0x190 (400), DH2
	m_background_color.set(255, 255, 255, 255);  // +0x194 (404), bytes all 0xff

	// Default text, then the variable value: set_text(def->m_default_text, false), set_text_value(to_string(), false).
	set_text(tu_string(m_def->m_default_text.c_str()), false);
	set_text_value(tu_string(to_string()), false);

	// Dummy fill style: resize(0), then push one default fill_style.
	m_dummy_style.resize(0);
	m_dummy_style.push_back(fill_style());
}

// 0x7916a4 _ZN7gameswf19edit_text_character7recycleEPNS_9characterEi
// Partial: the character base recycle runs first, then init(). The init gap (see above) applies here.
void edit_text_character::recycle(character* parent, int id)
{
	character::recycle(parent, id);
	init();
}

// 0x78c328 _ZN7gameswf19edit_text_characterD1Ev
// 0x78c410 _ZN7gameswf19edit_text_characterD2Ev (identical body; one C++ destructor serves both)
// Partial. The binary releases, in order: m_font (+0x178), m_text (tu_string at +0x138, heap buffer when its
// byte 0 is -1), the DH2 render cache at +0xd8..+0x137 (not declared, so not released here),
// m_dummy_line_style (+0xc4), m_dummy_style (+0xb4), m_text_glyph_records (+0xa4) by resize(0) then reserve(0),
// m_def (+0xa0), and then runs character::~character. The members declared in the header are released by
// C++ member destruction after this body. The render cache is the gap.
edit_text_character::~edit_text_character()
{
}

// 0x78ae10 _ZN7gameswf19edit_text_character19update_world_cxformEv
// Partial. Calls the character base update, then sets the two 64-bit words at +0xe0 and +0xd8
// (render cache, unk_e0 and unk_d8) to -1, in that order. The base returns int in the binary (r0 left
// over); this override is void.
void edit_text_character::update_world_cxform()
{
	character::update_world_cxform();
	unk_e0 = -1;    // +0xe0 (224)
	unk_d8 = -1;    // +0xd8 (216)
}

// 0x78ae30 _ZN7gameswf19edit_text_character19update_world_matrixEv
// Partial, same shape as update_world_cxform. The base returns int in the binary, not reproduced here.
void edit_text_character::update_world_matrix()
{
	character::update_world_matrix();
	unk_e0 = -1;    // +0xe0 (224)
	unk_d8 = -1;    // +0xd8 (216)
}

// 0x78b128 _ZN7gameswf19edit_text_character11show_cursorEv
// Upstream: the upstream show_cursor body in gameswf_text.cpp:823 matches (box {x, y, x, y+h},
// world matrix, red line style, 2 px width, draw_line_strip with 2 points). Not defined here.

// 0x78dde8 _ZN7gameswf19edit_text_character8get_rootEv
// Upstream: gameswf_text.cpp:818, get_parent()->get_root(). The dead weak-parent branch in the binary
// releases the proxy and then makes the same null virtual call as upstream. Not defined here.

// 0x78a398 _ZN7gameswf19edit_text_character10align_lineENS_23edit_text_character_def9alignmentEif
// Upstream: gameswf_text.cpp:1289. Same extra_space, WIDTH_FUDGE, LEFT/CENTER/RIGHT split and the
// has_x_offset shift loop. The binary's float return is r0 left over; callers ignore it. Not defined here.

// 0x78a990 _ZN7gameswf19edit_text_character12append_imageERKNS_9tu_stringEii
// Partial, not rebuilt. Binary: the image path. When the owner is a bitmap-capable character it takes its
// bitmap; otherwise it asks the texture-loader callback (s_texture_loader_callback) for an image and makes a
// bitmap through s_render_handler (vtable +0x18). It then appends one bitmap-backed glyph (advance = width
// * 20, in twips) to the text record list and advances m_x (+0x15c) by the same width.
// Gap: the DH2 glyph layout (see append_text) and the render handler slot at vtable +0x18 are not
// reconciled with upstream gameswf::glyph / render_handler.

// 0x78cb90 _ZN7gameswf19edit_text_character11append_textERKNS_9tu_stringERNS0_15text_attributesEb
// Partial, not rebuilt. Binary: the glyph loop. Per character it reads the kerning adjustment, handles
// the newline codes (two byte_9 constants), backspace (byte_8), space (0x20), nbsp (0xa0), and '&'
// (38), where '&' followed by "nbsp;" is read as a space when the flag is set. It looks up the glyph with
// font::get_glyph, wraps the line when the record would pass
// the right margin (WIDTH_FUDGE 80), re-flows the glyph array of the current record, and aligns each line
// through align_line. Gap: text_attributes is not declared; the DH2 glyph (advance at +0, a ref at +4,
// index as a short at +0x1e, code at +0x20) and the DH2 text_glyph_record (glyph array at +32, 48-byte
// stride) differ from upstream gameswf::glyph and text_glyph_record; the missing-glyph log is not reproduced.

// 0x78c0ec _ZN7gameswf19edit_text_character12preload_glyphsEPKNS_6filterE
// Partial, not rebuilt. Binary: for the text record at the given filter, it collects the 16-bit glyph codes
// of that record's glyph array (stride 36, code at +0x20 of each glyph), then calls preload_glyph_codes(font,
// codes, count, font, size, filter). It also releases a weak owner at this+0x2c/+0x30 first. Gap:
// gameswf::filter (44-byte entries) and preload_glyph_codes are absent from upstream r1714; the owner's
// field at +0xac is unresolved.

// 0x78c2d0 _ZN7gameswf19edit_text_character12preload_glyphsEv
// Partial, not rebuilt. Binary: loops over a 44-byte record array at this+0x50 (count at +8, data at +4 of
// that record; the struct is undeclared), calls preload_glyphs(filter) for each entry, then calls it once
// with a null filter. Gap: the struct at +0x50 is not declared.

// 0x78c3f4 _ZN7gameswf19edit_text_characterD0Ev
// Skipped: compiler-generated deleting destructor (call the complete destructor, then operator delete).
// It is emitted by the compiler from the class destructor above, not written by hand.

}  // namespace gameswf
