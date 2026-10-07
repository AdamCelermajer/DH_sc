#include "player_save_identity_authority_v29.hpp"
#include <stdexcept>
namespace dh2::player {
PlayerSaveIdentityAuthorityV29::PlayerSaveIdentityAuthorityV29(
 std::shared_ptr<data::PlayerSavegameV1> save,std::uintptr_t character,
 std::int16_t& property,const data::CharacterTable& table,data::LootRandom8V2& random,
 character::CharacterPropsIdServicesV1 source,data::PlayerSaveLoadServicesV1 reads,
 data::PlayerSaveWriteServicesV1 writes)
 :save_(std::move(save)),character_(character),property_id13c8_(&property),
 characters_(&table),random_(&random),source_(source){
 if(!save_||!character_||save_->character()!=character_||!source_.is_player)
  throw std::invalid_argument("Required same constructed player Save/metadata authority");
 source_save14e8_=reinterpret_cast<std::uintptr_t>(save_.get());
 load_=std::make_shared<data::PlayerSaveLoadOwnerV1>(save_,std::move(reads));
 write_=std::make_unique<data::PlayerSaveWriteOwnerV1>(load_,std::move(writes));
}
bool PlayerSaveIdentityAuthorityV29::source_load(void* raw,data::PlayerSavegameV1& save,
 std::int32_t mask,std::string& error){
 auto& self=*static_cast<PlayerSaveIdentityAuthorityV29*>(raw);
 if(&save!=self.save_.get()||save.character()!=self.character_){error="Required same source SG_Load Save";return false;}
 return self.load_->load(mask,error);
}
bool PlayerSaveIdentityAuthorityV29::safe_properties_id(std::int32_t& out,std::string& error){
 if(!save_||save_->character()!=character_){error="Retained Save character identity changed";return false;}
 // The exact IsPlayer callback keeps its original context. Only SG_Load is
 // redirected to the sole same-Save profile owner; no player/class fixture.
 struct Dispatch {PlayerSaveIdentityAuthorityV29& self;};Dispatch dispatch{*this};
 character::CharacterPropsIdServicesV1 services;
 services.context=&dispatch;
 services.is_player=[](void* raw,bool& value,std::string& e){auto& d=*static_cast<Dispatch*>(raw);if(!d.self.source_.is_player(d.self.source_.context,value,e))return false;if(!value){e="Required source IsPlayer=true for player Save authority";return false;}return true;};
 services.load_save=[](void* raw,data::PlayerSavegameV1& s,std::int32_t mask,std::string& e){auto& d=*static_cast<Dispatch*>(raw);return source_load(&d.self,s,mask,e);};
 services.preset=[](void*,const std::string&,const std::int16_t*&,std::uint32_t&,std::string& e){e="Player Save authority cannot substitute a non-player preset";return false;};
 return character::character_safe_props_id_v1(*property_id13c8_,"","",*characters_,save_.get(),*random_,services,out,error);
}
bool PlayerSaveIdentityAuthorityV29::set_slot(std::int32_t slot,std::string& error){
 error.clear();if(save_->character()!=character_){error="Source SG_SetSlot receiver changed";return false;}
 save_->set_slot(slot);return true;
}
bool PlayerSaveIdentityAuthorityV29::load(std::int32_t mask,std::string& error){
 if(save_->character()!=character_){error="Source SG_Load receiver changed";return false;}return load_->load(mask,error);
}
bool PlayerSaveIdentityAuthorityV29::save(std::string& error){
 if(save_->character()!=character_){error="Source SG_Save receiver changed";return false;}return write_->save(error);
}
}
