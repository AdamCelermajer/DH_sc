#include "../character_spawn_body.hpp"
#include "../character_target_bindings.hpp"
#include "../character_design_services.hpp"
#include <array>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
static void check(bool x){if(!x)throw std::runtime_error("Spawn1 audit mismatch");}
template<class T>T read(std::ifstream& f){T v{};f.read(reinterpret_cast<char*>(&v),sizeof v);check(bool(f));return v;}
struct Replay {
 State state{};std::int32_t rows[2][2][40]{};SpawnBody48 view{};SpawnBodyServices16 services{this,invoke};std::array<unsigned,16> p{};std::vector<SpawnBodyRequest32> calls;unsigned target=1,last=1;int failure=-1;bool nested=false;
 static int invoke(void* ctx,SpawnBody48* s,const SpawnBodyRequest32* r,unsigned* out){auto& f=*static_cast<Replay*>(ctx);check(r->character==0xa123456789abcdefull&&!r->argument1&&!r->argument2);f.calls.push_back(*r);if(int(r->service)==f.failure)return 1;
  const auto k=r->service,m=f.p[6];
  if(m==1&&k==spawn_animation_index)s->animation_rows=f.rows[1];
  if(m==2&&k==spawn_stance_mask)for(auto& table:f.rows)table[f.p[7]][32]=999;
  if(m==3&&k==spawn_set_animation)f.state.flags=0x12340000;
  if(m==4&&k==spawn_cancel_sneaking){f.state.flags=0x80004001;s->visual=f.p[5]?0:0xb123456789abcdefull;s->fade_word=0x80000000;}
  if(m==5&&k==spawn_debug_query&&!r->argument0){f.state.flags=0x2000;s->animation_rows=f.rows[1];}
  if(m==6&&k==spawn_debug_load)s->character=0xd123456789abcdefull;
  switch(k){
   case spawn_debug_query:*out=~0u;break;
   case spawn_animation_index:*out=f.p[7];break;
   case spawn_stance_mask:*out=f.p[3];break;
   case spawn_stance:*out=f.p[4];break;
   case spawn_set_animation:f.state.current_animation=static_cast<int>(r->argument0);if(f.nested){f.nested=false;check(dh2_character_spawn_body(s,state_method_event,0,0x28,reinterpret_cast<std::uintptr_t>("is_interactive"),&f.services)==1);}break;
   case spawn_set_target:f.target=0;break;
   case spawn_sync_last_target:f.last=f.target;break;
   case spawn_init_physical:f.state.body_present=1;break;
   case spawn_fade_in:check(dh2_character_spawn_fade_in_empty()==1);break;
   default:break;
  }
  return 0;
 }
 void init(const std::array<unsigned,16>& params){p=params;calls.clear();failure=-1;nested=false;target=p[10];last=p[11];state={};state.current=1;state.flags=p[2];state.body_present=p[12];for(unsigned k=0;k<2;++k)for(unsigned i=0;i<2;++i){for(auto& v:rows[k][i])v=0;rows[k][i][32]=static_cast<int>(p[8]+k*100+i);}view={&state,0xa123456789abcdefull,rows[0],p[5]?0xb123456789abcdefull:0,2,p[9],{0,0}};}
};
struct NativeTargetDebug {
 Replay body;DebugSwitches* debug=dh2_character_debug_create();unsigned missing_file=0,target_callbacks=0,required_fixture_deliveries=0;
 DebugFileServices24 files{this,open,close};TargetOwner16 owner{0xa123456789abcdefull,73,0,0};TargetState48 target{0xd123456789abcdefull,&owner,0xb123456789abcdefull,0xb123456789abcdefull,0xc123456789abcdefull,1,1,1,0,0};TargetServices16 target_services{this,target_call};SpawnBodyServices16 services{this,body_call};
 ~NativeTargetDebug(){dh2_character_debug_destroy(debug);}
 static int open(void* ctx,const char* name,std::uintptr_t* handle){auto& f=*static_cast<NativeTargetDebug*>(ctx);check(!std::strcmp(name,"DebugSwitches.savegame"));++f.missing_file;*handle=0;return 0;} // Explicit missing-file projection fixture.
 static int close(void*,std::uintptr_t){throw std::runtime_error("missing-file fixture cannot close a handle");}
 static int target_call(void* ctx,TargetState48*,const TargetRequest24* r,unsigned* out){auto& f=*static_cast<NativeTargetDebug*>(ctx);++f.target_callbacks;if(r->service==target_debug_load)return dh2_character_debug_load(f.debug,&f.files)==1?0:1;if(r->service==target_debug_query)return dh2_character_debug_get(out,f.debug,r->text,&f.files)==1?0:1;return 1;}
 static int body_call(void* ctx,SpawnBody48* s,const SpawnBodyRequest32* r,unsigned* out){auto& f=*static_cast<NativeTargetDebug*>(ctx);f.body.calls.push_back(*r);
  switch(r->service){
   case spawn_debug_load:return dh2_character_debug_load(f.debug,&f.files)==1?0:1;
   case spawn_debug_query:return dh2_character_debug_get(out,f.debug,r->argument0?"isTracingCSSpawn":"isTracingCharState",&f.files)==1?0:1;
   case spawn_animation_index:*out=0;return 0;
   case spawn_stance_mask:*out=1;return 0;
   case spawn_stance:*out=4;return 0;
   case spawn_set_animation:s->state->current_animation=static_cast<int>(r->argument0);++f.required_fixture_deliveries;return 0;
   case spawn_set_target:return dh2_character_ai_set_target(&f.target,0,0,&f.target_services);
   case spawn_sync_last_target:return dh2_character_ai_sync_last_target(&f.target);
   case spawn_fade_in:return dh2_character_spawn_fade_in_empty()==1?0:1;
   case spawn_cancel_sneaking:++f.required_fixture_deliveries;return 0;
   case spawn_init_physical:s->state->body_present=1;++f.required_fixture_deliveries;return 0;
   default:return 1;
  }
 }
};
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream file(argv[1],std::ios::binary);check(read<unsigned>(file)==0x31425053);const auto count=read<unsigned>(file);Replay f;unsigned requests=0,guards=0;const char* strings[]={"","is_interactive","Is_interactive","body","is_interactive_more"};
 for(unsigned i=0;i<count;++i){const auto p=read<std::array<unsigned,16>>(file);const auto expected=read<State>(file);const auto projection=read<std::array<unsigned,4>>(file);const auto calls=read<unsigned>(file);std::vector<SpawnBodyRequest32> gold(calls);for(auto& q:gold)q=read<SpawnBodyRequest32>(file);f.init(p);const auto payload=p[13]==0x28?reinterpret_cast<std::uintptr_t>(strings[p[14]]):0xb123456789abcdefull;check(dh2_character_spawn_body(&f.view,p[0],static_cast<int>(p[1]),p[13],payload,&f.services)==1);const std::array<unsigned,4> got{static_cast<unsigned>(bool(f.view.visual)),f.view.fade_word,f.target,f.last};check(!std::memcmp(&f.state,&expected,56)&&got==projection&&f.calls.size()==gold.size());for(unsigned j=0;j<calls;++j)check(!std::memcmp(&f.calls[j],&gold[j],32));requests+=calls;}
 check(file.peek()==std::ifstream::traits_type::eof());const std::array<unsigned,16> p{0,17,0x2000,1,4,1,0,0,800,0x3f800000,1,1,0,0,0,0};f.init(p);
 auto reject=[&](SpawnBody48 v,const SpawnBodyServices16* s){const auto before=f.state;check(dh2_character_spawn_body(&v,0,17,0,0,s)==-1&&!std::memcmp(&f.state,&before,56)&&f.calls.empty());++guards;};
 auto bad=f.view;bad.state=nullptr;reject(bad,&f.services);bad=f.view;bad.character=0;reject(bad,&f.services);bad=f.view;bad.animation_rows=nullptr;reject(bad,&f.services);bad=f.view;bad.animation_count=0;reject(bad,&f.services);bad=f.view;bad.reserved[0]=1;reject(bad,&f.services);bad=f.view;bad.reserved[1]=1;reject(bad,&f.services);reject(f.view,nullptr);auto missing=f.services;missing.invoke=nullptr;reject(f.view,&missing);f.state.current=17;reject(f.view,&f.services);f.state.current=1;check(dh2_character_spawn_body(&f.view,4,17,0,0,&f.services)==-1&&f.calls.empty());++guards;
 for(unsigned failed=0;failed<=spawn_fade_in;++failed){f.init(p);f.failure=failed;check(dh2_character_spawn_body(&f.view,0,17,0,0,&f.services)==-2&&f.calls.back().service==failed);if(failed<=spawn_debug_query)check(f.state.flags==0x2000);else check(f.state.flags==0x241||(failed==spawn_fade_in&&f.state.flags==0x2241));++guards;}
 f.init(p);f.p[7]=99;check(dh2_character_spawn_body(&f.view,0,17,0,0,&f.services)==-3&&f.state.flags==0x241&&f.calls.back().service==spawn_animation_index);++guards;
 f.init(p);check(dh2_character_spawn_body(&f.view,3,0,0x28,0,&f.services)==-3&&f.calls.empty());++guards;
 f.init(p);f.state.flags=0;f.failure=spawn_init_physical;check(dh2_character_spawn_body(&f.view,3,0,0x28,reinterpret_cast<std::uintptr_t>("is_interactive"),&f.services)==-2&&f.state.flags==0x2000);++guards;
 f.init(p);f.nested=true;check(dh2_character_spawn_body(&f.view,0,-1,0,0,&f.services)==1&&f.state.flags==0x2241&&f.state.body_present==1);const auto prior=f.calls.size();check(dh2_character_spawn_body(&f.view,1,0,0,0,&f.services)==1&&f.calls.size()==prior+2); // Blur sees marker and skips physical creation.
 NativeTargetDebug genuine;check(genuine.debug);genuine.body.init(p);check(dh2_character_spawn_body(&genuine.body.view,0,17,0,0,&genuine.services)==1);check(genuine.body.state.flags==0x2241&&genuine.body.state.current_animation==804&&!genuine.target.candidate&&!genuine.target.target&&!genuine.target.last_target&&genuine.owner.word14d0==0&&genuine.target.alive==1&&genuine.target.sight==1&&genuine.target.changed==1&&genuine.target_callbacks==2&&genuine.missing_file==1);unsigned loaded=0,keys=0;check(dh2_character_debug_snapshot(genuine.debug,&loaded,&keys)==1&&loaded==1&&keys==9);
 std::printf("{\"validation\":\"PASS\",\"original_cases\":%u,\"original_ordered_requests\":%u,\"guard_prefix_source_invalid_checks\":%u,\"nested_interactive_and_blur_checks\":1,\"genuine_native_target_set_sync_compositions\":1,\"genuine_DebugSwitches_map_keys\":%u,\"DebugSwitches_missing_file_fixture_deliveries\":%u,\"required_animation_CancelSneaking_fixture_deliveries\":%u,\"mismatches\":0}\n",count,requests,guards,keys,genuine.missing_file,genuine.required_fixture_deliveries);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
