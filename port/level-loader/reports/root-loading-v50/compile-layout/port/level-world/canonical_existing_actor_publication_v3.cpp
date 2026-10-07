#include "canonical_existing_actor_publication_v3.hpp"
#include <cstring>
namespace dh2::world {
bool CanonicalExistingActorPublicationV3::publish(const CanonicalExistingActorV3& input,
 const char* name,const char* archetype,std::int32_t room,bool network,
 target_providers::Handle16& out,std::string& error){
 const auto& actor=input.actor;
 if(!actor.identity||!actor.lease||!actor.shared_handle||!actor.type_f4||!actor.room64||
    !actor.set_name||!actor.set_archetype||!actor.as_character||
    !input.source_name||!input.source_archetype||!name||!archetype){
  error="existing canonical adoption requires the same retained receiver and source fields";return false;
 }
 // Reject mutation BEFORE Add's map/Handle prefix. The receiver's adoption
 // callbacks must independently enforce this same restriction.
 if(std::strcmp(input.source_name->c_str(),name)||std::strcmp(input.source_archetype->c_str(),archetype)||*actor.room64!=room){
  error="existing canonical adoption cannot rename, change archetype or move rooms";return false;
 }
 if(attempts_.count(actor.identity)){
  error="existing canonical adoption already attempted; source Add prefix cannot be replayed";return false;
 }
 // Reject a name collision without invoking Add's deleting destructor on an
 // actor that is already live. Exact source lookup still owns room policy.
 bool conflict{};
 // Preflight must not reserve a NULL map entry: source GetObjectByName
 // skips unpublished entries, so Add would allocate a second source key.
 if(!manager_.published_name_conflict_v4(name,room,conflict,error))return false;
 if(conflict){error="existing canonical adoption name already has a published receiver";return false;}
 auto [it,inserted]=attempts_.try_emplace(actor.identity);(void)inserted;
 const bool ok=manager_.add(actor,name,archetype,room,network,out,error);
 it->second.handle=out;it->second.complete=ok;
 return ok;
}
}
