#include "canonical_class_receiver_bindings_v1.hpp"
#include <cstring>
namespace dh2::world {
CanonicalClassReceiverV1* CanonicalClassReceiverBindingsV1::find(const CanonicalObjectBorrowV1& o,std::string& e){
 auto i=receivers_.find(o.identity);if(i==receivers_.end()){e="required registered actual class receiver";return nullptr;}if(!i->second.properties){e="required actual typed property receiver";return nullptr;}return &i->second;
}
bool CanonicalClassReceiverBindingsV1::construct(void* p,const CanonicalFactoryEntryV1& f,const CanonicalSourceObjectRequestV1& q,CanonicalObjectBorrowV1& out,std::string& e){
 auto& s=*static_cast<CanonicalClassReceiverBindingsV1*>(p);
 // Original factory340800 is shared by Character and Player catalog aliases.
 // Both retain the same genuine Character constructor/property receiver; the
 // factory subsequently publishes the selected source class-name20 alias.
 auto callback=(!std::strcmp(f.name,"Character")||!std::strcmp(f.name,"Player"))?s.construction_.character:!std::strcmp(f.name,"OpenableContainer")?s.construction_.openable_container:!std::strcmp(f.name,"AnimatedDecor")?s.construction_.animated_decor:nullptr;
 if(!callback){e="required actual registered class construction: ";e+=f.name;return false;}
 CanonicalClassReceiverV1 r;
 const bool ok=callback(s.construction_.context,q,r,e);
 r.source_lease=q.source_lease;
 // Retain genuine constructor mutation prefix even when its continuation fails.
 if(r.object.identity&&r.object.lease){
  if(s.receivers_.count(r.object.identity)){e="class constructor reused a live canonical identity";return false;}
  auto result=s.receivers_.emplace(r.object.identity,std::move(r));out=result.first->second.object;
  if(!ok)return false;
  if(!out.shared_handle||!out.class_name20||!result.first->second.properties){e="required actual Handle/class-name/property receiver";return false;}
  return true;
 }
 if(ok)e="class construction did not provide retained canonical identity";
 return false;
}
bool CanonicalClassReceiverBindingsV1::init_properties(void* p,const CanonicalObjectBorrowV1& o,std::string& e){auto& s=*static_cast<CanonicalClassReceiverBindingsV1*>(p);auto* r=s.find(o,e);if(!r)return false;auto a=r->properties();return s.map_.init_properties(a,e);}
bool CanonicalClassReceiverBindingsV1::set_template(void* p,const CanonicalObjectBorrowV1& o,const char* n,std::string& e){auto& s=*static_cast<CanonicalClassReceiverBindingsV1*>(p);auto* r=s.find(o,e);if(!r)return false;auto a=r->properties();return s.map_.set_template(a,n,e);}
bool CanonicalClassReceiverBindingsV1::defaults(void* p,const CanonicalObjectBorrowV1& o,std::string& e){auto& s=*static_cast<CanonicalClassReceiverBindingsV1*>(p);auto* r=s.find(o,e);if(!r)return false;auto a=r->properties();return s.map_.load_defaults(a,e);}
bool CanonicalClassReceiverBindingsV1::overrides(void* p,const CanonicalObjectBorrowV1& o,const CanonicalSourceObjectRequestV1& q,std::string& e){auto& s=*static_cast<CanonicalClassReceiverBindingsV1*>(p);auto* r=s.find(o,e);if(!r)return false;auto a=r->properties();return s.map_.load_overrides(a,q,e);}
bool CanonicalClassReceiverBindingsV1::init_post(void* p,const CanonicalObjectBorrowV1& o,std::string& e){auto* r=static_cast<CanonicalClassReceiverBindingsV1*>(p)->find(o,e);if(!r)return false;if(!r->init_post){e="required actual class InitPost receiver";return false;}return r->init_post(e);}
bool CanonicalClassReceiverBindingsV1::is_game_object(void* p,const CanonicalObjectBorrowV1& o,bool& v,std::string& e){auto* r=static_cast<CanonicalClassReceiverBindingsV1*>(p)->find(o,e);if(!r)return false;if(!r->is_game_object){e="required actual virtual IsGameObject receiver";return false;}return r->is_game_object(v,e);}
bool CanonicalClassReceiverBindingsV1::position(void* p,const CanonicalObjectBorrowV1& o,std::array<float,3>& v,std::string& e){auto* r=static_cast<CanonicalClassReceiverBindingsV1*>(p)->find(o,e);if(!r)return false;if(!r->position){e="required same receiver position producer";return false;}return r->position(v,e);}
bool CanonicalClassReceiverBindingsV1::set_position(void* p,const CanonicalObjectBorrowV1& o,const std::array<float,3>& v,bool update,std::string& e){auto* r=static_cast<CanonicalClassReceiverBindingsV1*>(p)->find(o,e);if(!r)return false;if(!r->set_position){e="required actual source SetPosition continuation";return false;}return r->set_position(v,update,e);}
bool CanonicalClassReceiverBindingsV1::unknown(void* p,const char* n,std::string& e){auto& s=*static_cast<CanonicalClassReceiverBindingsV1*>(p);if(!s.construction_.unknown_type_debug){e="required actual unknown-type Debug provider";return false;}return s.construction_.unknown_type_debug(s.construction_.context,n,e);}
CanonicalClassServicesV1 CanonicalClassReceiverBindingsV1::services()noexcept{return {this,construct,init_properties,set_template,defaults,overrides,init_post,is_game_object,position,set_position,unknown};}
const CanonicalClassReceiverV1* CanonicalClassReceiverBindingsV1::receiver(
 const CanonicalObjectBorrowV1& object,std::string& error)const{
 if(!object.identity||!object.lease){error="required canonical receiver identity/lease for read-only lookup";return nullptr;}
 auto found=receivers_.find(object.identity);
 if(found==receivers_.end()){error="required registered actual class receiver";return nullptr;}
 const auto& retained=found->second;
 if(retained.object.identity!=object.identity||!retained.object.lease||
    retained.object.lease.get()!=object.lease.get()||
    retained.object.lease.owner_before(object.lease)||object.lease.owner_before(retained.object.lease)||
    retained.object.shared_handle!=object.shared_handle||
    retained.object.class_name20!=object.class_name20||
    retained.object.context!=object.context){
  error="read-only class receiver lookup escaped the same raw identity/handle/lease";return nullptr;
 }
 if(!retained.properties){error="required actual typed property receiver";return nullptr;}
 error.clear();return &retained;
}
}
