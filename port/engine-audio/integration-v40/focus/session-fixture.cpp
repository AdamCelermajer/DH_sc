#include "audio_native_session_v40.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <cstdlib>
using namespace dh2::audio;
struct Counters {
    std::atomic<unsigned> constructed{0},destroyed{0},ticks{0},closes{0},reads{0};
    bool fail_close{},close_during_init{},throw_read{},fail_tick{};std::thread::id producer;
    std::atomic<bool> init_read_entered{false},cancel_done{false};
};
struct Fixture {std::string root;std::shared_ptr<Counters> counters;};
static bool asset(void* raw,const char* uri,std::shared_ptr<const std::vector<std::uint8_t>>& bytes,std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);++f.counters->reads;
    if(f.counters->throw_read)throw std::runtime_error("Explicit borrowed exact-read exception");
    if(f.counters->close_during_init&&f.counters->reads==1) {
        f.counters->init_read_entered=true;
        for(unsigned i=0;i<1000&&!f.counters->cancel_done;++i)std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if(!f.counters->cancel_done)throw std::runtime_error("fixture initialization cancellation timeout");
    }
    const std::string name=uri;
    const auto path=f.root+(name.rfind("data/pydata/",0)==0?"/port/engine-audio/reference/source-bindings-v38/":"/.local-inputs/audio-v34/cache/")+name.substr(12);
    std::ifstream stream(path,std::ios::binary);if(!stream){error="Required exact fixture asset";return false;}
    bytes=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(stream),std::istreambuf_iterator<char>());return true;
}
class FakeControl final:public AudioSessionControlOwnerV40 {
    Counters& counts_;AudioClockV40& clock_;AudioLifecycleGateV40& gate_;std::thread::id owner_;
    std::atomic<bool> ready_{false};bool closed_{};
public:
    FakeControl(Counters& c,AudioClockV40& clock,AudioLifecycleGateV40& gate):counts_(c),clock_(clock),gate_(gate),owner_(std::this_thread::get_id()) {
        if(owner_==counts_.producer)throw std::runtime_error("control constructed on producer");++counts_.constructed;
    }
    ~FakeControl()override {if(owner_!=std::this_thread::get_id()||!closed_)std::terminate();++counts_.destroyed;}
    bool tick(std::string& error)override {
        if(owner_!=std::this_thread::get_id())std::terminate();++counts_.ticks;
        if(counts_.fail_tick){ready_=false;error="Explicit borrowed native driver tick failure";return false;}
        ready_=gate_.permitted();
        if(ready_)clock_.publish({0,1000000000,0,std::uint64_t(gate_.source_epoch())<<32,32000,true});
        return true;
    }
    bool shutdown(std::string& error)override {
        if(owner_!=std::this_thread::get_id())std::terminate();++counts_.closes;ready_=false;
        if(counts_.fail_close){error="Explicit borrowed fixture close failure";return false;}
        closed_=true;return true;
    }
    bool close_succeeded()const noexcept override {return closed_;}
    bool ready()const noexcept override {return ready_.load()&&gate_.permitted();}
};
static std::unique_ptr<AudioSessionControlOwnerV40> control(void* raw,AudioMixerV34&,AudioClockV40& clock,AudioLifecycleGateV40& gate) {
    return std::make_unique<FakeControl>(*static_cast<Counters*>(raw),clock,gate);
}
int main(int argc,char** argv) {
    if(argc<2)return 2;
    unsigned checks{};auto check=[&](bool value){++checks;if(!value)throw std::runtime_error("session ownership assertion "+std::to_string(checks));};
    auto counts=std::make_shared<Counters>();counts->producer=std::this_thread::get_id();
    counts->fail_close=argc>2&&std::string(argv[2])=="failed_close";
    counts->close_during_init=argc>2&&std::string(argv[2])=="close_during_init";
    counts->throw_read=argc>2&&std::string(argv[2])=="throw_read";
    counts->fail_tick=argc>2&&std::string(argv[2])=="failed_tick";
    auto f=std::make_shared<Fixture>();f->root=argv[1];f->counters=counts;std::weak_ptr<Fixture> weak=f;
    AudioGameplaySourcesV40 sources;sources.context=f.get();sources.exact_assets={f.get(),asset};
    sources.gates={f.get(),[](void*,const dh2::sound::VoxPlay3DRequestV2&q,dh2::sound::VoxPlay3DResponseV2&r) {
        r={};if(q.operation==dh2::sound::VoxPlay3DOperationV2::current_level){r.identity=0x44;r.value=0;}return 0;
    }};
    sources.output_ready=[](void*,std::string&){return true;};
    AudioLifecycleGateV40 gate;check(gate.publish_activity(1,1,true,true,true,false));
    auto session=new AudioNativeSessionV40(0xabc,sources,f,gate,control,counts.get());std::string error;
    if(counts->throw_read) {
        check(!session->initialize(error));check(error.find("Explicit borrowed exact-read exception")!=std::string::npos);
        check(!gate.source_ready()&&counts->constructed==0);delete session;
        counts->throw_read=false;
        {AudioNativeSessionV40 replacement(0xabc,sources,f,gate,control,counts.get());check(replacement.initialize(error));check(replacement.shutdown(error));}
        check(counts->constructed==1&&counts->destroyed==1);
        std::cout<<"PASS "<<checks<<" throwing-provider checks; no callback existed, failed initialization released session claim\n";return 0;
    }
    if(counts->close_during_init) {
        std::thread cancel([&]{while(!counts->init_read_entered)std::this_thread::sleep_for(std::chrono::milliseconds(1));session->request_output_close();counts->cancel_done=true;});
        check(!session->initialize(error));cancel.join();f.reset();check(!weak.expired());
        check(!gate.source_ready()&&!session->ready_for_current_source());
        check(session->shutdown(error));check(weak.expired());check(counts->constructed==0&&counts->closes==0);
        check(!session->initialize(error));delete session;
        std::cout<<"PASS "<<checks<<" initialization cancellation checks; no output owner opened and provider released after explicit producer drain\n";return 0;
    }
    check(session->initialize(error));check(counts->reads==3);f.reset();check(!weak.expired());
    check(session->runtime_on_producer()->bindings().rows().size()==638);
    for(unsigned i=0;i<100&&!(counts->fail_tick?!session->control_error().empty():session->ready_for_current_source());++i)std::this_thread::sleep_for(std::chrono::milliseconds(2));
    if(counts->fail_tick) {
        check(!session->ready_for_current_source());check(session->control_error()=="Explicit borrowed native driver tick failure");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));check(counts->ticks==1);
        check(session->shutdown(error));check(weak.expired());delete session;
        std::cout<<"PASS "<<checks<<" driver-failure checks; exact diagnostics, no retry, safe explicit close/join\n";return 0;
    }
    check(session->ready_for_current_source());
    bool foreign_failed=false;std::thread foreign([&]{std::string e;foreign_failed=!session->shutdown(e);});foreign.join();check(foreign_failed);
    dh2::character::CombatSoundPlayV1 play;play.manager=0xabc;play.sound_id=0;
    check(session->submit_actual_play(play,1000000000,error)); // explicit phase0 fixture remains silent
    check(session->runtime_on_producer()->last_token()==0);
    std::thread ui([&]{session->request_output_close();});ui.join();check(!session->ready_for_current_source());
    check(!session->submit_actual_play(play,1000000000,error));
    std::atomic<bool> reader_done{false};std::thread reader([&]{while(!reader_done)session->ready_for_current_source();});
    const auto before=std::chrono::steady_clock::now();const bool closed=session->shutdown(error);
    reader_done=true;reader.join();
    if(counts->fail_close) {
        check(!closed);check(std::chrono::steady_clock::now()-before<std::chrono::milliseconds(1000));
        check(counts->closes==1&&counts->destroyed==0&&!weak.expired());
        std::this_thread::sleep_for(std::chrono::milliseconds(50));check(counts->closes==1);
        auto other=std::make_shared<Fixture>();other->root=argv[1];other->counters=counts;
        {AudioNativeSessionV40 blocked(0xdef,sources,other,gate,control,counts.get());check(!blocked.initialize(error));}
        std::cout<<"PASS "<<checks<<" failed-close checks; producer returned, complete owner retained, no retry/new session\n"<<std::flush;
        // This child process intentionally owns an unproved live callback
        // lifetime. Process exit is the fixture boundary, not safe destruction.
        std::_Exit(0);
    }
    check(closed);check(session->runtime_on_producer()==nullptr);check(weak.expired());
    check(counts->constructed==1&&counts->destroyed==1&&counts->closes==1);
    check(!session->initialize(error));
    delete session;
    std::cout<<"PASS "<<checks<<" session checks; exact source init, control thread, independent close/join/drain/release\n";
}
