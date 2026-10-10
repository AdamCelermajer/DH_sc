#pragma once
#include "../../actor_state.hpp"
#include "../../original_combat_properties.hpp"
#include "../../../game-data/ai.hpp"
#include "../../../level-world/character_update_aggro_v108.hpp"
#include <functional>

namespace dh::foundation::enemy_ai {
// All identities are the caller's same live actor/target identities. No copied
// world, private target, FSM state, detection radius or elapsed clock is owned.
struct Borrow {
    ActorState* actor{};
    const OriginalCombatProperties* properties{};
    const dh2::data::AiTables* tables{};
};
enum class MonsterEvent { enemy_spotted, target_died, target_out_of_sight,
    target_in_sight, target_out_of_range, target_in_ranged_range,
    target_in_close_range, target_in_melee_range, target_hit, flee,
    died, terminate };
enum class MonsterMeleeAction : std::uint8_t { attack, do_skill_zero };
enum class Operation { state, state_constant, has_path, state_time, position,
    random, stop, set_target, clear_target, head_to, move_to, attack, do_skill,
    flee, clear_aggro, create_buff, apply_buff, remove_buff };
struct Request {
    Operation operation{};
    ActorId target=invalid_actor_id;
    std::string name;
    std::int32_t argument0{}, argument1{};
    std::uintptr_t token{};
};
struct Response {
    std::int32_t integer{};
    std::uint32_t time_ms{};
    float x{},y{};
    std::uintptr_t token{};
};
using Invoke=std::function<bool(ActorState&,const Request&,Response&,std::string&)>;
// These are the actual monster.lua globals, borrowed from the script owner.
// Caller supplies results of source load/OnInit: SkillTree FromFixed(GetProp28),
// Buff_Speed GetPyOID, flee=true, buff=nil and GetPosition saved_X/saved_Y.
// Dynamic level initialization and script registration are separate services.
struct MonsterGlobals {
    float skill_tree_id=-1;
    std::int32_t buff_speed_id=-1;
    std::uintptr_t buff{};
    bool flee_flag=true;
    float saved_x{},saved_y{};
};
// Recovered original cache monster.luac event bodies. Service effects are
// incremental; failure stops the exact reached prefix with a useful error.
bool dispatch_monster_event(const Borrow&,MonsterGlobals&,MonsterEvent,
                           ActorId payload,ActorId defender,const Invoke&,std::string&);
bool source_target_event(std::uint32_t,MonsterEvent&) noexcept;
const dh2::data::AiProps* row(const Borrow&) noexcept;
// Exact source monster_OnTargetInMeleeRange choice after Stop and the optional
// GetRand(0,100) read. `source_roll` is consumed only for a skill-tree owner.
// The active AIS/SkillAI still executes DoSkill through its existing owner.
MonsterMeleeAction choose_monster_melee_action(float skill_tree_id,
                                               std::int32_t source_roll) noexcept;
// Only the source search's signed property gate, not complete visibility or
// target admission. Search separately checks visibility/zoning/interaction.
bool search_sneak_gate(const OriginalCombatProperties& owner,
                      const OriginalCombatProperties& candidate) noexcept;
// Reuses whole recovered _UpdateAggro0x3cf3f0. Only table radius providers are
// supplied here. Original classification/MyTurn/random/Debug/search/aggro/event
// services stay required. Mutable fields and target are borrowed source owners;
// SetTarget/raise callbacks must publish the same ActorState target immediately.
bool update_aggro(const Borrow&,dh2::character::CharacterAiPointerFieldsV105&,
    dh2::character::TargetState48&,dh2::character::AggroFrameServicesV108,std::string&);
} // namespace dh::foundation::enemy_ai
