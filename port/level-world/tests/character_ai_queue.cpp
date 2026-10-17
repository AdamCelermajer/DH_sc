#include "../character_ai_queue.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <array>
namespace q=dh2::character_ai_queue;
constexpr std::uintptr_t A=0x10014000, B=0x10028000, AI=0x10010000;
struct Fixture {
    std::array<q::Owner,8> owners{};
    std::array<q::Entry,8> actors{};
    std::array<q::Entry*,8> entries{};
    q::Owner replacement{};
    q::State state{};
    unsigned dt=16,faerie=0,follower=0,zonable=1,mutation=0;
    std::vector<std::array<std::uintptr_t,2>> calls;
    static int query(void* raw,q::State* s,q::Entry* e,const q::Request* r,std::uint32_t* out) {
        auto& f=*static_cast<Fixture*>(raw);
        f.calls.push_back({unsigned(r->operation),r->subject});
        if(r->operation==q::Operation::frame_delta) {
            *out=f.dt;
            if(f.mutation==4)s->timer=77; // captured positive timer must win.
        } else {
            *out=r->operation==q::Operation::is_faerie?f.faerie:
                r->operation==q::Operation::is_follower?f.follower:f.zonable;
            if((f.mutation==1 && r->operation==q::Operation::is_faerie) ||
               (f.mutation==2 && r->operation==q::Operation::is_follower) ||
               (f.mutation==3 && r->operation==q::Operation::is_zonable))e->owner=&f.replacement;
        }
        return 0;
    }
};
int main(int argc,char** argv) {
    if(argc!=14)return 2;
    Fixture f; unsigned x[13];
    for(unsigned i=0;i<13;++i)x[i]=std::strtoul(argv[i+1],nullptr,0);
    if(x[0]>8)return 2;
    f.state={f.entries.data(),x[0],8,static_cast<std::int32_t>(x[1]),static_cast<std::uint8_t>(x[3])};
    f.dt=x[2]; f.faerie=x[9];f.follower=x[10];f.zonable=x[11];f.mutation=x[12];
    for(unsigned i=0;i<8;++i) {
        f.owners[i]={A+i*0x1000,static_cast<std::uint8_t>(x[4]),static_cast<std::uint8_t>(x[5]),
            static_cast<std::uint8_t>(x[6]),static_cast<std::uint8_t>(x[7]),static_cast<std::uint8_t>(x[8])};
        f.actors[i]={AI+i*0x100,&f.owners[i]};f.entries[i]=&f.actors[i];
    }
    f.replacement={B,static_cast<std::uint8_t>(x[4]),static_cast<std::uint8_t>(x[5]),
        static_cast<std::uint8_t>(x[6]),0,static_cast<std::uint8_t>(x[8])};
    const q::Services services{&f,Fixture::query};q::Result r{};
    auto status=q::advance(&f.state,&services,&r);
    std::cout<<"{\"status\":"<<int(status)<<",\"decision\":"<<unsigned(r.decision)
      <<",\"timer\":"<<f.state.timer<<",\"rotations\":"<<r.rotations<<",\"front\":"<<r.front
      <<",\"queue\":[";
    for(unsigned i=0;i<f.state.count;++i){if(i)std::cout<<',';std::cout<<f.entries[i]->ai;}
    std::cout<<"],\"calls\":[";
    for(unsigned i=0;i<f.calls.size();++i){if(i)std::cout<<',';std::cout<<'['<<f.calls[i][0]<<','<<f.calls[i][1]<<']';}
    std::cout<<"]}\n";
}
