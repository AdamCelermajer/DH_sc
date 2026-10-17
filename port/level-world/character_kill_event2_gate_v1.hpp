#pragma once

#include <cstdint>

namespace dh2::character_kill_event2_gate_v1 {

// Event 2 is the tail of Ctrl_Kill, not a substitute for Character::Kill.
// This typed admission record makes every earlier Kill and OnDied/AI_SetDead
// owner explicit without owning or copying any of those systems.
struct Readiness {
    std::uint32_t kill_tail=0;       // loot, credit/XP and objective tail returned
    std::uint32_t character_ai=0;    // this Character's retained CharAI
    std::uint32_t active_ais=0;      // active AIS is the live source identity
    std::uint32_t script_session=0;  // same retained AIS VM and callback map
    std::uint32_t state_machine=0;   // canonical Character Coordinator/FSM
    std::uint32_t animation=0;       // actual CharAnimTable row and stance path
    std::uint32_t timers=0;          // same CharTimers owner for source IDs 10/14
    std::uint32_t target=0;          // canonical CharAI target/last-target owner
    std::uint32_t relations=0;       // sole bidirectional aggro map + peer lifetime
    std::uint32_t skill_cleanup=0;   // same prepared skill instances/session
    std::uint32_t spell_cleanup=0;   // same prepared faery instances/session
    std::uint32_t group_info=0;      // actual null GroupInfo or its real callback
};

enum Missing : std::uint32_t {
    missing_kill_tail=1u<<0, missing_character_ai=1u<<1,
    missing_active_ais=1u<<2, missing_script_session=1u<<3,
    missing_state_machine=1u<<4, missing_animation=1u<<5,
    missing_timers=1u<<6, missing_target=1u<<7,
    missing_relations=1u<<8, missing_skill_cleanup=1u<<9,
    missing_spell_cleanup=1u<<10, missing_group_info=1u<<11,
};

struct Episode {
    std::uintptr_t character=0;
    std::uint8_t event2_attempted=0;
    std::uint8_t failed=0;
    std::uint16_t reserved=0;
};

struct Request {
    std::uintptr_t character=0,killer=0;
    std::uint32_t kill_completed=0;
    Readiness readiness{};
};

struct Result {
    std::uint32_t missing=0;
    std::uint32_t callback_called=0;
    std::uint32_t delivered=0;
};

using RaiseEvent2 = int (*)(void*,std::uintptr_t character,
                            std::uintptr_t killer);
struct Backend { void* context=nullptr; RaiseEvent2 raise_event2=nullptr; };

enum class Status : std::uint32_t {
    complete=0, invalid_argument=1, missing_owner=2,
    consumed=3, failed=4
};

inline std::uint32_t missing(const Readiness& r) noexcept {
    const std::uint32_t values[]={r.kill_tail,r.character_ai,r.active_ais,
        r.script_session,r.state_machine,r.animation,r.timers,r.target,
        r.relations,r.skill_cleanup,r.spell_cleanup,r.group_info};
    constexpr std::uint32_t bits[]={missing_kill_tail,missing_character_ai,
        missing_active_ais,missing_script_session,missing_state_machine,
        missing_animation,missing_timers,missing_target,missing_relations,
        missing_skill_cleanup,missing_spell_cleanup,missing_group_info};
    std::uint32_t out=0;
    for(unsigned i=0;i<12;++i) {
        if(values[i]>1)return UINT32_MAX;
        if(!values[i])out|=bits[i];
    }
    return out;
}

// Called only after the complete source Character::Kill tail. The callback
// dispatches existing Character::RaiseEvent(2,killer): CharAI::OnDied invokes
// the active AIS OnDied and AI_SetDead before forwarding FSM event 2. Readiness
// requires those existing owners; the gate does not duplicate them. Admission
// failure is retryable because no source effect ran; once callback entry
// occurs, success and failure are both terminal to preserve prefixes.
inline Status dispatch(Episode* episode,const Request* request,
                       const Backend* backend,Result* out) noexcept {
    if(!episode||!request||!backend||!out||!episode->character||
       episode->reserved||episode->event2_attempted>1||episode->failed>1||
       request->character!=episode->character||request->kill_completed>1)
        return Status::invalid_argument;
    if(episode->event2_attempted||episode->failed)return Status::consumed;
    const auto absent=missing(request->readiness);
    if(absent==UINT32_MAX)return Status::invalid_argument;
    if(!request->kill_completed)return Status::missing_owner;
    *out={};out->missing=absent;
    if(absent)return Status::missing_owner;
    if(!backend->raise_event2)return Status::missing_owner;

    episode->event2_attempted=1;
    out->callback_called=1;
    int status=1;
    try { status=backend->raise_event2(backend->context,
                                      request->character,request->killer); }
    catch(...) { status=1; }
    if(status) {
        episode->failed=1;
        return Status::failed;
    }
    out->delivered=1;
    return Status::complete;
}

} // namespace dh2::character_kill_event2_gate_v1
