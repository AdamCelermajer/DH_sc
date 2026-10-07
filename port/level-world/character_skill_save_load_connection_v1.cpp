#include "character_skill_save_load_connection_v1.hpp"
namespace dh2::character::skills {
CharacterSkillSaveLoadConnectionV1::CharacterSkillSaveLoadConnectionV1(data::PlayerSaveLoadOwnerV1& load,std::shared_ptr<void> owner,decltype(selected_) selected):load_(load),selected_owner_(std::move(owner)),selected_(std::move(selected)){}
SkillSaveReloadServicesV6 CharacterSkillSaveLoadConnectionV1::services()noexcept{return {this,selected,load};}
bool CharacterSkillSaveLoadConnectionV1::load_mask(std::uint32_t mask,std::string& error){return load_.load(static_cast<std::int32_t>(mask),error);}
int CharacterSkillSaveLoadConnectionV1::selected(void* context,data::PlayerSavegameV1* save,std::uintptr_t character,const std::vector<std::int32_t>** out){
 auto& self=*static_cast<CharacterSkillSaveLoadConnectionV1*>(context);
 if(!out||save!=&self.load_.save()||character!=save->character()||!self.selected_owner_||!self.selected_){self.error_="Required SAME Character selected skill list owner unavailable";return -1;}
 const std::vector<std::int32_t>* list=nullptr;
 if(!self.selected_(character,list,self.error_)||!list)return -1;
 *out=list;return 0;
}
int CharacterSkillSaveLoadConnectionV1::load(void* context,data::PlayerSavegameV1* save,std::uint32_t mask){
 auto& self=*static_cast<CharacterSkillSaveLoadConnectionV1*>(context);
 if(save!=&self.load_.save()){self.error_="Skill reload attempted a different Save authority";return -1;}
 return self.load_mask(mask,self.error_)?0:-1;
}
}
