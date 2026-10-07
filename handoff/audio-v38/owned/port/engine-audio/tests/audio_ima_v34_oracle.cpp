#include "../audio_sample_v34.hpp"
#include "../audio_spatial_v34.hpp"
#include "../audio_native_envelope_v34.hpp"
#include <cstring>
extern "C" unsigned dh2_audio_ima_v34(const unsigned char*in,unsigned char*out){
 unsigned channels,size;std::memcpy(&channels,in,4);std::memcpy(&size,in+4,4);
 if(size>2048||channels>2)return 0;
 std::int16_t samples[4096]{};const auto frames=dh2::audio::audio_ima_block_v34(in+8,size,channels,samples,4096);
 std::memcpy(out,samples,frames*channels*2);return frames;
}
extern "C" unsigned dh2_audio_envelope_v34(const unsigned char*in,unsigned char*out){
 unsigned frames,channels;std::memcpy(&frames,in,4);std::memcpy(&channels,in+4,4);
 if(frames>256||channels>2)return 0;
 dh2::audio::AudioNativeEnvelopeV34 e;std::memcpy(&e.delay_frames,in+8,16);
 std::int16_t samples[512]{};std::int32_t accumulator[512]{};
 std::memcpy(samples,in+24,frames*channels*2);std::memcpy(accumulator,in+24+frames*channels*2,frames*channels*4);
 if(!dh2::audio::audio_native_envelope_mix_v34(samples,frames,channels,accumulator,e))return 0;
 std::memcpy(out,&e.delay_frames,16);unsigned stopped=e.stopped;std::memcpy(out+16,&stopped,4);std::memcpy(out+20,accumulator,frames*channels*4);return 20+frames*channels*4;
}
extern "C" unsigned dh2_audio_spatial_v34(const unsigned char*in,unsigned char*out){
 dh2::audio::AudioListenerV34 listener;dh2::audio::AudioSpatialSourceV34 source;dh2::audio::AudioSpatialResultV34 result;
 static_assert(sizeof listener==48&&sizeof source==52&&sizeof result==16);
 std::memcpy(&listener,in,48);std::memcpy(&source,in+48,52);
 if(!dh2::audio::audio_spatial_v34(listener,source,result))return 0;std::memcpy(out,&result,16);return 16;
}
