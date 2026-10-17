#pragma once
#include <cstdint>

namespace dh2::character_skill_fsm_callbacks_v1 {
// Fixed field locations in the captured Character, borrowing the existing
// coordinator's flags/attack_gate and actor storage. No second state owner.
struct Character {
    std::uintptr_t identity,ai,machine,animator,timers;
    std::uint32_t* flags_520;
    std::uint32_t* flags_528;
    // Character+0x412 is the OOI interaction-intent byte (set by
    // Character::UseOOI/ForceUseOOI; cleared when target handling completes).
    std::uint8_t* ooi_intent_412;
    // Character+0x554 aliases the embedded CharStateMachine+0x58 moving byte.
    const std::uint8_t* machine_moving_58;
    const std::uintptr_t* physical_2dc;
};
struct State {Character* character;};
struct Globals {std::uintptr_t debug_switches;};
enum class Callback : std::uint32_t {focus,blur};
enum class Operation : std::uint32_t {
    debug_load,string_construct,debug_query,string_destroy,sync_last_target,
    stop,raise_event,set_animation,set_speed,cancel_sneaking,unpin,pin,
    start_timer,is_monster,is_miniboss,is_boss
};
struct Request {
    Operation operation;
    std::uintptr_t character,subject,string,payload;
    std::uint32_t argument0,argument1,argument2;
    const char* text;
};
struct Response {std::uint32_t word;std::uintptr_t identity;};
struct Services {
    void* context;
    // 0 normal source return; nonzero/exception is a port provider failure.
    // Construct retains real native string storage; subsequent query/destroy
    // use that nonzero identity. Debug uses actual Runtime/load/GetSwitch.
    // Event forwarding may call the frozen skill dispatch/Use adapter. Timer
    // forwards into the same existing Coordinator; normal ID -1 is discarded.
    // Stop/CancelSneaking/physical/animation bodies are mandatory real services,
    // not successful no-ops. Classification words are tested against zero.
    std::int32_t (*invoke)(void*,State*,const Request*,Response*);
};
struct Result {
    std::uintptr_t character,debug,string,physical;
    std::uint32_t calls,debug_constructed,debug_destroyed,flags_written,
                  ooi_intent_cleared,physical_called,timer_attempted,complete;
    Operation last_operation;
};
enum class Status : std::int32_t {
    complete,invalid_argument,invalid_source_fact,service_unavailable,service_failed
};
Status execute(State*,Callback,const Globals*,const Services*,Result*);
// Source CSSkill Focus324B /Blur308B caller control. Entry Character, field
// locations, embedded receiver identities, services and Debug singleton are
// captured. Values at those field locations are read freshly after callbacks.
// Replacing State.character/Globals selection cannot redirect the captured
// original r4/r8. Focus physical2dc is captured before its flags528 update.
// Every nonquery source return, including GetSwitch and TMR_Start, is ignored.
// Stack-corruption traps and dependency bodies are excluded from body credit.
// One thread retains controls, captured owners, field backing and strings
// through synchronous return, including failed calls. Aligned controls/fields
// must be disjoint. Callbacks may nest with distinct outputs/string resources,
// including the same owner; captured backing cannot be retired during a call.
// No rollback, extra destructor, timer, pin,
// event, FSM transition or VM operation is added after provider failure.
} // namespace dh2::character_skill_fsm_callbacks_v1
