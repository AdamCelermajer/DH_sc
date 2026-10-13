// Batch v1-vendor-gameswf-gameswf_root-d24935-0: gameswf::root (DH2 layout, see
// vendor-gameswf/include/vendor-gameswf/gameswf_root.hpp). 15 binary functions. The two copies of
// ~root (0x774ebc, 0x774fc4) and of the constructor (0x775da0, 0x775f70) have identical bodies and
// share one definition each. Status per function is in ledger.d/v1-vendor-gameswf-gameswf_root-d24935-0.csv.
//
// Compiled by default (tools/trackb/check_tu.sh): notify_mouse_state (0x774128) and end_display (0x774298).
// Everything else sits behind VENDOR_GAMESWF_DH2_CORE_HEADERS. Those bodies need the upstream character,
// player, movie_def and action types, and those headers do not build in this tree (abc_def::get_super_class
// reads instance_info::m_super_name, which the DH2 instance_info dropped; gc_ptr<as_object> and
// gc_ptr<character> need a gc_object_collector_base, which the DH2 ref_counted is not). The guarded bodies
// also call dh2::render_landscape() and read gameswf::s_render_handler. No batch defines either yet, so a
// build that sets the switch must also link the render handler batch.

#include "vendor-gameswf/gameswf_root.hpp"
#include "vendor-gameswf/gameswf_root_transform.hpp"

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include <cmath>

#include "base/tu_random.h"
#include "gameswf/gameswf_action.h"     // event_id (upstream)
#include "gameswf/gameswf_character.h"  // character (upstream)
#include "gameswf/gameswf_movie_def.h"  // movie_def_impl (upstream)
#include "gameswf/gameswf_player.h"     // player (upstream)
#include "gameswf/gameswf_root.h"       // gameswf_engine_mutex() (upstream; the struct root there is #if 0)
#endif

namespace gameswf {

// 0x774128 gameswf::root::notify_mouse_state(int, int, int)
// The binary returns this in r0 and stores only the three fields (no mouse listener call in DH2).
void root::notify_mouse_state(int x, int y, int buttons)
{
	m_mouse_x = x;
	m_mouse_y = y;
	m_mouse_buttons = buttons;
}

// 0x774298 gameswf::root::end_display(void)
// NOT REPRODUCED: render_handler slot +0x2c (end_display) and then slot +0x48 with argument 0
// (set_context(nullptr)). The DH2 render_handler layout is not declared (see dh2::render_landscape).
void root::end_display()
{
}

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)

namespace {

// Movie frame rect of the definition (upstream rect m_frame_size). Binary: def +0xb4..+0xc0
// (x_min, x_max, y_min, y_max). The DH2 movie_definition layout is not declared yet (layout request).
dh2::root_frame_twips frame_rect(const movie_def_impl* def)
{
	const rect& r = def->m_frame_size;
	return dh2::root_frame_twips{r.m_x_min, r.m_x_max, r.m_y_min, r.m_y_max};
}

dh2::root_display_bounds display_bounds(const root* r)
{
	return dh2::root_display_bounds{r->m_display_x, r->m_display_y, r->m_display_width, r->m_display_height};
}

// render_handler slot +0xac: get_orientation(). Returns true for orientation 0 or 2 (the binary's v5).
bool landscape_orientation()
{
	return dh2::render_landscape();
}

}  // namespace

