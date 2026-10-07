// gameswf_text.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code for the text tags.


//TODO: csm_textsetting() is common for text_def & edit_text_def
//	therefore make new base class for text_def & edit_text_def
//	and move csm_textsetting() to it.

#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_sprite.h"
#include "swf_edit_text_connection_v1.hpp"
#include "edit_text_definition_v1.hpp"
#include <stdexcept>

namespace gameswf
{
    static void source_get_textformat_v1(const fn_call& fn){
        auto* field=cast_to<edit_text_character>(fn.this_ptr);
        if(!field||!field->m_source_text_v1)throw std::runtime_error("source getTextFormat receiver unavailable");
        field->m_source_text_v1->get_format(fn);
    }
    void edit_text_character::source_initialize_v1(){
        if(!m_source_text_v1)throw std::runtime_error("source edit-text initializer unavailable");
        m_source_text_v1->initialize();
    }
    void edit_text_character::source_set_text_v1(const tu_string& text,bool html,bool write){
        if(!m_source_text_v1)throw std::runtime_error("source edit-text setter unavailable");
        m_source_text_v1->set_text(std::string(text.c_str(),text.size()),html,write);
    }
    void edit_text_character::format_text(bool html){
        if(!m_source_text_v1)throw std::runtime_error("source edit-text formatter unavailable");
        m_source_text_v1->format(html);
    }

	// use safe container
	static array<char> s_gameswf_key_to_ascii;
	static array<char> s_gameswf_key_to_ascii_shifted;

	static void initialize_key_to_ascii()
	{
		if (s_gameswf_key_to_ascii.size() > 0)
		{
			return;
		}

		s_gameswf_key_to_ascii.resize(key::KEYCOUNT);
		s_gameswf_key_to_ascii_shifted.resize(key::KEYCOUNT);

		// the letters
		for (int i = 0; i < s_gameswf_key_to_ascii.size(); i++)
		{
			if (i >= key::A && i <= key::Z)
			{
				s_gameswf_key_to_ascii[i]         = 'a' + ( i - key::A );
				s_gameswf_key_to_ascii_shifted[i] = 'A' + ( i - key::A );
			}
			else
			{
				s_gameswf_key_to_ascii[i]         = 0;
				s_gameswf_key_to_ascii_shifted[i] = 0;
			}
		}

		// numbers
		s_gameswf_key_to_ascii_shifted[key::KP_0] = s_gameswf_key_to_ascii[key::KP_0] = s_gameswf_key_to_ascii[key::_0] = '0'; s_gameswf_key_to_ascii_shifted[key::_0] = ')';
		s_gameswf_key_to_ascii_shifted[key::KP_1] = s_gameswf_key_to_ascii[key::KP_1] = s_gameswf_key_to_ascii[key::_1] = '1'; s_gameswf_key_to_ascii_shifted[key::_1] = '!';
		s_gameswf_key_to_ascii_shifted[key::KP_2] = s_gameswf_key_to_ascii[key::KP_2] = s_gameswf_key_to_ascii[key::_2] = '2'; s_gameswf_key_to_ascii_shifted[key::_2] = '@';
		s_gameswf_key_to_ascii_shifted[key::KP_3] = s_gameswf_key_to_ascii[key::KP_3] = s_gameswf_key_to_ascii[key::_3] = '3'; s_gameswf_key_to_ascii_shifted[key::_3] = '#';
		s_gameswf_key_to_ascii_shifted[key::KP_4] = s_gameswf_key_to_ascii[key::KP_4] = s_gameswf_key_to_ascii[key::_4] = '4'; s_gameswf_key_to_ascii_shifted[key::_4] = '$';
		s_gameswf_key_to_ascii_shifted[key::KP_5] = s_gameswf_key_to_ascii[key::KP_5] = s_gameswf_key_to_ascii[key::_5] = '5'; s_gameswf_key_to_ascii_shifted[key::_5] = '%';
		s_gameswf_key_to_ascii_shifted[key::KP_6] = s_gameswf_key_to_ascii[key::KP_6] = s_gameswf_key_to_ascii[key::_6] = '6'; s_gameswf_key_to_ascii_shifted[key::_6] = '^';
		s_gameswf_key_to_ascii_shifted[key::KP_7] = s_gameswf_key_to_ascii[key::KP_7] = s_gameswf_key_to_ascii[key::_7] = '7'; s_gameswf_key_to_ascii_shifted[key::_7] = '&';
		s_gameswf_key_to_ascii_shifted[key::KP_8] = s_gameswf_key_to_ascii[key::KP_8] = s_gameswf_key_to_ascii[key::_8] = '8'; s_gameswf_key_to_ascii_shifted[key::_8] = '*';
		s_gameswf_key_to_ascii_shifted[key::KP_9] = s_gameswf_key_to_ascii[key::KP_9] = s_gameswf_key_to_ascii[key::_9] = '9'; s_gameswf_key_to_ascii_shifted[key::_9] = '(';

		// number pad others
		s_gameswf_key_to_ascii_shifted[key::KP_MULTIPLY] = s_gameswf_key_to_ascii[key::KP_MULTIPLY] = '*';
		s_gameswf_key_to_ascii_shifted[key::KP_ADD]      = s_gameswf_key_to_ascii[key::KP_ADD]      = '+';
		s_gameswf_key_to_ascii_shifted[key::KP_SUBTRACT] = s_gameswf_key_to_ascii[key::KP_SUBTRACT] = '-';
		s_gameswf_key_to_ascii_shifted[key::KP_DECIMAL]  = s_gameswf_key_to_ascii[key::KP_DECIMAL]  = '.';
		s_gameswf_key_to_ascii_shifted[key::KP_DIVIDE]   = s_gameswf_key_to_ascii[key::KP_DIVIDE]   = '/';

		// the rest...
		s_gameswf_key_to_ascii_shifted[key::SPACE] =  s_gameswf_key_to_ascii[key::SPACE]                     = ' ';
		s_gameswf_key_to_ascii[key::SEMICOLON]     = ';'; s_gameswf_key_to_ascii_shifted[key::SEMICOLON]     = ':';
		s_gameswf_key_to_ascii[key::EQUALS]        = '='; s_gameswf_key_to_ascii_shifted[key::EQUALS]        = '+';
		s_gameswf_key_to_ascii[key::MINUS]         = '-'; s_gameswf_key_to_ascii_shifted[key::MINUS]         = '_';
		s_gameswf_key_to_ascii[key::SLASH]         = '/'; s_gameswf_key_to_ascii_shifted[key::SLASH]         = '?';
		s_gameswf_key_to_ascii[key::BACKTICK]      = '`'; s_gameswf_key_to_ascii_shifted[key::BACKTICK]      = '~';
		s_gameswf_key_to_ascii[key::LEFT_BRACKET]  = '['; s_gameswf_key_to_ascii_shifted[key::LEFT_BRACKET]  = '{';
		s_gameswf_key_to_ascii[key::BACKSLASH]     = '\\';s_gameswf_key_to_ascii_shifted[key::BACKSLASH]     = '|';
		s_gameswf_key_to_ascii[key::RIGHT_BRACKET] = ']'; s_gameswf_key_to_ascii_shifted[key::RIGHT_BRACKET] = '}';
		s_gameswf_key_to_ascii[key::QUOTE]        = '\''; s_gameswf_key_to_ascii_shifted[key::QUOTE]         = '\"';
		s_gameswf_key_to_ascii[key::COMMA]         = ','; s_gameswf_key_to_ascii_shifted[key::COMMA]         = '<';
		s_gameswf_key_to_ascii[key::PERIOD]        = '.'; s_gameswf_key_to_ascii_shifted[key::PERIOD]        = '>';
	}

