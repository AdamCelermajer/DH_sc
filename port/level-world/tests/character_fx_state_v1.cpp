#include "../character_fx_state_v1.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <stdexcept>
using namespace dh2::fx;
struct Reader{std::ifstream f;explicit Reader(const char* p):f(p,std::ios::binary){if(!f)throw std::runtime_error(p);}template<class T>T get(){T v;f.read(reinterpret_cast<char*>(&v),sizeof(v));if(!f)throw std::runtime_error("fixture truncated");return v;}};
struct Call {std::uint32_t op,arg;std::uint64_t identity;std::uint32_t scalar;std::uint64_t payload;FxState96V1 state;};
struct Context {
 std::vector<Call> calls;std::uint32_t facts[5]{},kind{},mutation{},at{};std::size_t used{},fail_at{};unsigned failures{};bool mismatch{};
 static int invoke(void* p,FxState96V1* s,FxRequest32V1* q){auto& c=*static_cast<Context*>(p);if(c.used>=c.calls.size()){c.mismatch=true;return -1;}const auto& x=c.calls[c.used++];std::uint32_t f;std::memcpy(&f,&q->scalar,4);if(x.op!=std::uint32_t(q->operation)||x.arg!=q->argument||x.identity!=q->identity||x.scalar!=f||x.payload!=q->payload||q->reserved||std::memcmp(&x.state,s,96)){c.mismatch=true;return -1;}
  if(c.fail_at==c.used)return -1;
  if(c.mutation&&x.op==(c.kind==0||c.kind==3?1u:7u)&&c.used==(c.kind==0||c.kind==3?1u:2u)){s->loop=9;s->callback=0x2222222233333333ull;s->orient_once=7;}
  if(x.op==9)q->argument=c.facts[0];else if(x.op==10)q->argument=c.facts[1];else if(x.op==12)q->argument=c.facts[2];else if(x.op==16)q->argument=c.facts[3];else if(x.op==11)q->argument=c.facts[4];return 0;
 }
};
int main(int argc,char** argv){try{if(argc!=2)throw std::runtime_error("gold path required");Reader r(argv[1]);if(r.get<std::uint32_t>()!=0x31535846)throw std::runtime_error("gold magic");auto count=r.get<std::uint32_t>();unsigned requests=0,guards=0,prefixes=0;
 for(unsigned i=0;i<count;++i){Context c;c.kind=r.get<std::uint32_t>();auto initial=r.get<FxState96V1>();auto d=r.get<FxData32V1>();for(auto& f:c.facts)f=r.get<std::uint32_t>();c.mutation=r.get<std::uint32_t>();auto expected=r.get<FxState96V1>();auto n=r.get<std::uint32_t>();for(unsigned j=0;j<n;++j){Call x;x.op=r.get<std::uint32_t>();x.arg=r.get<std::uint32_t>();x.identity=r.get<std::uint64_t>();x.scalar=r.get<std::uint32_t>();x.payload=r.get<std::uint64_t>();x.state=r.get<FxState96V1>();c.calls.push_back(x);}requests+=n;FxServices16V1 v{&c,Context::invoke};auto actual=initial;int rc;
  if(c.kind==0)rc=dh2_fx_set_anim_v1(&actual,&d,0x2222222233333333ull,&v);else if(c.kind==1)rc=dh2_fx_handle_loop_end_v1(&actual,&v);else if(c.kind==2)rc=dh2_fx_update_v1(&actual,&v);else if(c.kind==3)rc=dh2_fx_play_v1(&actual,initial.position,i%512%4==0?initial.rotation:nullptr,initial.anchor,i%512%2?&d:nullptr,expected.callback,&v);else rc=dh2_fx_drop_reset_v1(&actual,&v);
  if(rc||c.mismatch||c.used!=c.calls.size()||std::memcmp(&actual,&expected,96))throw std::runtime_error("FX state gold mismatch "+std::to_string(i));
  // Required-provider failures stop at the actual source snapshot; no guessed
  // transactional rollback or accepted missing continuation is allowed.
  for(unsigned stop=1;stop<=n;++stop){auto failure=c;failure.used=0;failure.fail_at=stop;failure.mismatch=false;v.context=&failure;actual=initial;
   if(c.kind==0)rc=dh2_fx_set_anim_v1(&actual,&d,0x2222222233333333ull,&v);else if(c.kind==1)rc=dh2_fx_handle_loop_end_v1(&actual,&v);else if(c.kind==2)rc=dh2_fx_update_v1(&actual,&v);else if(c.kind==3)rc=dh2_fx_play_v1(&actual,initial.position,i%512%4==0?initial.rotation:nullptr,initial.anchor,i%512%2?&d:nullptr,expected.callback,&v);else rc=dh2_fx_drop_reset_v1(&actual,&v);
   if(rc!=-2||failure.mismatch||failure.used!=stop||std::memcmp(&actual,&c.calls[stop-1].state,96))throw std::runtime_error("required source-prefix failure "+std::to_string(i));++prefixes;
  }
 }
 if(r.f.peek()!=EOF)throw std::runtime_error("gold trailing bytes");FxState96V1 s{},before=s;FxData32V1 d{};Context c;FxServices16V1 absent{},v{&c,Context::invoke};
 for(unsigned k=0;k<3;++k){if(dh2_fx_set_anim_v1(k?&s:nullptr,k==1?nullptr:&d,1,k==2?&absent:&v)!=-1||std::memcmp(&s,&before,96))throw std::runtime_error("malformed atomic guard");++guards;}
 d.orient_once=256;if(dh2_fx_set_anim_v1(&s,&d,1,&v)!=-1||std::memcmp(&s,&before,96))throw std::runtime_error("byte range guard");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_services\":"<<requests<<",\"required_failure_prefix_checks\":"<<prefixes<<",\"atomic_guards\":"<<guards<<",\"sanitizer_findings\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
