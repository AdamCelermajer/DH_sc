#include "canonical_animated_decor_v1.hpp"
namespace dh2::world {
namespace {bool random_all(const std::string& name){const char* expected="randomall";if(name.size()!=9)return false;for(unsigned i=0;i<9;++i){auto c=static_cast<unsigned char>(name[i]);if(c>='A'&&c<='Z')c=static_cast<unsigned char>(c+('a'-'A'));if(c!=static_cast<unsigned char>(expected[i]))return false;}return true;}}
CanonicalAnimatedDecorV1::CanonicalAnimatedDecorV1(std::shared_ptr<void> pin,actor::RuntimeState& runtime,GameObjectInitializationServicesV1 init,AnimatedDecorServicesV1 services):base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(pin),runtime),initialization_(base_,std::move(init)),services_(std::move(services)){
 base_.animated_decor_constructor_static();inherited_=base_.properties().fields;
}
bool CanonicalAnimatedDecorV1::missing(const char* what,std::string& e)const{e="required AnimatedDecor source provider: ";e+=what;return false;}
bool CanonicalAnimatedDecorV1::read_bool(void* p,std::uint32_t o,std::uint8_t& v,std::string& e){auto& s=*static_cast<CanonicalAnimatedDecorV1*>(p);if(o==0x376){v=s.solid376_;return true;}return s.inherited_.read_bool(s.inherited_.context,o,v,e);}
bool CanonicalAnimatedDecorV1::write_bool(void* p,std::uint32_t o,std::uint8_t v,std::string& e){auto& s=*static_cast<CanonicalAnimatedDecorV1*>(p);if(o==0x376){s.solid376_=v;return true;}return s.inherited_.write_bool(s.inherited_.context,o,v,e);}
bool CanonicalAnimatedDecorV1::write_int(void* p,std::uint32_t o,std::int32_t v,std::string& e){auto& s=*static_cast<CanonicalAnimatedDecorV1*>(p);return s.inherited_.write_int(s.inherited_.context,o,v,e);}
bool CanonicalAnimatedDecorV1::write_float(void* p,std::uint32_t o,float v,std::string& e){auto& s=*static_cast<CanonicalAnimatedDecorV1*>(p);return s.inherited_.write_float(s.inherited_.context,o,v,e);}
bool CanonicalAnimatedDecorV1::write_string(void* p,std::uint32_t o,const std::string& v,std::string& e){auto& s=*static_cast<CanonicalAnimatedDecorV1*>(p);if(o==0x37c){s.startanim37c_=v;return true;}return s.inherited_.write_string(s.inherited_.context,o,v,e);}
bool CanonicalAnimatedDecorV1::write_vector3(void* p,std::uint32_t o,const std::array<float,3>& v,std::string& e){auto& s=*static_cast<CanonicalAnimatedDecorV1*>(p);return s.inherited_.write_vector3(s.inherited_.context,o,v,e);}
bool CanonicalAnimatedDecorV1::write_point2(void* p,std::uint32_t o,const std::array<std::int32_t,2>& v,std::string& e){auto& s=*static_cast<CanonicalAnimatedDecorV1*>(p);return s.inherited_.write_point2(s.inherited_.context,o,v,e);}
CanonicalPropertyActorV1 CanonicalAnimatedDecorV1::properties()noexcept{auto a=base_.properties();a.fields={this,read_bool,write_bool,write_int,write_float,write_string,write_vector3,write_point2};return a;}
bool CanonicalAnimatedDecorV1::sync_and_physical(std::string& e){
 const auto visual=*base_.pointer(0x2d8);
 if(!services_.visual_sync)return missing("VisualObject Sync470a54",e);
 if(!services_.visual_sync(visual,e))return false;
 bool physical=false;if(!services_.visual_physical28)return missing("same visual byte28",e);
 if(!services_.visual_physical28(visual,physical,e))return false;
 if(physical){std::uintptr_t body=0;if(!services_.construct_podecor)return missing("PODecor388a2c",e);if(!services_.construct_podecor(base_,body,e))return false;if(!body)return missing("constructed PODecor identity",e);if(!services_.set_physical)return missing("SetPhysicalObject394bf8",e);if(!services_.set_physical(body,false,e))return false;}
 return true;
}
bool CanonicalAnimatedDecorV1::random_animation(bool install,std::string& e){
 const auto visual=*base_.pointer(0x2d8);std::int32_t count=0,index=0;bool played=false;
 if(!services_.animation_count)return missing("timeline count slot10",e);if(!services_.animation_count(visual,count,e))return false;
 if(!services_.random)return missing("shared Random388c58",e);if(!services_.random(static_cast<std::int32_t>(static_cast<std::uint32_t>(count)-1),index,e))return false;
 if(!services_.play_index)return missing("timeline Play index slot1c",e);if(!services_.play_index(visual,index,false,played,e))return false;
 if(install){if(!services_.install_random_completion)return missing("timeline completion slot2c",e);return services_.install_random_completion(visual,[this](std::string& error){return animation_finished(error);},e);}
 if(!played){if(!services_.random_play_assertion)return missing("CallbackRandomAll source assertion",e);return services_.random_play_assertion(e);}
 return true;
}
bool CanonicalAnimatedDecorV1::init_post(std::string& e){
 *base_.byte(0x10c)=1;bool eligible=false;
 if(!initialization_.init_post(eligible,e))return false;
 // Decor::InitPost continues after base InitPost even when base was gated.
 if(*base_.pointer(0x2d8)&&!sync_and_physical(e))return false;
 // Factory342600 set375=0; source LoadFloorMap388730 therefore skips.
 const auto visual=*base_.pointer(0x2d8);if(!visual)return true;
 if(startanim37c_.empty())startanim37c_="idle"; // exact four-byte literal8c22b8
 if(random_all(startanim37c_)){
  if(!random_animation(true,e))return false;
 }else{
  bool found=false,played=false;if(!services_.animation_exists)return missing("timeline lookup slot14",e);if(!services_.animation_exists(visual,startanim37c_,found,e))return false;
  if(found){if(!services_.play_name)return missing("timeline Play name slot20",e);if(!services_.play_name(visual,startanim37c_,true,played,e))return false;}
  if(!found||!played){if(!services_.play_index)return missing("timeline fallback index0",e);if(!services_.play_index(visual,0,true,played,e))return false;}
 }
 if(!sync_and_physical(e))return false;
 if(!services_.update)return missing("GameObject Update virtual2c",e);return services_.update(e);
}
bool CanonicalAnimatedDecorV1::animation_finished(std::string& e){if(!*base_.pointer(0x2d8))return missing("completion same retained visual",e);return random_animation(false,e);}
bool CanonicalAnimatedDecorV1::destroy(std::string& e){
 // Original derived dtor frees startanim before GameObject dtor. Real base
 // teardown releases timeline callbacks/visual/body/registrations in its order.
 startanim37c_.clear();if(!services_.destroy_base)return missing("GameObject destructor38d378",e);return services_.destroy_base(e);
}
}
