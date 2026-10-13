// Batch v1-vendor-gameswf-gameswf-fddef6-3 (vendor-gameswf, owner gameswf, part 4 of 8).
//
// gameswf r1714 with Gameloft changes, as built into libDungeonHunter2.so (32-bit ARM).
// Per-function status and notes are in ledger.d/v1-vendor-gameswf-gameswf-fddef6-3.csv.
//
// Compile scope. The ungated part (write_le<float>, read_le<float>, as_boolean_valueof, removeHTML)
// needs only base/container.h and base/tu_file.h, and is checked by check_tu.sh. Everything else is
// behind VENDOR_GAMESWF_DH2_CORE_HEADERS, as in the pack7 batch: gameswf/gameswf.h does not build in
// this tree (base/tu_gc_singlethreaded_refcount.h, gameswf_action.h), so those bodies are written but
// not compiled. The ADPCM decoder is in vendor-gameswf/adpcm_dh2.hpp and is compiled and tested on its own.
//
// Upstream definitions replaced by this file (each wrapped in #if 0 in the upstream source):
//   gameswf_as_classes/as_boolean.cpp  as_boolean_valueof (binary is empty, upstream asserts)
//   gameswf_as_classes/as_key.cpp      key_init (binary presizes the member hash, see 0x79f3c4)
//   gameswf_sound.cpp                  define_sound_loader (binary decodes ADPCM inline; upstream
//                                      uses adpcm_expand, and the binary has no byte swap)
//   gameswf_shape.cpp                  write_le<float>, read_le<float> (binary writes and reads the raw
//                                      4 bytes through the file callbacks; upstream calls write_float32/read_float32)
// Upstream bodies that match the binary are not repeated here and are ledgered upstream: broadcaster_init,
// flash_init, geom_init, as_global_get_version, math_floor and math_fabs.

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "base/container.h"   // tu_string (global scope in upstream)
#include "base/tu_file.h"     // tu_file
#include "vendor-gameswf/adpcm_dh2.hpp"

#ifdef VENDOR_GAMESWF_DH2_CORE_HEADERS
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_stream.h"
#include "gameswf/gameswf_font.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_shape.h"
#include "gameswf/gameswf_object.h"
#include "gameswf/gameswf_as_classes/as_key.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include "gameswf/gameswf_as_classes/as_global.h"
#include "gameswf/gameswf_as_classes/as_math.h"
#include "gameswf/gameswf_as_classes/as_boolean.h"
#include "vendor-gameswf/as_value.hpp"
#include "vendor-gameswf/tesselator_accepter.hpp"
#endif

namespace gameswf {

// ---------------------------------------------------------------------------------------------
// Ungated: compiled by check_tu.sh.
// ---------------------------------------------------------------------------------------------

// Declared for the specializations below (the primary templates have no generic body).
template <typename T>
void write_le(::tu_file* out, T value);
template <typename T>
T read_le(::tu_file* in);

// 0x77b59c void gameswf::write_le<float>(gameswf::tu_file*, float)
template <>
void write_le<float>(::tu_file* out, float value)
{
	// The binary passes the 4 bytes of the float straight to the file's write callback, as
	// (*(out + 12))(&value, 4, *out). The header's write_bytes() calls the same callback (m_write, which the
	// DH2 layout places at +0x0c; base/tu_file.h notes the extra field). Host order, as the binary.
	out->write_bytes(&value, static_cast<int>(sizeof(value)));
}

// 0x77b84c float gameswf::read_le<float>(gameswf::tu_file*)
template <>
float read_le<float>(::tu_file* in)
{
	// The binary reads 4 bytes into an int and returns it. The float return is the same 32 bits.
	float value = 0.0f;
	in->read_bytes(&value, static_cast<int>(sizeof(value)));
	return value;
}

// 0x79b298 gameswf::as_boolean_valueof(gameswf::fn_call const&)
// The binary is a bare return. The upstream body (gameswf_as_classes/as_boolean.cpp) is assert(0) and is
// #if 0 there. The fn_call type is only named here, so this does not need the gameswf core headers.
struct fn_call;
void as_boolean_valueof(const fn_call& fn)
{
	(void)fn;
}

// 0x78b4d8 void gameswf::removeHTML(gameswf::tu_string&)
// Keeps the text between the nearest '>' before the first "</" and that "</". Binary (checked in the
// disassembly):
//   - empty string: unchanged.
//   - no "</": unchanged (strstr returns null).
//   - the backward scan from the '<' of "</" compares the pointer to the string start at each step, but
//     both pointers move together, so the bound never ends the loop. The binary reads below the string
//     when no '>' precedes "</". That is undefined; this body stops at the start of the string and leaves
//     the string unchanged. Partial for that case only.
//   - the first test of the byte at the '<' against '>' is dead (it is '<'), and is not written.
//   - text length zero: the string becomes "".
// The binary's tu_string layout (+0 length-or-0xFF, +1 inline text, +0x0c heap text) is handled by
// length() and c_str(), so this follows the upstream interface.
void removeHTML(tu_string& text)
{
	const int length = text.length();
	if (length <= 0) {
		return;
	}
	const char* start = text.c_str();
	const char* open_close = std::strstr(start, "</");
	if (open_close == nullptr) {
		return;
	}

	const char* gt = open_close - 1;
	while (gt >= start && *gt != '>') {
		--gt;
	}
	if (gt < start) {
		return;  // binary: undefined (no bound on the scan); see the comment above
	}

	const char* after = gt + 1;
	const int count = static_cast<int>(open_close - after);
	if (count <= 0) {
		text = "";
	} else {
		const std::string kept(after, static_cast<std::size_t>(count));
		text = kept.c_str();
	}
}

}  // namespace gameswf

