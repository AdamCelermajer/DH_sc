#pragma once
#include <canonical_gameobject_base_owner_v1.hpp>
#include <type_traits>
namespace dh2::world {
template<class S,class=void>struct HasNativePrefixCompanionV92:std::false_type{};
template<class S>struct HasNativePrefixCompanionV92<S,std::void_t<decltype(std::declval<S>().destroy_unpublished_native_prefixes_v92)>>:std::true_type{};
// SAME original GameObjectD2=38d378 native order over the caller's actual base
// and runtime. Class wrapper owns original non-replay/completed journal state.
// Services are genuine typed leaves shared by actual ColBox/Floor, not a Dtor
// placeholder. GameObject300 is explicitly LuaScript from38ef60/37c584.
template<class Services>bool game_object_source_destroy_v72(CanonicalGameObjectBaseOwnerV1& base,
 const Services& s,std::shared_ptr<void>& physical_owner,std::string& e){
 auto live=[&](){if(!s.owner||!s.validate_current){e="Required live GameObject source destruction authority";return false;}return s.validate_current(base,e);};
 if(!live())return false;
 for(auto offset:{0x2d8u,0x2dcu,0x2e0u}){
  auto* cell=base.pointer(offset);if(!cell){e="Required SAME GameObject owned-native pointer cell";return false;}
  const auto actual=*cell;if(actual){
  if(!s.destroy_native){e="Required actual visual/physical/attached deleting leaf";return false;}
  std::shared_ptr<void> pin=offset==0x2dc?physical_owner:s.owner;
  if(!s.destroy_native(base,offset,actual,pin,e)||!live())return false;
  if(*cell&&*cell!=actual){e="GameObject D0 replaced its owned source pointer";return false;}
  *cell=0;if(offset==0x2dc)physical_owner.reset();
  }
  if constexpr(HasNativePrefixCompanionV92<Services>::value){if(s.destroy_unpublished_native_prefixes_v92&&(!s.destroy_unpublished_native_prefixes_v92(base,offset,e)||!live()))return false;}
 }
 auto* script=base.pointer(0x300);if(!script){e="Required SAME LuaScript300 source cell";return false;}const auto actual_lua=*script;
 if(actual_lua){
  if(!s.destroy_lua_script300){e="Required actual LuaScript300 deleting leaf";return false;}
  if(!s.destroy_lua_script300(base,actual_lua,e)||!live())return false;
  if(*script&&*script!=actual_lua){e="LuaScript D0 replaced GameObject300";return false;}*script=0;
 }
 if(!s.stop_sound||!s.destroy_target_list||!s.destroy_pf_object||!s.destroy_object_base){e="Required actual GameObject sound/TargetList/PFObject/ObjectBase destruction leaves";return false;}
 const auto sound=static_cast<std::int16_t>(static_cast<std::uint16_t>(*base.integer(0x370)));
 if(!s.stop_sound(sound,0,e)||!live())return false; // actual VoxSoundManager.Stop369fec
 base.string(0x358)->clear();
 if(!s.destroy_target_list(base,e)||!live())return false; // TargetList304 D1:38d18c
 for(auto offset:{0x2c0u,0x2a8u,0x290u,0x278u})base.string(offset)->clear();
 if(!s.destroy_pf_object(base.runtime(),e)||!live())return false; // PFObject1c8 D1:524f50
 return s.destroy_object_base(base,e); // ObjectBaseD2:33e998; no retired-facet read
}
}
