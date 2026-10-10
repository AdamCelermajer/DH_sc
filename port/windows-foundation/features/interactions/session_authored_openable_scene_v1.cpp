#include "session_authored_openable_scene_v1.hpp"
#include "world_object_container_state_v1.hpp"

#include <algorithm>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b)noexcept{
    return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}

bool decode_names(const std::uint8_t* bytes,std::size_t size,
    std::size_t expected,std::vector<std::string>& names,std::string& error){
    names.clear();
    if(!bytes||size<4)return fail(error,"Original Openable row names source is missing or truncated");
    const auto u32=[&](std::size_t at){return std::uint32_t(bytes[at])|
        (std::uint32_t(bytes[at+1])<<8)|(std::uint32_t(bytes[at+2])<<16)|
        (std::uint32_t(bytes[at+3])<<24);};
    const auto count=u32(0);
    if(count!=expected)return fail(error,"Original Openable row-name count differs from loaded table");
    std::size_t at=4;names.reserve(count);
    for(std::uint32_t i=0;i<count;++i){
        if(size-at<4)return fail(error,"Original Openable row-name length is truncated");
        const auto length=u32(at);at+=4;
        if(length>size-at)return fail(error,"Original Openable row name exceeds source bytes");
        names.emplace_back(reinterpret_cast<const char*>(bytes+at),length);at+=length;
    }
    if(at!=size)return fail(error,"Original Openable row-name source has unexpected trailing bytes");
    error.clear();return true;
}

bool validate_policy(const ActorDefinition& definition,
    const dh2::world::OpenableContainerRowV1& row,
    const SessionContainerModernOpenablePolicyV1& policy,std::string& error){
    if(!policy.resolve_other_actor)
        return fail(error,"Authored Openable collection requires a typed same-session opener resolver");
    const auto& source=policy.source;
    if(!source.has_visual||!source.source_on_interact)
        return fail(error,"Authored Openable collection requires source visual and GameObject::Update providers");
    if(row.sound!=-1&&!source.play_sound_3d)
        return fail(error,"Authored Openable collection requires the source sound playback provider for this row");
    if(!source.has_script)
        return fail(error,"Authored Openable collection requires the source LuaScript presence provider");
    if(!row.script.empty()&&(!source.load_object_script||!source.script_call))
        return fail(error,"Authored scripted Openable row requires source script load and call providers");
    if(row.keep_physics&&(!source.apply_mesh_box||!source.create_attach_po_decor))
        return fail(error,"Authored KeepPhysics Openable row requires source mesh/physical-attachment providers");

    const auto& interaction=policy.interaction;
    if(!interaction.current_level||!interaction.assert_missing_level||
       !interaction.local_player_hosting||!interaction.constant||
       !interaction.room64||!interaction.raise_async)
        return fail(error,"Authored Openable collection requires typed current-Level and same-Level quest-event providers");
    if(!interaction.handle_as_character||!interaction.is_player||
       !interaction.props_add_int||!interaction.props_get_int||
       !interaction.is_local_player||!interaction.trophy_name_index||
       !interaction.unlock_trophy)
        return fail(error,"Authored Openable collection requires typed Character/property/trophy interaction providers");
    if(!definition.stableId||definition.stableId==invalid_actor_id)
        return fail(error,"Authored Openable provider policy received invalid ObjectId");
    error.clear();return true;
}

struct PreparedOpenable {
    const ActorDefinition* definition{};
    SessionContainerModernOpenablePolicyV1 policy;
};
}

