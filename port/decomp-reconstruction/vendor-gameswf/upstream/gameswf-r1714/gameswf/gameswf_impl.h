// gameswf_impl.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation code for the gameswf SWF player library.


#ifndef GAMESWF_IMPL_H
#define GAMESWF_IMPL_H


#include "gameswf/gameswf.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_render.h"
#include <assert.h>
#include "base/container.h"
#include "base/utility.h"
#include <stdarg.h>


namespace jpeg { struct input; }


namespace gameswf
{
	struct action_buffer;
	struct bitmap_character_def;
	struct bitmap_info;
	struct character;
	struct character_def;
	struct display_info;
	struct execute_tag;
	struct font;
	struct root;
	struct movie_definition_sub;

	struct stream;
	struct swf_event;

	struct swf_event
	{
		// NOTE: DO NOT USE THESE AS VALUE TYPES IN AN
		// array<>!  They cannot be moved!  The private
		// operator=(const swf_event&) should help guard
		// against that.

		event_id	m_event;
		as_value	m_method;

		swf_event() {}

	private:

		// DON'T USE THESE
		swf_event(const swf_event& s) { assert(0); }
		void	operator=(const swf_event& s) { assert(0); }
	};


	// For characters that don't store unusual state in their instances.
	struct generic_character : public character
	{
		gc_ptr<character_def>	m_def;

#if 0 /* DH2: replaced by v1-vendor-gameswf-pack1-607ee7-0.cpp */
		generic_character(player* player, character_def* def, character* parent, int id) :
			character(player, parent, id),
			m_def(def)
		{
			assert(m_def != NULL);
		}

		~generic_character()
		{
		}
#endif /* DH2 */
		generic_character(player* player, character_def* def, character* parent, int id);	// DH2: 0x75b75c, body in v1-vendor-gameswf-pack1-607ee7-0.cpp
		~generic_character();	// DH2: 0x75dde0 (D1), body in v1-vendor-gameswf-pack1-607ee7-0.cpp; 0x75de28 is compiler-generated
		virtual bool is(int class_id) const;	// DH2: 0x7599cc, vtable +0x08, body in v1-vendor-gameswf-pack1-607ee7-0.cpp
		virtual bool hit_test(float x, float y);	// DH2: 0x7599f8, vtable +0x6c, body in v1-vendor-gameswf-pack1-607ee7-0.cpp
		virtual character* get_topmost_mouse_entity(float x, float y);	// DH2: two-float overload at vtable +0x68, returns this or NULL (not bool; 0x75b6fc, other batch); hit_test calls it
		unsigned char unk_009d;	// DH2: +0x9d, zeroed by the constructor (name not recovered). Layout request: m_def above is a raw counted pointer at +0xa0 in the binary.

		virtual character_def* get_character_def() { return m_def.get_ptr();	}

		virtual void	display()
		{
			m_def->display(this);	// pass in transform info
			do_display_callback();
		}

		virtual bool get_topmost_mouse_entity( character * &te, float x, float y)
		{
			assert(get_visible());	// caller should check this.

			const matrix&	m = get_matrix();
			point	p;
			m.transform_by_inverse(&p, point(x, y));

			if (m_def->point_test_local(p.m_x, p.m_y))
			{
				// The mouse is inside the shape.
				te = this;
				return true;
			}
			return false;
		}
	};


	struct bitmap_character_def : public character_def
	{
		bitmap_character_def(player* player) :
			character_def(player)
		{
		}

		virtual gameswf::bitmap_info*	get_bitmap_info() = 0;
	};

	// Bitmap character
	struct bitmap_character : public bitmap_character_def
	{
		bitmap_character(movie_definition* rdef, bitmap_info* bi);
		bitmap_character(player* p, bitmap_info* bi, rect* r);	// DH2: 0x7ae1c8 (clone 0x75eaa4 with r NULL), body in v1-vendor-gameswf-pack1-607ee7-0.cpp
		~bitmap_character();	// DH2: 0x75df40 (D1), body in v1-vendor-gameswf-pack1-607ee7-0.cpp; 0x75dfd8 is compiler-generated
		virtual bool is(int class_id) const;	// DH2: 0x759a14, vtable +0x08, body in v1-vendor-gameswf-pack1-607ee7-0.cpp
		rect m_rect;	// DH2: +0x24..+0x33 (x_min, x_max, y_min, y_max), stored at construction; layout request, see v1-vendor-gameswf-pack1-607ee7-0.cpp

		// Return true if the specified point is on the interior of our shape.
		// Incoming coords are local coords.
		bool	point_test_local(float x, float y);

		virtual void get_bound(rect* bound);
		virtual void	display(character* ch);
		gameswf::bitmap_info*	get_bitmap_info();

		private:

			gc_ptr<gameswf::bitmap_info>	m_bitmap_info;
	};

#if 0 /* DH2: replaced by vendor-gameswf/include/vendor-gameswf/execute_tags.hpp (binary vtable has no execute_now slot; adds get_depth, get_action_offset, is_place_tag) */
	// Execute tags include things that control the operation of
	// the movie.  Essentially, these are the events associated
	// with a frame.
	struct execute_tag
	{
		virtual ~execute_tag() {}
		virtual void	execute(character* m) {}
		virtual void	execute_now(character* m) { execute(m); }
		virtual void	execute_state(character* m) {}
		virtual void	execute_state_reverse(character* m, int frame) { execute_state(m); }
		virtual bool	is_remove_tag() const { return false; }
		virtual bool	is_action_tag() const { return false; }
		virtual uint32	get_depth_id_of_replace_or_add_tag() const { return static_cast<uint32>(-1); }
	};
#endif  // DH2: execute_tag replaced, see above

	//
	// Loader callbacks.
	//

	// Register a loader function for a certain tag type.  Most
	// standard tags are handled within gameswf.  Host apps might want
	// to call this in order to handle special tag types.
	void	register_tag_loader(int tag_type, loader_function lf);


	// Tag loader functions.
	void	null_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	set_background_color_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	jpeg_tables_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_bits_jpeg_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_bits_jpeg2_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_bits_jpeg3_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_shape_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_shape_morph_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_font_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_font_info_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_text_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_edit_text_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	place_object_2_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_bits_lossless_2_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	sprite_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	end_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	remove_object_2_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	do_action_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	button_character_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	frame_label_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	export_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	import_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_sound_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	start_sound_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	button_sound_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	do_init_action_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_video_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	video_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	sound_stream_head_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	sound_stream_block_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_file_attribute_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_font_alignzones(stream* in, int tag_type, movie_definition_sub* m);
	void	define_csm_textsetting_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_metadata_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_enable_debugger_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_tabindex_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_abc_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	symbol_class_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_scene_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_font_name(stream* in, int tag_type, movie_definition_sub* m);

}	// end namespace gameswf


#endif // GAMESWF_IMPL_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
