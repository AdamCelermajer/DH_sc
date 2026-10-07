#include "audio_lifecycle_gate_v40.hpp"
#include <iostream>
#include <stdexcept>
#include <thread>
int main() {
    unsigned checks{};auto check=[&](bool value){++checks;if(!value)throw std::runtime_error("activity/source gate assertion");};
    dh2::audio::AudioLifecycleGateV40 gate;
    check(!gate.permitted());check(!gate.publish_activity(0,1,true,true,true,false));
    check(gate.publish_activity(1,1,true,true,true,false));check(!gate.permitted());
    const auto epoch=gate.begin_source();check(epoch==1);check(gate.publish_source(epoch,true));check(gate.permitted());
    check(gate.permitted_for(epoch));check(!gate.permitted_for(epoch+1));
    check(!gate.publish_activity(1,1,false,false,false,false));check(gate.permitted());
    check(gate.publish_activity(1,2,false,true,true,false));check(!gate.permitted());
    check(gate.publish_activity(1,3,true,false,true,false));check(!gate.permitted());
    check(gate.publish_activity(1,4,true,true,false,false));check(!gate.permitted());
    check(gate.publish_activity(1,5,true,true,true,false));check(gate.permitted());
    const auto next=gate.begin_source();check(next==2&&!gate.permitted());
    check(!gate.permitted_for(epoch));check(!gate.permitted_for(next));
    check(!gate.publish_source(epoch,true));check(!gate.permitted());
    check(gate.publish_source(next,true));check(gate.permitted());
    check(gate.permitted_for(next));check(!gate.permitted_for(epoch));
    check(gate.publish_activity(1,6,true,true,true,true));check(!gate.permitted()&&gate.destroyed());
    check(!gate.publish_activity(1,7,true,true,true,false));
    check(gate.publish_activity(2,1,true,true,true,false));check(gate.permitted());
    std::thread stale([&]{for(unsigned i=1;i<5001;++i)gate.publish_activity(1,i,false,false,false,true);});
    for(unsigned i=2;i<5002;++i)check(gate.publish_activity(2,i,true,true,true,false));
    stale.join();check(gate.permitted());
    check(gate.publish_source(next,false));check(!gate.permitted());
    check(!gate.publish_activity(2,0,true,true,true,false));
    check(!gate.publish_activity(2,0x1000000,true,true,true,false));
    std::cout<<"PASS "<<checks<<" gate checks; source epochs, stale activities, teardown and bounded concurrent publication\n";
}