// 0x775da0 gameswf::root::root(gameswf::player*, gameswf::movie_def_impl*)
// 0x775f70 gameswf::root::root(gameswf::player*, gameswf::movie_def_impl*)   (second copy, same body)
// The display bounds (+0x24..+0x30) are not written here: the binary leaves them unset, and the owner
// header gives them no initializer either. set_display_bounds compares against them before its first write.
root::root(player* p, movie_def_impl* def)
{
	m_def = def;                                 // +0x0c, adds a reference (smart_ptr)
	m_viewport_x0 = 0;                           // +0x14
	m_viewport_y0 = 0;                           // +0x18
	m_viewport_width = 1;                        // +0x1c
	m_viewport_height = 1;                       // +0x20
	m_pixel_scale = 1.0f;                        // +0x34
	m_background_color = rgba(0, 0, 0, 255);     // +0x38
	m_mouse_x = 0;                               // +0x3c
	m_mouse_y = 0;                               // +0x40
	m_mouse_buttons = 0;                         // +0x44
	m_drag_character = nullptr;                  // +0x58
	m_drag_flag_5c = 0;                          // +0x5c
	m_drag_lock_center = false;                  // +0x5d
	m_drag_bound = false;                        // +0x5e
	m_drag_bound_x_min = 0.0f;                   // +0x60
	m_drag_bound_y_min = 0.0f;                   // +0x64
	m_drag_bound_x_max = 1.0f;                   // +0x68
	m_drag_bound_y_max = 1.0f;                   // +0x6c
	m_on_event_load_called = false;              // +0x84
	m_shift_key_state = false;                   // +0x85
	m_flushing_buffered_text = false;            // +0x86
	m_current_active_entity = nullptr;           // +0x88
	m_time_remainder = 1.0f;                     // +0x8c
	m_garbage_timer = 0.0f;                      // +0x94
	m_player = p;                                // +0xc8 weak reference to the player

	set_display_viewport(0, 0, static_cast<int>(m_def->get_width_pixels()),
	                     static_cast<int>(m_def->get_height_pixels()));
	m_frame_time = 1.0f / get_frame_rate();      // +0x90
	p->set_root(this);
}

// 0x774ebc gameswf::root::~root()
// 0x774fc4 gameswf::root::~root()   (second copy, same body)
// PARTIAL: the listener blocks at +0xa8 and +0xb8 are opaque (no release is written for them), and
// the weak proxy at +0xc8 is released by its member destructor after this body.
root::~root()
{
	m_movie = nullptr;                    // binary: set_ref(this+16, nullptr)
	m_def = nullptr;                      // binary: set_ref(this+12, nullptr)
	m_buffered_text.resize(0);            // binary: array<smart_ptr<character>>::resize, then reserve(0)
	m_buffered_text.reserve(0);
	m_current_active_entity = nullptr;    // binary: drop_ref of +0x88
	if (m_ref_7c)
	{
		m_ref_7c->drop_ref();             // +0x7c
	}
	if (m_ref_78)
	{
		m_ref_78->drop_ref();             // +0x78
	}
}

// 0x773dc0 gameswf::root::screen_to_logical(gameswf::point&)
// PARTIAL: the frame rect is read from m_def->m_frame_size (see frame_rect); its DH2 offset is not
// verified (layout request for movie_definition).
float root::screen_to_logical(point& p)
{
	float x = p.m_x;
	float y = p.m_y;
	dh2::root_screen_to_logical(landscape_orientation(), frame_rect(m_def.get_ptr()), display_bounds(this), x, y);
	p.m_x = x;
	p.m_y = y;
	return y;
}

// 0x773f50 gameswf::root::logical_to_screen(gameswf::point&)
// PARTIAL: same frame rect dependency as screen_to_logical.
float root::logical_to_screen(point& p)
{
	float x = p.m_x;
	float y = p.m_y;
	dh2::root_logical_to_screen(landscape_orientation(), frame_rect(m_def.get_ptr()), display_bounds(this),
	                            m_viewport_width, m_viewport_height, x, y);
	p.m_x = x;
	p.m_y = y;
	return y;
}

// 0x774544 gameswf::root::notify_key_event(gameswf::player*, gameswf::key::code, bool)
// PARTIAL: the binary also calls listener::notify(event_id(KEY_PRESS, k)) on the keypress listener
// (+0xa8) when down is set. That listener is an opaque block here (layout request), so the call is
// not written.
void root::notify_key_event(player* p, key::code k, bool down)
{
	gameswf_engine_mutex().lock();
	p->notify_key_object(k, down);
	gameswf_engine_mutex().unlock();
}

// 0x7748b8 gameswf::root::flush_buffered_text(void)
void root::flush_buffered_text()
{
	if (m_buffered_text.size() > 0)
	{
		m_flushing_buffered_text = true;                        // +0x86
		for (int i = 0; i < m_buffered_text.size(); ++i)
		{
			m_buffered_text[i]->display();                       // character vptr +0x120
		}
		m_flushing_buffered_text = false;
		// The binary calls array::resize with the array as its only argument (the size is not visible
		// in Hex-Rays). Written as resize(0), the same effect as clear().
		m_buffered_text.resize(0);
	}
}

