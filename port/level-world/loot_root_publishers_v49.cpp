#include "loot_root_publishers_v49.hpp"
#include <stdexcept>
namespace dh2::character {
namespace {
bool require(bool v,const char* text,std::string& e){if(v)return true;e=std::string("Required actual loot publisher V49 ")+text;return false;}
}
PlayerSourceVisibilityV49::PlayerSourceVisibilityV49(std::shared_ptr<LootPlayerFieldAssociationV47> f,
 std::shared_ptr<void> pin,const char* kind,const std::uint8_t* stat)
 :fields_(std::move(f)),actor_lease_(std::move(pin)),class_name20_(kind),static84_(stat){
 if(!fields_||!actor_lease_||!kind||!*kind||!stat)
  throw std::invalid_argument("Required SAME missing player fields/class/static84/lifetime");
}
world::CanonicalPropertyActorV1 PlayerSourceVisibilityV49::properties()noexcept{
 return {class_name20_,&template8_,{this,read_bool,write_bool,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr}};
}
bool PlayerSourceVisibilityV49::read_bool(void* raw,std::uint32_t offset,std::uint8_t& out,std::string& e){
 auto& s=*static_cast<PlayerSourceVisibilityV49*>(raw);
 if(offset==0x84){out=*s.static84_;return true;}
 if(offset==0x80&&s.fields_->visible80()){out=*s.fields_->visible80();return true;}
 e="Required existing player source bool producer at "+std::to_string(offset);return false;
}
bool PlayerSourceVisibilityV49::write_bool(void* raw,std::uint32_t offset,std::uint8_t value,std::string& e){
 auto& s=*static_cast<PlayerSourceVisibilityV49*>(raw);
 if(offset!=0x80){e="Missing-field publisher cannot replay other player defaults";return false;}
 s.fields_->source_visible_store(value);return true;
}
bool PlayerSourceVisibilityV49::publish_missing_default(world::CanonicalPropertyMapV1& map,std::string& e){
 if(fields_->visible80())return true; // restore/explicit prior authored producer wins
 if(default_attempted_){e="Retained failed missing-visible property prefix; no retry";return false;}
 default_attempted_=true;auto actor=properties();
 if(!map.init_properties(actor,e)||!map.set_property(actor,"visible",nullptr,e))return false;
 return require(fields_->visible80()!=nullptr,"lowercase visible declared descriptor/default store80",e);
}
bool PlayerSourceVisibilityV49::set_visible38b0f0(bool requested,const PlayerSourceVisibilityServicesV49& s,std::string& e){
 if(!require(s.receiver_lease&&s.visual2d8,"SAME GameObject visual2d8 slot lifetime",e))return false;
 const auto visual=*s.visual2d8; // Original loads2d8 before store80.
 const auto visible=requested?enabled8a_:std::uint8_t{0};
 fields_->source_visible_store(visible); // prefix remains on callback failure
 if(!visual)return true;
 if(!require(bool(s.sync_visibility4713d0),"VisualObject.SyncVisibility4713d0",e))return false;
 return s.sync_visibility4713d0(s.context,visual,*fields_->visible80(),e);
}
bool LootPhysicalAssociationsV49::constructed_decor(world::RetainedGameObjectDecorV1& body,
 std::shared_ptr<void> lease,std::string& e){
 auto& base=body.source_owner_base_v49();const auto* object=manager_.object(base.shared_handle().key);
 if(!require(lease&&object&&object->identity==base.identity()&&object->shared_handle==&base.shared_handle()&&
    object->lease&&body.world_object().context==&body,"actual PODecor C1 base/registered canonical owner8",e))return false;
 auto existing=records_.find(&body);
 if(existing!=records_.end()){e="PhysicalObject constructor association already published";return false;}
 records_.emplace(&body,Record{lease,&body,nullptr,nullptr});return true;
}
bool LootPhysicalAssociationsV49::constructed_pod(world::CanonicalPodDecorBodyV49& body,
 std::shared_ptr<void> lease,std::string& e){
 auto& base=body.source_base();const auto* object=manager_.object(base.shared_handle().key);
 if(!require(lease&&object&&object->identity==base.identity()&&object->shared_handle==&base.shared_handle()&&
    object->lease&&*body.source_owner8()==base.identity()&&body.transport().context==&body,
    "actual separated PODecor C1/owner8/registered parent",e))return false;
 if(records_.find(&body)!=records_.end()){e="PhysicalObject constructor association already published";return false;}
 records_.emplace(&body,Record{lease,nullptr,&body,nullptr});return true;
}
bool LootPhysicalAssociationsV49::constructed_null_owner(void* address,const std::uintptr_t* slot,
 std::shared_ptr<void> lease,std::string& e){
 if(!require(address&&slot&&!*slot&&lease,"actual recognized PhysicalObject NULL-owner8 constructor slot",e))return false;
 if(records_.find(address)!=records_.end()){e="PhysicalObject constructor association already published";return false;}
 records_.emplace(address,Record{lease,nullptr,nullptr,slot});return true;
}
bool LootPhysicalAssociationsV49::validate_pod(Record& record,LootPhysicalPeerBorrowV44& out,std::string& e){
 auto pin=record.lease.lock();if(!require(pin&&record.pod,"live separated PODecor lease",e))return false;
 auto& body=*record.pod;auto& base=body.source_base();const auto* object=manager_.object(base.shared_handle().key);
 if(!require(object&&object->lease&&object->identity==base.identity()&&
    object->shared_handle==&base.shared_handle()&&*body.source_owner8()==base.identity()&&
    body.transport().context==&body,"SAME separated PODecor parent/owner8",e))return false;
 auto* visible=base.byte(0x80);if(!require(visible,"actual PODecor visible80 source producer",e))return false;
 out={};out.object=object;out.receiver_lease=std::move(pin);out.visible80=visible;return true;
}
bool LootPhysicalAssociationsV49::constructed_physical_v112(void* address,const std::uintptr_t* owner8,
 world::CanonicalGameObjectBaseOwnerV1& base,physical::WorldObject& transport,
 physical::NativePhysicalFilterBorrowV1 filter,std::shared_ptr<void> pin,std::string& e){
 const auto* canonical=manager_.object(base.shared_handle().key);
 if(!require(address&&pin&&owner8&&*owner8==base.identity()&&transport.context==address&&
    canonical&&canonical->lease&&canonical->identity==base.identity()&&canonical->shared_handle==&base.shared_handle()&&
    filter.world&&filter.body&&filter.primary&&filter.secondary&&filter.saved&&filter.disabled,
    "actual PhysicalObject C1/base/owner8/filter lifetime",e))return false;
 if(records_.count(address)){e="Actual PhysicalObject callback association already published";return false;}
 Record record;record.lease=pin;record.nullable_owner8=owner8;record.generic_base_v112=&base;
 record.generic_world_object_v112=&transport;record.generic_filter_v112=filter;
 records_.emplace(address,std::move(record));e.clear();return true;
}
bool LootPhysicalAssociationsV49::validate_physical_v112(Record& record,LootPhysicalPeerBorrowV44& out,std::string& e){
 auto pin=record.lease.lock();auto* base=record.generic_base_v112;
 if(!require(pin&&base&&record.nullable_owner8&&*record.nullable_owner8==base->identity()&&
    record.generic_world_object_v112,"live actual PhysicalObject/base owner8",e))return false;
 const auto* canonical=manager_.object(base->shared_handle().key);
 if(!require(canonical&&canonical->lease&&canonical->identity==base->identity()&&
    canonical->shared_handle==&base->shared_handle(),"SAME canonical PhysicalObject parent",e))return false;
 auto visible=base->byte(0x80);if(!require(visible,"actual physical parent visible80 producer",e))return false;
 out={};out.object=canonical;out.receiver_lease=std::move(pin);out.visible80=visible;e.clear();return true;
}
bool LootPhysicalAssociationsV49::validate_decor(Record& record,LootPhysicalPeerBorrowV44& out,std::string& e){
 auto pin=record.lease.lock();
 if(!require(pin&&record.decor,"live physical receiver lease",e))return false;
 auto& body=*record.decor;auto& base=body.source_owner_base_v49();
 const auto* canonical=manager_.object(base.shared_handle().key);
 if(!require(canonical&&canonical->lease&&canonical->identity==base.identity()&&
   canonical->shared_handle==&base.shared_handle()&&body.world_object().context==&body,
   "SAME current published canonical PODecor parent",e))return false;
 auto* visible=base.byte(0x80);
 if(!require(visible!=nullptr,"PODecor source PropertyMap/SetVisible80",e))return false;
 // C1 stores the parent before new body construction; assignment2dc follows
 // CreateShape/Debug. Do not incorrectly require assignment during that prefix.
 out={};out.object=canonical;out.receiver_lease=std::move(pin);out.visible80=visible;return true;
}
bool LootPhysicalAssociationsV49::peer(void* address,LootPhysicalPeerBorrowV44& out,std::string& e){
 auto it=records_.find(address);
 if(!require(it!=records_.end(),"recognized exact physical callback address",e))return false;
 auto& record=it->second;
 if(record.decor)return validate_decor(record,out,e);
 if(record.pod)return validate_pod(record,out,e);
 if(record.generic_base_v112)return validate_physical_v112(record,out,e);
 auto pin=record.lease.lock();
 if(!require(pin&&record.nullable_owner8&&!*record.nullable_owner8,"still-NULL actual PhysicalObject owner8/lifetime",e))return false;
 out={};out.receiver_lease=std::move(pin);return true;
}
bool LootPhysicalAssociationsV49::fields(std::uintptr_t id,LootPhysicalPeerBorrowV44& out,std::string& e){
 if(!require(id!=0,"nonNULL canonical owner identity",e))return false;
 for(auto& entry:records_){
  auto& record=entry.second;
  if(record.decor&&!record.lease.expired()&&record.decor->source_owner_base_v49().identity()==id)
   return validate_decor(record,out,e);
  if(record.pod&&!record.lease.expired()&&record.pod->source_base().identity()==id)
   return validate_pod(record,out,e);
  if(record.generic_base_v112&&!record.lease.expired()&&record.generic_base_v112->identity()==id)
   return validate_physical_v112(record,out,e);
 }
 e="Required actual registered PODecor parent for visible/cast field query";return false;
}
bool LootPhysicalAssociationsV49::physical_contact(void* address,navigation::PhysicalContact& out,std::string& e){
 auto it=records_.find(address);if(!require(it!=records_.end(),"registered physical contact receiver",e))return false;
 auto& record=it->second;LootPhysicalPeerBorrowV44 peer_fields;
 if(!peer(address,peer_fields,e))return false;
 if(record.pod)return record.pod->physical_contact(out,e);
 if(record.generic_base_v112){
  const auto& filter=record.generic_filter_v112;
  if(!require(filter.body&&filter.body->body&&filter.primary&&filter.secondary&&filter.disabled,
      "actual live generic PhysicalObject contact fields",e))return false;
  out={};out.present=1;out.disabled=*filter.disabled;out.owner_present=1;out.owner_enabled=*peer_fields.visible80;
  auto project=[](b2Shape* shape,navigation::ContactFilter& to){if(!shape)return;const auto f=shape->GetFilterData();to={f.groupIndex,f.categoryBits,f.maskBits,1};};
  project(*filter.primary,out.primary);project(*filter.secondary,out.secondary);e.clear();return true;
 }
 e="Required source physical contact fields for reached receiver; owner8 NULL is not a filter snapshot";return false;
}
}
