#include "audio_native_envelope_v34.hpp"
#include <cstring>
namespace dh2::audio {
namespace {
std::int32_t wrap_add(std::int32_t a,std::int32_t b){auto bits=std::uint32_t(a)+std::uint32_t(b);std::int32_t v;std::memcpy(&v,&bits,4);return v;}
}
bool audio_native_envelope_mix_v34(const std::int16_t*in,std::uint32_t frames,
 std::uint32_t channels,std::int32_t*out,AudioNativeEnvelopeV34&e)noexcept{
 if((channels!=1&&channels!=2)||frames>65536||(!in&&frames)||(!out&&frames)||e.delay_frames<0||e.remaining_frames<0)return false;
 for(unsigned frame=0;frame<frames;++frame){
  bool full=false,silent=e.stopped;int gain=0;
  if(!silent){if(e.delay_frames){--e.delay_frames;silent=e.increment_q30>=0;full=!silent;}
   else if(e.remaining_frames){gain=e.gain_q30>>15;}
   else{if(e.increment_q30<0){e.stopped=true;silent=true;}else full=true;e.delay_frames=e.remaining_frames=e.increment_q30=e.gain_q30=0;}}
  if(!silent)for(unsigned c=0;c<channels;++c){const auto i=frame*channels+c;const std::int32_t sample=full?in[i]:std::int32_t((std::int64_t(in[i])*gain)>>15);out[i]=wrap_add(out[i],sample);}
  if(!silent&&!full&&e.remaining_frames){e.gain_q30=wrap_add(e.gain_q30,e.increment_q30);if(!--e.remaining_frames){const bool dying=e.increment_q30<0;e.delay_frames=e.increment_q30=e.gain_q30=0;if(dying)e.stopped=true;}}
 }
 return true;
}
}
