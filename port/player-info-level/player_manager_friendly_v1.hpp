#pragma once
#include "player_locality_v1.hpp"
namespace dh2::player_manager_friendly_v1 {
using Registry=player_manager_host_level::PlayerRegistry;
using PlayerInfo=player_manager_host_level::PlayerInfoProjection;
enum class Status {complete,invalid_argument,invalid_registry,missing_provider,provider_failed,missing_projection};
enum class Operation {none,online,game_state_online,matching,room,net_manager,net_initialized,net_ids,internal_player,character,internal_word};
struct Services {
    const player_locality_v1::Services* queries=nullptr;
    void* context=nullptr;
    int (*internal_id_670)(void*,PlayerInfo*,std::int32_t*)=nullptr;
};
struct Result {
    Status status=Status::complete;
    Operation last_operation=Operation::none;
    std::uint32_t calls=0,examined=0;
    std::int32_t value=-1;
    PlayerInfo* player=nullptr;
};
// _InitEquipment runs while the single selected Character may not yet be
// present in PlayerManager's registered list. Source loot weighting still
// receives the current player's class count; this projection represents that
// one offline Character using its CharacterTable row index.
struct SinglePlayerClassCounts {
    std::int32_t mage=0,rogue=0,warrior=0;
};
enum class SinglePlayerVisualClass { warrior,rogue,mage };
struct SinglePlayerClassProjection {
    SinglePlayerVisualClass visual_class=SinglePlayerVisualClass::warrior;
    SinglePlayerClassCounts counts{};
};
// Projects the canonical CharacterTable row into the small UI/loot views that
// need a single-player class. Source Character `ClassID` properties are a
// separate domain and are rejected here.
Status project_single_player_class(std::int32_t character_class_row,
                                   SinglePlayerClassProjection*);
Status project_single_player_class_counts(std::int32_t character_class_row,
                                           SinglePlayerClassCounts*);
// Source manager+6a0 or online vector+6a8/6ac. Friendly ordinal selection
// scans all players, rather than GetLocalPlayer's separate local-only scan.
// All fields/providers borrow the same canonical owner; no new player store.
Status get_num_players(const Registry*,const Services*,Result*);
Status get_internal_id_by_friendly_id(const Registry*,const Services*,std::int32_t ordinal,std::uint32_t require_character,Result*);
Status get_player(const Registry*,const Services*,std::int32_t ordinal,std::uint32_t require_character,Result*);
} // namespace dh2::player_manager_friendly_v1
