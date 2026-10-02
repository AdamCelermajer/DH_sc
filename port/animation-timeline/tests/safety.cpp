#include "../timeline.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <limits>
using namespace dh2::timeline;
static std::uint32_t seed=20261002;
static std::uint32_t next() { seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed; }
int main() {
    for (int i=0;i<5000;++i) {
        State state{};
        const auto start=static_cast<std::int32_t>(next()%20000000)-10000000;
        const auto end=start+1+static_cast<std::int32_t>(next()%100000);
        const float speed=(static_cast<int>(next()%129)-64)/4.0f;
        assert(dh2_timeline_init(&state,start,end,speed,next()%2)==Error::ok);
        assert(dh2_timeline_jump(&state,start)==Error::ok);
        for (int j=0;j<10;++j) {
            if (i%3==0) {
                auto* bytes=reinterpret_cast<unsigned char*>(&state);
                bytes[next()%sizeof(state)]=static_cast<unsigned char>(next());
            }
            const auto before=state;
            const auto result=dh2_timeline_update(&state,static_cast<std::int32_t>(next()%100000000));
            if (result!=Error::ok) assert(std::memcmp(&before,&state,sizeof(state))==0);
        }
    }
    State state{7,8,9,0};const auto before=state;
    assert(dh2_timeline_init(&state,0,0,1,true)==Error::range);
    assert(std::memcmp(&before,&state,sizeof(state))==0);
    assert(dh2_timeline_init(&state,-1000000001,100,1,true)==Error::range);
    assert(dh2_timeline_init(&state,0,100,std::numeric_limits<float>::quiet_NaN(),true)==Error::nonfinite);
    assert(dh2_timeline_init(&state,0,100,65,true)==Error::range);
    assert(dh2_timeline_update(nullptr,0)==Error::argument);
    std::puts("timeline safety: 5000 sequences, 50000 corrupted/valid updates; unchanged error state");
}
