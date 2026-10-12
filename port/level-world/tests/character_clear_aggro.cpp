// Reuse frozen integration fixtures without changing their historical proof.
#define main historical_target_pipeline_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_target_pipeline.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_clear_aggro.hpp"
namespace {
using Words=std::vector<std::uint32_t>;
unsigned source_records=0,source_trace_calls=0,word_checks=0,guards=0;
std::uintptr_t identity(int index){return index<0?0:UINT64_C(0x100000001)+index;}
std::uint32_t index(std::uintptr_t id){return id?static_cast<std::uint32_t>(id-UINT64_C(0x100000001)):UINT32_MAX;}
Words read_words(std::istream& in,unsigned count){Words result;for(unsigned i=0;i<count;++i)result.push_back(word(in));return result;}
struct Replay {
 TargetOwner16 owners[4]{};TargetState48 receiver{},other{};TargetBindings48 binding{};
 dh2::data::AggroEntry outentries[4]{},inentries[4]{};dh2::data::AggroTable outgoing{outentries,0,4},incoming{inentries,0,4};
 ClearAggroState16 state{&receiver,&outgoing};ClearAggroCharacter32 target{};ClearAggroServices24 services{this,call,resolve};ClearAggroBindings16 bindings{&state,&services};
 std::vector<Words> trace;unsigned mutation=0;int fail=-1;bool invalidate=false;
 Replay(){for(int i=0;i<4;++i)owners[i].identity=identity(i);receiver.owner=&owners[0];other.owner=&owners[1];other.identity=UINT64_C(0x200000002);binding={&other,{this,setter},nullptr,{0,0}};target={identity(1),&incoming,&binding,identity(1)};}
 Words snapshot()const{Words result{static_cast<std::uint32_t>(receiver.owner-owners),index(other.target),outgoing.count};for(unsigned i=0;i<3;++i){result.push_back(i<outgoing.count?index(outentries[i].character):0);result.push_back(i<outgoing.count?outentries[i].threat_bits:0);}result.push_back(incoming.count);result.push_back(incoming.count?index(inentries[0].character):0);result.push_back(incoming.count?inentries[0].threat_bits:0);return result;}
 void stamp(unsigned op,unsigned subject,unsigned argument){Words w{op,subject,argument};auto s=snapshot();w.insert(w.end(),s.begin(),s.end());trace.push_back(w);}
 static int setter(void* p,TargetState48*,const TargetRequest24* request,std::uint32_t* out){auto& r=*static_cast<Replay*>(p);check(request->service<=1);if(!request->service)r.stamp(1,1,UINT32_MAX);*out=0;return 0;}
 static int call(void* p,ClearAggroState16*,ClearAggroCharacter32*,const ClearAggroRequest32* request){auto& r=*static_cast<Replay*>(p);check(!request->scope);r.stamp(request->service==clear_aggro_notify?0:2,request->service==clear_aggro_notify?1:index(request->receiver),request->service==clear_aggro_notify?index(request->other):UINT32_MAX);
  if(r.fail==static_cast<int>(request->service))return 1;
  if(request->service==clear_aggro_notify){if(r.mutation==1)r.other.target=0;else if(r.mutation==2)r.receiver.owner=&r.owners[2];else if(r.mutation==3)r.target.controller=identity(3);if(r.invalidate)r.state.receiver=reinterpret_cast<TargetState48*>(1);}return 0;}
 static int resolve(void* p,std::uintptr_t id,ClearAggroCharacter32** out){auto& r=*static_cast<Replay*>(p);if(id!=r.target.identity||r.fail==2)return 1;*out=&r.target;return 0;}
 void reset(const Words& before,unsigned mutation){this->mutation=mutation;fail=-1;invalidate=false;trace.clear();state.receiver=&receiver;receiver.owner=&owners[before[0]];other.target=identity(static_cast<std::int32_t>(before[1]));target.controller=identity(1);outgoing.count=before[2];for(unsigned i=0;i<outgoing.count;++i)outentries[i]={identity(before[3+i*2]),before[4+i*2],0};incoming.count=before[9];if(incoming.count)inentries[0]={identity(before[10]),before[11],0};binding.scope=nullptr;}
};
struct LuaAggro {
 Fixture& fixture;dh2::data::AggroEntry outgoing_entries[4]{},incoming_entries[4]{};
 dh2::data::AggroTable outgoing{outgoing_entries,0,4},incoming{incoming_entries,0,4};
 ClearAggroState16 state;ClearAggroCharacter32 target;ClearAggroServices24 services{this,call,resolve};ClearAggroBindings16 bindings{&state,&services};
 unsigned notices=0,stops=0,nested=0;bool recurse=false,fail_notify=false;
 explicit LuaAggro(Fixture& f):fixture(f),state{&f.owner->target,&outgoing},target{f.enemy->identity,&incoming,&f.enemy->binding,0x801}{}
 void seed(){fixture.seed();check(!dh2_character_ai_set_target(&fixture.enemy->target,fixture.owner->identity,0,&fixture.enemy->binding.services));outgoing.count=incoming.count=1;outgoing_entries[0]={fixture.enemy->identity,0x41200000,0};incoming_entries[0]={fixture.owner->identity,0x41200000,0};fixture.trace.clear();}
 static int resolve(void* p,std::uintptr_t id,ClearAggroCharacter32** out){auto& a=*static_cast<LuaAggro*>(p);if(!a.fixture.objects.find(id)||id!=a.target.identity)return 1;*out=&a.target;return 0;}
 static int call(void* p,ClearAggroState16*,ClearAggroCharacter32*,const ClearAggroRequest32* r){auto& a=*static_cast<LuaAggro*>(p);auto& f=a.fixture;
  if(r->service==clear_aggro_notify){check(r->receiver==f.enemy->target.identity&&r->other==f.owner->identity&&!a.outgoing.count&&!a.incoming.count);++a.notices;f.trace.push_back("native.OnDeAggro.fixture");if(a.fail_notify)return 1;
   if(a.recurse){a.recurse=false;check(r->scope&&dh2_script_callback_scope_valid(r->scope));auto old=f.scope;f.scope=r->scope;check(!f.dispatch(0xd));f.scope=old;check(dh2_script_callback_scope_valid(r->scope));++a.nested;}
  }else{check(r->receiver==a.target.controller&&!f.enemy->target.target);++a.stops;f.trace.push_back("native.CmdStop.fixture");}return 0;
 }
 static int prefix(void* p,TargetEventState32* s,const TargetEventRequest64* r,TargetEventResponse24* out){auto& a=*static_cast<LuaAggro*>(p);if(r->service==target_event_clear_aggro){a.fixture.trace.push_back("native.ClearAggro.prefix");check(r->other==a.target.identity);return dh2_character_clear_aggro(&a.state,&a.target,&a.services,nullptr);}return Fixture::target_call(&a.fixture,s,r,out);}
};
}
int main(int argc,char** argv){try{
 check(argc==6);std::ifstream gold(argv[1],std::ios::binary);check(word(gold)==0x31414743);auto count=word(gold);check(count==1792);Replay replay;
 for(unsigned n=0;n<count;++n){auto input=read_words(gold,8),before=read_words(gold,12),after=read_words(gold,12);auto calls=word(gold);std::vector<Words> trace;for(unsigned j=0;j<calls;++j)trace.push_back(read_words(gold,15));replay.reset(before,input[6]);dh2_script_value values[3]{};for(auto& v:values){v.type=input[0];v.identity=input[2]?identity(1):0;}
  auto result=input[7]?dh2_character_clear_aggro_values(&replay.bindings,nullptr,values,input[1]):dh2_character_clear_aggro(&replay.state,input[2]?&replay.target:nullptr,&replay.services,nullptr);check(!result&&replay.snapshot()==after&&replay.trace==trace&&!replay.binding.scope);++source_records;source_trace_calls+=calls;word_checks+=12+15*calls;}
 check(gold.peek()==EOF);
 Words before{0,0,1,1,0x41200000,0,0,0,0,1,0,0x41200000};replay.reset(before,0);auto entry=replay.snapshot();
 check(dh2_character_clear_aggro(nullptr,&replay.target,&replay.services,nullptr)==1);++guards;
 check(dh2_character_clear_aggro(&replay.state,nullptr,nullptr,nullptr)==0);++guards;
 replay.target.identity=0;check(dh2_character_clear_aggro(&replay.state,&replay.target,&replay.services,nullptr)==1&&replay.snapshot()==entry);replay.target.identity=identity(1);++guards;
 replay.incoming.count=5;check(dh2_character_clear_aggro(&replay.state,&replay.target,&replay.services,nullptr)==1&&replay.outgoing.count==1);replay.incoming.count=1;++guards;
 dh2_script_value value{};value.type=7;value.identity=identity(1);value.reserved=1;check(dh2_character_clear_aggro_values(&replay.bindings,nullptr,&value,1)==1);value.reserved=0;++guards;
 alignas(8)char bad[48]{};check(dh2_character_clear_aggro_values(&replay.bindings,nullptr,reinterpret_cast<dh2_script_value*>(bad+1),1)==1);++guards;
 replay.fail=2;check(dh2_character_clear_aggro_values(&replay.bindings,nullptr,&value,1)==2&&replay.outgoing.count==1);++guards;
 replay.reset(before,0);replay.fail=0;check(dh2_character_clear_aggro(&replay.state,&replay.target,&replay.services,nullptr)==2&&!replay.outgoing.count&&!replay.incoming.count&&replay.other.target==identity(0));++guards;
 replay.reset(before,0);replay.fail=1;check(dh2_character_clear_aggro(&replay.state,&replay.target,&replay.services,nullptr)==2&&!replay.other.target&&!replay.binding.scope);++guards;
 replay.reset(before,0);replay.invalidate=true;check(dh2_character_clear_aggro(&replay.state,&replay.target,&replay.services,nullptr)==2);++guards;replay.state.receiver=&replay.receiver;
 Inputs inputs(argv[2]);auto common=file(argv[3]),monster=file(argv[4]);std::ifstream threat_gold(argv[5],std::ios::binary);check(word(threat_gold)==0x31534543&&word(threat_gold)==409);auto threat=word(threat_gold);CharacterGameDesign design;std::string error;check(design.initialize(inputs.input,error));Fixture f(design);f.threat=threat;f.create(design,common,monster);LuaAggro aggro(f);check(!dh2_character_clear_aggro_bind(f.view.vm,&aggro.bindings));
 aggro.seed();f.lua("assert(select('#',ClearAggro())==0 and select('#',ClearAggro(nil))==0); assert(HasTarget()); saved=GetTarget()");check(aggro.outgoing.count==1);
 f.target_services.context=&aggro;f.target_services.invoke=LuaAggro::prefix;check(!f.dispatch(0xc));check(!aggro.outgoing.count&&!aggro.incoming.count&&!f.owner->target.target&&!f.owner->target.last_target&&!f.enemy->target.target&&aggro.notices==1&&aggro.stops==1);f.clean_scope();check(!f.enemy->binding.scope);
 check(at(f.trace,"native.OnDeAggro.fixture")<at(f.trace,"Stop.fixture")&&at(f.trace,"native.CmdStop.fixture")<at(f.trace,"Stop.fixture"));
 aggro.seed();aggro.recurse=true;f.lua("assert(select('#',ClearAggro(saved, 'ignored'))==0); assert(HasTarget())");check(aggro.nested==1&&!aggro.outgoing.count&&!f.enemy->binding.scope);f.clean_scope();
 aggro.seed();aggro.fail_notify=true;f.lua("local ok,err=pcall(ClearAggro,saved); assert(not ok and string.find(err,'Native ClearAggro delivery failed')); assert(HasTarget())");check(!aggro.outgoing.count&&!aggro.incoming.count&&f.enemy->target.target==f.owner->identity&&!f.enemy->binding.scope);aggro.fail_notify=false;f.clean_scope();
 aggro.seed();auto retained=f.enemy;f.lua("proxy=newproxy(true); getmetatable(proxy).__gc=function() ClearAggro(saved); ClearTarget() end");f.session.reset();check(!aggro.outgoing.count&&!aggro.incoming.count&&!f.owner->target.target&&!retained->target.target&&!retained->binding.scope);
 Dl_info lib{};check(dladdr(reinterpret_cast<void*>(dh2_character_clear_aggro),&lib));std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"source_records\":"<<source_records<<",\"ordered_calls\":"<<source_trace_calls<<",\"word_checks\":"<<word_checks<<",\"guards\":"<<guards<<",\"actual_monster_OutSight\":1,\"nested_VM_events\":"<<aggro.nested<<",\"genuine_Lua_ClearAggro\":true,\"notification_fixture_calls\":"<<aggro.notices<<",\"Stop_fixture_calls\":"<<aggro.stops<<",\"finalizer_clear\":true,\"library\":\""<<lib.dli_fname<<"\",\"full_enemy_AI\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
