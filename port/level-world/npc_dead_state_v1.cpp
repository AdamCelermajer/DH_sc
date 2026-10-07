#include "npc_dead_state_v1.hpp"
#include <cstring>
namespace dh2::character {namespace {
int plus(int a,int b){auto bits=std::uint32_t(a)+std::uint32_t(b);int out;std::memcpy(&out,&bits,4);return out;}
}
int NpcDeadStateV1::set(bool mode,std::uintptr_t payload,bool force){
 if(!b_.machine||!b_.properties||dh2_property_validate(b_.properties)||!b_.animations||!b_.constant||!b_.stance){error_="Required SAME NPC dead-state owners";return -1;}
 auto& fsm=b_.machine->native_fsm();auto& state=b_.machine->state();auto& pending=b_.machine->combat_fields().push_death;
 int index=b_.properties->resolved[2];if(index<0||std::size_t(index)>=b_.animations->characters.size())index=17;
 if(std::size_t(index)>=b_.animations->characters.size())return 0;
 // Parsed schema excludes the original leading template pointer. Source
 // +10,+14,+18,+1c become scalar fields3,4,5,6, in actual retained row.
 const auto& row=b_.animations->characters[index].fields;
 for(unsigned i:{3u,4u,5u,6u})if(row[i].size()!=1){error_="Required actual NPC death animation row";return -2;}
 auto modifier=[&](unsigned bit,int& out){int mask{};out=0;
  if(b_.constant(b_.context,"AnimStancedAnim","SL__LIST_IPHONE",&mask))return false;
  return !(std::uint32_t(mask)&bit)||!b_.stance(b_.context,fsm.character,&out);
 };
 int stance{};const int first=row[pending?3:6].front();
 if(!modifier(pending?0x20000:0x8000,stance)){error_="Required source dead primary stance";return -2;}
 state.animation_override=plus(first,stance);
 // Reread the SAME pending byte after the first synchronous getter.
 const int second=row[pending?5:4].front();
 if(!modifier(pending?0x40000:0x10000,stance)){error_="Required source dead secondary stance";return -2;}
 secondary_animation_=plus(second,stance);state.dead_alternate=mode;pending=0;
 auto& machine=b_.machine->owner().machine();
 if(fsm.current_present&&(state.current==0||state.current==17||state.current==16)){
  machine.current_index=-1;fsm.current_present=0;state.current=-1;
 }
 const int status=force?b_.machine->transition(12,0xc358,payload):b_.machine->event(0xc358,payload);
 if(status<0){error_=b_.machine->error();return status;}return 1;
}
}
