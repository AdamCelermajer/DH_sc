#pragma once
#include "canonical_character_candidate_v60.hpp"
#include <functional>
namespace dh2::world {
// Whole Character.Clean3a7cf0 over the retained native fields. The containing
// domain supplies its actual draw and FX-anchor retirement, never a World cast.
struct CharacterCleanServicesV123 {
 std::function<bool(std::string&)> retire_draws,detach_fx_anchors;
 std::function<bool(std::string&)> borrow_fx_owner;
};
inline bool character_clean_v123(CanonicalCharacterCandidateRecordV60& r,
 const CharacterCleanServicesV123& services,std::string& e){
 if(!r.actor){e="Required SAME Character receiver for Clean";return false;}
 auto& a=*r.actor;
 // A completed FakeRemove Clean may precede D1's second genuine delivery.
 // Failed prefixes cannot be replayed on the same native storage.
 if(r.clean_attempted_v107&&!r.clean_completed_v107){e="Character.Clean failed native prefix cannot replay";return false;}
 r.clean_attempted_v107=true;r.clean_completed_v107=false;
 if(!services.retire_draws||!services.retire_draws(e)){if(e.empty())e="Required actual Character draw retirement at Clean";return false;}
 auto drop=[&](std::uintptr_t& field){
  if(!field)return true; // Whole DropAnimatedFX's genuine NULL argument return.
  if(!r.target_fx_pin_v70||!r.target_fx_manager_v70){
   if(!services.borrow_fx_owner||!services.borrow_fx_owner(e)||!r.target_fx_pin_v70||!r.target_fx_manager_v70){if(e.empty())e="Required SAME Character FX instance owner at Clean";return false;}
  }
  return r.target_fx_manager_v70->drop(field,e);
 };
 if(!drop(a.source_self_fx1484())||!drop(a.source_state_fx148c()))return false;
 if(r.target_marker_v70){if(!r.target_marker_v70->release(e))return false;r.target_marker_v70.reset();}
 auto cross=a.source_target_cross149c_v70();if(!cross){e="Required source Character target-cross149c";return false;}
 if(!drop(*cross)||!drop(a.source_highlight14a0()))return false;
 if(!services.detach_fx_anchors||!services.detach_fx_anchors(e)){if(e.empty())e="Required actual Character FX anchor retirement at Clean";return false;}
 auto frame=a.source_frame_fields_v106();if(!frame){e="Required produced source Character.TimerUtil14e0";return false;}
 if(frame->timer14e0){
  if(!r.timer_util_v107||frame->timer14e0!=reinterpret_cast<std::uintptr_t>(r.timer_util_v107.get())){e="Required actual TimerUtil allocation owner before source CustomFree";return false;}
  r.timer_util_v107.reset(); // Original Clean does not store NULL into14e0.
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