	void	as_global_textfield_ctor(const fn_call& fn)
	// Constructor for ActionScript class XMLSocket
	{
		edit_text_character_def* empty_text_def = new edit_text_character_def(fn.get_player(), 0, 0);
		character* ch = new edit_text_character(fn.get_player(), NULL, empty_text_def, 0);
		fn.result->set_as_object(ch);
	}

	// Render the given glyph records.

	static void	display_glyph_records(
		const matrix& this_mat,
		character* inst,
		const array<text_glyph_record>& records,
		movie_definition_sub* root_def)
	{
		array<fill_style>	dummy_style;	// used to pass a color on to shape_character::display()
		array<line_style>	dummy_line_style;
		dummy_style.resize(1);

		matrix	mat = inst->get_world_matrix();
		mat.concatenate(this_mat);

		cxform	cx = inst->get_world_cxform();
		float	pixel_scale = inst->get_pixel_scale();

		matrix	base_matrix = mat;
//		float	base_matrix_max_scale = base_matrix.get_max_scale();

		float	scale = 1.0f;
		float	x = 0.0f;
		float	y = 0.0f;

		for (int i = 0; i < records.size(); i++)
		{
			// Draw the characters within the current record; i.e. consecutive
			// chars that share a particular style.
			const text_glyph_record&	rec = records[i];

			rec.m_style.resolve_font(root_def);

			font*	fnt = rec.m_style.m_font;
			if (fnt == NULL)
			{
				continue;
			}

			scale = rec.m_style.m_text_height / 1024.0f;	// the EM square is 1024 x 1024

			// Flash 8
			// All the EMSquare coordinates are multiplied by 20 at export,
			// allowing fractional resolution to 1/20 of a unit.
			if (fnt->is_define_font3())
			{
				scale /= 20.0f;
			}

//			float	text_screen_height = base_matrix_max_scale
//				* scale
//				* 1024.0f
//				/ 20.0f
//				* pixel_scale;

			if (rec.m_style.m_has_x_offset)
			{
				x = rec.m_style.m_x_offset;
			}
			if (rec.m_style.m_has_y_offset)
			{
				y = rec.m_style.m_y_offset;
			}

			dummy_style[0].set_color(rec.m_style.m_color);

			rgba	transformed_color = cx.transform(rec.m_style.m_color);

			for (int j = 0; j < rec.m_glyphs.size(); j++)
			{
				const glyph& g = rec.m_glyphs[j];

				mat = base_matrix;
				mat.concatenate_translation(x, y);
				mat.concatenate_scale(scale);

				if (g.m_glyph_index == -1 && g.m_bitmap_info == NULL)
				{
					// Invalid glyph; render it as an empty box.
					render::set_matrix(mat);
					render::line_style_color(transformed_color);

					// The EM square is 1024x1024, but usually isn't filled up.
					// We'll use about half the width, and around 3/4 the height.
					// Values adjusted by eye.
					// The Y baseline is at 0; negative Y is up.
					static const coord_component s_empty_char_box[5 * 2] =
					{
						32,   32,
						480,   32,
						480, -656,
						32, -656,
						32,   32
					};
					render::draw_line_strip(s_empty_char_box, 5);
					x += rec.m_glyphs[j].m_glyph_advance;
					continue;
				}

				if (g.m_bitmap_info != NULL)
				{
					// device font

					rect uv_bounds;
					uv_bounds.m_x_min = 0;
					uv_bounds.m_y_min = 0;
					uv_bounds.m_x_max = g.m_bounds.m_x_max;
					uv_bounds.m_y_max = g.m_bounds.m_y_max;

					rect bounds;
					bounds.m_x_max = g.m_bounds.m_x_max - g.m_bounds.m_x_min; 
					bounds.m_y_max = g.m_bounds.m_y_max - g.m_bounds.m_y_min;
					bounds.m_x_min = - g.m_bounds.m_x_min; 
					bounds.m_y_min = - g.m_bounds.m_y_min;

					float s_EM_scale = 1024.0f / g.m_fontsize;
					float	xscale = g.m_bitmap_info->get_width() * s_EM_scale;
					float yscale = g.m_bitmap_info->get_height() * s_EM_scale;

					if (fnt->is_define_font3())
					{
						xscale *= 20.0f;
						yscale *= 20.0f;
					}

					bounds.m_x_min *= xscale;
					bounds.m_x_max *= xscale;
					bounds.m_y_min *= yscale;
					bounds.m_y_max *= yscale;

					// for device font xscale = yscale, applied for edit_text_character only
					if (cast_to<edit_text_character>(inst) != NULL)
					{
						yscale = mat.get_y_scale();
						float radians = mat.get_rotation();
						mat.set_scale_rotation(yscale, yscale, radians);
					}
					render::draw_bitmap(mat, g.m_bitmap_info.get_ptr(), bounds, uv_bounds, transformed_color);
				}
				else
				if (g.m_shape_glyph != NULL)
				{
					g.m_shape_glyph->display(mat, cx, pixel_scale, dummy_style, dummy_line_style, render_handler::BLEND_NORMAL);
				}
				else
				if (g.m_glyph_index >= 0)
				{
					// static text
					shape_character_def* sh = fnt->get_glyph_by_index(g.m_glyph_index);
					if (sh)
					{
						sh->display(mat, cx, pixel_scale, dummy_style, dummy_line_style, render_handler::BLEND_NORMAL);
					}
				}

				x += rec.m_glyphs[j].m_glyph_advance;

			}
		}
	}


