#include "../../audio_world_producer_v38.hpp"
#include "../../audio_animation_swoosh_v38.hpp"
#include "../../audio_named_animation_sound_v38.hpp"
dh2::character::CombatSoundPlayV1 check_audio_world_producer_v38(
 std::uintptr_t manager,std::uintptr_t subject,std::int32_t sound,
 const std::array<float,3>& position){
 return dh2::audio::audio_world_request_v38(manager,subject,sound,position);
}
extern "C" int audio_swoosh_fixture_v38(const std::int32_t* input,std::int32_t* output){
 struct Fixture {const std::int32_t* in;std::int32_t* out;} f{input,output};
 output[0]=output[1]=0;
 dh2::character::AnimationSwooshServicesV4 s{&f,
 [](void*,std::int32_t kind,std::uintptr_t& item){item=kind;return 0;},
 [](void* raw,std::uintptr_t item,std::int32_t& sound,std::int32_t& fx){auto& f=*static_cast<Fixture*>(raw);sound=f.in[item-1];fx=f.in[item+1]?0:-1;return 0;},
 [](void* raw,std::int32_t sound){auto& f=*static_cast<Fixture*>(raw);f.out[2+f.out[1]++]=sound;return 0;},
 [](void*,std::int32_t,bool){return 0;}};
 bool sf=false,ff=false;std::string error;
 const int rc=dh2::audio::audio_animation_swoosh_v38(false,s,sf,ff,error);
 output[0]=(sf?1:0)|(ff?2:0);
 if(!rc&&sf)output[2+output[1]++]=232;
 return rc;
}
extern "C" int audio_request_fixture_v38(const std::uint32_t* input,std::uint32_t* output){
 std::array<float,3> position{};std::memcpy(position.data(),input,12);
 const auto p=dh2::audio::audio_world_request_v38(1,2,-1,position);
 std::memcpy(output,p.position.data(),12);output[3]=p.source_bool;output[4]=p.source_integer;
 std::memcpy(output+5,&p.source_float0,4);std::memcpy(output+6,&p.source_float1,4);
 output[7]=std::uint32_t(p.sound_id);return 0;
}
