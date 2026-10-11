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
    // Source CharStateMachine+0x44 overlaps Character+0x528. Keep this raw
    // persistent word on the one Coordinator; CombatFacts only borrow a copy.
    std::uint32_t source_machine_flags_44_528_ = 0;
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
    std::uint32_t source_machine_flags_44_528() const noexcept {
        return source_machine_flags_44_528_;
    }
    void set_source_machine_flags_44_528(std::uint32_t value) noexcept {
        source_machine_flags_44_528_ = value;
    }
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
    int transition(std::int32_t next, std::int32_t event = 0,
                   std::uint64_t payload = 0);
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
