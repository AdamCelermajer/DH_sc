#include "character_distribute_xp_player_owners_v1.hpp"

#include <cmath>

namespace dh2::character_distribute_xp_player_owners_v1 {

Status resolve(const Binding* binding, std::int32_t ordinal, Result* output,
               std::string& error) {
    error.clear();
    if (!binding || !output || ordinal < 0 || !binding->registry ||
        !binding->friendly || !binding->locality ||
        !binding->character_owner || !binding->object_manager ||
        !binding->property_rules) {
        error = "XP recipient owner binding is incomplete";
        return Status::invalid_argument;
    }

    player_manager_friendly_v1::Result selected{};
    const auto player_status = player_manager_friendly_v1::get_player(
        binding->registry, binding->friendly, ordinal, 1, &selected);
    if (player_status != player_manager_friendly_v1::Status::complete ||
        !selected.player) {
        error = "PlayerManager friendly ordinal did not resolve a registered Character";
        return Status::player_selection_failed;
    }

    if (!binding->locality->character_660) {
        error = "PlayerInfo Character+660 provider is unavailable";
        return Status::missing_character;
    }
    std::uintptr_t identity = 0;
    if (binding->locality->character_660(binding->locality->context,
                                         selected.player, &identity) != 0 ||
        !identity) {
        error = "Selected PlayerInfo has no live Character+660";
        return Status::missing_character;
    }

    auto* properties = binding->character_owner->properties_for(identity);
    auto* save = binding->character_owner->save_for(identity);
    if (!properties || !save || save->character() != identity) {
        error = "Character identity does not resolve its canonical Save and properties";
        return Status::character_owner_mismatch;
    }
    auto view = data::property_view(*binding->property_rules, *properties);
    if (dh2_property_validate(&view) != 0) {
        error = "Selected Character property view is invalid";
        return Status::invalid_properties;
    }

    // Character::DistributeXP uses GameObject world XY. Resolve through the
    // same identity in the live ObjectManager projection, and require its
    // borrowed coordinates: the value fallback may be an old snapshot.
    const auto* game_object = binding->object_manager->find_by_identity(identity);
    if (!game_object || game_object->identity != identity ||
        !game_object->live_fields.world_x || !game_object->live_fields.world_y) {
        error = "Selected Character has no live identity-matched GameObject position";
        return Status::game_object_missing;
    }
    const float world_x = *game_object->live_fields.world_x;
    const float world_y = *game_object->live_fields.world_y;
    if (!std::isfinite(world_x) || !std::isfinite(world_y)) {
        error = "Selected Character GameObject position is non-finite";
        return Status::invalid_position;
    }

    player_locality_v1::Result locality{};
    if (player_locality_v1::is_local_player(binding->registry,
            binding->locality, identity, &locality) !=
            player_locality_v1::Status::complete) {
        error = "Source IsLocalPlayer failed for the selected Character";
        return Status::local_query_failed;
    }

    Result result{};
    result.player = selected.player;
    result.character_identity = identity;
    result.properties = view;
    result.savegame = save;
    result.is_local = locality.value != 0;
    result.world_x = world_x;
    result.world_y = world_y;
    *output = result;
    return Status::complete;
}

} // namespace dh2::character_distribute_xp_player_owners_v1
