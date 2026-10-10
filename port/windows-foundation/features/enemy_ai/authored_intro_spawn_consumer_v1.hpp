#pragma once

#include "../../actor_definitions.hpp"
#include "../../actor_state.hpp"
#include "../../original_actor_lifecycle.hpp"
#include "../../original_campaign_world_adapter.hpp"
#include "../../source_world_objects.hpp"

#include <functional>
#include <string>
#include <vector>

namespace dh::foundation::enemy_ai {

struct AuthoredIntroSpawnBindingV1 {
    std::string module_occurrence;
    ActorId first = invalid_actor_id;
    ActorId second = invalid_actor_id;
};

// Composes the existing module-scoped SourceWorldObjects lookup with the
// caller-supplied actor lookup and OriginalActorLifecycle records. The actor
// lookup is a trusted contract: this API verifies ID/status agreement but the
// lifecycle does not expose its borrowed ActorState pointer for identity
// comparison. The callback borrows all owners; they must outlive the campaign
// provider. It admits only the two verified 001_swamp LizardMan_Intro rows.
bool bind_authored_intro_spawn_consumer_v1(
    const std::vector<ActorDefinition>& definitions,
    const SourceWorldObjects& source_objects,
    OriginalActorLifecycle& lifecycle,
    std::function<ActorState*(ActorId)> session_actor,
    OriginalCampaignWorldProviders& providers,
    AuthoredIntroSpawnBindingV1& binding,
    std::string& error);

} // namespace dh::foundation::enemy_ai
