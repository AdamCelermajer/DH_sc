#pragma once
#include <canonical_gameobject_base_owner_v1.hpp>
#include <audio_level_routing_stop_v94.hpp>
namespace dh2::loader {
inline bool gameobject_d2_stop_sound_v94(world::CanonicalGameObjectBaseOwnerV1& base,
 const audio::AudioApplicationBorrowV42& captured,const audio::AudioStopPrimitivesV94& leaves,std::string& e){
 auto cell=base.integer(0x370);if(!cell){e="Required SAME GameObject signed short370";return false;}
 const auto ordinal=static_cast<std::int16_t>(static_cast<std::uint16_t>(*cell));
 // Native38d434..44c: Stop(short370,0), then CString358. Neither370 nor372
 // is reset here. Qualified GameObjectD2 owns surrounding order/barriers.
 return audio::stop_sound_source_v94(captured,ordinal,0,leaves,e);
}
// Only reached UpdateIdleSound38aeb0 disabled373 branch. This helper is NOT a
// whole UpdateIdleSound replacement; distance/FadeOut/Play3D remain Main.
inline bool gameobject_disabled_idle_stop_v94(world::CanonicalGameObjectBaseOwnerV1& base,
 const audio::AudioApplicationBorrowV42& captured,const audio::AudioStopPrimitivesV94& leaves,std::string& e){
 if(!captured.manager){e.clear();return true;} // native initial global NULL branch
 auto disabled=base.byte(0x373);auto active=base.byte(0x372);
 if(!disabled||!active){e="Required SAME GameObject idle bytes372/373";return false;}
 if(!*disabled){e="Required actually reached disabled373 idle-stop branch";return false;}
 if(!*active){e.clear();return true;}
 *active=0; // source38aec0 BEFORE signed370 read/actual Stop callback
 auto cell=base.integer(0x370);if(!cell){e="Required SAME signed idle ordinal370";return false;}
 const auto ordinal=static_cast<std::int16_t>(static_cast<std::uint16_t>(*cell));
 return audio::stop_sound_source_v94(captured,ordinal,1000,leaves,e);
}
// Existing qualified D2 stop_sound cell preserves constructor/barrier/owner.
// Capture process manager at EACH reached call, never a World/Level owner.
template<class Services>bool bind_gameobject_stop_sound_v94(Services& out,
 std::function<bool(audio::AudioApplicationBorrowV42&,std::string&)> borrow,
 audio::AudioStopPrimitivesV94 leaves,std::string& e){
 if(!out.owner||!borrow){e="Required actual qualified D2/process audio borrower";return false;}
 if(out.stop_sound){e="Actual qualified Stop leaf already bound";return false;}
 out.stop_sound=[borrow=std::move(borrow),leaves=std::move(leaves)](std::int16_t ordinal,std::int32_t fade,std::string& e){audio::AudioApplicationBorrowV42 captured;if(!borrow(captured,e))return false;return audio::stop_sound_source_v94(captured,ordinal,fade,leaves,e);};
 e.clear();return true;
}
}
