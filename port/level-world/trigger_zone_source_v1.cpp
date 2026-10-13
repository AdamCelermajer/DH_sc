#include "trigger_zone_source_v1.hpp"

namespace dh2::trigger_zone_source_v1 {

Status update(object_manager_runtime_owner_v1::GameObject& row,
              const Definition& definition, State& state,
              const Services& services, std::string& error) {
    error.clear();
    if (!row.identity || !row.save_facts_ready) {
        error = "TriggerZone::Update row has no completed InitPost";
        return Status::invalid_state;
    }
    if (!services.local_player_scripted || !services.local_player_touching ||
        !services.player_counts || !services.delta_ms || !services.start_script ||
        !services.door_state) {
        error = "TriggerZone::Update providers are incomplete";
        return Status::provider_unavailable;
    }
    // Character+0x1480: a scripted local Player (cutscene) idles every zone.
    bool scripted = false;
    if (!services.local_player_scripted(services.context, scripted, error))
        return Status::provider_failed;
    if (scripted) return Status::skipped;
    // TriggerZone::IsDoorClosedActivated (0x39b27c).
    if (state.door_identity) {
        std::int32_t door_state = 0;
        if (!services.door_state(services.context, state.door_identity, door_state, error))
            return Status::provider_failed;
        if (door_state == 1 || door_state == 3) return Status::skipped;
    }
    // Trigger::CanActivate (0x3987ac): an exhausted count or a running delay
    // blocks; otherwise the ObjectBase +0x8a enabled byte decides.
    if ((row.trigger_count_3a8 >= 0 && row.trigger_count_3a8 <= row.trigger_max_count_3b4) ||
        row.trigger_pending_3b8 > 0 || !row.serialized_flag_8a)
        return Status::skipped;
    // GameObject::MeetCondition (0x38ab60) is constant true. The online
    // virtual +84 gate is skipped offline. Trigger::Update runs ZoneEx::Update
    // (contact list), UpdateTimer, RequireOnlineUpdate and UpdateIdleSound;
    // only the timer has an offline effect on this class.
    std::int32_t dt = 0;
    if (!services.delta_ms(services.context, dt, error)) return Status::provider_failed;
    if (row.trigger_pending_3b8 > 0) row.trigger_pending_3b8 -= dt;
    std::int32_t players = 0, touching = 0;
    if (!services.player_counts(services.context, row.identity, players, touching, error))
        return Status::provider_failed;
    state.activating_players_960 = touching;

    const bool all_players_script = state.script_all_player_id != -1;
    const std::int32_t start_id = all_players_script ? state.script_all_player_id
                                                     : state.script_id;
    const std::int32_t out_id = state.script_all_player_move_out_id == -1
        ? state.script_move_out_id : state.script_all_player_move_out_id;
    const bool entered = all_players_script ? touching == players : touching > 0;
    bool left = touching == 0;
    bool has_character = false, character_touching = false;
    if (!services.local_player_touching(services.context, row.identity, has_character,
                                        character_touching, error))
        return Status::provider_failed;

    const auto safe_start = [&](std::int32_t id) -> bool {
        // TriggerZone::SafeStartScriptOnlyOnce (0x39b2a0).
        if (id == -1) return true;
        return services.start_script(services.context, id, row.module_id_64, error);
    };
    const auto activate = [&]() {
        // Trigger::Activate (0x398794).
        row.trigger_max_count_3b4 += 1;
        row.trigger_pending_3b8 = definition.trigger_delay_ms;
    };

    bool fire = false;
    if (!entered) {
        if (state.touch_required_956) left = has_character && !character_touching;
    } else if (state.touch_required_956 && !has_character) {
        left = false;
    } else if (state.touch_required_956 && !character_touching) {
        left = true;
    } else {
        fire = true;
    }
    if (fire && !state.inside_1972) {
        state.inside_1972 = 1;
        if (!safe_start(start_id)) return Status::provider_failed;
        row.trigger_pending_3b8 = definition.trigger_delay_ms;
        // HideMarker: the effect_one_player marker only exists in multiplayer.
        if (out_id == -1) activate();
    }
    if (state.inside_1972 && left) {
        state.inside_1972 = 0;
        if (out_id != -1) {
            if (!safe_start(out_id)) return Status::provider_failed;
            activate();
        }
    }
    // Multiplayer-only "all players" presence handling (players > 1).
    if (players > 1 && !state.touch_required_956 && state.script_all_player_id != -1 &&
        state.script_id != -1) {
        const bool present = touching > 0;
        if (state.all_players_1973) {
            if (!present) {
                state.all_players_1973 = 0;
                if (!safe_start(state.script_move_out_id)) return Status::provider_failed;
            }
        } else if (present) {
            if (!safe_start(state.script_id)) return Status::provider_failed;
            state.all_players_1973 = 1;
        }
    }
    return Status::complete;
}

} // namespace dh2::trigger_zone_source_v1
