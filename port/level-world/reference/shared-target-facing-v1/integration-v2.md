# Shared target/facing integration V2

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

## Connected correction

`PlayerSkillsRuntime::WorldActor::refresh` projects the SAME actor runtime's
actual Euler Z (`GameObject+174`) to Search.Object48.rotation. Previously that
projection never had a producer, so both melee and skill acquisition always
queried the zero-facing cone. Desired heading (`+178`) remains separate and
changes through the original LookAt/rotation frame chain. The same refresh
now publishes current signed resolved properties198/199 to source Character
1310/1314. The payload begins ff8; these are Special_Sneak/Sneak_Detection.

The source Bashdown script searches160, FrontalFirst, then caches and LookAt's
the selected target before spending mana. Its Use rolls the cached target.
Charge searches300 FrontalFirst and LookAt's top in Pre, then searches300 with
120-degree cone/NoSort in Use. Both therefore use this shared projection.
The actual scripts are cached under character-skill-session-v2/cache/data/scripts/skills;
these script identities do not identify the skill visible in an online video.

## Connected source frame coordinator

Add `character_world_target_frame_v2.cpp` to level-world. Construct one
CharacterWorldTargetFrameV2 borrowing the existing world runtime, attack_geometry,
actual AI table and WorldTargetFrameServicesV2. Invoke update on the canonical
`world->player_object->target`; its identity is the embedded AI identity,
while target.owner->identity is the Character identity. No second target state.

Providers:

- machine_state: actual SM_GetState selection, state17 awaiting, state0 limbus,
  UINT_MAX for NULL current state. Animation labels are not the provider.
- raise_event: original Character.RaiseEvent route, including CharAI handler
  prefixes before selected AIS dispatch. Generic Lua OnTargetDied is insufficient.
- close_range: whole AI_IsInCloseRange, required only for actual ranged owner.

The owner supplies actual registered IsInteractive/IsDead, GetCharAIId,
GetTargetPosition, sight radius/arithmetic, Character.CanRangeAttack and whole
melee/ranged geometry. Positive non-Character interaction and ranged CloseRange
remain explicit services. Calling the frame kernel is not full AIS acceptance.

The production TU is now in level-world CMake. `renderer_player_target_frame_v2.inc`
constructs the retained owner after attack/heading binding and advances it before
the source FSM frame. RaiseEvent runs the existing Character→CharAI callable
table, whole target-handler prefix and freshly selected AIS virtual. No direct
Lua shortcut or independent death publisher was added.

The IPhone OutRange tail now composes whole source3dc698: actual +4a seeking
and flags520 gate, Idle/Attacking/HasPath checks, GetInteractionSpot, gated
point Cmd_MoveTo→Character remote gate→GameObject.PathTo→same PFWorld route,
then live HasPath and SetTarget/SyncLastTarget/stop-seeking reloads. The actual
debug/module owner controls path search. Positive profiling requires its real
statistics service. Interaction lookup retains the constructor-null cached
node/check state and searches the actual Scene forest; a found interaction
node requires its genuine absolute position rather than accepting a local
animation matrix. Other selected ranged/close and nonenemy melee continuations
remain required. Source GetTargetPosition handles an absent interaction node.

Source skill `fx_` events now restore their stripped prefix and dispatch the
same retained CharacterCombatFxRuntimeV2 ordered set-name lookup. An unresolved
positive effect/material service remains a failure; no trail or effect ID is
invented from the reference video.

Do NOT call World.notify_death at the HP/dead prefix. Source _UpdateTarget
first rejects a noninteractive target, clears current+last and raises event0c.
An ordinary hostile corpse is rejected here; a friendly non-monster corpse can
reach event0a. Captured alive/sight bytes commit AFTER synchronous events.

## Player event endpoint

Header-only `player_target_died_v2.hpp` implements whole inherited
AISPlayer.OnTargetDied3dd84c. Bind its borrow callback to the actual selected
AIS Character+98, freshly reloaded at each of three phases. First supplies
actual controller and CharacterControlServices; second supplies canonical
TargetState48/TargetServices; third reloads canonical target for SyncLastTarget.
Order is gated Cmd_Stop, SetTarget(NULL,false), SyncLastTarget. A blocked void
Cmd_Stop still proceeds to target clear; failed source service preserves prefix.

AISDefault OnTargetRevived3dbeb0, OutOfSight3dbeb4, InSight3dbeb8 are literal
bx-lr. IPhone ranged/close/melee/out-range endpoints tail to whole AISDefault
3dc560/3dc300/3dc284/3dc698 respectively and cannot be accepted as empty methods.
Full captures are player-target-lifecycle-original.json.

## Validation limits

Search correction: both-ABI strict compile of renderer and eight dependent TUs;
APK-linked isolated source search/rotation test PASS29. Frame and player endpoint:
APK-linked isolated test PASS11, registered real native World classification/life,
distinct AI/Character identities, source post-event stores and failure prefixes.
Pursuit composition adds10 APK-linked ordering/gate/reentry/failure checks.
The fixture's event, controllable and debug callbacks are explicitly fixtures;
this is not live Lua/frame/GPU or video parity acceptance. No app lifecycle,
touch, APK install or emulator routing occurred in these tests.

Source _UpdateTarget's existing original differential is2225 cases; CharAI
target handlers existing differential2013 cases. The new composition tests do
not re-label those existing proofs as proof of the new renderer wiring.
