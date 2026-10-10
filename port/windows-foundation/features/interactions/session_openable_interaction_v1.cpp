#include "session_openable_interaction_v1.hpp"
#include "world_object_container_state_v1.hpp"

#include "../../../level-world/openable_container_interaction_v2.hpp"
#include <tuple>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error, const char* message) { error = message; return false; }
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b) noexcept {
    return a && b && !a.owner_before(b) && !b.owner_before(a);
}
}

bool SessionOpenableInteractionV1::EventKey::operator<(const EventKey& o) const noexcept {
    return std::tie(actor, lifecycle, generation, slot, wall_ms, clip, name) <
           std::tie(o.actor, o.lifecycle, o.generation, o.slot, o.wall_ms, o.clip, o.name);
}

SessionOpenableInteractionV1::SessionOpenableInteractionV1(
    SessionOpenableInteractionServicesV1 services) : services_(std::move(services)) {}

std::shared_ptr<SessionOpenableInteractionV1> SessionOpenableInteractionV1::create(
    SessionOpenableInteractionServicesV1 services, std::string& error) {
    error.clear();
    if (!services.session_identity || !services.session_lease || !services.resolve_actor)
        return fail(error, "Session Openable requires same-session identity/lease/ActorId resolver"), nullptr;
    if (!services.bind_retained_visual || !services.play_retained_clip ||
        !services.retained_scene_flags)
        return fail(error, "Session Openable requires retained source visual event/clip/flag services"), nullptr;
    if (services.source.bind_timeline_callbacks || services.source.play_animation ||
        services.source.scene_flags || services.source.drop_loot_table)
        return fail(error, "Session Openable source interaction endpoints already bound"), nullptr;
    static_assert(sizeof(ActorId) <= sizeof(std::uintptr_t),
                  "source actor IDs must fit the original pointer-sized receiver field");
    return std::shared_ptr<SessionOpenableInteractionV1>(
        new SessionOpenableInteractionV1(std::move(services)));
}

bool SessionOpenableInteractionV1::borrow(
    ActorId id, bool require_openable, SessionContainerActorBorrowV1& out,
    std::string& error) {
    out = {};
    if (id == invalid_actor_id) return fail(error, "Session Openable requires valid ActorId");
    auto session_lease = services_.session_lease;
    if (!session_lease) return fail(error, "Session Openable same-session lease expired");
    SessionContainerActorBorrowV1 next;
    if (!services_.resolve_actor(services_.actor_context, services_.session_identity,
                                  id, next, error)) return false;
    if (next.session_identity != services_.session_identity || next.actor_id != id ||
        !same_owner(next.session_lease, session_lease) || !next.definition ||
        next.definition->stableId != id || (!next.state && !next.object) ||
        !next.binding_lifecycle)
        return fail(error, "Session Openable ObjectId/ActorDefinition/session lease mismatch");
    if (next.state && (next.state->id != id ||
        next.state->definition_id != next.definition->name))
        return fail(error, "Session Openable ActorState identity differs from authored definition");
    if (next.object && (next.object->id != id ||
        next.object->name != next.definition->name))
        return fail(error, "Session Openable WorldObject identity differs from authored definition");
    if (require_openable) {
        if (!next.openable_state)
            return fail(error, "Session Openable ActorId has no source-owned container state");
        const auto data_desc = next.definition->properties.find("data_desc");
        if (data_desc == next.definition->properties.end() ||
            data_desc->second != next.openable_state->data_desc)
            return fail(error, "Session Openable definition/data_desc differs from same ActorState");
        const auto initialized = init_attempts_.find(id);
        const bool already_hydrated = initialized != init_attempts_.end() &&
            initialized->second.lifecycle == next.binding_lifecycle;
        if (next.object && !already_hydrated) {
            SourceContainerObjsFieldsV1 saved;
            if (!read_source_container_objs_v1(*next.object,saved,error)) return false;
            next.openable_state->state394=saved.state394;
        }
    }
    out = std::move(next);
    return true;
}

bool SessionOpenableInteractionV1::persist_container_state(
    const SessionContainerActorBorrowV1& actor,std::int32_t state,
    std::string& error) {
    if (!actor.object) { error.clear(); return true; }
    if (state < 0 || state > 255)
        return fail(error,"Session Openable state394 does not fit the source OBJS byte");
    return write_source_container_state394_v1(*actor.object,
        static_cast<std::uint8_t>(state),error);
}

bool SessionOpenableInteractionV1::initialized(
    const SessionContainerActorBorrowV1& actor, std::string& error,
    bool during_init) {
    const auto found = init_attempts_.find(actor.actor_id);
    if (found == init_attempts_.end() || found->second.lifecycle != actor.binding_lifecycle ||
        !found->second.active || (!during_init && !found->second.ready))
        return fail(error, "Session Openable requires successful same-lifecycle source InitPost");
    return true;
}

