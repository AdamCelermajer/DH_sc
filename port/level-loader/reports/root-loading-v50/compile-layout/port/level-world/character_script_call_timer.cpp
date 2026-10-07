#include "character_script_call_timer.hpp"
#include <cstring>
extern "C" int dh2_character_script_call_timer(
 const dh2::character::ScriptTimerCall16* call,std::uint32_t timer_id){
 if(!call||!call->vm||!call->aliases)return -1;
 std::int32_t signed_id;
 std::memcpy(&signed_id,&timer_id,sizeof(signed_id));
 dh2_script_value argument{};
 argument.type=DH2_SCRIPT_NUMBER;argument.number=static_cast<float>(signed_id);
 return dh2_script_alias_call_discard_source(call->vm,call->aliases,"OnTimer",&argument,1);
}
