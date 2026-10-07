#include "canonical_openable_graph_v21.hpp"
#include <canonical_loading_receiver_v95.hpp>
#include <algorithm>
namespace dh2::world {
bool CanonicalOpenableGraphV21::source_filter_borrow_v105(std::uintptr_t id,physical::NativePhysicalFilterBorrowV1& out,std::string& e){
 const auto* slot=receiver_.base().pointer(0x2dc);
 if(!slot||*slot!=id){e="Required SAME Openable PhysicalObject2dc filter receiver";return false;}
 for(const auto& body:bodies_)if(body&&reinterpret_cast<std::uintptr_t>(body.get())==id)return body->source_filter_borrow_v105(out,e);
 e="Required owned Openable PODecor filter fields";return false;
}
namespace {bool required(const char* name,std::string& e){e=std::string("Required actual OpenableContainer graph ")+name;return false;}}
CanonicalOpenableGraphV21::CanonicalOpenableGraphV21(CanonicalOpenableGraphServicesV21 s)
 :services_(std::move(s)),receiver_(services_.world,runtime_,container_services()){
 visual_=std::make_shared<RetainedSceneVisualConnectionV3>(receiver_.base(),services_.visual,services_.roots);
 assets_=std::make_unique<GameObjectVisualAssetOwnerV1>(receiver_.base(),visual_->services(visual_));
 initialization_=std::make_unique<GameObjectInitializationOwnerV1>(receiver_.base(),initialization_services());
}
CanonicalOpenableGraphV21::~CanonicalOpenableGraphV21(){std::string e;release(e);}
bool CanonicalOpenableGraphV21::ready(std::string& e)const{
 return services_.world&&services_.roots&&services_.physics&&services_.physics->backend()?true:required("same World/scene/PhysicalWorld owners",e);
}
OpenableContainerServicesV1 CanonicalOpenableGraphV21::container_services(){
 auto s=services_.container;s.owner=services_.world;
 s.game_object_init_post=[this](auto& e){bool eligible{};return initialization_->init_post(eligible,e);};
 s.game_object_init_final=[this](auto& e){bool eligible{};return initialization_->init_final(eligible,e);};
 s.has_visual=[this](bool& out,auto& e){auto* p=receiver_.base().pointer(0x2d8);if(!p)return required("base visual2d8",e);out=*p!=0;return true;};
 // Source474d10 branches to SetCallbacksOnAll474cac on the actual list.
 s.bind_timeline_callbacks=[this](auto& e){
  std::shared_ptr<RetainedGenericAnimatorV21> a;if(!animator(a,e))return false;
  std::weak_ptr<CanonicalOpenableGraphV21> weak=shared_from_this();
  GenericAnimationCallbacksV21 f;
  f.completion=[weak](timeline::State& t,std::string& error){auto g=weak.lock();if(!g)return required("completion receiver lifetime",error);return g->receiver_.receiver().animation_finished(t.loop!=0,error);};
  f.event=[weak](const animation::TriggeredEvent& event,std::string& error){auto g=weak.lock();if(!g)return required("authored event receiver lifetime",error);return g->receiver_.receiver().animation_event(event.name,error);};
  return a->set_callbacks(std::move(f),e);
 };
 // Container named calls pass loop0 (39fa0c/10,39faa0,39fac8 and
 // corresponding spawn/activate/idleactive paths), unlike AnimatedDecor.
 s.play_animation=[this](const char* name,bool& accepted,auto& e){auto v=visual();if(!v)return required("attached named controller",e);return v->play(name,false,accepted,e);};
 s.apply_mesh_box=[this](auto& e){auto v=visual();return v?v->apply_mesh_box(e):required("ApplyMeshBox visual",e);};
 s.scene_flags=[this](std::uint32_t clear,std::uint32_t set,auto& e){auto v=visual();if(!v)return required("Container SetState scene11c",e);v->scene_flags()=(v->scene_flags()&~clear)|set;return true;};
 s.create_attach_po_decor=[this](auto& e){return physical(e);};
 s.detach_physical=[this](auto& e){auto* p=receiver_.base().pointer(0x2dc);if(!p)return required("physical2dc",e);if(!*p)return true;for(auto& body:bodies_)if(*p==reinterpret_cast<std::uintptr_t>(body.get()))return body->detach(e);return required("registered same PODecor detach receiver",e);};
 return s;
}
GameObjectInitializationServicesV1 CanonicalOpenableGraphV21::initialization_services(){
 auto s=services_.initialization;s.owner=services_.world;
 s.load_visual=[this](auto& e){return assets_->load_visual(e);};
 s.set_position=[this](const float* p,bool destination,auto& e){return set_position(p,destination,e);};
 s.visual_sync=[this](std::uintptr_t id,auto& e){auto v=visual();auto* p=receiver_.base().pointer(0x2d8);return v&&p&&*p==id?v->sync(e):required("same visual Sync identity",e);};
 s.set_visible=[this](bool value,auto& e){return set_visible(value,e);};
 s.visual_set_light_set=[this](std::uintptr_t id,std::int32_t value,auto& e){auto v=visual();auto* p=receiver_.base().pointer(0x2d8);if(!v||!p||*p!=id)return required("same visual light-set40",e);v->store_light_set(value);return true;};
 s.visual_root=[this](std::uintptr_t id,std::uintptr_t& root,auto& e){auto v=visual();auto* p=receiver_.base().pointer(0x2d8);if(!v||!p||*p!=id)return required("same visual Root",e);root=v->root_identity();return true;};
 s.node_from_name=[this](std::uintptr_t root,const char* name,std::uintptr_t& node,auto& e){auto v=visual();return v&&v->root_identity()==root?v->node_from_name(name,node,e):required("same root node lookup",e);};
 return s;
}
bool CanonicalOpenableGraphV21::set_visible(bool value,std::string& e){
 auto& b=receiver_.base();value=value&&b.lifecycle().enabled8a;
 if(!b.store_byte(0x80,value?1:0,e))return false;
 auto* p=b.pointer(0x2d8);if(!p)return required("visual visibility field",e);if(!*p)return true;
 auto v=visual();if(!v)return required("retained visual visibility",e);
 if(value&&!b.lifecycle().non_zonable2ed&&b.lifecycle().zoning2ee&&!b.lifecycle().entered2f0)value=false;
 return v->set_root_local_visibility_v3(value,e);
}
bool CanonicalOpenableGraphV21::destroy_physical(std::uintptr_t id,std::string& e){
 for(auto& b:bodies_)if(reinterpret_cast<std::uintptr_t>(b.get())==id)return b->release(e);
 return services_.decor.destroy_previous?services_.decor.destroy_previous(id,e):required("previous PhysicalObject destructor",e);
}
bool CanonicalOpenableGraphV21::physical(std::string& e){
 if(!ready(e))return false;auto v=visual();if(!v)return required("PODecor completed attached visual",e);
 auto s=services_.decor;s.owner=services_.world;s.destroy_previous=[this](auto id,auto& error){return destroy_physical(id,error);};
 // Each source call allocates a fresh PODecor. Retain failed candidates and
 // perform previous-body destruction in the source assignment continuation.
 bodies_.push_back(std::make_unique<RetainedGameObjectDecorV1>(receiver_.base(),*v,*services_.physics,std::move(s)));
 return bodies_.back()->initialize(e);
}
bool CanonicalOpenableGraphV21::set_position(const float* p,bool destination,std::string& e){
 auto s=services_.position;s.owner=services_.world;
 s.visual_sync=[this](auto id,auto& error){auto v=visual();auto* at=receiver_.base().pointer(0x2d8);return v&&at&&*at==id?v->sync(error):required("SetPosition same visual",error);};
 s.physical_position=[this](auto id,float x,float y,auto& error){for(auto& b:bodies_)if(reinterpret_cast<std::uintptr_t>(b.get())==id){const float xy[]{x,y};return dh2_native_body_set_position(&b->native(),xy)>=0?true:required("actual PODecor SetPosition",error);}return required("SetPosition same physical receiver",error);};
 return game_object_set_position_v2(receiver_.base(),p,destination,s,e);
}
bool CanonicalOpenableGraphV21::borrow_native_body_v69(std::uintptr_t id,
 std::shared_ptr<void>& pin,physical::NativeBody*& out,std::string& e){
 pin.reset();out=nullptr;const auto* actual=receiver_.base().pointer(0x2dc);
 if(!id||!actual||*actual!=id)return required("same assigned physical2dc",e);
 for(const auto& body:bodies_)if(reinterpret_cast<std::uintptr_t>(body.get())==id){
  if(!body->assigned()||!body->native().body)return required("live owned PODecor native body",e);
  pin=shared_from_this();out=&body->native();e.clear();return true;
 }
 return required("registered PODecor owner",e);
}
CanonicalClassReceiverV1 CanonicalOpenableGraphV21::factory_receiver(){
 auto self=shared_from_this();auto alias=std::shared_ptr<CanonicalOpenableContainerV1>(self,&receiver_);
 auto out=canonical_class_receiver_v1(alias);
 bind_gameobject_loading_fields_v95(alias,false,out);
 out.source_init_final_v95=[self](std::string& e){return self->init_final(e);};
 out.init_post=[self](auto& e){return self->receiver_.receiver().init_post(e);};
 out.is_game_object=[](bool& value,auto&){value=true;return true;}; // actual GameObject virtual
 out.position=[self](auto& out,auto&){std::copy_n(self->runtime_.subobjects.position,3,out.begin());return true;};
 out.set_position=[self](const auto& value,bool destination,auto& e){return self->set_position(value.data(),destination,e);};
 return out;
}
bool CanonicalOpenableGraphV21::animator(std::shared_ptr<RetainedGenericAnimatorV21>& out,std::string& e){
 auto v=visual();if(!v)return required("attached generic animator",e);
 if(animator_visual_.lock()!=v){
  auto candidate=std::make_shared<RetainedGenericAnimatorV21>(v);
  if(!candidate->initialize(e))return false;
  animator_=std::move(candidate);animator_visual_=v;
 }
 out=animator_;return true;
}
bool CanonicalOpenableGraphV21::frame(std::uint32_t ms,std::string& e){
 std::shared_ptr<RetainedGenericAnimatorV21> a;return animator(a,e)&&a->frame(ms,e);
}
bool CanonicalOpenableGraphV21::release(std::string& e){
 if(animator_)animator_->clear();animator_.reset();animator_visual_.reset();
 for(auto i=bodies_.rbegin();i!=bodies_.rend();++i)if(!(*i)->release(e))return false;bodies_.clear();
 if(assets_&&!assets_->set_visual(std::uintptr_t{0},e))return false;
 return !visual_||visual_->discard_unattached(e);
}
}

