#include "../character_animation_selection_debug_v126.hpp"
#include "../character_world_npc_state_owner_v1.hpp"
// White-box input fixture for an already source-loaded Debug map. Production
// owns this same map/query implementation; this test does not extend its
// unavailable save-file decoder or turn SetSwitch's failure into success.
#include "../character_design_services.cpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace dh2;
namespace {
unsigned checks{};
void check(bool value,const char* reason){++checks;if(!value)throw std::runtime_error(reason);}
struct Files {
 unsigned opens{},closes{};int failure{};std::uintptr_t handle{};
 static int open(void* raw,const char* name,std::uintptr_t* out){auto& f=*static_cast<Files*>(raw);check(std::string(name)=="DebugSwitches.savegame","same Debug file");++f.opens;*out=f.handle;return f.failure;}
 static int close(void* raw,std::uintptr_t){++static_cast<Files*>(raw)->closes;return 0;}
 character::DebugFileServices24 services(){return {this,open,close};}
};
using DebugOwner=std::unique_ptr<character::DebugSwitches,decltype(&dh2_character_debug_destroy)>;
DebugOwner debug(){return DebugOwner(dh2_character_debug_create(),dh2_character_debug_destroy);}
std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),"current authored animation input");return {std::istreambuf_iterator<char>(f),{}};}
struct Policy {
 character::DebugSwitches* debug;character::CharacterAnimationSelectionDebugV126 native;
 data::AnimationSelectionPolicyServicesV126 actual;std::vector<unsigned> trace;
 int mutate=-1;bool fail_query{};
 Policy(character::DebugSwitches* d,const character::DebugFileServices24* f):debug(d),native(d,f),actual(native.services()){}
 static bool tracing(void* raw,std::string& e){auto& p=*static_cast<Policy*>(raw);p.trace.push_back(1);return p.actual.trace(p.actual.context,e);}
 static bool random(void* raw,bool& enabled,std::string& e){auto& p=*static_cast<Policy*>(raw);p.trace.push_back(3);return p.actual.random_enabled(p.actual.context,enabled,e);}
 static void event(void* raw,data::AnimationScheduler&,unsigned event){auto& p=*static_cast<Policy*>(raw);p.trace.push_back(event==0x24?2:4);if(event==0x24){if(p.mutate>=0)p.debug->switches["MP_MinimalRandoms"]=static_cast<std::uint8_t>(p.mutate);if(p.fail_query)p.debug->failure=-2;}}
 static bool prepare(void* raw,data::AnimationScheduler&){static_cast<Policy*>(raw)->trace.push_back(5);return true;}
 data::AnimationSelectionPolicyServicesV126 services(){return {this,tracing,random};}
};
struct StateFixture {
 Files files;character::DebugFileServices24 file_services;DebugOwner actual_debug;
 character::StateOwnerDebugDiagnostics diagnostics;
 character::Facts facts{};character::Services bodies{this,body};character::StateOwnerBehaviorPredicate8 predicates{};
 character::PreSpawnState48 pre{};character::PreSpawnServices16 pre_services{this,spawn};std::int32_t rows[1][40]{};
 std::unique_ptr<character::CharacterWorldNpcStateOwnerV1> npc;std::vector<unsigned> calls;
 explicit StateFixture(int file_failure=0,bool existing=false):file_services(files.services()),actual_debug(debug()),diagnostics(*actual_debug,file_services){
  files.failure=file_failure;files.handle=existing?17:0;rows[0][25]=777;rows[0][32]=778;facts.idle=11;
  character::WorldNpcStateServicesV1 s;s.facts=&facts;s.bodies=&bodies;s.predicates=&predicates;s.pre_spawn=&pre;s.spawn_services=&pre_services;
  s.diagnostics=&diagnostics;s.diagnostics_required=1;s.remaining_methods={this,remaining};
  npc=std::make_unique<character::CharacterWorldNpcStateOwnerV1>(0x1818,s);
  pre={&npc->state(),0x1818,rows,1,0,0,0,nullptr};
 }
 static void body(void* raw,character::State*,const character::Request* q){static_cast<StateFixture*>(raw)->calls.push_back(q->service);}
 static int remaining(void*,character::StateOwnerMachine40*,const character::StateOwnerRequest48* q,character::StateOwnerResponse8*){return q->operation==character::state_owner_character_event?0:1;}
 static int spawn(void* raw,character::PreSpawnState48*,const character::PreSpawnRequest32* q,character::PreSpawnResponse8* out){auto& f=*static_cast<StateFixture*>(raw);check(f.diagnostics.queries()&&f.diagnostics.destructions()==f.diagnostics.queries(),"PreSpawn trace completes before body");f.calls.push_back(100+q->service);if(q->service==character::pre_spawn_animation_index)out->word=0;return 0;}
 void stage17(){npc->state().current=17;npc->owner().machine().current_index=17;npc->native_fsm().current_present=1;npc->state().flags=0xabcdef;}
};
}
int main(int argc,char** argv){try{
 check(argc==2,"authored data directory");const std::string root=argv[1];
 auto names=read(root+"/animations_dictionary_pyarraynames.bin"),paths=read(root+"/animations_dictionary_pyarray.bin");data::Dictionary dictionary;std::string error;
 check(data::load_dictionary({names.data(),names.size()},{paths.data(),paths.size()},dictionary,error),error.c_str());
 auto records=read(root+"/animations_pyarray.bin"),keys=read(root+"/animations_pyarraynames.bin"),fields=read(root+"/animations_pystructnames.bin");data::AnimationTables tables;
 check(data::load_animation_tables({records.data(),records.size()},{keys.data(),keys.size()},{fields.data(),fields.size()},dictionary,tables,error),error.c_str());
 unsigned authored_routes{};
 for(const auto row:{62u,64u}){
  check(tables.character_names.at(row)==(row==62?"Skeleton":"Slime"),"authored Character table identity");
  const int sequence=tables.characters.at(row).fields[9].at(0);check(sequence==(row==62?596:613)&&tables.sequences.at(sequence).type==2,"current authored Idle type2");
  for(unsigned minimal=0;minimal<2;++minimal){
   Files files;auto file_services=files.services();auto owner=debug();check(dh2_character_debug_load(owner.get(),&file_services)==1,"same source Debug load");owner->switches["MP_MinimalRandoms"]=static_cast<std::uint8_t>(minimal);
   Policy p(owner.get(),&file_services);auto policy=p.services();data::AnimationScheduler scheduler;data::AnimationRandom random{123456789,0},expected=random;
   const auto step=minimal?0:dh2_animation_random(&expected.seed,&expected.calls,tables.sequences[sequence].steps.size());
   const data::AnimationSelectionServices services{&p,Policy::event,Policy::prepare,&policy,&error};
   check(scheduler.start_with_services(tables,sequence,random,error,services),error.c_str());
   check(scheduler.frames().back().step==step&&random.seed==expected.seed&&random.calls==expected.calls,"authored step and SAME RNG receipt");
   check(p.trace==std::vector<unsigned>({1,2,3,4,5}),"metadata trace event36 policy event38 prepare order");++authored_routes;
  }
 }
 for(const auto mutate:{0,1}){
  Files files;auto f=files.services();auto d=debug();check(dh2_character_debug_load(d.get(),&f)==1,"loaded Debug for live callback");d->switches["MP_MinimalRandoms"]=1-mutate;
  Policy p(d.get(),&f);p.mutate=mutate;auto policy=p.services();data::AnimationScheduler scheduler;data::AnimationRandom random{123456789,0};const data::AnimationSelectionServices s{&p,Policy::event,Policy::prepare,&policy,&error};
  check(scheduler.start_with_services(tables,596,random,error,s),"live switch policy");check(random.calls==unsigned(!mutate),"read policy AFTER event36");
 }
 {auto fixed=tables;fixed.sequences[596].type=0;Files files;auto f=files.services();auto d=debug();Policy p(d.get(),&f);auto policy=p.services();data::AnimationScheduler scheduler;data::AnimationRandom random{77,0};const data::AnimationSelectionServices s{&p,Policy::event,Policy::prepare,&policy,&error};
  check(scheduler.start_with_services(fixed,596,random,error,s),"nonrandom authored row");check(p.trace==std::vector<unsigned>({1,2,4,5})&&d->switches.count("MP_MinimalRandoms")==0&&random.seed==77&&!random.calls,"non-type2 never queries random policy");}
 // Redirects and repeats must reenter the original per-occurrence prefix.
 for(const auto minimal:{0,1}){
  auto synthetic=tables;synthetic.sequences.resize(2);for(auto& s:synthetic.sequences){s.type=2;s.loop=-1;s.steps.resize(1);s.steps[0].anim=19;s.steps[0].redir=0;}
  synthetic.sequences[0].steps[0].anim=1;synthetic.sequences[0].steps[0].redir=1;
  Files files;auto f=files.services();auto d=debug();check(dh2_character_debug_load(d.get(),&f)==1,"redirect Debug");d->switches["MP_MinimalRandoms"]=minimal;Policy p(d.get(),&f);auto policy=p.services();data::AnimationScheduler scheduler;data::AnimationRandom random{77,0};const data::AnimationSelectionServices s{&p,Policy::event,Policy::prepare,&policy,&error};
  check(scheduler.start_with_services(synthetic,0,random,error,s),"redirect policy");check(random.calls==unsigned(minimal?0:2),"per-redirect RNG");check(p.trace==std::vector<unsigned>({1,2,3,4,1,2,3,4,5}),"per-redirect trace/query order");
  const auto before=random.calls;p.trace.clear();data::AnimationCompletionServices completion;completion.selection=&s;
  check(scheduler.complete_with_services(synthetic,random,error,completion),"repeat policy");check(random.calls==before+unsigned(!minimal),"repeat RNG policy");check(p.trace==std::vector<unsigned>({1,2,3,4,5}),"repeat source prefix");
 }
 // A genuine trace failure leaves the original metadata prefix but no
 // selection callbacks, step effects, or Random consumption.
 {Files files;files.failure=1;auto f=files.services();auto d=debug();Policy p(d.get(),&f);auto policy=p.services();data::AnimationScheduler scheduler;data::AnimationRandom random{77,0};const data::AnimationSelectionServices s{&p,Policy::event,Policy::prepare,&policy,&error};check(!scheduler.start_with_services(tables,596,random,error,s),"animation trace failure");check(p.trace==std::vector<unsigned>({1})&&random.seed==77&&!random.calls&&scheduler.frames().back().sequence==596&&!scheduler.active(),"trace failure retains ONLY original metadata prefix");}
 {Files files;auto f=files.services();auto d=debug();Policy p(d.get(),&f);p.fail_query=true;auto policy=p.services();data::AnimationScheduler scheduler;data::AnimationRandom random{77,0};const data::AnimationSelectionServices s{&p,Policy::event,Policy::prepare,&policy,&error};check(!scheduler.start_with_services(tables,596,random,error,s),"post-event query failure");check(p.trace==std::vector<unsigned>({1,2,3})&&random.seed==77&&!random.calls&&!scheduler.active(),"policy failure stops before RNG/event38");}
 for(const auto failure:{0,1})for(const bool blur:{false,true}){
  StateFixture f(failure,failure==0);if(blur)f.stage17();else f.npc->state().flags=0xabcdef;const auto before=f.npc->state();
  check((blur?f.npc->transition(3,-1,0):f.npc->initialize_level_preset_v95(17))<0,"PreSpawn tracing failure");
  check(f.calls.empty()&&f.npc->state().flags==before.flags&&f.npc->state().body_present==before.body_present,"PreSpawn trace failure before state/body effects");
  check(f.files.opens==1&&f.files.closes==unsigned(!failure),"actual failed/existing Debug file prefix");
 }
 {StateFixture f;check(f.npc->initialize_level_preset_v95(17)>0,"authentic PreSpawn Focus");check(f.npc->state().flags==0x1300&&f.diagnostics.loads()==1&&f.diagnostics.queries()==1&&f.diagnostics.destructions()==1,"Focus source trace receipt");f.calls.clear();check(f.npc->transition(3,-1,0)>0,"authentic PreSpawn Blur plus Idle Focus");check(f.diagnostics.loads()==3&&f.diagnostics.queries()==3&&f.diagnostics.destructions()==3&&f.npc->state().flags==0x2380,"Blur source trace receipt");}
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"authored_Skeleton_Slime_policy_routes\":"<<authored_routes<<",\"live_event36_policy_mutations\":2,\"redirect_repeat_policies\":2,\"PreSpawn_trace_failure_prefixes\":4,\"same_real_Debug_map_fixture\":true,\"device_launch\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