// 0x774918 gameswf::root::set_active_entity(gameswf::character*)
// The binary re-reads the current entity after add_ref, so the else branch is not reached in practice;
// it is kept because it is in the binary.
void root::set_active_entity(character* ch)
{
	character* prev = m_current_active_entity.get_ptr();
	if (prev)
	{
		prev->add_ref();
		if (prev == m_current_active_entity.get_ptr())
		{
			m_current_active_entity = ch;
		}
		else
		{
			character* now = m_current_active_entity.get_ptr();
			if (now)
			{
				now->on_event(event_id(event_id::KILLFOCUS));
				m_current_active_entity = nullptr;
			}
			if (prev->on_event(event_id(event_id::SETFOCUS)))
			{
				m_current_active_entity = prev;
			}
			m_current_active_entity = ch;
		}
		prev->drop_ref();
	}
	else
	{
		m_current_active_entity = ch;
	}
}

// 0x775154 gameswf::root::start_drag(gameswf::character*, bool, bool, gameswf::rect&)
// The bound rect is copied in the binary's order: x_min, y_min, x_max, y_max (rect words 0, 2, 1, 3).
void root::start_drag(character* ch, bool lock_center, bool bound, rect& r)
{
	if (m_drag_character)
	{
		stop_drag();
	}
	m_drag_lock_center = lock_center;
	m_drag_bound = bound;
	m_drag_character = ch;
	m_drag_bound_x_min = r.m_x_min;
	m_drag_bound_y_min = r.m_y_min;
	m_drag_bound_x_max = r.m_x_max;
	m_drag_bound_y_max = r.m_y_max;
	m_drag_flag_5c = 0;
	ch->notify_need_advance();
}

// 0x7751bc gameswf::root::begin_display(void)
// PARTIAL: (1) the frame rect is read as in screen_to_logical (layout request). (2) The binary then
// calls render_handler slot +0x48 (set_context) with the player's context, read at player +0xac. That
// field is not named yet, so the call is not written. The rest follows the binary.
void root::begin_display()
{
	const dh2::root_frame_twips f = frame_rect(m_def.get_ptr());
	point p0;
	p0.m_x = f.x_min;
	p0.m_y = f.y_min;
	logical_to_screen(p0);
	point p1;
	p1.m_x = f.x_max;
	p1.m_y = f.y_max;
	logical_to_screen(p1);

	// NOT REPRODUCED: render_handler slot +0x30 (set_blend_mode(0)), slot +0x48 (set_context of the
	// player's context at player +0xac), and slot +0x28 (begin_display with m_background_color, the
	// viewport +0x14..+0x20 and the two corners p0 and p1). Layout request: DH2 render_handler.
	(void)p0;
	(void)p1;
}

// 0x775304 gameswf::root::advance(float, bool)
// PARTIAL: not reproduced: (1) m_listener (+0xb8) advance and alive (the block is opaque, and
// listener::alive is not declared upstream); (2) set_flash_vars from the player's vars (player +0x68);
// (3) sprite_instance::do_init_actions for the movie when it is a sprite (not declared). The garbage
// calls are unconditional, as in the binary: a null player crashes there too (set_as_garbage increments
// the player's counter at +0x30 with no null check).
void root::advance(float delta_time, bool fixed_step)
{
	gameswf_engine_mutex().lock();
	m_time_remainder += delta_time;                          // +0x8c
	m_garbage_timer -= delta_time;                           // +0x94
	if (m_time_remainder >= m_frame_time)
	{
		tu_random::next_random();                            // binary: result discarded
		do
		{
			if (m_frame_time > m_time_remainder)
			{
				break;
			}
			if (!m_on_event_load_called)
			{
				// NOT REPRODUCED: sprite_instance::do_init_actions when the movie is a sprite (is(2)).
				m_movie->construct();                        // character vptr +0x148
			}
			const float step = fixed_step ? m_frame_time : delta_time;
			m_movie->advance(step);                          // character vptr +0x5c
			if (!m_on_event_load_called)
			{
				m_on_event_load_called = true;
				m_movie->on_event(event_id(static_cast<event_id::id_code>(10)));   // event 10 (name not recovered)
			}
			m_time_remainder -= m_frame_time;
		} while (fixed_step);

		if (m_garbage_timer <= 0.0f)
		{
			player* garbage_player = m_player.get_ptr();
			garbage_player->set_as_garbage();                // binary: called unconditionally
			// NOT REPRODUCED: listener::alive on m_listener (+0xb8), see above.
			m_movie->this_alive();                           // character vptr +0x44
			garbage_player->clear_garbage();                 // binary: called unconditionally
			m_garbage_timer = 2.0f;                          // binary: 0x40000000
		}
		m_time_remainder = std::fmod(m_time_remainder, m_frame_time);
	}
	gameswf_engine_mutex().unlock();
}

