#pragma once

#include "character_state.hpp"
#include "character_timers.hpp"
#include "character_ai_frame.hpp"
#include "character_skill_state_dispatch_v1.hpp"
#include "character_cast_state_dispatch_v1.hpp"
#include "character_cast_lifecycle_v1.hpp"
#include <array>
#include <vector>

namespace dh2::character {

class Coordinator;
enum class TimerRouting { machine, delivered, failed };

struct SkillProjectionRetirementStatus {
    bool coordinator_bound=false;
    bool projection_matches=false;
    bool dispatch_active=false;
    std::uint32_t timer_update_depth=0;
    std::uintptr_t expected_projection=0;
    std::uintptr_t bound_projection=0;
    std::int32_t current_state=-1;
};

enum class LogicFramePhase : std::uint32_t {
    not_started, character_timers, char_ai, character_state, complete
};

struct LogicFrameResult {
    LogicFramePhase phase = LogicFramePhase::not_started;
    std::int32_t timer_status = 0;
    std::int32_t ai_status = 0;
    std::int32_t state_status = 0;
    AIFrameResult16 ai{};
};

// Runtime composition of the recovered state and timer kernels. This is a
// native owner, not an original Character/CharAI memory-layout overlay.
struct CoordinatorBindings {
    void* context = nullptr;
    Facts (*facts)(void*) = nullptr;
    Services services{};
    // Character/AI timer forwarding remains a host service. The before hook
    // runs synchronously before the state event even if the host's AI virtual
    // callback is suppressed. The captured event cannot be changed by a hook
    // mutating Timer32.event. Both hooks may reenter this same coordinator.
    void (*before_timer_event)(void*, Coordinator&, std::int32_t,
                               Timer32&, std::uint32_t gate_before) = nullptr;
    void (*after_timer_event)(void*, Coordinator&, std::int32_t,
                              Timer32&, std::uint32_t gate_before) = nullptr;
    SpawnFacts (*spawn_facts)(void*) = nullptr;
    // Source Character::RaiseEvent/CharAI dispatch may deliver an event without
    // forwarding to SM_RaiseEvent (for example 0x33/0x34 and direct buff 0x36).
    // Runs between existing before/after observers. Default preserves the
    // existing machine route. Failed/throw preserves timer/provider effects
    // and stops before machine forwarding and the after observer.
    TimerRouting (*route_timer_event)(void*, Coordinator&, std::int32_t,
                                      Timer32&, std::uint32_t gate_before) = nullptr;
    // Optional CSSkill projection. Its machine must be this Coordinator's
    // State; all callback graph storage remains host-owned and borrowed.
    character_skill_state_dispatch_v1::Projection* skill_projection = nullptr;
    // Optional CSCast callback graph, independent of CSSkill. It borrows this
    // Coordinator State plus the existing Character/machine/animator owners.
    character_cast_lifecycle_v1::Projection* cast_projection = nullptr;
    // Source CharAI identity associated with this Character owner. Zero means
    // the relationship is not yet established, so frame dispatch stays closed.
    std::uintptr_t ai_identity = 0;
    // Canonical native projection of this Character's source AI owner and
    // controller. CharAI::Update reads forced/locked from this owner; callers
    // cannot supply a copied owner record with a forged forced bypass.
    AIFrameOwner48* ai_owner_projection = nullptr;
    std::uintptr_t controller_identity = 0;
    // Combat states8-11 borrow the same coordinator State and Character
    // owners; this callback only refreshes transient source facts.
    CombatFacts (*combat_facts)(void*) = nullptr;
};

class Coordinator {
    struct Scope;
    std::vector<Timer32> timer_storage_;
    TimerStore32 timers_{};
    CoordinatorBindings bindings_{};
    Facts* active_facts_ = nullptr;
    std::uint32_t event_cause_ = 0;
    // Canonical source CharStateMachine fields used by SM_SetDeadState. The
    // death sequence itself is State.animation_override (+0x28); these retain
    // the neighboring +0x38 despawn sequence and +0x3e dead byte on this same
    // Coordinator instead of creating a parallel machine owner.
    std::int32_t source_despawn_animation_38_ = 0;
    std::uint8_t source_dead_byte_3e_ = 0;
    // SM_SetInteractState's requested row (+0x48) is retained on this same
    // Character Coordinator so registered event34 can select its source next
    // state without borrowing an animation label or a second FSM owner.
    std::int32_t source_interaction_row_48_ = -1;
    // CSAnim's three Character bytes (see AnimFacts): the CharStateMachine
    // +0x44/+0x45 bytes SM_SetAnimState stores (Character+0x540/+0x541) and the
    // scripted byte Script_MarkCharacterAsScripted writes (Character+0x1480).
    // The pending AnimTable row (+0x28) is State.animation_override.
    std::uint8_t source_anim_wait_540_ = 0;
    std::uint8_t source_anim_idle_541_ = 0;
    std::uint8_t source_scripted_1480_ = 0;
    AnimFacts anim_facts() const noexcept {
        return {source_anim_wait_540_, source_anim_idle_541_, source_scripted_1480_};
    }
    // Character::RegisterState populates the one source CharStateMachine with
    // IDs 0..19. The native dispatcher is shared, but each registered source
    // state retains its handler token on this same Coordinator owner.
    std::array<std::uintptr_t, 20> source_state_handlers_{};

