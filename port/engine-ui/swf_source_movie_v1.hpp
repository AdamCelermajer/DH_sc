#pragma once
#include "swf_movie.hpp"
namespace dh2::ui {
// Prepare LEGACY facade services before load with actual source history/frame
// owners. It holds no strong graph/player/root lease. Caller context lifetime
// retains the facade's existing borrowed contract; typed native owners remain
// required. Do not wrap services already owned by SwfInputSession.
bool source_movie_services_v1(const SwfServices& original,SwfServices& wrapped,
                             std::string& error);
}
