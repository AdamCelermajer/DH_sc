#include "canonical_decor_physical_connection_v49.hpp"
namespace dh2::world {
CanonicalDecorPhysicalConnectionV49::CanonicalDecorPhysicalConnectionV49(
 CanonicalGameObjectBaseOwnerV1& base,physical::NativeWorld& physics,
 character::LootPhysicalAssociationsV49& associations,std::weak_ptr<void> lease,
 std::function<std::shared_ptr<RetainedGameObjectVisualV1>(std::uintptr_t)> visual,
 RetainedGameObjectDecorServicesV1 source)
 :base_(base),physics_(physics),associations_(associations),parent_lease_(std::move(lease)),
 visual_(std::move(visual)),source_(std::move(source)){}
bool CanonicalDecorPhysicalConnectionV49::borrow_visual(std::uintptr_t id,
 std::shared_ptr<RetainedGameObjectVisualV1>& out,std::string& e){
 if(!id||!visual_||parent_lease_.expired()){e="Required SAME Decor parent/visual source lifetime";return false;}
 auto actual=visual_(id);
 if(!actual||reinterpret_cast<std::uintptr_t>(actual.get())!=id||!actual->ready()){
  e="Required actual retained Decor VisualObject identity";return false;
 }
 out=std::move(actual);return true;
}
bool CanonicalDecorPhysicalConnectionV49::construct(CanonicalGameObjectBaseOwnerV1& parent,
 std::uintptr_t& out,std::string& e){
 if(&parent!=&base_){e="PODecor constructor must borrow SAME canonical base";return false;}
 auto lease=parent_lease_.lock();if(!lease){e="Required actual Decor receiver lease";return false;}
 auto* slot=base_.pointer(0x2d8);std::shared_ptr<RetainedGameObjectVisualV1> visual;
 if(!slot||!borrow_visual(*slot,visual,e))return false;
 auto source=source_;
 // Source old-body destruction keeps external context and releases only a
 // recognized same class body through this owned source lifecycle.
 source.destroy_previous=[this](std::uintptr_t id,std::string& error){
  if(bodies_.find(id)!=bodies_.end())return destroy(id,error);
  if(!source_.destroy_previous){error="Required actual foreign previous PhysicalObject destructor";return false;}
  return source_.destroy_previous(id,error);
 };
 auto body=std::make_unique<CanonicalPodDecorBodyV49>(base_,*visual,physics_,std::move(source));
 const auto id=reinterpret_cast<std::uintptr_t>(body.get());bodies_.emplace(id,std::move(body));creation_order_.push_back(id);
 if(!associations_.constructed_pod(*bodies_.at(id),lease,e)||!bodies_.at(id)->construct(e))return false;
 out=id;return true;
}
bool CanonicalDecorPhysicalConnectionV49::assign(std::uintptr_t id,bool pin,std::string& e){
 auto body=bodies_.find(id);if(body==bodies_.end()){e="Required SAME constructed PODecor SetPhysicalObject receiver";return false;}
 return body->second->assign(pin,e);
}
bool CanonicalDecorPhysicalConnectionV49::destroy(std::uintptr_t id,std::string& e){
 auto body=bodies_.find(id);if(body==bodies_.end()){e="Required actual owned PODecor destructor";return false;}
 if(!body->second->release(e))return false;
 associations_.released(body->second.get());bodies_.erase(body);return true;
}
void CanonicalDecorPhysicalConnectionV49::bind(DecorServicesV15& services){
 services.visual_sync=[this](std::uintptr_t id,std::string& e){std::shared_ptr<RetainedGameObjectVisualV1> visual;if(!borrow_visual(id,visual,e))return false;return visual->apply_mesh_box(e);};
 services.visual_physical=[this](std::uintptr_t id,bool& out,std::string& e){std::shared_ptr<RetainedGameObjectVisualV1> visual;if(!borrow_visual(id,visual,e))return false;out=visual->marker().found!=0;return true;};
 services.construct_podecor=[this](CanonicalGameObjectBaseOwnerV1& base,std::uintptr_t& out,std::string& e){return construct(base,out,e);};
 services.set_physical=[this](std::uintptr_t id,bool pin,std::string& e){return assign(id,pin,e);};
 services.visual_root=[this](std::uintptr_t id,std::uintptr_t& out,std::string& e){std::shared_ptr<RetainedGameObjectVisualV1> visual;if(!borrow_visual(id,visual,e))return false;out=visual->root_identity();return true;};
}
bool CanonicalDecorPhysicalConnectionV49::borrow_native_body(std::uintptr_t id,std::shared_ptr<void>& pin,physical::NativeBody*& out,std::string& e){
 pin.reset();out=nullptr;auto same=parent_lease_.lock();const auto* actual=base_.pointer(0x2dc);
 if(!same||!id||!actual||*actual!=id){e="Required SAME live Decor physical2dc receiver";return false;}
 auto found=bodies_.find(id);
 if(found==bodies_.end()||!found->second||!found->second->assigned()||&found->second->source_base()!=&base_){e="Required actual assigned owned PODecor native-body facet";return false;}
 pin=std::move(same);out=&found->second->native();e.clear();return true;
}
bool CanonicalDecorPhysicalConnectionV49::release(std::string& e){
 // Preserve source allocation order even though native addresses are unordered.
 for(auto it=creation_order_.rbegin();it!=creation_order_.rend();++it)
  if(bodies_.find(*it)!=bodies_.end()&&!destroy(*it,e))return false;
 creation_order_.clear();return true;
}
bool CanonicalDecorPhysicalConnectionV49::create_door_physical(std::string& e){
 auto lease=parent_lease_.lock();if(!lease){e="Required SAME Door receiver lease";return false;}
 auto source=source_;
 source.destroy_previous=[this](std::uintptr_t id,std::string& error){
  if(bodies_.find(id)!=bodies_.end())return destroy(id,error);
  if(!source_.destroy_previous){error="Required actual previous Door PhysicalObject destructor";return false;}
  return source_.destroy_previous(id,error);
 };
 auto body=std::make_unique<CanonicalPodDecorBodyV49>(base_,physics_,std::move(source));
 const auto id=reinterpret_cast<std::uintptr_t>(body.get());bodies_.emplace(id,std::move(body));creation_order_.push_back(id);
 // Retain the allocation and publish genuine owner8 before CreateShape;
 // constructor/assignment failures drain through the existing native journal.
 if(!associations_.constructed_pod(*bodies_.at(id),lease,e)||!bodies_.at(id)->construct_door(e))return false;
 if(!assign(id,false,e))return false;
 // SetPhysicalObject's MP_NoPhysics branch genuinely invokes this fresh D0.
 if(!bodies_.at(id)->assigned())return destroy(id,e);
 return true;
}
bool CanonicalDecorPhysicalConnectionV49::source_filter_borrow_v105(std::uintptr_t id,physical::NativePhysicalFilterBorrowV1& out,std::string& e){
 auto same=parent_lease_.lock();const auto* slot=base_.pointer(0x2dc);const auto body=bodies_.find(id);
 if(!same||!slot||*slot!=id||body==bodies_.end()||!body->second){e="Required SAME actual PODecor source filter owner";return false;}
 return body->second->source_filter_borrow_v105(out,e);
}
}

