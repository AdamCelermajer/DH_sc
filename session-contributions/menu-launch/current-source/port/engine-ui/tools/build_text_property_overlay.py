"""Generate a reviewable HTML property bridge without modifying frozen vendor sources.

The original setter dispatches text/plain and htmlText/HTML to its bound-variable
text producer. HTML then reaches reconstructed text_layout_v1 with real font/glyph
services; inline image and underline display producers remain explicit errors.
"""
from pathlib import Path

root=Path(__file__).resolve().parents[1]
source=root/'vendor/gameswf1714/gameswf/gameswf_text.cpp'
text=source.read_text(encoding='cp1252')
text='#include "swf_text_layout_connection.hpp"\n#include <stdexcept>\n'+text
setter='\tbool\tedit_text_character::set_member(const tu_stringi& name, const as_value& val)\n\t// We have a "text" member.\n\t{'
getter='\tbool\tedit_text_character::get_member(const tu_stringi& name, as_value* val)\n\t{'
formatter='\tvoid\tedit_text_character::format_text()\n\t{'
assert text.count(setter)==1 and text.count(getter)==1
text=text.replace(setter,setter+'''
        // Original ARM setter +0x168 reaches set_text_value(..., true).
        // Preserve the real field and bound-variable update, rather than
        // leaving htmlText as an ordinary AS string that cannot be drawn.
        if (name == "htmlText")
        {
            builtin_member("__dh2_original_html", as_value(true));
            set_text_value(val.to_tu_string());
            std::string error;
            if (!dh2::ui::swf_original_text_layout(this, true, error))
                throw std::runtime_error(error);
            return character::set_member(name, val);
        }
        if (name == "text") builtin_member("__dh2_original_html", as_value(false));
''')
assert text.count(formatter)==1
text=text.replace(formatter,formatter+'''
        // Stock display() reformats when the world matrix changes. Keep the
        // explicit field mode and original formatter on that retained path.
        as_value html_mode;
        if (as_object::get_member("__dh2_original_html", &html_mode) && html_mode.to_bool())
        {
            std::string error;
            if (!dh2::ui::swf_original_text_layout(this, true, error))
                throw std::runtime_error(error);
            return;
        }
''')
text=text.replace(getter,getter+'''
        // Original getter 0x78b000/0x78afd0 reads the formatter's retained
        // text bounding box and converts twips to pixels. About's authored
        // onEnterFrame uses textHeight to scroll and wrap its credits.
        if (name == "textHeight")
        {
            val->set_double((m_text_bounding_box.m_y_max - m_text_bounding_box.m_y_min) / 20.0f);
            return true;
        }
        if (name == "textWidth")
        {
            val->set_double((m_text_bounding_box.m_x_max - m_text_bounding_box.m_x_min) / 20.0f);
            return true;
        }
        if (name == "htmlText")
        {
            val->set_tu_string(m_text);
            return true;
        }
''')
mask_start='''		//  text should not exceed the bounds of the box
		render::begin_submit_mask();
		render::fill_style_color(0,	m_background_color);
		render::draw_mesh_strip(&icoords[0], 4); 
		render::end_submit_mask();'''
mask_end='''		// turn off mask
		render::disable_mask();'''
assert text.count(mask_start)==1 and text.count(mask_end)==1
text=text.replace(mask_start,'''
        // Original edit_text_character::display 0x792928..0x792a6c draws
        // normal glyph records without the stock field rectangle mask. That
        // path does not branch on plain vs HTML text. Plain confirmation text
        // can extend beyond its authoring rectangle too; ancestor masks stay
        // active. Keep the unproven stock border path's existing policy.
        const bool stock_clip = m_def->m_border;
        if (stock_clip) {
            render::begin_submit_mask();
            render::fill_style_color(0, m_background_color);
            render::draw_mesh_strip(&icoords[0], 4);
            render::end_submit_mask();
        }''')
text=text.replace(mask_end,'\t\tif (stock_clip) render::disable_mask();')
target=root/'overlays/text-property-v1/gameswf_text.cpp'
target.write_text(text,encoding='utf-8',newline='\n')
print(target)
