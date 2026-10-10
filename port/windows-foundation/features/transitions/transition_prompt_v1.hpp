#pragma once
// P17 TRANSITIONS2: the travel prompt of a level exit.
//
// Original (IDA, TriggerZoneExitLevel::Update 0039c33c): while the local host touches the zone and the zone's
// condition holds, DisplayFastTravel(true, LevelList display text, levelName, entryId) is invoked on the HUD root.
// The HUD movie shows the "Travel to:" text (ingame.english INGAME_GO_TO_LEVEL) with the level's display name,
// and its button calls NativeGoToZone(levelName, entryId). Leaving the zone calls DisplayFastTravel(false, ...).
//
// PC adaptation: the button is the interact key (E). The art below is a PLACEHOLDER frame (a dark panel with a
// gold border), not the dqhud_droid travel panel. The HUD owner (HUDART) replaces build_prompt_frame's shapes with
// the original panel; the text, the show/hide rule and the confirm path stay as they are.

#include "level_transitions_v1.hpp"

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
    std::string target_display;  // LevelList display text (falls back to the level name)
    std::string target_file;     // data/scene/<file>
    int entry_id = 0;
};

// The first built level_exit zone that the player is inside and whose condition holds. Procedural targets have
// no EXE path yet, so they never show a prompt (the test-hook path still reports the refusal).
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
};
struct PromptFrame {
    std::vector<PromptRect> rects;
    std::vector<PromptText> texts;
};
PromptFrame build_prompt_frame(const ArmedExit& exit);

} // namespace dh::foundation::transitions