// ---------------------------------------------------------------------------------------------
// Gated: VENDOR_GAMESWF_DH2_CORE_HEADERS. Not compiled in this tree (see the header comment).
// ---------------------------------------------------------------------------------------------
#ifdef VENDOR_GAMESWF_DH2_CORE_HEADERS

namespace gameswf {

// External to this batch. The DH2 sound handler pointer is gameswf_sound.cpp's static (local in the binary).
extern sound_handler* s_sound_handler;
int get_sample_rate(int index);
void as_global_array_ctor(const fn_call& fn);
void as_global_textformat_ctor(const fn_call& fn);
void as_global_point_ctor(const fn_call& fn);
void as_global_matrix_ctor(const fn_call& fn);
void as_global_transform_ctor(const fn_call& fn);
void as_global_color_transform_ctor(const fn_call& fn);
void as_broadcaster_initialize(const fn_call& fn);
void key_add_listener(const fn_call& fn);
void key_get_ascii(const fn_call& fn);
void key_get_code(const fn_call& fn);
void key_is_down(const fn_call& fn);
void key_is_toggled(const fn_call& fn);
void key_remove_listener(const fn_call& fn);

// 0x77d0d0 void gameswf::define_sound_loader(gameswf::stream*, int, gameswf::movie_definition_sub*)
// DH2 DefineSound (tag 14). Header fields are read even when no sound handler is set, as the binary does.
// ADPCM (format 1) is decoded here and handed to the handler as native 16-bit (format 7). Format 3 with
// 16-bit samples is also sent as format 7, with no byte swap (the binary is little-endian).
// Partial: the sound_sample object is built as the binary does (player pointer from m_player, then
// add_sound_sample), but the weak-pointer release that the binary inlines before the call is
// represented by get_player(). The sound_sample layout (0x24 bytes, vtable off_990668) is not declared here.
void define_sound_loader(stream* in, int tag_type, movie_definition_sub* m)
{
	(void)tag_type;
	const int character_id = in->read_u16();
	int format = in->read_uint(4);
	const int rate_code = in->read_uint(2);
	const bool sample_16bit = in->read_uint(1) != 0;
	const bool stereo = in->read_uint(1) != 0;
	const int sample_count = static_cast<int>(in->read_u32());

	if (!s_sound_handler) {
		return;
	}

	std::vector<unsigned char> data;
	int data_bytes = 0;
	if (format == 1) {
		// ADPCM: sample_count frames, two bytes per sample, two samples per frame in stereo.
		std::vector<std::int16_t> samples(static_cast<std::size_t>(sample_count) * (stereo ? 2u : 1u));
		dh2::adpcm_expand(*in, samples.data(), sample_count, stereo);
		data_bytes = static_cast<int>(samples.size() * sizeof(std::int16_t));
		data.resize(static_cast<std::size_t>(data_bytes));
		if (data_bytes > 0) {
			std::memcpy(data.data(), samples.data(), static_cast<std::size_t>(data_bytes));
		}
		format = 7;  // FORMAT_NATIVE16
	} else {
		const int tag_bytes = in->get_tag_end_position() - in->get_position();
		data_bytes = tag_bytes > 0 ? tag_bytes : 0;
		data.resize(static_cast<std::size_t>(data_bytes));
		for (int i = 0; i < data_bytes; ++i) {
			data[static_cast<std::size_t>(i)] = in->read_u8();
		}
		if (format == 3 && sample_16bit) {
			format = 7;  // FORMAT_NATIVE16
		}
	}

	const int handler_id = s_sound_handler->create_sound(
		data.data(), data_bytes, sample_count,
		static_cast<sound_handler::format_type>(format),
		get_sample_rate(rate_code), stereo);

	sound_sample* sam = new sound_sample(m->get_player(), handler_id);
	m->add_sound_sample(character_id, sam);
}

// 0x789d98 void gameswf::execute_tesselate(gameswf::mesh_set*, gameswf::shape_character_def const*, float)
// Partial. Written: the two tesselator accepters (strip/direct flags 0,0,0 and 0,1,0) and the new_layer()
// call. Missing: the two final_shape arrays (fill styles of the shape with keep flag 0 and style = index,
// line styles with keep flag 1; the array sizes come from shape +40 and +56, not resolved here), and the
// per-path loop (fills at +0/+4, line style at +8, start point at +12/+16, edge list at +20 with count at
// +24, and a flag at +76) and its flushes. The path element stride in the loop is 40 but the flag is read
// at +76, which does not fit a 40-byte element, so the path layout cannot be confirmed from this function
// alone. Tolerance: the binary uses 1.0f when err <= 0.
void execute_tesselate(mesh_set* ms, const shape_character_def* sh, float error_tolerance)
{
	tesselator_accepter fill_tess(false, false, false);
	tesselator_accepter line_tess(false, true, false);

	(void)sh;
	const float tolerance_fill = error_tolerance > 0.0f ? error_tolerance : 1.0f;
	(void)tolerance_fill;  // used by the missing loop (see the comment above)

	ms->new_layer();
	// Missing: the two final_shape arrays, the path loop and the flush calls. See the comment above.
	(void)line_tess;
}

// 0x78b240 int gameswf::preload_glyph_codes(gameswf::player_context*, unsigned short*, int, gameswf::font*, int, gameswf::filter const*)
// Partial. Flow is as in the binary: the glyph is fetched (font::get_glyph); when the glyph's height matches the
// texture cache's height, the glyph goes to glyph_texture_cache::get_glyph_region with a 3-byte filter record
// (filter type 2 uses max(+32, +36) as a size, type 0 and 1 use the two floats), otherwise to
// bitmap_glyph_texture_cache::get_glyph_region. Not confirmed: the glyph record (the binary's glyph starts with
// a float width of 512.0 and then a pointer at +4 that is dropped at the end, which does not match the upstream
// glyph struct), and the player_context offsets (+12, +16, +40, +52). Those are not written here.
int preload_glyph_codes(player_context* ctx, std::uint16_t* codes, int count, font* f, int size, const filter* flt)
{
	(void)ctx;
	(void)codes;
	(void)f;
	(void)size;
	(void)flt;
	return count <= 0 ? 0 : count;  // not reconstructed beyond the count result; see the comment above
}

// 0x791d48 void gameswf::get_textformat(gameswf::fn_call const&)
// Partial. Creates the TextFormat object in fn.result (as_global_textformat_ctor), then sets its members from the
// text format record of this_ptr: leftMargin, indent, rightMargin, leading, letterSpacing (twips / 20), color (the
// four bytes at +0x170 packed as b370 | b369 << 8 | b368 << 16 | b371 << 24), size (twips / 20), align
// (1 right, 2 center, 3 justify, otherwise left), font (name from the font record at +0x178, +0x30), bold (font
// record +0x4d) and italic (+0x4c). The record offsets are the binary's; the header that declares them is
// not in this tree, so the body is not written here. The align and font strings come from the player's
// permanent_string_cache through a weak player pointer, which is not declared either.
void get_textformat(const fn_call& fn)
{
	(void)fn;
}

// 0x798204 void gameswf::as_array_reverse(gameswf::fn_call const&)
// Reverses the array of this_ptr in place: swaps element i with element n-1-i for i < n/2, through a temporary
// as_value (the binary's three operator= calls). Partial only in the member name: the array is read at +0x4c
// (buffer) and +0x50 (size), which is the as_array array member.
void as_array_reverse(const fn_call& fn)
{
	as_array* arr = cast_to<as_array>(fn.this_ptr);
	const int n = arr->m_array.size();
	for (int i = 0; i < n / 2; ++i) {
		as_value tmp = arr->m_array[i];
		arr->m_array[i] = arr->m_array[n - 1 - i];
		arr->m_array[n - 1 - i] = tmp;
	}
}

// 0x798e44 as_c_function* gameswf::array_init(gameswf::player*)
// Creates the Array constructor (0x40 bytes, as_global_array_ctor) with the sort-option constants:
// CASEINSENSITIVE 1, DESCENDING 2, UNIQUESORT 4, RETURNINDEXEDARRAY 8, NUMERIC 16 (all numbers).
// Matches the binary's double constants (1.0, 2.0, 4.0, 8.0, 16.0).
as_c_function* array_init(player* p)
{
	as_c_function* ctor = new as_c_function(p, as_global_array_ctor);
	static const struct {
		const char* name;
		double value;
	} kConstants[] = {
		{"CASEINSENSITIVE", 1.0},
		{"DESCENDING", 2.0},
		{"UNIQUESORT", 4.0},
		{"RETURNINDEXEDARRAY", 8.0},
		{"NUMERIC", 16.0},
	};
	for (const auto& c : kConstants) {
		as_value v;
		v.set_double(c.value);
		ctor->builtin_member(tu_string(c.name), v);
	}
	return ctor;
}

// 0x79e700 void gameswf::as_global_get_active_controller(gameswf::fn_call const&)
// Partial. The binary checks the environment's player weak pointer (env +0x64 proxy, +0x68 pointer), takes
// player::get_root() of it, and returns the int at root +0x50 as a number. The env and root members are not
// declared in the vendored headers under these names, so they appear as unk_ placeholders. The field at root
// +0x50 is unnamed (controller id or count; not identified).
void as_global_get_active_controller(const fn_call& fn)
{
	// env +0x64 is the weak proxy (check_proxy), env +0x68 the player pointer.
	player* p = fn.env->unk_104;
	const auto* root = player::get_root(p);
	fn.result->set_double(static_cast<double>(root->unk_50));   // root +0x50, int
}

// 0x79f3c4 as_key* gameswf::key_init(gameswf::player*)
// Upstream gameswf_as_classes/as_key.cpp:107 is #if 0 for this body. Same members as upstream, in binary order:
// the 18 key codes (BACKSPACE 8, CAPSLOCK 20, CONTROL 17, DELETEKEY 46, DOWN 40, END 35, ENTER 13, ESCAPE 27,
// HOME 36, INSERT 45, LEFT 37, PGDN 33, PGUP 34, RIGHT 39, SHIFT 16, SPACE 32, TAB 9, UP 38), then the six
// methods. The binary also presizes the member hash (set_raw_capacity: max(size, 32) * 3 / 2, or 48 when
// empty) before the constants. That size field is not declared, so the presize is not written. No visible
// effect; the constants and methods are exact.
as_key* key_init(player* p)
{
	as_key* key_obj = new as_key(p);

	static const struct {
		const char* name;
		double code;
	} kKeyCodes[] = {
		{"BACKSPACE", 8.0},  {"CAPSLOCK", 20.0}, {"CONTROL", 17.0},  {"DELETEKEY", 46.0}, {"DOWN", 40.0},
		{"END", 35.0},       {"ENTER", 13.0},    {"ESCAPE", 27.0},   {"HOME", 36.0},      {"INSERT", 45.0},
		{"LEFT", 37.0},      {"PGDN", 33.0},     {"PGUP", 34.0},     {"RIGHT", 39.0},    {"SHIFT", 16.0},
		{"SPACE", 32.0},     {"TAB", 9.0},       {"UP", 38.0},
	};
	for (const auto& k : kKeyCodes) {
		as_value v;
		v.set_double(k.code);
		key_obj->set_member(tu_string(k.name), v);
	}

	struct {
		const char* name;
		as_c_function_ptr fn;
	} const kMethods[] = {
		{"addListener", key_add_listener},  {"getAscii", key_get_ascii},     {"getCode", key_get_code},
		{"isDown", key_is_down},            {"isToggled", key_is_toggled},   {"removeListener", key_remove_listener},
	};
	for (const auto& m : kMethods) {
		as_value v;
		v.set_as_c_function(m.fn);
		key_obj->builtin_member(tu_string(m.name), v);
	}
	return key_obj;
}

// 0x7a01d8 void gameswf::math_min(gameswf::fn_call const&)
// DH2 addition (the r1714 two-argument math functions are #if 0). Reads arg(0) then arg(1) (env bottom
// first_arg_bottom_index, then one below), and returns the smaller. NaN in arg(0) is kept, as the binary's
// compare does.
void math_min(const fn_call& fn)
{
	double a = fn.arg(0).to_number();
	const double b = fn.arg(1).to_number();
	if (a >= b) {
		a = b;
	}
	fn.result->set_double(a);
}

// 0x7a0254 void gameswf::math_max(gameswf::fn_call const&)
// DH2 addition, same argument order as math_min. Returns the larger.
void math_max(const fn_call& fn)
{
	double a = fn.arg(0).to_number();
	const double b = fn.arg(1).to_number();
	if (a <= b) {
		a = b;
	}
	fn.result->set_double(a);
}

}  // namespace gameswf

#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS
