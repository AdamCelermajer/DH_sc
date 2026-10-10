#include "session_destructible_interaction_v1.hpp"
#include "world_object_container_state_v1.hpp"

#include <tuple>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error, const char* message) { error = message; return false; }
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b) noexcept {
    return a && b && !a.owner_before(b) && !b.owner_before(a);
}
}

bool SessionDestructibleInteractionV1::EventKey::operator<(const EventKey& o) const noexcept {
    return std::tie(actor,lifecycle,generation,slot,wall_ms,clip,name) <
           std::tie(o.actor,o.lifecycle,o.generation,o.slot,o.wall_ms,o.clip,o.name);
}

SessionDestructibleInteractionV1::SessionDestructibleInteractionV1(
    SessionDestructibleInteractionServicesV1 services) : services_(std::move(services)) {}

std::shared_ptr<SessionDestructibleInteractionV1> SessionDestructibleInteractionV1::create(
    SessionDestructibleInteractionServicesV1 services, std::string& error) {
    error.clear();
    if (!services.session_identity || !services.session_lease || !services.resolve_actor ||
        !services.table)
        return fail(error, "Session Destructible requires same-session identity/lease/ActorId resolver and source table"), nullptr;
    return std::shared_ptr<SessionDestructibleInteractionV1>(
        new SessionDestructibleInteractionV1(std::move(services)));
}

bool SessionDestructibleInteractionV1::borrow(
    ActorId id, SessionContainerActorBorrowV1& out, std::string& error) {
    out = {};
    if (id == invalid_actor_id || !services_.session_lease)
        return fail(error, "Session Destructible requires live source ActorId/session lease");
    SessionContainerActorBorrowV1 next;
    if (!services_.resolve_actor(services_.actor_context, services_.session_identity,
                                  id, next, error)) return false;
    if (next.session_identity != services_.session_identity || next.actor_id != id ||
        !same_owner(next.session_lease, services_.session_lease) || !next.definition ||
        next.definition->stableId != id || (!next.state && !next.object) ||
        !next.binding_lifecycle || !next.destructible_state)
        return fail(error, "Session Destructible ObjectId/ActorDefinition/session lease mismatch");
    if(next.state&&(next.state->id!=id||next.state->definition_id!=next.definition->name))
        return fail(error,"Session Destructible ActorState identity differs from authored definition");
    if(next.object&&(next.object->id!=id||next.object->name!=next.definition->name))
        return fail(error,"Session Destructible WorldObject identity differs from authored definition");
    const auto data_desc = next.definition->properties.find("data_desc");
    if (data_desc == next.definition->properties.end() ||
        data_desc->second != next.destructible_state->data_desc)
        return fail(error, "Session Destructible definition/data_desc differs from same ActorState");
    if(next.object){SourceContainerObjsFieldsV1 saved;
        if(!read_source_container_objs_v1(*next.object,saved,error))return false;
        next.destructible_state->state394=saved.state394;}
    out = std::move(next);
    return true;
}

bool SessionDestructibleInteractionV1::borrow_interactor(
    ActorId id,SessionContainerActorBorrowV1& out,std::string& error){
    out={};
    if(id==invalid_actor_id||!services_.session_lease)
        return fail(error,"Session Destructible opener requires current ActorId/session lease");
    SessionContainerActorBorrowV1 next;
    if(!services_.resolve_actor(services_.actor_context,services_.session_identity,id,next,error))
        return false;
    if(next.session_identity!=services_.session_identity||next.actor_id!=id||
       !same_owner(next.session_lease,services_.session_lease)||!next.definition||
       next.definition->stableId!=id||(!next.state&&!next.object)||!next.binding_lifecycle)
        return fail(error,"Session Destructible opener ObjectId/definition/session lease mismatch");
    if(next.state&&(next.state->id!=id||next.state->definition_id!=next.definition->name))
        return fail(error,"Session Destructible opener ActorState identity differs from definition");
    if(next.object&&(next.object->id!=id||next.object->name!=next.definition->name))
        return fail(error,"Session Destructible opener WorldObject identity differs from definition");
    out=std::move(next);error.clear();return true;
}

bool SessionDestructibleInteractionV1::persist_container_state(
    const SessionContainerActorBorrowV1& actor,std::int32_t state,
    std::string& error){
    if(!actor.object){error.clear();return true;}
    if(state<0||state>255)return fail(error,"Session Destructible state394 does not fit the source OBJS byte");
    return write_source_container_state394_v1(*actor.object,static_cast<std::uint8_t>(state),error);
}

bool SessionDestructibleInteractionV1::initialize(ActorId source, std::string& error) {
    error.clear();
    SessionContainerActorBorrowV1 actor;
    if (!borrow(source, actor, error)) return false;
    if (init_attempts_.find(source) != init_attempts_.end())
        return fail(error, "Session Destructible initialization already attempted for ActorId");
    auto& fields = *actor.destructible_state;
    fields.data_id = services_.table->data_id(fields.data_desc);
    const auto* row = services_.table->row(fields.data_id);
    if (!row) return fail(error, "Session Destructible data_desc absent from original source table");
    if (!services_.visual_asset)
        return fail(error, "Session Destructible requires actual source visual asset resolver");
    if (!services_.visual_asset(services_.visual_context, services_.session_identity,
                                source, row->visual(), error)) return false;
    // Latch the lifecycle before registering external callbacks. A prefix
    // failure is terminal for this binding, matching the source's destructive
    // open/drop boundary and preventing duplicate animation listeners.
    init_attempts_.emplace(source, std::make_pair(actor.binding_lifecycle, false));
    if (!services_.load_object_script)
        return fail(error, "Session Destructible requires same-session source object script loader");
    if (!services_.load_object_script(
            services_.visual_context, services_.session_identity, source,
            row->script30.c_str(), "data/scripts/objects/", error)) return false;
    if (!services_.has_visual)
        return fail(error, "Session Destructible requires same-session retained visual query");
    if (!services_.has_visual(services_.visual_context, services_.session_identity,
                              source, fields.has_visual, error)) return false;
    if (fields.has_visual) {
        if (!services_.bind_retained_visual || !services_.animation_count)
            return fail(error, "Session Destructible visual present but authored timeline/count provider unavailable");
        const auto weak = weak_from_this();
        SessionContainerEventCallbackV1 event = [weak](ActorId actor_id,
            const RetainedAnimationEvent& value, std::string& e) {
            auto self=weak.lock();
            return self ? self->animation_event(actor_id,value,e) : fail(e,"Session Destructible owner released");
        };
        SessionContainerCompletionCallbackV1 finished = [weak](ActorId actor_id,
            std::uint64_t generation, bool active, std::string& e) {
            auto self=weak.lock();
            return self ? self->animation_finished(actor_id,generation,active,e) :
                          fail(e,"Session Destructible owner released");
        };
        if (!services_.bind_retained_visual(services_.visual_context,
                services_.session_identity, source, std::move(event), std::move(finished), error)) return false;
        if (!services_.animation_count(services_.visual_context,
                services_.session_identity, source, fields.animation_count, error)) return false;
        if (fields.animation_count > 3)
            fields.stages = fields.remaining = fields.animation_count - 3;
    }
    fields.initialized = true;
    init_attempts_.at(source).second = true;
    return true;
}

bool SessionDestructibleInteractionV1::services(ActorId source, std::string& error) {
    SessionContainerActorBorrowV1 actor;
    if (!borrow(source, actor, error)) return false;
    const auto found = init_attempts_.find(source);
    if (found == init_attempts_.end() || found->second.first != actor.binding_lifecycle ||
        !found->second.second || !actor.destructible_state->initialized)
        return fail(error, "Session Destructible requires successful same-lifecycle visual/table initialization");
    return true;
}

bool SessionDestructibleInteractionV1::raise_destroy(
    ActorId source, ActorId actor_id, SessionContainerActorBorrowV1& source_actor,
    std::string& error) {
    const auto* row = services_.table->row(source_actor.destructible_state->data_id);
    if (!row) return fail(error, "Session Destructible lost original source row");
    if (!services_.raise_destroy_quest)
        return fail(error, "Session Destructible requires same-session DestroyGameObject event provider");
    return services_.raise_destroy_quest(services_.visual_context,
        services_.session_identity, source, actor_id,
        source_actor.destructible_state->data_id, error);
}

bool SessionDestructibleInteractionV1::drop_table(
    ActorId source, std::int32_t table, std::uintptr_t opener,
    std::int32_t fixed, bool source_flag, std::string& error) {
    SessionContainerActorBorrowV1 container, actor;
    if (!borrow(source, container, error) ||
        !borrow_interactor(static_cast<ActorId>(opener), actor, error)) return false;
    SourceContainerLootReceiptV1 receipt;
    if(container.object)
        return loot_.drop(source,container.binding_lifecycle,*container.definition,
            *container.object,actor.actor_id,table,fixed,source_flag,
            services_.loot,receipt,error);
    return loot_.drop(source, container.binding_lifecycle, *container.definition,
        *container.state, actor.actor_id, table, fixed, source_flag,
        services_.loot, receipt, error);
}

bool SessionDestructibleInteractionV1::open(
    ActorId source, SessionContainerActorBorrowV1& actor, std::string& error) {
    auto& fields = *actor.destructible_state;
    const auto* row = services_.table->row(fields.data_id);
    if (!row) return fail(error, "Session Destructible missing original source row");
    SourceContainerLootReceiptV1 receipt;
    const bool dropped=actor.object ?
        loot_.drop(source,actor.binding_lifecycle,*actor.definition,*actor.object,
            fields.opener,row->loot(),-1,false,services_.loot,receipt,error) :
        loot_.drop(source, actor.binding_lifecycle, *actor.definition, *actor.state,
            fields.opener, row->loot(), -1, false, services_.loot, receipt, error);
    if(!dropped)return false;
    bool script{};
    if (!services_.has_script || !services_.has_script(services_.visual_context,
            services_.session_identity, source, script, error))
        return fail(error, "Session Destructible requires source LuaScript presence provider");
    return !script || (services_.script_call && services_.script_call(
        services_.visual_context, services_.session_identity, source,
        "OnOpen", fields.opener, nullptr, error));
}

bool SessionDestructibleInteractionV1::play(
    ActorId source, const char* clip, bool& accepted, std::string& error) {
    if (!services_.play_retained_clip || !clip)
        return fail(error, "Session Destructible requires current retained clip service");
    return services_.play_retained_clip(services_.visual_context,
        services_.session_identity, source, clip, accepted, error);
}

bool SessionDestructibleInteractionV1::flags(
    ActorId source, std::uint32_t clear, std::uint32_t set,
    std::string& error) {
    if (!services_.retained_scene_flags)
        return fail(error, "Session Destructible requires same-session retained scene flags");
    return services_.retained_scene_flags(
        services_.visual_context, services_.session_identity, source, clear, set, error);
}

bool SessionDestructibleInteractionV1::interact(
    ActorId source, ActorId opener, std::string& error) {
    error.clear();
    SessionContainerActorBorrowV1 container, opener_actor;
    if (!borrow(source, container, error) || !borrow_interactor(opener, opener_actor, error) ||
        !services(source, error)) return false;
    auto& fields = *container.destructible_state;
    const bool action_ok=[&](){
        const auto* row = services_.table->row(fields.data_id);
        if (!row) return fail(error, "Session Destructible lost original source row");
        if (fields.remaining) {
            --fields.remaining;
            if (fields.has_visual) {
                if (!services_.play_retained_index)
                    return fail(error, "Session Destructible requires same-session retained indexed clip playback");
                if (!services_.play_retained_index(services_.visual_context,
                        services_.session_identity, source,
                        fields.stages - fields.remaining, false, error)) return false;
            }
            if(!services_.play_sound_3d)
                return fail(error,"Session Destructible requires same-session source sound callback");
            return services_.play_sound_3d(
                services_.visual_context, services_.session_identity, source,
                row->sound(), error);
        }
        if (!raise_destroy(source, opener, container, error)) return false;
        fields.opener = opener;
        fields.state394 = 4;
        if (!row->keep_physics20 && (!services_.detach_physical || !services_.detach_physical(
                services_.visual_context, services_.session_identity, source, error))) return false;
        if (fields.has_visual) {
            fields.state394 = 3;
            bool accepted{};
            if (!play(source, "activate", accepted, error)) return false;
        } else if (!open(source, container, error)) return false;
        if (!services_.play_sound_3d || !services_.play_sound_3d(
                services_.visual_context, services_.session_identity, source,
                row->sound(), error)) return false;
        if (!services_.source_on_interact || !services_.source_on_interact(
                services_.visual_context, services_.session_identity, source, error)) return false;
        bool character{};
        if (!services_.as_character || !services_.as_character(
                services_.visual_context, services_.session_identity, opener, character, error)) return false;
        if (!character) return true;
        if (!services_.increment_stat || !services_.increment_stat(
                services_.visual_context, services_.session_identity, opener, 217, 1, error)) return false;
        std::int32_t count{};
        if (!services_.get_stat || !services_.get_stat(
                services_.visual_context, services_.session_identity, opener, 217, count, error)) return false;
        if (count <= 199) return true;
        bool local{};
        if (!services_.is_local_player || !services_.is_local_player(
                services_.visual_context, services_.session_identity, opener, local, error)) return false;
        if (!local) return true;
        std::int32_t trophy{};
        if (!services_.trophy_id || !services_.trophy_id(
                services_.visual_context, "destroy_200_breakables", trophy, error)) return false;
        return services_.unlock_trophy && services_.unlock_trophy(
            services_.visual_context, trophy, error);
    }();
    std::string persist_error;
    if(!persist_container_state(container,fields.state394,persist_error)){
        if(action_ok||error.empty())error=persist_error;
        return false;
    }
    return action_ok;
}