	text_character_def::text_character_def(player* player, movie_definition_sub* root_def) :
		character_def(player), m_root_def(root_def), m_use_flashtype(false), m_grid_fit(0), m_thickness(0.0f), m_sharpness(0.0f),
		m_is_glyphs_drawn(false)
	{
		assert(m_root_def);
	}

	void	text_character_def::read(stream* in, int tag_type, movie_definition_sub* m)
	{
		assert(m != NULL);
		assert(tag_type == 11 || tag_type == 33);

		m_rect.read(in);
		m_matrix.read(in);

		int	glyph_bits = in->read_u8();
		int	advance_bits = in->read_u8();

		IF_VERBOSE_PARSE(log_msg("begin text records\n"));

		bool	last_record_was_style_change = false;

		text_style	style;
		for (;;)
		{
			int	first_byte = in->read_u8();

			if (first_byte == 0)
			{
				// This is the end of the text records.
				IF_VERBOSE_PARSE(log_msg("end text records\n"));
				break;
			}

			// Style changes and glyph records just alternate.
			// (Contrary to what most SWF references say!)
			if (last_record_was_style_change == false)
			{
				// This is a style change.

				last_record_was_style_change = true;

				bool	has_font = (first_byte >> 3) & 1;
				bool	has_color = (first_byte >> 2) & 1;
				bool	has_y_offset = (first_byte >> 1) & 1;
				bool	has_x_offset = (first_byte >> 0) & 1;

				IF_VERBOSE_PARSE(log_msg("  text style change\n"));

				if (has_font)
				{
					Uint16	font_id = in->read_u16();
					style.m_font_id = font_id;
					IF_VERBOSE_PARSE(log_msg("  has_font: font id = %d\n", font_id));
				}
				if (has_color)
				{
					if (tag_type == 11)
					{
						style.m_color.read_rgb(in);
					}
					else
					{
						assert(tag_type == 33);
						style.m_color.read_rgba(in);
					}
					IF_VERBOSE_PARSE(log_msg("  has_color\n"));
				}
				if (has_x_offset)
				{
					style.m_has_x_offset = true;
					style.m_x_offset = in->read_s16();
					IF_VERBOSE_PARSE(log_msg("  has_x_offset = %g\n", style.m_x_offset));
				}
				else
				{
					style.m_has_x_offset = false;
					style.m_x_offset = 0.0f;
				}
				if (has_y_offset)
				{
					style.m_has_y_offset = true;
					style.m_y_offset = in->read_s16();
					IF_VERBOSE_PARSE(log_msg("  has_y_offset = %g\n", style.m_y_offset));
				}
				else
				{
					style.m_has_y_offset = false;
					style.m_y_offset = 0.0f;
				}
				if (has_font)
				{
					style.m_text_height = in->read_u16();
					IF_VERBOSE_PARSE(log_msg("  text_height = %g\n", style.m_text_height));
				}
			}
			else
			{
				// Read the glyph record.

				last_record_was_style_change = false;

				int	glyph_count = first_byte;

				m_text_glyph_records.resize(m_text_glyph_records.size() + 1);
				m_text_glyph_records.back().m_style = style;
				m_text_glyph_records.back().read(in, glyph_count, glyph_bits, advance_bits);

				IF_VERBOSE_PARSE(log_msg("  glyph_records: count = %d\n", glyph_count));
			}
		}
	}


