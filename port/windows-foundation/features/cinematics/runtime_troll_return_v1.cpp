#include "runtime_troll_return_v1.hpp"
#include "../../content_paths.hpp"
#include "../../retained_pose_playback.hpp"
#include <algorithm>
#include <cstring>
#include <set>
#include <stdexcept>

namespace dh::foundation { namespace {
bool scalar_i32(const OriginalCampaignCommand& c,unsigned offset,std::int32_t& out){
    const auto found=c.scalars.find(offset);if(found==c.scalars.end())return false;
    std::memcpy(&out,&found->second,sizeof(out));return true;
}
bool string_at(const OriginalCampaignCommand& c,unsigned offset,const std::string*& out){
    const auto found=c.strings.find(offset);if(found==c.strings.end())return false;
    out=&found->second;return true;
}
std::string attr(const OriginalCampaignTrigger& t,const char* key){
    const auto found=t.attributes.find(key);return found==t.attributes.end()?std::string{}:found->second;
}
bool fail(std::string& e,const char* s){e=s;return false;}
}

bool TrollReturnSourceBankV1::matches_command(const OriginalCampaignCommand& c,std::size_t* index) const noexcept{
    if(!campaign||script_id<0||static_cast<std::size_t>(script_id)>=campaign->scripts().size())return false;
    const auto& commands=campaign->scripts()[static_cast<std::size_t>(script_id)].commands;
    for(const auto& need:clip_needs)if(need.command_index<commands.size()&&&commands[need.command_index]==&c){if(index)*index=need.command_index;return true;}
    return false;
}

bool TrollReturnSourceBankV1::selection_for(const OriginalCampaignCommand& c,
    OriginalAttackSelection& selection,TrollReturnClipNeedV1& need,std::string& error) const{
    std::size_t index=0;if(!matches_command(c,&index)){error="Command is not one of this loaded TrollReturn script's authored animation records";return false;}
    const auto found=std::find_if(clip_needs.begin(),clip_needs.end(),[&](const auto& item){return item.command_index==index;});
    if(found==clip_needs.end()){error="TrollReturn source clip leaf is absent";return false;}
    selection={};selection.state="Attack";selection.variant=0;selection.group_path=found->source_path;selection.actor_rate=1.0;
    need=*found;error.clear();return true;
}

bool build_troll_return_source_bank_v1(const AssetCatalog& assets,
    const OriginalMeleeBindings& bindings,const OriginalCampaignRuntime& campaign,
    const CharacterVisualConfig& sameVisual,const std::string& role,
    TrollReturnSourceBankV1& output,std::string& error){
    try{
        if(role.empty())throw std::runtime_error("TrollReturn clip bank needs a caller role ID");
        TrollReturnSourceBankV1 next;next.campaign=&campaign;
        next.script_id=campaign.script_id("TrollReturn",false);
        if(next.script_id<0)throw std::runtime_error("Loaded original campaign has no level TrollReturn script");
        const OriginalCampaignTrigger* trigger=nullptr;
        for(const auto& row:campaign.triggers())if(attr(row.second,"script")=="TrollReturn"){
            if(attr(row.second,"name")!="_prim_TriggerZone_001")continue;
            if(trigger)throw std::runtime_error("TrollReturn trigger declaration is ambiguous");
            trigger=&row.second;next.trigger_key=row.first;
        }
        if(!trigger||attr(*trigger,"gametype")!="TriggerZone"||attr(*trigger,"triggercount")!="1"||
           attr(*trigger,"triggerdelay")!="0"||!attr(*trigger,"script_move_out").empty())
            throw std::runtime_error("Expected authored one-shot TrollReturn TriggerZone declaration");
        const auto& script=campaign.scripts().at(static_cast<std::size_t>(next.script_id));
        if(script.name!="TrollReturn"||script.scope!="level")throw std::runtime_error("TrollReturn script identity/scope changed");
        const auto* actor=bindings.find_actor("Troll");
        if(!actor)throw std::runtime_error("Original Troll melee profile binding is unavailable");
        const auto state=actor->states.find("Attack");
        if(state==actor->states.end()||state->second.empty())throw std::runtime_error("Original Troll Attack binding is unavailable");
        const auto& sequence=state->second.front();
        if(sequence.name!="Troll_Attack1H"||sequence.loop!=0)throw std::runtime_error("Unexpected Troll Attack source sequence");
        if(!original_sequence_policies(bindings,next.policies,error))throw std::runtime_error(error);
        next.plan.profileId="Troll";next.plan.roleId=role;next.plan.config=sameVisual;
        OriginalCombatSequencePlan seq;seq.state="Attack";seq.variant=0;seq.id=sequence.id;
        seq.name=sequence.name;seq.loop=sequence.loop;seq.type=sequence.type;seq.properties=sequence.properties;
        next.plan.stateNames.push_back("Attack");

        struct AuthoredLeaf { OriginalCombatPhase phase; std::string uri; };
        std::vector<AuthoredLeaf> allLeaves;
        std::function<void(const OriginalMeleeStep&,std::vector<std::size_t>,std::vector<std::int64_t>,std::vector<OriginalCombatRedirect>)> visit;
        visit=[&](const OriginalMeleeStep& step,std::vector<std::size_t> path,
                  std::vector<std::int64_t> indices,std::vector<OriginalCombatRedirect> ancestors){
            indices.push_back(step.index);
            if(!step.children.empty()){
                ancestors.push_back({step.index,step.animationId,step.redirect,step.speed,step.blendOut,step.moveGO,step.properties});
                for(std::size_t child=0;child<step.children.size();++child){auto p=path;p.push_back(child);visit(step.children[child],std::move(p),indices,ancestors);}
                return;
            }
            OriginalCombatPhase phase;phase.sourcePath=std::move(path);phase.sourceIndices=std::move(indices);
            phase.ancestors=std::move(ancestors);phase.animationId=step.animationId;phase.redirect=step.redirect;
            phase.blendOut=step.blendOut;phase.moveGO=step.moveGO;phase.speed=step.speed;
            phase.sourceUri=step.uri;phase.properties=step.properties;allLeaves.push_back({std::move(phase),step.uri});
        };
        for(std::size_t root=0;root<sequence.steps.size();++root)visit(sequence.steps[root],{root},{},{});

        struct RequestedClip {std::size_t command_index=0;bool wait=false;};
        std::map<std::int32_t,RequestedClip> requested;std::vector<std::int32_t> requestedOrder;
        for(std::size_t commandIndex=0;commandIndex<script.commands.size();++commandIndex){
            const auto& command=script.commands[commandIndex];if(command.kind!=45)continue;
            std::int32_t animation=0,wait=0;const std::string* actorName=nullptr;
            if(!scalar_i32(command,8,animation)||!scalar_i32(command,28,wait)||!string_at(command,24,actorName))
                throw std::runtime_error("TrollReturn PlayActorAnim record lacks source animation/wait/receiver fields");
            if(animation<0||(wait!=0&&wait!=1)||!requested.emplace(animation,RequestedClip{commandIndex,wait!=0}).second)
                throw std::runtime_error("TrollReturn animation records are duplicated or malformed");
            requestedOrder.push_back(animation);
            if(next.actor_name.empty())next.actor_name=*actorName;
            if(*actorName!=next.actor_name)throw std::runtime_error("TrollReturn PlayActorAnim receiver changes within authored chain");
        }
        if(next.actor_name!="_prim_Monster_53_03_001"||requestedOrder!=std::vector<std::int32_t>{1377,1359,1373})
            throw std::runtime_error("TrollReturn source actor animation command chain differs from authored V19 contract");
        for(const auto& leaf:allLeaves){
            OriginalCombatPhase phase=leaf.phase;
            phase.resolvedPath=leaf.uri.empty()?std::string{}:resolve_content_path(assets,normalize_content_uri(leaf.uri),actor->model)
                .lexically_relative(assets.root()).generic_string();
            phase.clipName=role+"/Troll_Attack1H/animation-"+std::to_string(phase.animationId);
            seq.phases.push_back(phase);next.plan.clipRates.emplace(phase.clipName,phase.speed);
            const auto command=requested.find(static_cast<std::int32_t>(phase.animationId));if(command==requested.end())continue;
            if(leaf.uri.empty()||phase.redirect!=0||phase.sourcePath.size()!=2)
                throw std::runtime_error("TrollReturn animation ID does not resolve to one visual Troll Attack leaf");
            if(assets.read(leaf.uri).empty())throw std::runtime_error("TrollReturn animation clip resource is empty: "+leaf.uri);
            const auto* sourceClip=bindings.find_clip(leaf.uri);
            if(!sourceClip)throw std::runtime_error("TrollReturn animation binding has no original clip metadata: "+leaf.uri);
            auto existing=std::find_if(next.plan.config.clips.begin(),next.plan.config.clips.end(),[&](const auto& clip){return clip.first==phase.clipName;});
            if(existing!=next.plan.config.clips.end()){
                if(existing->second!=phase.resolvedPath)throw std::runtime_error("TrollReturn source clip alias conflicts with same actor visual");
            }else{
                const auto pair=std::make_pair(phase.clipName,phase.resolvedPath);
                next.plan.config.clips.push_back(pair);next.additional_clips.push_back(pair);
            }
            requested.erase(command);
        }
        if(!requested.empty())throw std::runtime_error("TrollReturn authored animation ID is absent from Troll Attack hierarchy");
        for(const auto animation:requestedOrder){
            const auto commandIt=std::find_if(script.commands.begin(),script.commands.end(),[&](const auto& c){
                std::int32_t id=0;return c.kind==45&&scalar_i32(c,8,id)&&id==animation;
            });
            if(commandIt==script.commands.end())throw std::runtime_error("TrollReturn command record disappeared during source compilation");
            const auto commandIndex=static_cast<std::size_t>(commandIt-script.commands.begin());
            const auto found=std::find_if(seq.phases.begin(),seq.phases.end(),[&](const auto& p){return p.animationId==animation;});
            if(found==seq.phases.end())throw std::runtime_error("TrollReturn compiled sequence leaf is absent");
            std::int32_t wait=0;if(!scalar_i32(*commandIt,28,wait))throw std::runtime_error("TrollReturn source wait field disappeared");
            next.clip_needs.push_back({animation,commandIndex,wait!=0,found->sourcePath,found->sourceUri,found->resolvedPath,found->clipName});
        }
        if(next.actor_name!="_prim_Monster_53_03_001"||next.clip_needs.size()!=3||
           next.clip_needs[0].animation_id!=1377||next.clip_needs[1].animation_id!=1359||next.clip_needs[2].animation_id!=1373||
           !next.clip_needs[0].wait||next.clip_needs[1].wait||!next.clip_needs[2].wait)
            throw std::runtime_error("TrollReturn source actor animation command chain differs from authored V19 contract");
        const auto policy=next.policies.find(706);
        if(policy==next.policies.end()||policy->second.type!=1||policy->second.loop!=0)
            throw std::runtime_error("TrollReturn attack redirect 706 must remain source type1 finite");
        next.plan.sequences.push_back(std::move(seq));
        output=std::move(next);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}

TrollReturnRuntimeV1::TrollReturnRuntimeV1(OriginalCampaignRuntime& campaign,CombatSession& session,
    const TrollReturnSourceBankV1& bank,TrollReturnRuntimeServicesV1 services)
    :campaign_(campaign),session_(session),bank_(bank),services_(std::move(services)),
     session_lease_(session.actor_binding_lease()){
    if(bank_.campaign!=&campaign_||bank_.script_id<0||
       static_cast<std::size_t>(bank_.script_id)>=campaign_.scripts().size()||
       campaign_.scripts()[static_cast<std::size_t>(bank_.script_id)].name!="TrollReturn")
        construction_error_="TrollReturn bank is not bound to this loaded OriginalCampaignRuntime";
    else if(bank_.clip_needs.size()!=3||bank_.actor_name!="_prim_Monster_53_03_001")
        construction_error_="TrollReturn authored clip bank is incomplete";
    else if(session_lease_.expired())construction_error_="TrollReturn requires an initialized live CombatSession";
}

bool TrollReturnRuntimeV1::validate_session(std::string& error) const{
    const auto initial=session_lease_.lock();const auto current=session_.actor_binding_lease().lock();
    if(!initial||!current||initial!=current){error="TrollReturn CombatSession lease expired or changed";return false;}
    error.clear();return true;
}
bool TrollReturnRuntimeV1::validate_active(const ActiveCommand& active,std::string& error) const{
    if(active.skipped){error.clear();return true;}
    const auto expected=active.session_lease.lock();const auto current=session_.actor_binding_lease().lock();
    if(!expected||!current||expected!=current){error="TrollReturn active source animation lost its CombatSession lease";return false;}
    if(active.actor_id==invalid_actor_id||active.source_object_name!=bank_.actor_name||active.module<0||
       !active.actor_binding_lease||active.actor_binding_lease!=expected||!active.pose_owner||
       !session_.actor(active.actor_id)||session_.retained_actor_pose(active.actor_id)!=active.pose_owner){
        error="TrollReturn active source name/module/ActorId binding or retained pose owner changed";return false;
    }
    error.clear();return true;
}

bool TrollReturnRuntimeV1::command(CampaignCommandPhase phase,const OriginalCampaignCommand& command,
    int module,bool skip,bool& handled,bool& blocking,std::string& error){
    handled=false;blocking=false;error.clear();
    if(!construction_error_.empty()){error=construction_error_;return false;}
    if(!bank_.matches_command(command)){
        if(!services_.fallback_command)return fail(error,"TrollReturn existing cinematic command fallback is unavailable");
        return services_.fallback_command(phase,command,module,skip,handled,blocking,error);
    }
    handled=true;
    if(!validate_session(error))return false;
    OriginalAttackSelection selection;TrollReturnClipNeedV1 need;
    if(!bank_.selection_for(command,selection,need,error))return false;
    if(phase==CampaignCommandPhase::update)return true; // CombatSession::update is the one source frame clock.
    if(phase==CampaignCommandPhase::execute){
        ActiveCommand active;
        if(skip){active.skipped=true;active.session_lease=session_lease_;active_[&command]=std::move(active);return true;}
        const std::string* authoredActor=nullptr;
        if(!string_at(command,24,authoredActor)||*authoredActor!=bank_.actor_name)
            return fail(error,"TrollReturn animation receiver differs from the loaded source record");
        if(!services_.borrow_actor)return fail(error,"TrollReturn same-CombatSession authored object to ActorId resolver is unavailable");
        TrollReturnActorBorrowV1 borrowed;
        if(!services_.borrow_actor(*authoredActor,module,session_,borrowed,error))return false;
        const auto currentSessionLease=session_.actor_binding_lease().lock();
        if(borrowed.session!=&session_||borrowed.source_object_name!=*authoredActor||borrowed.module!=module||
           borrowed.actor_id==invalid_actor_id||!borrowed.actor_binding_lease||!currentSessionLease||
           borrowed.actor_binding_lease!=currentSessionLease)
            return fail(error,"TrollReturn resolver did not map the exact source name/module to an ActorId retained by this CombatSession");
        const auto* pose=session_.retained_actor_pose(borrowed.actor_id);
        if(!session_.actor(borrowed.actor_id)||!pose)return fail(error,"TrollReturn source actor has no retained CombatSession pose owner");
        if(!services_.animation_event)return fail(error,"TrollReturn original retained animation-event provider is unavailable");
        CombatSessionStateAnimationServices callbacks;
        const auto animationEvent=services_.animation_event;auto* const session=&session_;
        const auto sessionLease=session_lease_;
        callbacks.event=[animationEvent,session,actor=borrowed.actor_id,sessionLease](ActorId eventActor,const RetainedAnimationEvent& event,std::string& e){
            const auto expected=sessionLease.lock();const auto current=session->actor_binding_lease().lock();
            if(!expected||!current||expected!=current||eventActor!=actor){e="TrollReturn animation event lost its same actor/session owner";return false;}
            return animationEvent(*session,actor,event,e);
        };
        callbacks.finished=[session=&session_,lease=session_lease_,actor=borrowed.actor_id,pose](ActorId finishedActor,std::string& e){
            const auto expected=lease.lock();const auto current=session->actor_binding_lease().lock();
            if(!expected||!current||expected!=current||finishedActor!=actor||!session->actor(actor)||
               session->retained_actor_pose(actor)!=pose){e="TrollReturn source sequence completion lost the same actor/pose/session owner";return false;}
            e.clear();return true;
        };
        if(!session_.play_actor_source_sequence(borrowed.actor_id,bank_.plan,bank_.policies,selection,std::move(callbacks),error)){
            error="TrollReturn retained CombatSession source sequence: "+error;return false;
        }
        active.actor_id=borrowed.actor_id;active.source_object_name=borrowed.source_object_name;active.module=borrowed.module;
        active.actor_binding_lease=std::move(borrowed.actor_binding_lease);active.session_lease=session_lease_;active.pose_owner=pose;
        active_[&command]=std::move(active);return true;
    }
    std::int32_t wait=0;if(!scalar_i32(command,28,wait)||(wait!=0&&wait!=1))return fail(error,"TrollReturn source command wait flag is missing or invalid");
    const auto found=active_.find(&command);
    if(found==active_.end())return fail(error,"TrollReturn animation wait queried before its exact source command executed");
    if(!found->second.skipped&&found->second.module!=module)
        return fail(error,"TrollReturn animation wait changed the authored actor module context");
    if(!validate_active(found->second,error))return false;
    if(wait&&!found->second.skipped){
        const auto* pose=session_.retained_actor_pose(found->second.actor_id);
        blocking=!pose->current_ended();
    }
    return true;
}

bool TrollReturnRuntimeV1::trigger_contact(bool inside,bool qualified,int module,std::string& error){
    if(!construction_error_.empty()){error=construction_error_;return false;}
    if(!validate_session(error))return false;
    if(bank_.trigger_key.empty())return fail(error,"TrollReturn source trigger key is unavailable");
    return campaign_.trigger_contact(bank_.trigger_key,inside,qualified,module,error);
}
} // namespace dh::foundation
