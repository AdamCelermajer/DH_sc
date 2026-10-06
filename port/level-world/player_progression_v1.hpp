#pragma once
#include "../game-data/properties.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include "../game-data/design_settings.hpp"
#include <functional>
namespace dh2::character {
// Borrowed source facts; x/y are Character+160/+164, not a renderer distance.
struct ProgressionActorV1 {
 std::uintptr_t identity{};
 data::PropertyState* state{};data::PropertyView* properties{};
 data::PlayerSavegameV1* save{};
 const data::CharacterTable* actors{};const data::ClassRow* classes{};
 std::uint32_t class_count{};std::int32_t actor_index{};
 float x{},y{};
 bool player{},remotely_updated{},local{};
 std::int32_t current_level_policy{},current_difficulty{};
};
// Each callback is a reached source boundary, not a blanket accepted event.
// Level presentation must deliver the complete source branch (localized
// message/menu, visual event87, tutorial/campaign gates, local achievements).
struct ProgressionServicesV1 {
 std::function<bool(const char*,std::int32_t&,std::string&)> constant;
 std::function<bool(const char*,bool&,std::string&)> debug;
 std::function<bool(std::string&)> negative_award_assert;
 std::function<bool(ProgressionActorV1&,bool mp,std::string&)> regen_full;
 std::function<bool(ProgressionActorV1&,std::string&)> save;
 std::function<bool(ProgressionActorV1&,std::int32_t level,std::string&)> level_presentation;
 // Original lookup PlayerManager::GetPlayerByCharacter is required even
 // though the subsequent IncreaseStat3790e0 is a literal empty method.
 std::function<bool(ProgressionActorV1&,std::string&)> statistics_player_lookup;
 std::function<bool(ProgressionActorV1& victim,ProgressionActorV1& player,
                    std::int32_t modified_integer,std::string&)> xp_text;
};
struct ProgressionResultV1 {
 bool accepted{},leveled{},complete{};
 std::int32_t raw_requested{},raw_added{},level_before{},level_after{},xp_after{};
};
// Actual source settings row176; no injected XP/range/default rates.
float progression_scaled_xp_v1(float base,std::int32_t player_level,
 std::int32_t victim_level,const data::DesignSettingsProjection176&) noexcept;
std::int32_t progression_modified_xp_v1(std::int32_t raw,std::int32_t property201) noexcept;
std::int32_t progression_award_raw_v1(float scaled) noexcept;
bool progression_give_xp_v1(ProgressionActorV1&,std::int32_t raw,bool record_statistics,
 const ProgressionServicesV1&,ProgressionResultV1&,std::string&);
// Caller supplies actual PlayerManager enumeration (at most4); killer may
// be null. Prefixes survive failure. No retry after partially delivered XP.
bool progression_distribute_xp_v1(ProgressionActorV1* killer,ProgressionActorV1& victim,
 ProgressionActorV1* const* players,std::uint32_t count,
 const data::DesignSettingsProjection176&,const ProgressionServicesV1&,
 std::vector<ProgressionResultV1>&,std::string&);
// Receipt belongs to the SAME actual character lifetime. Arm only at the
// source Kill XP call after the first real alive→dead transition. It marks
// attempted BEFORE dispatch, including prefix failure; it is not a kill hook.
class ProgressionDeathReceiptV1 {
 std::uintptr_t victim_{};bool attempted_{};
public:
 explicit ProgressionDeathReceiptV1(std::uintptr_t victim):victim_(victim){}
 bool dispatch(ProgressionActorV1*,ProgressionActorV1&,bool real_alive_to_dead,
 ProgressionActorV1* const*,std::uint32_t,const data::DesignSettingsProjection176&,
 const ProgressionServicesV1&,std::vector<ProgressionResultV1>&,std::string&);
 bool attempted()const noexcept{return attempted_;}
 // Only an actual source Revive/new lifetime producer may reset delivery.
 void source_revive()noexcept{attempted_=false;}
};
}
