#pragma once

#include <string>

namespace dh::foundation::persistence::death_restart {

enum class LocalDeathRouteV1 {
    none,
    show_offline_death_fade,
    start_online_death_timer
};

enum class ReviveRouteV1 {
    offline_checkpoint_revive,
    online_handoff
};

struct ReviveTicketV1 {
    ReviveRouteV1 route = ReviveRouteV1::offline_checkpoint_revive;
    bool valid = false;
    bool consumed = false;
};

// Mirrors PlayerManager::_CheckLocalDeaths and _HandleLocalDeaths. The caller
// supplies the result of the source character-death predicate.
LocalDeathRouteV1 local_death_route_v1(bool gameplay_level_type_38,
                                       bool local_character_assigned,
                                       bool character_dead,
                                       bool online);

// Mirrors the gates in NativeReviveAllPlayers. The offline route is available
// only for the authored revive action on an active gameplay level.
bool prepare_revive_v1(bool gameplay_level_type_38, bool online,
                       ReviveTicketV1&, std::string& error);

struct ReviveServicesV1 {
    bool (*pop_fade_to_black)(void*, std::string&) = nullptr;
    bool (*push_fade_from_black)(void*, std::string&) = nullptr;
    // Must perform Level::LoadCheckpoint, including its player QEST-only
    // checkpoint restore and Character::SG_Update(1) prefix.
    bool (*load_level_checkpoint)(void*, std::string&) = nullptr;
    bool (*clear_character_buffs)(void*, std::string&) = nullptr;
    bool (*restore_revive_position)(void*, std::string&) = nullptr;
    // Must mirror Character::Revive(..., 1), including _InitHpMp and body init.
    bool (*revive_to_source_vitals)(void*, std::string&) = nullptr;
    bool (*set_character_idle)(void*, std::string&) = nullptr;
    // Caller resolves CharacterDesign.MinimumPotionsOnDeath from source data.
    bool (*ensure_minimum_potions)(void*, std::string&) = nullptr;
    bool (*restart_level_music)(void*, std::string&) = nullptr;
    bool (*notify_online_handoff)(void*, std::string&) = nullptr;
    void* context = nullptr;
};

// Runs one authored NativeReviveAllPlayers occurrence. It consumes the ticket
// before side effects so a partial failure cannot replay checkpoint mutations.
bool execute_revive_v1(ReviveTicketV1&, const ReviveServicesV1&,
                       std::string& error);

} // namespace dh::foundation::persistence::death_restart
