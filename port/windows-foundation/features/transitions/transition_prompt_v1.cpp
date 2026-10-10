// P17 TRANSITIONS2: travel prompt selection and placeholder frame (see the header for the original rule).
#include "transition_prompt_v1.hpp"

namespace dh::foundation::transitions {

std::optional<ArmedExit> find_armed_exit(const ExitZoneSet& zones,
                                         const std::vector<LevelRow>& levels,
                                         ConditionSource& conditions) {
    const auto& all = zones.zones();
    for (std::size_t i = 0; i < all.size(); ++i) {
        const auto& zone = all[i];
        if (!zone.built || zone.kind != ZoneKind::level_exit || !zone.inside || zone.target_procedural) continue;
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

PromptFrame build_prompt_frame(const ArmedExit& exit) {
    // PLACEHOLDER art: dark panel, gold frame. Text positions are the frame's own layout (stage units).
    PromptFrame frame;
    frame.rects.push_back({130.f, 196.f, 220.f, 52.f, {0, 0, 0, 184}});          // panel
    frame.rects.push_back({130.f, 196.f, 220.f, 1.f, {168, 128, 64, 255}});       // gold frame, top
    frame.rects.push_back({130.f, 247.f, 220.f, 1.f, {168, 128, 64, 255}});       // gold frame, bottom
    frame.texts.push_back({140.f, 202.f, 200.f, 14.f, "Travel to:", {255, 230, 171, 255}});
    frame.texts.push_back({140.f, 218.f, 200.f, 20.f, exit.target_display, {255, 255, 255, 255}});
    frame.texts.push_back({300.f, 202.f, 44.f, 14.f, "[E]", {191, 191, 191, 255}});  // PC key hint
    return frame;
}

} // namespace dh::foundation::transitions
