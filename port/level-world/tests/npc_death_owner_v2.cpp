#include "../npc_death_owner_v2.hpp"
#include "../../script-runtime/script_constants.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <algorithm>
using namespace dh2;using namespace dh2::character;
namespace sk=dh2::character::skills;
static unsigned checks{};
static void require(bool b,int line){++checks;if(!b)throw std::runtime_error("NPC death composition line "+std::to_string(line));}
#define CHECK(x) require(bool(x),__LINE__)
static std::vector<std::uint8_t> file(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Missing actual cache: "+p);return {std::istreambuf_iterator<char>(f),{}};}
struct Cache {
 data::CharacterTable characters;data::PropertyRules rules;data::SkillTables skills;data::FaeryTables faeries;
 data::Dictionary dictionary;data::AnimationTables animations;dh2_script_constants* constants=dh2_script_constants_create();
 Cache(const std::string& p){std::string e;auto load=[&](const char* n){return file(p+"/"+n);};
  auto a=load("character_properties_pyarray.bin"),b=load("character_properties_pyarraynames.bin"),c=load("character_properties_pystructnames.bin");
  CHECK(data::load_characters({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},characters,e));CHECK(data::load_property_rules(characters,rules,e));
  a=load("skills_pyarray.bin");b=load("skills_pyarraynames.bin");c=load("skills_pystructnames.bin");CHECK(skills.load({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},e));
  a=load("faeries_pyarray.bin");b=load("faeries_pyarraynames.bin");c=load("faeries_pystructnames.bin");CHECK(faeries.load({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},e));
  a=load("animations_dictionary_pyarraynames.bin");b=load("animations_dictionary_pyarray.bin");CHECK(data::load_dictionary({a.data(),a.size()},{b.data(),b.size()},dictionary,e));
  a=load("animations_pyarray.bin");b=load("animations_pyarraynames.bin");c=load("animations_pystructnames.bin");CHECK(data::load_animation_tables({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},dictionary,animations,e));
  a=load("scripts_pycst.bin");dh2_script_constants_reload r{};CHECK(!dh2_script_constants_load(constants,a.data(),unsigned(a.size()),&r));
 }
 ~Cache(){dh2_script_constants_destroy(constants);}
};
// External Script/animation/FX/body/group services are explicit observers.
// Actual tables, instance storage, target, timers, reciprocal maps and registered
// source FSM/OnDied bodies below are production owners, never substitute maps.
struct Graph {
 static constexpr std::uintptr_t id=0x100000002ull,ai=id+0x3c8,other=0x100000003ull,third=0x100000004ull;
 Cache& cache;data::PropertyState properties;data::PropertyView view;
 Facts facts{};Services bodies{this,body};StateOwnerBehaviorPredicate8 predicates{};WorldNpcStateServicesV1 state_services{};
 std::unique_ptr<CharacterWorldNpcStateOwnerV1> machine;std::unique_ptr<NpcDeadStateV1> dead;
 std::unique_ptr<sk::CharacterSkillOwner> skills;
 TargetOwner16 target_owner{id,8,0,0};TargetState48 target{ai,&target_owner,other,other,third,1,1,1,0,0};TargetBindings48 targets{&target,{this,target_service},nullptr,{0,0}};
 TargetOwner16 other_owners[2]{{other,0,0,0},{third,0,0,0}};
 TargetState48 other_targets[2]{{other+0x3c8,&other_owners[0],id,id,id,1,1,1,0,0},{third+0x3c8,&other_owners[1],id,id,id,1,1,1,0,0}};
 TargetBindings48 other_bindings[2]{{&other_targets[0],{this,target_service},nullptr,{0,0}},{&other_targets[1],{this,target_service},nullptr,{0,0}}};
 Timer32 slots[8]{};TimerStore32 timers{slots,0,8,id,0,0};TimerServices32 timer_services{};AIDeathOwner24 death_owner{};AIDeathState64 death{};
 AIEventOwner48 event_owner{};std::uintptr_t keys[51]{},ai_keys[51]{};AIEventState64 events{};
 struct Relations {data::AggroEntry out[4]{},in[4]{};data::AggroTable outgoing{out,0,4},incoming{in,0,4};};Relations relations[3];
 std::unique_ptr<NpcDeathOwnerV2> owner;std::vector<std::string> trace;unsigned cleanup{},animation_calls{},ais_calls{};int fail=-1;std::string path="data/scripts/ai/";sk::Row16 path_row{};
 bool record(const std::string& s){trace.push_back(s);return fail<0||int(trace.size())!=fail;}
 static void body(void* p,State* state,const Request* q){auto& g=*static_cast<Graph*>(p);g.trace.push_back("body:"+std::to_string(q->service));
  if(q->service==set_animation){CHECK(q->argument[0]>=0&&std::size_t(q->argument[0])<g.cache.animations.sequences.size());state->current_animation=q->argument[0];++g.animation_calls;}
  if(q->service==start_timer)CHECK(dh2_character_timer_start(&g.timers,std::uint32_t(q->argument[0]),q->argument[1],q->argument[2],q->identity,&g.timer_services)>=0);
 }
 static int remaining(void*,StateOwnerMachine40*,const StateOwnerRequest48* q,StateOwnerResponse8*){return q->operation==state_owner_character_event||q->operation==state_owner_pin||q->operation==state_owner_profile_begin||q->operation==state_owner_profile_end?0:-1;}
 static int constant(void* p,const char* key,const char* group,int* out){return dh2_script_constants_get(static_cast<Graph*>(p)->cache.constants,key,group,out);}
 static int stance(void*,std::uintptr_t who,int* out){CHECK(who==id);*out=0;return 0;} // Declared empty-inventory stance fixture.
 static int target_service(void* p,TargetState48* s,const TargetRequest24* q,unsigned* out){auto& g=*static_cast<Graph*>(p);CHECK(s==&g.target||s==&g.other_targets[0]||s==&g.other_targets[1]);*out=0;return g.record("target:"+std::to_string(q->service))?0:-1;}
 static int script(void* p,sk::State40* s,const sk::Request48* q,sk::Response32* out){auto& g=*static_cast<Graph*>(p);CHECK(s->owner==id);
  if(!g.record("script:"+std::to_string(q->operation)))return -1;
  switch(q->operation){case sk::active_script:out->object=77;break;case sk::capture_path:g.path_row={g.path.data(),unsigned(g.path.size()),0};out->row=&g.path_row;break;
  case sk::assign_path:g.path.assign(q->name,q->count);break;case sk::load_file:out->count=1;break;case sk::set_skill:CHECK(q->instance&&q->instance->owner==id);out->object=88;break;
  case sk::call_cleanup:CHECK(std::string(q->name)=="OnSkillCleanUp");++g.cleanup;break;default:break;}return 0;
 }
 AggroClearActorBorrowV2 borrow(unsigned i){return {i==0?id:i==1?other:third,&relations[i].outgoing,&relations[i].incoming,i==0?&targets:&other_bindings[i-1]};}
 static bool actor(void* p,std::uintptr_t who,AggroClearActorBorrowV2& out,std::string& e){auto& g=*static_cast<Graph*>(p);if(!g.record("actor:"+std::to_string(who))){e="declared actor failure";return false;}unsigned i=who==id?0:who==other?1:who==third?2:3;if(i==3)return false;out=g.borrow(i);return true;}
 static bool notify(void* p,std::uintptr_t receiver,std::uintptr_t source,const dh2_script_callback_scope*,std::string& e){auto& g=*static_cast<Graph*>(p);CHECK(receiver==id||receiver==other||receiver==third);CHECK(source==id||source==other||source==third);if(!g.record("deaggro:"+std::to_string(receiver)+":"+std::to_string(source))){e="declared notification failure";return false;}return true;}
 static bool group(void* p,std::uintptr_t receiver,std::uintptr_t who,std::uintptr_t attacker,std::string& e){auto& g=*static_cast<Graph*>(p);CHECK(receiver==99&&who==id&&attacker==other);if(!g.record("group")){e="declared group failure";return false;}return true;}
 static bool ais(void* p,std::uintptr_t,std::uintptr_t,std::uintptr_t,const dh2_script_callback_scope*,std::string& e){++static_cast<Graph*>(p)->ais_calls;e="Unrecovered selected AIS";return false;}
 static int ai_event(void* p,AIEventState64*,const AIEventRequest40* q,unsigned* out){auto& g=*static_cast<Graph*>(p);CHECK(q->event==2);
  if(q->service==ai_event_state_event){const int result=g.machine->event(2,q->payload);CHECK(result>=0);*out=unsigned(result);return 0;}
  CHECK(q->service==ai_event_virtual&&q->operation==0x24&&q->callee==0x3d1000);AIDeathResult24 r{};return g.owner->on_died(r,q->payload)==1?0:-1;}
 Graph(Cache& c):cache(c){
  auto row=std::find(c.characters.names.begin(),c.characters.names.end(),"Crypt_Skeleton");CHECK(row!=c.characters.names.end());
  data::reset_properties(c.rules,properties,&c.characters.rows[std::size_t(row-c.characters.names.begin())]);view=data::property_view(c.rules,properties);std::string e;CHECK(data::recalc_properties(c.rules,properties,e));
  // Controlled sheet selects existing authored nonempty skill/faery lists.
  auto sb=c.skills.borrow();unsigned list=0;for(;list<sb.lists().size();++list){bool any=false;for(auto n:sb.lists()[list])if(n>=0&&std::size_t(n)<sb.skills().size())any|=!sb.skills()[n].script.empty();if(any)break;}CHECK(list<sb.lists().size());properties.base[28]=int(list);properties.base[29]=1;
  skills=std::make_unique<sk::CharacterSkillOwner>(id,&view,c.skills.borrow(),c.faeries.borrow(),sk::Services16{this,script});CHECK(skills->configure()==1);
  const int animation_row=dh2_character_animation_table_id(properties.resolved[2],int(c.animations.characters.size()));
  const auto* idle=data::animation_state(c.animations,animation_row,"Idle");CHECK(idle);facts.idle=int(idle-c.animations.sequences.data());
  state_services.facts=&facts;state_services.bodies=&bodies;state_services.predicates=&predicates;state_services.remaining_methods={this,remaining};machine=std::make_unique<CharacterWorldNpcStateOwnerV1>(id,state_services);
  const int initialized=machine->initialize_level("Idle");if(initialized!=1)throw std::runtime_error("NPC fixture initialization: "+machine->error());
  dead=std::make_unique<NpcDeadStateV1>(NpcDeadStateBorrowV1{machine.get(),&view,&c.animations,this,constant,stance});
  CHECK(dh2_character_timer_start(&timers,100,0,0x33,0,&timer_services)==0);CHECK(dh2_character_timer_start(&timers,200,0,0x34,0,&timer_services)==1);
  auto fsm=std::uintptr_t(&machine->owner().machine());death_owner={id,fsm,&timers};keys[9]=0x3dbe90;ai_keys[9]=0x3d1000;
  event_owner={id,id+0x1000,fsm,std::uintptr_t(&view),0,0,0,0};events={ai,&event_owner,ai_keys,88,keys,0,0,0,0,0,0};death={ai,&death_owner,&target,99,88,keys,0,1,0};
  for(unsigned i=1;i<3;++i){data::AggroChange r{};auto b=borrow(i);data::AggroRequest q{&relations[0].outgoing,b.incoming,id,b.identity,0x3f800000,0};CHECK(!dh2_aggro_apply(&r,&q,data::aggro_set));q={b.outgoing,&relations[0].incoming,b.identity,id,0x40000000,0};CHECK(!dh2_aggro_apply(&r,&q,data::aggro_set));}
  owner=std::make_unique<NpcDeathOwnerV2>(NpcDeathBorrowV2{&death,&events,&targets,borrow(0),dead.get(),skills.get()},NpcDeathServicesV2{{this,actor,notify},this,group,ais});trace.clear();cleanup=animation_calls=0;
 }
 unsigned instances()const{unsigned n=0;for(auto* s:{&skills->state().skills,&skills->state().spells})for(unsigned i=0;i<s->count;++i)n+=s->items[i]!=nullptr;return n;}
};
int main(int argc,char** argv){try{CHECK(argc==2);Cache c(argv[1]);Graph g(c);const auto expected=g.instances();CHECK(expected>0);AIEventResult16 r{};AIEventPayload24 payload{Graph::other,0,0,0};AIEventServices24 services{&g,Graph::ai_event,63,0};
 const int delivered=dh2_character_ai_event(&r,&g.events,2,&payload,&services);
 if(delivered){for(const auto& step:g.trace)std::cerr<<step<<'\n';throw std::runtime_error("Death phase: "+g.owner->error()+"; FSM: "+g.machine->error());}
 CHECK(g.machine->state().current==12&&g.machine->state().controller_locked==1&&g.animation_calls==1);
 CHECK(g.target.target==0&&g.target.last_target==0&&g.target.candidate==0&&g.target_owner.word14d0==0);CHECK(g.death.timer0==UINT32_MAX&&g.death.timer1==UINT32_MAX&&!g.slots[0].active&&!g.slots[1].active);
 CHECK(g.cleanup==expected&&g.ais_calls==0);for(auto& a:g.relations)CHECK(a.outgoing.count==0&&a.incoming.count==0);CHECK(g.trace.front()=="group");
 unsigned prefixes=0;const auto trace=g.trace;for(unsigned i=0;i<trace.size();++i){if(trace[i].find("body:")==0)continue;Graph failed(c);failed.fail=int(i+1);AIDeathResult24 out{};CHECK(failed.owner->on_died(out,Graph::other)<0);CHECK(!failed.owner->error().empty());CHECK(failed.trace==std::vector<std::string>(trace.begin(),trace.begin()+i+1));++prefixes;}
 Graph bad(c);bad.events.owner->owner=Graph::other;AIDeathResult24 untouched{1,2,3,4,5,6};CHECK(bad.owner->on_died(untouched,Graph::other)==-1&&untouched.phase==1&&bad.trace.empty());
 Graph unknown(c);unknown.keys[9]=0x1234;AIDeathResult24 stopped{};CHECK(unknown.owner->on_died(stopped,Graph::other)==-2&&unknown.ais_calls==1&&unknown.target.target==Graph::other&&unknown.slots[0].active);
 std::cout<<"{\"status\":\"PASS\",\"checks\":"<<checks<<",\"failure_prefixes\":"<<prefixes<<",\"retained_skill_spell_cleanup\":"<<expected<<",\"actual_cache\":true,\"registered_death_FSM\":true,\"source_AI_event2\":true,\"reciprocal_actors\":3,\"animation_endpoint_fixture\":true,\"script_endpoint_fixture\":true,\"actual_finite_animation_completion\":false,\"full_CtrlKill_rewards\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
