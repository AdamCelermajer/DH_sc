#include "../original_actor_lifecycle.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool x,const std::string&e){if(!x)throw std::runtime_error(e);}
int main(){try{
 OriginalActorLifecycle lifecycle;ActorState actor;actor.id=41;OriginalLifecycleFacts facts;facts.preset_ai_state="Limbus";facts.initial_transform.position={10,20,250};facts.initial_transform.rotation={0,0,1};facts.initially_enabled=true;facts.pre_spawn_has_animation=false;facts.pre_spawn_stay_enabled=false;
 std::vector<OriginalLifecycleRequest> calls;OriginalLifecycleServices services;
 services.floor_height=[](std::array<float,3>p,bool&found,float&h,std::string&){check(p[2]==250,"Wrong authored anchor");found=true;h=258;return true;};
 services.invoke=[&](const OriginalLifecycleRequest&q,std::string&){check(q.actor==&actor,"Lifecycle replaced SAME actor");calls.push_back(q);if(q.operation==OriginalLifecycleOperation::restore_initial_position)actor.transform.position=q.initial_transform.position;if(q.operation==OriginalLifecycleOperation::restore_initial_rotation)actor.transform.rotation=q.initial_transform.rotation;return true;};lifecycle.bind(services);std::string e;
 check(lifecycle.add(actor,facts,e),e);check(lifecycle.status(actor.id)->state==17&&!lifecycle.status(actor.id)->enabled&&!lifecycle.combat_enabled(actor.id),"Preset Limbus must initialize actual PreSpawn17");
 check(actor.transform.position[2]==258,"Source initial floor snap missing");
 bool frozen=false,removed=false;for(const auto&q:calls){frozen|=q.operation==OriginalLifecycleOperation::freeze_animation_speed;removed|=q.operation==OriginalLifecycleOperation::remove_physical;}check(frozen&&removed,"PreSpawn fallback/physical setup missing");calls.clear();
 check(lifecycle.spawn(actor.id,e),e);check(lifecycle.status(actor.id)->state==1&&!lifecycle.combat_enabled(actor.id),"Spawn prematurely enabled combat");
 check(calls.size()>=8&&calls[0].operation==OriginalLifecycleOperation::set_enabled&&calls[1].operation==OriginalLifecycleOperation::revive&&calls[2].operation==OriginalLifecycleOperation::set_collisions_enabled&&calls[2].collisions_enabled,"Source PreSpawn OnBlur order");
 check(calls[3].state==1&&calls[3].flags==0x241,"Source Spawn focus flags");calls.clear();
 check(lifecycle.animation_event(actor.id,"is_interactive",e),e);check(lifecycle.status(actor.id)->flags&0x2000,"Authored interactive event lost");calls.clear();
 check(lifecycle.animation_finished(actor.id,e),e);check(lifecycle.status(actor.id)->state==3&&lifecycle.combat_enabled(actor.id),"Source finished event34 did not enter Idle");
 for(auto&q:calls)check(q.operation!=OriginalLifecycleOperation::init_physical,"Already-interactive Spawn reinitialized physics");check(calls.back().operation==OriginalLifecycleOperation::notify_state_changed&&calls.back().previous_state==1,"Source state event29 previous state missing");
 calls.clear();check(lifecycle.put_idle(actor.id,e),e);check(!calls.empty(),"Original same-state blur/focus incorrectly suppressed");
 check(lifecycle.put_limbus(actor.id,e),e);actor.transform.position={100,200,300};calls.clear();check(lifecycle.spawn(actor.id,e),e);check(actor.transform.position==facts.initial_transform.position||actor.transform.position==std::array<float,3>{10,20,258},"Limbus source anchor not restored");
 check(calls.size()>4&&calls[1].operation==OriginalLifecycleOperation::restore_initial_position&&calls[2].operation==OriginalLifecycleOperation::restore_initial_rotation&&calls[3].operation==OriginalLifecycleOperation::revive,"Limbus Blur restoration order");
 // B080/B094: an early skipped cutscene leaves Spawn before its interactive marker.
 // The explicit Idle owner must perform Spawn.Blur and admit the body exactly once.
 const auto anchor=actor.transform.position;calls.clear();check(lifecycle.put_idle(actor.id,e),e);
 unsigned physicalAdmissions=0;for(const auto&q:calls)physicalAdmissions+=q.operation==OriginalLifecycleOperation::init_physical;
 check(physicalAdmissions==1&&calls.front().operation==OriginalLifecycleOperation::init_physical,
       "Skipped Spawn must admit physics through Spawn.Blur before Idle.Focus");
 check(lifecycle.combat_enabled(actor.id)&&lifecycle.status(actor.id)->state==3&&actor.transform.position==anchor,
       "Skipped Spawn must become targetable/combat-ready at its existing authored anchor");
 calls.clear();check(lifecycle.put_idle(actor.id,e),e);for(const auto&q:calls)check(q.operation!=OriginalLifecycleOperation::init_physical,
       "Duplicate skipped Idle must not create a second physical body");
 // B141: a Continue/F9/death restore replaces every actor. Records drop, the preset PreSpawn baseline survives and registers the restored
 // actor at PreSpawn17 without replaying effects, so a later Script_SpawnCharacter reaches the Spawn transition instead of
 // "Unknown original lifecycle actor". Only preset PreSpawn/Limbus actors keep a baseline; clear() stays the full reset.
 {OriginalActorLifecycle restored;ActorState first,plain,newer,third;first.id=77;plain.id=78;newer.id=77;third.id=77;
  ActorState* expected=&first;std::vector<OriginalLifecycleRequest> log;OriginalLifecycleServices s2;
  s2.floor_height=[](std::array<float,3>,bool&found,float&,std::string&){found=false;return true;};
  s2.invoke=[&](const OriginalLifecycleRequest&q,std::string&){check(q.actor==expected,"Lifecycle must act on the restored actor");log.push_back(q);return true;};
  restored.bind(s2);
  OriginalLifecycleFacts idleFacts;idleFacts.initial_transform=facts.initial_transform;idleFacts.initially_enabled=true;
  check(restored.add(first,facts,e),e);expected=&plain;check(restored.add(plain,idleFacts,e),e);
  restored.detach_for_restore();
  check(!restored.status(77)&&!restored.status(78),"Restore detach must drop the borrowed records");
  check(restored.pre_spawn_baseline_ids()==std::vector<ActorId>{77},"Only the preset PreSpawn actor keeps a baseline across a restore");
  expected=&newer;log.clear();
  check(!restored.restore_pre_spawn(78,plain,true,e),"An Idle-preset actor has no PreSpawn baseline");
  check(restored.restore_pre_spawn(77,newer,false,e),e);
  check(log.empty(),"Restore registration must not replay lifecycle effects");
  check(restored.status(77)->state==17&&restored.status(77)->flags==0x1300&&!restored.status(77)->enabled&&!restored.combat_enabled(77),"Restored actor must be PreSpawn17");
  check(!restored.restore_pre_spawn(77,newer,false,e),"Duplicate restore registration must be refused");
  // B155: the Idle-preset campaign actor (auto_spawn=0, e.g. the Swamp King's intro lizards) keeps its own baseline. After the
  // restore it registers at Idle3 (or Limbus0 when it was saved in Limbus) without effects, so the intro's PutCharacterInLimbus works.
  {check(restored.idle_baseline_ids()==std::vector<ActorId>{78},"The Idle-preset actor keeps a non-preset baseline across a restore");
   ActorState plainAgain;plainAgain.id=78;expected=&plainAgain;log.clear();
   check(!restored.restore_idle_or_limbus(77,newer,true,false,e),"A PreSpawn-preset actor has no Idle baseline");
   check(restored.restore_idle_or_limbus(78,plainAgain,true,false,e),e);
   check(log.empty(),"Idle restore registration must not replay lifecycle effects");
   check(restored.status(78)->state==3&&restored.status(78)->flags==0x2380&&restored.status(78)->enabled&&restored.combat_enabled(78),"Restored Idle actor is Idle3");
   check(!restored.restore_idle_or_limbus(78,plainAgain,true,false,e),"Duplicate Idle restore registration must be refused");
   check(restored.put_limbus(78,e)&&restored.status(78)->state==0&&!log.empty(),"Script_PutCharacterInLimbus reaches the restored Idle actor: "+e);
   restored.remove(78);log.clear();
   check(restored.restore_idle_or_limbus(78,plainAgain,true,true,e)&&restored.status(78)->state==0&&!restored.status(78)->enabled&&log.empty(),"A saved Limbus actor restores at Limbus0");
   check(restored.put_idle(78,e)&&restored.status(78)->state==3,"A restored Limbus actor can be put back in Idle: "+e);
   restored.remove(78);expected=&newer;}
  // B155 review: only a live, Idle3, physically present actor (or any Limbus record) is re-registered after a restore.
  {using L=OriginalActorLifecycle;const std::optional<bool> present=true,absent=false,unknown;
   check(L::idle_preset_restorable(false,true,present,3,true,true),"A live Idle3 actor with its body is re-registered");
   check(L::idle_preset_restorable(false,true,unknown,3,true,true)&&!L::idle_preset_restorable(false,true,unknown,3,false,true),"Unknown physical presence follows the placed enable flag");
   check(!L::idle_preset_restorable(false,true,absent,3,true,true),"A bodiless targetable actor keeps no record (it would become untargetable)");
   check(L::idle_preset_restorable(false,true,absent,3,true,false),"A bodiless presentation prop (intro lizard) is re-registered so a script can put it in Limbus");
   check(!L::idle_preset_restorable(false,false,present,3,true,true),"A dead actor keeps no record");
   check(!L::idle_preset_restorable(false,true,present,1,true,true)&&!L::idle_preset_restorable(false,true,present,17,true,true)&&!L::idle_preset_restorable(false,true,present,12,true,true),
         "Spawn/PreSpawn/Dead saved states are not forced to Idle3");
   check(L::idle_preset_restorable(true,false,absent,12,false,false),"A Limbus record always qualifies");}
  // B145: anchor check against the preserved authored baseline (first axis-by-axis tolerance, no baseline = false).
  {ActorState probe=newer;probe.transform.position=facts.initial_transform.position;
   check(restored.at_pre_spawn_anchor(probe),"Actor on its authored anchor");
   probe.transform.position[1]+=1.0f;check(restored.at_pre_spawn_anchor(probe),"Within tolerance");
   probe.transform.position[1]+=5.0f;check(!restored.at_pre_spawn_anchor(probe),"Moved actor is off its anchor");
   probe.transform.position[1]=std::nanf("");check(!restored.at_pre_spawn_anchor(probe),"Non-finite position is off its anchor");
   ActorState idle;idle.id=78;idle.transform.position=facts.initial_transform.position;check(!restored.at_pre_spawn_anchor(idle),"No PreSpawn baseline, no anchor");}
  check(restored.spawn(77,e),e);
  check(restored.status(77)->state==1&&!log.empty(),"Script_SpawnCharacter must reach the Spawn transition on a restored actor");
  restored.detach_for_restore();expected=&third;
  check(restored.restore_pre_spawn(77,third,true,e),e);check(restored.status(77)->enabled,"Second restore keeps the baseline and takes the current enable fact");
  restored.clear();check(restored.pre_spawn_baseline_ids().empty()&&restored.idle_baseline_ids().empty()&&!restored.status(77),"clear() is the full reset");
 }
 // B142: a death-checkpoint reload restores saved state 17 over a Character that has since spawned (Character::Deserialize ->
 // SM_SetPreSpawnState). Only the CSPreSpawn Focus effects run (no old-state Blur: the restored data replaced what it undoes), the record
 // lands at PreSpawn17 with the previous state in the notification, and a later Script_SpawnCharacter works again (repeat death).
 {OriginalActorLifecycle death;ActorState first,plain,spawned;first.id=91;plain.id=92;spawned.id=91;
  ActorState* expected=&first;std::vector<OriginalLifecycleRequest> log;bool failRemove=false;OriginalLifecycleServices s3;
  s3.floor_height=[](std::array<float,3>,bool&found,float&,std::string&){found=false;return true;};
  s3.invoke=[&](const OriginalLifecycleRequest&q,std::string&err){check(q.actor==expected,"Lifecycle must act on the restored actor");
      if(failRemove&&q.operation==OriginalLifecycleOperation::remove_physical){err="body owner refused";return false;}log.push_back(q);return true;};
  death.bind(s3);
  OriginalLifecycleFacts idleFacts;idleFacts.initial_transform=facts.initial_transform;idleFacts.initially_enabled=true;
  check(death.add(first,facts,e),e);expected=&plain;check(death.add(plain,idleFacts,e),e);expected=&first;
  check(death.spawn(91,e)&&death.put_idle(91,e)&&death.status(91)->state==3,"Chief-like actor reaches Idle3 before the death: "+e);
  death.detach_for_restore();expected=&spawned;log.clear();
  check(!death.enter_restored_pre_spawn(92,plain,true,3,e),"An Idle-preset actor has no PreSpawn baseline");
  check(log.empty()&&!death.status(92),"A refused actor must not run effects or register");
  failRemove=true;check(!death.enter_restored_pre_spawn(91,spawned,true,3,e)&&!death.status(91),"A failed Focus effect must not leave a PreSpawn record");
  failRemove=false;log.clear();
  check(death.enter_restored_pre_spawn(91,spawned,true,3,e),e);
  check(!log.empty()&&log.front().operation==OriginalLifecycleOperation::set_flags&&log.front().flags==0x1300,"Restore runs the PreSpawn Focus first, no old-state Blur");
  bool blur=false,removed=false,collisionsOff=false,animation=false;for(const auto&q:log){
      blur|=q.operation==OriginalLifecycleOperation::revive||q.operation==OriginalLifecycleOperation::init_physical||q.operation==OriginalLifecycleOperation::clear_idle_suppressed||q.operation==OriginalLifecycleOperation::restore_initial_position;
      removed|=q.operation==OriginalLifecycleOperation::remove_physical;animation|=q.operation==OriginalLifecycleOperation::select_state_animation;
      collisionsOff|=q.operation==OriginalLifecycleOperation::set_collisions_enabled&&!q.collisions_enabled;}
  check(!blur&&removed&&collisionsOff&&animation,"Focus(17) removes the body, disables collisions and selects the PreSpawn pose without Blur effects");
  check(log.back().operation==OriginalLifecycleOperation::notify_state_changed&&log.back().previous_state==3&&log.back().state==17,"State notification carries the replaced live state");
  check(death.status(91)->state==17&&death.status(91)->flags==0x1300&&!death.combat_enabled(91),"Restored actor is PreSpawn17 and not combat-ready");
  check(!death.enter_restored_pre_spawn(91,spawned,true,3,e),"A second registration for the same actor must be refused");
  check(death.spawn(91,e)&&death.status(91)->state==1,"The scene can spawn the restored actor again");
  check(death.put_idle(91,e),e);death.detach_for_restore();ActorState again;again.id=91;expected=&again;
  check(death.enter_restored_pre_spawn(91,again,true,3,e)&&death.status(91)->state==17,"A repeated death restores the same actor to PreSpawn17 again");
 }
 lifecycle.clear();check(!lifecycle.status(actor.id),"Restore detach must clear lifecycle references");
 ActorState missing;missing.id=2;OriginalLifecycleFacts bad=facts;bad.pre_spawn_has_animation.reset();check(!lifecycle.add(missing,bad,e),"Unbound source PreSpawn fact accepted");
 std::cout<<"Original lifecycle tests passed preset0->state17->Spawn1->Idle3 sameActor=true sourceFlags=true\n";
}catch(const std::exception&ex){std::cerr<<ex.what()<<"\n";return 1;}}
