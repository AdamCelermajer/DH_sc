#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#define main source_session_fixture_main
#include "character_script_session.cpp"
#undef main
#pragma GCC diagnostic pop
#include "../npc_injury_runtime_v1.hpp"
#include "../character_animation_instance.hpp"
#include <map>
namespace {
namespace target_providers=dh2::target_providers;
struct Reaction {
 NpcInjuryRuntimeV1* runtime{};CharacterWorldNpcStateOwnerV1* machine{};
 CharacterAnimationInstance* animation{};const dh2::data::AnimationTables* tables{};
 dh2::data::AnimationRandom random{};std::string error;std::uintptr_t looked{};
 unsigned selected=0,ends=0;
 static int remaining(void* p,StateOwnerMachine40* m,const StateOwnerRequest48* q,StateOwnerResponse8*){
  auto& r=*static_cast<Reaction*>(p);if(r.runtime){auto result=r.runtime->body(m,*q);if(result)return result==1?0:-1;}
  // Explicit observer: genuine monster OnStateChanged routing is separate.
  // Named outer fixtures isolate source VM notification, pin and profiling.
  if(q->operation==state_owner_character_event||q->operation==state_owner_pin||q->operation==state_owner_profile_begin||q->operation==state_owner_profile_end)return 0;
  std::cerr<<"Missing NPC method op "<<q->operation<<" state "<<q->state<<" source "<<std::hex<<q->source_function<<std::dec<<std::endl;return -1;
 }
 static int set(void* p,State* s,int sequence){auto& r=*static_cast<Reaction*>(p);if(sequence==-1){sequence=s->animation_override;s->animation_override=-1;}if(!r.animation->start(*r.tables,sequence,r.random,1,r.error))return -1;s->current_animation=sequence;++r.selected;return 0;}
 static void body(void* p,State* s,const Request* q){check(q->service==set_animation);check(set(p,s,q->argument[0])==0);}
 static void observe(void* p,dh2::actor::BlendedPlayback&,const dh2::actor::BlendedPlaybackEvent& event){auto& r=*static_cast<Reaction*>(p);if(event.event.handoff.event_id==0x22){++r.ends;check(r.machine->event(0x22,0)==1);}}
};
struct Debug {DebugSwitches* owner{};DebugFileServices24 files{};std::map<std::uintptr_t,std::string> tokens;unsigned serial=1;
 ~Debug(){if(owner)dh2_character_debug_destroy(owner);}
 static int call(void* p,const dh2::character::skills::SkillAttackNativeRequestV6* q,std::uintptr_t* out){auto& d=*static_cast<Debug*>(p);using namespace dh2::character::skills;switch(q->service){
 case skill_attack_debug_load_v6:return dh2_character_debug_load(d.owner,&d.files)==1?0:-1;
 case skill_attack_string_construct_v6:*out=++d.serial;d.tokens[*out]=q->name;return 0;
 case skill_attack_debug_get_v6:{std::uint32_t value{};auto at=d.tokens.find(q->subject);if(at==d.tokens.end())return -1;auto result=dh2_character_debug_get(&value,d.owner,at->second.c_str(),&d.files);*out=value;return result==1?0:-1;}
 case skill_attack_string_destroy_v6:return d.tokens.erase(q->subject)==1?0:-1;default:return -1;}}
};
}
int main(int argc,char** argv){try{
 check(argc==2);const std::string root=argv[1],assets=root+"/port/android-native/app/src/main/assets";
 Inputs raw((root+"/port/level-world/reference/character-game-design/real-cache-inputs.bin").c_str());raw.constants.push_back(file((assets+"/data/animations_pycst.bin").c_str()));raw.views.clear();for(const auto& constant:raw.constants)raw.views.push_back({constant.data(),constant.size()});raw.input.constants=raw.views.data();raw.input.constant_count=raw.views.size();CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error));auto d=design.borrow();
 auto common=file((root+"/.local-inputs/character-script-owner-discovery/ai-commons-source.luac").c_str()),monster=file((root+"/.local-inputs/character-script-owner-extension/monster.luac").c_str());auto authored=placements(file((assets+"/worlds/crypt01.dact").c_str()));
 Host host;host.rows.resize(d.levels()->levels.size());for(unsigned i=0;i<host.rows.size();++i)std::memcpy(&host.rows[i],d.levels()->levels[i].scalar.words+12,24);auto level=std::find(d.levels()->level_names.begin(),d.levels()->level_names.end(),"GOTHICUS_CRYPT_01");check(level!=d.levels()->level_names.end());host.level={int(level-d.levels()->level_names.begin()),0};host.player.cached_level=256;HostContextBindings16 host_binding{{&host,Host::invoke}};
 MissingSave missing;Debug debug;debug.files={&missing,MissingSave::open,MissingSave::close};debug.owner=dh2_character_debug_create();check(debug.owner);DebugLevelBinding16 dl{debug.owner,&debug.files};LevelServices16 levels{&dl,dh2_character_debug_level_service};skills::SkillAttackNativeServicesV6 debug_binding{&debug,Debug::call};
 auto input=[&](const char* name){return file((assets+"/data/"+name).c_str());};auto dn=input("animations_dictionary_pyarraynames.bin"),dv=input("animations_dictionary_pyarray.bin");dh2::data::Dictionary dictionary;check(dh2::data::load_dictionary({dn.data(),dn.size()},{dv.data(),dv.size()},dictionary,error));auto ar=input("animations_pyarray.bin"),an=input("animations_pyarraynames.bin"),af=input("animations_pystructnames.bin");dh2::data::AnimationTables tables;check(dh2::data::load_animation_tables({ar.data(),ar.size()},{an.data(),an.size()},{af.data(),af.size()},dictionary,tables,error));
 std::map<int,std::shared_ptr<const CharacterAnimationResources>> banks;
 for(const std::string tag:{"skeleton","slime","ghost"}){auto bank=input(("monster-"+tag+"-animation-bank.bin").c_str());auto model=file((assets+"/actors/"+(tag=="slime"?"slime_green_v2":tag)+".bdae").c_str());dh2::resources::BresView bres{};check(dh2_bres_open(&bres,model.data(),model.size())==dh2::resources::BresError::ok);dh2::scene::Scene factory;check(dh2::scene::load(bres,factory,error));std::shared_ptr<const CharacterAnimationResources> resources;check(CharacterAnimationResources::load({bank.data(),bank.size()},factory,[&](const dh2::data::AnimationBankResource& resource,Raw& out,std::string&){out=file((assets+"/"+resource.asset).c_str());return true;},resources,error));banks[resources->metadata().animation_table]=resources;}
 unsigned actors=0,ends=0;
 for(const auto& placement:authored){auto at=std::find(d.characters()->names.begin(),d.characters()->names.end(),placement.row);check(at!=d.characters()->names.end());CharacterScriptSessionInput in;in.identity=0x100000000ull+(++actors);in.name=placement.name;in.properties=std::make_shared<dh2::data::PropertyState>();in.combat=std::make_shared<dh2::data::CombatActorState>();in.position=placement.position;in.source_is_character=1;in.common={common.data(),common.size()};in.external={monster.data(),monster.size()};in.host=&host_binding;in.level=&levels;dh2::data::reset_properties(*d.rules(),*in.properties,&d.characters()->rows[at-d.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*in.properties,error));auto session=CharacterScriptSession::create(design.borrow(),in,error);check(bool(session)&&session->start()==0&&session->error().empty());
 auto& props=session->property_view();check(props.resolved[198]<=0);const auto table=props.resolved[2];auto bank=banks.find(table);check(bank!=banks.end());auto animation=CharacterAnimationInstance::create(bank->second,error);check(bool(animation));
 Reaction reaction;reaction.animation=animation.get();reaction.tables=&tables;Facts facts{};facts.idle=tables.characters[table].fields[9].front();StateOwnerBehaviorPredicate8 predicates{};Services bodies{&reaction,Reaction::body};WorldNpcStateServicesV1 state_services{};state_services.facts=&facts;state_services.bodies=&bodies;state_services.predicates=&predicates;state_services.remaining_methods={&reaction,Reaction::remaining};CharacterWorldNpcStateOwnerV1 machine(in.identity,state_services);reaction.machine=&machine;
 TargetOwner16 target_owner{in.identity,0,0,0};TargetState48 target{};target.owner=&target_owner;target.identity=in.identity;target.target=0xabcdef;
 skills::SkillTargetCharacterV6 character{};character.identity=in.identity;character.resolved=props.resolved;character.interactive415=1;float gate=-1;
 std::vector<int> types;for(const auto& row:d.ai()->rows)types.push_back(row.type);target_providers::Types16 type_table{types.data(),unsigned(types.size()),0};
 struct Query {skills::SkillTargetCharacterV6* character;target_providers::Types16* types;}query{&character,&type_table};target_providers::Services16 queries{&query,[](void* p,const target_providers::Request24* q,std::uintptr_t* out){auto& t=*static_cast<Query*>(p);if(q->service!=target_providers::virtual_player||q->subject!=t.character->identity)return -1;int result{};if(skills::dh2_character_skill_target_query_v6(&result,target_providers::is_player,t.character,nullptr,t.types,nullptr))return -1;*out=result;return 0;}};
 NpcInjuryBorrowV1 borrow{&machine,session.get(),&props,&tables,&d,&target,&character,&gate,in.combat.get()};NpcInjuryServicesV1 services{&debug_binding,queries,&reaction,nullptr,Reaction::set,[](void* p,std::uintptr_t,std::uintptr_t target){static_cast<Reaction*>(p)->looked=target;return 0;}};NpcInjuryRuntimeV1 runtime(borrow,services);reaction.runtime=&runtime;check(runtime.valid());check(machine.initialize_level("Idle")==1);check(runtime.cancel_sneaking()==0&&character.interactive415==1);
 skills::SkillApplyRequestV6 request{skills::skill_apply_injure_v6,0,0,0,in.identity,0x987654,in.identity,nullptr,0,0};skills::SkillApplyResponseV6 response{};const auto application=runtime.application(request,&response);if(application!=1)std::cerr<<"Injury status "<<application<<" state "<<machine.state().current<<" override "<<machine.state().animation_override<<" gate "<<gate<<" animation error "<<reaction.error<<" owner error "<<machine.error()<<std::endl;check(application==1&&machine.state().current==11&&gate==3000.f);check(machine.state().current_animation==tables.characters[table].fields[14].front());animation->playback().observer={&reaction,Reaction::observe};
 for(unsigned frame=0;frame<240&&machine.state().current==11;++frame){check(animation->scene_phase(1000+frame*17,error));check(animation->animator_phase(tables,reaction.random,1,error));}
 check(machine.state().current==3&&reaction.ends==1&&reaction.looked==target.target);ends+=reaction.ends;check(runtime.tick(3001)==1&&gate==-1.f);
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_Crypt_sessions\":"<<actors<<",\"native_injury_animation_end_to_Idle\":"<<ends<<",\"actual_CPU_animation\":true,\"controller_heading_fixture\":true,\"NPC_state_notification_fixture\":true}"<<std::endl;return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
