#pragma once
#include "swf_movie.hpp"
namespace dh2::ui {
// Explicit implementation identity, not an inferred callback/flag scan.
// These predicates are supplied by include-wrapper TUs compiling the exact
// frozen source owner/session implementations once instead of their old TUs.
bool swf_source_movie_startup_owned_v1(const SwfServices&) noexcept;
bool swf_input_session_startup_owned_v1(const SwfServices&) noexcept;
bool swf_input_session_startup_owned_v2(const SwfServices&) noexcept;
bool swf_source_startup_owned_v1(const SwfServices&) noexcept;
}
