#pragma once
// P17 TRANSITIONS2: the travel prompt of a level exit.
//
// Original (IDA, TriggerZoneExitLevel::Update 0039c33c): while the local host touches the zone and the zone's
// condition holds, DisplayFastTravel(true, LevelList display text, levelName, entryId) is invoked on the HUD root.
// The HUD movie shows the localized destination name directly on LocationsMenu.btn_LocTravel,
// and its button calls NativeGoToZone(levelName, entryId). Leaving the zone calls DisplayFastTravel(false, ...).
//
// B113: original LocationsMenu.btn_LocTravel art and destination-only text. E is the PC input adaptation.
// The independent btn_FastTravel / World map menu route is outside this destination prompt.

#include "level_transitions_v1.hpp"
#include "../generic_skills/pc_gameplay_hud_button_art_v1.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dh::foundation::transitions {

// The exit whose prompt is on screen this frame.
struct ArmedExit {
    std::size_t zone = ExitZoneSet::npos;
    std::string zone_name;
    std::string target_name;     // LevelList name (levelName)
    std::string target_display;  // destination button: caller swaps in the localized LevelList display text
    std::int32_t display_string_id = -1; // LevelList display text id (localize with the menu text owner)
    std::string target_file;     // data/scene/<file>
    int entry_id = 0;
};

// The first built level_exit zone that the player is inside and whose condition holds. Procedural targets
// are generated on confirm (P17 PROCGEN); they use the same prompt as fixed levels.
std::optional<ArmedExit> find_armed_exit(const ExitZoneSet& zones,
                                         const std::vector<LevelRow>& levels,
                                         ConditionSource& conditions);

// Stage units: 480 x 320 (the original HUD stage). Caller maps them to the window like the cinematic boxes.
struct PromptRect {
    float x = 0, y = 0, w = 0, h = 0;
    std::array<std::uint8_t, 4> rgba{0, 0, 0, 255};
};
struct PromptText {
    float x = 0, y = 0, w = 0, h = 0;
    std::string text;
    std::array<std::uint8_t, 4> rgba{255, 255, 255, 255};
    float height = 10.f;
    int align = 2;
};
struct PromptFrame {
    std::vector<PromptRect> rects; // empty: source art replaces the old placeholder
    const std::vector<generic_skills::PcGameplayHudArtBatchV1>* art = nullptr;
    std::vector<PromptText> texts;
};
PromptFrame build_prompt_frame(const ArmedExit& exit);
// Measured-width one-line PC fit, bounded by the authored EditText width. Invalid metrics reject.
float prompt_text_fit(const PromptText& text, float measured_width) noexcept;

} // namespace dh::foundation::transitions

