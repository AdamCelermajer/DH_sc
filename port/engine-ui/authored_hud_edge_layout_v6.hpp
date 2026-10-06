#pragma once
#include "authored_gameplay_hud_v1.hpp"
#include <functional>
namespace dh2::ui {
// Modern wide/tall display policy. Temporarily translate actual authored HUD
// groups to their screen edges, then restore their source matrices. The same
// scoped translations serve rendering and original shape-based hit testing.
bool with_authored_hud_edge_layout_v6(SwfMovie&,const AuthoredGameplayHudV1&,
 const std::function<bool(std::string&)>& operation,std::string&);
}
