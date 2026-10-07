#include "character_hit_player_tail_v7.hpp"
namespace dh2::character {
int hit_player_tail_v7(const HitPlayerTailBorrowV7* actor,const HitPlayerTailServicesV7* services) {
 if(!actor||!services||!actor->character||!actor->properties||
  !actor->source_low_health_1448||!actor->properties->resolved)return -1;
 // Source3a8d88..9c integer half truncates toward zero. Read current fields
 // after common HitFor writes and required Kill, never the pre-hit snapshot.
 auto* sheet=actor->properties->resolved;
 if(sheet[36]<=sheet[38]/2) {
  bool online{};
  if(!services->online||services->online(services->context,&online))return -2;
  if(!online) {
   int difficulty{};
   if(!services->difficulty||services->difficulty(services->context,actor->character,&difficulty))return -2;
   if(difficulty==0) {
    if(!actor->source_tutorial_2d)return -2;
    if(*actor->source_tutorial_2d) {
     int id{};
     if(!services->script_id||services->script_id(services->context,"cinematic_Tuto_potionUse",&id))return -2;
     if(id!=-1&&(!services->start_script||services->start_script(services->context,id,-1,false)))return -2;
     *actor->source_tutorial_2d=0;
     if(!services->start_update_job||services->start_update_job(services->context))return -2;
    }
   }
  }
  if(*actor->source_low_health_1448) {
   *actor->source_low_health_1448=0;
   int index{};
   const char* name=actor->character==actor->captured_main_player?"sfx_mc_low_hp":"sfx_friend_low_hp";
   if(!services->sound_index||services->sound_index(services->context,name,&index))return -2;
   if(!services->play_sound||services->play_sound(services->context,index,false,0,0,false))return -2;
  }
 } else {
  bool player{};
  if(!services->is_player||services->is_player(services->context,actor->character,&player))return -2;
  if(!player||*actor->source_low_health_1448)return 1;
  // Original ARM soft-float converts current/max separately, multiplies .75,
  // then calls __aeabi_fcmpge(30e4b4). Preserve signed values and >= boundary.
  volatile float current=float(sheet[36]),maximum=float(sheet[38]);
  volatile float threshold=maximum*0.75f;
  if(current>=threshold)*actor->source_low_health_1448=1;
 }
 return 1;
}
}
