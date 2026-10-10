#include "session_source_object_admission_v1.hpp"
#include "world_object_container_state_v1.hpp"

#include <cmath>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
struct AdmissionOwnerV1 {
    const void* session{};
    std::shared_ptr<const void> lease;
    ActorId id{invalid_actor_id};
    std::string definition_name,data_desc;
    std::int32_t data_id{-1},network_id{-1},probability{100},roll{};
};
struct StagedFailureV1 {
    bool hidden{},deleted{},marked{};
    std::uint8_t byte82{};
};
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b)noexcept{
    return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
}

bool admit_session_source_object_v1(
    CombatSession& session,SessionSourceObjectAdmissionRequestV1 request,
    SessionSourceObjectAdmissionReceiptV1& output,std::string& error){
    output={};error.clear();
    auto* const initial_world=session.world();
    auto initial_lease=session.actor_binding_lease().lock();
    auto initial_lifetime=session.lifetime_lease().lock();
    if(!initial_world||!initial_lease||!initial_lifetime)
        return fail(error,"Source object admission requires a live same-session WorldObject owner");
    const auto same_current_binding=[&](){
        auto current=session.actor_binding_lease().lock();
        auto lifetime=session.lifetime_lease().lock();
        return session.world()==initial_world&&same_owner(initial_lease,current)&&
            lifetime&&lifetime.get()==initial_lifetime.get();
    };
    if(!request.definition||request.definition->stableId==invalid_actor_id||
       request.definition->name.empty()||request.definition->gametype.empty())
        return fail(error,"Source object admission requires an exact authored ActorDefinition");
    const auto source_id=request.definition->stableId;
    const std::string source_name=request.definition->name;
    const std::string source_gametype=request.definition->gametype;
    const auto desc_it=request.definition->properties.find("data_desc");
    const std::string source_data_desc=desc_it==request.definition->properties.end()?std::string{}:desc_it->second;
    if(source_gametype!="OpenableContainer")
        return fail(error,"This admission receipt is scoped to source OpenableContainer definitions");
    if(request.candidate.id!=source_id||request.candidate.name!=source_name)
        return fail(error,"Staged candidate identity differs from its authored source definition");
    if(request.candidate.id==invalid_object_id)
        return fail(error,"Source admission candidate has an invalid ObjectId");
    if(initial_world->find_object(request.candidate.id))
        return fail(error,"Source admission candidate is already published; stage before WorldObject enrollment");
    if(initial_world->find_actor(request.candidate.id))
        return fail(error,"Source admission ObjectId collides with an enrolled Character ActorId");
    if(request.probability274<0||request.probability274>100)
        return fail(error,"Source probability274 is outside the original [0,100] range");
    if(source_data_desc.empty())
        return fail(error,"Source admission requires the authored data_desc for its eventual Openable receipt");
    if(!validate_world_object(request.candidate,error))
        return fail(error,"Staged source candidate failed WorldObject validation");
    SourceContainerObjsFieldsV1 initial_state;
    if(!read_source_container_objs_v1(request.candidate,initial_state,error))
        return fail(error,"Staged Openable candidate requires its authored/constructor OBJS component");

    bool meet=false;
    if(!dh2::world::game_object_meet_condition_v1(meet,error)||!meet)
        return fail(error,"Original GameObject MeetCondition rejected the staged candidate");

    // The original constructor owns these defaults. They remain explicit in
    // the receipt so a caller cannot mistake a missing modern producer for a
    // source value. Other branches still demand the real online/handle leaves.
    std::int32_t cached270=-1;
    std::uint8_t byte82=0;
    std::int32_t probability=request.probability274;
    std::int32_t network_id=request.network_id108;
    std::int32_t owner_fc=request.online_owner_fc.value_or(0);
    bool classified_player=false,online=false,online_provider_evaluated=false;
    StagedFailureV1 staged;
    dh2::world::GameObjectSpawnProbabilityBorrowV1 borrow;
    borrow.owner=std::const_pointer_cast<void>(initial_lease);
    borrow.cached_roll270=&cached270;borrow.probability274=&probability;
    borrow.network_id108=&network_id;
    borrow.online_owner_fc=request.online_owner_fc?&owner_fc:nullptr;
    borrow.byte82=&byte82;
    // OpenableContainer derives through GameObject/ObjectBase, whose source
    // IsCharacter virtual (+0x24) is the literal false implementation. The
    // ObjectHandle Character conversion therefore yields null for a correctly
    // registered self-handle; if its lookup is null, conversion is also null.
    // CheckSpawnProbability consequently never calls Character::IsPlayer(+0x28).
    borrow.handle_as_player_character=[&](bool& player,std::string&){
        player=false;classified_player=false;return true;
    };
    borrow.online_byte5=[&](bool& value,std::string& e){
        if(!request.online_byte5){e="Required actual source online byte5 provider";return false;}
        if(!request.online_byte5(value,e))return false;
        online_provider_evaluated=true;
        if(!same_current_binding()){
            e="Session World/binding lease changed during source online-byte5 callback";return false;
        }
        online=value;return true;
    };
    borrow.set_visible_false=[&](std::string& e){
        staged.hidden=true;request.candidate.visual.visible=false;
        initial_state.visible80=0;
        return bind_source_container_objs_v1(request.candidate,initial_state,e);
    };
    borrow.object_base_delete=[&](std::string&){
        staged.deleted=true;byte82=2;return true;
    };
    borrow.mark_for_deletion=[&](std::string&){staged.marked=true;return true;};
    std::int32_t roll{},resolved_probability{};
    bool kernel_ran=false,kernel_ok=false;
    const auto run_kernel=[&](dh2::data::LootRandom8V2* channel0,
        dh2::data::LootRandom8V2* channel1,std::string& e){
        if(kernel_ran){e="Source spawn RNG loan attempted duplicate admission kernel execution";return false;}
        if(!same_current_binding()){
            e="Session World/binding lease changed before source probability kernel";return false;
        }
        borrow.random0=channel0;borrow.random1=channel1;
        kernel_ran=true;
        kernel_ok=dh2::world::game_object_check_spawn_probability_v1(
            borrow,roll,resolved_probability,e);
        if(!same_current_binding()){
            e="Session World/binding lease changed during source probability callbacks";return false;
        }
        return kernel_ok;
    };
    if(request.with_spawn_random){
        if(!same_owner(request.random_owner,initial_lease))
            return fail(error,"Source RNG loan requires this current Session's strong canonical owner lease");
        if(!request.with_spawn_random([&](dh2::data::LootRandom8V2& channel0,
            dh2::data::LootRandom8V2* channel1,std::string& e){
                return run_kernel(&channel0,channel1,e);
            },error))return false;
    }else if(!run_kernel(nullptr,nullptr,error))return false;
    if(!same_current_binding())
        return fail(error,"Session World/binding lease changed before source candidate publication");
    if(!kernel_ran||!kernel_ok)
        return fail(error,"Source spawn RNG provider returned without running the admission kernel");
    output.roll=roll;output.cached_roll270=cached270;output.probability=resolved_probability;
    output.data_id_ec=request.data_id_ec;output.network_id108=network_id;
    output.player_handle=classified_player;output.online=online;
    output.online_provider_evaluated=online_provider_evaluated;
    output.candidate_hidden=staged.hidden;output.candidate_deleted=staged.deleted;
    output.marked_for_deletion=staged.marked;
    // Fresh candidates have cache -1. Source CheckSpawnProbability writes -2
    // only for an accepted candidate; a rejected roll is deleted and queued.
    output.admitted=(cached270==-2);
    if(!output.admitted){
        if(!staged.hidden||!staged.deleted||!staged.marked||byte82!=0)
            return fail(error,"Source probability rejection did not complete hide/Delete/Mark staging effects");
        error.clear();return true;
    }

    auto lease=initial_lease;
    const auto id=request.candidate.id;
    const auto name=request.candidate.name;
    const auto model=request.candidate.visual.model;
    auto owner=std::make_shared<AdmissionOwnerV1>();
    owner->session=&session;owner->lease=lease;owner->id=id;
    owner->definition_name=name;owner->data_desc=source_data_desc;
    owner->data_id=request.data_id_ec;owner->network_id=network_id;
    owner->probability=resolved_probability;owner->roll=roll;
    SessionContainerPriorAdmissionV1 prior;
    prior.session_identity=&session;prior.session_lease=lease;
    prior.admission_owner=std::static_pointer_cast<const void>(owner);
    prior.object_id=id;prior.definition_name=name;prior.data_desc=source_data_desc;
    // Publication is deliberately after all source admission gates. Preserve
    // the caller's authored source visual reference; do not bind callbacks here.
    std::string bind_error;
    if(!same_current_binding())return fail(error,"Session World/binding lease changed at source publication boundary");
    if(initial_world->find_actor(id)||initial_world->find_object(id))
        return fail(error,"Source candidate ID became occupied before publication");
    if(!initial_world->bind_object(std::move(request.candidate),bind_error))
        return fail(error,bind_error.c_str());
    auto* published=initial_world->find_object(id);
    if(!published||published->id!=id||published->name!=name||published->visual.model!=model){
        initial_world->remove_object(id);
        output.admitted=false;
        return fail(error,"Published source candidate did not retain its exact staged identity/visual");
    }
    output.prior_admission=std::move(prior);
    error.clear();return true;
}

} // namespace dh::foundation::interactions
