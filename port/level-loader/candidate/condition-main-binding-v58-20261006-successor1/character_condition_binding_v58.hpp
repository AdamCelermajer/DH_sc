#pragma once
#include "character_parent_init_post_v51.hpp"
#include "condition_data_kernel_v51.hpp"
#include "condition_list_owner_v55.hpp"
namespace dh2::loader {
// Configure only: no C1, Init, Eval, Clear, event, save or quest execution.
// Main calls character_parent_init_post_v51 solely for the qualified reached
// ordered NPC request38be5c. That kernel invokes this condition_init callback.
inline bool bind_character_conditions_v58(
 const character::CharacterParentInitFieldsV48& actual,
 const std::shared_ptr<world::ConditionListOwnerV55>& lists,
 world::GameObjectInitializationServicesV1& services,std::string& error){
 if(!actual.receiver_lease||!actual.identity||!lists){error="Required pinned SAME Character and actual ConditionList owner";return false;}
 for(unsigned i=0;i<2;++i){const auto& condition=actual.conditions[i];
  if(condition.source_offset!=(i?0xb0u:0x8cu)||!condition.name||!condition.compiled||!condition.tested){error="Required SAME source Character ConditionData8c/b0 fields";return false;}
 }
 auto fields=world::character_gameobject_fields_v51(actual);
 auto concrete=lists->services();
 services.condition_init=[fields,concrete,lists](std::uint32_t offset,std::string& e)mutable{
  return world::condition_data_init_borrow_v51(fields,offset,concrete,e);
 };
 error.clear();return true;
}
}