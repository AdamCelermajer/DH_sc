#include "character_clear_target_v1.hpp"

namespace dh2::character_clear_target_v1 {

Status clear(character_ai_initialization::State* source,
             character::set_target::OwnerFacts* owner,
             std::uintptr_t expected_character,
             const character::set_target::Services* services) {
    if (!source || !owner || !expected_character || !source->identity ||
        source->owner_04 != expected_character ||
        owner->identity != expected_character || !services || !services->invoke)
        return Status::invalid_argument;

    character::set_target::State projection{
        source->identity, owner, source->requested_target_3c, source->target_40,
        source->last_target_44, source->alive_48, source->sight_49,
        source->sticky_4c, 0};
    int result = character::set_target::source_service_failed;
    try {
        result = character::set_target::dh2_character_ai_set_target(
            &projection, 0, 0, services);
    } catch (...) {
        result = character::set_target::source_service_failed;
    }

    // AI_SetTarget writes its requested target before its debug services. Keep
    // that source prefix even if a later provider fails.
    source->requested_target_3c = projection.requested_target;
    source->target_40 = projection.target;
    source->last_target_44 = projection.last_target;
    source->alive_48 = projection.alive_snapshot;
    source->sight_49 = projection.sight_snapshot;
    source->sticky_4c = projection.sticky;
    return result == character::set_target::complete
        ? Status::complete : Status::source_failed;
}

} // namespace dh2::character_clear_target_v1
