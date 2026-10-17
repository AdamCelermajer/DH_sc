#pragma once
#include "../game-data/player_savegame_v1.hpp"
#include "../game-data/properties.hpp"
#include "../player-info-level/player_locality_v1.hpp"
#include <cstdint>
#include <string>
extern "C" {
#include "../pydata-constants/constants.h"
}

namespace dh2::character_give_xp_v1 {
// Character::_GiveXP body after a caller has already selected this Character
// and computed its fixed-point amount. Killer/player eligibility belongs to
// DistributeXP and is not an _GiveXP argument or gate. XP property 33 and the
// XP-bonus property 200 use the canonical PropertyView owned with this Save.
struct Bindings {
    std::uintptr_t character=0;
    std::int32_t amount_fixed=0;
    data::PropertyView* properties=nullptr;
    data::PlayerSavegameV1* save=nullptr;
    const dh2_pycst_view* constants=nullptr;
    bool update_player_stat=true; // Original _GiveXP a3; both current XP callers pass 1.
};

enum class Operation : std::uint32_t {
    character_virtual_40,character_virtual_84,current_level_suppression,
    one_kill_level_up,current_level_difficulty,trace_character_stats,
    level_up,player_by_character
};
struct Request {
    Operation operation{};
    std::uintptr_t character=0;
    data::PropertyView* properties=nullptr;
    data::PlayerSavegameV1* save=nullptr;
    std::int32_t argument=0;
    const char* name=nullptr;
};
struct Reply {std::int32_t value=0;std::uint32_t word=0;};
struct PlayerByCharacterBinding {
    const player_locality_v1::Registry* registry=nullptr;
    const player_locality_v1::Services* player_services=nullptr;
};
struct PlayerByCharacterResult {
    player_locality_v1::Route route=player_locality_v1::Route::none;
    player_locality_v1::PlayerInfo* player=nullptr;
    std::int32_t internal_id=-1;
};
enum class PlayerByCharacterStatus : std::uint32_t {
    complete,invalid_argument,lookup_failed,missing_player
};
// Exact `_GiveXP` tail adapter: source invokes
// GetPlayerByCharacter(character, false), then reads selected PlayerInfo+0x670.
// The existing PlayerManager/locality owner supplies the exact offline/network
// selection and manager+8 fallback; this adapter creates no player registry.
PlayerByCharacterStatus player_by_character_internal_id(
    const PlayerByCharacterBinding*,std::uintptr_t character,
    PlayerByCharacterResult*);
struct Backend {
    void* context=nullptr;
    // Services query the live Character virtuals, current Level, DebugSwitches,
    // LevelUp owner and PlayerManager. The LevelUp delivery must perform the
    // complete source side effects; a partial property-only substitute is not
    // accepted. Zero means delivered, nonzero/throw is an explicit port error.
    std::int32_t (*invoke)(void*,const Request*,Reply*,std::string& error)=nullptr;
    // A full source LevelUp owner is separately required only if this award
    // crosses the threshold. Generic Character services cannot fake it.
    std::int32_t (*level_up)(void*,const Request*,Reply*,std::string& error)=nullptr;
    // Optional source adapters. The pinned _GiveXP reads its base max-level
    // constant before querying SG_GetGameDifficultyUnlocked, then may query
    // difficulty again before selecting the hard-mode override. Production
    // defaults use the canonical view/save; adapters let the audit observe the
    // exact source order without replacing either owner.
    std::int32_t (*get_constant)(void*,const dh2_pycst_view*,const char*,const char*,
                                 dh2_pycst_result*,std::string& error)=nullptr;
    std::int32_t (*get_unlocked_difficulty)(void*,std::uintptr_t,
                                            const data::PlayerSavegameV1*,
                                            std::int32_t*,std::string& error)=nullptr;
    // When present, source GetPlayerByCharacter and +0x670 reads use the same
    // canonical PlayerManager registry/services as locality and XP selection.
    const PlayerByCharacterBinding* player_lookup=nullptr;
};
enum class Status : std::uint32_t {
    complete,invalid_argument,busy,property_failed,
    service_unavailable,service_failed,level_up_required,projection_changed
};
struct Result {
    Status status=Status::complete;
    Operation last_operation=Operation::character_virtual_40;
    std::uint32_t calls=0,source_return=0,xp_added=0,level_up_called=0,
                  xp_clamped=0,stat_player_lookups=0;
    std::int32_t level=0,max_level=0,raw_amount_fixed=0,
                 modified_amount_fixed=0,xp_after=0,max_xp=0,
                 level_up_overage=0,player_internal_id=-1;
};

// One call is one reached _GiveXP episode. XP is stored before the original
// LevelUp call; if the full LevelUp service is absent/fails, that XP prefix is
// retained and the status reports the boundary. No rollback is source-faithful.
class Runtime {
    Bindings bindings_;
    Backend backend_;
    data::PropertyView captured_properties_{};
    std::uintptr_t captured_save_character_=0;
    bool busy_=false;
    Status invoke(Operation,const char*,std::int32_t,Reply&,Result&,std::string&);
    bool coherent()const noexcept;
public:
    Runtime(Bindings,Backend);
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    Status give_xp(Result*,std::string& error);
};
// Source boundary details: 0x3bf498 Character::_GiveXP(Character*,int,bool),
// 0x3de7ec PROPS_GetModifiedXP (the resolved Special_Bonus_To_XP property 200),
// 0x3beb88 Character::LevelUp, and 0x3bf6d8 PlayerManager::GetPlayerByCharacter.
// A caller supplies the already-calculated fixed-point share from
// DistributeXP. Recipient selection, killer identity and multiplayer/radius
// policy remain upstream; this function has no killer or player-count input.
}