namespace dh2::world {
bool CanonicalOpenableGraphV21::physical_peer_v90(void* context,std::uintptr_t& id,std::shared_ptr<void>& pin,bool& handled,std::string& e){
 handled=false;pin.reset();
 for(const auto& body:bodies_)if(body&&body->world_object().context==context){
  pin=shared_from_this();id=body->source_owner_base_v49().identity();handled=true;e.clear();return true;
 }
 e.clear();return true;
}
}

namespace dh2::world {
bool CanonicalOpenableGraphV21::destroy_visual_source_v92(std::uintptr_t id,std::string& e){
 const auto cell=receiver_.base().pointer(0x2d8);if(!id||!cell||*cell!=id||!assets_)return required("same assigned visual D0 receiver",e);
 if(animator_)animator_->clear();animator_.reset();animator_visual_.reset();
 return assets_->set_visual(0,e);
}
bool CanonicalOpenableGraphV21::destroy_physical_source_v92(std::uintptr_t id,std::string& e){
 auto p=std::find_if(bodies_.begin(),bodies_.end(),[id](const auto& body){return reinterpret_cast<std::uintptr_t>(body.get())==id;});
 if(p==bodies_.end())return required("same owned assigned PhysicalObject D0",e);
 if(!(*p)->release(e))return false;bodies_.erase(p);e.clear();return true;
}
bool CanonicalOpenableGraphV21::require_resources_released_v92(std::string& e)const{
 if(animator_||!bodies_.empty()||(visual_&&visual_->retained_count()))return required("real owned animation/body/visual prefix teardown",e);
 e.clear();return true;
}
}

namespace dh2::world {
bool CanonicalOpenableGraphV21::discard_unpublished_source_v92(std::uint32_t offset,std::string& e){
 if(offset==0x2d8)return !visual_||(visual_->discard_unattached(e)&&visual_->discard_failed(e));
 if(offset==0x2dc){auto slot=receiver_.base().pointer(0x2dc);if(!slot||*slot)return required("NULL assigned body before failed/unassigned body journal drain",e);
  for(auto p=bodies_.rbegin();p!=bodies_.rend();++p)if(!(*p)->release(e))return false;bodies_.clear();}
 e.clear();return true;
}
}
