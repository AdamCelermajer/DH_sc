#pragma once
#include "canonical_character_family_v4.hpp"
#include "game_object_initialization_fields_v51.hpp"
#include "game_object_initialization_kernel_v51.hpp"
namespace dh2::loader {
// Qualified38be5c only at the existing NPC owner's reached ordered request.
// Caller composes same actual condition/device/visual/PF services. Missing
// reached providers stop; no C1/model/template shortcut or InitFinal call.
inline bool character_parent_init_post_v51(world::CanonicalCharacterRecordV4& record,
 const character::NpcInitPostRequestV1& request,character::NpcInitPostResponseV1& response,
 const world::GameObjectInitializationServicesV1& services,std::string& error){
 if(request.source_entry!=0x38be5c||request.argument0||request.argument1||request.payload||request.text||!record.actor||request.subject!=record.actor->shared_handle().cached||!record.init_attempted||!record.init||record.init->last_entry()!=0x38be5c||!record.init->error().empty()){error="Required SAME reached qualified Character parent InitPost38be5c request";return false;}
 auto alias=std::shared_ptr<const void>(record.actor,record.actor.get());character::CharacterParentInitFieldsV48 actual;
 if(!record.actor->parent_init_fields_v48(alias,actual,error))return false;
 if(record.fields.identity!=actual.identity||record.fields.position160!=actual.position160||record.fields.rotation16c!=actual.rotation16c||record.fields.scale120!=actual.scale120||record.fields.visual2d8!=actual.visual2d8||record.fields.spawn_probability274!=actual.probability274){error="Required SAME actual Character parent/NPC pose and visual backing";return false;}
 auto fields=world::character_gameobject_fields_v51(actual);bool eligible{};
 if(!world::GameObjectInitializationKernelV51<world::GameObjectInitializationFieldsV51>(fields,services).init_post(eligible,error))return false;
 response.value=eligible;error.clear();return true;
}
}