// 0x7755f4 gameswf::root::set_display_bounds(int, int, int, int, gameswf::scale_mode)
// PARTIAL: (1) the frame rect is read as in screen_to_logical (layout request). (2) When the player
// is alive the binary builds an as_object with xMin, yMin, xMax, yMax and sets it as "Viewport" on
// player +0x34 (as_object vptr +0x1c set_member). Not reproduced: the player member is not named yet.
void root::set_display_bounds(int x, int y, int w, int h, scale_mode mode)
{
	const bool landscape = landscape_orientation();
	const dh2::root_frame_twips f = frame_rect(m_def.get_ptr());
	const float frame_w = (f.x_max - f.x_min) / 20.0f;
	const float frame_h = (f.y_max - f.y_min) / 20.0f;

	const float ratio_h = landscape ? static_cast<float>(h) / frame_h : static_cast<float>(h) / frame_w;
	const float ratio_w = landscape ? static_cast<float>(w) / frame_w : static_cast<float>(w) / frame_h;
	const float ratio = ratio_h / ratio_w;

	int new_x = x;
	int new_y = y;
	int new_w = w;
	int new_h = h;

	const int mode_value = static_cast<int>(mode);
	bool crop_height = false;   // binary LABEL_30: the height follows the aspect ratio
	bool crop_width = false;    // binary fall-through: the width follows the aspect ratio
	if (mode_value == 1)
	{
		if (ratio < 1.0f)
		{
			crop_height = true;
		}
		else
		{
			crop_width = true;
		}
	}
	else if (mode_value == 2)
	{
		if (ratio >= 1.0f)
		{
			crop_height = true;
		}
		else
		{
			crop_width = true;
		}
	}

	if (crop_height)
	{
		new_h = static_cast<int>(static_cast<float>(h) / ratio);
		new_y = y - (new_h - h) / 2;
	}
	else if (crop_width)
	{
		const int width_delta = static_cast<int>(static_cast<float>(w) * ratio) - w;
		new_w = static_cast<int>(static_cast<float>(w) * ratio);
		new_x = x - width_delta / 2;
	}

	if (new_x == m_display_x && new_y == m_display_y && new_w == m_display_width && new_h == m_display_height)
	{
		return;
	}
	m_display_x = new_x;
	m_display_y = new_y;
	m_display_width = new_w;
	m_display_height = new_h;

	const float px_w = landscape ? static_cast<float>(new_w) / frame_w : static_cast<float>(new_w) / frame_h;
	const float px_h = landscape ? static_cast<float>(new_h) / frame_h : static_cast<float>(new_h) / frame_w;
	m_pixel_scale = (px_w >= px_h) ? px_w : px_h;
	// NOT REPRODUCED: "Viewport" as_object on the player (see above).
}

// Declared in the owner header; the definition is in another vendor-gameswf batch (0x775d38) and is
// not part of this batch. Used by the constructor above.
// void root::set_display_viewport(int x0, int y0, int w, int h);

// Declared in the owner header; the definition is in another vendor-gameswf batch (0x7741a0).
// float root::get_frame_rate() const;

#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS

}  // namespace gameswf
