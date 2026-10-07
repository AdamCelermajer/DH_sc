# Renderer NPC reactions V2 integration

`renderer_npc_reactions_v2.inc` compiles against the current complete renderer
for BOTH arm64-v8a and x86_64 using its actual CMake compile commands. Receipt
command: `python .local-inputs/check_renderer_npc_reactions_v2.py`. This creates
only `.local-inputs/renderer-npc-reactions-v2-check.cpp`; main renderer untouched.

## Exact insertion points

1. Add portable headers `npc_skill_reactions_v2.hpp`,
   `character_head_object_v2.hpp`, `canonical_point3d_globals_v1.hpp`. Existing
   renderer already includes collision lifecycle/native filter/Box2D headers.
2. Before MonsterScriptHandle declaration: `class RendererNpcReactionsV2;`.
   On MonsterScriptHandle retain `std::shared_ptr<RendererNpcReactionsV2>
   reactions;` (shared_ptr allows the current inline destructor to see an
   incomplete type). This is orchestration, not new FSM/properties/timers.
3. Include `renderer_npc_reactions_v2.inc` immediately after
   `renderer_player_heading_v1.inc`, where PlayerSkillsRuntime and actual
   NativeWorld/PF/global services are fully declared.
4. After `bind_npc_injuries(t)`, construct each same NPC adapter with
   `std::make_shared<RendererNpcReactionsV2>(m,t,inputs)`. Pin it on m. Inputs
   borrow actual ObjectBase byte118, actual source PhysicalObject or genuine
   null, exact primary/secondary/saved-filter/disabled fields of that physical
   owner, and actual BuffOwner if produced. The current renderer has NO NPC
   physical/buff owner: do not substitute Prince or decor bodies. Current
   machine.physical must remain its source constructor null. Original
   ObjectBaseC1 33f2c8 initializes remote byte118=0; expose that field on the
   retained NPC receiver once, and route future source network writes there.
5. SkillApply callback near the existing `npc->injury->application` hook:
   `if (npc->reactions) { int h=npc->reactions->application(*q,out);
   if(h) return h==1?0:-1; }` before old injury fallback. It handles11–16.
6. NPC state_method: before old injury hook call `m.reactions->body(machine,
   *request)` and translate handled1 to delivery0. Keep existing notification
   callbacks. Body wrapper republishes controller byte8 after source mutations.
7. Add a remaining_updates callback at construct_retained_monster_actor_v1's
   `WorldNpcStateServicesV1` setup. It delegates to the adapter's `update`;
   translate1 to0, otherwise preserve missing-service failure. States8/9/10
   own genuine source updates; do not run a second frame/update clock.
8. NPC native_frame: for fsm_set_stun/fsm_set_scare delegate `frame_effect` and
   translate nonnegative source completion to delivery0. Existing FSM Update
   alone advances elapsed time.
9. NPC animation_service helper3d4204/3d3ff8 guard: accept states3,8,9,10,11.
   The existing original AnimationAI body returns genuine empty completion
   for8/9/10. Retain actual authored event22→CharAI→AISExternal OnEndOfAnim→FSM.
10. Render retained NPC CPU scene for states8/9/10 as already done for11;
    do not sample the old group.scheduler over a live reaction animation.
11. Sole NPC timer scan: `reactions->update_timers(dt,source_script_blocked)`.
    Do not also call session.update_timers. Existing TimerStore is reused;
    every timer gets its actual expiry, and a required callback failure is
    recorded while later source timers continue scanning. StartTimer uses
    existing capacity; capacity expansion remains required when reached.

## Concrete callbacks now composed

* Animation uses same CharacterAnimationInstance/start, including actual
  pending animation override consumption. StopLoop(false) calls its real
  BlendedPlayback. No invented completion event.
* GetAnimStance uses actual World IsPlayer then actual AnimStances constant;
  complete nonplayer source branch yields0 for every COUNT_IPHONE. No fake
  empty inventory is needed/reached in that source branch.
* Heading uses same m.runtime PathController/PathObject/position/destination,
  same target/object services, actual GetTargetPosition, source network−1 /
  byte118 remote predicate and actual flags520 bit2 physical policy. Scare's
  random point heading and NULL object heading invoke the whole proven owner.
* Random uses same live CombatRandom via dh2_combat_random, proven arithmetic
  equivalent to source clone3c26a0, retaining exact four-draw scare ordering.
* LookAt uses actual world target position and retained rotation field.
* Collision Enable uses CharacterCollisionLifecycleV1, same PFObject/floor
  registry and proven Character virtual IsPFObstacle/weight/extent producers.
  Missing initialized PF backing yields real required failure.
* Physical setFilter46ece8/resetFilter46ec6c use exact borrowed source shapes,
  saved halfwords and byte26, genuine Box2D SetFilterData/Refilter, re-reading
  secondary after primary. Pin/unpin use actual NativeBody functions. Current
  null body source branches don't invoke these services.
* Debug uses same World Debug Load/Get. CancelSneaking delegates same NPC
  injury receiver and canonical interactive415.
* Timer43/44 performs real CharAI virtual84/88, validates actual selected
  Session and inherited AISExternal callback3dbee0/3dbee4, then delivers SAME
  FSM. Its actual source wrapper clears pending bits; no manual mask reset.

## Still reached-required providers (no fabricated acceptance)

`inputs.timer` must implement other actual CharAI timer events33/34/29/35 etc;
the current NPC animation-only callback lacks those bodies. It is called for
every other timer, including source startup AI/DoT timers. Diagnostics remain
visible, and failures don't mask subsequent43/44 expiry.

Positive Slow requires SAME NPC BuffOwner and genuine BuffFX/expiration/property
group wiring. Current WorldSkillExecution registration has buffs=null for NPCs;
adapter cannot treat that as successful slow. Boss/duration0/class absence are
genuine source early returns.

Dead knockback event23 requires whole actual ANIM_Stop3c9924 and source
SetDeadState. `inputs.stop_animation` and `inputs.dead` expose those remaining
bindings. ANIM_Stop differs from deferred StopLoop: resets stack depth, stops
actual visual controller and synchronously raises22 iff sequence was open.
Existing BlendedPlayback does not expose that whole operation; the adapter
rejects its absence instead of fabricating a finite animator event.

All those requirements are explicit at their reached branches. Ordinary alive
push/stun/scare bodies don't reach dead stop. No new active AIS, state graph,
HP, canonical target, RNG or timer storage is created.

## Native test

`port/level-world/tests/character_knockback_reaction_v1.cpp` is standalone;
link actual new character_knockback_reaction_v1.cpp and existing game-data
property implementation. Root's APK-linked test can add just this new TU to
its executable before the new module reaches APK CMake. Trace callbacks test
source order/prefix semantics only, not successful live body/animation resources.
Strict ARM64 test syntax passed; runtime not claimed by this handoff.
