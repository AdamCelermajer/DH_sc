#pragma once

#include "../game-data/player_savegame_v1.hpp"
#include "../game-data/properties.hpp"
#include "level_construction_fields.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_distribute_xp_v1 {

// Borrowed source Character identity and canonical data owners. A killer needs
// only identity/position; killed and player Characters also need properties,
// and a player additionally needs its current Save for the SCT difficulty gate.
struct CharacterView {
    std::uintptr_t identity{};
    data::PropertyView* properties{};
    data::PlayerSavegameV1* savegame{};
    float x{};
    float y{};
};

struct State {
    CharacterView* killer{}; // null follows the source victim-centered fallback
    CharacterView* killed{};
};

enum class Operation : std::uint32_t {
    design_setting,
    player_count,
    player_by_friendly_ordinal,
    give_xp,
    is_local_player,
    current_level_difficulty,
    scrolling_xp_text,
};

struct Request {
    Operation operation{};
    CharacterView* character{};
    std::uint32_t index{};
    // Byte offset from Arrays::DesignSettingsTable::members.
    std::uint32_t design_offset{};
    std::uint32_t require_character{};
    // Fixed-point XP for give_xp; whole displayed XP for scrolling_xp_text.
    std::int32_t amount{};
};

struct Reply {
    // player_by_friendly_ordinal returns a fresh borrowed Character projection;
    // identity==0 represents PlayerInfo+1632 == null.
    CharacterView character{};
    std::int32_t value{};
    std::uint32_t word{};
    float real{};
};

struct Services {
    void* context{};
    // Reads the actual loaded DesignSettings row word at the requested byte
    // offset. Required offsets are 160, 164, 156, 140, 144, 152, and 148.
    std::int32_t (*design_setting)(void*, std::uint32_t offset,
                                   float* value, std::string& error){};
    // PlayerManager::GetNumPlayers and GetPlayer(index, true). The player
    // provider must query the same canonical PlayerRegistry/PlayerInfo owners.
    std::int32_t (*player_count)(void*, std::int32_t* count,
                                 std::string& error){};
    std::int32_t (*player_by_friendly_ordinal)(void*, std::uint32_t index,
                                               std::uint32_t require_character,
                                               CharacterView* character,
                                               std::string& error){};
    // Adapter for Character::_GiveXP on the supplied Character/PropertyView/
    // Save owners. Reuse character_give_xp_v1::Runtime; pass update-stat=true.
    // `source_return` is the original bool return, distinct from delivery status.
    std::int32_t (*give_xp)(void*, CharacterView*, std::int32_t amount_fixed,
                            std::uint32_t update_player_stat,
                            std::uint32_t* source_return,
                            std::string& error){};
    std::int32_t (*is_local_player)(void*, std::uintptr_t character,
                                    std::uint32_t* is_local,
                                    std::string& error){};
    std::int32_t (*current_level_difficulty)(void*, std::int32_t* difficulty,
                                             std::string& error){};
    // Owns the remaining F_ApplyScrollingCombatTextXP leaf: style/color lookup,
    // GAMEPLAYMENUS_REWARD_XP parsing, and FlashAnimManager delivery.
    std::int32_t (*scrolling_xp_text)(void*, std::uintptr_t killed_character,
                                      std::int32_t displayed_xp,
                                      std::string& error){};
};

// Adapter over the active native Application::GetCurrentLevel projection.
// `fields` is the same Level+0x118 owner used by level callbacks; readiness
// is borrowed from its lifecycle owner so teardown/candidate levels fail closed.
struct CurrentLevelDifficultyOwner {
    const level_construction_fields::State* fields{};
    const bool* ready{};
};
std::int32_t current_level_difficulty_from_owner(
    void*,std::int32_t* difficulty,std::string& error);

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    invalid_source_fact,
    service_unavailable,
    service_failed,
};

struct Result {
    Status status{Status::complete};
    Operation last_operation{Operation::design_setting};
    std::uint32_t service_calls{};
    std::uint32_t player_slots{};
    std::uint32_t player_characters{};
    std::uint32_t range_eligible{};
    std::uint32_t give_xp_calls{};
    std::uint32_t xp_awarded{};
    std::uint32_t local_player_checks{};
    std::uint32_t scrolling_text_calls{};
    std::int32_t victim_base_xp{};
    float xp_share_percent{};
    std::int32_t last_xp_amount_fixed{};
    std::int32_t last_displayed_xp{};
};

// Source `Character::DistributeXP(killed, ...)` body at ELF 0x3bf828 plus
// `GetLevelScaledXP` at 0x3bd918. DesignSettings, roster, `_GiveXP`, local
// PlayerManager membership, current Level, and scrolling-combat-text leaves
// remain explicit borrowed providers. The function does not create or copy a
// PlayerManager, Save, PropertyView, XP runtime, or UI owner. Reached XP awards
// remain applied if a later provider is missing/fails, matching source prefixes.
Status distribute(State*, const Services*, Result*, std::string& error);

} // namespace dh2::character_distribute_xp_v1