SessionAuthoredOpenableSceneV1::SessionAuthoredOpenableSceneV1(
    CombatSession& session,std::shared_ptr<const void> session_lease,
    std::shared_ptr<const void> session_lifetime_lease,
    std::shared_ptr<const std::vector<ActorDefinition>> definitions,
    std::shared_ptr<SessionContainerRetainedVisualV1> visual,
    std::shared_ptr<SessionContainerModernDropV1> drop,
    std::shared_ptr<const dh2::world::OpenableContainerTableV1> table,
    std::shared_ptr<const void> provider_owner)
    :session_(&session),session_lease_(std::move(session_lease)),
     session_lifetime_lease_(std::move(session_lifetime_lease)),
     definitions_(std::move(definitions)),visual_(std::move(visual)),
     drop_(std::move(drop)),table_(std::move(table)),
     provider_owner_(std::move(provider_owner)){}

SessionAuthoredOpenableSceneV1::~SessionAuthoredOpenableSceneV1(){
    std::string ignored;(void)release_all(ignored);
}

bool SessionAuthoredOpenableSceneV1::current(std::string& error)const{
    if(!session_||!definitions_||!visual_||!drop_||!table_||!provider_owner_||
       !session_lease_||!session_lifetime_lease_)
        return fail(error,"Authored Openable scene collection lost a retained owner");
    if(!same_owner(session_lease_,session_->actor_binding_lease().lock()))
        return fail(error,"Authored Openable scene collection belongs to a stale Session binding");
    const auto life=session_->lifetime_lease().lock();
    if(!life||life.get()!=session_lifetime_lease_.get())
        return fail(error,"Authored Openable scene collection belongs to a stale Session lifetime");
    if(visual_->session_identity()!=session_||
       !same_owner(session_lease_,visual_->session_lease()))
        return fail(error,"Authored Openable scene collection lost the same-session retained visual owner");
    error.clear();return true;
}

bool SessionAuthoredOpenableSceneV1::bind(CombatSession& session,
    const std::vector<ActorDefinition>& source_scene,
    const std::vector<SessionSourceObjectAdmissionReceiptV1>& admitted,
    const AssetCatalog& assets,
    std::shared_ptr<SessionContainerRetainedVisualV1> visual,
    std::shared_ptr<SessionContainerModernDropV1> drop,
    std::shared_ptr<const dh2::world::OpenableContainerTableV1> table,
    SessionAuthoredOpenableSceneProvidersV1 providers,
    std::shared_ptr<SessionAuthoredOpenableSceneV1>& output,
    std::string& error){
    return bind_session_authored_openable_scene_v1(session,source_scene,admitted,
        assets,std::move(visual),std::move(drop),std::move(table),
        std::move(providers),output,error);
}

