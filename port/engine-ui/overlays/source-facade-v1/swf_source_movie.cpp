#include "../../swf_source_movie_v1.cpp"
#include "../../swf_source_startup_v1.hpp"
namespace dh2::ui {
bool swf_source_movie_startup_owned_v1(const SwfServices&s) noexcept {
 return s.graph_start==SourceOwner::start;
}
}
