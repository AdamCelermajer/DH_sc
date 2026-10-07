#include "audio_output_v40.hpp"
#include <algorithm>
#include <ctime>
#include <dlfcn.h>
namespace dh2::audio {
bool AAudioApiV40::load(std::string&error){if(library)return true;library=dlopen("libaaudio.so",RTLD_NOW|RTLD_LOCAL);if(!library){error="Required actual Android API26+ AAudio device library";return false;}
#define DH2_AAUDIO_V34(field,name) field=reinterpret_cast<decltype(field)>(dlsym(library,name));if(!field){error="Required actual AAudio entry " name;unload();return false;}
 DH2_AAUDIO_V34(create,"AAudio_createStreamBuilder");DH2_AAUDIO_V34(delete_builder,"AAudioStreamBuilder_delete");DH2_AAUDIO_V34(direction,"AAudioStreamBuilder_setDirection");DH2_AAUDIO_V34(format,"AAudioStreamBuilder_setFormat");DH2_AAUDIO_V34(channels,"AAudioStreamBuilder_setChannelCount");DH2_AAUDIO_V34(performance,"AAudioStreamBuilder_setPerformanceMode");DH2_AAUDIO_V34(sharing,"AAudioStreamBuilder_setSharingMode");DH2_AAUDIO_V34(data_callback,"AAudioStreamBuilder_setDataCallback");DH2_AAUDIO_V34(error_callback,"AAudioStreamBuilder_setErrorCallback");DH2_AAUDIO_V34(open,"AAudioStreamBuilder_openStream");DH2_AAUDIO_V34(get_format,"AAudioStream_getFormat");DH2_AAUDIO_V34(get_channels,"AAudioStream_getChannelCount");DH2_AAUDIO_V34(rate,"AAudioStream_getSampleRate");DH2_AAUDIO_V34(burst,"AAudioStream_getFramesPerBurst");DH2_AAUDIO_V34(buffer,"AAudioStream_setBufferSizeInFrames");DH2_AAUDIO_V34(start,"AAudioStream_requestStart");DH2_AAUDIO_V34(pause,"AAudioStream_requestPause");DH2_AAUDIO_V34(stop,"AAudioStream_requestStop");DH2_AAUDIO_V34(close,"AAudioStream_close");DH2_AAUDIO_V34(timestamp,"AAudioStream_getTimestamp");
#undef DH2_AAUDIO_V34
 return true;}
void AAudioApiV40::unload()noexcept{if(library)dlclose(library);*this={};}
aaudio_data_callback_result_t AndroidAudioOutputV40::data(AAudioStream*,void* raw,void* out,std::int32_t frames) {
    auto& self=*static_cast<AndroidAudioOutputV40*>(raw);
    if(frames<0)return AAUDIO_CALLBACK_RESULT_STOP;
    if(self.audible_.load(std::memory_order_acquire)&&self.gate_.permitted_for(self.stream_source_epoch_.load(std::memory_order_acquire)))self.mixer_.render(static_cast<float*>(out),unsigned(frames));
    else {
        std::fill_n(static_cast<float*>(out),std::size_t(frames)*2,0.f);
        self.mixer_.advance_silence(unsigned(frames));
    }
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}
void AndroidAudioOutputV40::error(AAudioStream*,void* raw,aaudio_result_t) {
    auto& self=*static_cast<AndroidAudioOutputV40*>(raw);
    self.audible_.store(false,std::memory_order_release);
    self.disconnected_.store(true,std::memory_order_release);
}
bool AndroidAudioOutputV40::on_control(std::string& error)const {
    if(std::this_thread::get_id()==control_)return true;
    error="Required same dedicated native audio control thread";return false;
}
AndroidAudioOutputV40::~AndroidAudioOutputV40() {
    // A failed close must retain the complete runtime owner; freeing a borrowed
    // callback mixer after an unproved barrier would be unsafe.
    if(stream_||std::this_thread::get_id()!=control_)std::terminate();
    api_.unload();
}
bool AndroidAudioOutputV40::open(std::string& error) {
    if(!on_control(error))return false;
    if(close_failed_){error=close_error_;return false;}
    if(stream_)return true;
    if(!gate_.permitted()){error="Required actual foreground/focus/source initialization";return false;}
    if(!api_.load(error))return false;
    AAudioStreamBuilder* builder=nullptr;
    auto status=api_.create(&builder);
    if(status!=AAUDIO_OK||!builder){error="AAudio builder "+std::to_string(status);return false;}
    api_.direction(builder,AAUDIO_DIRECTION_OUTPUT);api_.format(builder,AAUDIO_FORMAT_PCM_FLOAT);
    api_.channels(builder,2);api_.performance(builder,AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
    api_.sharing(builder,AAUDIO_SHARING_MODE_SHARED);api_.data_callback(builder,data,this);
    api_.error_callback(builder,AndroidAudioOutputV40::error,this);
    status=api_.open(builder,&stream_);api_.delete_builder(builder);
    if(status!=AAUDIO_OK||!stream_){stream_=nullptr;error="AAudio output open "+std::to_string(status);return false;}
    if(api_.get_format(stream_)!=AAUDIO_FORMAT_PCM_FLOAT||api_.get_channels(stream_)!=2||!mixer_.set_rate(unsigned(api_.rate(stream_)))) {
        std::string close_error;
        if(!close(close_error)){error=close_error;return false;}
        error="Required actual AAudio float stereo format/rate";return false;
    }
    const int burst=api_.burst(stream_);
    if(burst>0)api_.buffer(stream_,burst*2);
    stream_base_frame_=mixer_.output_frame();++generation_;stream_source_epoch_=gate_.source_epoch();
    disconnected_.store(false,std::memory_order_release);return true;
}
bool AndroidAudioOutputV40::close(std::string& error) {
    if(!on_control(error))return false;
    audible_.store(false,std::memory_order_release);
    if(close_failed_){error=close_error_;return false;}
    if(!stream_)return true;
    api_.stop(stream_);
    const auto status=api_.close(stream_);
    if(status!=AAUDIO_OK){close_failed_=true;close_error_="Required AAudio close barrier "+std::to_string(status)+"; complete owner retained without retry";error=close_error_;return false;}
    stream_=nullptr;running_=false;return true;
}
bool AndroidAudioOutputV40::apply(std::string& error) {
    if(!on_control(error))return false;
    if(stream_&&stream_source_epoch_.load(std::memory_order_acquire)!=gate_.source_epoch()&&!close(error))return false;
    if(!gate_.permitted()) {
        audible_.store(false,std::memory_order_release);
        if(!gate_.source_ready()||gate_.destroyed())return close(error);
        if(stream_&&running_) {
            const auto status=api_.pause(stream_);
            if(status!=AAUDIO_OK){error="AAudio pause "+std::to_string(status);return false;}
            running_=false;
        }
        return true;
    }
    if(!open(error))return false;
    // Recheck after opening; a main-thread loss may have arrived while the
    // driver was opening. Callback also checks the same atomic gate.
    if(!gate_.permitted())return apply(error);
    if(running_&&audible_.load(std::memory_order_acquire))return true;
    audible_.store(true,std::memory_order_release);
    const auto status=api_.start(stream_);
    if(status!=AAUDIO_OK){audible_.store(false,std::memory_order_release);error="AAudio start "+std::to_string(status);return false;}
    running_=true;
    return true;
}
bool AndroidAudioOutputV40::recover(std::string& error) {
    if(!on_control(error))return false;
    if(!disconnected_.exchange(false,std::memory_order_acq_rel))return true;
    if(!close(error))return false;
    return apply(error);
}
bool AndroidAudioOutputV40::snapshot(AudioDeviceClockSnapshotV40& out)const noexcept {
    if(std::this_thread::get_id()!=control_||!stream_||!audible_requested())return false;
    AudioDeviceClockSnapshotV40 next;
    if(api_.timestamp(stream_,CLOCK_MONOTONIC,&next.position,&next.monotonic_ns)!=AAUDIO_OK||next.position<0)return false;
    next.rate=api_.rate(stream_);if(next.rate<=0)return false;
    next.base_frame=stream_base_frame_;next.generation=(std::uint64_t(stream_source_epoch_.load(std::memory_order_acquire))<<32)|(generation_&0xffffffffu);out=next;return true;
}
}
