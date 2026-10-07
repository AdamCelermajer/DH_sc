#pragma once
#include "character_controller_commands.hpp"
#include "character_target_bindings.hpp"
#include <string>
namespace dh2::character {
enum class PlayerTargetDiedPhaseV2 { controller, set_target, sync_target };
struct PlayerTargetDiedBorrowV2 {
 ControllerCommandState32 controller{};
 const CharacterControlServices16* control{};
 TargetState48* target{};
 const TargetServices16* target_services{};
};
struct PlayerTargetDiedServicesV2 {
 void* context{};
 // Read the SAME selected AISPlayer receiver's live Character+98 each time.
 // Source re-loads after Cmd_Stop and after AI_SetTarget; do not cache owner.
 int(*borrow)(void*,PlayerTargetDiedPhaseV2,PlayerTargetDiedBorrowV2*){};
};
// Whole AISPlayer.OnTargetDied3dd84c (inherited by AISPlayerIPhone):
// Cmd_Stop -> AI_SetTarget(NULL,false) -> AI_SyncLastTarget. Source void command
// blocked gate still proceeds to target clearing. Failure preserves prefix.
inline int player_target_died_v2(const PlayerTargetDiedServicesV2& services,std::string& error){
 error.clear();PlayerTargetDiedBorrowV2 b{};
 if(!services.borrow||services.borrow(services.context,PlayerTargetDiedPhaseV2::controller,&b)||!b.control){error="Required selected AISPlayer controller owner";return -1;}
 if(dh2_character_controller_character(&b.controller,controller_stop,0,b.control)!=1){error="Required AISPlayer Cmd_Stop body";return -1;}
 b={};if(services.borrow(services.context,PlayerTargetDiedPhaseV2::set_target,&b)||!b.target||!b.target_services){error="Required reloaded AISPlayer target owner";return -1;}
 if(dh2_character_ai_set_target(b.target,0,0,b.target_services)){error="Required AISPlayer AI_SetTarget source service";return -1;}
 b={};if(services.borrow(services.context,PlayerTargetDiedPhaseV2::sync_target,&b)||!b.target){error="Required reloaded AISPlayer last-target owner";return -1;}
 if(dh2_character_ai_sync_last_target(b.target)){error="Invalid AISPlayer last-target owner";return -1;}return 0;
}
}
