#pragma once
#include "../../actor_state.hpp"
#include "../../original_combat_properties.hpp"
#include "../../../game-data/ai.hpp"
#include <functional>

namespace dh::foundation::companions {
struct Borrow {
    ActorState* actor{};
    const OriginalCombatProperties* properties{};
    const dh2::data::AiTables* tables{};
};
enum class Event { friend_spotted, enemy_spotted, master_out_of_sight, master_in_sight,
    master_out_of_range, master_in_ranged_range, master_in_close_range,
    master_in_melee_range, target_died, target_out_of_sight, target_out_of_range,
    target_in_ranged_range, target_in_close_range, target_in_melee_range };
enum class Operation { has_master, set_master, has_target, set_target,
    clear_target, state, move_state_constant, move_to, warp_behind, stop,
    can_attack_from_range, can_attack_in_melee, attack, flee, trace_unable_to_attack,
    is_idle, target_position, look_at_vector, head_to, head_towards, warp_to,
    faery_association, set_faery_association, current_faery_id, change_faery,
    default_update, default_event, has_visual, set_modular_skin,
    is_master_host_player, host_player, get_target, has_path, warp_to_actor,
    create_haste_buff, apply_haste_buff, remove_haste_buff, enable_collisions,
    start_caught_up_timer, attack_current_target };
struct Request { Operation operation{}; ActorId subject=invalid_actor_id;
    ActorId argument=invalid_actor_id; std::array<float,3> vector{}; std::int32_t integer{}; std::uintptr_t token{}; };
struct Response { bool boolean{}; std::int32_t integer{}; ActorId actor=invalid_actor_id;
    std::array<float,3> vector{}; std::uintptr_t token{}; };
// Must invoke SAME source AI/controller/target/master/skill owner. MoveTo uses
// host source command/path publication; WarpBehind uses original warp service.
using Invoke=std::function<bool(ActorState&,const Request&,Response&,std::string&)>;
// Borrowed Lua globals. follower has g_master only; attacking_follower starts
// g_can_attack=true, g_master=nil, g_target=nil. Script owner initializes them.
struct Globals { ActorId master=invalid_actor_id,target=invalid_actor_id; bool can_attack=true; };
const dh2::data::AiProps* row(const Borrow&) noexcept;
bool dispatch(const Borrow&,Globals&,Event,ActorId payload,const Invoke&,std::string&);
// Native source script owner supplies CST_MOVE/OID_HASTE from actual GetPyCst/
// GetPyOID. OnInitFinal level producer remains with actual skill/property owner.
struct ReneGlobals { ActorId master=invalid_actor_id,target=invalid_actor_id;
    bool is_far=false,is_catching_up=false; std::uint32_t catch_time{};
    std::uintptr_t catchup_timer{}; std::optional<std::int32_t> move_state,haste_id; };
bool dispatch_rene(const Borrow&,ReneGlobals&,Event,ActorId payload,const Invoke&,std::string&);
bool update_rene(const Borrow&,ReneGlobals&,const Invoke&,std::string&);
// Register this callback with SAME timer owner; 200ms/no-repeat is authored.
bool rene_caught_up(const Borrow&,ReneGlobals&,const Invoke&,std::string&);
// AISFaery is a native script. Admission must be proved by the source script
// selector, rather than inferred from model/name/AI row. master and skin_id are
// the actual character+1048 and AISFaery+196 cells, borrowed by reference.
struct FaeryBorrow { ActorState* actor{}; ActorId* master{}; std::int32_t* skin_id{};
    std::function<bool(ActorState&,std::string&)> admit_native_script; };
bool dispatch_faery(const FaeryBorrow&,Event,ActorId payload,const Invoke&,std::string&);
bool update_faery(const FaeryBorrow&,const Invoke&,std::string&);
} // namespace dh::foundation::companions