	void	text_character_def::display(character* inst)
	// Draw the string.
	{

		// try glyph provider
		if (m_is_glyphs_drawn == false)
		{
			m_is_glyphs_drawn = true;

			glyph_provider* fp = get_glyph_provider();
			if (fp)
			{
				for (int i = 0; i < m_text_glyph_records.size(); i++)
				{
					text_glyph_record&	rec = m_text_glyph_records[i];
					rec.m_style.resolve_font(m_root_def);

					font*	fnt = rec.m_style.m_font;
					if (fnt == NULL)
					{
						continue;
					}

					for (int j = 0; j < rec.m_glyphs.size(); j++)
					{
						glyph& g = rec.m_glyphs[j];
						if (g.m_glyph_index < 0)
						{
							continue;
						}

						int char_code = fnt->get_code_by_index(g.m_glyph_index);
						if (char_code < 0)
						{
							continue;
						}

						g.m_shape_glyph = fnt->get_glyph_by_index(g.m_glyph_index);
						if (g.m_shape_glyph)
						{
							g.m_fontsize = (int) rec.m_style.m_text_height / 20.0f;

							// find final glyph fontsize
							matrix m = inst->get_world_matrix();
							float yscale = m.get_y_scale();
							g.m_fontsize = int(yscale * g.m_fontsize);
							if (g.m_fontsize > 96)
							{
								g.m_fontsize = 96;
							}

							g.m_bitmap_info = fp->get_char_image(g.m_shape_glyph, char_code, fnt->get_name(), fnt->is_bold(), fnt->is_italic(), 
								g.m_fontsize,	&g.m_bounds, NULL);
						}
					}
				}
			}
		}
		display_glyph_records(m_matrix, inst, m_text_glyph_records, m_root_def);
	}

	void	text_character_def::csm_textsetting(stream* in, int tag_type)
	{
		assert(tag_type == 74);	// Flash 8

		m_use_flashtype = in->read_uint(2) == 0 ? false : true;

		// 0 = Do not use grid fitting. AlignmentZones and LCD sub-pixel information
		// will not be used.
		// 1 = Pixel grid fit. Only supported for left-aligned dynamic text.
		// This setting provides the ultimate in FlashType readability, with
		// crisp letters aligned to pixels.
		// 2 = Sub-pixel grid fit. Align letters to the 1/3 pixel used by LCD monitors.
		// Can also improve quality for CRT output.
		m_grid_fit = in->read_uint(3);

		in->read_uint(3); // reserved;

		m_thickness = in->read_fixed();
		m_sharpness = in->read_fixed();

		in->read_u8();	// reserved

		IF_VERBOSE_PARSE(
			log_msg("reading CSMTextSetting tag\n");
		log_msg("	m_use_flashtype = %s\n", m_use_flashtype ? "true" : "false");
		log_msg("	m_grid_fit = %d\n", m_grid_fit);
		log_msg("	m_thickness = %f\n", m_thickness);
		log_msg("	m_sharpness = %f\n", m_sharpness);
		);

	}

	void text_character_def::get_bound(rect* bound)
	{
		*bound = m_rect;
	}

	void	define_text_loader(stream* in, int tag_type, movie_definition_sub* m)
	// Read a DefineText tag.
	{
		assert(tag_type == 11 || tag_type == 33);

		Uint16	character_id = in->read_u16();

		text_character_def*	ch = new text_character_def(m->get_player(), m);
		IF_VERBOSE_PARSE(log_msg("text_character, id = %d\n", character_id));
		ch->read(in, tag_type, m);

		// IF_VERBOSE_PARSE(print some stuff);

		m->add_character(character_id, ch);
	}


	//
	// edit_text_character_def
	//

	// creates empty dynamic text field
	edit_text_character_def::edit_text_character_def(player* player, int width, int height) :
		character_def(player),
		m_root_def(NULL),
		m_word_wrap(false),
		m_multiline(false),
		m_password(false),
		m_readonly(false),
		m_auto_size(false),
		m_no_select(false),
		m_border(false),
		m_html(false),
		m_use_outlines(false),
		m_font_id(-1),
		m_font(NULL),
		m_text_height(PIXELS_TO_TWIPS(12)),	//	default 12
		m_max_length(0),
		m_alignment(ALIGN_LEFT),
		m_left_margin(0.0f),
		m_right_margin(0.0f),
		m_indent(0.0f),
		m_leading(0.0f),
		m_use_flashtype(false),
		m_grid_fit(0),
		m_thickness(0.0f),
		m_sharpness(0.0f)
	{
		m_rect.m_x_min = 0;
		m_rect.m_y_min = 0;
		m_rect.m_x_max = PIXELS_TO_TWIPS(width);
		m_rect.m_y_max = PIXELS_TO_TWIPS(height);

		m_color.set(0, 0, 0, 255);
		m_font = new font(player);
	}

	edit_text_character_def::edit_text_character_def(player* player, movie_definition_sub* root_def) :
		character_def(player),
		m_root_def(root_def),
		m_word_wrap(false),
		m_multiline(false),
		m_password(false),
		m_readonly(false),
		m_auto_size(false),
		m_no_select(false),
		m_border(false),
		m_html(false),
		m_use_outlines(false),
		m_font_id(-1),
		m_font(NULL),
		m_text_height(PIXELS_TO_TWIPS(12)),	//	default 12
		m_max_length(0),
		m_alignment(ALIGN_LEFT),
		m_left_margin(0.0f),
		m_right_margin(0.0f),
		m_indent(0.0f),
		m_leading(0.0f),
		m_use_flashtype(false),
		m_grid_fit(0),
		m_thickness(0.0f),
		m_sharpness(0.0f)
	{
		assert(m_root_def);
		m_color.set(0, 0, 0, 255);
	}

	edit_text_character_def::~edit_text_character_def()
	{
	}

