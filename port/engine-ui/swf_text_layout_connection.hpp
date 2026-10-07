#pragma once
#include <string>
namespace gameswf {struct edit_text_character;}
namespace dh2::ui {
// Connect original layout records to the retained field and real scoped glyph
// provider. Does not parse HTML with the upstream formatter.
bool swf_original_text_layout(gameswf::edit_text_character*,bool,std::string&);
}
