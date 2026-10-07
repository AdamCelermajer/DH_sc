#include "canonical_npc_skills_v84.hpp"
#include "retained_character_actor_v1.hpp"
#include <stdexcept>
namespace dh2::character {
int CanonicalNpcSkillsV84::unload_script_v105(RetainedCharacterActorV1& actor,bool final){
 if(!actor.session)return -1;auto& source=actor.session->owner().lifecycle();
 if(!source.active||(!final&&!source.delayed))return 1;
 if(owner_->cleanup_skills()<0||owner_->cleanup_spells()<0)return -2;
 if(owner_->source_destroy_instances_v105(0)<0||owner_->source_destroy_instances_v105(1)<0)return -2;
 return actor.session->owner().source_release_active_v105();
}
CanonicalNpcSkillsV84::CanonicalNpcSkillsV84(RetainedCharacterActorV1& actual,
 data::SkillTables::Borrow skill_tables,data::FaeryTables::Borrow faery_tables,const fx::PreloadServices16& preload){
 if(!actual.object||!actual.object->properties||!actual.session||!actual.machine||!skill_tables||!faery_tables||
  actual.session->property_view().resolved!=actual.object->properties->resolved.data()||
  actual.machine->native_fsm().character!=actual.object->identity)
  throw std::invalid_argument("Required SAME canonical NPC session/properties/FSM/Skill/Faery tables");
 services_=std::make_unique<skills::CharacterSkillSessionServices>(*actual.session,preload,actual.machine->native_fsm());
 //This native helper adopts the fresh Character's real CharAI CC/D0/D1
 //constructor values, not an observed actor's defaults. Player's same fields
 //remain solely in its existing PlayerSkills authority; this helper is NPC-only.
 ai_fields_produced_v115_=actual.source_ctor_empty_skill_vectors_v84()!=nullptr;
 if(ai_fields_produced_v115_){ai_fields_v115_.current=-1;ai_fields_v115_.continued=0;ai_fields_v115_.last=0;} //3cee30/34/5c
 owner_=std::make_unique<skills::CharacterSkillOwner>(actual.object->identity,&actual.session->property_view(),skill_tables,faery_tables,services_->services());
 //No configure/Init/cleanup call here. Source C1 vectors are genuinely empty;
 //the actual AI_InitSkills3d8cfc producer subsequently fills THESE vectors.
}
}