    static void invoke_service(void*, State*, const Request*);
    static int grow_timers(void*, TimerStore32*, std::uint32_t);
    static void timer_expired(void*, std::uintptr_t, std::int32_t, Timer32*);
    Services state_services();
    TimerServices32 timer_services();

public:
    State state;

    explicit Coordinator(std::uintptr_t owner, std::uint32_t initial_timers = 20);
    Coordinator(const Coordinator&) = delete;
    Coordinator& operator=(const Coordinator&) = delete;
    Coordinator(Coordinator&&) = delete;
    Coordinator& operator=(Coordinator&&) = delete;

    // Host backends retain scene, playback, physics, AI and renderer ownership.
    // Both the facts producer and synchronous state services are required.
    void bind(const CoordinatorBindings& bindings);
    bool bound() const;
    // CSSkill callback storage is borrowed. These lifecycle methods may be
    // used after bind() without replacing the Coordinator's State/FSM owner.
    // Projection binding through this API is single-assignment (the same
    // pointer is idempotent); normal unbind requires that exact pointer and
    // is refused while its state is live. Terminal retirement is separate.
    bool bind_skill_projection(
        character_skill_state_dispatch_v1::Projection* projection);
    bool unbind_skill_projection(
        character_skill_state_dispatch_v1::Projection* projection);
    // Terminal Character destruction discards the whole state machine without
    // dispatching an outgoing state callback, matching Character::~Character.
    // This separate path still refuses to detach during a borrowed dispatch.
    bool retire_skill_projection(
        character_skill_state_dispatch_v1::Projection* projection);
    SkillProjectionRetirementStatus skill_projection_retirement_status(
        const character_skill_state_dispatch_v1::Projection* projection) const noexcept;
    bool bind_cast_projection(
        character_cast_lifecycle_v1::Projection* projection);
    bool unbind_cast_projection(
        character_cast_lifecycle_v1::Projection* projection);
    bool retire_cast_projection(
        character_cast_lifecycle_v1::Projection* projection);
    std::uintptr_t owner() const { return timers_.owner; }
    // Source CharStateMachine+0x2c (Character+0x528) is the one machine word
    // that the state kernels carry as State.attack_gate: bit 0 attack-delay
    // gate (event 0x2a), 2 stun (event 0x2b), 4 scare (event 0x2c), 0x8/0x10
    // knock-back facts and 0x20 "not interruptible". Combat status owners read
    // and write that same word; CombatFacts only borrow a copy.
    std::uint32_t source_machine_flags_44_528() const noexcept {
        return state.attack_gate;
    }
    void set_source_machine_flags_44_528(std::uint32_t value) noexcept {
        state.attack_gate = value;
    }
    std::int32_t source_despawn_animation_38() const noexcept {
        return source_despawn_animation_38_;
    }
    void set_source_despawn_animation_38(std::int32_t value) noexcept {
        source_despawn_animation_38_ = value;
    }
    std::uint8_t source_dead_byte_3e() const noexcept {
        return source_dead_byte_3e_;
    }
    void set_source_dead_byte_3e(std::uint8_t value) noexcept {
        source_dead_byte_3e_ = value;
    }
    std::int32_t source_interaction_row_48() const noexcept {
        return source_interaction_row_48_;
    }
    void set_source_interaction_row_48(std::int32_t value) noexcept {
        source_interaction_row_48_ = value;
    }
    // Character+0x1480, written by Script_MarkCharacterAsScripted (0x459550 /
    // 0x45958c) and read by CSAnim::OnUpdate.
    bool source_scripted_1480() const noexcept { return source_scripted_1480_ != 0; }
    void set_source_scripted_1480(bool scripted) noexcept {
        source_scripted_1480_ = scripted ? 1 : 0;
    }
    bool source_anim_wait_540() const noexcept { return source_anim_wait_540_ != 0; }
    bool source_anim_idle_541() const noexcept { return source_anim_idle_541_ != 0; }
    bool register_source_state(std::uintptr_t character, std::uint32_t state,
                               std::uintptr_t handler) noexcept;
    bool unregister_source_state(std::uintptr_t character, std::uint32_t state,
                                 std::uintptr_t handler) noexcept;
    bool source_state_registered(std::uintptr_t character,
                                 std::uint32_t state) const noexcept;
    std::uint32_t source_state_handler_mask() const noexcept;
    bool source_state_handlers_complete() const noexcept;
    std::uint32_t event_cause() const { return event_cause_; }
    const TimerStore32& timers() const { return timers_; }

