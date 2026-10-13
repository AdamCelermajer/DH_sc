// Batch v1-vendor-gameswf-gameswf-fddef6-2 (vendor-gameswf, owner gameswf, part 3 of 8).
//
// gameswf r1714 with Gameloft changes, as built into libDungeonHunter2.so (32-bit ARM).
// Bodies follow the Hex-Rays pseudocode. Per-function status and notes are in
// ledger.d/v1-vendor-gameswf-gameswf-fddef6-2.csv.
//
// Globals defined here have external linkage, as in the binary. The matching upstream
// statics in gameswf_movie_def.cpp and gameswf_player.cpp are now extern declarations.
//
// BUILD STATUS (fix pass, 2026-10-13): check_tu.sh FAILS on this file, and on every other TU that
// includes gameswf/gameswf.h. The batch's own code has no errors. Every error that remains comes
// from the vendored tree, and the integrator owns those fixes:
//   1. gameswf/gameswf.h:49 (DECLARE_GC_TYPES(tu_gc::singlethreaded_refcount)) instantiates
//      gc_ptr<as_object> and gc_ptr<character>. base/tu_gc_singlethreaded_refcount.h:120,123
//      then needs a gc_object_collector_base, but as_object_interface derives from ref_counted
//      (gameswf/gameswf.h:225). Pre-existing; game-app
//      g1-game-app-global-f216b6-21_gameswf.cpp fails the same way.
//   2. gameswf/gameswf_abc.h:224: abc_def::get_super_class reads instance_info::m_super_name,
//      which the tree's instance_info does not declare.
// The earlier blockers (bitmap_info duplicate declarations, action_buffer, traits_info) no longer
// reproduce. Every row stays partial until the headers build.

#include <cstddef>
#include <cstdint>

#include "base/container.h"
#include "base/tu_file.h"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_value.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_object.h"
#include "gameswf/gameswf_as_sprite.h"
#include "gameswf/gameswf_as_classes/as_boolean.h"
#include "gameswf/gameswf_as_classes/as_number.h"
#include "gameswf/gameswf_as_classes/as_string.h"
#include "glitch-video/vertex_streams.hpp"
#include "vendor-gameswf/gameswf_dh2_helpers.hpp"

