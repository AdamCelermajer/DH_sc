#include "canonical_floor_v72.hpp"
#include "game_object_source_destroy_v72.hpp"
#include <algorithm>
#include <utility>
namespace dh2::world {
CanonicalFloorV72::CanonicalFloorV72(std::shared_ptr<void> pin,actor::RuntimeState& runtime,
 GameObjectInitializationServicesV1 init,FloorServicesV72 engine):
 base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(pin),runtime),init_services_(std::move(init)),
 initialization_(base_,init_services_),services_(std::move(engine)){base_.lifecycle().static84=1;}
bool CanonicalFloorV72::live(std::string& e){if(!services_.owner||!services_.validate_current){e="Required actual Floor engine/source authority";return false;}return services_.validate_current(base_,e);}
bool CanonicalFloorV72::init_post(std::string& e){
 if(init_attempted_){e="Floor InitPost cannot replay reached source prefix";return false;}init_attempted_=true;
 if(!live(e))return false;bool eligible{};if(!initialization_.init_post(eligible,e)||!live(e))return false;
 // Source derived body continues after void base path; no spawn gate added.
 if(!init_services_.set_visible){e="Required actual Floor GameObject.SetVisible38b0f0";return false;}
 if(!init_services_.set_visible(false,e)||!live(e))return false;
 const auto visual=*base_.pointer(0x2d8);if(!visual)return true; // actual null branch388704
 if(!services_.visual_apply_mesh_box){e="Required actual VisualObject.ApplyMeshBox470a54";return false;}
 if(!services_.visual_apply_mesh_box(visual,e)||!live(e))return false;
 if(*base_.pointer(0x2d8)!=visual){e="Floor mesh-box delivery replaced actual visual2d8";return false;}
 if(!services_.visual_set_visible){e="Required actual VisualObject.SetVisible471368";return false;}
 if(!services_.visual_set_visible(visual,false,e)||!live(e))return false;
 if(*base_.pointer(0x2d8)!=visual){e="Floor visibility delivery replaced actual visual2d8";return false;}
 if(!services_.visual_root||!services_.get_node_poly_count){e="Required actual VisualObject root8 / GetNodePolyCount50e568";return false;}
 std::uintptr_t root{};if(!services_.visual_root(visual,root,e)||!live(e))return false;
 // Native root8 is passed as-is; genuine GetNodePolyCount owns its null behavior.
 std::uint32_t ignored_count{};if(!services_.get_node_poly_count(root,true,ignored_count,e))return false;return live(e);
}
bool CanonicalFloorV72::set_position(const std::array<float,3>& p,bool destination,std::string& e){
 if(!init_services_.set_position){e="Required SAME Floor SetPosition393db4";return false;}
 if(!live(e)||!init_services_.set_position(p.data(),destination,e))return false;return live(e);
}
bool CanonicalFloorV72::destroy(std::string& e){
 if(destroy_done_){e.clear();return true;}if(destroy_attempted_){e=destroy_error_;return false;}destroy_attempted_=true;
 // Original Floor D1/D0 installs GameObject dispatch then calls SAME D2.
 if(!game_object_source_destroy_v72(base_,services_,physical_owner_,e)){destroy_error_=e;return false;}
 destroy_done_=true;e.clear();return true;
}
CanonicalClassReceiverV1 CanonicalFloorV72::factory_receiver(std::shared_ptr<CanonicalFloorV72> owner,std::shared_ptr<const void> xml){
 auto out=canonical_class_receiver_v1(owner);out.source_lease=std::move(xml);out.init_post=[owner](std::string& e){return owner->init_post(e);};
 out.is_game_object=[](bool& v,std::string& e){v=true;e.clear();return true;};
 out.position=[owner](std::array<float,3>& p,std::string& e){std::copy_n(owner->base().vector3(0x160),3,p.begin());e.clear();return true;};
 out.set_position=[owner](const auto& p,bool d,std::string& e){return owner->set_position(p,d,e);};return out;
}
}
