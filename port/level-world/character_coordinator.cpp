#include "character_coordinator.hpp"

#include <algorithm>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

namespace dh2::character {

struct Coordinator::Scope {
    Coordinator& coordinator;
    Facts* previous_facts;
    std::uint32_t previous_cause;

    Scope(Coordinator& owner, Facts& facts, std::uint32_t cause)
        : coordinator(owner), previous_facts(owner.active_facts_),
          previous_cause(owner.event_cause_) {
        owner.active_facts_ = &facts;
        owner.event_cause_ = cause;
    }
    ~Scope() {
        coordinator.active_facts_ = previous_facts;
        coordinator.event_cause_ = previous_cause;
    }
};

Coordinator::Coordinator(std::uintptr_t owner, std::uint32_t initial_timers) {
    reset_timers(owner, initial_timers);
}

void Coordinator::bind(const CoordinatorBindings& bindings) {
    if (active_facts_ || timers_.update_depth)
        throw std::logic_error("Character bindings cannot change during dispatch");
    if (!bindings.facts || !bindings.services.invoke)
        throw std::invalid_argument("Character facts and services are required");
    if (bindings.cast_projection &&
        (bindings.cast_projection->character.coordinator_state != &state ||
         bindings.cast_projection->character.flags_520 != &state.flags ||
         !bindings.cast_projection->character.identity ||
         !bindings.cast_projection->character.machine ||
         !bindings.cast_projection->character.animator ||
         !bindings.cast_projection->character.ooi_intent_412 ||
         !bindings.cast_projection->globals.debug_switches ||
         !bindings.cast_projection->services.invoke || state.current == 7))
        throw std::invalid_argument("Cast projection must borrow this Coordinator and valid source owners before state7");
    if (bindings.ai_identity || bindings.ai_owner_projection ||
        bindings.controller_identity) {
        if (!bindings.ai_identity || !bindings.ai_owner_projection ||
            !bindings.controller_identity ||
            bindings.ai_owner_projection->owner != timers_.owner ||
            bindings.ai_owner_projection->controller != bindings.controller_identity)
            throw std::invalid_argument("CharAI frame binding must use this Character and its canonical controller owner");
    }
    bindings_ = bindings;
}

bool Coordinator::bound() const {
    return bindings_.facts && bindings_.services.invoke;
}

bool Coordinator::register_source_state(std::uintptr_t character,
                                        std::uint32_t state_id,
                                        std::uintptr_t handler) noexcept {
    if (!character || character != timers_.owner || state_id >= source_state_handlers_.size() ||
        !handler || active_facts_ || timers_.update_depth)
        return false;
    auto& slot = source_state_handlers_[state_id];
    if (slot) return slot == handler;
    slot = handler;
    return true;
}

bool Coordinator::unregister_source_state(std::uintptr_t character,
                                          std::uint32_t state_id,
                                          std::uintptr_t handler) noexcept {
    if (!character || character != timers_.owner || state_id >= source_state_handlers_.size() ||
        !handler || active_facts_ || timers_.update_depth)
        return false;
    auto& slot = source_state_handlers_[state_id];
    if (slot != handler) return false;
    slot = 0;
    return true;
}

bool Coordinator::source_state_registered(std::uintptr_t character,
                                         std::uint32_t state_id) const noexcept {
    return character && character == timers_.owner &&
        state_id < source_state_handlers_.size() &&
        source_state_handlers_[state_id] != 0;
}

std::uint32_t Coordinator::source_state_handler_mask() const noexcept {
    std::uint32_t mask = 0;
    for (std::size_t i = 0; i < source_state_handlers_.size(); ++i)
        if (source_state_handlers_[i]) mask |= (std::uint32_t{1} << i);
    return mask;
}

bool Coordinator::source_state_handlers_complete() const noexcept {
    return source_state_handler_mask() == ((std::uint32_t{1} << source_state_handlers_.size()) - 1);
}

bool Coordinator::bind_skill_projection(
    character_skill_state_dispatch_v1::Projection* projection) {
    if (!bound() || active_facts_ || timers_.update_depth || !projection ||
        projection->machine != &state)
        return false;
    if (bindings_.skill_projection)
        return bindings_.skill_projection == projection;
    bindings_.skill_projection = projection;
    return true;
}

bool Coordinator::unbind_skill_projection(
    character_skill_state_dispatch_v1::Projection* projection) {
    if (!bound() || active_facts_ || timers_.update_depth || !projection ||
        bindings_.skill_projection != projection || state.current == 6)
        return false;
    bindings_.skill_projection = nullptr;
    return true;
}

bool Coordinator::retire_skill_projection(
    character_skill_state_dispatch_v1::Projection* projection) {
    if (!bound() || active_facts_ || timers_.update_depth || !projection ||
        bindings_.skill_projection != projection)
        return false;
    bindings_.skill_projection = nullptr;
    return true;
}

SkillProjectionRetirementStatus Coordinator::skill_projection_retirement_status(
    const character_skill_state_dispatch_v1::Projection* projection) const noexcept {
    return {bound(),projection&&bindings_.skill_projection==projection,
            active_facts_!=nullptr,timers_.update_depth,
            reinterpret_cast<std::uintptr_t>(projection),
            reinterpret_cast<std::uintptr_t>(bindings_.skill_projection),
            state.current};
}

bool Coordinator::bind_cast_projection(
    character_cast_lifecycle_v1::Projection* projection) {
    if (!bound() || active_facts_ || timers_.update_depth || !projection ||
        projection->character.coordinator_state != &state ||
        projection->character.flags_520 != &state.flags ||
        !projection->character.identity || !projection->character.machine ||
        !projection->character.animator || !projection->character.ooi_intent_412 ||
        !projection->globals.debug_switches || !projection->services.invoke ||
        (!bindings_.cast_projection && state.current == 7))
        return false;
    if (bindings_.cast_projection)
        return bindings_.cast_projection == projection;
    bindings_.cast_projection = projection;
    return true;
}

bool Coordinator::unbind_cast_projection(
    character_cast_lifecycle_v1::Projection* projection) {
    if (!bound() || active_facts_ || timers_.update_depth || !projection ||
        bindings_.cast_projection != projection || state.current == 7)
        return false;
    bindings_.cast_projection = nullptr;
    return true;
}

bool Coordinator::retire_cast_projection(
    character_cast_lifecycle_v1::Projection* projection) {
    if (!bound() || active_facts_ || timers_.update_depth || !projection ||
        bindings_.cast_projection != projection)
        return false;
    bindings_.cast_projection = nullptr;
    return true;
}

Services Coordinator::state_services() { return {this, invoke_service}; }
TimerServices32 Coordinator::timer_services() {
    return {this, timer_expired, grow_timers, 0};
}

void Coordinator::invoke_service(void* context, State* state,
                                 const Request* request) {
    auto& coordinator = *static_cast<Coordinator*>(context);
    if (state != &coordinator.state || !request || !coordinator.bound())
        throw std::logic_error("Character service owner is invalid");
    coordinator.bindings_.services.invoke(
        coordinator.bindings_.services.context, state, request);
}

void Coordinator::refresh_facts() {
    if (active_facts_) *active_facts_ = bindings_.facts(bindings_.context);
}

int Coordinator::event(std::uint32_t event_id, std::uint64_t payload) {
    if (!bound()) return -1;
    if (source_state_handler_mask() && state.current >= 0 && state.current < 20 &&
        !source_state_registered(timers_.owner,
            static_cast<std::uint32_t>(state.current))) return -1;
    // CharStateMachine::RaiseStateEvent clears the machine gate bit of timer
    // events 0x2a/0x2b/0x2c before the current state sees them, whichever
    // state (CSSkill and CSCast included) is current.
    if (event_id >= 0x2au && event_id <= 0x2cu)
        state.attack_gate &= ~(1u << (event_id - 0x2au));
    if ((state.current == 0 || state.current == 1) &&
        bindings_.spawn_facts) return spawn_event(event_id);
    // CSInteract registers event34 -> CSM_AfterInteraction. That callback
    // selects CSLiftingIdle only for requested Interact row 4; row 3, the
    // authored normal-NPC path currently produced by Character::Interact,
    // returns to Idle. Rows 4/5 require the separate LiftableObject owner and
    // remain unavailable until that canonical provider is present.
    if (state.current == 13 && event_id == 34u) {
        if (source_interaction_row_48_ == 4) return -1;
        if (source_interaction_row_48_ != 3) return -1;
        return transition(3, static_cast<std::int32_t>(event_id), payload);
    }
    auto facts = bindings_.facts(bindings_.context);
    Scope scope(*this, facts, event_id);
    if (state.current == 14) {
        // CSAnim registers only 50001 (-> CSMove) and 50005 (-> CSSkill) and
        // handles event 34 itself; everything else is a source no-op.
        const auto anim = anim_facts();
        const auto services = state_services();
        return dh2_character_anim_state_event(&state, &facts, &anim,
            bindings_.skill_projection, event_id, payload, &services);
    }
    const bool cast_source_state=state.current==3||state.current==4||state.current==5;
    if(state.current==7||(event_id==50006u&&cast_source_state)){
        auto* projection=bindings_.cast_projection;
        if(!projection||projection->character.coordinator_state!=&state||
           projection->character.flags_520!=&state.flags)return -1;
        character_cast_state_dispatch_v1::Result result{};
        if(character_cast_state_dispatch_v1::event(&state,event_id,&result)!=
           character_cast_state_dispatch_v1::Status::complete)return -1;
        if(!result.registered)return 0;
        const auto services=state_services();
        return dh2_character_cast_state_transition(&state,&facts,projection,
            result.next,static_cast<std::int32_t>(event_id),payload,&services);
    }
    if(state.current==11&&event_id==50005u){
        auto* projection=bindings_.skill_projection;
        if(!projection||projection->machine!=&state||!bindings_.combat_facts)return -1;
        const auto combat=bindings_.combat_facts(bindings_.context);
        const auto services=state_services();
        return dh2_character_combat_skill_state_transition(&state,&facts,&combat,
            projection,6,static_cast<std::int32_t>(event_id),payload,&services);
    }
    const bool selected_skill_state=state.current==3||state.current==4||
        state.current==5||state.current==6;
    if(state.current==6||(event_id==0xc355&&selected_skill_state)){
        auto* projection=bindings_.skill_projection;
        if(!projection||projection->machine!=&state)return -1;
        character_skill_state_dispatch_v1::Result result{};
        const auto status=character_skill_state_dispatch_v1::event(&state,event_id,
            event_id==0x28?reinterpret_cast<const char*>(static_cast<std::uintptr_t>(payload)):nullptr,
            &result);
        if(status!=character_skill_state_dispatch_v1::Status::complete)return -1;
        if(result.next<0)return 0;
        const auto services=state_services();
        return dh2_character_skill_state_transition(&state,&facts,projection,
            result.next,static_cast<std::int32_t>(event_id),payload,&services);
    }
    const auto services = state_services();
    const bool combat_registration_event=event_id==50008u||event_id==50010u||
        event_id==50011u||event_id==50012u||event_id==50013u;
    if((state.current>=8&&state.current<=11)||
       (combat_registration_event&&(state.current==3||state.current==4||state.current==5))){
        if(!bindings_.combat_facts)return -1;
        const auto combat=bindings_.combat_facts(bindings_.context);
        return dh2_character_combat_state_event(&state,&facts,&combat,event_id,
                                                payload,&services);
    }
    return dh2_character_state_event(&state, &facts, event_id, payload, &services);
}

int Coordinator::raise_dead_state_event_50008(std::uint64_t payload) {
    constexpr std::uint32_t event_id = 50008u;
    if (!bound()) return -1;
    if (source_state_handler_mask() && state.current >= 0 && state.current < 20 &&
        !source_state_registered(timers_.owner,
            static_cast<std::uint32_t>(state.current))) return -1;
    if (state.current == 1 && bindings_.spawn_facts) {
        auto facts = bindings_.facts(bindings_.context);
        const auto spawn = bindings_.spawn_facts(bindings_.context);
        Scope scope(*this, facts, event_id);
        const auto services = state_services();
        return dh2_character_spawn_dead_transition(&state, &facts, &spawn,
                                                    payload, &services);
    }
    if (state.current == 0 && bindings_.spawn_facts)
        return spawn_event(event_id);

    auto facts = bindings_.facts(bindings_.context);
    Scope scope(*this, facts, event_id);
    const auto services = state_services();
    // CSAnim::OnInit registers only 50001 and 50005, so RaiseStateEvent(50008)
    // reaches no transition while a scripted animation is playing.
    if (state.current == 14) return 0;
    if (state.current == 7) {
        auto* projection = bindings_.cast_projection;
        if (!projection || projection->character.coordinator_state != &state ||
            projection->character.flags_520 != &state.flags) return -1;
        character_cast_state_dispatch_v1::Result result{};
        if (character_cast_state_dispatch_v1::event(&state, event_id, &result) !=
            character_cast_state_dispatch_v1::Status::complete) return -1;
        if (!result.registered) return 0;
        return dh2_character_cast_state_transition(&state, &facts, projection,
            result.next, static_cast<std::int32_t>(event_id), payload, &services);
    }
    if (state.current == 6) {
        auto* projection = bindings_.skill_projection;
        if (!projection || projection->machine != &state) return -1;
        character_skill_state_dispatch_v1::Result result{};
        if (character_skill_state_dispatch_v1::event(&state, event_id, nullptr,
                &result) != character_skill_state_dispatch_v1::Status::complete)
            return -1;
        if (result.next < 0) return 0;
        return dh2_character_skill_state_transition(&state, &facts, projection,
            result.next, static_cast<std::int32_t>(event_id), payload, &services);
    }
    const bool combat_registration_event = state.current == 3 ||
        state.current == 4 || state.current == 5;
    if ((state.current >= 8 && state.current <= 11) || combat_registration_event) {
        if (!bindings_.combat_facts) return -1;
        const auto combat = bindings_.combat_facts(bindings_.context);
        return dh2_character_combat_state_event(&state, &facts, &combat,
            event_id, payload, &services);
    }
    return dh2_character_state_event(&state, &facts, event_id, payload,
                                    &services);
}

int Coordinator::transition(std::int32_t next, std::int32_t event_id,
                            std::uint64_t payload) {
    if (!bound()) return -1;
    if (source_state_handler_mask() &&
        ((state.current >= 0 && state.current < 20 &&
          !source_state_registered(timers_.owner,
              static_cast<std::uint32_t>(state.current))) ||
         (next >= 0 && next < 20 &&
          !source_state_registered(timers_.owner,
              static_cast<std::uint32_t>(next))))) return -1;
    auto facts = bindings_.facts(bindings_.context);
    Scope scope(*this, facts, static_cast<std::uint32_t>(event_id));
    const auto services = state_services();
    if (state.current == 1 && next == 12 && event_id == 50008 &&
        bindings_.spawn_facts) {
        const auto spawn = bindings_.spawn_facts(bindings_.context);
        return dh2_character_spawn_dead_transition(&state, &facts, &spawn,
                                                    payload, &services);
    }
    if(next==14&&(state.current==0||state.current==1||state.current==17)){
        // The outgoing Limbus/Spawn/PreSpawn state runs its source OnBlur with
        // the same Spawn facts the other spawn entry points borrow.
        if(!bindings_.spawn_facts)return -1;
        const auto spawn = bindings_.spawn_facts(bindings_.context);
        const auto anim = anim_facts();
        return dh2_character_anim_spawn_state_transition(&state,&facts,&spawn,&anim,
            event_id,&services);
    }
    if(state.current==14&&next==0){
        // SM_SetLimbusState while CSAnim plays: CSLimbus::OnFocus needs the
        // Spawn facts the other Limbus entry points borrow.
        if(!bindings_.spawn_facts)return -1;
        const auto spawn = bindings_.spawn_facts(bindings_.context);
        const auto anim = anim_facts();
        return dh2_character_anim_limbus_transition(&state,&facts,&spawn,&anim,
            event_id,&services);
    }
    if(state.current==14||next==14){
        const auto anim = anim_facts();
        if(state.current==6||state.current==7){
            // CSSkill / CSCast leave through their own callback projections.
            if(state.current==6){
                auto* projection=bindings_.skill_projection;
                if(!projection||projection->machine!=&state)return -1;
                return dh2_character_anim_skill_state_transition(&state,&facts,&anim,
                    projection,next,event_id,payload,&services);
            }
            auto* projection=bindings_.cast_projection;
            if(!projection||projection->character.coordinator_state!=&state||
               projection->character.flags_520!=&state.flags)return -1;
            return dh2_character_anim_cast_state_transition(&state,&facts,&anim,
                projection,next,event_id,payload,&services);
        }
        if(state.current==14&&next==6){
            auto* projection=bindings_.skill_projection;
            if(!projection||projection->machine!=&state)return -1;
            return dh2_character_anim_skill_state_transition(&state,&facts,&anim,
                projection,next,event_id,payload,&services);
        }
        const bool combat_side=(state.current>=8&&state.current<=11)||(next>=8&&next<=11);
        CombatFacts combat{};
        if(combat_side){
            if(!bindings_.combat_facts)return -1;
            combat=bindings_.combat_facts(bindings_.context);
        }
        return dh2_character_anim_state_transition(&state,&facts,&anim,
            combat_side?&combat:nullptr,next,event_id,payload,&services);
    }
    if((state.current>=8&&state.current<=11)||(next>=8&&next<=11)){
        if(!bindings_.combat_facts)return -1;
        const auto combat=bindings_.combat_facts(bindings_.context);
        if((next==6&&state.current==11)||(state.current==6&&next>=8&&next<=11))
            return dh2_character_combat_skill_state_transition(&state,&facts,&combat,
                bindings_.skill_projection,next,event_id,payload,&services);
        if(state.current==7&&next>=8&&next<=11)
            return dh2_character_combat_cast_state_transition(&state,&facts,&combat,
                bindings_.cast_projection,next,event_id,payload,&services);
        return dh2_character_combat_state_transition(&state,&facts,&combat,next,
            event_id,payload,&services);
    }
    if(state.current==7||next==7){
        auto* projection=bindings_.cast_projection;
        if(!projection||projection->character.coordinator_state!=&state||
           projection->character.flags_520!=&state.flags)return -1;
        return dh2_character_cast_state_transition(&state,&facts,projection,next,
            event_id,payload,&services);
    }
    if(state.current==6||next==6){
        auto* projection=bindings_.skill_projection;
        if(!projection||projection->machine!=&state)return -1;
        return dh2_character_skill_state_transition(&state,&facts,projection,next,
            event_id,payload,&services);
    }
    return dh2_character_state_transition(&state, &facts, next, event_id,
                                          payload, &services);
}

int Coordinator::set_idle_state(bool suppressed) {
    if (!bound()) return -1;
    // SM_SetIdleState writes +0x60 before invoking _SetState. Preserve that
    // source prefix even if a later state transition provider rejects.
    state.idle_suppressed = suppressed ? 1u : 0u;
    // _SetState(3) from Limbus / Spawn / PreSpawn runs the spawn-family blur
    // (CSLimbus::OnBlur revive, CSSpawn::OnBlur body creation) before CSIdle.
    if (state.current == 0 || state.current == 1 || state.current == 17)
        return spawn_transition(3);
    return transition(3, -1, 0);
}

int Coordinator::set_anim_state(std::int32_t row, bool wait_for_end,
                                bool idle_on_update) {
    if (!bound()) return -1;
    // SM_SetAnimState writes +0x45, +0x28 and +0x44 before _SetState(14, -1, 0);
    // keep that source prefix even if the transition provider rejects.
    source_anim_idle_541_ = idle_on_update ? 1 : 0;
    state.animation_override = row;
    source_anim_wait_540_ = wait_for_end ? 1 : 0;
    return transition(14, -1, 0);
}

int Coordinator::update_state(std::uint32_t dt_ms) {
    if (!bound()) return -1;
    if (source_state_handler_mask() && state.current >= 0 && state.current < 20 &&
        !source_state_registered(timers_.owner,
            static_cast<std::uint32_t>(state.current))) return -1;
    if (state.current == 14) {
        auto facts = bindings_.facts(bindings_.context);
        Scope scope(*this, facts, event_cause_);
        const auto anim = anim_facts();
        const auto services = state_services();
        return dh2_character_anim_state_update(&state, &facts, &anim, dt_ms, &services);
    }
    if(state.current>=8&&state.current<=11){
        if(!bindings_.combat_facts)return -1;
        auto facts=bindings_.facts(bindings_.context);
        const auto combat=bindings_.combat_facts(bindings_.context);
        Scope scope(*this,facts,0);
        const auto services=state_services();
        return dh2_character_combat_state_update(&state,&facts,&combat,dt_ms,&services);
    }
    if(state.current==7){
        if(!bindings_.cast_projection||
           bindings_.cast_projection->character.coordinator_state!=&state||
           bindings_.cast_projection->character.flags_520!=&state.flags)return -1;
        state.elapsed_ms+=dt_ms; // CSCast::OnUpdate is a source no-op.
        return 1;
    }
    if ((state.current == 0 || state.current == 1) &&
        bindings_.spawn_facts) {
        const auto spawn = bindings_.spawn_facts(bindings_.context);
        const int result = dh2_character_spawn_update(&state, &spawn);
        if (result == 0) state.elapsed_ms += dt_ms;
        return result;
    }
    if(state.current==6){
        if(!bindings_.skill_projection||bindings_.skill_projection->machine!=&state)
            return -1;
        character_skill_state_dispatch_v1::Result result{};
        if(character_skill_state_dispatch_v1::update(&state,&result)!=
           character_skill_state_dispatch_v1::Status::complete)return -1;
        state.elapsed_ms+=dt_ms;
        return 1;
    }
    auto facts = bindings_.facts(bindings_.context);
    Scope scope(*this, facts, event_cause_);
    const auto services = state_services();
    return dh2_character_state_update(&state, &facts, dt_ms, &services);
}

int Coordinator::update_ai_frame(AIFrameState32* frame,
                                const AIFrameServices24* services,
                                AIFrameResult16* result) {
    if (!bound() || !frame || !frame->owner || !bindings_.ai_identity ||
        !bindings_.ai_owner_projection || !bindings_.controller_identity ||
        frame->ai != bindings_.ai_identity ||
        frame->owner != bindings_.ai_owner_projection ||
        frame->owner->controller != bindings_.controller_identity ||
        frame->owner->owner != owner() ||
        frame->owner->flags520 != state.flags ||
        frame->owner->locked != state.controller_locked)
        return -1;
    return dh2_character_ai_frame(result, frame, services);
}

int Coordinator::spawn_transition(std::int32_t next) {
    if (!bound() || !bindings_.spawn_facts) return -1;
    if (source_state_handler_mask() &&
        ((state.current >= 0 && state.current < 20 &&
          !source_state_registered(timers_.owner,
              static_cast<std::uint32_t>(state.current))) ||
         (next >= 0 && next < 20 &&
          !source_state_registered(timers_.owner,
              static_cast<std::uint32_t>(next))))) return -1;
    auto facts = bindings_.facts(bindings_.context);
    const auto spawn = bindings_.spawn_facts(bindings_.context);
    Scope scope(*this, facts, 0);
    const auto services = state_services();
    return dh2_character_spawn_transition(&state, &facts, &spawn, next, &services);
}

int Coordinator::spawn_event(std::uint32_t event_id, const char* exact_name) {
    if (!bound() || !bindings_.spawn_facts) return -1;
    if (source_state_handler_mask() && state.current >= 0 && state.current < 20 &&
        !source_state_registered(timers_.owner,
            static_cast<std::uint32_t>(state.current))) return -1;
    auto facts = bindings_.facts(bindings_.context);
    const auto spawn = bindings_.spawn_facts(bindings_.context);
    Scope scope(*this, facts, event_id);
    const auto services = state_services();
    return dh2_character_spawn_event(&state, &facts, &spawn, event_id,
                                    exact_name, &services);
}

int Coordinator::grow_timers(void* context, TimerStore32* store,
                             std::uint32_t minimum) {
    auto& coordinator = *static_cast<Coordinator*>(context);
    if (store != &coordinator.timers_ || store->update_depth) return 0;
    const auto doubled = static_cast<std::uint64_t>(store->capacity) * 2;
    const auto capacity = std::max<std::uint64_t>(minimum,
        std::max<std::uint64_t>(20, std::min<std::uint64_t>(doubled,
            std::numeric_limits<std::int32_t>::max())));
    try {
        coordinator.timer_storage_.resize(static_cast<std::size_t>(capacity));
    } catch (const std::bad_alloc&) {
        return 0;
    } catch (const std::length_error&) {
        return 0;
    }
    store->slots = coordinator.timer_storage_.data();
    store->capacity = static_cast<std::uint32_t>(capacity);
    return 1;
}

void Coordinator::timer_expired(void* context, std::uintptr_t owner,
                                std::int32_t event_id, Timer32* timer) {
    auto& coordinator = *static_cast<Coordinator*>(context);
    if (owner != coordinator.owner() || !timer || !coordinator.bound())
        throw std::logic_error("Character timer owner is invalid");
    const auto gate = coordinator.state.attack_gate;
    if (coordinator.bindings_.before_timer_event)
        coordinator.bindings_.before_timer_event(coordinator.bindings_.context,
            coordinator, event_id, *timer, gate);
    const auto route = coordinator.bindings_.route_timer_event
        ? coordinator.bindings_.route_timer_event(coordinator.bindings_.context,
            coordinator, event_id, *timer, gate)
        : TimerRouting::machine;
    if (route == TimerRouting::machine) {
        if (coordinator.event(static_cast<std::uint32_t>(event_id),
                              reinterpret_cast<std::uintptr_t>(timer)) < 0)
            throw std::runtime_error("Character timer state forwarding failed");
    } else if (route != TimerRouting::delivered) {
        throw std::runtime_error("Character timer source routing failed");
    }
    if (coordinator.bindings_.after_timer_event)
        coordinator.bindings_.after_timer_event(coordinator.bindings_.context,
            coordinator, event_id, *timer, gate);
}

int Coordinator::update_timers(std::uint32_t dt_ms, std::uint32_t script_blocked) {
    if (!bound()) return -1;
    const auto services = timer_services();
    const auto previous_depth = timers_.update_depth;
    try {
        return dh2_character_timers_update(&timers_, dt_ms, script_blocked, &services);
    } catch (...) {
        // The host may report a service failure with an exception. Keep already
        // completed state/timer side effects, but release the native borrow.
        timers_.update_depth = previous_depth;
        throw;
    }
}

int Coordinator::update_logic_frame(std::uint32_t dt_ms,
                                    std::uint32_t script_blocked,
                                    AIFrameState32* frame,
                                    const AIFrameServices24* ai_services,
                                    LogicFrameResult* result) {
    if (!result || !bound() || !frame || !ai_services ||
        !bindings_.ai_owner_projection)
        return -1;
    *result = {};

    result->phase = LogicFramePhase::character_timers;
    result->timer_status = update_timers(dt_ms, script_blocked);
    if (result->timer_status != 1) return -2;

    // CharTimers can synchronously transition Character state. This is the
    // same canonical owner projection already required by update_ai_frame,
    // refreshed from this Coordinator rather than copied into a second owner.
    bindings_.ai_owner_projection->flags520 = state.flags;
    bindings_.ai_owner_projection->locked = state.controller_locked;

    result->phase = LogicFramePhase::char_ai;
    result->ai_status = update_ai_frame(frame, ai_services, &result->ai);
    if (result->ai_status != 0) return -3;

    // CharAI callbacks may synchronously change the same Character fields.
    bindings_.ai_owner_projection->flags520 = state.flags;
    bindings_.ai_owner_projection->locked = state.controller_locked;

    result->phase = LogicFramePhase::character_state;
    result->state_status = update_state(dt_ms);
    if (result->state_status < 0) return -4;

    result->phase = LogicFramePhase::complete;
    return 0;
}

std::int32_t Coordinator::start_timer(std::uint32_t duration_ms,
    std::int32_t repeat, std::int32_t event_id, std::uintptr_t user_ref) {
    if (!bound()) return -1;
    const auto services = timer_services();
    return dh2_character_timer_start(&timers_, duration_ms, repeat, event_id,
                                     user_ref, &services);
}

int Coordinator::pause_timer(std::uint32_t id, std::uint32_t paused) {
    return dh2_character_timer_pause(&timers_, id, paused);
}
int Coordinator::stop_timer(std::uint32_t id) {
    return dh2_character_timer_stop(&timers_, id);
}
int Coordinator::stop_timers() { return dh2_character_timers_stop_all(&timers_); }

bool Coordinator::retire_timers() noexcept {
    if (active_facts_ || timers_.update_depth) return false;
    const auto owner = timers_.owner;
    std::vector<Timer32> empty;
    timer_storage_.swap(empty);
    timers_ = {nullptr, 0, 0, owner, 0, 0};
    return true;
}

void Coordinator::reset_timers(std::uintptr_t owner, std::uint32_t initial_timers) {
    if (active_facts_ || timers_.update_depth)
        throw std::logic_error("Character timer storage is borrowed");
    if (!owner || !initial_timers ||
        initial_timers > std::uint32_t(std::numeric_limits<std::int32_t>::max()))
        throw std::invalid_argument("Character timer owner and capacity are required");
    std::vector<Timer32> fresh(initial_timers);
    timer_storage_ = std::move(fresh);
    timers_ = {timer_storage_.data(), 0, initial_timers, owner, 0, 0};
}

} // namespace dh2::character
