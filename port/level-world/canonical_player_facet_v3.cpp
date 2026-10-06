#include "canonical_player_facet_v3.hpp"
namespace dh2::world {
bool CanonicalPlayerFacetV3::name(void* p,const char* value,std::string& error){
 auto& f=static_cast<CanonicalPlayerFacetV3*>(p)->fields_;
 if(!value||*f.name!=value){error="published player adoption cannot change source name";return false;}
 *f.name=value;return true;
}
bool CanonicalPlayerFacetV3::archetype(void* p,const char* value,std::string& error){
 auto& f=static_cast<CanonicalPlayerFacetV3*>(p)->fields_;
 if(!value||*f.archetype!=value){error="published player adoption cannot change source archetype";return false;}
 *f.archetype=value;return true;
}
bool CanonicalPlayerFacetV3::as_character(void* p,std::uintptr_t& result,std::string&){
 result=static_cast<CanonicalPlayerFacetV3*>(p)->fields_.object->identity;return true;
}
bool CanonicalPlayerFacetV3::borrow(std::shared_ptr<void> lease,CanonicalExistingActorV3& out,std::string& error){
 auto& f=fields_;
 if(!lease||!f.world_lease||!f.object||!f.object->identity||!f.object->properties||!f.object->life||
    !f.runtime||!f.handle||!f.type_f4||*f.type_f4!=0||!f.room64||
    !f.class_name20||!*f.class_name20||!f.name||!f.archetype||*f.name!=f.object->name){
  error="required same-player canonical source fields/class/name/runtime producers";return false;
 }
 CanonicalObjectBorrowV1 b;b.identity=f.object->identity;b.lease=std::move(lease);
 b.shared_handle=f.handle;b.type_f4=f.type_f4;b.room64=f.room64;b.across_rooms87=f.across_rooms87;
 b.class_name20=f.class_name20;b.context=this;b.set_name=name;b.set_archetype=archetype;b.as_character=as_character;
 out={std::move(b),f.name,f.archetype};return true;
}
}
