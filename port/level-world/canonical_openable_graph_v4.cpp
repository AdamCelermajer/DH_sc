#include "canonical_openable_graph_v4.hpp"
#include <algorithm>
namespace dh2::world {
namespace {bool required(const char* name,std::string& e){e=std::string("Required actual OpenableContainer graph ")+name;return false;}}
CanonicalOpenableGraphV4::CanonicalOpenableGraphV4(CanonicalOpenableGraphServicesV4 s)
 :services_(std::move(s)),receiver_(services_.world,runtime_,container_services()){
 visual_=std::make_shared<RetainedSceneVisualConnectionV3>(receiver_.base(),services_.visual,services_.roots);
 assets_=std::make_unique<GameObjectVisualAssetOwnerV1>(receiver_.base(),visual_->services(visual_));
 initialization_=std::make_unique<GameObjectInitializationOwnerV1>(receiver_.base(),initialization_services());
}
CanonicalOpenableGraphV4::~CanonicalOpenableGraphV4(){std::string e;release(e);}
bool CanonicalOpenableGraphV4::ready(std::string& e)const{
 return services_.world&&services_.roots&&services_.physics&&services_.physics->backend()?true:required("same World/scene/PhysicalWorld owners",e);
}
OpenableContainerServicesV1 CanonicalOpenableGraphV4::container_services(){
 auto s=services_.container;s.owner=services_.world;
 s.game_object_init_post=[this](auto& e){bool eligible{};return initialization_->init_post(eligible,e);};
 s.game_object_init_final=[this](auto& e){bool eligible{};return initialization_->init_final(eligible,e);};
 s.has_visual=[this](bool& out,auto& e){auto* p=receiver_.base().pointer(0x2d8);if(!p)return required("base visual2d8",e);out=*p!=0;return true;};
 // Original base AnimController::SetCallbacks474d10 is bx lr. The actual
 // timeline is not assigned invented opened/completion callback events.
 s.bind_timeline_callbacks=[](auto&){base_named_animation_set_callbacks_v1();return true;};
 // Container named calls pass loop0 (39fa0c/10,39faa0,39fac8 and
 // corresponding spawn/activate/idleactive paths), unlike AnimatedDecor.
 s.play_animation=[this](const char* name,bool& accepted,auto& e){auto v=visual();if(!v)return required("attached named controller",e);return v->play(name,false,accepted,e);};
 s.apply_mesh_box=[this](auto& e){auto v=visual();return v?v->apply_mesh_box(e):required("ApplyMeshBox visual",e);};
 s.scene_flags=[this](std::uint32_t set,std::uint32_t clear,auto& e){auto v=visual();if(!v)return required("Container SetState scene11c",e);v->scene_flags()=(v->scene_flags()|set)&~clear;return true;};
 s.create_attach_po_decor=[this](auto& e){return physical(e);};
 s.detach_physical=[this](auto& e){auto* p=receiver_.base().pointer(0x2dc);if(!p)return required("physical2dc",e);if(!*p)return true;for(auto& body:bodies_)if(*p==reinterpret_cast<std::uintptr_t>(body.get()))return body->detach(e);return required("registered same PODecor detach receiver",e);};
 return s;
}
GameObjectInitializationServicesV1 CanonicalOpenableGraphV4::initialization_services(){
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
bool CanonicalOpenableGraphV4::set_visible(bool value,std::string& e){
 auto& b=receiver_.base();value=value&&b.lifecycle().enabled8a;
 if(!b.store_byte(0x80,value?1:0,e))return false;
 auto* p=b.pointer(0x2d8);if(!p)return required("visual visibility field",e);if(!*p)return true;
 auto v=visual();if(!v)return required("retained visual visibility",e);
 if(value&&!b.lifecycle().non_zonable2ed&&b.lifecycle().zoning2ee&&!b.lifecycle().entered2f0)value=false;
 return v->set_root_local_visibility_v3(value,e);
}
bool CanonicalOpenableGraphV4::destroy_physical(std::uintptr_t id,std::string& e){
 for(auto& b:bodies_)if(reinterpret_cast<std::uintptr_t>(b.get())==id)return b->release(e);
 return services_.decor.destroy_previous?services_.decor.destroy_previous(id,e):required("previous PhysicalObject destructor",e);
}
bool CanonicalOpenableGraphV4::physical(std::string& e){
 if(!ready(e))return false;auto v=visual();if(!v)return required("PODecor completed attached visual",e);
 auto s=services_.decor;s.owner=services_.world;s.destroy_previous=[this](auto id,auto& error){return destroy_physical(id,error);};
 // Each source call allocates a fresh PODecor. Retain failed candidates and
 // perform previous-body destruction in the source assignment continuation.
 bodies_.push_back(std::make_unique<RetainedGameObjectDecorV1>(receiver_.base(),*v,*services_.physics,std::move(s)));
 return bodies_.back()->initialize(e);
}
bool CanonicalOpenableGraphV4::set_position(const float* p,bool destination,std::string& e){
 auto s=services_.position;s.owner=services_.world;
 s.visual_sync=[this](auto id,auto& error){auto v=visual();auto* at=receiver_.base().pointer(0x2d8);return v&&at&&*at==id?v->sync(error):required("SetPosition same visual",error);};
 s.physical_position=[this](auto id,float x,float y,auto& error){for(auto& b:bodies_)if(reinterpret_cast<std::uintptr_t>(b.get())==id){const float xy[]{x,y};return dh2_native_body_set_position(&b->native(),xy)>=0?true:required("actual PODecor SetPosition",error);}return required("SetPosition same physical receiver",error);};
 return game_object_set_position_v2(receiver_.base(),p,destination,s,e);
}
CanonicalClassReceiverV1 CanonicalOpenableGraphV4::factory_receiver(){
 auto self=shared_from_this();auto alias=std::shared_ptr<CanonicalOpenableContainerV1>(self,&receiver_);
 auto out=canonical_class_receiver_v1(alias);
 out.init_post=[self](auto& e){return self->receiver_.receiver().init_post(e);};
 out.is_game_object=[](bool& value,auto&){value=true;return true;}; // actual GameObject virtual
 out.position=[self](auto& out,auto&){std::copy_n(self->runtime_.subobjects.position,3,out.begin());return true;};
 out.set_position=[self](const auto& value,bool destination,auto& e){return self->set_position(value.data(),destination,e);};
 return out;
}
bool CanonicalOpenableGraphV4::release(std::string& e){
 for(auto i=bodies_.rbegin();i!=bodies_.rend();++i)if(!(*i)->release(e))return false;bodies_.clear();
 if(assets_&&!assets_->set_visual(std::uintptr_t{0},e))return false;
 return !visual_||visual_->discard_unattached(e);
}
}