	void	edit_text_character_def::read(stream* in, int tag_type, movie_definition_sub* m)
	{
		assert(m != NULL);
		assert(tag_type == 37);

		m_rect.read(in);

		in->align();
		bool	has_text = in->read_uint(1) ? true : false;
		m_word_wrap = in->read_uint(1) ? true : false;
		m_multiline = in->read_uint(1) ? true : false;
		m_password = in->read_uint(1) ? true : false;
		m_readonly = in->read_uint(1) ? true : false;
		bool	has_color = in->read_uint(1) ? true : false;
		bool	has_max_length = in->read_uint(1) ? true : false;
		bool	has_font = in->read_uint(1) ? true : false;

		in->read_uint(1);	// reserved
		m_auto_size = in->read_uint(1) ? true : false;
		bool	has_layout = in->read_uint(1) ? true : false;
		m_no_select = in->read_uint(1) ? true : false;
		m_border = in->read_uint(1) ? true : false;
		in->read_uint(1);	// reserved
		m_html = in->read_uint(1) ? true : false;
		m_use_outlines = in->read_uint(1) ? true : false;

		if (has_font)
		{
			m_font_id = in->read_u16();
			m_text_height = (float) in->read_u16();
		}

		if (has_color)
		{
			m_color.read_rgba(in);
		}

		if (has_max_length)
		{
			m_max_length = in->read_u16();
		}

		if (has_layout)
		{
			m_alignment = (alignment) in->read_u8();
			m_left_margin = (float) in->read_u16();
			m_right_margin = (float) in->read_u16();
			m_indent = (float) in->read_s16();
			m_leading = (float) in->read_s16();
		}

		in->read_string(&m_var_name);

		if (has_text)
		{
			in->read_string(&m_default_text);
            std::string text(m_default_text.c_str(),m_default_text.size()),error;
            if(!dh2::ui::edit_text_definition_v1::remove_html(text,error))throw std::runtime_error(error);
            m_default_text=tu_string(text.data(),text.size());
		}

		IF_VERBOSE_PARSE(log_msg("edit_text_char, varname = %s, text = %s\n",
			m_var_name.c_str(), m_default_text.c_str()));
	}

	void	edit_text_character_def::csm_textsetting(stream* in, int tag_type)
	{
		assert(tag_type == 74);	// Flash 8
		m_use_flashtype = in->read_uint(2) == 0 ? false : true;

		// 0 = Do not use grid fitting. AlignmentZones and LCD sub-pixel information
		// will not be used.
		// 1 = Pixel grid fit. Only supported for left-aligned dynamic text.
		// This setting provides the ultimate in FlashType readability, with
		// crisp letters aligned to pixels.
		// 2 = Sub-pixel grid fit. Align letters to the 1/3 pixel used by LCD monitors.
		// Can also improve quality for CRT output.
		m_grid_fit = in->read_uint(3);

		in->read_uint(3); // reserved;

		m_thickness = in->read_fixed();
		m_sharpness = in->read_fixed();

		in->read_u8();	// reserved

		IF_VERBOSE_PARSE(
			log_msg("reading CSMTextSetting tag\n");
		log_msg("	m_use_flashtype = %s\n", m_use_flashtype ? "true" : "false");
		log_msg("	m_grid_fit = %d\n", m_grid_fit);
		log_msg("	m_thickness = %f\n", m_thickness);
		log_msg("	m_sharpness = %f\n", m_sharpness);
		);

	}

	//
	// edit_text_character
	//

	void	set_textformat(const fn_call& fn)
	{
		edit_text_character* et = cast_to<edit_text_character>(fn.this_ptr);
		if(fn.nargs!=1)return;
		if(!et)throw std::runtime_error("source setTextFormat unsafe receiver");
		if (fn.nargs == 1)
		{
			as_textformat* tf = cast_to<as_textformat>(fn.arg(0).to_object());
			et->reset_format(tf);
		}
	}

	edit_text_character::edit_text_character(player* player, character* parent,
			edit_text_character_def* def, int id)	:
		character(player, parent, id),
		m_def(def),
		m_has_focus(false), 
		m_password(def->m_password),
		m_cursor(0), 
		m_xcursor(0.0f), 
		m_ycursor(0.0f),
		m_x(0.0f), 
		m_y(0.0f)
	{
        if(!m_def)throw std::runtime_error("source edit-text constructor requires definition");
        initialize_key_to_ascii();
        m_font=NULL;m_color.set(255,255,255,255);m_background_color.set(255,255,255,255);
        m_source_text_v1=std::make_shared<dh2::ui::SwfEditTextFieldV1>(*this);
        // Original C1 791804 calls init. Placement later calls it again.
        source_initialize_v1();
    }

	edit_text_character::~edit_text_character() 
	{ 
		// on_event(event_id::KILLFOCUS) will be executed
		// during remove_display_object()
	} 

