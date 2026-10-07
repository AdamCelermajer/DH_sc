#include "canonical_character_spawn_select_v87.hpp"
#include "canonical_character_candidate_v60.hpp"
namespace dh2::character {
bool CanonicalCharacterSpawnSelectV87::select(std::uint32_t delayed,std::uint32_t ignored,std::string& e){
 auto r=record_.lock();if(!r||!r->actor||!r->actor->machine||!r->services.random||!r->services.random_lease){e="Required SAME canonical spawn FSM/App Random lease";return false;}
 auto* bounds=r->actor->source_spawn_delay_v87();if(delayed&&!bounds){e="Required actual Character1434/1438 property producer";return false;}
 const auto* in_animation=r->animation_random_inflight_v101;
 data::CombatRandom random{in_animation?in_animation->seed:r->services.random->seed,in_animation?in_animation->calls:r->services.random->calls};
 NativeSpawn24 selection{&r->actor->machine->native_fsm(),&random,bounds?bounds[0]:0,bounds?bounds[1]:0};
 struct Delivery {
  world::CanonicalCharacterCandidateRecordV60& record;std::int32_t* bounds;bool delayed;std::string& error;
  static int invoke(void* raw,NativeSpawn24* selection,const SpawnSelectRequest32* q,std::uint32_t* result){
   auto& d=*static_cast<Delivery*>(raw);auto& r=d.record;
   if(!selection||!q||q->character!=r.actor->object->identity){d.error="Wrong actual spawn selector receiver";return 1;}
   //The pure selector has completed source clamps/RNG. Publish BEFORE any
   //FSM focus or timer callback can reenter and observe the original globals.
   if(d.delayed){if(!d.bounds)return 1;d.bounds[0]=selection->minimum_ms;d.bounds[1]=selection->maximum_ms;}
   r.services.random->seed=selection->random->seed;r.services.random->calls=selection->random->calls;
   if(r.animation_random_inflight_v101){r.animation_random_inflight_v101->seed=selection->random->seed;r.animation_random_inflight_v101->calls=selection->random->calls;}
   if(q->service==spawn_select_state){if(r.actor->machine->transition(1,-1,0)<0){d.error=r.actor->machine->error();return 1;}return 0;}
   if(q->service!=spawn_select_timer){d.error="Unknown source spawn selection operation";return 1;}
   TimerStore32* timers{};const TimerServices32* services{};
   if(r.player_script_owner_v62){auto& s=r.player_script_owner_v62->session();timers=&s.timers();services=&s.timer_services();}
   else if(r.actor->session){timers=&r.actor->session->timers();services=&r.actor->session->native_timer_services();}
   if(!timers||!services||timers->owner!=q->character){d.error="Required SAME source spawn TimerStore/services";return 1;}
   auto id=dh2_character_timer_start(timers,q->argument0,0,0x2d,0,services);
   if(id< -1){d.error="Source spawn timer delivery failed";return 1;}
   *result=static_cast<std::uint32_t>(id);return 0; //original ignores returned−1 too
  }
 }delivery{*r,bounds,delayed!=0,e};
 const SpawnSelectServices16 services{&delivery,Delivery::invoke};
 if(dh2_character_spawn_select(&selection,delayed,ignored,&services)!=1){if(e.empty())e="Required actual SM_SetSpawnState source continuation";return false;}
 e.clear();return true;
}
bool source_character_select_spawn_v87(world::CanonicalCharacterCandidateRecordV60& r,std::uint32_t delay,std::uint32_t mode,std::string& e){
 if(!r.spawn_select_v87)r.spawn_select_v87=std::make_unique<CanonicalCharacterSpawnSelectV87>(r.shared_from_this());
 return r.spawn_select_v87->select(delay,mode,e);
}
}