bool SessionDestructibleInteractionV1::animation_event(
    ActorId source, const RetainedAnimationEvent& event, std::string& error) {
    error.clear();
    if (!services(source, error)) return false;
    SessionContainerActorBorrowV1 actor;
    if (!borrow(source, actor, error)) return false;
    EventKey key{source,actor.binding_lifecycle,event.generation,event.slot,
                 event.wall_timestamp_ms,event.clip_id,event.name};
    const auto previous=event_receipts_.find(key);
    if (previous!=event_receipts_.end()) {
        if (previous->second.success) return true;
        error=previous->second.error; return false;
    }
    auto receipt=event_receipts_.emplace(key,Receipt{false,{}}).first;
    bool ok=true;
    if (event.name=="fx") { receipt->second={true,{}}; return true; }
    if (event.name=="opened") {
        ok=raise_destroy(source,invalid_actor_id,actor,error) && open(source,actor,error);
    } else {
        bool script{};
        ok=services_.has_script && services_.has_script(services_.visual_context,
            services_.session_identity,source,script,error);
        if (ok && script) ok=services_.script_call && services_.script_call(
            services_.visual_context,services_.session_identity,source,
            "OnAnimEvent",invalid_actor_id,event.name.c_str(),error);
    }
    std::string persist_error;
    if(!persist_container_state(actor,actor.destructible_state->state394,persist_error)){
        if(ok||error.empty())error=persist_error;
        ok=false;
    }
    receipt->second={ok,error};
    return ok;
}

bool SessionDestructibleInteractionV1::animation_finished(
    ActorId source, std::uint64_t generation, bool active, std::string& error) {
    error.clear();
    if (!services(source,error)) return false;
    SessionContainerActorBorrowV1 actor;
    if (!borrow(source,actor,error)) return false;
    const auto key=std::make_pair(source,generation);
    const auto previous=completion_receipts_.find(key);
    if (previous!=completion_receipts_.end()) {
        if (previous->second.success) return true;
        error=previous->second.error; return false;
    }
    auto receipt=completion_receipts_.emplace(key,Receipt{false,{}}).first;
    auto& fields=*actor.destructible_state;
    bool ok=true,accepted{};
    if (fields.state394==1) {
        fields.state394=2; ok=play(source,"idle",accepted,error);
    } else if (fields.state394==3) {
        fields.state394=4; ok=play(source,"idleactive",accepted,error);
    } else if (!active) ok=flags(source,0x200u,0u,error);
    std::string persist_error;
    if(!persist_container_state(actor,fields.state394,persist_error)){
        if(ok||error.empty())error=persist_error;
        ok=false;
    }
    receipt->second={ok,error};
    return ok;
}

} // namespace dh::foundation::interactions
