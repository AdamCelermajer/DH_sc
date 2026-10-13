// gameswf::root as the DH2 binary has it (Gameloft). This replaces the upstream struct in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_root.h, which is wrapped in #if 0.
//
// The binary's layout differs from r1714 from +0x24 on (display bounds, pixel scale, drag state,
// buffered text, listener blocks). Offsets below are the binary's (+0xNN). Fields that no batch has
// named yet stay as pad_<hex> blocks; nothing here is a stand-in for another class.
//
// Owner batch for the first 15 functions of gameswf::root (0x773dc0 .. 0x775f70):
//   vendor-gameswf/src/v1-vendor-gameswf-gameswf_root-d24935-0.cpp
//
// This header declares every gameswf::root member the binary has (the queue rows 0x773d48 .. 0x775f70).
// Parameter types follow the binary's mangled names, and return types follow the upstream r1714
// declarations (Hex-Rays shows int for the void setters, which return this in r0 and whose callers do
// not use it). A declared member without a definition yet is not an error: its definition belongs to
// the batch named in the queue. Not root members in the binary, so not declared here: is_avm2 (no
// symbol, no queue row), and set_drag_state and do_mouse_drag (character methods, 0x752d0c, 0x75e3a8).
//
// Virtual slots (vptr offsets, from vtables-001.json, _ZTVN7gameswf4rootE): +0x00 D1, +0x04 D0,
// +0x08 get_mouse_state, +0x0c get_pixel_scale, +0x10 has_looped, +0x14 goto_labeled_frame,
// +0x18 set_play_state, +0x1c get_play_state, +0x20 set_variable(char const*, char const*),
// +0x24 set_variable(char const*, wchar_t const*), +0x28 get_variable, +0x2c call_method(varargs),
// +0x30 call_method_args, +0x34 call_method(as_value*, int), +0x38 set_visible, +0x3c get_visible,
// +0x40 get_userdata, +0x44 set_userdata, +0x48 attach_display_callback, +0x4c get_movie_version,
// +0x50 get_movie_width, +0x54 get_movie_height, +0x58 get_movie_fps.

#pragma once

#include <cstdint>
#include <stdarg.h>  // va_list (the binary's call_method_args takes std::__va_list)

// Includes stop before the upstream character, player, movie_def and action headers: those fail
// standalone in the DH2 build (gc_ptr<as_object> instantiation, see the batch notes). Types they
// define are forward-declared below, so this header compiles on its own.
#include "base/smart_ptr.h"    // smart_ptr<T> (upstream)
#include "base/weak_ptr.h"     // weak_ptr<T> (upstream): proxy at +0x00, cached pointer at +0x04
#include "base/container.h"    // array<T>: data, size, buffer size (12 bytes)
#include "gameswf/gameswf.h"   // rect, point, key, player, character, movie_def_impl (forward), render_handler (upstream)
#include "gameswf/gameswf_types.h"   // rgba (upstream)
#include "vendor-gameswf/ref_counted.hpp"  // DH2 ref_counted (12 bytes)

// DH2: the core headers switch. Defined for a build where gameswf_character.h, gameswf_player.h and
// gameswf_movie_def.h compile (they do not in the current tree, see the batch notes). Only the two
// play-state members below depend on it: they name character::play_state, a nested enum.
#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include "gameswf/gameswf_character.h"  // character::play_state (upstream)
#endif

