#include "source_campaign_faery_v109.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_retirement_v88.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "source_campaign_character_zoning_v108.hpp"
#include "source_campaign_character_frame_v111.hpp"
#include "player_save_difficulty_global_v29.hpp"
#include "model_renderer.hpp"
#include <application_player_manager_bootstrap_v59.hpp>
#include "character_update_pointers_v105.hpp"
#include <cstring>
#include <algorithm>
namespace model_renderer {
class SourceCampaignFaeryV109 final:public std::enable_shared_from_this<SourceCampaignFaeryV109> {
public:
 std::weak_ptr<SourceWorldBorrowV61> world;
 SourceFaeryNativeV109 native;
 bool current(std::shared_ptr<SourceWorldBorrowV61>& out,std::string& e){
  out=world.lock();SourceCampaignCandidateBorrowV55 actual;
  if(!out||source_campaign_retirement_requested_v88()||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=out->owner||actual.application!=out->application){if(e.empty())e="Retired/replaced actual faery campaign";return false;}return true;
 }
 bool character(std::uintptr_t id,SourceCampaignCharacterBorrowV62& out,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;return current(w,e)&&borrow_source_campaign_character_v62(w->owner,id,out,e);}
 bool difficulty(std::uintptr_t id,std::int32_t& out,std::string& e){SourceCampaignCharacterBorrowV62 b;if(!character(id,b,e)||!b.character->save_fields)return false;auto r=b.character;
  return dh2::player::character_game_difficulty_v29({r->actor,id,r->save_fields->save_slot14e8(),r->services.difficulty_global},out,e);
 }
 bool change(std::uintptr_t id,std::uint32_t chosen,std::string& e){
  SourceCampaignCharacterBorrowV62 b;if(!character(id,b,e))return false;auto r=b.character;std::int32_t tier{};
  if(!r->save||!r->player_script_owner_v62||r->player_script_owner_v62->native_savegame()!=r->save.get()||!difficulty(id,tier,e)){if(e.empty())e="Required SAME source ChangeFaery Save/skills";return false;}
  if(!dh2::ui::character_menu_store_current_faery_v1(*r->save,id,chosen,tier,e))return false;
  if(r->player_script_owner_v62->update()!=1){e=r->player_script_owner_v62->error();return false;}
  if(!r->faery_association_v68||r->faery_association_v68->character!=id){e="Required SAME source Character420 association";return false;}
  const auto faery=r->faery_association_v68->faery420;if(!faery){e.clear();return true;}
  const char* model{};
  if(!native.visual.model_name||!native.visual.model_name(faery,model,e)){if(e.empty())e="Required actual faery GetCharModelName";return false;}
  if(!native.visual.set_visual||!native.visual.set_visual(faery,model,nullptr,true,e)){if(e.empty())e="Required actual faery SetVisualObject(model,NULL,true)";return false;}
  if(!native.visual.add_animation_set||!native.visual.add_animation_set(faery,e)){if(e.empty())e="Required actual faery ANIM_AddSetToRenderObject";return false;}
  e.clear();return true; // Character.ChangeFaery only; outer menu owns Place tail.
 }
};
namespace {
bool provider(const std::shared_ptr<void>& expected,std::shared_ptr<SourceCampaignFaeryV109>& out,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!expected||!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=expected||
    !borrow_source_campaign_condition_world_v70(candidate,world,e)||!world->menu_faery_v109){if(e.empty())e="Required enrolled SAME source faery provider";return false;}
 out=world->menu_faery_v109;return true;
}
}
bool enroll_source_campaign_faery_v109(const SourceCampaignCandidateBorrowV55& candidate,SourceFaeryNativeV109 native,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!world||!world->canonical_world)return false;
 if(world->menu_faery_v109){e="Source faery provider already enrolled; preserve actual resource authority";return false;}
 for(const auto& owner:{native.placement.owner,native.visual.owner})if(owner&&!owner.owner_before(world->owner)&&!world->owner.owner_before(owner)){e="Faery native leaves cannot own containing World";return false;}
 auto out=std::make_shared<SourceCampaignFaeryV109>();out->world=world;out->native=std::move(native);world->menu_faery_v109=std::move(out);e.clear();return true;
}
bool enroll_native_source_campaign_faery_v109(const std::shared_ptr<void>& expected,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!expected||!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=expected||!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(world->menu_faery_v109){std::shared_ptr<SourceWorldBorrowV61> same;return world->menu_faery_v109->current(same,e);}
 struct NativeResourceLoan {};auto loan=std::make_shared<NativeResourceLoan>();const std::weak_ptr<void> weak=expected;
 SourceFaeryNativeV109 native;native.placement.owner=loan;native.visual.owner=loan;
 auto actor=[weak](auto id,SourceCampaignCharacterBorrowV62& out,auto& e){auto world=weak.lock();if(!world){e="Released native faery resource campaign";return false;}return borrow_source_campaign_character_v62(world,id,out,e);};
 native.placement.target_position=[actor](auto id,auto& out,auto& e){SourceCampaignCharacterBorrowV62 b;if(!actor(id,b,e))return false;
  dh2::world::GameObjectInitializationFieldsV62 fields;if(!b.character->actor->inherited_initialization_fields_v62(b.character,fields,e))return false;
  const auto* target=fields.pointer(0x180);const auto* enabled=fields.byte(0x80);
  if(!target||!enabled){e="Required SAME GetTargetPosition target180/enabled80 cells";return false;}
  const auto* point=fields.vector3(*target&&*enabled?0x184:0x160);if(!point){e="Required SAME source target/current position vector";return false;}std::copy_n(point,3,out.begin());return true;
 };
 native.visual.model_name=[actor](auto id,const char*& out,auto& e){SourceCampaignCharacterBorrowV62 b;if(!actor(id,b,e))return false;auto r=b.character;
  if(!r->properties||!r->services.models||!r->init_fields.properties_id13c8){e="Required actual source faery model dictionary/properties";return false;}
  auto services=r->services.model_name;services.receiver=r->actor;
  services.is_faery=[r](bool& value,auto& e){if(!r->design.ai()){e="Required real IsFaery AI table";return false;}const auto* ai=dh2::data::ai_props(*r->design.ai(),r->properties->resolved[1]);if(!ai){e="Required real IsFaery row";return false;}value=ai->type==3;return true;};
  services.is_player=[r](bool& value,auto& e){return r->is_player(value,e);};
  services.is_local_player=[r,id](bool& value,auto& e){if(!r->services.is_local_player){e="Required actual local-player model predicate";return false;}return r->services.is_local_player(id,value,e);};
  services.faery_master418=[actor,id](auto& master,auto& e){SourceCampaignCharacterBorrowV62 actual;if(!actor(id,actual,e))return false;return source_campaign_character_master_v111(actual.actual_world,id,master,e);};
  services.current_faery=[actor](auto master,auto& chosen,auto& e){SourceCampaignCharacterBorrowV62 actual;if(!actor(master,actual,e))return false;auto r=actual.character;
   if(!r->save||!r->save_fields){e="Required actual faery master Save";return false;}std::int32_t tier{};
   if(!dh2::player::character_game_difficulty_v29({r->actor,master,r->save_fields->save_slot14e8(),r->services.difficulty_global},tier,e))return false;chosen=r->save->current_faery(tier);return true;
  };
  services.faery_model=[actor](auto master,auto chosen,auto& model,auto& e){SourceCampaignCharacterBorrowV62 actual;if(!actor(master,actual,e))return false;auto r=actual.character;const auto& table=r->services.faeries_v70;
   if(!r->properties||!table||table.lists().empty()){e="Required SAME faery master property29/list tables";return false;}auto list=r->properties->resolved[29];if(list<0||std::size_t(list)>=table.lists().size())list=0;
   if(chosen<0||std::size_t(chosen)>=table.lists()[std::size_t(list)].size()){e="Selected faery outside actual source list";return false;}const auto row=table.lists()[std::size_t(list)][std::size_t(chosen)];
   if(row<0||std::size_t(row)>=table.faeries().size()){e="Actual faery model row missing";return false;}const auto bits=table.faeries()[std::size_t(row)].scalar.words[3];std::memcpy(&model,&bits,4);return true;
  };
  return dh2::character::character_model_name_v62(r->properties->resolved[3],*r->init_fields.properties_id13c8,*r->services.models,services,out,e);
 };
 native.visual.set_visual=[actor](auto id,const char* model,const char* xref,bool force,auto& e){SourceCampaignCharacterBorrowV62 actual;if(!actor(id,actual,e))return false;
  if(!actual.character->visual){e="Required SAME faery family visual owner";return false;}return actual.character->visual->source_set_visual_v109(model,xref,force,e);
 };
 native.visual.add_animation_set=[actor](auto id,auto& e){SourceCampaignCharacterBorrowV62 actual;if(!actor(id,actual,e))return false;
  if(!actual.character->visual){e="Required SAME faery animation/visual owner";return false;}return actual.character->visual->source_add_animation_set_v109(e);
 };
 return enroll_source_campaign_faery_v109(candidate,std::move(native),e);
}
bool source_campaign_change_faery_v109(const std::shared_ptr<void>& world,std::uintptr_t character,std::uint32_t chosen,std::string& e){std::shared_ptr<SourceCampaignFaeryV109> p;return provider(world,p,e)&&p->change(character,chosen,e);}
bool borrow_source_campaign_faery_placement_v109(const std::shared_ptr<void>& world,dh2::world::LevelFaeryPlacementServicesV8& out,std::string& e){
 std::shared_ptr<SourceCampaignFaeryV109> p;if(!provider(world,p,e))return false;std::shared_ptr<SourceWorldBorrowV61> w;if(!p->current(w,e))return false;
 out=p->native.placement;out.owner=p;out.objects=&w->canonical_world->manager;
 out.char_type=[p](auto id,auto& value,auto& e){SourceCampaignCharacterBorrowV62 b;if(!p->character(id,b,e))return false;auto r=b.character;
  if(!r->properties||!r->design.ai()){e="Required actual faery/follower AI row";return false;}const auto* row=dh2::data::ai_props(*r->design.ai(),r->properties->resolved[1]);if(!row){e="Missing source Character.GetCharType row";return false;}value=row->type;return true;
 };
 out.look_at_vec=[p](auto id,auto& value,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;return p->current(w,e)&&source_campaign_character_look_at_v68(w->owner,id,value,e);};
 out.set_position=[p](auto id,const auto& value,bool destination,auto& e){SourceCampaignCharacterBorrowV62 b;return p->character(id,b,e)&&b.character->set_position(value,destination,e);};
 out.force_position=[p](auto id,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;return p->current(w,e)&&source_campaign_character_force_position_v111(w->owner,id,e);};
 out.disable_zoning=[p](auto id,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;return p->current(w,e)&&source_campaign_character_zoning_v108(w->owner,id,false,e);};
 out.master50=[p](auto id,auto& value,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;return p->current(w,e)&&source_campaign_character_master_v111(w->owner,id,value,e);};
 out.set_master=[p](auto id,auto master,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;return p->current(w,e)&&source_campaign_character_set_master_v111(w->owner,id,master,e);};
 out.faery420_field=[p](auto id,auto*& field,auto& e){SourceCampaignCharacterBorrowV62 b;if(!p->character(id,b,e))return false;auto& association=b.character->faery_association_v68;
  if(!association||association->character!=id){e="Required SAME writable Character420 field";return false;}field=&association->faery420;return true;
 };
 out.selected_faery=[p](auto id,auto& value,auto& e){SourceCampaignCharacterBorrowV62 b;std::int32_t tier{};if(!p->character(id,b,e)||!b.character->save||!p->difficulty(id,tier,e))return false;value=std::uint32_t(b.character->save->current_faery(tier));return true;};
 out.source_player_count6c4=[p](auto& count,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!p->current(w,e)||!w->player_manager||!w->player_manager->count_field()){if(e.empty())e="Required SAME PM count6c4";return false;}count=*w->player_manager->count_field();return true;};
 out.player=[p](auto index,bool remote,auto& id,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;dh2::player::PlayerInfoFieldsV1* info{};if(!p->current(w,e)||!w->player_manager||!w->player_manager->manager()||!w->player_manager->manager()->get_player(index,remote,info,e))return false;id=info?info->character660:0;return true;};
 out.local_player=[p](auto index,bool flag,auto& id,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;dh2::player::PlayerInfoFieldsV1* info{};if(!p->current(w,e)||!w->player_manager||!w->player_manager->get_local_player(index,flag,info,e))return false;id=info?info->character660:0;return true;};
 out.change_faery=[p](auto id,auto chosen,auto& e){return p->change(id,chosen,e);};
 e.clear();return true;
}
bool borrow_source_campaign_menu_faery_v109(const std::shared_ptr<void>& world,std::uintptr_t id,dh2::character::CharacterMenuFaeryServicesV8& out,std::string& e){
 std::shared_ptr<SourceCampaignFaeryV109> p;if(!provider(world,p,e))return false;out=p->native.visual;out.owner=p;
 out.difficulty=[p,id](auto& value,auto& e){return p->difficulty(id,value,e);};
 out.faery_character=[p](auto id,auto& value,auto& e){SourceCampaignCharacterBorrowV62 b;if(!p->character(id,b,e))return false;auto& association=b.character->faery_association_v68;if(!association||association->character!=id){e="Required SAME source Character420";return false;}value=association->faery420;return true;};
 out.current_level=[](auto& id,auto& e){dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;id=current.identity();return true;};
 out.place_faery_followers=[p](auto level,auto selected,auto& e){dh2::loader::CanonicalCurrentLevelBorrowV1 current;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!p->current(w,e)||!borrow_current_native_level_v27(current,e)||!current||current.identity()!=level){if(e.empty())e="Required SAME actual current Level faery placement";return false;}
  dh2::world::LevelFaeryPlacementServicesV8 placement;if(!borrow_source_campaign_faery_placement_v109(w->owner,placement,e))return false;
  return dh2::world::level_place_faery_followers_v8(selected,placement,e);
 };
 e.clear();return true;
}
}
