#pragma once
#include "live_enemy_binding.hpp"
#include "../../../level-world/object_identity.hpp"
#include "../../../level-world/character_script_owner.hpp"
namespace dh::foundation::enemy_ai {
// Works with the existing CharacterScriptSession or V3 retained session.
// Load/init is owned by that session's source lifecycle: this adapter never
// calls OnInit independently or populates copied monster globals.
template<class Session>
bool loan_authored_monster_callback(Session& session,const LiveEnemyBorrow& b,
 SelectedMonsterCallback& result,std::string& error){
 error.clear();
 if(!validate_live_enemy_borrow(b,b.actor?b.actor->id:invalid_actor_id,error))return false;
 dh2::character::ScriptSessionView selected{};
 if(!session.owner().active(selected)||selected.kind!=dh2::character::script_external||
    !selected.vm||!selected.constructor_fields||selected.constructor_fields->character!=b.actor->id||
    session.script_name()!="monster"||session.property_view().resolved!=b.properties->resolved){
  error="Required actual initialized authored monster AISExternal and SAME property backing";return false;
 }
 result={b.actor->id,selected.identity,selected.kind,true,
  [&session,b,selected](std::uint32_t event,ActorId payload,std::string& e){
   dh2::character::ScriptSessionView fresh{};
   if(!session.owner().active(fresh)||fresh.identity!=selected.identity||fresh.vm!=selected.vm){e="Selected original monster Session changed during callback loan";return false;}
   std::uint32_t target_event{};
   switch(event){
    case 9:target_event=dh2::object_identity::enemy_spotted;break;
    case 10:target_event=dh2::object_identity::target_died;break;
    case 12:target_event=dh2::object_identity::target_out_of_sight;break;
    case 13:target_event=dh2::object_identity::target_in_sight;break;
    case 14:target_event=dh2::object_identity::target_out_of_range;break;
    case 15:target_event=dh2::object_identity::target_in_ranged_range;break;
    case 16:target_event=dh2::object_identity::target_in_close_range;break;
    case 17:target_event=dh2::object_identity::target_in_melee_range;break;
    default:e="No authored monster target callback for Character event";return false;
   }
   // Actual wrapper applies retained VCB mask/alias membership and source
   // userdata representation. Callback failure keeps its existing VM prefix.
   const int code=session.dispatch_target(target_event,payload);
   if(code){e=session.error();if(e.empty())e="Original monster AISExternal callback failed";return false;}
   if(b.target->target!=b.actor->target_id){e="Original monster callback target publication missing";return false;}
   return true;
  }};
 return true;
}
}
