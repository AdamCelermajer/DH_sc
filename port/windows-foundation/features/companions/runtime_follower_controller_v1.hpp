#pragma once
// B077: per-frame follower controller over the live CombatSession. Selects actors purely by their AI row script
// (follower / attacking_follower / rene, see follower_decision_v1.hpp) and drives them toward the host player with
// the shared sewn-floor route + locomotion owner (RuntimeEnemyNavigationV1); it never writes positions itself
// except through the injected warp service.
#include "follower_decision_v1.hpp"
#include "../../combat_session.hpp"

#include <array>
#include <functional>
#include <map>
#include <string>

namespace dh::foundation::companions {

struct RuntimeFollowerServicesV1 {
    // MoveTo(master): continue/start the route and select walking. `haste` selects the catch-up speed. Sets
    // `route_found` (HasPath). Required.
    std::function<bool(CombatSession&, ActorState& owner, ActorState& master, const dh2::data::AiProps&,
                       bool haste, bool& route_found, std::string& error)> approach;
    // Stop(): drop the route and select idle. Required.
    std::function<bool(CombatSession&, ActorState&, std::string& error)> stop;
    // Warp the actor (physical body moves with it). Required.
    std::function<bool(CombatSession&, ActorState&, const std::array<float, 3>&, std::string& error)> warp;
    // Optional: master body radius (game units); default 0.
    std::function<float(const ActorState&)> body_radius;
    // Optional: false while the actor is hidden/not enabled in the level population.
    std::function<bool(ActorId)> enabled;
    // Optional global gate (false during cutscenes: scripts own the actors).
    std::function<bool()> active;
};

struct RuntimeFollowerReportV1 {
    std::size_t ticks = 0, approaches = 0, stops = 0, warps_behind = 0, warps_to_master = 0, haste_on = 0, haste_off = 0;
    std::size_t failures = 0;
};

class RuntimeFollowerControllerV1 final {
public:
    explicit RuntimeFollowerControllerV1(RuntimeFollowerServicesV1);
    // Call from the session's actor decision provider. A failing follower is disabled with one log line; the frame
    // itself keeps running.
    bool update(CombatSession&, double dt_seconds, std::string& error);
    const RuntimeFollowerReportV1& report() const noexcept { return report_; }
    std::size_t follower_count() const noexcept { return actors_.size(); }
    const FollowerMemoryV1* memory(ActorId) const noexcept;

private:
    struct Runtime { FollowerMemoryV1 memory; FollowerActionV1 last = FollowerActionV1::none; bool failed = false; };
    RuntimeFollowerServicesV1 services_;
    std::map<ActorId, Runtime> actors_;
    RuntimeFollowerReportV1 report_;
};

} // namespace dh::foundation::companions