    // Used by a host service that changes a producer (e.g. Stop clears heading)
    // before the state kernel reads its borrowed Facts again. Nested scopes
    // restore the outer facts/event cause, including when a callback throws.
    void refresh_facts();
    // In CSSkill state6, event0x28 payload is the source NUL-terminated label
    // pointer; other events retain their ordinary opaque payload semantics.
    int event(std::uint32_t event, std::uint64_t payload = 0);
    // CharStateMachine::RaiseStateEvent(50008,payload), bypassing Character's
    // RaiseEvent/CharAI routing and the source-spawn timer/script shortcut.
    int raise_dead_state_event_50008(std::uint64_t payload = 0);
    int transition(std::int32_t next, std::int32_t event = 0,
                   std::uint64_t payload = 0);
    // Source CharStateMachine::SM_SetIdleState(bool): store the existing
    // machine idle-suppression byte, then run the ordinary _SetState(3,-1,0)
    // transition on this same Coordinator.
    int set_idle_state(bool suppressed);
    // Source CharStateMachine::SM_SetAnimState(row, wait_for_end, idle_on_update)
    // 0x3c1aa8: stores the pending AnimTable row (+0x28) and the two bytes
    // (+0x44/+0x45), then runs _SetState(14, -1, 0) on this same Coordinator.
    // CSAnim::OnFocus plays the row through the ordinary set_animation service.
    int set_anim_state(std::int32_t row, bool wait_for_end, bool idle_on_update);
    int spawn_transition(std::int32_t next);
    int spawn_event(std::uint32_t event, const char* exact_event_name = nullptr);
    int update_state(std::uint32_t dt_ms);
    // Dispatch the original CharAI frame through this Character's one FSM
    // owner. The frame projection must mirror this Coordinator's Character
    // owner, CharAI identity, canonical owner projection, and controller;
    // required target/master/aggro/AIS services remain
    // explicit and unavailable services stop at their source-ordered prefix.
    // Returns -1 when the projection is not this Character; otherwise returns
    // dh2_character_ai_frame's status without creating another owner/store.
    int update_ai_frame(AIFrameState32*, const AIFrameServices24*,
                        AIFrameResult16*);
    int update_timers(std::uint32_t dt_ms, std::uint32_t script_blocked);
    // Reconstruct the source Character::Update logic sequence after its
    // outer eligibility gates and optional TimerUtil phase: CharTimers,
    // CharAI::Update, then CharStateMachine::Update. Each existing owner and
    // its service providers remain authoritative; a failure stops at that
    // phase and leaves completed source effects intact.
    int update_logic_frame(std::uint32_t dt_ms, std::uint32_t script_blocked,
                           AIFrameState32*, const AIFrameServices24*,
                           LogicFrameResult*);
    std::int32_t start_timer(std::uint32_t duration_ms, std::int32_t repeat,
                             std::int32_t event, std::uintptr_t user_ref);
    int pause_timer(std::uint32_t id, std::uint32_t paused);
    int stop_timer(std::uint32_t id);
    int stop_timers();
    // Terminal Character destruction discards its CharTimers vector. Release
    // this Coordinator's timer storage without allocation; the next timer
    // start grows it through the existing owner callback.
    bool retire_timers() noexcept;

    // Fresh level/session allocation; forbidden inside a borrowed state or
    // timer update. In-process Activity recreation may instead retain timers.
    void reset_timers(std::uintptr_t owner, std::uint32_t initial_timers = 20);
};

} // namespace dh2::character
