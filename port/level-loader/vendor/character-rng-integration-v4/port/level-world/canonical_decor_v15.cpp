#include "canonical_decor_v15.hpp"
#include <algorithm>
namespace dh2::world {
CanonicalDecorV15::CanonicalDecorV15(std::shared_ptr<void> p,actor::RuntimeState& r,GameObjectInitializationServicesV1 i,DecorServicesV15 s):base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(p),r),init_services_(std::move(i)),initialization_(base_,init_services_),services_(std::move(s)){base_.lifecycle().static84=1;}
bool CanonicalDecorV15::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){if(o==0x376){v=solid376_;return true;}auto a=base_.properties().fields;return a.read_bool(a.context,o,v,e);}
bool CanonicalDecorV15::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){if(o==0x376){solid376_=v;return true;}auto a=base_.properties().fields;return a.write_bool(a.context,o,v,e);}
bool CanonicalDecorV15::write_int(std::uint32_t o,std::int32_t v,std::string& e){auto a=base_.properties().fields;return a.write_int(a.context,o,v,e);}
bool CanonicalDecorV15::write_string(std::uint32_t o,const std::string& v,std::string& e){auto a=base_.properties().fields;return a.write_string(a.context,o,v,e);}
bool CanonicalDecorV15::load_floor_map(std::string& e){auto visual=*base_.pointer(0x2d8);if(!visual||!load_floor375_)return true;
 if(!services_.visual_root){e="Required Decor same visual root8";return false;}std::uintptr_t root{};if(!services_.visual_root(visual,root,e))return false;
 if(!services_.load_room){e="Required Decor PFWorld LoadRoom523c14";return false;}std::uintptr_t room{};std::uint32_t* flags{};
 if(!services_.load_room(root,base_.room64(),*base_.string(0x30),room,flags,e))return false;
 if(room){if(!flags){e="Required SAME PFRoom flags24";return false;}if(solid376_)*flags|=1;else *flags&=~1u;
  if(!services_.extend_bounds){e="Required PFRoom ExtendBoundingBox388218";return false;}if(!services_.extend_bounds(room,base_.absolute_aabb12c(),e))return false;}
 load_floor375_=0;return true;
}
bool CanonicalDecorV15::init_post(std::string& e){bool eligible{};if(!initialization_.init_post(eligible,e))return false;const auto visual=*base_.pointer(0x2d8);if(!visual)return true;
 if(!services_.visual_sync){e="Required Decor VisualObject Sync470a54";return false;}if(!services_.visual_sync(visual,e))return false;
 if(!services_.visual_physical){e="Required Decor same visual byte28";return false;}bool physical{};if(!services_.visual_physical(visual,physical,e))return false;
 if(physical){if(!services_.construct_podecor){e="Required Decor PODecor388a2c";return false;}std::uintptr_t body{};if(!services_.construct_podecor(base_,body,e))return false;
  if(!body||!services_.set_physical){e="Required actual Decor SetPhysicalObject394bf8";return false;}if(!services_.set_physical(body,false,e))return false;}
 return load_floor_map(e);
}
bool CanonicalDecorV15::set_position(const std::array<float,3>& p,bool b,std::string& e){if(!init_services_.set_position){e="Required SAME Decor SetPosition";return false;}return init_services_.set_position(p.data(),b,e);}
bool CanonicalDecorV15::destroy(std::string& e){if(destroyed_){e="Decor destruction cannot replay";return false;}if(!services_.destroy_base){e="Required actual Decor GameObject destruction";return false;}destroyed_=true;return services_.destroy_base(base_,e);}
CanonicalClassReceiverV1 CanonicalDecorV15::factory_receiver(std::shared_ptr<CanonicalDecorV15> o,std::shared_ptr<const void> xml){auto a=canonical_class_receiver_v1(o);a.source_lease=std::move(xml);a.init_post=[o](std::string& e){return o->init_post(e);};a.is_game_object=[](bool& b,std::string&){b=true;return true;};a.position=[o](std::array<float,3>& p,std::string&){std::copy_n(o->base().vector3(0x160),3,p.begin());return true;};a.set_position=[o](const auto& p,bool b,std::string& e){return o->set_position(p,b,e);};return a;}
}
