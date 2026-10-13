#pragma once
// B158 test aid (harness only, never active without --follow-actor): a scripted hero cannot walk after a boss that relocates
// (the Mud Lord dives and re-emerges at spawn points 01/03/04/05), so the continuous afterwitch -> lever -> intro -> fight run
// could not land enough hits. Like --player-to it places the hero through the SetActorPosition owner; it only runs inside a frame
// window and only when the hero is farther than the follow distance from a live target actor.
#include <cmath>
#include <cstdlib>
#include <optional>
#include <stdexcept>
#include <string>

namespace dh::foundation::test_aids {

// B158 (mudlord3): the smallest follow distance that keeps the hero outside the boss body. Run m3f1 (distance 170) placed the
// hero inside the boss body (boss body radius 224.06 + hero controller radius 65.45 = 289.5, from the run log): the hero did not
// move while the boss was pushed away from him. Distance 300 (run m3f2/m3f4) kept the hero in reach for the whole fight.
inline constexpr float kFollowContactDistance = 290.0f;

struct FollowActorAidV1 {
    std::string profile;      // population profile id of the actor to follow (e.g. SwampKing)
    int from_frame = 0;       // inclusive
    int to_frame = 0;         // exclusive
    float distance = 300.0f;  // hero is placed this far from the target when farther than distance * 1.5
    int period_frames = 30;   // re-evaluated every period (a player re-approaches, not every frame)
};

// "PROFILE@FROM-TO[:DISTANCE]"
inline FollowActorAidV1 parse_follow_actor_aid_v1(const std::string& spec) {
    FollowActorAidV1 aid;
    const auto at = spec.find('@');
    const auto dash = at == std::string::npos ? at : spec.find('-', at + 1);
    if (at == std::string::npos || at == 0 || dash == std::string::npos || dash == at + 1 || dash + 1 >= spec.size())
        throw std::runtime_error("--follow-actor expects PROFILE@FROM-TO[:DISTANCE]");
    aid.profile = spec.substr(0, at);
    aid.from_frame = std::stoi(spec.substr(at + 1, dash - at - 1));
    const auto colon = spec.find(':', dash + 1);
    aid.to_frame = std::stoi(spec.substr(dash + 1, colon == std::string::npos ? std::string::npos : colon - dash - 1));
    if (colon != std::string::npos) aid.distance = std::stof(spec.substr(colon + 1));
    if (aid.to_frame <= aid.from_frame || !(aid.distance > 0)) throw std::runtime_error("--follow-actor window or distance is empty");
    if (aid.distance < kFollowContactDistance)
        throw std::runtime_error("--follow-actor distance must be at least 290 (inside the boss body the placement is not applied)");
    return aid;
}

struct FollowPlacementV1 { float x, y, z; };

// Returns where to place the hero (the same height, on the line from the target toward the hero, `distance` away from the
// target), or nothing when the aid is inactive this frame, the hero is already near, or the target is dead/absent.
inline std::optional<FollowPlacementV1> follow_actor_placement_v1(const FollowActorAidV1& aid, int frame, bool target_alive,
                                                                  const float hero[3], const float target[3]) {
    if (frame < aid.from_frame || frame >= aid.to_frame || aid.period_frames <= 0 || (frame - aid.from_frame) % aid.period_frames != 0 ||
        !target_alive)
        return std::nullopt;
    const float dx = hero[0] - target[0], dy = hero[1] - target[1];
    const float length = std::sqrt(dx * dx + dy * dy);
    if (length <= aid.distance * 1.5f) return std::nullopt;
    const float ux = length > 0.001f ? dx / length : 1.0f, uy = length > 0.001f ? dy / length : 0.0f;
    return FollowPlacementV1{target[0] + ux * aid.distance, target[1] + uy * aid.distance, hero[2]};
}

} // namespace dh::foundation::test_aids
