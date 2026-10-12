#include "../character_pre_spawn.hpp"
#include <array>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
static void check(bool x){if(!x)throw std::runtime_error("pre-spawn mismatch");}
template<class T>T read(std::ifstream& f){T v{};f.read(reinterpret_cast<char*>(&v),sizeof v);check(bool(f));return v;}
struct Fixture {
 std::array<std::uint32_t,20> p{};std::int32_t rows[2][2][40]{};State state{};std::uint32_t word=77;PreSpawnState48 view{};PreSpawnServices16 services{};std::vector<PreSpawnRequest32> calls;unsigned index=0;int fail=-1;bool reenter=false;
 static int invoke(void* opaque,PreSpawnState48* s,const PreSpawnRequest32* r,PreSpawnResponse8* out){auto& f=*static_cast<Fixture*>(opaque);check(r->character==0xa123456789abcdefull&&!r->argument2);f.calls.push_back(*r);
  switch(r->service){
  case pre_spawn_animation_index:++f.index;out->word=f.p[f.index==1?10:11];if(f.p[12]==1)s->animation_rows=f.rows[f.index%2];break;
  case pre_spawn_stance_mask:out->word=f.p[8];if(f.p[12]==2)for(auto& row:f.rows)row[0][32]=999;break;
  case pre_spawn_stance:out->word=f.p[9];break;
  case pre_spawn_set_animation:f.state.current_animation=static_cast<std::int32_t>(r->argument0);if(f.reenter){f.reenter=false;check(dh2_character_pre_spawn_body(s,state_method_event,0x28,reinterpret_cast<std::uintptr_t>("is_interactive"),&f.services)==1);}break;
  case pre_spawn_remove_physical:f.state.body_present=0;if(f.p[12]==3)s->stay_enabled=0;break;
  case pre_spawn_predicate:out->word=f.p[13];out->next=static_cast<std::int32_t>(f.p[14]);break;
  case pre_spawn_set_spawn:if(f.p[12]==4)s->ai_kind=3;break;
  case pre_spawn_assert_policy:out->word=f.p[15];break;
  default:break;
  }
  return int(r->service)==f.fail?1:0;
 }
 void init(const std::array<std::uint32_t,20>& params){p=params;index=0;fail=-1;reenter=false;calls.clear();state=State{};state.current=17;state.flags=p[2];state.body_present=p[16];word=p[5];for(unsigned k=0;k<2;++k)for(unsigned i=0;i<2;++i){for(auto& v:rows[k][i])v=0;rows[k][i][25]=static_cast<std::int32_t>(p[6])==-1?-1:static_cast<std::int32_t>(p[6]+k*10+i);rows[k][i][32]=static_cast<std::int32_t>(p[7]+k*10+i);}view={&state,0xa123456789abcdefull,rows[0],2,p[3],p[4],0,&word};services={this,invoke};}
};
int main(int argc,char** argv){try{check(argc==2);std::ifstream file(argv[1],std::ios::binary);check(read<std::uint32_t>(file)==0x31505350);const auto count=read<std::uint32_t>(file);Fixture f;std::uint32_t requests=0,guards=0;const char* strings[]={"","is_interactive","Is_interactive","body","is_interactive_more"};
 for(unsigned i=0;i<count;++i){const auto params=read<std::array<std::uint32_t,20>>(file);const auto expected=read<State>(file);const auto expected_word=read<std::uint32_t>(file),result=read<std::uint32_t>(file),calls=read<std::uint32_t>(file);std::vector<PreSpawnRequest32> gold(calls);for(auto& r:gold)r=read<PreSpawnRequest32>(file);f.init(params);const auto payload=params[1]==0x28?reinterpret_cast<std::uintptr_t>(strings[params[17]]):0xb123456789abcdefull;check(dh2_character_pre_spawn_body(&f.view,params[0],params[1],payload,&f.services)==int(result));check(!std::memcmp(&f.state,&expected,sizeof expected)&&f.word==expected_word&&f.calls.size()==gold.size());for(unsigned j=0;j<calls;++j)check(!std::memcmp(&f.calls[j],&gold[j],sizeof gold[j]));requests+=calls;}
 check(file.peek()==std::ifstream::traits_type::eof());std::array<std::uint32_t,20> params{0,0,0xdeadbeef,0,0,77,0xffffffffu,800,1,4,0,1,0,1,1,0,1,0,0,0};f.init(params);
 auto reject=[&](PreSpawnState48 candidate,bool missing=false){const auto before=f.state;const auto calls=f.calls.size();check(dh2_character_pre_spawn_body(&candidate,0,0,0,missing?nullptr:&f.services)==-1&&!std::memcmp(&f.state,&before,sizeof before)&&f.calls.size()==calls);++guards;};
 auto bad=f.view;bad.state=nullptr;reject(bad);bad=f.view;bad.character=0;reject(bad);bad=f.view;bad.animation_rows=nullptr;reject(bad);bad=f.view;bad.animation_count=0;reject(bad);bad=f.view;bad.stay_enabled=256;reject(bad);bad=f.view;bad.reserved=1;reject(bad);reject(f.view,true);f.state.current=3;reject(f.view);f.state.current=17;check(dh2_character_pre_spawn_body(&f.view,4,0,0,&f.services)==-1);++guards;
 // Missing providers stop at their actual source prefix; completed fields stay.
 f.init(params);check(dh2_character_pre_spawn_body(&f.view,0,0,0,&f.services)==1);const auto focus_calls=f.calls;
 for(const auto& q:focus_calls){f.init(params);f.fail=q.service;check(dh2_character_pre_spawn_body(&f.view,0,0,0,&f.services)==-2&&f.state.flags==0x1300&&f.calls.back().service==q.service);++guards;}
 f.init(params);f.p[10]=99;check(dh2_character_pre_spawn_body(&f.view,0,0,0,&f.services)==-3&&f.calls.size()==1&&f.state.flags==0x1300);++guards;
 f.init(params);check(dh2_character_pre_spawn_body(&f.view,3,0x28,0,&f.services)==-3&&f.calls.empty());++guards;
 f.init(params);f.p[14]=0;f.p[15]=2;check(dh2_character_pre_spawn_body(&f.view,3,9,0xb123456789abcdefull,&f.services)==-3&&f.calls.size()==2);++guards;
 f.init(params);f.view.ai_kind=3;f.view.ai_word=nullptr;check(dh2_character_pre_spawn_body(&f.view,3,9,0xb123456789abcdefull,&f.services)==-3&&f.calls.size()==2);++guards;
 f.init(params);f.reenter=true;check(dh2_character_pre_spawn_body(&f.view,0,0,0,&f.services)==1&&f.state.flags==0x3300&&f.state.body_present==0);bool saw=false;for(const auto& q:f.calls)if(q.service==pre_spawn_init_physical)saw=true;check(saw);
 std::printf("{\"validation\":\"PASS\",\"original_cases\":%u,\"original_ordered_requests\":%u,\"guard_prefix_source_invalid_checks\":%u,\"nested_interactive_event_checks\":1,\"mismatches\":0}\n",count,requests,guards);return 0;}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