bool SessionOpenableInteractionV1::source_services(
    ActorId source, dh2::world::OpenableContainerServicesV1& out,
    std::string& error) {
    SessionContainerActorBorrowV1 current;
    if (!borrow(source, true, current, error)) return false;
    out = services_.source;
    // OpenableContainerServices stores a mutable owner lease. Alias the exact
    // same session control block without taking ownership of another runtime.
    const auto lease = std::const_pointer_cast<void>(services_.session_lease);
    out.owner = lease;
    const auto weak = weak_from_this();
    out.bind_timeline_callbacks = [weak, source](std::string& e) {
        auto self = weak.lock();
        return self ? self->register_visual(source, e) : fail(e, "Session Openable owner released");
    };
    out.play_animation = [weak, source](const char* clip, bool& accepted, std::string& e) {
        auto self = weak.lock();
        return self ? self->play_clip(source, clip, accepted, e) : fail(e, "Session Openable owner released");
    };
    out.scene_flags = [weak, source](std::uint32_t clear, std::uint32_t set, std::string& e) {
        auto self = weak.lock();
        return self ? self->scene_flags(source, clear, set, e) : fail(e, "Session Openable owner released");
    };
    out.drop_loot_table = [weak, source](std::int32_t table, std::uintptr_t opener,
        std::int32_t fixed, bool source_flag, std::string& e) {
        auto self = weak.lock();
        return self ? self->drop_table(source, table, opener, fixed, source_flag, e) :
                      fail(e, "Session Openable owner released");
    };
    return true;
}

bool SessionOpenableInteractionV1::init_post(ActorId source, std::string& error) {
    error.clear();
    SessionContainerActorBorrowV1 actor;
    if (!borrow(source, true, actor, error)) return false;
    if (init_attempts_.find(source) != init_attempts_.end())
        return fail(error, "Session Openable InitPost already attempted for ActorId");
    // Latch before callbacks because source InitPost is destructive and may
    // install the retained visual binder before a later prefix fails.
    init_attempts_.emplace(source, InitReceipt{actor.binding_lifecycle, true, false});
    dh2::world::OpenableContainerServicesV1 active;
    if (!source_services(source, active, error)) {
        init_attempts_.at(source).active = false;
        return false;
    }
    dh2::world::OpenableContainerOwnerV1 owner(*actor.openable_state, std::move(active));
    const bool initialized_ok=owner.init_post(error);
    std::string persist_error;
    if(!persist_container_state(actor,actor.openable_state->state394,persist_error)) {
        if(initialized_ok||error.empty())error=persist_error;
        init_attempts_.at(source).active = false;
        return false;
    }
    if (!initialized_ok) {
        init_attempts_.at(source).active = false;
        return false;
    }
    init_attempts_.at(source).ready = true;
    return true;
}

bool SessionOpenableInteractionV1::restore_silently(
    ActorId source,std::string& error){
    error.clear();
    SessionContainerActorBorrowV1 actor;
    if(!borrow(source,true,actor,error))return false;
    if(actor.openable_state->state394!=2&&actor.openable_state->state394!=4)
        return fail(error,"Session Openable silent rebind accepts only restored source states 2 or 4");
    if(init_attempts_.find(source)!=init_attempts_.end())
        return fail(error,"Session Openable restore rebind already attempted for this ActorId");
    if(!services_.source.resolve_row)
        return fail(error,"Session Openable silent rebind requires original source row lookup");
    dh2::world::OpenableContainerRowV1 row;std::int32_t row_id=-1;
    if(!services_.source.resolve_row(actor.openable_state->data_desc,row_id,row,error))return false;
    if(row_id<0)return fail(error,"Session Openable restored data_desc has no original table row");
    actor.openable_state->data374=row_id;
    auto receipt=init_attempts_.emplace(source,
        InitReceipt{actor.binding_lifecycle,true,false}).first;
    if(actor.openable_state->state394==2){
        if(!register_visual(source,error)){
            receipt->second.active=false;return false;
        }
    }
    receipt->second.ready=true;error.clear();return true;
}

bool SessionOpenableInteractionV1::interact(ActorId source, ActorId opener,
                                             std::string& error) {
    error.clear();
    SessionContainerActorBorrowV1 container, actor;
    if (!borrow(source, true, container, error) || !borrow(opener, false, actor, error) ||
        !initialized(container, error)) return false;
    // The original IsInteractive admission gate accepts only state394==2.
    // Keep this check at the same-session boundary so a restored state4 object
    // cannot raise another quest event or run interaction side effects.
    if(container.openable_state->state394!=2)return true;
    dh2::world::OpenableContainerServicesV1 active;
    if (!source_services(source, active, error)) return false;
    dh2::world::OpenableContainerOwnerV1 owner(*container.openable_state, std::move(active));
    const bool ok=dh2::world::openable_container_interact_v2(
        owner, *container.openable_state, services_.interaction,
        static_cast<std::uintptr_t>(opener), error);
    std::string persist_error;
    if(!persist_container_state(container,container.openable_state->state394,persist_error)){
        if(ok||error.empty())error=persist_error;
        return false;
    }
    return ok;
}

