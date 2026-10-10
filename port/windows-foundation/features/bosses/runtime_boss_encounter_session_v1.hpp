#pragma once

#include "runtime_boss_encounter_plan_v1.hpp"
#include "../../combat_session.hpp"

namespace dh2::bosses::runtime_v1 {

using dh::foundation::ActorId;
using dh::foundation::CombatSession;
using dh::foundation::CombatSessionLifetime;
inline constexpr ActorId invalid_session_actor_id = dh::foundation::invalid_actor_id;

struct SessionFrameFactsV1 {
    ActorId source_actor{invalid_session_actor_id};
    bool encounter_admitted{};
    float source_hp_percent{};
    bool source_script_idle{};
    bool can_dive_now{};
    bool source_native_ai_idle{};
    bool source_native_ai_skill{};
};

struct SessionStatusV1 {
    Phase phase{Phase::unknown};
    bool attack_ready{};
    bool timers_started{};
    double attack_timer_elapsed_ms{};
    std::uint64_t timer_callbacks{};
    std::uint64_t last_session_frame{};
};

// Same-CombatSession consumer for the recovered Swamp King phase switch and
// repeating attack-readiness timer. Native admission, script-local SK_STATE /
// canDiveNow, and source HP percentage remain typed inputs from their owners.
// Retained ActorState/pose are always revalidated against this exact
// CombatSession/ActorId pair; source AI state remains a typed owner input.
class SwampKingSessionConsumerV1 {
public:
    bool bind(CombatSession&, ActorId, std::string& error);
    // Called only from the owning CombatSession frame-begin lane. This starts
    // the original 1500 ms timer after admitted+native Idle, then advances it
    // only with that Session's dt and frame serial.
    bool on_frame_begin(CombatSession&, double dt, const SessionFrameFactsV1&,
                        std::string& error);
    SessionStatusV1 status() const noexcept { return status_; }
    ActorId actor_id() const noexcept { return actor_id_; }
private:
    CombatSession* session_{};
    ActorId actor_id_{invalid_session_actor_id};
    std::weak_ptr<const CombatSessionLifetime> session_lifetime_;
    std::weak_ptr<const void> actor_lease_;
    SessionStatusV1 status_{};
    double remainder_ms_{};
    bool validate_session(CombatSession&, std::string&) const;
};

} // namespace dh2::bosses::runtime_v1
