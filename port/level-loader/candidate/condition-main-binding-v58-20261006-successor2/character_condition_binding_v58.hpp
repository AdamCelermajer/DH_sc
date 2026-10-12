#pragma once
#include "character_parent_init_post_v51.hpp"
#include "condition_data_kernel_v51.hpp"
#include "condition_list_owner_v55.hpp"
namespace dh2::loader {
// Configure only: no C1, Init, Eval, Clear, event, save or quest execution.
// The explicit actor and checked V48 view must identify the SAME receiver.
inline bool bind_character_conditions_v58(
 const std::shared_ptr<character::RetainedCharacterActorV1>& actor,
 const character::CharacterParentInitFieldsV48& actual,
 const std::shared_ptr<world::ConditionListOwnerV55>& lists,
 world::GameObjectInitializationServicesV1& services,std::string& error){
 if(!actor||!actual.receiver_lease||actual.receiver_lease.get()!=actor.get()||actual.identity!=actor->shared_handle().cached||!lists){error="Required pinned SAME Character identity/alias and actual ConditionList owner";return false;}
 character::CharacterParentInitFieldsV48 checked;
 if(!actor->parent_init_fields_v48(std::shared_ptr<const void>(actor,actor.get()),checked,error))return false;
 for(unsigned i=0;i<2;++i){const auto& condition=actual.conditions[i];const auto& same=checked.conditions[i];
  if(condition.source_offset!=(i?0xb0u:0x8cu)||condition.name!=same.name||condition.compiled!=same.compiled||condition.tested!=same.tested){error="Required SAME checked Character ConditionData8c/b0 cells";return false;}
 }
 auto fields=world::character_gameobject_fields_v51(checked);
 auto concrete=lists->services();
 services.condition_init=[fields,concrete,lists](std::uint32_t offset,std::string& e)mutable{
  return world::condition_data_init_borrow_v51(fields,offset,concrete,e);
 };
 error.clear();return true;
}
// Main's production call site uses this composition, so callback and qualified
// parent cannot receive two different actors. Services are a local copy;
// the existing V51 route enforces its complete reached ordered request guard.
inline bool character_parent_init_post_with_conditions_v58(
 world::CanonicalCharacterRecordV4& record,
 const character::NpcInitPostRequestV1& request,character::NpcInitPostResponseV1& response,
 const std::shared_ptr<world::ConditionListOwnerV55>& lists,
 world::GameObjectInitializationServicesV1 services,std::string& error){
 if(request.source_entry!=0x38be5c||!record.actor){error="Required SAME reached qualified Character parent InitPost38be5c request";return false;}
 character::CharacterParentInitFieldsV48 actual;
 if(!record.actor->parent_init_fields_v48(std::shared_ptr<const void>(record.actor,record.actor.get()),actual,error))return false;
 if(!bind_character_conditions_v58(record.actor,actual,lists,services,error))return false;
 return character_parent_init_post_v51(record,request,response,services,error);
}
}