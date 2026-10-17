#pragma once
#include "ais_external_update.hpp"
#include <cstdint>
namespace dh2::ais_default_collision_persist {
// Shares the actual selected-AIS owner/+bc storage with the frozen OnUpdate
// caller. +c0 is a persistent application-frame marker, not elapsed time.
struct State { ais_external_update::State* ais; std::uint32_t frame_c0; };
struct OwnerFacts {
    std::uintptr_t identity,target_408,master_418;
    std::uint32_t byte_3e0;
};
struct PeerFacts { std::uintptr_t identity; std::uint32_t type_f4; };
struct Application { std::uintptr_t identity; std::uint32_t frame_74; };
enum class View : std::uint32_t { owner,peer,application };
enum class Operation : std::uint32_t {
    state_is_moving, is_character, is_player, is_enemy, set_target,
    cancel_sneaking, frame_delta,
};
struct Request {
    Operation operation;
    std::uintptr_t subject,peer;
    std::uint32_t argument;
};
struct Services {
    void* context;
    // Synchronous genuine callee; zero success and raw source word in output.
    // moving argument=0 means SM_IsMoving(false); set_target is the owning
    // Character's embedded AI_SetTarget(peer,force=0) tail boundary.
    // frame_delta receives the captured original Application identity.
    std::int32_t (*invoke)(void*,State*,const Request*,std::uint32_t*);
    // PURE identity-to-projection/global resolution, not an extra game call.
    // Owner/peer identities are exact keys; application is selected with key0.
    // Return a borrowed matching view without mutating game facts or State.
    std::int32_t (*view)(void*,View,std::uintptr_t,const void**);
};
enum class Decision : std::uint32_t {
    incomplete,not_moving,current_target,collision_false,noncountable_peer,
    same_frame,owner_flagged,target_set,counter_added,
};
struct Result {
    Decision decision;
    std::uint32_t service_calls,view_reads,frame_word,delta_word,
        counter_added,cancel_sneaking_calls,set_target_calls;
    std::uintptr_t captured_target,captured_master;
};
enum class Status : std::int32_t {
    complete,invalid_argument,service_unavailable,service_failed,invalid_source_fact,
};
Status persist(State*,std::uintptr_t peer,std::uint32_t collision_flag,
               const Services*,Result*);
// One owning thread retains the fixed AIS projection/identity, every owner,
// peer, Application and borrowed view/context until return, including retired
// backing. Genuine callbacks may replace the AIS owner and live field values;
// later source owner loads are fresh. View resolution is pure. Same-State
// reentry, replacing State::ais, rewriting identity keys, output/services
// overwrite and borrowed destruction are forbidden. Independent calls may
// nest. Errors preserve effects, including a+c0 store before a GetDt failure;
// no rollback or added cleanup. No synthetic counter accumulation is provided.
} // namespace dh2::ais_default_collision_persist