	void edit_text_character::reset_format(as_textformat* tf)
	{
		if(!tf)throw std::runtime_error("source setTextFormat requires actual TextFormat");
		as_value val;

		if (tf->get_member("leftMargin", &val))
		{
			m_left_margin = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("indent", &val))
		{
			m_indent = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("rightMargin", &val))
		{
			m_right_margin = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("leading", &val))
		{
			m_leading = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("letterSpacing", &val))
        {m_letter_spacing=val.to_float()*20.0f;}

        if (tf->get_member("color", &val))
		{
			int rgb = val.to_int();
			Uint8 r = (rgb >> 16) & 0xFF;
			Uint8 g = (rgb >> 8) & 0xFF;
			Uint8 b = rgb & 0xFF;
			m_color.set(r, g, b, 255);
		}

		if (tf->get_member("size", &val))
		{
			m_text_height = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("align", &val))
		{
			if (val.to_tu_string() == "left")
			{
				m_alignment = edit_text_character_def::ALIGN_LEFT;
			}
			else
			if (val.to_tu_string() == "center")
			{
				m_alignment = edit_text_character_def::ALIGN_CENTER;
			}
			else
			if (val.to_tu_string() == "right")
			{
				m_alignment = edit_text_character_def::ALIGN_RIGHT;
			}
			else
			if (val.to_tu_string() == "justify")
			{
				m_alignment = edit_text_character_def::ALIGN_JUSTIFY;
			}
		}

		if(!m_font)throw std::runtime_error("source reset-format reached null font");
        tu_string fontname = m_font->get_name();
		if (tf->get_member("font", &val))
		{
			fontname = val.to_tu_string();
		}

		bool bold = m_font->is_bold();
		if (tf->get_member("bold", &val))
		{
			bold = val.to_bool();
		}

		bool italic  = m_font->is_italic();
		if (tf->get_member("italic", &val))
		{
			italic = val.to_bool();
		}

		if (italic  != m_font->is_italic() ||
				bold  != m_font->is_bold() ||
				fontname  != m_font->get_name())
		{
			// try to find embedded font
			character_def* res = find_exported_resource(fontname);
			font* embedded_font = NULL;
			if (res)
			{
				embedded_font = cast_to<font>(res);
			}

			if (embedded_font)
			{
				// we have embedded font
				m_font = cast_to<font>(embedded_font);
			}
			else
			{
				m_font = new font(get_player());
			}
			m_font->set_bold(bold);
			m_font->set_italic(italic);
			m_font->set_name(fontname);
		}

		format_text(false);
	}

	root* edit_text_character::get_root()
	{
		return get_parent()->get_root(); 
	} 

	void edit_text_character::show_cursor(){m_source_text_v1->cursor();}
	void edit_text_character::display(){m_source_text_v1->display();}

	bool edit_text_character::on_event(const event_id& id){return m_source_text_v1->event(std::uint8_t(id.m_id),std::uint8_t(id.m_key_code));}

	bool edit_text_character::can_handle_mouse_event() 
	{
		return m_def->m_readonly == false;
	}

	bool edit_text_character::get_topmost_mouse_entity( character * &te, float x, float y)
	{ 
		if (get_visible() == false) 
		{ 
			return NULL; 
		} 

		const matrix&  m = get_matrix();

		point   p; 
		m.transform_by_inverse(&p, point(x, y)); 

		if (m_def->m_rect.point_test(p.m_x, p.m_y)) 
		{
			te = this;
			return true; 
		} 

		return false; 
	} 

	const tu_string&	edit_text_character::get_var_name() const
	{
		return m_def->m_var_name; 
	}

	void	edit_text_character::reset_bounding_box(float x, float y)
	// Reset our text bounding box to the given point.
	{
		m_text_bounding_box.m_x_min = x;
		m_text_bounding_box.m_x_max = x;
		m_text_bounding_box.m_y_min = y;
		m_text_bounding_box.m_y_max = y;
	}

	void edit_text_character::advance(float delta_time) 
	{ 
		to_string();
	}


	void	edit_text_character::set_text(const tu_string& new_text)
	// Local. Set our text to the given string.
	{source_set_text_v1(new_text,false,false);}

	void	edit_text_character::set_text_value(const tu_string& new_text)
	// Set our text to the given string.
	{source_set_text_v1(new_text,false,true);}

	const char*	edit_text_character::to_string()
	{
        if(!m_source_text_v1)throw std::runtime_error("source text sidecar unavailable");
        m_source_text_v1->refresh();return m_text.c_str();
    }


	bool	edit_text_character::set_member(const tu_stringi& name, const as_value& val)
	// We have a "text" member.
	{
        if(name=="text")source_set_text_v1(val.to_tu_string(),false,true);
        else if(name=="htmlText")source_set_text_v1(val.to_tu_string(),true,true);
        else if(name=="textColor"){const unsigned rgb=val.to_int();m_color.set(rgb>>16,rgb>>8,rgb,255);format_text(false);}
        else if(name=="backgroundColor"){const unsigned rgb=val.to_int();m_background_color.set(rgb>>16,rgb>>8,rgb,255);format_text(false);}
        else if(name=="border"){m_def->m_border=val.to_bool();format_text(false);}
        else if(name=="multiline"){m_def->m_multiline=val.to_bool();format_text(false);}
        else if(name=="wordWrap"){m_def->m_word_wrap=val.to_bool();format_text(false);}
        else if(name=="type"){if(val.to_tu_string()=="input")m_def->m_readonly=false;else if(val.to_tu_string()=="dynamic")m_def->m_readonly=true;}
        return character::set_member(name,val);
    }


	bool	edit_text_character::get_member(const tu_stringi& name, as_value* val)
	{
        if(name=="text"||name=="htmlText")val->set_tu_string(m_text);
        else if(name=="textWidth")val->set_double(float(m_text_bounding_box.width())/20.0f);
        else if(name=="textHeight")val->set_double(float(m_text_bounding_box.height())/20.0f);
        else if(name=="textColor")val->set_int((m_color.m_r<<16)+(m_color.m_g<<8)+m_color.m_b);
        else if(name=="backgroundColor")val->set_int((m_background_color.m_r<<16)|(m_background_color.m_g<<8)|m_background_color.m_b);
        else if(name=="border")val->set_bool(m_def->m_border);
        else if(name=="multiline")val->set_bool(m_def->m_multiline);
        else if(name=="wordWrap")val->set_bool(m_def->m_word_wrap);
        else if(name=="type")val->set_tu_string(m_def->m_readonly?"dynamic":"input");
        if(name=="setTextFormat"){val->set_as_c_function(set_textformat);return true;}
        if(name=="getTextFormat"){val->set_as_c_function(source_get_textformat_v1);return true;}
        // Original78af78 always performs method lookup then base lookup,
        // even after writing a recognized property into val.
        return character::get_member(name,val);
    }

