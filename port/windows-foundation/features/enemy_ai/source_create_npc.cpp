#include "source_create_npc.hpp"

#include <cstdio>
#include <limits>

namespace dh2::enemy_ai {
namespace {
std::uint32_t source_auto_name_counter{}; // CreateNPC function-static BSS counter.
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_receiver(const world::CanonicalClassReceiverV1& receiver,
 std::uintptr_t identity,const std::shared_ptr<const void>& lease){
 return identity&&receiver.object.identity==identity&&receiver.object.lease&&lease&&
  !receiver.object.lease.owner_before(lease)&&!lease.owner_before(receiver.object.lease)&&
  receiver.object.lease.get()==lease.get();
}
}

bool source_create_npc_v1(std::int32_t character_id,const char* supplied_name,
 bool description_field,bool network,const SourceCreateNpcServicesV1& services,
 SourceCreateNpcResultV1& out,std::string& error){
 error.clear();
 if(!services.characters||!services.source_manager_spawn)
  return fail(error,"Required same CharacterTable and source ObjectManager::Spawn/canonical Character factory");
 const auto& table=*services.characters;
 if(character_id<0||static_cast<std::size_t>(character_id)>=table.names.size()||
    static_cast<std::size_t>(character_id)>=table.rows.size())
  return fail(error,"Character::CreateNPC CharacterTable index outside same source table");

 // 0x3ad1f8 selects CharacterTable[id]. The null-name branch in 0x3ad198
 // increments its one process-static counter before sprintf("%04u", counter).
 std::string generated;
 const char* spawn_name=supplied_name;
 if(!spawn_name){
  ++source_auto_name_counter;
  char buffer[32]{};
  const int written=std::snprintf(buffer,sizeof(buffer),"%04u",source_auto_name_counter);
  if(written<0||static_cast<std::size_t>(written)>=sizeof(buffer))
   return fail(error,"Source CreateNPC generated ObjectManager name overflow");
  generated.assign(buffer,static_cast<std::size_t>(written));
  spawn_name=generated.c_str();
 }

 SourceCreateNpcResultV1 next;
 next.character_id=character_id;next.generated_name=generated;
 bool created=false;
 // Full source overload calls ObjectManager::Spawn("Character", name,
 // deferred=true, network=<caller bool>). For `_Summon`, that final bool is
 // false. Spawn's own C1/factory/manager prefix remains the caller's service.
 if(!services.source_manager_spawn("Character",spawn_name,true,network,
    next.receiver,created,error)){
  if(error.empty())error="Original ObjectManager::Spawn required service failed";
  return false;
 }
 if(!created){out=std::move(next);return true;} // Source ObjectHandle cast is NULL.
 if(!next.receiver.object.identity||!next.receiver.object.lease||
    !next.receiver.source_lease||!next.receiver.properties||!next.receiver.init_post||
    !next.receiver.source_init_final_v95)
  return fail(error,"Required same retained Character receiver, CString fields, InitPost and InitFinal");
 const auto identity=next.receiver.object.identity;
 const auto lease=next.receiver.object.lease;

 // 0x3ad104 writes the looked-up CharacterTable name into Character+0x1398
 // when its internal selector is false (the exact `_Summon` call), or +0x13b0
 // when true. These are source Character CString fields, not the object name.
 const std::uint32_t source_name_field=description_field?0x13b0u:0x1398u;
 auto properties=next.receiver.properties();
 if(!properties.fields.context||!properties.fields.write_string||
    !properties.fields.write_string(properties.fields.context,source_name_field,
      table.names[static_cast<std::size_t>(character_id)],error)){
  if(error.empty())error="Required source Character CString write at CreateNPC";
  return false;
 }
 if(!same_receiver(next.receiver,identity,lease))
  return fail(error,"CreateNPC source CString callback changed the retained Character receiver");

 // The original body calls Character virtual+0x1c, then virtual+0x58, then
 // CharStateMachine::SM_SetIdleState(false). Preserve each source prefix.
 if(!next.receiver.init_post(error)){
  if(error.empty())error="Character::CreateNPC virtual+0x1c InitPost failed";
  return false;
 }
 if(!same_receiver(next.receiver,identity,lease))
  return fail(error,"CreateNPC InitPost changed the retained Character receiver");
 if(!next.receiver.source_init_final_v95(error)){
  if(error.empty())error="Character::CreateNPC virtual+0x58 InitFinal failed";
  return false;
 }
 if(!same_receiver(next.receiver,identity,lease))
  return fail(error,"CreateNPC InitFinal changed the retained Character receiver");
 if(!services.set_idle_state||!services.set_idle_state(next.receiver,false,error)){
  if(error.empty())error="Required same Character SM_SetIdleState(false)";
  return false;
 }
 if(!same_receiver(next.receiver,identity,lease))
  return fail(error,"CreateNPC idle-state callback changed the retained Character receiver");
 next.created=true;
 out=std::move(next);
 return true;
}

} // namespace dh2::enemy_ai
