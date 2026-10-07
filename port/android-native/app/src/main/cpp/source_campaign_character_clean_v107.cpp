#include "source_campaign_character_clean_v107.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_fx_v77.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
namespace model_renderer {
bool source_campaign_character_clean_v107(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character->actor)return false;
 auto& r=*actual.character;auto& a=*r.actor;
 //FakeRemove349240 delivers Clean; the eventual CharacterD1 delivers it
 //again at3a7cf0. Only a failed prefix is nonretryable. A completed first
 //delivery may enter again with the source-cleared fields, preserving the
 //literal second nullable Drop calls and animator-user predicate.
 if(r.clean_attempted_v107&&!r.clean_completed_v107){e="Character.Clean failed native prefix cannot replay";return false;}
 r.clean_attempted_v107=true;r.clean_completed_v107=false;
 //Compiled nodes own real transferred animation/mesh references. Retire
 //their native draw/backlinks at source Clean before the same owners die;
 //FakeRemove may still publish this record and authentic D1 repeats Clean.
 if(!retire_source_campaign_batch_object_v113(world,id,e))return false;
 auto drop=[&](std::uintptr_t& field){
  //Actual DropAnimatedFX receives even a NULL field, whose whole body returns.
  if(!field)return true;if(!r.target_fx_pin_v70||!r.target_fx_manager_v70){
   if(!borrow_source_campaign_fx_v77(world,r.target_fx_pin_v70,r.target_fx_manager_v70,e)||!r.target_fx_manager_v70)return false;
  }
  return r.target_fx_manager_v70->drop(field,e);
 };
 if(!drop(a.source_self_fx1484())||!drop(a.source_state_fx148c()))return false;
 if(r.target_marker_v70){if(!r.target_marker_v70->release(e))return false;r.target_marker_v70.reset();}
 auto cross=a.source_target_cross149c_v70();if(!cross){e="Required source Character target-cross149c";return false;}
 if(!drop(*cross)||!drop(a.source_highlight14a0()))return false;
 //Skills/projectiles can retain effects past the victim's lifetime. Detach
 //their anchors while the actual Character is still published, before its
 //visual/save/animator receivers are released.
 if(!retire_source_campaign_fx_anchor_v117(world,id,e))return false;
 auto frame=a.source_frame_fields_v106();if(!frame){e="Required produced source Character.TimerUtil14e0";return false;}
 if(frame->timer14e0){
  if(!r.timer_util_v107||frame->timer14e0!=reinterpret_cast<std::uintptr_t>(r.timer_util_v107.get())){e="Required actual TimerUtil allocation owner before source CustomFree";return false;}
  r.timer_util_v107.reset(); //original Clean does not store NULL into14e0.
 }
 if(r.visual&&r.visual->animator()&&r.visual->animator()->source_has_active_users_v107())
  if(!r.visual->animator()->source_dec_anim_set_users_v107(e))return false;
 if(!r.save_fields){e="Required SAME Character Save14e8 field";return false;}
 const auto save=*r.save_fields->save_slot14e8();
 if(save){if(!r.services.destroy_character_save_v107){e="Required whole positive PlayerSavegame14e8 D0";return false;}
  if(!r.services.destroy_character_save_v107(r,save,e))return false;
  if(*r.save_fields->save_slot14e8()){e="Source Save D0 completed without same Character14e8 NULL store";return false;}}
 auto aux=a.source_auxiliary14ec_v107();if(!aux){e="Required produced source Character auxiliary14ec";return false;}
 if(*aux){const auto captured=*aux;if(!r.services.destroy_character_aux14ec_v107){e="Required whole Character14ec virtual1c release";return false;}
  if(!r.services.destroy_character_aux14ec_v107(r,captured,e))return false;*aux=0;}
 r.clean_completed_v107=true;e.clear();return true;
}
}
