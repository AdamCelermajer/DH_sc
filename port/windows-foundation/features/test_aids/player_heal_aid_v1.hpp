#pragma once
// act1rogue TEST ONLY harness aid (never active without --test-player-heal). A melee Rogue (177 HP) cannot survive the Mud Lord's
// 87-damage bites for the length of a real fight, and --test-player-hp N (N > max) makes every later save fail the validator
// ("health exceeds max_health"). This aid instead tops the live hero up to MAX HP (never above it, never reviving a dead hero)
// every PERIOD frames inside a frame window, so saves, transitions and quest scripts stay valid. Each run records the frames where
// the hero would have died without it (the pre-heal HP is logged by the host).
#include <stdexcept>
#include <string>

namespace dh::foundation::test_aids {

struct PlayerHealAidV1 {
    int from_frame = -1;  // inclusive
    int to_frame = -1;    // exclusive
    int period_frames = 15;
    bool due(int frame) const { return from_frame >= 0 && frame >= from_frame && frame < to_frame && (frame - from_frame) % period_frames == 0; }
};

// "FROM-TO[:PERIOD]"
inline PlayerHealAidV1 parse_player_heal_aid_v1(const std::string& spec) {
    const auto dash = spec.find('-');
    const auto colon = spec.find(':');
    if (dash == std::string::npos || dash == 0 || dash + 1 >= spec.size())
        throw std::runtime_error("--test-player-heal expects FROM-TO[:PERIOD]");
    PlayerHealAidV1 aid;
    aid.from_frame = std::stoi(spec.substr(0, dash));
    aid.to_frame = std::stoi(spec.substr(dash + 1, colon == std::string::npos ? std::string::npos : colon - dash - 1));
    if (colon != std::string::npos) aid.period_frames = std::stoi(spec.substr(colon + 1));
    if (aid.from_frame < 0 || aid.to_frame <= aid.from_frame || aid.period_frames <= 0)
        throw std::runtime_error("--test-player-heal window or period is empty");
    return aid;
}

} // namespace dh::foundation::test_aids
