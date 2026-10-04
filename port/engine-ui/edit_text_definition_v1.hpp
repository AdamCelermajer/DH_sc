#pragma once
#include <string>
namespace dh2::ui::edit_text_definition_v1 {
// Original78b4d8 strips the content preceding the FIRST closing-tag opener
// and following the preceding '>'. It is not a general HTML sanitizer.
bool remove_html(std::string&,std::string&);
}
