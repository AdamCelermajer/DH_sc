#include "canonical_colbox_v71.hpp"
#include "game_object_source_destroy_v72.hpp"
#include <algorithm>
#include <cstring>
#include <utility>
namespace dh2::world {
CanonicalColBoxV71::CanonicalColBoxV71(std::shared_ptr<void> pin,actor::RuntimeState& runtime,
 GameObjectInitializationServicesV1 init,ColBoxServicesV71 services):
 base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(pin),runtime),init_services_(std::move(init)),
 initialization_(base_,init_services_),services_(std::move(services)){
 // Inlined factory340fe0..341040: GameObjectC1 first, three dimension stores0,
 // then ColBox vtable/static84. Native C++ type is ColBox, not a shadow vtable.
 base_.lifecycle().static84=1;
}
bool CanonicalColBoxV71::live(std::string& e){if(!services_.owner||!services_.validate_current){e="Required actual ColBox engine/source authority";return false;}return services_.validate_current(base_,e);}
CanonicalPropertyActorV1 CanonicalColBoxV71::properties()noexcept{
 auto p=canonical_family_fields_v15(*this);p.fields.write_vector3=[](void* p,std::uint32_t o,const auto& v,std::string& e){return static_cast<CanonicalColBoxV71*>(p)->write_vector3(o,v,e);};return p;
}
bool CanonicalColBoxV71::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){auto f=base_.properties().fields;return f.read_bool(f.context,o,v,e);}
bool CanonicalColBoxV71::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){auto f=base_.properties().fields;return f.write_bool(f.context,o,v,e);}
bool CanonicalColBoxV71::write_int(std::uint32_t o,std::int32_t v,std::string& e){auto f=base_.properties().fields;return f.write_int(f.context,o,v,e);}
bool CanonicalColBoxV71::write_string(std::uint32_t o,const std::string& v,std::string& e){auto f=base_.properties().fields;return f.write_string(f.context,o,v,e);}
bool CanonicalColBoxV71::write_vector3(std::uint32_t o,const std::array<float,3>& v,std::string& e){if(o==0x374){dimensions374_=v;return true;}auto f=base_.properties().fields;return f.write_vector3(f.context,o,v,e);}
bool CanonicalColBoxV71::set_physical_object(ColBoxPhysicalReferenceV71& next,bool,std::string& e){
 // Whole reached SetPhysicalObject394bf8 with initialization argument false.
 // The other argument's Start46eb20 branch is not reached by ColBox InitPost.
 if(!live(e)||!services_.debug_value){if(e.empty())e="Required actual MP_NoPhysics debug query";return false;}
 bool no_physics{};if(!services_.debug_value("MP_NoPhysics",no_physics,e)||!live(e))return false;
 if(no_physics){if(next.actual){
   if(!services_.destroy_native){e="Required POColmap D0 in MP_NoPhysics branch";return false;}
   if(!services_.destroy_native(base_,0x2dc,next.actual,next.owner,e)||!live(e))return false;
  }next={};pending_physical_pin_.reset();return true;
 }
 auto* current=base_.pointer(0x2dc);if(!current){e="Required same ColBox physical2dc cell";return false;}
 if(*current!=next.actual){
  if(*current&&!release_pointer(0x2dc,e))return false;
  *current=next.actual;physical_owner_=next.owner;next={};pending_physical_pin_.reset();
 }
 if(!services_.update_pf_object){e="Required actual GameObject UpdatePFObject393ea0";return false;}
 if(!services_.update_pf_object(base_,e))return false;return live(e);
}
bool CanonicalColBoxV71::init_post(std::string& e){
 if(init_attempted_){e="ColBox InitPost cannot replay a reached source prefix";return false;}init_attempted_=true;
 if(!live(e))return false;
 bool eligible{};if(!initialization_.init_post(eligible,e)||!live(e))return false;
 // Original derived body continues after the void GameObject call, even when
 // spawn/high-performance base path returns without constructing a visual.
 const float* scale=base_.vector3(0x120);
 for(unsigned i=0;i<3;++i){volatile float scaled=dimensions374_[i]*scale[i];dimensions374_[i]=scaled;}
 for(unsigned i=0;i<3;++i){volatile float half=dimensions374_[i]*.5f;std::uint32_t bits;float positive=half;std::memcpy(&bits,&positive,4);bits^=0x80000000u;
  float negative;std::memcpy(&negative,&bits,4);base_.relative_aabb144()[i]=negative;base_.relative_aabb144()[i+3]=positive;
 }base_.update_absolute_aabb(); // exact UpdateAbsoluteAABB38aac8; no flat-box repair
 if(!services_.physical_world||!services_.construct_po_colmap){e="Required actual application PhysicalWorld44 / POColmap source constructor";return false;}
 std::uintptr_t physical_world{};std::shared_ptr<void> world_borrow;
 if(!services_.physical_world(physical_world,world_borrow,e)||!live(e))return false;
 if(!physical_world||!world_borrow){e="Required genuine PhysicalWorld receiver/borrow";return false;}
 ColBoxPhysicalArgumentsV71 args;
 if(!services_.construct_po_colmap(physical_world,base_,args,pending_physical_,e)){pending_physical_pin_=pending_physical_.owner;return false;}
 pending_physical_pin_=pending_physical_.owner;
 if(!live(e))return false;
 if(!pending_physical_.actual||!pending_physical_.owner){e="POColmap constructor did not produce an actual retained native receiver";return false;}
 return set_physical_object(pending_physical_,false,e);
}
bool CanonicalColBoxV71::set_position(const std::array<float,3>& p,bool destination,std::string& e){if(!init_services_.set_position){e="Required SAME ColBox SetPosition393db4";return false;}if(!live(e)||!init_services_.set_position(p.data(),destination,e))return false;return live(e);}
bool CanonicalColBoxV71::release_pointer(std::uint32_t offset,std::string& e){
 auto* p=base_.pointer(offset);if(!p){e="Required SAME ColBox/GameObject owned pointer cell";return false;}if(!*p)return true;
 if(!services_.destroy_native){e="Required actual owned native D0 leaf";return false;}
 std::shared_ptr<void> local_pin=offset==0x2dc?physical_owner_:services_.owner;
 if(!services_.destroy_native(base_,offset,*p,local_pin,e)||!live(e))return false;
 *p=0;if(offset==0x2dc)physical_owner_.reset();return true;
}
bool CanonicalColBoxV71::destroy(std::string& e){
 if(destroy_done_){e.clear();return true;}if(destroy_attempted_){e=destroy_error_;return false;}destroy_attempted_=true;
 auto fail=[&](){if(e.empty())e="Required reached actual ColBox/GameObject source destruction leaf";destroy_error_=e;return false;};
 if(!live(e))return fail();
 // A failed InitPost may retain an uninstalled actual POColmap prefix. This
 // separate native journal must drain before actual GameObject D2 continues.
 if(pending_physical_.actual){if(!services_.destroy_native||!services_.destroy_native(base_,0x2dc,pending_physical_.actual,pending_physical_.owner,e)||!live(e))return fail();pending_physical_={};pending_physical_pin_.reset();}
 // ColBox D1/D0 enters SAME GameObject D2 shared with real Floor.
 if(!game_object_source_destroy_v72(base_,services_,physical_owner_,e))return fail();
 destroy_done_=true;e.clear();return true;
 // D0's storage delete310440 follows external real manager/Room/transport
 // unpublication and native-alias exhaustion, then journal retirement.
}
CanonicalClassReceiverV1 CanonicalColBoxV71::factory_receiver(std::shared_ptr<CanonicalColBoxV71> owner,std::shared_ptr<const void> xml){
 auto out=canonical_class_receiver_v1(owner);out.source_lease=std::move(xml);out.init_post=[owner](std::string& e){return owner->init_post(e);};
 out.is_game_object=[](bool& v,std::string& e){v=true;e.clear();return true;};
 out.position=[owner](std::array<float,3>& v,std::string& e){std::copy_n(owner->base().vector3(0x160),3,v.begin());e.clear();return true;};
 out.set_position=[owner](const auto& p,bool d,std::string& e){return owner->set_position(p,d,e);};return out;
}
}


