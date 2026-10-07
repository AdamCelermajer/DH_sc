#include "player_injure_state_v7.hpp"
#include <cstring>
namespace dh2::character {
namespace {
bool valid(const PlayerInjureBorrowV7* b,const PlayerInjureServicesV7* s){
 return b&&s&&b->character&&b->state&&b->source_gate_14fc;
}
int add_bits(int a,int b){unsigned bits=unsigned(a)+unsigned(b);int value;std::memcpy(&value,&bits,4);return value;}
}
int player_set_injure_v7(const PlayerInjureBorrowV7* b,std::uintptr_t attacker,
 bool direct,const PlayerInjureServicesV7* s) {
 if(!valid(b,s))return -1;
 if(*b->source_gate_14fc>0.f)return 1;
 bool player{};
 if(!s->is_player||s->is_player(s->context,b->character,&player))return -2;
 // Source queries IsPlayer but does not branch on its returned word.
 (void)player;
 *b->source_gate_14fc=3000.f;
 int table{};
 if(!s->animation_table||s->animation_table(s->context,b->character,&table))return -2;
 if(table<0)return 1;
 bool found{};int animation{};
 if(!s->injure_animation||s->injure_animation(s->context,table,&found,&animation))return -2;
 if(!found)return 1;
 int stanced{};
 if(!s->constant||s->constant(s->context,"AnimStancedAnim","SL__LIST_IPHONE",&stanced))return -2;
 int offset=0;
 if(stanced&0x1000){if(!s->stance||s->stance(s->context,b->character,&offset))return -2;}
 b->state->animation_override=add_bits(animation,offset);
 if(direct){if(!s->transition||s->transition(s->context,11,50010,attacker))return -2;}
 else if(!s->event||s->event(s->context,50010,attacker))return -2;
 return 1;
}
int player_injure_focus_v7(const PlayerInjureBorrowV7* b,const PlayerInjureServicesV7* s) {
 if(!valid(b,s))return -1;
 if(!s->debug||s->debug(s->context,"isTracingCharState")||s->debug(s->context,"isTracingCSInjured"))return -2;
 b->state->flags=0x2b41;
 if(!s->set_animation||s->set_animation(s->context,-1))return -2;
 if(!s->cancel_sneaking||s->cancel_sneaking(s->context,b->character))return -2;
 return 1;
}
int player_injure_blur_v7(const PlayerInjureBorrowV7* b,const PlayerInjureServicesV7* s) {
 if(!valid(b,s))return -1;
 if(!s->debug||s->debug(s->context,"isTracingCharState"))return -2;
 if(!s->look_at_source_408||s->look_at_source_408(s->context,b->character))return -2;
 return 1;
}
int player_injure_empty_v7(std::uint32_t function){return function==0x3c0044||function==0x3c0040?1:-2;}
int player_injure_gate_tick_v7(float* gate,std::uint32_t dt) {
 if(!gate)return -1;
 if(*gate>0.f){volatile float converted=float(dt);*gate=*gate-converted;}
 return 1;
}
}