namespace gameswf {

// Native handlers that no r1714 header declares. The DH2 binary defines them in Gameloft
// sources (as_array_reverse, get_textformat and sprite_instance_at_depth have no r1714 body).
void as_s_function_call(const fn_call& fn);
void as_array_tostring(const fn_call& fn);
void as_array_push(const fn_call& fn);
void as_array_pop(const fn_call& fn);
void as_array_concat(const fn_call& fn);
void as_array_join(const fn_call& fn);
void as_array_reverse(const fn_call& fn);
void as_array_sort(const fn_call& fn);
void as_array_shift(const fn_call& fn);
void as_array_unshift(const fn_call& fn);
void as_array_slice(const fn_call& fn);
void as_array_splice(const fn_call& fn);
void as_array_length(const fn_call& fn);
void set_textformat(const fn_call& fn);
void get_textformat(const fn_call& fn);
void sprite_instance_at_depth(const fn_call& fn);

// Upstream gameswf_movie_def.cpp: static hash<int, loader_function> s_tag_loaders. Shared
// by is_tag_loaders_initialized() and get_tag_loader() here.
hash<int, loader_function> s_tag_loaders;

// Upstream gameswf_player.cpp: static stringi_hash<as_standard_member>
// s_standard_property_map. Filled lazily by get_standard_member() and cleared by
// clear_standard_property_map().
stringi_hash<as_standard_member> s_standard_property_map;

// Forward declaration for the template instances below. The primary templates have no
// generic body, only the explicit specializations in this file.
template <typename T>
void write_le(tu_file* out, T value);
template <typename T>
T read_le(tu_file* in);
template <typename T>
void collect(const unsigned int* indices, unsigned int count,
             const glitch::video::SVertexStream& stream, point* out);

namespace {

// Adds a native method to a builtin method table (the hash<tu_stringi, as_value>::add call
// plus as_value::set_as_c_function and drop_refs in the binary).
void add_c_function(stringi_hash<as_value>* map, const char* name, as_c_function_ptr handler)
{
	as_value value;
	value.set_as_c_function(handler);
	map->add(tu_stringi(name), value);
}

// Adds a getter-only property such as String.length or Array.length. The binary builds a getter
// as_value (set_as_c_function), a default setter (undefined), then as_value(getter, setter)
// (0x769540), and adds that value (hash add). The temporaries are dropped in reverse order.
void add_getter_property(stringi_hash<as_value>* map, const char* name, as_c_function_ptr getter)
{
	as_value getter_value;
	getter_value.set_as_c_function(getter);
	as_value setter_value;
	map->add(tu_stringi(name), as_value(getter_value, setter_value));
}

struct standard_member_name
{
	const char* name;
	int id;  // binary value of as_standard_member
};

// Standard property names in the binary's insertion order. The ids are the DH2 numbering.
// It differs from the r1714 enum in gameswf_action.h: htmlText (23) and textHeight (25) are
// absent there, and ids 32 to 45 are shifted. See the notes in the ledger.
const standard_member_name kStandardMembers[] = {
	{"_x", 0},
	{"_y", 1},
	{"_xscale", 2},
	{"_yscale", 3},
	{"_currentframe", 4},
	{"_totalframes", 5},
	{"_alpha", 6},
	{"_visible", 7},
	{"_width", 8},
	{"_height", 9},
	{"_rotation", 10},
	{"_target", 11},
	{"_framesloaded", 12},
	{"_name", 13},
	{"_droptarget", 14},
	{"_url", 15},
	{"_highquality", 16},
	{"_focusrect", 17},
	{"_soundbuftime", 18},
	{"_xmouse", 19},
	{"_ymouse", 20},
	{"_parent", 21},
	{"text", 22},
	{"htmlText", 23},
	{"textWidth", 24},
	{"textHeight", 25},
	{"textColor", 26},
	{"border", 27},
	{"multiline", 28},
	{"wordWrap", 29},
	{"type", 30},
	{"backgroundColor", 31},
	{"_this", 32},
	{"this", 33},
	{"_root", 34},
	{".", 35},
	{"..", 36},
	{"_level0", 37},
	{"_global", 38},
	{"Stage", 39},
	{"enabled", 40},
	{"transform", 41},
	{"matrix", 42},
	{"concatenatedMatrix", 43},
	{"colorTransform", 44},
	{"concatenatedColorTransform", 45},
};

// Body of gameswf::collect<T>. The binary maps the stream's buffer, walks the vertices, then
// releases the map inline (the same count and callback logic as IBuffer::unmap, 0x637a68).
template <typename T>
void collect_stream(const unsigned int* indices, unsigned int count,
                    const glitch::video::SVertexStream& stream, point* out)
{
	// The binary calls IBuffer::map(1) with R1 = E_BUFFER_READ_MAP_ACCESS through the const
	// method (0x5a1adc). stream.buffer.operator-> returns a non-const IBuffer*, which would pick
	// the non-const map (0x5a19f0) instead, so go through a const pointer.
	const glitch::video::IBuffer* buffer = stream.buffer.get();
	std::uint8_t* mapped = buffer->map(glitch::video::E_BUFFER_READ_MAP_ACCESS(1));
	const std::uint8_t* base = mapped ? mapped + stream.offset : nullptr;
	dh2::collect_triangle_points<T>(base, stream.field0e, indices, count,
	                                reinterpret_cast<float*>(out));
	if (base)
	{
		// IBuffer::unmap (glitch-video buffer.cpp) calls unmapImpl() (vtable +0x18) on the last
		// release when flag 0x20 is set, as the binary does.
		stream.buffer->unmap();
	}
}

}  // namespace

// Standard method tables (BUILTIN ids 0 to 4 are upstream; 5 to 7 are binary-only).
// The r1714 builtin_object enum and the array sized by BUILTIN_COUNT stop at 4. Upstream
// BUILTIN_COUNT (gameswf_action.h) has to grow to 8 before ids 5 to 7 are safe.
namespace {
inline builtin_object builtin_id(int id)
{
	return static_cast<builtin_object>(id);
}
}  // namespace

// 0x764c5c bool gameswf::is_tag_loaders_initialized()
bool is_tag_loaders_initialized()
{
	// Reserve room for max(count, 96) entries. set_capacity computes the raw size as
	// 3/2 of that, which is the binary's (3 * v2) >> 1, and uses 144 when the table is
	// absent. Then report whether any loader is registered.
	int count = s_tag_loaders.size();
	s_tag_loaders.set_capacity(count < 96 ? 96 : count);
	return s_tag_loaders.size() > 0;
}

// 0x7652d0 bool gameswf::get_tag_loader(int, void (**)(gameswf::stream*, int, gameswf::movie_definition_sub*))
bool get_tag_loader(int tag_type, loader_function* lf)
{
	return s_tag_loaders.get(tag_type, lf);
}

// 0x76c768 gameswf::get_standard_property_map()
stringi_hash<as_standard_member>* get_standard_property_map()
{
	return &s_standard_property_map;
}

// 0x76c870 void gameswf::destruct<gameswf::permanent_string>(gameswf::permanent_string const*)
// SKIPPED: permanent_string is not declared in r1714 or in this tree. The body (free the
// heap buffer when the flag byte is 0xff, then free the object) needs its layout.
// 0x76c9ac void gameswf::destruct<gameswf::bitmap_glyph_provider>(gameswf::bitmap_glyph_provider const*)
// SKIPPED: bitmap_glyph_provider is not declared in r1714 or in this tree. The binary calls
// vtable slot 0 and then frees the object. Whether slot 0 is the deleting destructor is not
// established, so the template cannot be written yet.

// 0x76d594 gameswf::tu_string* gameswf::get_static_string(gameswf::tu_string const&)
// PARTIAL: the binary returns permanent_string_cache::get(&s_static_strings, name). The
// cache and its global are Gameloft types with no definition in r1714 or this tree.
tu_string* get_static_string(const tu_string& name)
{
	(void)name;
	return nullptr;
}

// 0x76ef5c void gameswf::register_as_native_function(char const*, void (*)(gameswf::fn_call const&))
void register_as_native_function(const char* name, as_c_function_ptr handler)
{
	stringi_hash<as_value>* map = new_standard_method_map(BUILTIN_OBJECT_METHOD);
	tu_stringi key(name);
	as_value value;
	value.set_as_c_function(handler);
	map->set(key, value);
}

// 0x76f5c0 void gameswf::standard_method_map_init()
void standard_method_map_init()
{
	// as_object builtins
	stringi_hash<as_value>* v1 = new_standard_method_map(BUILTIN_OBJECT_METHOD);
	add_c_function(v1, "addProperty", as_object_addproperty);
	add_c_function(v1, "registerClass", as_object_registerclass);
	add_c_function(v1, "hasOwnProperty", as_object_hasownproperty);
	add_c_function(v1, "watch", as_object_watch);
	add_c_function(v1, "unwatch", as_object_unwatch);
	add_c_function(v1, "addEventListener", as_object_add_event_listener);

	// as_number builtins
	stringi_hash<as_value>* v2 = new_standard_method_map(BUILTIN_NUMBER_METHOD);
	add_c_function(v2, "toString", as_number_to_string);
	add_c_function(v2, "valueOf", as_number_valueof);

	// as_boolean builtins
	stringi_hash<as_value>* v3 = new_standard_method_map(BUILTIN_BOOLEAN_METHOD);
	add_c_function(v3, "toString", as_boolean_to_string);
	add_c_function(v3, "valueOf", as_boolean_valueof);

	// as_string builtins
	stringi_hash<as_value>* v4 = new_standard_method_map(BUILTIN_STRING_METHOD);
	add_c_function(v4, "toString", string_to_string);
	add_c_function(v4, "fromCharCode", string_from_char_code);
	add_c_function(v4, "charCodeAt", string_char_code_at);
	add_c_function(v4, "concat", string_concat);
	add_c_function(v4, "indexOf", string_index_of);
	add_c_function(v4, "lastIndexOf", string_last_index_of);
	add_c_function(v4, "slice", string_slice);
	add_c_function(v4, "split", string_split);
	add_c_function(v4, "substring", string_substring);
	add_c_function(v4, "substr", string_substr);
	add_c_function(v4, "toLowerCase", string_to_lowercase);
	add_c_function(v4, "toUpperCase", string_to_uppercase);
	add_c_function(v4, "charAt", string_char_at);
	add_getter_property(v4, "length", string_length);

	// as_s_function builtins (binary id 5)
	stringi_hash<as_value>* v5 = new_standard_method_map(builtin_id(5));
	add_c_function(v5, "call", as_s_function_call);

	// sprite_instance builtins
	stringi_hash<as_value>* v6 = new_standard_method_map(BUILTIN_SPRITE_METHOD);
	add_c_function(v6, "play", sprite_play);
	add_c_function(v6, "stop", sprite_stop);
	add_c_function(v6, "gotoAndStop", sprite_goto_and_stop);
	add_c_function(v6, "gotoAndPlay", sprite_goto_and_play);
	add_c_function(v6, "nextFrame", sprite_next_frame);
	add_c_function(v6, "prevFrame", sprite_prev_frame);
	add_c_function(v6, "getBytesLoaded", sprite_get_bytes_loaded);
	add_c_function(v6, "getBytesTotal", sprite_get_bytes_total);
	add_c_function(v6, "swapDepths", sprite_swap_depths);
	add_c_function(v6, "duplicateMovieClip", sprite_duplicate_movieclip);
	add_c_function(v6, "getDepth", sprite_get_depth);
	add_c_function(v6, "createEmptyMovieClip", sprite_create_empty_movieclip);
	add_c_function(v6, "removeMovieClip", sprite_remove_movieclip);
	add_c_function(v6, "hitTest", sprite_hit_test);
	add_c_function(v6, "getBounds", sprite_get_bounds);
	add_c_function(v6, "localToGlobal", sprite_local_global);
	add_c_function(v6, "globalToLocal", sprite_global_local);
	add_c_function(v6, "startDrag", sprite_start_drag);
	add_c_function(v6, "stopDrag", sprite_stop_drag);
	add_c_function(v6, "loadMovie", sprite_loadmovie);
	add_c_function(v6, "unloadMovie", sprite_unloadmovie);
	add_c_function(v6, "getNextHighestDepth", sprite_getnexthighestdepth);
	add_c_function(v6, "getInstanceAtDepth", sprite_instance_at_depth);
	add_c_function(v6, "createTextField", sprite_create_text_field);
	add_c_function(v6, "attachMovie", sprite_attach_movie);
	add_c_function(v6, "beginFill", sprite_begin_fill);
	add_c_function(v6, "endFill", sprite_end_fill);
	add_c_function(v6, "lineTo", sprite_line_to);
	add_c_function(v6, "moveTo", sprite_move_to);
	add_c_function(v6, "curveTo", sprite_curve_to);
	add_c_function(v6, "clear", sprite_clear);
	add_c_function(v6, "lineStyle", sprite_line_style);
	add_c_function(v6, "setFPS", sprite_set_fps);
	add_c_function(v6, "addFrameScript", sprite_add_script);

	// text field builtins (binary id 6)
	stringi_hash<as_value>* v7 = new_standard_method_map(builtin_id(6));
	add_c_function(v7, "setTextFormat", set_textformat);
	add_c_function(v7, "getTextFormat", get_textformat);

	// array builtins (binary id 7)
	stringi_hash<as_value>* v8 = new_standard_method_map(builtin_id(7));
	add_c_function(v8, "toString", as_array_tostring);
	add_c_function(v8, "push", as_array_push);
	add_c_function(v8, "pop", as_array_pop);
	add_c_function(v8, "concat", as_array_concat);
	add_c_function(v8, "join", as_array_join);
	add_c_function(v8, "reverse", as_array_reverse);
	add_c_function(v8, "sort", as_array_sort);
	add_c_function(v8, "shift", as_array_shift);
	add_c_function(v8, "unshift", as_array_unshift);
	add_c_function(v8, "slice", as_array_slice);
	add_c_function(v8, "splice", as_array_splice);
	add_getter_property(v8, "length", as_array_length);
}

// 0x772030 gameswf::as_standard_member gameswf::get_standard_member(gameswf::tu_stringi const&)
as_standard_member get_standard_member(const tu_stringi& name)
{
	if (s_standard_property_map.size() == 0)
	{
		const int count = static_cast<int>(sizeof(kStandardMembers) / sizeof(kStandardMembers[0]));
		// 46 entries at 3/2 is the binary's set_raw_capacity(69).
		s_standard_property_map.set_capacity(count);
		for (const standard_member_name& m : kStandardMembers)
		{
			s_standard_property_map.add(tu_stringi(m.name), static_cast<as_standard_member>(m.id));
		}
		if (s_standard_property_map.size() == 0)
		{
			return M_INVALID_MEMBER;
		}
	}

	as_standard_member result = M_INVALID_MEMBER;
	s_standard_property_map.get(name, &result);
	return result;
}

// 0x7775f4 void gameswf::collect<signed char>(unsigned int const*, unsigned int, glitch::video::SVertexStream const&, gameswf::point*)
template <>
void collect<signed char>(const unsigned int* indices, unsigned int count,
                          const glitch::video::SVertexStream& stream, point* out)
{
	collect_stream<signed char>(indices, count, stream, out);
}

// 0x7777c4 void gameswf::collect<unsigned int>(unsigned int const*, unsigned int, glitch::video::SVertexStream const&, gameswf::point*)
template <>
void collect<unsigned int>(const unsigned int* indices, unsigned int count,
                           const glitch::video::SVertexStream& stream, point* out)
{
	collect_stream<unsigned int>(indices, count, stream, out);
}

// 0x777994 void gameswf::collect<float>(unsigned int const*, unsigned int, glitch::video::SVertexStream const&, gameswf::point*)
template <>
void collect<float>(const unsigned int* indices, unsigned int count,
                    const glitch::video::SVertexStream& stream, point* out)
{
	collect_stream<float>(indices, count, stream, out);
}

// 0x777b1c void gameswf::collect<unsigned char>(unsigned int const*, unsigned int, glitch::video::SVertexStream const&, gameswf::point*)
template <>
void collect<unsigned char>(const unsigned int* indices, unsigned int count,
                            const glitch::video::SVertexStream& stream, point* out)
{
	collect_stream<unsigned char>(indices, count, stream, out);
}

// 0x777cec void gameswf::collect<short>(unsigned int const*, unsigned int, glitch::video::SVertexStream const&, gameswf::point*)
template <>
void collect<short>(const unsigned int* indices, unsigned int count,
                    const glitch::video::SVertexStream& stream, point* out)
{
	collect_stream<short>(indices, count, stream, out);
}

// 0x777ebc void gameswf::collect<unsigned short>(unsigned int const*, unsigned int, glitch::video::SVertexStream const&, gameswf::point*)
template <>
void collect<unsigned short>(const unsigned int* indices, unsigned int count,
                             const glitch::video::SVertexStream& stream, point* out)
{
	collect_stream<unsigned short>(indices, count, stream, out);
}

// 0x77808c void gameswf::collect<int>(unsigned int const*, unsigned int, glitch::video::SVertexStream const&, gameswf::point*)
template <>
void collect<int>(const unsigned int* indices, unsigned int count,
                  const glitch::video::SVertexStream& stream, point* out)
{
	collect_stream<int>(indices, count, stream, out);
}

// 0x77932c void gameswf::write_le<short>(gameswf::tu_file*, short)
template <>
void write_le<short>(tu_file* out, short value)
{
	// The binary passes the two little-endian bytes straight to the file's write callback.
	out->write_bytes(&value, static_cast<int>(sizeof(value)));
}

// 0x77935c short gameswf::read_le<short>(gameswf::tu_file*)
template <>
short read_le<short>(tu_file* in)
{
	std::uint16_t v = 0;
	in->read_bytes(&v, static_cast<int>(sizeof(v)));
	return static_cast<short>(v);
}

// 0x7797e4 int gameswf::sort_by_decreasing_error(void const*, void const*)
// Used as the qsort comparator in shape_character_def::sort_and_clean_meshes(). The binary's
// body (fcmplt, then fcmpgt) is the same as the upstream static in gameswf_shape.cpp, which is
// #if 0 there because the shape_character_def batch calls this external definition.
int sort_by_decreasing_error(const void* A, const void* B)
{
	// Each element is a mesh_set*. The error tolerance is its first member (+0x00).
	const float* a = *static_cast<const float* const*>(A);
	const float* b = *static_cast<const float* const*>(B);
	return dh2::compare_decreasing_error(a, b);
}

}  // namespace gameswf