bool SessionOpenableInteractionV1::animation_event(
    ActorId source, const RetainedAnimationEvent& event, std::string& error) {
    error.clear();
    SessionContainerActorBorrowV1 container;
    if (!borrow(source, true, container, error) || !initialized(container, error, true)) return false;
    // Source restore state4 selects idleactive; no in-flight `opened` marker
    // from the old visual generation can legitimately survive that restore.
    if(container.openable_state->state394==4)return true;
    EventKey key{source, container.binding_lifecycle, event.generation, event.slot,
                 event.wall_timestamp_ms, event.clip_id, event.name};
    const auto previous = event_receipts_.find(key);
    if (previous != event_receipts_.end()) {
        if (previous->second.success) return true;
        error = previous->second.error;
        return false;
    }
    auto receipt = event_receipts_.emplace(key, Receipt{false, {}}).first;
    dh2::world::OpenableContainerServicesV1 active;
    bool ok = source_services(source, active, error);
    if (ok) {
        dh2::world::OpenableContainerOwnerV1 owner(*container.openable_state, std::move(active));
        ok = owner.animation_event(event.name.c_str(), error);
    }
    std::string persist_error;
    if(!persist_container_state(container,container.openable_state->state394,persist_error)){
        if(ok||error.empty())error=persist_error;
        ok=false;
    }
    receipt->second = {ok, error};
    return ok;
}

bool SessionOpenableInteractionV1::animation_finished(
    ActorId source, std::uint64_t generation, bool timeline_active,
    std::string& error) {
    error.clear();
    SessionContainerActorBorrowV1 container;
    if (!borrow(source, true, container, error) || !initialized(container, error, true)) return false;
    const auto key = std::make_pair(source, generation);
    const auto previous = completion_receipts_.find(key);
    if (previous != completion_receipts_.end()) {
        if (previous->second.success) return true;
        error = previous->second.error;
        return false;
    }
    auto receipt = completion_receipts_.emplace(key, Receipt{false, {}}).first;
    dh2::world::OpenableContainerServicesV1 active;
    bool ok = source_services(source, active, error);
    if (ok) {
        dh2::world::OpenableContainerOwnerV1 owner(*container.openable_state, std::move(active));
        ok = owner.animation_finished(timeline_active, error);
    }
    std::string persist_error;
    if(!persist_container_state(container,container.openable_state->state394,persist_error)){
        if(ok||error.empty())error=persist_error;
        ok=false;
    }
    receipt->second = {ok, error};
    return ok;
}

bool SessionOpenableInteractionV1::drop_table(
    ActorId source, std::int32_t table, std::uintptr_t opener,
    std::int32_t fixed, bool source_flag, std::string& error) {
    SessionContainerActorBorrowV1 container, actor;
    if (!borrow(source, true, container, error) ||
        !borrow(static_cast<ActorId>(opener), false, actor, error)) return false;
    SourceContainerLootReceiptV1 receipt;
    if(container.object)
        return loot_.drop(source,container.binding_lifecycle,*container.definition,
            *container.object,actor.actor_id,table,fixed,source_flag,
            services_.loot,receipt,error);
    return loot_.drop(source, container.binding_lifecycle, *container.definition,
        *container.state, actor.actor_id, table, fixed, source_flag,
        services_.loot, receipt, error);
}

bool SessionOpenableInteractionV1::register_visual(ActorId source, std::string& error) {
    if (!services_.bind_retained_visual)
        return fail(error, "Session Openable retained visual event binder unavailable");
    const auto weak = weak_from_this();
    SessionContainerEventCallbackV1 event = [weak](ActorId actor,
        const RetainedAnimationEvent& value, std::string& e) {
        auto self = weak.lock();
        return self ? self->animation_event(actor, value, e) : fail(e, "Session Openable owner released");
    };
    SessionContainerCompletionCallbackV1 finished = [weak](ActorId actor,
        std::uint64_t generation, bool active, std::string& e) {
        auto self = weak.lock();
        return self ? self->animation_finished(actor, generation, active, e) :
                      fail(e, "Session Openable owner released");
    };
    return services_.bind_retained_visual(services_.visual_context,
        services_.session_identity, source, std::move(event), std::move(finished), error);
}

bool SessionOpenableInteractionV1::play_clip(
    ActorId source, const char* clip, bool& accepted, std::string& error) {
    SessionContainerActorBorrowV1 actor;
    if (!borrow(source, true, actor, error)) return false;
    if (!clip || !services_.play_retained_clip)
        return fail(error, "Session Openable retained source clip service unavailable");
    return services_.play_retained_clip(services_.visual_context,
        services_.session_identity, source, clip, accepted, error);
}

bool SessionOpenableInteractionV1::scene_flags(
    ActorId source, std::uint32_t clear, std::uint32_t set,
    std::string& error) {
    SessionContainerActorBorrowV1 actor;
    if (!borrow(source, true, actor, error)) return false;
    if (!services_.retained_scene_flags)
        return fail(error, "Session Openable retained scene flag service unavailable");
    return services_.retained_scene_flags(services_.visual_context,
        services_.session_identity, source, clear, set, error);
}

} // namespace dh::foundation::interactions
