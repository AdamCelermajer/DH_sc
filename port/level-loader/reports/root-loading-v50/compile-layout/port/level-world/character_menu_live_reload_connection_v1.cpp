#include "character_menu_live_reload_connection_v1.hpp"
#include <stdexcept>
namespace dh2::character::skills {
CharacterMenuLiveReloadConnectionV1::CharacterMenuLiveReloadConnectionV1(std::uintptr_t character,CharacterPlayerSkillsV6& skills,CharacterSkillSaveLoadConnectionV1& saved,data::PlayerSavegameV1& save,player::PlayerEquipmentRenderOwnerV1& gear,data::PropertyView& properties,CharacterMenuLiveReloadServicesV1 services):character_(character),skills_(skills),saved_(saved),save_(save),gear_(gear),properties_(properties),services_(std::move(services)){
 auto* gear_view=gear.property_view();
 if(!character_||save.character()!=character_||skills.native_savegame()!=&save||!gear_view||gear.properties()!=skills.session().properties()||gear_view->saved!=properties.saved||gear_view->resolved!=properties.resolved||&skills.session().property_view()!=&properties||!services_.owner)throw std::invalid_argument("Outer menu reload requires SAME live Save/V6/Gear/property owner");
}
int CharacterMenuLiveReloadConnectionV1::deliver(void* context,const ui::MenuReloadRequest32V1* request,ui::MenuReloadResponse16V1* response){
 using namespace ui;auto& self=*static_cast<CharacterMenuLiveReloadConnectionV1*>(context);
 if(!request||!response)return -1;*response={};
 auto missing=[&](const char* name){self.error_=std::string("Required original NativeReloadSkills ")+name+" provider unavailable";return -1;};
 if(request->service!=reload_spec_prompt_v1&&request->subject!=self.character_)return missing("same Character identity");
 switch(request->service){
 case reload_remove_buffs_v1:{
  auto* buffs=self.skills_.native_buffs();if(!buffs)return missing("CharProperties BuffOwner");
  BuffResult24 result{};if(dh2_character_buffs_remove_all(&result,buffs)!=1){self.error_="Original RemoveBuffs failed at phase "+std::to_string(result.phase);return -1;}return 0;
 }
 case reload_saved_skills_v1:return self.saved_.load_mask(request->argument,self.error_)?0:-1;
 case reload_skill_instances_v1:{
  auto source=self.saved_.services();if(self.skills_.native_reload_skills(&source)!=1){self.error_=self.skills_.error();if(!self.saved_.error().empty())self.error_+=' '+self.saved_.error();return -1;}return 0;
 }
 case reload_update_skills_v1:if(self.skills_.update()!=1){self.error_=self.skills_.error();return -1;}return 0;
 case reload_recalculate_v1:return self.services_.recalculate?(self.services_.recalculate(self.properties_,request->argument,self.error_)?0:-1):missing("CharProperties::Recalc");
 case reload_check_items_v1:return self.services_.check_items?(self.services_.check_items(self.gear_,self.error_)?0:-1):(self.gear_.check_item_requirements_v1(self.error_)?0:-1);
 case reload_saved_level_v1:response->value=self.save_.level();return 0;
 case reload_saved_class_v1:response->value=self.save_.class_id();return 0;
 case reload_menu_fx_v1:return self.services_.menu_fx?(self.services_.menu_fx(response->identity,self.error_)?0:-1):missing("MultiMenuManager RenderFX");
 case reload_spec_prompt_v1:return self.services_.invoke?(self.services_.invoke(request->subject,request->path,request->callback,request->argument!=0,self.error_)?0:-1):missing("RenderFX::Invoke IsSpecTime");
 default:return missing("operation");
 }
}
int CharacterMenuLiveReloadConnectionV1::reload(ui::MenuReloadResult16V1& out){error_.clear();auto callbacks=services();return dh2_character_menu_reload_v1(&out,character_,&callbacks);}
}
