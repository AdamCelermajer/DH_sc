#include "../../swf_input_session_v1.cpp"
#include "../../swf_source_startup_v1.hpp"
namespace dh2::ui {
bool swf_input_session_startup_owned_v1(const SwfServices&s) noexcept {
 return s.graph_start==SwfInputSessionV1::Provider::start;
}
}
