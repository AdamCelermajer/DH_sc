#include "canonical_container_target_v104.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::world {
CanonicalContainerTargetV104::CanonicalContainerTargetV104(CanonicalObjectManagerV1& manager,
 character::skills::CharacterWorldRuntimeV1& targets,CanonicalContainerTargetServicesV104 services)
 :manager_(manager),targets_(targets),services_(std::move(services)){
 if(!services_.provider||!services_.base||!services_.state394||!services_.visual||
  (services_.selected_interaction_type!=0&&services_.selected_interaction_type!=8))
  throw std::invalid_argument("Required actual selected Container target receiver services");
}
bool CanonicalContainerTargetV104::actual(CanonicalGameObjectBaseOwnerV1*& base,std::string& e){
 if(!services_.base(receiver_inflight_,base,e)||!receiver_inflight_||!base)return false;
 auto& handle=base->shared_handle();const auto* published=manager_.object(handle.key);
 if(!published||published->identity!=base->identity()||published->shared_handle!=&handle||
  (identity_&&base->identity()!=identity_)){e="Container target must borrow SAME published canonical Handle";return false;}
 return true;
}
int CanonicalContainerTargetV104::refresh(void* raw,character::skills::WorldTargetActorBorrowV1* out){
 if(!raw||!out)return -1;auto& self=*static_cast<CanonicalContainerTargetV104*>(raw);CanonicalGameObjectBaseOwnerV1* base{};
 struct Scope {CanonicalContainerTargetV104& self;~Scope(){self.receiver_inflight_.reset();self.state_inflight_.reset();self.visual_inflight_.reset();}} scope{self};
 if(!self.actual(base,self.error_))return -1;
 const auto* visible=base->byte(0x80);const auto* zoned=base->byte(0x2ee);const auto* inzone=base->byte(0x2f0);
 const auto* node=base->pointer(0x180);const auto* position=base->vector3(0x160);const auto* rotation=base->vector3(0x16c);
 if(!visible||!zoned||!inzone||!node||!position||!rotation){self.error_="Unproduced SAME Container source target pose/lifecycle";return -1;}
 if(!self.services_.visual(self.visual_inflight_,self.error_))return -1;
 //GameObject.Update owns the SAME cached184 tuple in RuntimeState. Querying
 //a Scene node here would bypass its original cache/write phase.
 const float* cached=*node&&*visible?base->runtime().target_position:nullptr;
 self.search_={};self.search_.identity=base->identity();std::copy_n(position,3,self.search_.position);
 self.search_.rotation=rotation[2];self.search_.visible=*visible;self.search_.zoned=*zoned;self.search_.in_zone=*inzone;
 if(cached){std::copy_n(cached,3,self.search_.target_position);self.search_.has_target_position=1;}
 character::skills::WorldTargetActorBorrowV1 actual;actual.identity=base->identity();actual.search=&self.search_;
 actual.scene=self.visual_inflight_?&self.visual_inflight_->scene():nullptr;
 actual.target_node=node;actual.target_enabled=visible;actual.cached_target_position=cached;actual.position=position;
 actual.heading_angle=base->scalar(0x178);actual.base_byte2ed=base->byte(0x2ed);actual.aabb6=base->absolute_aabb12c();
 struct FieldsPin {std::shared_ptr<void> receiver;std::shared_ptr<RetainedGameObjectVisualV1> visual;};
 auto pin=std::make_shared<FieldsPin>();pin->receiver=self.receiver_inflight_;pin->visual=self.visual_inflight_;
 actual.receiver_lease=std::move(pin);actual.base_target_context=&self;actual.base_target_query=query;*out=actual;return 0;
}
int CanonicalContainerTargetV104::query(void* raw,std::uint32_t op,std::uintptr_t,std::int32_t* out){
 if(!raw||!out)return -1;auto& self=*static_cast<CanonicalContainerTargetV104*>(raw);CanonicalGameObjectBaseOwnerV1* base{};
 struct Scope {CanonicalContainerTargetV104& self;~Scope(){self.receiver_inflight_.reset();self.state_inflight_.reset();}} scope{self};
 if(!self.actual(base,self.error_))return -1;
 using namespace target_providers;
 if(op==is_player||op==is_character){*out=0;return 0;} //ObjectBase33dcd8 / GameObject IsCharacter.
 if(op==interaction_type){*out=self.services_.selected_interaction_type;return 0;} //3a16cc /3a0d60.
 if(op==is_zonable){auto* byte=base->byte(0x2ed);if(!byte)return -1;return dh2_gameobject_target_query(out,4,*byte)?-1:0;}
 if(op!=is_dead&&op!=is_interactive){self.error_="Required different selected Container target query";return -1;}
 const std::int32_t* state{};if(!self.services_.state394(state,self.state_inflight_,self.error_)||!state||!self.state_inflight_)return -1;
 if(op==is_dead){*out=std::uint32_t(*state)-3u<=1u;return 0;} //39f364 unsigned range3..4.
 auto* disabled=base->byte(0x81);if(!disabled)return -1;*out=!*disabled&&*state==2;return 0; //39f384.
}
bool CanonicalContainerTargetV104::register_after_init_post(std::string& e){
 if(registered_){e="Container source target index already enrolled";return false;}
 CanonicalGameObjectBaseOwnerV1* base{};if(!actual(base,e))return false;
 identity_=base->identity();published_key_=base->shared_handle().key;
 targets_.begin_frame(manager_.source_frame78_v4());character::skills::WorldActorRegistrationV1 registration;
 registration.identity=identity_;registration.handle_key=published_key_;registration.context=this;
 registration.refresh=refresh;registration.shared_handle=&base->shared_handle();
 if(targets_.add(registration)){receiver_inflight_.reset();e=targets_.error();return false;}registered_=true;receiver_inflight_.reset();e.clear();return true;
}
bool CanonicalContainerTargetV104::retire_after_unpublication(std::string& e){
 if(!registered_){e.clear();return true;}const auto* source=manager_.object(published_key_);
 if(source&&source->identity==identity_){e="Retire Container target index only after actual canonical unpublication";return false;}
 if(targets_.remove(identity_)){e=targets_.error();return false;}registered_=false;receiver_inflight_.reset();state_inflight_.reset();visual_inflight_.reset();e.clear();return true;
}
}
