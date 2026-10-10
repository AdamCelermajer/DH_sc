#pragma once

#include "container_receiver_bindings.hpp"
#include "container_live_loot_binding.hpp"

namespace dh::foundation::interactions {

// Prepare the two authored OpenableContainer endpoints as one preconstruction
// operation. The caller still owns construction/publication/InitPost. Work on
// a copy so a missing loot provider cannot leave half of the source callbacks
// installed, and refuse an existing V21 or DropLoot binding to avoid duplicate
// event/reward paths.
inline bool prepare_openable_source_pipeline_v21(
    dh2::world::OpenableContainerServicesV1& services,
    dh2::world::ContainerAnimatorProviderV21 current_visual_animator,
    std::shared_ptr<dh2::character::WorldLootGameplayV23> gameplay,
    ContainerLiveDropTableV104 drop_table,
    std::shared_ptr<void> world_lease,
    ContainerLootSourceResolverV104 resolve_source,
    OpenableAnimationBindingV21& out,
    std::string& error) {
    error.clear();
    if (services.bind_timeline_callbacks || services.play_animation || services.scene_flags) {
        error = "Interactions Openable V21 animation endpoints already bound";
        return false;
    }
    auto next = services;
    OpenableAnimationBindingV21 animation;
    if (!prepare_openable_animation_v21(next, std::move(current_visual_animator),
                                         animation, error)) return false;
    if (!bind_container_live_loot_v104(next, std::move(gameplay), drop_table,
                                        std::move(world_lease), std::move(resolve_source),
                                        error)) return false;
    services = std::move(next);
    out = std::move(animation);
    return true;
}

// DestructibleContainer keeps its distinct native service bundle and event
// receiver; only its common presentation DropLoot endpoint composes with the
// same initialized world loot owner.
inline bool prepare_destructible_source_pipeline_v21(
    dh2::world::DestructibleContainerServicesV16& services,
    dh2::world::ContainerAnimatorProviderV21 current_visual_animator,
    std::shared_ptr<dh2::character::WorldLootGameplayV23> gameplay,
    ContainerLiveDropTableV104 drop_table,
    std::shared_ptr<void> world_lease,
    ContainerLootSourceResolverV104 resolve_source,
    DestructibleAnimationBindingV21& out,
    std::string& error) {
    error.clear();
    if (services.bind_callbacks || services.animation_count || services.play_index ||
        services.common.play_animation || services.common.scene_flags) {
        error = "Interactions Destructible V21 animation endpoints already bound";
        return false;
    }
    auto next = services;
    DestructibleAnimationBindingV21 animation;
    if (!prepare_destructible_animation_v21(next, std::move(current_visual_animator),
                                             animation, error)) return false;
    if (!bind_container_live_loot_v104(next.common, std::move(gameplay), drop_table,
                                        std::move(world_lease), std::move(resolve_source),
                                        error)) return false;
    services = std::move(next);
    out = std::move(animation);
    return true;
}

} // namespace dh::foundation::interactions
