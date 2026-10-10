#include "source_create_npc.hpp"

namespace dh2::enemy_ai {

bool source_canonical_manager_spawn_v1(SourceCanonicalSpawnOwnerV1& owner,
 const char* type,const char* name,bool deferred,bool network,
 world::CanonicalClassReceiverV1& receiver,bool& created,std::string& error){
 created=false;receiver={};
 if(!owner.manager||!owner.properties){error="Required same canonical ObjectManager and PropertyMap for source Spawn";return false;}
 world::CanonicalSpawnAttemptV1 attempt(*owner.manager,*owner.properties,owner.spawn);
 if(!attempt.spawn(type,name,deferred,network,error))return false;
 const auto& constructed=attempt.constructed_receiver();
 if(!constructed.object.identity){
  // CanonicalSpawnAttempt preserves source NULL allocation / unknown-type
  // behavior. CreateNPC's subsequent ObjectHandle<Character> cast is NULL.
  error.clear();return true;
 }
 const auto* published=owner.manager->object(attempt.handle().key);
 if(!published||published->identity!=constructed.object.identity||!published->lease||
    !constructed.object.lease||published->lease.get()!=constructed.object.lease.get()||
    published->lease.owner_before(constructed.object.lease)||
    constructed.object.lease.owner_before(published->lease)){
  error="Source ObjectManager::Spawn did not resolve the same canonical Character receiver";return false;
 }
 receiver=constructed;created=true;error.clear();return true;
}

} // namespace dh2::enemy_ai
