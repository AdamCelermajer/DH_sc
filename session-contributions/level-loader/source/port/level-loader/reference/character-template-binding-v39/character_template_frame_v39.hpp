#pragma once
#include "retained_character_actor_v1.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "character_template_assets_v35.hpp"
namespace dh2::loader {
// A callback-frame binding only: refs point into the genuine retained actor.
// It pins the record alias and the one existing application RNG owner. No
// detached cache cells, seed copies, C1 replay or load-stage ordering policy.
class CharacterTemplateFrameV39 {
 std::shared_ptr<character::RetainedCharacterActorV1> actor_;
 character::CharacterLoaderFieldsV38 fields_;
 std::shared_ptr<world::ApplicationSpawnRandomOwnerV4> application_;
 CharacterTemplateAssetsV35::Borrow templates_;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_player_;
public:
 static bool bind(std::shared_ptr<character::RetainedCharacterActorV1> receiver_alias,
  std::shared_ptr<world::ApplicationSpawnRandomOwnerV4> actual_application,
  const data::LootRandom8V2* actual_family_random_authority,
  CharacterTemplateAssetsV35::Borrow actual_templates,
  std::function<bool(std::uintptr_t,bool&,std::string&)> actual_is_player,
  CharacterTemplateFrameV39& out,std::string& error){
  if(!receiver_alias){error="Required actual retained Character receiver alias";return false;}
  character::CharacterLoaderFieldsV38 fields;
  if(!receiver_alias->loader_fields_v38(receiver_alias,fields,error))return false;
  if(!actual_application||actual_family_random_authority!=&actual_application->channel(0)){
   error="Required SAME existing application channel0/family RNG authority";return false;
  }
  CharacterTemplateFrameV39 candidate;candidate.actor_=std::move(receiver_alias);candidate.fields_=std::move(fields);
  candidate.application_=std::move(actual_application);candidate.templates_=std::move(actual_templates);candidate.is_player_=std::move(actual_is_player);
  out=std::move(candidate);error.clear();return true;
 }
 bool select_nonplayer_template(std::int32_t& row,std::string& error){
  if(!actor_||!fields_.properties13c8||!fields_.template13ca||!fields_.receiver_lease){error="Required produced actual Character13c8/13ca field binding";return false;}
  if(*fields_.properties13c8!=-1){row=*fields_.properties13c8;error.clear();return true;} // source cache precedes IsPlayer and table/Random
  bool player{};if(!is_player_){error="Required actual Character IsPlayer before uncached template selection";return false;}
  if(!is_player_(fields_.identity,player,error))return false;
  if(player){error="Required whole original player SG_Load/class branch; template frame cannot select it";return false;}
  const auto* name=actor_->source_string(0x1398);
  if(!name){error="Required SAME actual Character char_template1398 producer";return false;}
  if(name->empty()){error="Reached direct charpropsname branch; existing actual CharacterProperties name lookup required";return false;}
  if(!templates_){error="Required actual retained CharacterTemplateAssetsV35 snapshot";return false;}
  if(!templates_.select(*name,*fields_.properties13c8,*fields_.template13ca,application_->channel(0),error))return false;
  row=*fields_.properties13c8;error.clear();return true;
 }
};
}