namespace gameswf {

// DH2: global render handler (binary gameswf::s_render_handler, one word in .bss). Defined by
// another vendor-gameswf batch.
extern render_handler* s_render_handler;

// DH2: true when the render handler's orientation (slot +0xac, get_orientation()) is 0 or 2. The DH2
// render_handler is not declared in upstream gameswf.h (its vtable differs), so the value is read through
// this function. Defined by the render handler batch; not in this batch.
namespace dh2 {
bool render_landscape();
}  // namespace dh2

// DH2: scale modes of set_display_bounds. Values 1 and 2 are the only ones the batch reads;
// the enumerators are not recovered (same treatment as bitmap_info::filter_mode).
enum scale_mode : int {};

// Forward declaration: the upstream mouse_button_state (gameswf_root.h) holds gc_ptr<character> members,
// so only the pointer parameter of generate_mouse_button_events is declared here.
struct mouse_button_state;

struct root : public ref_counted {
	smart_ptr<movie_def_impl>	m_def;				// +0x0c
	smart_ptr<character>		m_movie;			// +0x10
	int		m_viewport_x0;						// +0x14 (ctor 0)
	int		m_viewport_y0;						// +0x18 (ctor 0)
	int		m_viewport_width;					// +0x1c (ctor 1)
	int		m_viewport_height;					// +0x20 (ctor 1)
	int		m_display_x;						// +0x24 set_display_bounds: a2 (not set by ctor)
	int		m_display_y;						// +0x28 set_display_bounds: a3
	int		m_display_width;					// +0x2c set_display_bounds: a4
	int		m_display_height;					// +0x30 set_display_bounds: a5
	float	m_pixel_scale;						// +0x34 (ctor 1.0), max(w/frame_w, h/frame_h)
	rgba	m_background_color;					// +0x38 (ctor 0,0,0,255)
	int		m_mouse_x;							// +0x3c notify_mouse_state (ctor 0)
	int		m_mouse_y;							// +0x40 notify_mouse_state (ctor 0)
	int		m_mouse_buttons;					// +0x44 notify_mouse_state (ctor 0)
	uint8_t	pad_48[0x10] = {};					// +0x48..+0x57: not named (ctor zeroes them)

	// Drag state (start_drag writes +0x58..+0x6c; stop_drag and do_mouse_drag not in this batch).
	character*	m_drag_character;				// +0x58 raw pointer, start_drag adds no reference
	uint8_t		m_drag_flag_5c = 0;				// +0x5c cleared by start_drag
	bool		m_drag_lock_center = false;		// +0x5d start_drag: a3
	bool		m_drag_bound = false;			// +0x5e start_drag: a4
	uint8_t		pad_5f[1] = {};					// +0x5f
	float		m_drag_bound_x_min = 0.0f;		// +0x60 start_drag: rect[0]
	float		m_drag_bound_y_min = 0.0f;		// +0x64 start_drag: rect[2]
	float		m_drag_bound_x_max = 1.0f;		// +0x68 start_drag: rect[1] (ctor 1.0)
	float		m_drag_bound_y_max = 1.0f;		// +0x6c start_drag: rect[3] (ctor 1.0)

	uint8_t		pad_70[8] = {};					// +0x70..+0x77 (ctor zeroes them)
	ref_counted*	m_ref_78 = nullptr;			// +0x78 released by ~root (drop_ref); not named yet
	ref_counted*	m_ref_7c = nullptr;			// +0x7c released by ~root (drop_ref); not named yet
	uint8_t		pad_80[4] = {};					// +0x80..+0x83 (ctor zeroes them)
	bool		m_on_event_load_called = false;	// +0x84 advance(): event 10 sent once
	bool		m_shift_key_state = false;		// +0x85
	bool		m_flushing_buffered_text = false;	// +0x86 set during flush_buffered_text
	uint8_t		pad_87[1] = {};					// +0x87

	smart_ptr<character>	m_current_active_entity;	// +0x88 set_active_entity
	float	m_time_remainder = 1.0f;			// +0x8c advance() accumulator (ctor 1.0)
	float	m_frame_time = 1.0f;				// +0x90 1 / frame rate (ctor sets it)
	float	m_garbage_timer = 0.0f;				// +0x94 advance(): counts down, reset to 2.0

	array<smart_ptr<character> >	m_buffered_text;	// +0x98 flush_buffered_text (12 bytes)
	uint8_t		pad_a4[4] = {};					// +0xa4..+0xa7
	uint8_t		m_keypress_listener_block[16] = {};	// +0xa8 listener block (array + bool); not a listener object yet
	uint8_t		m_listener_block[16] = {};			// +0xb8 listener block (array + bool)
	weak_ptr<player>	m_player;				// +0xc8 proxy at +0xc8, cached player at +0xcc

	root(player* player, movie_def_impl* def);
	virtual ~root();

	// The 15 functions of the owner batch (0x773dc0 .. 0x775f70).
	float	screen_to_logical(point& p);		// 0x773dc0
	float	logical_to_screen(point& p);		// 0x773f50
	void	notify_mouse_state(int x, int y, int buttons);	// 0x774128
	void	end_display();						// 0x774298
	void	notify_key_event(player* player, key::code k, bool down);	// 0x774544
	void	flush_buffered_text();				// 0x7748b8
	void	set_active_entity(character* ch);	// 0x774918
	void	start_drag(character* ch, bool lock_center, bool bound, rect& r);	// 0x775154
	void	begin_display();					// 0x7751bc
	void	advance(float delta_time, bool fixed_step);	// 0x775304
	void	set_display_bounds(int x, int y, int w, int h, scale_mode mode);	// 0x7755f4