	// @@ WIDTH_FUDGE is a total fudge to make it match the Flash player!  Maybe
	// we have a bug?
#define WIDTH_FUDGE 80.0f


	void	edit_text_character::align_line(edit_text_character_def::alignment align, int last_line_start_record, float x)
	// Does LEFT/CENTER/RIGHT alignment on the records in
	// m_text_glyph_records[], starting with
	// last_line_start_record and going through the end of
	// m_text_glyph_records.
	{
		float	extra_space = (m_def->m_rect.width() - m_right_margin) - x - WIDTH_FUDGE;

		// extra_space may be < 0
		// assert(extra_space >= 0.0f);

		float	shift_right = 0.0f;

		if (align == edit_text_character_def::ALIGN_LEFT)
		{
			// Nothing to do; already aligned left.
			return;
		}
		else if (align == edit_text_character_def::ALIGN_CENTER)
		{
			// Distribute the space evenly on both sides.
			shift_right = extra_space / 2;
		}
		else if (align == edit_text_character_def::ALIGN_RIGHT)
		{
			// Shift all the way to the right.
			shift_right = extra_space;
		}

		// Shift the beginnings of the records on this line.
		for (int i = last_line_start_record; i < m_text_glyph_records.size(); i++)
		{
			text_glyph_record&	rec = m_text_glyph_records[i];

			if (rec.m_style.m_has_x_offset)
			{
				rec.m_style.m_x_offset += shift_right;
			}
		}

		// This fix the cursor position with centered texts
		// Thanks Francois Guibert
		m_xcursor += shift_right; 
	}

	// Convert the characters in m_text into a series of
	// text_glyph_records to be rendered.
	void	edit_text_character::format_text()
	{format_text(false);}

	// HTML content
	// <p> ... </p> Defines a paragraph.
	//		The attribute align may be present, with value left, right, or center.
	// <br> Inserts a line break.
	// <a> ... </a> Defines a hyperlink.
	// <font> ... </font> Defines a span of text that uses a given font. 
	//		• face, which specifies a font name
	//		• size, which is specified in twips, and may include a leading ‘+’ or ‘-’ for relative sizes
	//		• color, which is specified as a #RRGGBB hex triplet
	// <b> ... </b> Defines a span of bold text.
	// <i> ... </i> Defines a span of italic text.
	// <u> ... </u> Defines a span of underlined text.
	// <li> ... </li> Defines a bulleted paragraph.
	// <textformat> ... </textformat>
	// <tab> Inserts a tab character
	//
	// sample:
	// <p align="left"><font face="Times New Roman" size="74" color="#cccccc"
	// letterSpacing="0.000000" kerning="0">aaa</font></p>
	// <p align="left"><font face="Courier" size="74" color="#ff0000"
	// letterSpacing="0.000000" kerning="0">bbb</font></p>

	// paragraph parser
	const char*	edit_text_character::html_paragraph(const char* p, text_glyph_record& rec)
	{
//		printf("html paragraph\n");
		const char* pend = p + strlen(p);
		while (p <= pend)
		{
			if (strncmp(p, "<p", 2) == 0)
			{
				html_paragraph(p + 3, rec);
			}
			else
			if (strncmp(p, "</p>", 4) == 0)
			{
				return p + 4;
			}
			else
			if (strncmp(p, "align=", 6) == 0)
			{
				p += 7;
				if (strncmp(p, "left", 4) == 0)
				{
					m_alignment = edit_text_character_def::ALIGN_LEFT;
					p += 5;
				}
				else
				if (strncmp(p, "center", 6) == 0)
				{
					m_alignment = edit_text_character_def::ALIGN_CENTER;
					p += 7;
				}
				else
				if (strncmp(p, "right", 5) == 0)
				{
					m_alignment = edit_text_character_def::ALIGN_RIGHT;
					p += 6;
				}
				else
				{
					assert(0);	// invalid align
				}

				assert(*p == '>');
				p++;
			}
			else
			if (strncmp(p, "<font ", 5) == 0)
			{
				p = html_font(p + 6, rec);
			}
			else
			{
				p++;
			}
		}

		//assert(0);
		return NULL;
	}

	// text parser
	const char*	edit_text_character::html_text(const char* p, text_glyph_record& rec)
	{
		// keep old values
		bool saved_bold = m_font->is_bold();
		bool saved_italic = m_font->is_italic();

		// on default is 'false'
		rec.m_style.m_font->set_bold(false);
		rec.m_style.m_font->set_italic(false);

		tu_string text;

		const char* pend = p + strlen(p);
		while (true)
		{
			assert(p <= pend);

			if (strncmp(p, "<b>", 3) == 0)
			{
				p += 3;
				rec.m_style.m_font->set_bold(true);
			}
			else
			if (strncmp(p, "<i>", 3) == 0)
			{
				p += 3;
				rec.m_style.m_font->set_italic(true);
			}
			else
			if (strncmp(p, "</i>", 4) == 0)
			{
				if (text.size() > 0)
				{
					format_plain_text(text, rec);
					text = "";
				}
				p += 4;
				rec.m_style.m_font->set_italic(false);
			}
			else
			if (strncmp(p, "</b>", 4) == 0)
			{
				if (text.size() > 0)
				{
					format_plain_text(text, rec);
					text = "";
				}
				p += 4;
				rec.m_style.m_font->set_bold(false);
			}
			else
			if (*p == '<')
			{
				if (text.size() > 0)
				{
					format_plain_text(text, rec);
					text = "";
				}
				return p;
			}
			else
			{
				if (strncmp(p, "&nbsp;", 5) == 0)
				{
					text += " ";
					p += 6;
				}
				else
				{
					// plain text
					text += *p;
					p++;
				}
			}
		}

		rec.m_style.m_font->set_bold(saved_bold);
		rec.m_style.m_font->set_italic(saved_italic);

		return p;
	}

