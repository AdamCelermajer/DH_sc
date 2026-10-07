#pragma once
#include "swf_movie.hpp"
namespace dh2::ui {
struct HurtCornersLayoutV7 {
 float stage[4]{},display[4]{},paint_bounds[4]{};
 std::int32_t viewport[4]{};
};
// Modern fullscreen treatment of the actual single authored blood shape.
// Temporarily map its same outer sprite from movie stage to the real display
// rectangle, preserving its HP frame, nested pulse, texture and alpha.
bool display_authored_hurt_corners_v7(SwfMovie&,HurtCornersLayoutV7*,std::string&);
}
