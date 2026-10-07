#pragma once
#include "character_ai_pointer_fields_v105.hpp"
#include <functional>
#include <string>
namespace dh2::character {
enum class MasterQueryV108 { OwnerAiId, Dead, Sight, MyTurn, CanRange, CloseRange, RangedRange, MeleeRange };
struct MasterServicesV108 {
 std::function<bool(MasterQueryV108,std::uintptr_t,std::uint32_t&,std::string&)> query;
 std::function<bool(std::uint32_t,std::uintptr_t,std::string&)> raise;
};
// Whole CharAI::_UpdateMaster3cc5a4. The pointer is reloaded after every
// synchronous event; captured Boolean results are stored after that event.
// This header-only body is shared with the native frame receiver, not a second
// master or target state owner.
inline bool character_update_master_v108(CharacterAiPointerFieldsV105& f,
 const MasterServicesV108& s,std::string& e){
 if(!f.master50)return true;
 if(!s.query||!s.raise){e="Required original master queries/event receiver";return false;}
 std::uint32_t value{};
 if(!s.query(MasterQueryV108::OwnerAiId,0,value,e))return false;
 if(!f.master50){e="Required master receiver after GetCharAIId";return false;}
 if(!s.query(MasterQueryV108::Dead,f.master50,value,e))return false;
 const auto alive=static_cast<std::uint8_t>(value^1u);
 if(f.master_alive54){if(!alive&&!s.raise(18,f.master50,e))return false;}
 else if(alive&&!s.raise(19,f.master50,e))return false;
 f.master_alive54=alive;
 if(!f.master50)return true;
 if(!s.query(MasterQueryV108::Sight,f.master50,value,e))return false;
 const auto sight=value;
 if(f.master_sight55){if(!sight&&!s.raise(20,f.master50,e))return false;}
 else if(sight&&!s.raise(21,f.master50,e))return false;
 f.master_sight55=static_cast<std::uint8_t>(sight);
 if(!f.master50||!f.master_alive54||!sight)return true;
 if(!s.query(MasterQueryV108::MyTurn,0,value,e))return false;
 if(!value)return true;
 if(!s.query(MasterQueryV108::CanRange,0,value,e))return false;
 std::uint32_t event;
 if(value){
  if(!s.query(MasterQueryV108::CloseRange,f.master50,value,e))return false;
  if(value)event=24;
  else{if(!s.query(MasterQueryV108::RangedRange,f.master50,value,e))return false;event=value?23:22;}
 }else{if(!s.query(MasterQueryV108::MeleeRange,f.master50,value,e))return false;event=value?25:22;}
 return s.raise(event,f.master50,e);
}
}
