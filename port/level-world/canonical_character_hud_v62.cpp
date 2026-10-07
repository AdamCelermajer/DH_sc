#include "canonical_character_hud_v62.hpp"
namespace dh2::world {
int canonical_character_hud_actor_v62(CanonicalCharacterCandidateRecordV60& r,const ui::HudManagerRequest& q,ui::HudManagerResponse& out,std::string& e){
 using O=ui::HudManagerOperation;
 if(q.subject!=reinterpret_cast<std::uintptr_t>(&r.hud_actor_v62))return 0;
 if(!r.refresh_hud_actor_v62(e))return -1;
 const auto& p=r.properties->resolved;
 auto integer=[](std::int32_t raw){return raw>=0?raw/256:std::int32_t(-((-(std::int64_t(raw))+255)/256));};
 switch(q.operation){
 case O::player_class:if(!r.init_fields.properties_id13c8){e="Required actual Character13c8 for HUD class";return -1;}out.value=*r.init_fields.properties_id13c8;return 1;
 case O::property_int:if(q.index>=p.size()){e="Invalid original HUD property index";return -1;}out.value=integer(p[q.index]);return 1;
 case O::is_character:out.value=1;return 1; //Actual Character virtual classification.
 case O::is_dead:out.value=r.life->dead!=0;return 1;
 case O::is_monster:case O::is_boss:{const auto* row=data::ai_props(*r.design.ai(),p[1]);if(!row){e="Required actual HUD AiProps";return -1;}out.value=q.operation==O::is_monster?row->type==4:(row->flags&4)!=0;return 1;}
 case O::level:out.value=integer(p[19]);return 1;
 case O::hp_fraction:out.fraction=float(p[36])/float(p[38]);return 1; //Original3bd2dc, including zero denominator.
 case O::potions:if(!r.inventory37c){e="Required same embedded inventory37c for HUD potions";return -1;}out.value=r.inventory37c->num_potions();return 1;
 case O::skill_slot:if(!r.save_fields){e="Required actual Character Save14e8 field";return -1;}if(!*r.save_fields->save_slot14e8()){out.value=-1;return 1;}if(!r.save||*r.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r.save.get())){e="Required same HUD Save14e8";return -1;}out.value=r.save->skill_in_slot(q.index);return 1;
 case O::skill_usable:{if(!r.player_script_owner_v62){e="Required same initialized player skill VM for HUD CheckUsable";return -1;}std::uint32_t value{};if(r.player_script_owner_v62->callback(q.index,character::skills::skill_check_usable_v3,&value)!=0){e=r.player_script_owner_v62->error();return -1;}out.value=value;return 1;}
 default:return 0;
 }
}
}
