#include "../../swf_input_session_v2.cpp"
#include "../../swf_source_startup_v1.hpp"
namespace dh2::ui {
bool swf_input_session_startup_owned_v2(const SwfServices&s) noexcept {
 return s.graph_start==SwfInputSessionV2::Provider::start;
}
}
