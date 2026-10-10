#pragma once
#include "encounter_services.hpp"
#include "../../../level-world/application_spawn_random_owner_v4.hpp"
#include "../../../level-world/game_object_initialization_owner_v1.hpp"
namespace dh::foundation::encounters {
// Install before the actual receiver runs InitPost. This only supplies reached
// source initialization operations: it never runs a new initialization phase,
// changes module IDs, synthesizes lifecycle state, or admits an actor by itself.
struct ActivationReceiver {
 std::shared_ptr<void> scope;
 // Weakly resolve actual record->receiver AFTER its constructor. Failure is
 // explicit; never retain the receiver through a callback it itself owns.
 std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1*&,std::string&)> same_base;
};
bool bind_source_activation(ActivationReceiver,
 std::shared_ptr<Admission> same_admission,
 dh2::world::CanonicalSpawnApplicationServicesV4 actual_application,
 dh2::world::GameObjectInitializationServicesV1&,std::string&);
}
