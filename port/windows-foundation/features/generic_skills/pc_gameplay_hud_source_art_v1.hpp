#pragma once

#include "../../hud_geometry.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace dh::foundation::generic_skills {

// Shape and pixel/UV triangles decoded from the original dqhud_droid movie.
// source_frame is the zero-based SWF frame, before the source's gotoAndStop
// one-based conversion. Empty frame8 is retained as authored.
struct PcGameplayHudSourceFaeryIconV1 {
    std::uint32_t source_frame{};
    std::vector<std::uint32_t> source_shape_ids;
    std::vector<HudGeometryVertex> triangles;
};

const std::array<PcGameplayHudSourceFaeryIconV1, 13>&
original_pc_gameplay_hud_faery_icons_v1() noexcept;

// Original HealthBars.btn_potion.btframe -> sprite106 -> shape105 pixels.
const std::vector<HudGeometryVertex>&
original_pc_gameplay_hud_potion_icon_v1() noexcept;

} // namespace dh::foundation::generic_skills
