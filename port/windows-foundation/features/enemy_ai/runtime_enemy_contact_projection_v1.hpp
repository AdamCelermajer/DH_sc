#pragma once

#include "../../combat_session.hpp"
#include "../physics/runtime_session_contact_v1.hpp"
#include "../../../game-data/skill_tables.hpp"

#include <functional>
#include <string>
#include <vector>

namespace dh::foundation::enemy_ai {

enum class RuntimeEnemyContactAISPolicyV1 : std::uint8_t {
    unknown = 0,
    absent = 1,
    inherited_default = 2
};

// One live projection of the selected same-Session controller. Controller
// identity points into RuntimeEnemyControllerV1's canonical ActorRuntime map.
struct RuntimeEnemyContactControllerProjectionV1 {
    const void* controller_projection{};
    bool preferred_target_known = false;
    ActorId preferred_target = invalid_actor_id;
    std::uint32_t application_frame{}; // modern same-Session update serial
    std::uint32_t dt_ms{};             // exact caller-provided source millisecond step
    std::uint32_t paused{};            // source collision pause byte, 0 or 1
    std::uint8_t interactive415{};      // same-controller CancelSneaking byte
    RuntimeEnemyContactAISPolicyV1 ais_policy = RuntimeEnemyContactAISPolicyV1::unknown;
};

// Read-only projection and delivered script actions for the source's positive
// Special_Sneak branch. No callbacks means the native kernel fails only if an
// eligible nonnull skill script is actually reached.
struct RuntimeEnemyContactSneakingServicesV1 {
    dh2::data::SkillTables::Borrow skill_tables;
    // Delivers the real per-instance removal from the same actor's buff owner.
    // The controller's registry supplies key/order/identity; absent provider
    // is an error only when a matching active record is reached.
    std::function<bool(ActorId, std::uint32_t, std::uintptr_t,
                       std::string&)> delete_buff_record;
    std::function<bool(ActorId, std::vector<std::uintptr_t>&,
                       std::string&)> skill_scripts;
    std::function<bool(ActorId, std::uintptr_t, std::uint32_t, bool&,
                       std::string&)> skill_active;
    std::function<bool(ActorId, std::uintptr_t, std::uint32_t,
                       std::string&)> skill_pre;
};

} // namespace dh::foundation::enemy_ai
