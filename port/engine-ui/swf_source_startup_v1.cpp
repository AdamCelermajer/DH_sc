#include "swf_source_startup_v1.hpp"
namespace dh2::ui {
bool swf_source_startup_owned_v1(const SwfServices&s) noexcept {
 return swf_source_movie_startup_owned_v1(s)||
        swf_input_session_startup_owned_v1(s)||
        swf_input_session_startup_owned_v2(s);
}
}
