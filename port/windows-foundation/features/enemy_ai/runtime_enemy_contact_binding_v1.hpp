#pragma once

#include "../../combat_session.hpp"
#include "runtime_enemy_contact_projection_v1.hpp"

#include <functional>
#include <memory>

namespace dh::foundation::enemy_ai {

class RuntimeEnemyControllerV1;

struct RuntimeEnemyContactProjectionServicesV1 {
    std::function<bool(ActorId, RuntimeEnemyContactControllerProjectionV1&,
                       std::string&)> controller;
    // Required by the controller-owned composition constructor. Selection is
    // explicit source ownership: no class/script-name guess is made here.
    std::function<bool(ActorId, RuntimeEnemyContactAISPolicyV1&,
                       std::string&)> selected_ais_policy;
    RuntimeEnemyContactSneakingServicesV1 sneaking;
    // Exact source Debug::Load/IsTracingPlayersCollision operations.
    std::function<bool(std::string&)> debug_load;
    std::function<bool(const char*, bool&, std::string&)> debug_switch;
    // Legacy typed continuation for the adapter without a live enemy
    // controller. Controller-owned composition runs the recovered native
    // CancelSneaking kernel instead.
    std::function<bool(ActorId, std::string&)> cancel_sneaking;
};

// Projects source collision identities and target effects from the existing
// CombatSession. Pass services() directly to RuntimeSessionContactOwnerV1;
// source contact delivery remains owned by that canonical physical owner.
class RuntimeEnemyContactBindingV1 final {
public:
    RuntimeEnemyContactBindingV1(CombatSession&,
                                 RuntimeEnemyContactProjectionServicesV1);
    RuntimeEnemyContactBindingV1(CombatSession&, RuntimeEnemyControllerV1&,
                                 RuntimeEnemyContactProjectionServicesV1);
    physics::RuntimeSessionContactServicesV1 services() const;

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

} // namespace dh::foundation::enemy_ai
