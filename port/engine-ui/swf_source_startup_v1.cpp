#include "swf_source_startup_v1.hpp"
#include "swf_text_font_platform_v1.hpp"
namespace dh2::ui {
bool swf_source_startup_owned_v1(const SwfServices&s) noexcept {
 return swf_source_movie_startup_owned_v1(s)||
        swf_input_session_startup_owned_v1(s)||
        swf_input_session_startup_owned_v2(s)||SwfTextFontPlatformV1::owns_source_startup(s);
}
}
