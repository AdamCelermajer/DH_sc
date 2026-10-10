#include "runtime_troll_return_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../retained_pose_playback.hpp"
#include "../../../game-data/properties.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
void run(const std::filesystem::path& repo){
    const auto assetRoot=repo/".local-inputs/windows-source-clock-checkpoint-v17/assets";
    AssetCatalog campaignAssets(assetRoot),assets(assetRoot/"original-cache");std::string error;
    OriginalCampaignRuntime campaign;check(campaign.load(campaignAssets,"original-campaign.xml",error),error);
    OriginalMeleeBindings bindings;check(bindings.load(campaignAssets,"original-melee-bindings.xml",error),error);
    auto triggerRuntime=std::make_shared<int>(1); // Actual trigger/contact and command records load above.
    (void)triggerRuntime;

    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan trollVisual;
    check(build_original_combat_visual_plan(assets,bindings,"Troll",customization,"troll-source-profile",trollVisual,error),error);
    TrollReturnSourceBankV1 bank;
    check(build_troll_return_source_bank_v1(assets,bindings,campaign,trollVisual.config,"troll-return-v1",bank,error),error);
    check(bank.script_id==campaign.script_id("TrollReturn",false)&&bank.actor_name=="_prim_Monster_53_03_001","TrollReturn same loaded source script/actor");
    check(bank.trigger_key=="port\\windows-foundation\\assets\\original-cache\\data\\3d\\modules\\swamp\\mgp\\obj_1of4_brdwalk_nse_00.mgp::_prim_TriggerZone_001","Exact source trigger key");
    check(bank.clip_needs.size()==3&&bank.additional_clips.size()==3,"Only authored TrollReturn source leaves are preloaded");
    const std::int32_t ids[]={1377,1359,1373};const std::vector<std::vector<std::size_t>> paths={{2,0},{2,1},{2,2}};const bool waits[]={true,false,true};
    const auto& script=campaign.scripts().at(static_cast<std::size_t>(bank.script_id));
    for(std::size_t i=0;i<3;++i){
        check(bank.clip_needs[i].animation_id==ids[i]&&bank.clip_needs[i].source_path==paths[i]&&bank.clip_needs[i].wait==waits[i],"Original TrollReturn animation ID/path/wait mapping");
        check(assets.read(bank.clip_needs[i].uri).size()>0,"Authored source clip present before CombatSession initialization");
        check(bank.matches_command(script.commands[bank.clip_needs[i].command_index]),"Same OriginalCampaignCommand identity retained");
        OriginalAttackSelection selected;TrollReturnClipNeedV1 need;
        check(bank.selection_for(script.commands[bank.clip_needs[i].command_index],selected,need,error),error);
        check(selected.state=="Attack"&&selected.variant==0&&selected.group_path==paths[i]&&selected.actor_rate==1.0,"Source hierarchy selection is exact; no fabricated timeline");
    }
    check(bank.policies.at(706).type==1&&bank.policies.at(706).loop==0,"Original finite Troll sequence redirect policy");

    OriginalPropertyDatabase database;check(load_original_property_tables(assets,"data/pydata",database,error),error);
    auto visualConfig=trollVisual.config;visualConfig.motion_node_id="auto";visualConfig.consume_root_motion=true;
    CombatSessionConfig config;config.diagnosticRngSeed=37;config.playerId=1;config.playerProfileId="Troll";
    config.tableRoot="data/pydata";config.playerVisualConfig=visualConfig;
    CombatSessionProfile profile;profile.initialIdle={"Idle",0,{0}};
    profile.sequenceAction=OriginalAttackSelection{};profile.sequenceAction->state="Attack";
    profile.sequenceAction->group_path={2,1};profile.retainedPhaseClock=true;profile.customization=customization;
    profile.damageMarkerNames={"attack_mainhand"};
    profile.propertyOptions={std::nullopt,true};profile.motionRoot="auto";
    profile.sourceAnimationClips=bank.additional_clips;config.profiles.emplace("Troll",profile);
    ActorPopulation population;CharacterVisual player;CombatSession session;
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),"session init: "+error);
    const ActorId trollActorId=session.player_id();
    const auto* retained=session.retained_actor_pose(trollActorId);check(retained,"Same Troll CombatSession retained pose exists");
    unsigned markers=0,admissions=0,motions=0;
    session.set_motion_handler([&](ActorState& actor,Vec3 motion,bool,std::string& e){
        if(actor.id!=trollActorId||!std::isfinite(motion.x)||!std::isfinite(motion.y)||!std::isfinite(motion.z)){e="invalid same-owner source motion";return false;}
        ++motions;e.clear();return true;
    });
    TrollReturnRuntimeServicesV1 runtimeServices;
    unsigned cameraFallbacks=0;
    runtimeServices.fallback_command=[&](CampaignCommandPhase phase,const OriginalCampaignCommand& c,int module,bool,
        bool& wasHandled,bool& wait,std::string& e){
        if(module!=7||c.kind!=5||c.scalars.at(8)!=44||phase!=CampaignCommandPhase::execute){e="unexpected non-TrollReturn fallback record";return false;}
        ++cameraFallbacks;wasHandled=true;wait=false;e.clear();return true;
    };
    runtimeServices.borrow_actor=[&](const std::string& name,int module,CombatSession& current,TrollReturnActorBorrowV1& out,std::string& e){
        if(name!="_prim_Monster_53_03_001"||module!=7||&current!=&session){e="wrong source actor/module/session";return false;}
        out.session=&current;out.source_object_name=name;out.module=module;out.actor_id=current.player_id();
        out.actor_binding_lease=current.actor_binding_lease().lock();e.clear();return static_cast<bool>(out.actor_binding_lease);
    };
    runtimeServices.animation_event=[&](CombatSession& current,ActorId actor,const RetainedAnimationEvent&,std::string& e){
        if(&current!=&session||actor!=trollActorId){e="foreign same-session marker";return false;}++markers;e.clear();return true;
    };
    TrollReturnRuntimeV1 runtime(campaign,session,bank,runtimeServices);
    OriginalCampaignServices campaignServices;campaignServices.admit_start=[&](int id,int module,bool received,bool& admit,std::string& e){
        if(id!=bank.script_id||module!=7||received){e="TrollReturn trigger context changed";return false;}++admissions;admit=true;e.clear();return true;};
    campaignServices.command=[&](CampaignCommandPhase phase,const OriginalCampaignCommand& c,int module,bool& block,std::string& e){
        bool handled=false;return runtime.command(phase,c,module,false,handled,block,e)&&handled;};
    campaign.bind(std::move(campaignServices));
    check(runtime.trigger_contact(true,true,7,error),error);
    check(campaign.running(bank.script_id)&&admissions==1,"Real authored one-shot contact starts the same campaign session once");
    check(runtime.trigger_contact(true,true,7,error)&&admissions==1,"Same inside state cannot re-trigger one-shot script");

    auto commandAt=[&](std::size_t index)->const OriginalCampaignCommand&{return script.commands.at(index);};
    auto sourceCommandIndex=[&](int id){return std::find_if(bank.clip_needs.begin(),bank.clip_needs.end(),[&](const auto& n){return n.animation_id==id;})->command_index;};
    bool handled=false,blocking=false;
    const auto& pre=commandAt(sourceCommandIndex(1377));
    TrollReturnRuntimeServicesV1 missingActorServices;
    TrollReturnRuntimeV1 missingActorRuntime(campaign,session,bank,std::move(missingActorServices));
    check(!missingActorRuntime.command(CampaignCommandPhase::execute,pre,7,false,handled,blocking,error)&&
          error.find("authored object to ActorId resolver is unavailable")!=std::string::npos,
          "Missing same-session source object mapping fails without requiring a native Character");
    TrollReturnRuntimeServicesV1 mismatchedActorServices;
    mismatchedActorServices.borrow_actor=[&](const std::string& name,int module,CombatSession& current,TrollReturnActorBorrowV1& out,std::string& e){
        out.session=&current;out.source_object_name=name;out.module=module+1;out.actor_id=current.player_id();
        out.actor_binding_lease=current.actor_binding_lease().lock();e.clear();return static_cast<bool>(out.actor_binding_lease);
    };
    TrollReturnRuntimeV1 mismatchedActorRuntime(campaign,session,bank,std::move(mismatchedActorServices));
    check(!mismatchedActorRuntime.command(CampaignCommandPhase::execute,pre,7,false,handled,blocking,error)&&
          error.find("exact source name/module")!=std::string::npos,
          "Mismatched source module mapping cannot borrow an actor from the same session");
    check(runtime.command(CampaignCommandPhase::execute,pre,7,false,handled,blocking,error)&&handled,error);
    check(session.retained_actor_pose(trollActorId)==retained,"Authored command preserves the same retained pose owner");
    check(runtime.command(CampaignCommandPhase::is_blocking,pre,7,false,handled,blocking,error)&&blocking,"Wait command reads shared retained pose completion");
    InputActions input;
    for(unsigned frame=0;frame<240&&blocking;++frame){
        check(session.update(1.0/60.0,input,{0,0,0},0,error),error);
        check(runtime.command(CampaignCommandPhase::is_blocking,pre,7,false,handled,blocking,error),error);
    }
    check(!blocking&&session.retained_actor_pose(trollActorId)==retained&&motions>0,"Pre-attack completes on the same CombatSession clock/owner and motion sink");

    const auto& attack=commandAt(sourceCommandIndex(1359));
    check(runtime.command(CampaignCommandPhase::execute,attack,7,false,handled,blocking,error)&&handled,error);
    check(runtime.command(CampaignCommandPhase::is_blocking,attack,7,false,handled,blocking,error)&&!blocking,"Authored non-wait attack command retains source wait semantics");
    for(unsigned frame=0;frame<240;++frame)check(session.update(1.0/60.0,input,{0,0,0},0,error),error);
    check(session.retained_actor_pose(trollActorId)==retained&&markers>0,"Attack leaf forwards its original marker and advances only with shared CombatSession clock");

    const auto& cameraCommand=commandAt(11);
    check(runtime.command(CampaignCommandPhase::execute,cameraCommand,7,false,handled,blocking,error)&&handled&&cameraFallbacks==1,
          "Authored camera44 stays on the existing cinematic command owner");

    const auto& post=commandAt(sourceCommandIndex(1373));
    check(runtime.command(CampaignCommandPhase::execute,post,7,false,handled,blocking,error)&&handled,error);
    for(unsigned frame=0;frame<240;++frame){
        check(runtime.command(CampaignCommandPhase::is_blocking,post,7,false,handled,blocking,error),error);
        if(!blocking)break;check(session.update(1.0/60.0,input,{0,0,0},0,error),error);
    }
    check(!blocking&&session.retained_actor_pose(trollActorId)==retained,"Post-attack completes through shared session owner");
    std::cout<<"PASS TrollReturn exact trigger/command identities, three source clip leaves, same CombatSession pose/clock completion, camera44 delegated to existing owner; no hit claim\n";
}
}
int main(int argc,char** argv){try{check(argc==2,"Repository root required");run(argv[1]);return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
