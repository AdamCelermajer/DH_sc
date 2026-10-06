#include "audio_output_v34.hpp"
#include <algorithm>
#include <ctime>
#include <dlfcn.h>
namespace dh2::audio {
bool AAudioApiV34::load(std::string&error){if(library)return true;library=dlopen("libaaudio.so",RTLD_NOW|RTLD_LOCAL);if(!library){error="Required actual Android API26+ AAudio device library";return false;}
#define DH2_AAUDIO_V34(field,name) field=reinterpret_cast<decltype(field)>(dlsym(library,name));if(!field){error="Required actual AAudio entry " name;unload();return false;}
 DH2_AAUDIO_V34(create,"AAudio_createStreamBuilder");DH2_AAUDIO_V34(delete_builder,"AAudioStreamBuilder_delete");DH2_AAUDIO_V34(direction,"AAudioStreamBuilder_setDirection");DH2_AAUDIO_V34(format,"AAudioStreamBuilder_setFormat");DH2_AAUDIO_V34(channels,"AAudioStreamBuilder_setChannelCount");DH2_AAUDIO_V34(performance,"AAudioStreamBuilder_setPerformanceMode");DH2_AAUDIO_V34(sharing,"AAudioStreamBuilder_setSharingMode");DH2_AAUDIO_V34(data_callback,"AAudioStreamBuilder_setDataCallback");DH2_AAUDIO_V34(error_callback,"AAudioStreamBuilder_setErrorCallback");DH2_AAUDIO_V34(open,"AAudioStreamBuilder_openStream");DH2_AAUDIO_V34(get_format,"AAudioStream_getFormat");DH2_AAUDIO_V34(get_channels,"AAudioStream_getChannelCount");DH2_AAUDIO_V34(rate,"AAudioStream_getSampleRate");DH2_AAUDIO_V34(burst,"AAudioStream_getFramesPerBurst");DH2_AAUDIO_V34(buffer,"AAudioStream_setBufferSizeInFrames");DH2_AAUDIO_V34(start,"AAudioStream_requestStart");DH2_AAUDIO_V34(pause,"AAudioStream_requestPause");DH2_AAUDIO_V34(stop,"AAudioStream_requestStop");DH2_AAUDIO_V34(close,"AAudioStream_close");DH2_AAUDIO_V34(timestamp,"AAudioStream_getTimestamp");
#undef DH2_AAUDIO_V34
 return true;}
void AAudioApiV34::unload()noexcept{if(library)dlclose(library);*this={};}
aaudio_data_callback_result_t AndroidAudioOutputV34::data(AAudioStream*,void*raw,void*out,std::int32_t frames){auto&self=*static_cast<AndroidAudioOutputV34*>(raw);if(frames<0)return AAUDIO_CALLBACK_RESULT_STOP;
 if(self.audible_.load(std::memory_order_acquire))self.mixer_.render(static_cast<float*>(out),unsigned(frames));else {std::fill_n(static_cast<float*>(out),std::size_t(frames)*2,0.f);self.mixer_.advance_silence(unsigned(frames));}return AAUDIO_CALLBACK_RESULT_CONTINUE;}
void AndroidAudioOutputV34::error(AAudioStream*,void*raw,aaudio_result_t){auto&self=*static_cast<AndroidAudioOutputV34*>(raw);self.audible_.store(false,std::memory_order_release);self.disconnected_.store(true,std::memory_order_release);}
AndroidAudioOutputV34::~AndroidAudioOutputV34(){close();api_.unload();}
bool AndroidAudioOutputV34::open(std::string&error){
 if(stream_)return true;if(!api_.load(error))return false;AAudioStreamBuilder*builder=nullptr;auto status=api_.create(&builder);if(status!=AAUDIO_OK||!builder){error="AAudio builder "+std::to_string(status);return false;}
 api_.direction(builder,AAUDIO_DIRECTION_OUTPUT);api_.format(builder,AAUDIO_FORMAT_PCM_FLOAT);api_.channels(builder,2);api_.performance(builder,AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);api_.sharing(builder,AAUDIO_SHARING_MODE_SHARED);api_.data_callback(builder,data,this);api_.error_callback(builder,AndroidAudioOutputV34::error,this);
 status=api_.open(builder,&stream_);api_.delete_builder(builder);
 if(status!=AAUDIO_OK||!stream_){stream_=nullptr;error="AAudio output open "+std::to_string(status);return false;}
 if(api_.get_format(stream_)!=AAUDIO_FORMAT_PCM_FLOAT||api_.get_channels(stream_)!=2||!mixer_.set_rate(unsigned(api_.rate(stream_)))){close();error="Required actual AAudio float stereo format/rate";return false;}
 const int burst=api_.burst(stream_);if(burst>0)api_.buffer(stream_,burst*2);
 stream_base_frame_=mixer_.output_frame();disconnected_.store(false,std::memory_order_release);return true;
}
void AndroidAudioOutputV34::close()noexcept{audible_.store(false,std::memory_order_release);if(stream_){api_.stop(stream_);api_.close(stream_);stream_=nullptr;}}
bool AndroidAudioOutputV34::lifecycle(bool resumed,bool focused,std::string&error){resumed_=resumed;focused_=focused;
 if(!resumed||!focused){audible_.store(false,std::memory_order_release);if(stream_){const auto status=api_.pause(stream_);if(status!=AAUDIO_OK){error="AAudio pause "+std::to_string(status);return false;}}return true;}
 if(!open(error))return false;audible_.store(true,std::memory_order_release);const auto status=api_.start(stream_);if(status!=AAUDIO_OK){audible_.store(false,std::memory_order_release);error="AAudio start "+std::to_string(status);return false;}return true;
}
bool AndroidAudioOutputV34::recover(std::string&error){if(!disconnected_.exchange(false,std::memory_order_acq_rel))return true;close();return lifecycle(resumed_,focused_,error);}
bool AndroidAudioOutputV34::frame_at_monotonic_ns(std::int64_t event,std::uint64_t&frame)const noexcept{
 if(!stream_||event<0)return false;std::int64_t position,time;auto status=api_.timestamp(stream_,CLOCK_MONOTONIC,&position,&time);if(status!=AAUDIO_OK||position<0)return false;const auto rate=api_.rate(stream_);const double delta=double(event-time)*double(rate)/1e9;const double target=double(stream_base_frame_)+double(position)+delta;if(target<0||target>=double(UINT64_MAX))return false;frame=std::max(mixer_.output_frame(),std::uint64_t(target));return true;
}
}
