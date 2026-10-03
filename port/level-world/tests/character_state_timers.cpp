#include "character_state.hpp"
#include "character_timers.hpp"
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
void check(bool yes){if(!yes)throw std::runtime_error("state timer mismatch");}
template<class T>T read(std::ifstream& in){T x{};in.read(reinterpret_cast<char*>(&x),sizeof x);check(bool(in));return x;}
struct Observation {std::uint32_t phase,gate;};
struct Fixture {State state;Facts facts;Services service;std::vector<Observation> order;unsigned locked=0,blocked=0,forced=0;};
void service(void*,State*,const Request*){throw std::runtime_error("unexpected source service");}
void expire(void* context,std::uintptr_t owner,std::int32_t event,Timer32* timer){
 auto& f=*static_cast<Fixture*>(context);check(owner==0xabcdef0123456789ull&&timer->id==0&&event==0x2a);
 f.order.push_back({0,f.state.attack_gate});
 if(f.forced||!(f.locked||f.blocked))f.order.push_back({1,f.state.attack_gate}); // Borrowed AI expired-service boundary.
 f.order.push_back({2,f.state.attack_gate});
 check(dh2_character_state_event(&f.state,&f.facts,unsigned(event),reinterpret_cast<std::uintptr_t>(timer),&f.service)==0);
 f.order.push_back({3,f.state.attack_gate});
}
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream in(argv[1],std::ios::binary);check(bool(in));check(read<std::uint32_t>(in)==0x31545453);auto count=read<std::uint32_t>(in);Fixture f;f.service={&f,service};f.state.current=5;
 for(unsigned i=0;i<count;++i){auto event=read<std::uint32_t>(in);f.state.attack_gate=read<std::uint32_t>(in);auto expected=read<std::uint32_t>(in);check(dh2_character_state_event(&f.state,&f.facts,event,0,&f.service)==0&&f.state.attack_gate==expected);}
 auto chains=read<std::uint32_t>(in);
 for(unsigned i=0;i<chains;++i){f.locked=read<unsigned>(in);f.blocked=read<unsigned>(in);f.forced=read<unsigned>(in);auto ais=read<unsigned>(in);check(ais<=1);auto n=read<unsigned>(in);f.state.attack_gate=0xa5;f.order.clear();Timer32 timer{0,0,10,0,1,0,0,0x2a,0x12345678};TimerStore32 store{&timer,1,1,0xabcdef0123456789ull,0,0};TimerServices32 callbacks{&f,expire,nullptr,0};check(dh2_character_timers_update(&store,10,0,&callbacks)==1&&f.order.size()==n&&!timer.active);
  for(unsigned j=0;j<n;++j){auto phase=read<unsigned>(in),gate=read<unsigned>(in);check(f.order[j].phase==phase&&f.order[j].gate==gate);}
 }
 check(in.peek()==std::ifstream::traits_type::eof());f.state.attack_gate=7;State before=f.state;check(dh2_character_state_event(&f.state,&f.facts,0x2a,0,nullptr)==-1&&f.state.attack_gate==before.attack_gate);
 std::printf("{\"source_gate_projection_cases\":%u,\"source_expiry_routing_chain_cases\":%u,\"invalid_callback_gate_unchanged\":true,\"AI_expiry_service_fixture\":true,\"mismatches\":0}\n",count,chains);return 0;
}catch(const std::exception& e){std::fprintf(stderr,"state timer audit: %s\n",e.what());return 1;}}
