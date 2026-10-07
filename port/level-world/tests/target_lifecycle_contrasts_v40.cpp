#define main historical_clear_aggro_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_world_clear_aggro_v40.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_target_events.hpp"
struct Lifecycle : Pair {
 bool interactive=true,dead=false,missing=false;unsigned machine=3;float point[3]{25,0,0};
 std::vector<unsigned> events;bool retarget_in_sight=false;
 static int event_service(void* raw,TargetEventState32* state,const TargetEventRequest64* q,TargetEventResponse24* out){auto& f=*static_cast<Lifecycle*>(raw);*out={};
  switch(q->service){
  case target_event_debug_load:case target_event_debug_construct:case target_event_debug_query:case target_event_debug_destroy:return 0; // explicit debug endpoint fixture
  case target_event_is_target_character:out->word=state->target->target!=0;return 0;
  case target_event_get_target_character:out->identity=state->target->target;return 0;
  case target_event_clear_aggro:{auto o=f.owner();return o.clear(q->subject,q->other);}
  case target_event_owner_ai_id:out->word=0;return 0;
  case target_event_owner_position:return 0;
  case target_event_play_sound:return 0; // explicit presentation endpoint fixture, no audio claim
  case target_event_active_dispatch:
   if(q->event==0xd&&f.retarget_in_sight)state->target->target=3;
   return 0; // selected AIS endpoint fixture; no full AI acceptance
  default:return -1;
  }
 }
 static int query(void* raw,TargetState48* state,const TargetUpdateRequest24* q,std::uint32_t* out){auto& f=*static_cast<Lifecycle*>(raw);*out=0;
  switch(q->service){
  case target_update_awaiting_spawn:*out=f.machine==17;return 0;
  case target_update_in_limbus:*out=f.machine==0;return 0;
  case target_update_interactive:if(f.missing)return -1;*out=f.interactive;return 0;
  case target_update_dead:*out=f.dead;return 0;
  case target_update_owner_ai_id:return 0;
  case target_update_sight:{const float zero[3]{};return dh2_character_target_sight(out,zero,f.point,100);}
  case target_update_can_range:return 0;
  case target_update_melee_range:*out=1;return 0;
  case target_update_raise_event:{f.events.push_back(q->event);TargetEventState32 e{state,77,1,{},0};const std::int32_t sounds[]{-1};TargetEventServices40 s{&f,event_service,sounds,1,0,88};return dh2_character_target_event(&e,q->event,&s);}
  default:return -1;
  }
 }
 int update(){TargetUpdateServices16 s{this,query};return dh2_character_target_update(&actors[0].target,&s);}
 void selected(){seed(true,true,true);actors[0].target.target=actors[0].target.last_target=2;actors[0].target.alive=actors[0].target.sight=1;actors[0].target.changed=1;}
};
int main(){unsigned checks=0;
 Lifecycle moving;moving.selected();moving.point[0]=200;assert(!moving.update());assert(moving.events==std::vector<unsigned>{0xc});assert(moving.trace==std::vector<unsigned>({1,2,3}));assert(moving.actors[0].target.target==2&&moving.actors[0].target.last_target==2&&!moving.actors[0].target.sight&&!moving.actors[0].target.changed&&moving.actors[1].target.target==0);++checks;
 // InSight is strictly 3D view-radius, not an invented wall raycast.
 Lifecycle vertical;vertical.selected();vertical.point[0]=0;vertical.point[2]=100;assert(!vertical.update()&&vertical.events==std::vector<unsigned>{0xc});++checks;
 Lifecycle hidden_dead;hidden_dead.selected();hidden_dead.dead=true;hidden_dead.interactive=false;assert(!hidden_dead.update()&&!hidden_dead.actors[0].target.target&&!hidden_dead.actors[0].target.last_target&&hidden_dead.events==std::vector<unsigned>{0xc}&&hidden_dead.trace.empty());assert(hidden_dead.actors[0].target.alive==1);++checks; // source early return retains old cached alive
 Lifecycle visible_dead;visible_dead.selected();visible_dead.dead=true;assert(!visible_dead.update()&&!visible_dead.actors[0].target.alive&&visible_dead.events.front()==0xa);++checks;
 Lifecycle swapped;swapped.selected();swapped.actors[0].target.sight=0;swapped.retarget_in_sight=true;assert(!swapped.update()&&swapped.actors[0].target.target==3&&swapped.actors[0].target.sight==1&&swapped.events.front()==0xd);++checks; // source captured sight commits after callback
 Lifecycle missing;missing.selected();missing.missing=true;auto before=missing.actors[0].target;assert(missing.update()==2&&!std::memcmp(&before,&missing.actors[0].target,sizeof(before))&&missing.trace.empty());++checks;
 for(unsigned state:{0u,17u}){Lifecycle inactive;inactive.selected();inactive.machine=state;assert(!inactive.update()&&inactive.events.empty()&&inactive.actors[0].target.target==2);++checks;}
 std::cout<<"target lifecycle V40 PASS "<<checks<<" contrasts: moving/3D-radius/dead-prefix/reentrant-retarget/missing/spawn-limbus; selectedAIS/audio/debug endpoints are fixtures, no whole live HUD claim\n";
}
