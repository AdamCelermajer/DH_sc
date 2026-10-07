#include "../character_script_session.hpp"
#include "../character_script_objects.hpp"
#include "../character_enemy_spotted.hpp"
#include "../character_ai_events.hpp"
#include "../character_controller_commands.hpp"
#include "../character_path_commands.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <dlfcn.h>
using namespace dh2::character;
namespace {
using Raw=std::vector<std::uint8_t>;
unsigned checks=0,lua_checks=0;
void require(bool ok,unsigned line){++checks;if(!ok)throw std::runtime_error("Target pipeline line "+std::to_string(line));}
#define check(x) require(bool(x),__LINE__)
std::uint32_t word(std::istream& in){std::uint32_t w=0;in.read(reinterpret_cast<char*>(&w),4);check(in);return w;}
Raw blob(std::istream& in){Raw b(word(in));if(!b.empty())in.read(reinterpret_cast<char*>(b.data()),b.size());check(in);return b;}
Raw file(const char* path){std::ifstream in(path,std::ios::binary);check(in);return {std::istreambuf_iterator<char>(in),{}};}
struct Inputs {
 std::array<std::array<Raw,3>,5> tables;std::vector<Raw> constants;std::vector<dh2::data::Bytes> views;GameDesignInputs256 input{};
 explicit Inputs(const char* path){std::ifstream in(path,std::ios::binary);check(in&&word(in)==0x314f4447);
  for(auto& t:tables)for(auto& b:t)b=blob(in);auto n=word(in);for(unsigned i=0;i<n;++i){blob(in);constants.push_back(blob(in));}n=word(in);for(unsigned i=0;i<n;++i)blob(in);check(in.peek()==EOF);
  GameDesignTableInput48* dst[]={&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};for(unsigned i=0;i<5;++i)*dst[i]={{tables[i][0].data(),tables[i][0].size()},{tables[i][1].data(),tables[i][1].size()},{tables[i][2].data(),tables[i][2].size()}};
  for(auto& b:constants)views.push_back({b.data(),b.size()});input.constants=views.data();input.constant_count=views.size();
 }
};
struct Fixture {
 CharacterGameDesign::Borrow design;DebugSwitches* debug=dh2_character_debug_create();
 DebugFileServices24 files{this,open,close};DebugLevelBinding16 db{debug,&files};LevelServices16 level_services{&db,dh2_character_debug_level_service};
 HostPlayer8 player{1,0};HostLevel8 level{23,0};std::vector<LevelRangeRow24> rows;HostContextBindings16 host{{this,host_call}};
 CharacterScriptObjects objects;std::shared_ptr<ScriptCharacterObject> owner,enemy;
 State state{};NativeFsm24 fsm{};ScriptCommandState48 command_state{};ScriptCommandBindings40 commands{&command_state,{this,command,number},nullptr};
 std::unique_ptr<CharacterScriptSession> session;ScriptSessionView view{};
 TargetEventState32 prefix{};EnemySpottedState16 spotted{};std::uint32_t threat=0;
 EnemySpottedServices24 enemy_services{this,enemy_call,&threat};std::vector<std::int32_t> sounds;
 TargetEventServices40 target_services{this,target_call,nullptr,0,0,0x501};
 AIEventOwner48 router_owner{};AIEventState64 router{};std::array<std::uintptr_t,51> ai_vtable{};AIEventServices24 routes{this,route,(1u<<ai_event_virtual)|(1u<<ai_event_state_event),0};
 std::vector<std::string> trace;unsigned source_events=0,endpoints=0,revived=0,fsm_calls=0,commands_called=0,path_queries=0,opens=0,aggro_adds=0,sound_calls=0,fixture_clears=0,nested=0,busy_guards=0;
 bool recurse=false,fail_command=false;unsigned gate=0;const dh2_script_callback_scope* scope=nullptr;
 explicit Fixture(CharacterGameDesign& d):design(d.borrow()),objects(d.borrow(),debug,&files){check(debug);
  for(const auto& r:design.levels()->levels){LevelRangeRow24 row;std::memcpy(&row,r.scalar.words+12,24);rows.push_back(row);}for(const auto& r:design.ai()->rows)sounds.push_back(r.on_aggro_sfx);
  auto it=std::find(design.characters()->names.begin(),design.characters()->names.end(),"Crypt_Skeleton");check(it!=design.characters()->names.end());auto index=it-design.characters()->names.begin();
  auto make=[&](std::uintptr_t id,const char* name){auto p=std::make_shared<dh2::data::PropertyState>();auto c=std::make_shared<dh2::data::CombatActorState>();dh2::data::reset_properties(*design.rules(),*p,&design.characters()->rows[index]);std::string error;check(dh2::data::recalc_properties_with_class(*design.classes(),*design.rules(),*p,error));return objects.add(id,name,p,c,{0,0,0});};
  owner=make(UINT64_C(0x100000101),"source_monster");enemy=make(UINT64_C(0x100000102),"retained_enemy");enemy->position={1,2,3};
  state.current=3;fsm={&state,owner->identity,1,0};static const float axis[3]{0,0,1};command_state={owner->identity,0x701,0,owner->position.data(),axis,0,0};
  prefix={&owner->target,0,1,{},0};spotted={&prefix,0x601};target_services.ai_sounds=sounds.data();target_services.ai_count=sounds.size();
  router_owner={owner->identity,command_state.controller,reinterpret_cast<std::uintptr_t>(&fsm),reinterpret_cast<std::uintptr_t>(owner->properties.get()),0,0,0,0};
  for(unsigned i=0;i<ai_vtable.size();++i)ai_vtable[i]=0x1000+i;router={owner->target.identity,&router_owner,ai_vtable.data(),0,nullptr,0,0,0,0,0,0};
 }
 ~Fixture(){session.reset();dh2_character_debug_destroy(debug);}
 static int open(void* p,const char* name,std::uintptr_t* out){auto& f=*static_cast<Fixture*>(p);check(!std::strcmp(name,"DebugSwitches.savegame"));++f.opens;*out=0;return 0;} // Explicit filesystem missing-file fixture.
 static int close(void*,std::uintptr_t){check(false);return 1;}
 static int host_call(void* p,const HostContextRequest16* r,HostContextResponse16* out){auto& f=*static_cast<Fixture*>(p);if(r->service==host_get_player)out->data=&f.player;else if(r->service==host_get_current_level)out->data=&f.level;else if(r->service==host_get_range_rows){out->data=f.rows.data();out->count=f.rows.size();}else return 1;return 0;}
 void create(CharacterGameDesign& d,const Raw& common,const Raw& monster){CharacterScriptSessionInput in;in.identity=owner->identity;in.name=owner->name;in.properties=owner->properties;in.combat=owner->life;in.position=owner->position;in.source_is_character=1;in.common={common.data(),common.size()};in.external={monster.data(),monster.size()};in.host=&host;in.level=&level_services;in.target=&owner->binding;in.state_machine=&fsm;in.objects=&objects.services();in.commands=&commands;
  std::string error;session=CharacterScriptSession::create(d.borrow(),in,error);check(session&&error.empty()&&!session->start()&&session->owner().active(view));check(session->script_name()=="monster");prefix.active=router.active=view.identity;
 }
 void lua(const char* code){int status=dh2_script_vm_load_source_file(view.vm,code,std::strlen(code));if(status)std::cerr<<dh2_script_vm_error(view.vm)<<'\n';check(!status);++lua_checks;}
 int dispatch(unsigned event){AIEventPayload24 payload{enemy->identity,0,0,0};AIEventResult16 result{};++source_events;return dh2_character_ai_event(&result,&router,event,&payload,&routes);}
 static int route(void* p,AIEventState64*,const AIEventRequest40* r,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);*out=0;
  if(r->service==ai_event_state_event){f.trace.push_back("FSM");++f.fsm_calls;check(r->subject==reinterpret_cast<std::uintptr_t>(&f.fsm));return 0;} // Required source FSM event body is fixture.
  check(r->service==ai_event_virtual);f.trace.push_back("prefix");int status=r->event==9?dh2_character_enemy_spotted(&f.spotted,r->payload,&f.enemy_services):dh2_character_target_event(&f.prefix,r->event,&f.target_services);return status?1:0;
 }
 int debug_call(unsigned op,const char* text,unsigned load,unsigned construct,unsigned query,unsigned destroy){
  if(op==load){trace.push_back("debug.load");return dh2_character_debug_load(debug,&files)<0;}
  if(op==construct){check(text);trace.push_back(std::string("debug.name:")+text);return 0;} // Owned std::string test projection; original string helper remains separately proved.
  if(op==query){std::uint32_t value;trace.push_back("debug.query");return dh2_character_debug_get(&value,debug,text,&files)<0;}
  check(op==destroy);trace.push_back("debug.destroy");return 0;
 }
 static int enemy_call(void* p,EnemySpottedState16*,const EnemySpottedRequest48* r,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);*out=0;
  if(r->service<=enemy_debug_destroy)return f.debug_call(r->service,r->text,enemy_debug_load,enemy_debug_construct,enemy_debug_query,enemy_debug_destroy);
  if(r->service==enemy_group_spotted){f.trace.push_back("group");check(r->subject==f.spotted.group&&r->other==f.owner->identity&&r->enemy==f.enemy->identity);return 0;}
  if(r->service==enemy_awaiting_spawn||r->service==enemy_in_limbus){f.trace.push_back("SM");unsigned selected=r->service==enemy_awaiting_spawn?(r->subject==f.enemy->identity?1:2):(r->subject==f.enemy->identity?3:4);*out=f.gate==selected;return 0;}
  if(r->service==enemy_in_combat){f.trace.push_back("combat");return 0;}
  if(r->service==enemy_get_aggro){f.trace.push_back("aggro.get");return 0;}
  if(r->service==enemy_add_aggro){f.trace.push_back("aggro.add");check(r->word==f.threat&&r->enemy==f.enemy->identity&&r->subject==f.owner->identity);*out=r->word;++f.aggro_adds;return 0;}
  check(r->service==enemy_active_dispatch);return f.endpoint(9,r->enemy);
 }
 static int target_call(void* p,TargetEventState32*,const TargetEventRequest64* r,TargetEventResponse24* out){auto& f=*static_cast<Fixture*>(p);*out={};
  if(r->service<=target_event_debug_destroy)return f.debug_call(r->service,r->text,target_event_debug_load,target_event_debug_construct,target_event_debug_query,target_event_debug_destroy);
  if(r->service==target_event_is_target_character){auto current=f.objects.find(f.owner->target.target);out->word=current!=nullptr;f.trace.push_back("target.character");return 0;} // Registry class predicate fixture; no fake lookup.
  if(r->service==target_event_get_target_character){out->identity=f.owner->target.target;f.trace.push_back("target.get");return 0;}
  if(r->service==target_event_clear_aggro){check(r->other==f.owner->target.target);f.trace.push_back("aggro.clear.prefix");return 0;}
  if(r->service==target_event_owner_ai_id){std::int32_t id;auto status=dh2_character_target_ai_id(&id,f.owner->properties->resolved.data(),f.design.ai()->rows.size());std::memcpy(&out->word,&id,4);f.trace.push_back("AI.id");return status;}
  if(r->service==target_event_owner_position){std::memcpy(out->position,f.owner->position.data(),12);f.trace.push_back("position");return 0;}
  if(r->service==target_event_play_sound){check(r->sound==f.sounds[f.owner->properties->resolved[1]]&&r->flag==1&&r->integer==0&&r->parameters[0]==-1&&r->parameters[1]==-1);check(!std::memcmp(r->position,f.owner->position.data(),12));f.trace.push_back("sound");++f.sound_calls;return 0;}
  check(r->service==target_event_active_dispatch);return f.endpoint(r->event,0);
 }
 int endpoint(unsigned event,std::uintptr_t id){trace.push_back("AIS");++endpoints;
  if(event==0xb){++revived;trace.push_back("inherited.empty.revived");return 0;} // Exact selected Default/External +44 bxLR body proved in endpoints-original-probe.json.
  unsigned mapped=event==9?1:event==0xa?2:event-9;
  int status=session->dispatch_target(mapped,id,scope);if(status)trace.push_back("Lua.error");else trace.push_back("Lua.ok");return status?1:0;
 }
 static int number(void*,const dh2_script_value* v,float* out){if(v->type!=DH2_SCRIPT_NUMBER)return 1;*out=v->number;return 0;} // Unused by object-only authored callbacks.
 static int control(void* p,const CharacterControlRequest32* r,CharacterControlResponse16* out){auto& f=*static_cast<Fixture*>(p);
  if(r->service==control_is_remotely_updated){out->word=0;f.trace.push_back("remote.fixture");return 1;}
  if(r->service==control_target_position){auto object=f.objects.find(r->subject);if(!object)return -1;std::memcpy(out->position,object->position.data(),12);f.trace.push_back("target.position");return 1;}
  if(r->service==control_path_to){PathToState40 state{f.owner->identity,0,0,0,0,{0,0,0},0};PathToResult16 result{};PathToServices16 services{&f,[](void* p,const PathToRequest32* r,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);check(r->owner==f.owner->identity&&!std::memcmp(r->target,f.enemy->position.data(),12));*out=0;++f.path_queries;f.trace.push_back("FindPath.failed.fixture");return 0;}};check(!dh2_character_path_to(&result,&state,r->position,&services)&&result.requested&&!result.find_result);return 1;}
  if(r->service==control_stop_object){f.trace.push_back("Stop.fixture");f.command_state.path_count=0;return 1;}
  if(r->service==control_character_event){check(r->argument==0x3f);f.trace.push_back("event3f.fixture");return 1;}return -1;
 }
 static int command(void* p,ScriptCommandState48*,const ScriptCommandRequest40* r,const float**){auto& f=*static_cast<Fixture*>(p);check(f.commands.scope&&dh2_script_callback_scope_valid(f.commands.scope));++f.commands_called;f.trace.push_back("command");if(f.fail_command)return 1;
  if(f.recurse){f.recurse=false;auto old=f.scope;f.scope=f.commands.scope;dh2_script_value ignored{};check(dh2_script_vm_get_global(f.view.vm,"should_fail",&ignored)==-1);++f.busy_guards;
   check(f.session->dispatch_target(4)==-2);++f.busy_guards; // Generic recursive entry stays forbidden.
   check(!f.dispatch(0xd));f.scope=old;check(dh2_script_callback_scope_valid(f.commands.scope));++f.nested;f.trace.push_back("nested.done");
  }
  if(r->service==script_controller_move_object||r->service==script_controller_stop){ControllerCommandState32 ctrl{f.command_state.controller,f.owner->identity,0,0,0,0};CharacterControlServices16 services{&f,control};check(dh2_character_controller_character(&ctrl,r->service==script_controller_stop?controller_stop:controller_move_object,r->target,&services)==1);return 0;}
  if(r->service==script_controller_attack){check(r->target==f.owner->target.target);f.trace.push_back("Attack.fixture");return 0;}
  return 1;
 }
 static int clear_fixture(void* p,const dh2_script_callback_scope* scope,const dh2_script_value* args,unsigned n,char*,std::size_t){auto& f=*static_cast<Fixture*>(p);check(dh2_script_callback_scope_valid(scope)&&n==1&&(args[0].type==DH2_SCRIPT_SOURCE_OBJECT||args[0].type==DH2_SCRIPT_IDENTITY)&&args[0].identity==f.enemy->identity);++f.fixture_clears;f.trace.push_back("ClearAggro.Lua.fixture");return 0;}
 void clear(){check(!dh2_character_clear_target(&owner->target,&owner->binding.services));command_state.target=0;trace.clear();}
 void seed(){check(!dh2_character_ai_set_target(&owner->target,enemy->identity,0,&owner->binding.services));command_state.target=owner->target.target;trace.clear();}
 void clean_scope(){check(!commands.scope&&!owner->binding.scope);}
};
std::size_t at(const std::vector<std::string>& v,const char* s){auto i=std::find(v.begin(),v.end(),s);check(i!=v.end());return i-v.begin();}
}
int main(int argc,char** argv){try{
 check(argc==5);Inputs inputs(argv[1]);auto common=file(argv[2]),monster=file(argv[3]);std::ifstream gold(argv[4],std::ios::binary);check(word(gold)==0x31534543);check(word(gold)==409);auto threat=word(gold);check(threat==0x41200000);
 CharacterGameDesign design;std::string error;check(design.initialize(inputs.input,error));Fixture f(design);f.threat=threat;f.create(design,common,monster);f.lua("assert(not HasTarget()); assert(type(GetID())=='userdata')");
 // Real source enemy Lua sets the retained target and executes HeadTo via actual controller/Character/PathTo kernels.
 check(!f.dispatch(9));check(f.owner->target.target==f.enemy->identity&&f.owner->target.last_target==f.enemy->identity&&f.path_queries==1);f.clean_scope();
 check(at(f.trace,"group")<at(f.trace,"SM")&&at(f.trace,"aggro.add")<at(f.trace,"AIS")&&at(f.trace,"AIS")<at(f.trace,"FindPath.failed.fixture")&&at(f.trace,"FindPath.failed.fixture")<at(f.trace,"FSM"));
 f.lua("saved=GetTarget(); assert(type(saved)=='table' and saved:GetID()==saved._this); assert(saved~=GetTarget() and saved:GetID()==GetTarget():GetID())");
 auto paths=f.path_queries;check(!f.dispatch(9)&&f.path_queries==paths); // Source HasTarget prevents SetTarget/HeadTo.
 f.prefix.continued=1;f.trace.clear();check(!f.dispatch(0xa));check(!f.prefix.continued&&!f.owner->target.target&&!f.owner->target.last_target&&at(f.trace,"Stop.fixture")<f.trace.size());check(std::find(f.trace.begin(),f.trace.end(),"FSM")==f.trace.end());f.clean_scope();
 // Revived executes selected inherited empty body and preserves both target and continued.
 f.seed();f.prefix.continued=77;check(!f.dispatch(0xb)&&f.revived==1&&f.prefix.continued==77&&f.owner->target.target==f.enemy->identity);f.clean_scope();
 // Preserve the actual unsupported wrapper failure prefix before a named test-only suffix fixture.
 f.owner->target.changed=1;check(f.dispatch(0xc)==3);check(f.owner->target.target==f.enemy->identity&&!f.owner->target.changed&&f.session->error().find("Unsupported source global ClearAggro")!=std::string::npos);f.clean_scope();
 check(!dh2_script_vm_bind_source_scoped(f.view.vm,"ClearAggro",Fixture::clear_fixture,&f));f.trace.clear();check(!f.dispatch(0xc));check(!f.owner->target.target&&!f.owner->target.last_target&&f.fixture_clears==1);check(at(f.trace,"aggro.clear.prefix")<at(f.trace,"Stop.fixture")&&at(f.trace,"Stop.fixture")<at(f.trace,"ClearAggro.Lua.fixture"));f.clean_scope();
 f.seed();check(!f.dispatch(0xd));check(f.sound_calls==1);f.lua("assert(HasTarget())");
 f.trace.clear();check(!f.dispatch(0xe));check(f.path_queries==paths+1&&!f.owner->target.changed);f.clean_scope();
 // Real ranged/melee callbacks can require unrecovered skills. Assert actual success or explicit missing global, retaining prefixes.
 unsigned missing_skill=0;for(unsigned event:{0xfu,0x11u}){f.seed();auto result=f.dispatch(event);if(result){check(result==3&&f.session->error().find("Unsupported source global")!=std::string::npos);++missing_skill;}else check(at(f.trace,"Attack.fixture")<f.trace.size());f.clean_scope();}
 f.seed();check(f.dispatch(0x10)==3&&f.session->error().find("Unsupported source global CreateBuff")!=std::string::npos);check(f.owner->target.target==f.enemy->identity);f.clean_scope();
 // 72 source gate combinations: blocked/locked events bypass prefixes and go only to FSM, forced bypasses both gates.
 unsigned gate_cases=0;for(unsigned event=9;event<=0x11;++event)for(unsigned flags=0;flags<8;++flags){f.seed();f.router_owner.forced=flags&1;f.router.global_blocked=(flags>>1)&1;f.router_owner.locked=(flags>>2)&1;bool blocked=!f.router_owner.forced&&(f.router.global_blocked||f.router_owner.locked);auto before=f.endpoints,fsms=f.fsm_calls;auto status=f.dispatch(event);if(blocked){check(!status&&f.endpoints==before&&f.fsm_calls==fsms+1&&f.trace==std::vector<std::string>{"FSM"});}else {check(f.endpoints==before+1);check(f.fsm_calls==fsms+(event==9?1:0));if(status)check(status==3);}f.clean_scope();++gate_cases;}
 f.router_owner.forced=f.router_owner.locked=f.router.global_blocked=0;
 for(unsigned gate=1;gate<=4;++gate){f.clear();f.gate=gate;auto endpoints=f.endpoints,fsms=f.fsm_calls;check(!f.dispatch(9)&&f.endpoints==endpoints&&f.fsm_calls==fsms+1&&!f.owner->target.target);check(at(f.trace,"group")<at(f.trace,"SM"));}f.gate=0;
 // Null active keeps prefix effects and event9 FSM, but no fabricated AIS call.
 f.clear();f.prefix.active=0;auto endpoints=f.endpoints;check(!f.dispatch(9)&&f.endpoints==endpoints&&!f.owner->target.target);f.prefix.active=f.view.identity;
 // Real nested source event chain while HeadTo holds a private same-VM callback capability.
 f.clear();f.recurse=true;auto sounds=f.sound_calls;check(!f.dispatch(9)&&f.nested==1&&f.sound_calls==sounds+1&&f.owner->target.target==f.enemy->identity);check(at(f.trace,"sound")<at(f.trace,"nested.done")&&at(f.trace,"nested.done")<at(f.trace,"FSM"));f.clean_scope();
 f.clear();f.fail_command=true;check(f.dispatch(9)==3&&f.owner->target.target==f.enemy->identity);check(std::find(f.trace.begin(),f.trace.end(),"FSM")==f.trace.end());f.clean_scope();f.fail_command=false;
 // Retained tables stay callable when the caller releases its enemy shared_ptr; registry pins through Lua close.
 auto identity=f.enemy->identity;f.enemy.reset();check(f.objects.find(identity));f.lua("assert(saved:GetID()==saved._this); proxy=newproxy(true); getmetatable(proxy).__gc=function() assert(GetTarget():GetID()==saved:GetID()); ClearTarget() end");f.session.reset();check(!f.owner->target.target&&!f.owner->target.last_target);f.clean_scope();
 check(f.opens==1);Dl_info world{},runtime{},prefix{};check(dladdr(reinterpret_cast<void*>(dh2_character_ai_event),&world)&&dladdr(reinterpret_cast<void*>(dh2_script_vm_create),&runtime)&&dladdr(reinterpret_cast<void*>(dh2_character_enemy_spotted),&prefix));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"Lua_checks\":"<<lua_checks<<",\"source_events\":"<<f.source_events<<",\"AIS_endpoints\":"<<f.endpoints<<",\"FSM_fixture_deliveries\":"<<f.fsm_calls<<",\"gate_cases\":"<<gate_cases<<",\"retained_records\":2,\"original_monster_Init\":1,\"nested_source_events\":"<<f.nested<<",\"busy_guards\":"<<f.busy_guards<<",\"ClearAggro_fixture_suffixes\":"<<f.fixture_clears<<",\"missing_skill_callbacks\":"<<missing_skill<<",\"sound_fixture_calls\":"<<f.sound_calls<<",\"FindPath_failed_fixture_queries\":"<<f.path_queries<<",\"finalizer_clear\":true,\"world_library\":\""<<world.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\",\"prefix_library\":\""<<prefix.dli_fname<<"\",\"whole_chain_original_differential\":false,\"full_enemy_AI\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
