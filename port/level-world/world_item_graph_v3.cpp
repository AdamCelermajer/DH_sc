#include "world_item_graph_v3.hpp"
#include "condition_scoped_binding_v72.hpp"
#include <cstring>
namespace dh2::character {
world::GameObjectInitializationServicesV1 WorldItemGraphV3::initialization(){auto s=services_.initialization;s.set_position=[this](const float* p,bool destination,std::string& e){return position(p,destination,e);};return s;}
WorldItemGraphV3::WorldItemGraphV3(std::shared_ptr<RetainedWorldItemObjectV1> item,physical::NativeWorld& world,WorldItemGraphServicesV3 s)
 :item_(std::move(item)),services_(std::move(s)),visual_(item_,services_.visual,initialization()),physical_(*item_,world,services_.physical),world_(world){}
bool WorldItemGraphV3::visibility(bool requested,std::string& e){
 auto& b=item_->base();const bool visible=requested&&b.lifecycle().enabled8a;
 if(!b.store_byte(0x80,visible?1:0,e))return false;
 auto* pointer=b.pointer(0x2d8);if(!pointer){e="Required same Item visual field";return false;}if(!*pointer)return true;
 auto v=visual_.visual();if(!v){e="Required same Item visibility visual";return false;}
 bool effective=visible;if(effective&&!b.lifecycle().non_zonable2ed&&b.lifecycle().zoning2ee&&!b.lifecycle().entered2f0)effective=false;
 return v->set_root_local_visibility_v3(effective,e);
}
bool WorldItemGraphV3::filter(bool enabled,std::string& e){
 auto* pointer=item_->base().pointer(0x2dc);if(!pointer){e="Required same Item physical field";return false;}if(!*pointer)return true;
 if(*pointer!=reinterpret_cast<std::uintptr_t>(&physical_)||!world_.backend()){e="Required same POItem filter receiver/world";return false;}
 if(filter_disabled26_==std::uint8_t(!enabled))return true;
 if(auto* sensor=physical_.secondary_shape()){
  b2FilterData data{};if(enabled){const auto& saved=physical_.config().shape;data.groupIndex=static_cast<int16>(saved.group_index);data.categoryBits=static_cast<uint16>(saved.category_bits);data.maskBits=static_cast<uint16>(saved.mask_bits);}
  else data.groupIndex=data.categoryBits=data.maskBits=0;
  sensor->SetFilterData(data);world_.backend()->Refilter(sensor);
 }
 filter_disabled26_=enabled?0:1;return true;
}
bool WorldItemGraphV3::enable_event(bool enabled,std::string& e){
 auto& b=item_->base();if(enabled){if(!visibility(true,e))return false;b.lifecycle().updating85=1;}
 else{b.lifecycle().updating85=0;if(!visibility(false,e))return false;}
 auto& flags=b.runtime().object.motion.object_flags;if(enabled)flags|=8u;else flags&=~8u;
 if(!filter(enabled,e))return false;b.lifecycle().disabled373=enabled?0:1;return true;
}
bool WorldItemGraphV3::position(const float* p,bool destination,std::string& e){
 auto s=services_.position;
 s.visual_sync=[this](std::uintptr_t id,std::string& error){auto v=visual_.visual();auto* pointer=item_->base().pointer(0x2d8);if(!v||!pointer||*pointer!=id){error="Required same Item visual Sync";return false;}return v->sync(error);};
 s.physical_position=[this](std::uintptr_t id,float x,float y,std::string& error){if(id!=reinterpret_cast<std::uintptr_t>(&physical_)){error="Required same Item physical position receiver";return false;}const float xy[]={x,y};if(dh2_native_body_set_position(&physical_.native(),xy)<0){error="Actual POItem position rejected";return false;}return true;};
 return world::game_object_set_position_v2(item_->base(),p,destination,s,e);
}
bool WorldItemGraphV3::clear_conditions_v75(std::string& e){
 if(services_.condition_binding_v75)return services_.condition_binding_v75->clear(e);
 const auto* activate=item_->base().pointer(0xa8);const auto* deactivate=item_->base().pointer(0xcc);
 if(!activate||!deactivate||*activate||*deactivate){e="Required actual Item condition owner before receiver destruction";return false;}
 e.clear();return true;
}
bool WorldItemGraphV3::route(const WorldItemRequestV1& q,std::int32_t& result,bool& handled,std::string& e){
 handled=true;if(q.object!=item_->base().identity()){e="Item operation must use same canonical graph";return false;}result=0;
 switch(q.operation){
 case WorldItemOperationV1::game_init_post:return visual_.init_post(e);
 case WorldItemOperationV1::apply_mesh_box:return visual_.apply_mesh_box(e);
 case WorldItemOperationV1::visual_item_material:return visual_.init_again_source(services_.color,e);
 case WorldItemOperationV1::create_decor_physical:
  if(!physical_.construct(e))return false;filter_disabled26_=0;pending_physical_assignment_=true;return true;
 case WorldItemOperationV1::set_physical:
  if(pending_physical_assignment_){if(!physical_.assign(e))return false;pending_physical_assignment_=false;return true;}
  return physical_.detach(e);
 case WorldItemOperationV1::enable:{auto& b=item_->base();world::ObjectEnableConditionBorrowV2 borrow{b.byte(0x8a),b.integer(0xec),b.byte(0xf1),b.pointer(0xa8),b.byte(0xac)};auto s=services_.enable;s.context=this;s.enabled_event=[](void* p,bool value,std::string& error){return static_cast<WorldItemGraphV3*>(p)->enable_event(value,error);};return world::object_set_enable_v2(borrow,s,q.flag,e);}
 case WorldItemOperationV1::remove_all:
  if(!q.flag){e="Required source RemoveAllItems(false) equipped-item continuation";return false;}
  return item_->inventory().remove_all_owned_v2(services_.destruction_context,services_.before_item_destroy,e);
 case WorldItemOperationV1::set_position:return position(q.position,q.flag,e);
 case WorldItemOperationV1::set_destination:
  if(!q.position){e="Required original Item destination input";return false;}
  std::memcpy(item_->runtime().controller.destination,q.position,12);return true;
 case WorldItemOperationV1::drop_sound:
  if(!services_.vox){e="Required same world Vox Play3D owner";return false;}
  return world_item_drop_sound_v2(*services_.vox,services_.vox_identity,q.object,static_cast<std::int16_t>(q.integer),q.position,e);
 default:handled=false;return true;
 }
}
}
