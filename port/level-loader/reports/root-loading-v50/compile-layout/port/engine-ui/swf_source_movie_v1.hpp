#pragma once
#include "swf_movie.hpp"
namespace dh2::ui {
class SwfFrameConnection;
struct SwfSourceFrameBorrowV1 {
 std::shared_ptr<SwfInputHistory> history;
 SwfFrameConnection* frames{};
};
// Borrow the existing wrapper's sole history/frame owners before load. The
// wrapped services/movie retain their lifetime; do not wrap or bind them twice.
bool source_movie_frame_borrow_v1(const SwfServices&,SwfSourceFrameBorrowV1&,std::string&);
// Prepare LEGACY facade services before load with actual source history/frame
// owners. It holds no strong graph/player/root lease. Caller context lifetime
// retains the facade's existing borrowed contract; typed native owners remain
// required. Do not wrap services already owned by SwfInputSession.
bool source_movie_services_v1(const SwfServices& original,SwfServices& wrapped,
                             std::string& error);
}
