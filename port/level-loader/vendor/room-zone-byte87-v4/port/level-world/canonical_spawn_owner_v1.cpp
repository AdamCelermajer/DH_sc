#include "canonical_spawn_owner_v1.hpp"
#include <cstring>
namespace dh2::world {
bool CanonicalSpawnAttemptV1::resolve(bool refresh,const CanonicalObjectBorrowV1*& out,std::string& e){
 out=nullptr;if(!services_.resolve||!services_.resolve(services_.context,handle_,refresh,out,e)){if(e.empty())e="Required SAME canonical Spawn Handle resolver";return false;}
 const auto* actual=manager_.object(handle_.key);
 if(out&&(!actual||out->identity!=actual->identity||out->lease!=actual->lease)){e="Spawn resolver returned a different canonical receiver";return false;}return true;
}
bool CanonicalSpawnAttemptV1::spawn(const char* type,const char* name,bool deferred,bool network,std::string& e){
 if(attempted_||!type||!name||!services_.construct){e="Invalid/retried source Spawn attempt";return false;}attempted_=true;
 const CanonicalFactoryEntryV1* factory=nullptr;
 for(const auto& f:canonical_factories_v1())if(!std::strcmp(f.name,type)){factory=&f;break;}
 if(!factory){if(!services_.unknown_type_debug||!services_.unknown_type_debug(services_.context,type,e))return false;phase_=CanonicalSpawnPhaseV1::complete;return true;}
 phase_=CanonicalSpawnPhaseV1::constructor;
 if(!services_.construct(services_.context,*factory,receiver_,e))return false;
 auto& constructed=receiver_.object;
 if(!constructed.identity){
  // Actual source NULL allocation falls through the remaining catalog and
  // reaches unknown-type Debug. No fake InitPost or network publication.
  if(!services_.unknown_type_debug||!services_.unknown_type_debug(services_.context,type,e))return false;phase_=CanonicalSpawnPhaseV1::complete;return true;
 }
 if(!constructed.lease||!constructed.class_name20){e="Required actual constructed Spawn class-name field";return false;}
 *constructed.class_name20=factory->name;
 if(!manager_.add(constructed,name,type,-1,network,handle_,e))return false;phase_=CanonicalSpawnPhaseV1::registered;
 const CanonicalObjectBorrowV1* object{};if(!resolve(false,object,e))return false;phase_=CanonicalSpawnPhaseV1::resolve_false;
 if(!object){phase_=CanonicalSpawnPhaseV1::complete;return true;}
 // A duplicate Add returns the OLD source receiver and destroys the new one.
 // Its genuine PropertyMap/virtual methods must be borrowed by a provider,
 // never silently applied to this discarded constructed receiver.
 actual_receiver_=&receiver_;
 if(object->identity!=constructed.identity){
  actual_receiver_=nullptr;
  if(!services_.receiver||!services_.receiver(services_.context,*object,actual_receiver_,e)||!actual_receiver_||actual_receiver_->object.identity!=object->identity||actual_receiver_->object.lease!=object->lease){e="Required source Spawn existing-receiver property/virtual dispatch after duplicate Add";return false;}
 }
 if(!actual_receiver_->properties){e="Required same Spawn PropertyMap receiver";return false;}
 if(!resolve(true,object,e)||!object)return false;auto actor=actual_receiver_->properties();
 if(!properties_.init_properties(actor,e))return false;phase_=CanonicalSpawnPhaseV1::properties;
 if(!resolve(true,object,e)||!object)return false;actor=actual_receiver_->properties();
 if(!properties_.load_defaults(actor,e))return false;phase_=CanonicalSpawnPhaseV1::defaults;
 if(!resolve(true,object,e)||!object||!object->set_name||!object->set_name(object->context,name,e))return false;phase_=CanonicalSpawnPhaseV1::name;
 if(!resolve(true,object,e)||!object||!object->set_archetype||!object->set_archetype(object->context,type,e))return false;phase_=CanonicalSpawnPhaseV1::archetype;
 if(!deferred){
  if(!resolve(true,object,e)||!object||!actual_receiver_->init_post||!actual_receiver_->init_post(e))return false;phase_=CanonicalSpawnPhaseV1::init_post;
  if(!resolve(true,object,e)||!object||!services_.test_enable_condition||!services_.test_enable_condition(services_.context,*object,true,e))return false;phase_=CanonicalSpawnPhaseV1::condition;
 }
 bool accepted{};if(!resolve(true,object,e)||!object||!services_.virtual38||!services_.virtual38(services_.context,*object,accepted,e))return false;phase_=CanonicalSpawnPhaseV1::accepted;
 if(accepted){if(!resolve(false,object,e)||!object||!services_.append_pending||!services_.append_pending(services_.context,*object,e))return false;phase_=CanonicalSpawnPhaseV1::pending;}
 phase_=CanonicalSpawnPhaseV1::complete;return true;
}
}
