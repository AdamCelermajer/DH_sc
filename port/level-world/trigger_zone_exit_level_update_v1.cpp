#include "trigger_zone_exit_level_update_v1.hpp"

namespace dh2::trigger_zone_exit_level_update_v1 {
namespace {
Status unavailable(const char* name, std::string& error) {
    error = std::string("TriggerZoneExitLevel::Update provider unavailable: ") + name;
    return Status::provider_unavailable;
}
Status invalid(const char* message, std::string& error) {
    error = message;
    return Status::invalid_state;
}
}

Status set_enabled(object_manager_runtime_owner_v1::Owner& manager,
                   std::uintptr_t identity, bool enabled,
                   std::string& error) {
    error.clear();
    auto* object = manager.find_by_identity(identity);
    if (!identity || !object || object->identity != identity ||
        !object->exit_level.definition_ready ||
        !object->source_provenance_ready ||
        !object->visible_80_ready ||
        !object->serialized_flag_8a_ready ||
        object->serialized_flag_8a != static_cast<std::uint8_t>(enabled))
        return invalid("exit-zone SetEnable lost its canonical post-transition row", error);

    // ObjectBase::Enabled invokes SetVisible then TriggerZone::setUpdating;
    // Disabled invokes setUpdating then SetVisible. The shared render/update
    // owners consume these fields directly; no second condition pass occurs.
    if (enabled) {
        object->visible_80 = object->serialized_flag_8a;
        object->updating_85 = 1;
        object->pf_object_flags_1cc |= 8u;
        object->game_object_nav_dead_end_373 = 0;
    } else {
        object->updating_85 = 0;
        object->exit_level.trigger_zone_update_state_7b4 = 0;
        object->visible_80 = 0;
        object->pf_object_flags_1cc &= ~8u;
        object->game_object_nav_dead_end_373 = 1;
    }
    if (object->live_fields.visible_80)
        *object->live_fields.visible_80 = object->visible_80;
    if (object->live_fields.updating_85)
        *object->live_fields.updating_85 = object->updating_85;
    return Status::complete;
}

Status update(source_level_owner_v1::Owner& level_owner,
              const data::LevelTables& levels, const Request& request,
              const Services& services, std::string& error) {
    error.clear();
    const auto level = level_owner.snapshot();
    if (!request.level_identity || !request.level_generation ||
        !request.trigger_identity ||
        level.phase != source_level_owner_v1::Phase::active ||
        level.source_level != request.level_identity ||
        level.generation != request.level_generation ||
        !level.object_manager_identity)
        return invalid("exit-zone update crossed its retained source Level generation", error);

    auto* trigger = level_owner.object_manager().find_by_identity(
        request.trigger_identity);
    if (!trigger || trigger->source_handle != request.trigger_source_handle ||
        !trigger->source_provenance_ready)
        return invalid("exit-zone identity is not the requested canonical ObjectManager row", error);

    auto& zone = trigger->exit_level;
    if (!zone.definition_ready || zone.fasttravel_unlocked_2072 > 1 ||
        zone.fasttravel_menu_open_2073 > 1 || zone.active_players_960 < 0 ||
        zone.level_list_row_2004 < -1 ||
        (zone.level_list_row_2004 >= 0 &&
         (static_cast<std::size_t>(zone.level_list_row_2004) >= levels.levels.size() ||
          levels.levels[static_cast<std::size_t>(zone.level_list_row_2004)].name !=
              zone.level_name_2012)) ||
        zone.entrypoint_id_2036 < -1)
        return invalid("exit-zone source definition or Update bytes are invalid", error);

    // TriggerZoneExitLevel::Update checks GetCurrentLevel and Level+0x144 at
    // 0x39c388-0x39c394. This active Level skips all player/menu work if the
    // source loaded byte has not yet been published.
    if (level.transition_flag_144 == 0)
        return Status::skipped_level_transition_gate;
    if (level.transition_flag_144 != 1)
        return invalid("source Level+0x144 is outside its observed byte range", error);

    if (request.local_character && zone.active_players_960 > 0 &&
        !zone.fasttravel_unlocked_2072) {
        if (!services.set_fasttravel_unlocked)
            return unavailable("Character::SG_SetFastTravelIdUnlocked", error);
        if (!services.set_fasttravel_unlocked(services.context,
                request.local_character, zone.fasttravel_id_2048,
                true, -1, error))
            return Status::provider_failed;
        zone.fasttravel_unlocked_2072 = 1;
    }

    bool touching = false;
    bool hosting = false;
    bool meets_condition = false;
    if (request.local_character) {
        if (!services.is_player_touching)
            return unavailable("GameObject::IsTouching", error);
        if (!services.is_player_touching(services.context, request.trigger_identity,
                                         request.local_character, touching, error))
            return Status::provider_failed;
    }
    if (touching) {
        if (!services.is_local_player_hosting)
            return unavailable("PlayerManager::IsLocalPlayerHosting", error);
        if (!services.is_local_player_hosting(services.context, hosting, error))
            return Status::provider_failed;
    }
    if (touching && hosting) {
        if (!services.meet_condition)
            return unavailable("GameObject::MeetCondition", error);
        if (!services.meet_condition(services.context, request.trigger_identity,
                                     meets_condition, error))
            return Status::provider_failed;
    }

    if (touching && hosting && meets_condition) {
        // Source order calls RequireOnlineUpdate before checking +2004/+2073.
        if (!services.require_online_update)
            return unavailable("GameObject::RequireOnlineUpdate", error);
        if (!services.require_online_update(services.context,
                request.trigger_identity, error))
            return Status::provider_failed;
        if (zone.level_list_row_2004 >= 0 &&
            !zone.fasttravel_menu_open_2073) {
            if (!services.resolve_localized_level_name)
                return unavailable("StringManager::getString(LevelList +36)", error);
            std::string localized_name;
            if (!services.resolve_localized_level_name(services.context,
                    zone.level_list_row_2004, localized_name, error))
                return Status::provider_failed;
            if (!services.display_fast_travel)
                return unavailable("RenderFX::InvokeASCallback(DisplayFastTravel)", error);
            if (!services.display_fast_travel(services.context, true,
                    localized_name, zone.level_name_2012,
                    zone.entrypoint_id_2036, error))
                return Status::provider_failed;
            zone.fasttravel_menu_open_2073 = 1;
        }
        return Status::complete;
    }

    if (zone.fasttravel_menu_open_2073) {
        if (zone.level_list_row_2004 < 0)
            return invalid("open fast-travel UI has no resolved LevelList row for its close callback", error);
        if (!services.resolve_localized_level_name)
            return unavailable("StringManager::getString(LevelList +36)", error);
        std::string localized_name;
        if (!services.resolve_localized_level_name(services.context,
                zone.level_list_row_2004, localized_name, error))
            return Status::provider_failed;
        if (!services.display_fast_travel)
            return unavailable("RenderFX::InvokeASCallback(DisplayFastTravel)", error);
        if (!services.display_fast_travel(services.context, false,
                localized_name, zone.level_name_2012,
                zone.entrypoint_id_2036, error))
            return Status::provider_failed;
        zone.fasttravel_menu_open_2073 = 0;
    }
    return Status::complete;
}

} // namespace dh2::trigger_zone_exit_level_update_v1
