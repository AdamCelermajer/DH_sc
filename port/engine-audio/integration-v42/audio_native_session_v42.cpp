#include "audio_native_session_v42.hpp"
#if defined(__ANDROID__) && !defined(DH2_AUDIO_NATIVE_SESSION_FIXTURE)
#include "../integration-v40/focus/audio_control_v40.hpp"
#endif
#include <stdexcept>
#include <cstdio>
#include "../audio_probe_v1.hpp"
namespace dh2::audio {
namespace {
std::atomic<AudioNativeSessionV42*> active_session{nullptr};
#if defined(__ANDROID__) && !defined(DH2_AUDIO_NATIVE_SESSION_FIXTURE)
class RealControl final:public AudioSessionControlOwnerV40 {
    AudioControlV40 control_;
public:
    RealControl(AudioMixerV34& mixer,AudioClockV40& clock,AudioLifecycleGateV40& gate):control_(mixer,clock,gate){}
    bool tick(std::string& error)override{return control_.tick(error);}
    bool shutdown(std::string& error)override{return control_.shutdown(error);}
    bool close_succeeded()const noexcept override{return control_.close_succeeded();}
    bool ready()const noexcept override{return control_.ready_for_current_source();}
};
std::unique_ptr<AudioSessionControlOwnerV40> construct_android_control(
 void*,AudioMixerV34& mixer,AudioClockV40& clock,AudioLifecycleGateV40& gate,std::string& error){
 error.clear();return std::make_unique<RealControl>(mixer,clock,gate);
}
#endif
}
AudioNativeSessionV42::AudioNativeSessionV42(std::uintptr_t manager,AudioGameplaySourcesV40 sources,
 std::shared_ptr<void> lease,AudioLifecycleGateV40& gate):manager_(manager),originals_(sources),provider_lease_(std::move(lease)),
 control_factory_(default_audio_session_control_factory_v42()),gate_(gate){}
AudioNativeSessionV42::AudioNativeSessionV42(std::uintptr_t manager,AudioGameplaySourcesV40 sources,
 std::shared_ptr<void> lease,AudioLifecycleGateV40& gate,AudioSessionControlFactoryV42 factory)
 :manager_(manager),originals_(sources),provider_lease_(std::move(lease)),control_factory_(std::move(factory)),gate_(gate){}
#ifdef DH2_AUDIO_NATIVE_SESSION_FIXTURE
AudioNativeSessionV42::AudioNativeSessionV42(std::uintptr_t manager,AudioGameplaySourcesV40 sources,
 std::shared_ptr<void> lease,AudioLifecycleGateV40& gate,Factory factory,void* context)
 :AudioNativeSessionV42(manager,sources,std::move(lease),gate){factory_=factory;factory_context_=context;}
#endif
AudioNativeSessionV42::~AudioNativeSessionV42() {
    if(runtime_||worker_.joinable()||claimed_)std::terminate();
}
bool AudioNativeSessionV42::producer(std::string& error)const {
    if(std::this_thread::get_id()==producer_)return true;
    error="Required same actual audio gameplay producer thread";return false;
}
bool AudioNativeSessionV42::initialize(std::string& error,std::chrono::milliseconds timeout) {
    if(!producer(error))return false;
    if(!control_factory_.valid()
#ifdef DH2_AUDIO_NATIVE_SESSION_FIXTURE
       &&!factory_
#endif
       ){error="Required actual platform audio control factory";return false;}
    if(initialization_attempted_){error="Native audio session initialization is single-use; keep failed owners or construct a new owner after verified teardown";return false;}
    initialization_attempted_=true;
    if(runtime_||worker_.joinable()||claimed_){error="Existing audio session must complete close/join/drain before a new initialization";return false;}
    if(!manager_||!provider_lease_){error="Required actual Vox identity and complete World/provider lifetime lease";return false;}
    AudioNativeSessionV42* empty=nullptr;
    if(!active_session.compare_exchange_strong(empty,this)){error="Another native audio session retains output ownership";return false;}
    claimed_=true;epoch_=gate_.begin_source();
    auto sources=originals_;sources.context=this;sources.output_ready=output_ready;sources.source_command=source_command;
    try {
        runtime_=std::make_unique<AudioGameplayRuntimeV42>(manager_,sources);
        if(!epoch_||!runtime_->initialize_exact_source(error)) {
            gate_.publish_source(epoch_,false);runtime_.reset();active_session.store(nullptr);claimed_=false;return false;
        }
    } catch(const std::exception& failure) {
        gate_.publish_source(epoch_,false);runtime_.reset();active_session.store(nullptr);claimed_=false;
        error=std::string("Actual audio source initialization exception: ")+failure.what();return false;
    } catch(...) {
        gate_.publish_source(epoch_,false);runtime_.reset();active_session.store(nullptr);claimed_=false;
        error="Actual audio source initialization provider threw an unknown exception";return false;
    }
    // Only the producer reads this ordinary runtime bool. Control receives its
    // actual result through the epoch mailbox, not through borrowed mutable data.
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(close_requested_){error="Audio close was requested during source initialization; retain session for shutdown";return false;}
        if(!gate_.publish_source(epoch_,runtime_->source_data_initialized())) {
            error="Source initialization epoch was superseded; keep session until shutdown";return false;
        }
    }
    try {worker_=std::thread(&AudioNativeSessionV42::control_thread,this);}
    catch(const std::exception& failure) {
        gate_.publish_source(epoch_,false);runtime_.reset();active_session.store(nullptr);claimed_=false;error=failure.what();return false;
    }
    std::unique_lock<std::mutex> lock(mutex_);
    if(!changed_.wait_for(lock,timeout,[this]{return startup_known_;})) {error="Audio control construction timeout; retain session and request close";return false;}
    if(!startup_ok_) {error=worker_error_;return false;}
    if(close_requested_){error="Audio close was requested during source initialization; retain session for shutdown";return false;}
    accepting_.store(true,std::memory_order_release);error.clear();return true;
}
bool AudioNativeSessionV42::source_command(void* raw,const dh2::character::CombatSoundPlayV1& play,
 const AudioSoundV34& sound,const AudioGroupV34& group,AudioCommandV34& command,std::string& error) {
    auto& self=*static_cast<AudioNativeSessionV42*>(raw);
    if(!self.originals_.source_command){error="Required actual source command/listener/settings provider";return false;}
    return self.originals_.source_command(self.originals_.context,play,sound,group,command,error);
}
bool AudioNativeSessionV42::output_ready(void* raw,std::string& error) {
    auto& self=*static_cast<AudioNativeSessionV42*>(raw);
    if(self.originals_.output_ready&&!self.originals_.output_ready(self.originals_.context,error))return false;
    if(!self.ready_for_current_source()) {
        error=self.control_error();
        if(error.empty())error="Required actual same-epoch foreground audio output timestamp";
        return false;
    }
    return true;
}
bool AudioNativeSessionV42::ready_for_current_source()const {
    std::lock_guard<std::mutex> lock(mutex_);
    return accepting_.load(std::memory_order_acquire)&&live_control_&&live_control_->ready();
}
std::string AudioNativeSessionV42::control_error()const {
    std::lock_guard<std::mutex> lock(mutex_);return worker_error_;
}
bool AudioNativeSessionV42::submit_actual_play(const dh2::character::CombatSoundPlayV1& play,std::int64_t event,std::string& error) {
    if(!producer(error))return false;
    if(in_submit_||!runtime_||!accepting_.load(std::memory_order_acquire)){error="Audio session submission is unavailable, reentrant or closing";return false;}
    in_submit_=true;struct Exit{bool& value;~Exit(){value=false;}}exit{in_submit_};
    return runtime_->submit_actual_play(play,event,error);
}
AudioGameplayRuntimeV42* AudioNativeSessionV42::runtime_on_producer()noexcept {
    return std::this_thread::get_id()==producer_?runtime_.get():nullptr;
}
void AudioNativeSessionV42::request_output_close() {
    accepting_.store(false,std::memory_order_release);
    std::lock_guard<std::mutex> lock(mutex_);
    if(epoch_)gate_.publish_source(epoch_,false);
    close_requested_=true;changed_.notify_all();
}
bool AudioNativeSessionV42::shutdown(std::string& error,std::chrono::milliseconds timeout) {
    if(!producer(error))return false;
    if(in_submit_){error="Required producer submission completion before teardown";return false;}
    if(!runtime_&&!worker_.joinable())return true;
    accepting_.store(false,std::memory_order_release);
    if(runtime_){std::string stop_error;runtime_->stop_world(stop_error);}
    request_output_close();
    if(!worker_.joinable()) {
        std::lock_guard<std::mutex> lock(mutex_);
        close_completed_.store(true,std::memory_order_release);close_result_known_=true;
    }
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if(!changed_.wait_for(lock,timeout,[this]{return close_result_known_;})) {error="Audio close timeout; complete session/provider lease retained";return false;}
        if(!close_completed_.load(std::memory_order_acquire)){error=worker_error_.empty()?"Unproved output close; complete session retained":worker_error_;return false;}
    }
    if(worker_.joinable())worker_.join();
    control_joined_.store(true,std::memory_order_release);
    if(runtime_&&!runtime_->finalize_after_output_closed({&runtime_->mixer(),&close_completed_,&control_joined_},error))return false;
    runtime_.reset();provider_lease_.reset();control_factory_={};active_session.store(nullptr);claimed_=false;error.clear();return true;
}
void AudioNativeSessionV42::control_thread() {
    std::unique_ptr<AudioSessionControlOwnerV40> control;
    try {
        std::string construction_error;
#ifdef DH2_AUDIO_NATIVE_SESSION_FIXTURE
        if(factory_)control=factory_(factory_context_,runtime_->mixer(),runtime_->clock(),gate_);
        else
#endif
        if(control_factory_.valid())control=control_factory_.construct(control_factory_.context,
            runtime_->mixer(),runtime_->clock(),gate_,construction_error);
        else throw std::runtime_error("Required actual platform audio control factory");
        if(!control&&!construction_error.empty())throw std::runtime_error(construction_error);
        if(!control)throw std::runtime_error("Control owner construction returned no owner");
    } catch(const std::exception& failure) {
        std::lock_guard<std::mutex> lock(mutex_);worker_error_=failure.what();startup_known_=true;startup_ok_=false;
        close_completed_.store(true,std::memory_order_release);close_result_known_=true;changed_.notify_all();return;
    }
    std::unique_lock<std::mutex> lock(mutex_);
    live_control_=control.get();startup_known_=true;startup_ok_=true;changed_.notify_all();
    bool tick_failed=false;
    while(!close_requested_) {
        lock.unlock();std::string error;
        bool tick_ok=true;
        {static double probeLast=0;const double probeNow=probe::ms_now();if(probeLast>0&&probeNow-probeLast>60)std::printf("PROBE loop gap ms=%.1f at=%.0f\n",probeNow-probeLast,probeNow);probeLast=probeNow;}
        try {if(!tick_failed){const double probeT0=probe::ms_now();tick_ok=control->tick(error);const double probeD=probe::ms_now()-probeT0;if(probeD>20)std::printf("PROBE tick ms=%.1f at=%.0f\n",probeD,probeT0);}}
        catch(const std::exception& failure){tick_ok=false;error=failure.what();}
        if(!tick_failed&&!tick_ok) {
            lock.lock();worker_error_=error;tick_failed=true;lock.unlock();
        }
        {const double probeL=probe::ms_now();
        lock.lock();
        const double probeW=probe::ms_now();
        changed_.wait_for(lock,std::chrono::milliseconds(20),[this]{return close_requested_;});
        const double probeE=probe::ms_now();if(probeW-probeL>5||probeE-probeW>40)std::printf("PROBE wait lockMs=%.1f waitMs=%.1f at=%.0f\n",probeW-probeL,probeE-probeW,probeL);}
    }
    lock.unlock();std::string error;
    bool closed=false;
    try {closed=control->shutdown(error)&&control->close_succeeded();}
    catch(const std::exception& failure){error=failure.what();}
    lock.lock();close_completed_.store(closed,std::memory_order_release);close_result_known_=true;
    if(!closed)worker_error_=error.empty()?"Required actual output close barrier":error;
    if(closed)live_control_=nullptr;
    changed_.notify_all();
    if(!closed) {
        // The producer observes the failure and returns. Keep control, mixer,
        // samples and provider receivers alive without busy retry or joining.
        changed_.wait(lock,[]{return false;});
    }
    lock.unlock();control.reset(); // same thread that constructed the output
}

AudioSessionControlFactoryV42 default_audio_session_control_factory_v42() noexcept{
#if defined(__ANDROID__) && !defined(DH2_AUDIO_NATIVE_SESSION_FIXTURE)
    return {construct_android_control,nullptr,{}};
#else
    return {};
#endif
}
}