	// The other gameswf::root members of the binary. Definitions belong to their own batches.
	void	stop_drag();						// 0x77415c
	void	set_display_viewport(int x0, int y0, int w, int h);	// 0x775d38
	float	get_frame_rate() const;				// 0x7741a0

	virtual int		get_movie_version();				// 0x773d48 (virtual +0x4c)
	virtual int		get_movie_width();					// 0x773d64 (virtual +0x50)
	virtual int		get_movie_height();					// 0x773d84 (virtual +0x54)
	virtual float	get_movie_fps();					// 0x773da4 (virtual +0x58)
	virtual void	get_mouse_state(int* x, int* y, int* buttons);	// 0x774138 (virtual +0x08)
	character*	get_root_movie();				// 0x774154 (binary symbol is not const)
	movie_definition*	get_movie_definition();	// 0x774168
	int		get_current_frame() const;			// 0x774184
	virtual float	get_pixel_scale() const;	// 0x7741bc (virtual +0x0c)
	character*	get_character(int character_id);	// 0x7741c4
	void	set_background_color(const rgba& color);	// 0x7741e0
	void	set_background_alpha(float alpha);	// 0x7741f4
	float	get_background_alpha() const;		// 0x774244
	void	goto_frame(int target_frame_number);	// 0x774260
	virtual bool	has_looped() const;			// 0x77427c (virtual +0x10)
	virtual bool	goto_labeled_frame(const tu_stringi& label);	// 0x7742f4 (virtual +0x14)
#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
	virtual void	set_play_state(character::play_state s);	// 0x774340 (virtual +0x18)
	virtual character::play_state	get_play_state() const;		// 0x77435c (virtual +0x1c)
#endif
	virtual void	set_variable(const char* path_to_var, const char* new_value);	// 0x774378 (virtual +0x20)
	virtual void	set_variable(const char* path_to_var, const wchar_t* new_value);	// 0x774394 (virtual +0x24)
	virtual const char*	get_variable(const char* path_to_var) const;	// 0x7743b0 (virtual +0x28)
	virtual const char*	call_method(const char* method_name, const char* method_arg_fmt, ...);	// 0x7744a4 (virtual +0x2c)
	virtual const char*	call_method_args(const char* method_name, const char* method_arg_fmt, va_list args);	// 0x7743cc (virtual +0x30)
	virtual tu_string	call_method(const char* method_name, as_value* arguments, int argument_count);	// 0x7743e8 (virtual +0x34)
	virtual void	set_visible(bool visible);	// 0x77441c (virtual +0x38)
	virtual bool	get_visible() const;		// 0x774428 (virtual +0x3c)
	virtual void*	get_userdata();				// 0x774434 (virtual +0x40)
	virtual void	set_userdata(void* ud);		// 0x77443c (virtual +0x44)
	virtual void	attach_display_callback(const char* path_to_object, void (*callback)(void* user_ptr), void* user_ptr);	// 0x774444 (virtual +0x48)
	void	set_frame_rate(float rate);			// 0x7744e0
	void	set_root_movie(character* root_movie);	// 0x77453c
	void	set_flash_vars(const tu_string& vars);	// 0x774660
	void	generate_mouse_button_events(mouse_button_state* ms);	// 0x774a04
	void	display();							// 0x775574

	// DH2: the listener calls on the +0xb8 block. The binary calls gameswf::listener::add(root+0xb8, obj) and
	// gameswf::listener::remove(root+0xb8, obj) from the as_timer constructors (0x7a6f24, 0x7a7064) and
	// as_timer::clear (0x7a6da8). The DH2 listener type is not declared yet (layout request, see the
	// pack3 0x760940 row), so the calls are declared here with the upstream root names. The definitions
	// belong to the listener batch and are not in this header.
	void	add_listener(as_object* obj);		// DH2: listener::add(root+0xb8, obj)
	bool	remove_listener(as_object* obj);	// DH2: listener::remove(root+0xb8, obj)
};

}  // namespace gameswf
