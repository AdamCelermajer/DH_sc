#pragma once
#include "authored_gameplay_hud_v1.hpp"
namespace dh2::ui {
struct AuthoredHudPortraitDiagnosticV4 {
 SwfClipInfo button,bitmap_container,portrait;
 float shape_world_bounds[4]{};
 float display_rectangle[4]{};
 std::int32_t viewport[4]{};
 std::int32_t actual_shape_id{};
};
// Read-only same-movie measurement. No placement correction or viewport owner.
bool authored_hud_portrait_v4(SwfMovie&,const AuthoredGameplayHudV1&,
 AuthoredHudPortraitDiagnosticV4&,std::string&);
struct AuthoredHudPortraitAlignmentV5 {
 std::int32_t shape{};float before_world[2]{},target_world[2]{},after_world[2]{},delta_local[2]{};
};
// Modern presentation correction: align decoded portrait paint with the
// decoded ornate aperture on SAME current frame155. Not original AS parity.
bool authored_hud_portrait_align_v5(SwfMovie&,const AuthoredGameplayHudV1&,
 AuthoredHudPortraitAlignmentV5&,std::string&);
}
