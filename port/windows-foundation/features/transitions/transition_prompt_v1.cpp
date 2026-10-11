// P17 TRANSITIONS2: travel prompt selection and original destination button (see the header for the original rule).
#include "transition_prompt_v1.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation::transitions {

std::optional<ArmedExit> find_armed_exit(const ExitZoneSet& zones,
                                         const std::vector<LevelRow>& levels,
                                         ConditionSource& conditions) {
    const auto& all = zones.zones();
    for (std::size_t i = 0; i < all.size(); ++i) {
        const auto& zone = all[i];
        if (!zone.built || zone.kind != ZoneKind::level_exit || !zone.inside) continue; // procedural targets are generated on confirm (P17 PROCGEN)
        if (!zone.condition.empty() && !conditions.condition_active(zone.condition)) continue;
        ArmedExit armed;
        armed.zone = i;
        armed.zone_name = zone.name;
        armed.target_name = zone.target_name;
        armed.target_file = zone.target_file;
        armed.entry_id = zone.entry_id;
        const auto* row = find_level(levels, zone.target_name);
        armed.target_display = zone.target_name;
        armed.display_string_id = row ? row->display_string_id : -1;
        return armed;
    }
    return std::nullopt;
}

#include "transition_prompt_art_v1.inc"

PromptFrame build_prompt_frame(const ArmedExit& exit) {
    PromptFrame frame;
    frame.art = &travelButtonArt;
    auto destination = travelDestination;
    destination.text = exit.target_display;
    frame.texts.push_back(std::move(destination));
    // PC adaptation: dedicated travel key beside the authored control. No extra original caption.
    frame.texts.push_back({320.f, 228.45f, 24.f, 15.9f, "[E]", {255,255,255,255}, 10.f, 0});
    return frame;
}

float prompt_text_fit(const PromptText& text, float measured_width) noexcept {
    // EditText keeps a two-pixel internal gutter on either side (source text layout WIDTH_FUDGE).
    const float available = text.w - 4.f;
    if (!std::isfinite(measured_width) || measured_width < 0.f ||
        !std::isfinite(available) || available <= 0.f) return 0.f;
    return measured_width <= available ? 1.f : available / measured_width;
}

} // namespace dh::foundation::transitions
