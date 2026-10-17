#include "../character_ai_initialization.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

namespace init = dh2::character_ai_initialization;
constexpr std::uintptr_t AI = 0x10010000;
using Fields = std::vector<std::pair<unsigned, std::uintptr_t>>;
Fields fields(const init::State& s) {
    Fields f{{0,s.dispatch_table},{4,s.owner_04},{8,s.word_08},{12,s.word_0c},
        {16,s.word_10},{20,s.word_14},{24,s.paused_18},{28,s.active_ais_1c},
        {32,s.alternate_ais_20},{36,s.byte_24},{40,s.pointer_28},{44,s.byte_2c},
        {48,s.script_name_30},{52,s.word_34},{56,s.word_38},{60,s.requested_target_3c},
        {64,s.target_40},{68,s.last_target_44},{72,s.alive_48},{73,s.sight_49},
        {74,s.byte_4a},{75,s.byte_4b},{76,s.sticky_4c},{77,s.targetable_4d},
        {80,s.master_50},{84,s.byte_54},{85,s.byte_55},{88,s.word_58}};
    const init::TreeHeader* trees[]{&s.tree_5c,&s.tree_7c,&s.tree_94};
    const unsigned offsets[]{0x5c,0x7c,0x94};
    for (unsigned i=0;i<3;++i) {
        const auto& h=*trees[i]; const auto self=reinterpret_cast<std::uintptr_t>(&h);
        auto normalize=[&](std::uintptr_t v){return v==self?AI+offsets[i]:v;};
        f.push_back({offsets[i],h.color});f.push_back({offsets[i]+4,h.parent});
        f.push_back({offsets[i]+8,normalize(h.left)});f.push_back({offsets[i]+12,normalize(h.right)});
        f.push_back({offsets[i]+16,h.count});
    }
    const auto self=reinterpret_cast<std::uintptr_t>(&s.list_ac);
    f.push_back({0xac,s.list_ac.next==self?AI+0xac:s.list_ac.next});
    f.push_back({0xb0,s.list_ac.previous==self?AI+0xac:s.list_ac.previous});
    for(unsigned i=0;i<6;++i)f.push_back({0xb4+4*i,s.words_b4_to_c8[i]});
    f.push_back({0xcc,s.word_cc});f.push_back({0xd0,s.byte_d0});f.push_back({0xd1,s.byte_d1});
    return f;
}
struct Fixture {
    std::uintptr_t expected=AI, appended=0;
    unsigned calls=0, fail=0, mutation=0;
    Fields before;
    static int append(void* p,init::State* s,std::uintptr_t ai) {
        auto& f=*static_cast<Fixture*>(p);++f.calls;assert(ai==f.expected);
        f.before=fields(*s);f.appended=ai;
        if(f.mutation){s->targetable_4d=17;s->active_ais_1c=0x10018000;s->owner_04=0x1001a000;}
        if(f.fail==2)throw 1;
        return f.fail==1?7:0;
    }
};
void write_fields(const Fields& f) {
    std::cout<<'[';for(unsigned i=0;i<f.size();++i){if(i)std::cout<<',';std::cout<<'['<<f[i].first<<','<<f[i].second<<']';}std::cout<<']';
}
int main(int argc,char** argv) {
    if(argc==5) {
        unsigned fill=std::strtoul(argv[1],nullptr,0), table=std::strtoul(argv[2],nullptr,0);
        Fixture f;f.mutation=std::strtoul(argv[3],nullptr,0);f.fail=std::strtoul(argv[4],nullptr,0);
        init::State s;std::memset(&s,static_cast<int>(fill),sizeof s);s.identity=AI;
        // Original pointer slots are 32-bit; only the oracle fixture limits
        // preserved owner to its ARM32 pattern. The runtime uses uintptr_t.
        s.owner_04=fill*0x01010101u;
        const init::Services c{&f,Fixture::append};init::Result r{};
        const auto status=init::construct(&s,table,&c,&r);
        std::cout<<"{\"status\":"<<int(status)<<",\"queue_calls\":"<<r.queue_calls
                 <<",\"queued\":"<<r.queued<<",\"appended\":"<<f.appended<<",\"fields\":";
        write_fields(fields(s));std::cout<<",\"before_queue\":";write_fields(f.before);std::cout<<"}\n";return 0;
    }
    assert(argc==1);
    unsigned cases=0;
    Fixture f;init::State s{};s.identity=AI;s.owner_04=UINT64_C(0x123456789);s.alive_48=255;
    init::Services c{&f,Fixture::append};init::Result r{};
    assert(init::construct(&s,UINT64_C(0x234567891),&c,&r)==init::Status::complete);++cases;
    assert(s.owner_04==UINT64_C(0x123456789)&&s.dispatch_table==UINT64_C(0x234567891)&&s.alive_48==255);++cases;
    assert(s.targetable_4d==1&&s.active_ais_1c==0&&s.master_50==0&&r.queued&&r.queue_calls==1);++cases;
    for(unsigned failure=1;failure<=2;++failure) {
        f.fail=failure;f.mutation=1;
        assert(init::construct(&s,9,&c,&r)==init::Status::service_failed);++cases;
        assert(s.targetable_4d==17&&s.active_ais_1c==0x10018000&&r.queue_calls==1&&!r.queued);++cases;
    }
    c.append_queue=nullptr;s.targetable_4d=0;
    assert(init::construct(&s,9,&c,&r)==init::Status::service_unavailable&&s.targetable_4d==1&&!r.queue_calls);++cases;
    const auto previous=fields(s);r={99,99};
    assert(init::construct(&s,0,&c,&r)==init::Status::invalid_argument&&r.queued==99&&previous==fields(s));++cases;
    assert(init::construct(nullptr,9,&c,&r)==init::Status::invalid_argument);++cases;
    assert(init::construct(&s,9,nullptr,&r)==init::Status::invalid_argument);++cases;
    assert(init::construct(&s,9,&c,nullptr)==init::Status::invalid_argument);++cases;
    assert(init::construct(&s,9,reinterpret_cast<init::Services*>(&s),&r)==init::Status::invalid_argument);++cases;
    assert(init::construct(&s,9,&c,reinterpret_cast<init::Result*>(&s))==init::Status::invalid_argument);++cases;
    alignas(init::State) unsigned char storage[sizeof(init::State)+8]{};
    assert(init::construct(reinterpret_cast<init::State*>(storage+1),9,&c,&r)==init::Status::invalid_argument);++cases;
    s.identity=0;assert(init::construct(&s,9,&c,&r)==init::Status::invalid_argument);++cases;
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<"}\n";
}