	// font parser
	const char*	edit_text_character::html_font(const char* p, text_glyph_record& rec)
	{
		// Start a new record with a new style
		if (rec.m_glyphs.size() > 0)
		{
			m_text_glyph_records.push_back(rec);
			rec.m_glyphs.resize(0);
			rec.m_style.m_x_offset = m_x;
			rec.m_style.m_y_offset = m_y;
		}

		const char* pend = p + strlen(p);
		while (true)
		{
			assert(p <= pend);

			if (*p == ' ')
			{
				p++;
			}
			else
			if (*p == '>')	// begin of text
			{
				p = html_text(p + 1, rec);
			}
			else
			if (strncmp(p, "</font>", 7) == 0)
			{
				if (rec.m_style.m_lastfont != NULL)
				{
					rec.m_style.m_font = rec.m_style.m_lastfont;
					rec.m_style.m_lastfont = NULL;
				}
				return p + 7;
			}
			else
			if (strncmp(p, "face=", 5) == 0)
			{
				p +=6;
				const char* end = strchr(p, '"');
				assert(end);
				int len = int(end - p);
				tu_string face(p, len);
				p += len + 1;

				movie_def_impl* md = cast_to<movie_def_impl>(m_def->m_root_def);
				assert(md);
				font* f = md->find_font(face.c_str());
				if (f)
				{
					rec.m_style.m_lastfont = rec.m_style.m_font;
					rec.m_style.m_font = f;
				}
				else
				{
					log_error("html_font: can't find font, face = '%s'\n", face.c_str());
				}

			}
			else
			if (strncmp(p, "size=", 5) == 0)
			{
				p += 6;
				const char* end = strchr(p, '"');
				int len = int(end - p);
				tu_string size(p, len);
				p += len + 1;

				// reset size
				double res;
				string_to_number(&res, size.c_str());
				rec.m_style.m_text_height = static_cast<float>(PIXELS_TO_TWIPS(res));

				// reset advance
				rec.m_style.m_scale =  rec.m_style.m_text_height / 1024.0f;
				if (m_font->is_define_font3())
				{
					rec.m_style.m_scale /= 20.0f;
				}
			}
			else
			if (strncmp(p, "color=", 6) == 0)
			{
				p += 7;
				assert(*p == '#');
				++p;
				
				Uint32 rgb = strtol(p, 0, 16);
				rec.m_style.m_color.m_r = rgb >> 16;
				rec.m_style.m_color.m_g = (rgb >> 8) & 0xFF;
				rec.m_style.m_color.m_b = rgb & 0xFF;

				p += 7;
			}
			else
			if (strncmp(p, "letterSpacing=", 14) == 0)
			{
				p += 15;
				const char* end = strchr(p, '"');
				int len = int(end - p);
				tu_string letterSpacing(p, len);
				p += len + 1;
			}
			else
			if (strncmp(p, "kerning=", 8) == 0)
			{
				p += 9;
				const char* end = strchr(p, '"');
				int len = int(end - p);
				tu_string letterSpacing(p, len);
				p += len + 1;
			}
			else
			if (strncmp(p, "<font ", 5) == 0)
			{
				p = html_font(p + 6, rec);
			}
			else if (strncmp(p, "<a ", 3) == 0)
			{
				p += 3;
				log_error("html_font: Define a hyperlink is not implemented yet\n");

				// skip
				for (; strncmp(p, "</a>", 4); p++) {}
				p += 4;
			}
			else
			{
				p = html_text(p, rec);
				/*log_error("html_font: unknown keyword %s\n", p);
				assert(0);*/
			}
		}

		assert(0);
		return NULL;
	}

	bool	edit_text_character::format_html_text(text_glyph_record& rec)
	{
//		printf("%s\n", m_text.c_str());
		const char* p = m_text.c_str();

		if (strncmp(p, "<p", 2) != 0)
		{
			// it is not html text
			return false;
		}

		do
		{
			p = html_paragraph(p + 3, rec);
		}
		while (p != NULL && strncmp(p, "<p", 2) == 0);

		return true;
	}

	void	edit_text_character::format_plain_text(const tu_string& text, text_glyph_record&	rec)
	{(void)text;(void)rec;format_text(false);}

	character*	edit_text_character_def::create_character_instance(character* parent, int id)
	{
		if (m_font == NULL)
		{
			// Resolve the font, if possible.
			if (m_root_def)
			{
				m_font = m_root_def->get_font(m_font_id);
				if (m_font == NULL)
				{
					log_error("error: text style with undefined font; font_id = %d\n", m_font_id);
				}
			}
		}

		edit_text_character*	ch = new edit_text_character(get_player(), parent, this, id);
		// instanciate_registered_class(ch);	//TODO: test it

		return ch;
	}


	void	define_edit_text_loader(stream* in, int tag_type, movie_definition_sub* m)
	// Read a DefineText tag.
	{
		assert(tag_type == 37);

		Uint16	character_id = in->read_u16();

		edit_text_character_def*	ch = new edit_text_character_def(m->get_player(), m);
		IF_VERBOSE_PARSE(log_msg("edit_text_char, id = %d\n", character_id));
		ch->read(in, tag_type, m);

		m->add_character(character_id, ch);
	}

}	// end namespace gameswf


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