bool bind_session_authored_openable_scene_v1(CombatSession& session,
    const std::vector<ActorDefinition>& source_scene,
    const std::vector<SessionSourceObjectAdmissionReceiptV1>& admitted,
    const AssetCatalog& assets,
    std::shared_ptr<SessionContainerRetainedVisualV1> visual,
    std::shared_ptr<SessionContainerModernDropV1> drop,
    std::shared_ptr<const dh2::world::OpenableContainerTableV1> table,
    SessionAuthoredOpenableSceneProvidersV1 providers,
    std::shared_ptr<SessionAuthoredOpenableSceneV1>& output,
    std::string& error){
    output.reset();error.clear();
    auto* const world=session.world();
    const auto lease=session.actor_binding_lease().lock();
    const auto lifetime=session.lifetime_lease().lock();
    if(!world||!lease||!lifetime||!visual||!drop||!table||!providers.owner||
       !providers.make_policy||!providers.validate_visual_row||
       !providers.source_row_names||providers.source_row_names_size<4)
        return fail(error,"Authored Openable scene binding requires current Session, table, retained visual/drop owners and source policy/visual providers");
    if(visual->session_identity()!=&session||
       !same_owner(lease,visual->session_lease()))
        return fail(error,"Authored Openable scene visual owner is not bound to this current Session");

    auto scene=std::make_shared<const std::vector<ActorDefinition>>(source_scene);
    std::vector<std::string> source_row_names;
    if(!decode_names(providers.source_row_names,providers.source_row_names_size,
        table->size(),source_row_names,error))return false;
    std::map<ObjectId,const ActorDefinition*> openables;
    for(const auto& definition:*scene){
        if(definition.gametype!="OpenableContainer")continue;
        if(definition.stableId==invalid_actor_id||definition.name.empty())
            return fail(error,"Source scene contains an Openable declaration without exact stable identity");
        const auto desc=definition.properties.find("data_desc");
        if(desc==definition.properties.end()||desc->second.empty())
            return fail(error,"Source scene Openable declaration has no authored data_desc");
        if(!openables.emplace(definition.stableId,&definition).second)
            return fail(error,"Authored scene has an ambiguous duplicate Openable ObjectId");
    }

    std::map<ObjectId,const SessionSourceObjectAdmissionReceiptV1*> receipts;
    for(const auto& receipt:admitted){
        if(!receipt.admitted||!receipt.online_provider_evaluated||
           !receipt.prior_admission.admission_owner)
            return fail(error,"Authored Openable scene requires a successful source admission receipt with online-byte5 provenance");
        if(!receipts.emplace(receipt.prior_admission.object_id,&receipt).second)
            return fail(error,"Authored Openable scene has duplicate prior-admission receipts for one ObjectId");
        const auto found=openables.find(receipt.prior_admission.object_id);
        if(found==openables.end())
            return fail(error,"Openable admission receipt does not map to exactly one source scene declaration");
        const auto& definition=*found->second;
        const auto desc=definition.properties.find("data_desc");
        if(receipt.prior_admission.session_identity!=&session||
           !same_owner(receipt.prior_admission.session_lease,lease)||
           receipt.prior_admission.definition_name!=definition.name||
           receipt.prior_admission.data_desc!=desc->second)
            return fail(error,"Openable admission receipt is not bound to this exact Session/ObjectId/definition/data_desc");
        auto* object=world->find_object(definition.stableId);
        if(!object||object->id!=definition.stableId||object->name!=definition.name)
            return fail(error,"Admitted Openable receipt has no exact current same-world WorldObject");
    }

    for(const auto& pair:openables){
        if(world->find_object(pair.first)&&receipts.find(pair.first)==receipts.end())
            return fail(error,"Current source Openable WorldObject has no caller-proven prior-admission receipt");
    }

    auto collection=std::shared_ptr<SessionAuthoredOpenableSceneV1>(
        new SessionAuthoredOpenableSceneV1(session,lease,lifetime,scene,visual,
            drop,table,providers.owner));
    std::vector<PreparedOpenable> prepared;
    prepared.reserve(receipts.size());
    for(const auto& receipt_pair:receipts){
        const auto definition_it=openables.find(receipt_pair.first);
        if(definition_it==openables.end())return fail(error,"Admitted Openable declaration disappeared during binding");
        const auto& definition=*definition_it->second;
        const auto& receipt=*receipt_pair.second;
        const auto desc_it=definition.properties.find("data_desc");
        std::int32_t row_id=-1;dh2::world::OpenableContainerRowV1 row;
        if(!table->resolve(desc_it->second,row_id,row,error))return false;
        if(row_id<0)return fail(error,"Admitted authored Openable data_desc has no source row");
        if(static_cast<std::size_t>(row_id)>=source_row_names.size()||
           source_row_names[static_cast<std::size_t>(row_id)]!=desc_it->second||
           std::count(source_row_names.begin(),source_row_names.end(),desc_it->second)!=1)
            return fail(error,"Authored Openable data_desc is ambiguous in the original source row-name table");
        auto* object=world->find_object(definition.stableId);
        if(!object||!providers.validate_visual_row(providers.context,definition,row,*object,error))
            return fail(error,"Openable row visual does not match the exact current source WorldObject asset");
        SessionContainerModernOpenablePolicyV1 policy;
        if(!providers.make_policy(providers.context,definition,row,policy,error))return false;
        policy.authored_table=table;
        policy.prior_admission=receipt.prior_admission;
        policy.source_fields.data_desc=desc_it->second;
        if(!validate_policy(definition,row,policy,error))return false;
        if(!collection->current(error))return false;
        prepared.push_back(PreparedOpenable{&definition,std::move(policy)});
    }

    // Resolve and validate every source callback before binding any visual or
    // interaction callback. A bad later row must not leave earlier objects
    // partially enrolled.
    for(auto& candidate:prepared){
        const auto& definition=*candidate.definition;
        const bool had_visual=session.retained_object_visual_borrow(definition.stableId)!=nullptr;
        std::shared_ptr<SessionContainerModernOpenableV1> owner;
        if(!SessionContainerModernOpenableV1::create(session,definition,assets,
            *visual,drop,std::move(candidate.policy),owner,error)){
            if(!had_visual)session.unbind_object_visual(definition.stableId);
            return false;
        }
        if(!owner||!owner->initialize_admitted(error)){
            if(!had_visual)session.unbind_object_visual(definition.stableId);
            return false;
        }
        if(!collection->current(error)){
            if(!had_visual)session.unbind_object_visual(definition.stableId);
            return false;
        }
        collection->entries_.emplace(definition.stableId,
            SessionAuthoredOpenableSceneV1::Entry{std::move(owner),definition.name});
    }
    output=std::move(collection);error.clear();return true;
}

