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
        if (name == "htmlText")
        {
            val->set_tu_string(m_text);
            return true;
        }
''')
target=root/'overlays/text-property-v1/gameswf_text.cpp'
target.write_text(text,encoding='utf-8',newline='\n')
print(target)
