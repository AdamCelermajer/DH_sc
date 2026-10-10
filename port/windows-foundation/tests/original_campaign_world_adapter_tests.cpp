#include "../original_campaign_world_adapter.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool x,const std::string&e){if(!x)throw std::runtime_error(e);}
int main(int argc,char**argv){try{
 check(argc==2,"Original campaign asset directory required");AssetCatalog assets(argv[1]);OriginalCampaignRuntime runtime;std::string error;check(runtime.load(assets,"original-campaign.xml",error),error);
 ActorState first,second;first.id=41;second.id=42;first.health=second.health=100;
 OriginalActorLifecycle lifecycle;OriginalLifecycleServices life;
 life.floor_height=[](std::array<float,3>p,bool&found,float&height,std::string&){found=true;height=p[2];return true;};
 std::vector<ActorId>spawned;life.invoke=[&](const OriginalLifecycleRequest&q,std::string&){if(q.operation==OriginalLifecycleOperation::select_state_animation&&q.state==1)spawned.push_back(q.actor->id);if(q.operation==OriginalLifecycleOperation::restore_initial_position)q.actor->transform.position=q.initial_transform.position;return true;};lifecycle.bind(life);
 OriginalLifecycleFacts facts;facts.preset_ai_state="Limbus";facts.initially_enabled=true;facts.pre_spawn_has_animation=false;facts.pre_spawn_stay_enabled=false;facts.initial_transform.position={-3725.75f,252.509f,250};check(lifecycle.add(first,facts,error),error);facts.initial_transform.position={-4175.23f,349.838f,250};check(lifecycle.add(second,facts,error),error);
 CampaignCameraProviders cp;std::vector<std::string>camera_names;
 cp.named_target=[&](const std::string&name,std::uint64_t&id,bool&found,std::string&){camera_names.push_back(name);id=80;found=true;return true;};cp.local_player=[](std::uint64_t&id,std::string&){id=1;return true;};cp.anchor=[](std::uint64_t id,CameraVec3&out,std::string&){out={float(id),0,0};return true;};CampaignCameraAdapter camera(cp);check(camera.seed_target(1,error),error);
 OriginalCampaignWorldAdapter adapter(lifecycle,&camera);adapter.bind_runtime(runtime);OriginalCampaignWorldProviders world;bool global_block=false,tutorial_enabled=false;unsigned named_locks=0,tutorial_queries=0,tutorial_consumed=0,settings_saved=0;std::vector<std::string>effects;
 world.named_character=[&](const std::string&name,int module,ActorId&id,bool&found,std::string&){check(module==7,"Spawn/lock lookup lost exact module context");found=true;if(name=="_prim_Monster_LizManIntro1")id=first.id;else if(name=="_prim_Monster_LizManIntro2")id=second.id;else{found=false;id=0;}return true;};
 world.character_selector=[](const std::string&,int module,std::vector<ActorId>&ids,std::string&){check(module==7,"Selector module scope changed");ids={41,42};return true;};
 world.global_controller_blocked=[&](bool b,std::string&){global_block=b;effects.push_back(b?"global-lock":"global-unlock");return true;};
 world.character_controller_blocked=[&](ActorId,bool,std::string&){++named_locks;return true;};world.set_scripted=[&](ActorId,bool b,std::string&){effects.push_back(b?"scripted":"unscripted");return true;};world.stop_actor=[](ActorId,std::string&){return true;};
 world.idle_gate=[&](ActorId id,bool&allowed,std::string&){auto*s=lifecycle.status(id);allowed=s&&s->state!=0&&s->state!=17;return true;};
 // Fake typed original Idle owner retains waitForAnim and does not force Idle.
 world.set_idle=[&](ActorId,bool wait,std::string&){check(wait,"Normal script lost source waitForAnim");effects.push_back("wait-idle");return true;};
 world.cutscene_mode=[&](bool enter,std::string&){effects.push_back(enter?"cutscene-enter":"cutscene-exit");return true;};
 world.flash=[](bool,const std::string&,std::uint32_t,bool wait,CampaignCommandPhase,bool&blocking,std::string&){check(!wait,"Fixture needs original nonwaiting Flash commands");blocking=false;return true;};
 world.dialog=[](const OriginalCampaignCommand&,CampaignCommandPhase,bool&blocking,std::string&){blocking=false;return true;};world.flush_messages=[](std::string&){return true;};world.request_save=[](std::string&){return true;};world.block_save=[](std::string&){return true;};
 world.tutorial_gate=[&](int id,OriginalTutorialGate&gate,std::string&){check(id==7,"Original tutorial ID changed");++tutorial_queries;gate.player_available=true;gate.difficulty=0;gate.online=false;gate.enabled=tutorial_enabled;return true;};world.consume_tutorial=[&](int,std::string&){++tutorial_consumed;return true;};world.save_tutorial_settings=[&](bool immediately,std::string&){check(!immediately,"Closed settings menu incorrectly saved immediately");++settings_saved;return true;};adapter.bind(world);
 OriginalCampaignServices scheduler;scheduler.admit_start=[](int,int,bool,bool&allowed,std::string&){allowed=true;return true;};scheduler.command=[&](CampaignCommandPhase p,const OriginalCampaignCommand&c,int module,bool&block,std::string&e){return adapter.command(p,c,module,false,block,e);};runtime.bind(scheduler);
 auto id=runtime.script_id("LizardMan_Intro",false);check(runtime.start(id,7,false,error),error);
 check(runtime.tick(0,error)&&runtime.tick(0,error),error);check(global_block&&spawned.empty(),"Source cutscene lock order");
 check(runtime.tick(500,error)&&spawned.empty(),error);check(runtime.tick(0,error)&&spawned==std::vector<ActorId>{41},error);
 check(lifecycle.status(41)->state==1&&!lifecycle.combat_enabled(41),"Spawn command skipped actual state1");
 check(runtime.tick(1500,error)&&runtime.tick(0,error)&&spawned==std::vector<ActorId>({41,42}),error);
 check(runtime.tick(2000,error)&&runtime.tick(0,error)&&runtime.tick(0,error),error);
 check(!runtime.running(id)&&!global_block&&named_locks==0,"All lock was incorrectly expanded into per-character locks");check(camera_names.size()==2&&camera_names[0]=="_prim_Waypoint_NewCamSpot"&&camera_names[1]=="LocalPlayer","Source camera operands/order changed");
 check(tutorial_queries==1&&tutorial_consumed==0&&settings_saved==0,"Disabled tutorial gate bypassed");
 check(lifecycle.status(41)->state==1&&lifecycle.status(42)->state==1,"Idle waitForAnim provider bypassed");check(lifecycle.animation_finished(41,error)&&lifecycle.combat_enabled(41),error);
 OriginalCampaignCommand unbound;unbound.kind=5;unbound.class_name="Script_PlayCamera";bool block=false;check(!adapter.command(CampaignCommandPhase::execute,unbound,7,false,block,error)&&error.find("Unbound")!=std::string::npos,"Unbound source effect swallowed"); // P16 CINE2: kind 5 has its own provider now
 OriginalCampaignWorldAdapter missing(lifecycle);OriginalCampaignCommand spawn;spawn.kind=30;spawn.strings[12]="source-actor";check(!missing.command(CampaignCommandPhase::execute,spawn,7,false,block,error),"Missing lookup provider accepted");
 // Source Lock matches All case-insensitively, but source Unlock uses strcmp.
 OriginalCampaignCommand lock;lock.kind=24;lock.strings[12]="ALL";check(adapter.command(CampaignCommandPhase::execute,lock,7,false,block,error)&&global_block,error);lock.kind=25;check(adapter.command(CampaignCommandPhase::execute,lock,7,false,block,error)&&global_block,"Unlock's source case-sensitive rule changed");lock.strings[12]="All";check(adapter.command(CampaignCommandPhase::execute,lock,7,false,block,error)&&!global_block,error);
 lock.kind=24;check(adapter.command(CampaignCommandPhase::execute,lock,7,true,block,error)&&!global_block,"Skipped source Lock must do nothing");
 tutorial_enabled=true;OriginalCampaignCommand tutorial;tutorial.kind=78;tutorial.scalars[16]=7;tutorial.strings[12]="CombatTuto";
 check(adapter.command(CampaignCommandPhase::execute,tutorial,7,false,block,error),error);check(runtime.running(runtime.script_id("CombatTuto"))&&tutorial_consumed==1&&settings_saved==1,"Eligible tutorial did not start source continuation/consume/save");
 std::cout<<"Original campaign world adapter passed realSpawns=2 scope=7 sourcePreSpawn17=true disabledTutorialPreserved=true\n";
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
