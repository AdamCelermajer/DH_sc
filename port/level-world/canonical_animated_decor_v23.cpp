#include "canonical_animated_decor_v23.hpp"
#include <algorithm>
#include <utility>

namespace dh2::world {
CanonicalAnimatedDecorV23::CanonicalAnimatedDecorV23(std::shared_ptr<void> world,
 actor::RuntimeState& runtime,GameObjectInitializationServicesV1 initialization,
 AnimatedDecorServicesV23 services):
 CanonicalDecorV15(std::move(world),runtime,std::move(initialization),std::move(services.decor)),
 services_(std::move(services)){
 // Original342680 changes the SAME inherited source375 after Decor stores.
 source_load_floor375()=0;
}
bool CanonicalAnimatedDecorV23::write_string(std::uint32_t offset,const std::string& value,std::string& error){
 if(offset==0x37c){startanim37c_=value;return true;}
 return CanonicalDecorV15::write_string(offset,value,error);
}
bool CanonicalAnimatedDecorV23::missing_source(const char* what,std::string& e)const{
 e="Required actual AnimatedDecor source leaf: ";e+=what;return false;
}
bool CanonicalAnimatedDecorV23::source_mesh_box_and_physical(std::string& e){
 const auto visual=*base().pointer(0x2d8);const auto& source=services_.source_init;
 // Source470a54 is ApplyMeshBox. The historical visual_sync leaf name is
 // retained for API compatibility; binding it to plain Sync is incorrect.
 if(!source.visual_sync)return missing_source("VisualObject.ApplyMeshBox470a54",e);
 if(!source.visual_sync(visual,e))return false;
 const auto current=*base().pointer(0x2d8);bool physical{};
 if(!source.visual_physical28)return missing_source("actual VisualObject byte28",e);
 if(!source.visual_physical28(current,physical,e))return false;
 if(physical){
  if(!source.construct_podecor)return missing_source("PODecor388a2c",e);
  std::uintptr_t body{};if(!source.construct_podecor(base(),body,e))return false;
  if(!body||!source.set_physical)return missing_source("actual SetPhysicalObject394bf8",e);
  if(!source.set_physical(body,false,e))return false;
 }
 return true;
}
bool CanonicalAnimatedDecorV23::source_random_animation(bool install,std::string& e){
 const auto& source=services_.source_init;std::int32_t count{},index{};bool accepted{};
 const auto visual=*base().pointer(0x2d8);
 if(!source.animation_count)return missing_source("actual timeline count slot10",e);
 if(!source.animation_count(visual,count,e))return false;
 if(!source.random)return missing_source("SAME process Random388c58",e);
 const auto maximum=static_cast<std::int32_t>(static_cast<std::uint32_t>(count)-1u);
 if(!source.random(maximum,index,e))return false;
 if(!source.play_index)return missing_source("actual timeline Play index slot1c",e);
 if(!source.play_index(*base().pointer(0x2d8),index,false,accepted,e))return false;
 if(install){
  // Source InitPost ignores this first Play result and always installs the
  // actual completion callback; the callback's later Play has an assertion.
  if(!source.install_random_completion)return missing_source("timeline completion slot2c",e);
  return source.install_random_completion(*base().pointer(0x2d8),
   [this](std::string& error){return animation_finished(error);},e);
 }
 if(!accepted){if(!source.random_play_assertion)return missing_source("CallbackRandomAll native assertion policy",e);return source.random_play_assertion(e);}
 return true;
}
bool CanonicalAnimatedDecorV23::init_post(std::string& e){
 // Existing legacy service remains a separate explicit route. The production
 // catalog now supplies typed source_init and executes this local source body.
 if(!services_.source_init.owner){
  if(!services_.whole_init_post)return missing_source("source389128 typed engine services",e);
  return services_.whole_init_post(*this,e);
 }
 *base().byte(0x10c)=1;
 if(!CanonicalDecorV15::init_post(e))return false;
 // Qualified GameObject.MeetCondition38ab60 is literally mov1;bx lr.
 // Original false-visibility branch is unreachable for this qualified call.
 const auto visual=*base().pointer(0x2d8);if(!visual)return true;
 if(startanim37c_.empty())startanim37c_="idle"; // actual CString literal8c22b8
 auto random_all=[](const std::string& name){
  const char* token="randomall";if(name.size()!=9)return false;
  for(unsigned i=0;i<9;++i){auto ch=static_cast<unsigned char>(name[i]);
   if(ch>='A'&&ch<='Z')ch=static_cast<unsigned char>(ch+('a'-'A'));
   if(ch!=static_cast<unsigned char>(token[i]))return false;
  }return true;
 }; // original imported strcasecmp30e6e8 against literal8c22c0
 const auto& source=services_.source_init;
 if(random_all(startanim37c_)){if(!source_random_animation(true,e))return false;}
 else{
  if(!source.animation_exists)return missing_source("actual timeline name lookup slot14",e);
  bool found{},accepted{};if(!source.animation_exists(visual,startanim37c_,found,e))return false;
  if(found){
   if(!source.play_name)return missing_source("actual timeline Play name slot20",e);
   if(!source.play_name(*base().pointer(0x2d8),startanim37c_,true,accepted,e))return false;
  }
  if(!found||!accepted){
   if(!source.play_index)return missing_source("actual timeline index0 fallback",e);
   if(!source.play_index(*base().pointer(0x2d8),0,true,accepted,e))return false;
  }
 }
 if(!source_mesh_box_and_physical(e))return false;
 if(!source.update)return missing_source("actual GameObject.Update38cbe8",e);
 return source.update(e);
}
bool CanonicalAnimatedDecorV23::animation_finished(std::string& e){
 if(destroyed_||!*base().pointer(0x2d8))return missing_source("live SAME callback actor/visual",e);
 return source_random_animation(false,e);
}
bool CanonicalAnimatedDecorV23::destroy(std::string& error){
 if(destroyed_){error="AnimatedDecor destruction cannot replay";return false;}
 destroyed_=true;startanim37c_.clear();
 return CanonicalDecorV15::destroy(error);
}
CanonicalClassReceiverV1 CanonicalAnimatedDecorV23::factory_receiver(
 std::shared_ptr<CanonicalAnimatedDecorV23> owner,std::shared_ptr<const void> xml){
 auto result=canonical_class_receiver_v1(owner);result.source_lease=std::move(xml);
 result.init_post=[owner](std::string& error){return owner->init_post(error);};
 result.is_game_object=[](bool& value,std::string&){value=true;return true;};
 result.position=[owner](std::array<float,3>& value,std::string&){std::copy_n(owner->base().vector3(0x160),3,value.begin());return true;};
 result.set_position=[owner](const std::array<float,3>& value,bool destination,std::string& error){return owner->set_position(value,destination,error);};
 return result;
}
}
