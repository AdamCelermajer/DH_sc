#pragma once

#include "../../combat_session.hpp"
#include "monster_decisions.hpp"
#include "runtime_enemy_contact_projection_v1.hpp"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::enemy_ai {

struct RuntimeEnemyControllerServicesV1 {
    // Real current-level/character visibility and interaction query. Search,
    // radius, faction, sneak and same-world candidate selection are built in.
    std::function<bool(CombatSession&, const ActorState&, const ActorState&,
                       const OriginalCombatProperties&,
                       const OriginalCombatProperties&,
                       const dh2::data::AiProps&, bool&, std::string&)> can_see;
    // Existing shared navigation/controller consumer. Used after acquisition
    // and when an existing target remains outside original melee reach.
    std::function<bool(CombatSession&, ActorState&, ActorState&,
                       const dh2::data::AiProps&, std::string&)> approach;
    // Optional shared retained-pose/animation stop consumer. Target and route
    // cleanup remain owned here and do not depend on animation readiness.
    std::function<bool(CombatSession&, ActorState&, std::string&)> stop;
    // Optional source special skill/buff behavior. `handled` means it consumed
    // this decision. Missing service leaves ordinary melee available.
    std::function<bool(ActorState&, ActorState&,
                       const OriginalCombatProperties&,
                       const dh2::data::AiProps&, bool& handled,
                       std::string&)> special_action;
};

struct RuntimeEnemyControllerReportV1 {
    std::size_t supported_actors = 0;
    std::size_t search_calls = 0;
    std::size_t candidates_rejected_by_faction_or_sneak = 0;
    std::size_t acquired_targets = 0;
    std::size_t lost_targets = 0;
    std::size_t approaches = 0;
    std::size_t attack_requests = 0;
    std::size_t unsupported_special_actions = 0;
    std::size_t per_actor_ticks = 0;
    std::size_t target_deaths = 0;
    std::size_t target_sight_losses = 0;
    std::size_t target_range_losses = 0;
};

// Stateless glue over the session's one live world and original tables. The
// caller installs the returned function with CombatSession's existing actor
// decision provider; CombatSession remains the sole frame/animation owner.
class RuntimeEnemyControllerV1 final {
public:
    explicit RuntimeEnemyControllerV1(RuntimeEnemyControllerServicesV1);
    bool update(CombatSession&, double dt, std::string& error);
    // Bind actual millisecond/frame values once per current Session update.
    // Session serial is the modern projection identity; dt_ms must be passed
    // from the same real frame clock, never reconstructed from render time.
    bool begin_contact_frame(CombatSession&, std::uint32_t dt_ms,
                             std::string& error);
    bool project_contact(CombatSession&, ActorId,
                         RuntimeEnemyContactAISPolicyV1,
                         RuntimeEnemyContactControllerProjectionV1&,
                         std::string& error);
    bool set_contact_target(CombatSession&, ActorId owner, ActorId target,
                            std::uint32_t flags, std::string& error);
    bool process_contact_pause(CombatSession&,
                               physics::RuntimeSessionContactOwnerV1&,
                               std::string& error);
    // Same-controller implementation of Character::CancelSneaking. The
    // source buff registry starts empty with this ActorRuntime and stores only
    // records explicitly published by the live per-actor buff owner.
    bool track_contact_buff(CombatSession&, ActorId, std::uint32_t buff_key,
                            std::uintptr_t record, std::string& error);
    bool cancel_contact_sneaking(CombatSession&, ActorId,
        const RuntimeEnemyContactSneakingServicesV1&, std::string& error);
    const RuntimeEnemyControllerReportV1& report() const noexcept { return report_; }
    void clear_report() noexcept { report_ = {}; }
private:
    enum class Route : std::uint8_t { searching, approach, attack };
    struct ActorRuntime {
        Route route = Route::searching;
        ActorId observed_target = invalid_actor_id;
        double state_elapsed = 0.0;
        bool aggroed = false;
        ActorId preferred_target = invalid_actor_id;
        bool preferred_target_known = true;
        std::uint8_t collision_paused{};
        std::uint32_t pause_timer_remaining_ms{};
        std::uint64_t pause_timer_last_frame{};
        std::uint8_t interactive415{};
        std::map<std::uint32_t, std::vector<std::uintptr_t>> source_buffs;
    };
    bool ensure_session(CombatSession&, std::string& error);
    RuntimeEnemyControllerServicesV1 services_;
    std::map<ActorId, ActorRuntime> actors_;
    std::weak_ptr<const void> binding_lease_;
    bool lease_initialized_ = false;
    std::uint64_t contact_frame_{};
    std::uint32_t contact_dt_ms_{};
    bool contact_clock_valid_ = false;
    RuntimeEnemyControllerReportV1 report_;
};

CombatSession::ActorDecisionProvider make_runtime_enemy_decision_provider_v1(
    RuntimeEnemyControllerServicesV1);

} // namespace dh::foundation::enemy_ai