bool SessionAuthoredOpenableSceneV1::find(ObjectId id,
    std::shared_ptr<SessionContainerModernOpenableV1>& output,
    std::string& error)const{
    output.reset();if(!current(error))return false;
    const auto found=entries_.find(id);
    if(found==entries_.end())return fail(error,"ObjectId is not retained by this authored Openable scene collection");
    output=found->second.owner;error.clear();return true;
}

bool SessionAuthoredOpenableSceneV1::interact(ObjectId id,ActorId opener,
    std::string& error){
    std::shared_ptr<SessionContainerModernOpenableV1> owner;
    if(!find(id,owner,error))return false;
    return owner->interact(opener,error);
}
bool SessionAuthoredOpenableSceneV1::animation_event(ObjectId id,
    const RetainedAnimationEvent& event,std::string& error){
    std::shared_ptr<SessionContainerModernOpenableV1> owner;
    if(!find(id,owner,error))return false;
    return owner->animation_event(event,error);
}
bool SessionAuthoredOpenableSceneV1::animation_finished(ObjectId id,
    std::uint64_t generation,bool active,std::string& error){
    std::shared_ptr<SessionContainerModernOpenableV1> owner;
    if(!find(id,owner,error))return false;
    return owner->animation_finished(generation,active,error);
}

bool SessionAuthoredOpenableSceneV1::release(ObjectId id,std::string& error){
    const auto found=entries_.find(id);
    if(found==entries_.end())return fail(error,"Openable collection has no entry for requested ObjectId");
    if(session_&&same_owner(session_lease_,session_->actor_binding_lease().lock())){
        auto* object=session_->world()?session_->world()->find_object(id):nullptr;
        if(object&&object->name==found->second.definition_name)
            session_->unbind_object_visual(id);
    }
    entries_.erase(found);error.clear();return true;
}

bool SessionAuthoredOpenableSceneV1::release_all(std::string& error){
    for(auto it=entries_.begin();it!=entries_.end();){
        const auto id=it->first;
        if(session_&&same_owner(session_lease_,session_->actor_binding_lease().lock())){
            auto* object=session_->world()?session_->world()->find_object(id):nullptr;
            if(object&&object->name==it->second.definition_name)
                session_->unbind_object_visual(id);
        }
        it=entries_.erase(it);
    }
    error.clear();return true;
}

std::vector<ObjectId> SessionAuthoredOpenableSceneV1::object_ids()const{
    std::vector<ObjectId> ids;ids.reserve(entries_.size());
    for(const auto& entry:entries_)ids.push_back(entry.first);
    return ids;
}

} // namespace dh::foundation::interactions
