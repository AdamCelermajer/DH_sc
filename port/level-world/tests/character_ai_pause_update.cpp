#include "../character_ai_pause_update.hpp"
#include "../character_ai_events.hpp"
#include "../character_timers.hpp"
#include <array>
#include <cstdlib>
#include <iostream>
using namespace dh2::character_ai_pause_update;
using namespace dh2::character;
namespace {
constexpr std::uintptr_t ai_id=0x10001000, owner_id=0x10002000, other_id=0x10003000;
struct Fixture {
    AIFrameOwner48 owner{owner_id, 1, 0x100, 0, 0, 0, 0, 0, 0, 0};
    AIFrameOwner48 other{other_id, 2, 0x100, 0, 0, 0, 0, 0, 0, 0};
    State state{ai_id, &owner, 0, 0, 0, 0};
    unsigned mutation=0,calls=0,paused_at_call=0;
    Request captured{};
    static std::int32_t start(void* context, State* state, const Request* r, std::int32_t* timer) {
        auto& f=*static_cast<Fixture*>(context);++f.calls;f.captured=*r;f.paused_at_call=state->paused;
        *timer=-17;
        if(f.mutation==1)state->owner=&f.other;
        if(f.mutation==2)state->paused=0;
        if(f.mutation==3){state->owner=&f.other;state->paused=255;}
        if(f.mutation==4)return 1;
        return 0;
    }
    Services services{this,start};
};
void check(bool pass,const char* message){if(!pass){std::cerr<<message<<'\n';std::exit(1);}}
std::int32_t unexpected(void*,AIEventState64*,const AIEventRequest40*,std::uint32_t*){std::abort();}
struct TimerFixture : Fixture {
    std::array<Timer32,2> slots{};
    TimerStore32 timers{slots.data(),0,2,owner_id,0,0};
    unsigned expirations=0;
    static void expired(void* context,std::uintptr_t owner,std::int32_t event,Timer32*){
        auto& f=*static_cast<TimerFixture*>(context);
        check(owner==owner_id&&event==0x31,"wrong timer recipient/event");
        AIEventOwner48 event_owner{owner_id,1,2,3,0,0,0,0};
        AIEventState64 projection{ai_id,&event_owner,nullptr,0,nullptr,f.state.paused,0,0,0,0,0};
        AIEventServices24 providers{nullptr,unexpected,0,0};
        AIEventPayload24 payload{};AIEventResult16 result{};
        check(dh2_character_ai_event(&result,&projection,0x31,&payload,&providers)==0,"source event failed");
        f.state.paused=projection.paused;++f.expirations;
    }
    TimerServices32 timer_services{this,expired,nullptr,0};
    static std::int32_t real_start(void* context,State*,const Request* r,std::int32_t* id){
        auto& f=*static_cast<TimerFixture*>(context);
        check(r->owner==f.timers.owner&&r->source_timer_offset==0x3b4,"wrong captured timer owner");
        *id=dh2_character_timer_start(&f.timers,r->duration_ms,r->repeat,r->event,r->user_ref,&f.timer_services);
        return *id<0?1:0;
    }
};
}
int main(int argc,char** argv){
    if(argc==4){
        Fixture f;f.state.paused=static_cast<unsigned>(std::strtoul(argv[1],nullptr,0));
        auto duration=static_cast<unsigned>(std::strtoul(argv[2],nullptr,0));
        f.mutation=static_cast<unsigned>(std::strtoul(argv[3],nullptr,0));Result r{};
        auto status=pause(&f.state,duration,&f.services,&r);
        std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"calls\":"<<f.calls
          <<",\"paused_at_call\":"<<f.paused_at_call<<",\"paused\":"<<f.state.paused
          <<",\"owner\":"<<f.state.owner->owner<<",\"timer_owner\":"<<f.captured.owner
          <<",\"duration\":"<<f.captured.duration_ms<<",\"repeat\":"<<f.captured.repeat
          <<",\"event\":"<<f.captured.event<<",\"user_ref\":"<<f.captured.user_ref
          <<",\"timer_id\":"<<r.timer_id<<"}\n";return 0;
    }
    unsigned cases=0;
    Fixture f;Result r{99,99};
    check(pause(nullptr,0,&f.services,&r)==Status::invalid_argument&&r.called==99,"invalid mutated output");++cases;
    check(pause(&f.state,0,nullptr,&r)==Status::service_unavailable&&f.state.paused==1&&!r.called,"missing provider rolled back pause");++cases;
    f.mutation=4;check(pause(&f.state,1000,&f.services,&r)==Status::service_failed&&f.state.paused==1&&r.called,"failure rolled back pause");++cases;
    TimerFixture t;Services services{&t,TimerFixture::real_start};
    check(pause(&t.state,1000,&services,&r)==Status::complete&&t.state.paused==1&&r.timer_id==0,"real Start failed");++cases;
    check(t.slots[0].event==0x31&&!t.slots[0].repeat&&!t.slots[0].user_ref,"real timer fields changed");++cases;
    check(dh2_character_timers_update(&t.timers,999,0,&t.timer_services)==1&&t.state.paused==1&&!t.expirations,"early unpause");++cases;
    check(dh2_character_timers_update(&t.timers,1,1,&t.timer_services)==1&&t.state.paused==1,"blocked clock advanced");++cases;
    check(dh2_character_timers_update(&t.timers,1,0,&t.timer_services)==1&&!t.state.paused&&t.expirations==1,"event31 did not unpause");++cases;
    check(pause(&t.state,0,&services,&r)==Status::complete&&t.state.paused==1,"zero duration changed");++cases;
    check(dh2_character_timers_update(&t.timers,5000,0,&t.timer_services)==1&&t.state.paused==1&&t.expirations==1,"zero duration expired");++cases;
    check(pause(&t.state,10,&services,&r)==Status::complete&&r.timer_id==1,"existing zero timer replaced");++cases;
    check(pause(&t.state,10,&services,&r)==Status::service_failed&&t.state.paused==1,"storage failure unpaused");++cases;
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<"}\n";
}
