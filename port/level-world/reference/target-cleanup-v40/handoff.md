# Generic target cleanup V40

## Proven production gap and change

The pre-V40 live player target-event adapter omitted `target_event_clear_aggro`. Original CharAI.OnTargetOutOfSight0xc requires this service before its selected AIS endpoint. A living target moving beyond sight therefore reached a missing source provider. This is a generic movement/target lifecycle gap, not a skill-specific visual issue.

`CharacterWorldClearAggroV40` composes whole original AI_ClearAggro3d6d68 on the existing reciprocal maps, TargetBindings, source target/controller and provider lifetime. It uses the existing native aggro erase and complete SetTarget kernel. No second target, table, heading, FSM, health, actor or death observer is created.

Ordered behavior:

1. NULL other returns before any receiver/provider access.
2. If the outgoing relation exists, erase outgoing and reciprocal incoming, then deliver actual selected target OnDeAggro(other=owner).
3. Reload source receiverAI.owner and targetAI.current target after the notification. A callback retargeting either can skip the remaining clear/Stop.
4. If the target still targets that owner, execute actual AI_SetTarget(NULL,false).
5. Reload Character.controller378 after the setter, then deliver actual Cmd_Stop to that freshly produced controller. Original load is3d6ee0 after SetTarget3d6edc.

Missing notification/Stop/setter services preserve the executed mutation prefix and exact error. The owner performs no automatic retry and records no synthetic once-only ledger. Existing V1 ClearAggro API/source remain unchanged; V40 is a separate live-borrow composition so its source controller reload does not freeze an early projection.

## Root wiring

Root reported these insertions integrated and both-ABI APK compiled during this task; no live receipt is included here yet:

- Add `character_world_clear_aggro_v40.cpp` to level-world; it depends on existing `aggro.cpp`, `character_target_bindings.cpp` and script scope validity API.
- Include its header in renderer.
- Declare `static int player_target_clear_aggro_v40(PlayerSkillsRuntime&,std::uintptr_t,std::uintptr_t);` before renderer_player_target_frame_v2.inc.
- Include `renderer_target_cleanup_v40.inc` after target-frame/NPC command/player aggro adapters.
- Add `case target_event_clear_aggro:return player_target_clear_aggro_v40(t,q->subject,q->other);` in its target-event service.

The include borrows exactly current player/NPC maps and TargetBindings, pins existing World/provider storage for the synchronous call, routes notifications through player_aggro_callback_v2, and routes NPC Stop through refreshed real NpcScriptCommands (including actual heading/Character event path). Global World teardown/reload must not destroy object_groups table backing during this synchronous source invocation. Positive selected AIS/audio/physics branches still require their genuine existing providers; no placeholder fallback was added.

## T05/T06 evidence and limits

| Item | Source behavior / current status |
|---|---|
| Attack auto-acquisition | Existing source attack owner uses original target list, current Euler, radius/interaction margin, cone and sort; it replaces no target with an arbitrary nearest development actor. Continued attacks can retain valid live current target when original heading/continuation gates permit. |
| Skill focus | Source Lua Pre/Search target list and LookAt are separate from current AI40 and raw OOI14a4. No forced synchronization was added. Area/directional scripts retain their authored focus choice. |
| Sight / visibility | Captured Search tests visible/zone/interactive/faction/dead/sneak and cone/radius; captured InSight is strict3D view-radius. Neither inspected entry contains a terrain raycast. This does not claim all game scripts lack separate line-of-sight queries. |
| Moving target / loss of sight | New lifecycle contrast exercises `_UpdateTarget` → CharAI.OutSight → V40 reciprocal clear → setter/Stop. Caller target can remain selected: Player inherited OutSight is literal return; do not invent caller clear/HUD hide. |
| Reentry / changed controller |64 original direct ClearAggro cases; native133 checks underO1/O2ASAN+UBSAN including callback retarget, changed owner, changed controller and setter-time controller replacement. Native setter, maps and ordered failure prefixes execute; notification and Stop endpoints are fixtures. |
| Dead monster / noninteractive prefix | Source frame clears AI40/current and AI44/last before OutSight and early returns; old cached alive byte can remain1. Test preserves this exact behavior. No fabricated TargetDied from a death-store prefix. |
| Interactive dead target | Source alive transition emits0xa then stores cached alive after callback. Full AISPlayer death stop/clear/sync is existing separate owner, not duplicated here. |
| Target swapped during callback | Captured sight value commits after callback even if current target changes. Test preserves original ordering instead of overwriting it with a guessed fresh cache. |
| Missing actor | Named required failure; no origin substitute or guessed dead flag. Original dangling-pointer/invalid-lifetime domain is not silently converted into success. |
| Awaiting spawn / limbus | Original state17/state0 early gates preserve target without dispatch. Source null current state isUINT_MAX, not guessed Idle3. |
| HUD / target marker | Consumers continue borrowing current/last/OOI and actual life/source scene. V40 does not directly hide HUD, clear OOI or publish a death event. Root live validation is required for rendered lifecycle. |
| Ranged/nonenemy continuation | V38 closes close-range query. Selected ranged/close AIS and nonenemy interaction/attack continuations still have explicit required source boundaries; this package does not certify full ranged gameplay. |
| Lethal damage / awards | Full CtrlKill loot/XP/quest/AI ownership remains a separate system. No direct dead write, extra award observer or loot/XP skip was introduced. |

## Proof receipts

- `world-clear-aggro-v40-host.json`: O1/O2,133 checks each, ASAN+UBSAN/leak checks, frozen dependency hashes. Includes64 original direct contrasts from immutable original capture. Original map/ownership/erase instructions execute; callback endpoints were supplied in the original oracle too.
- `target-lifecycle-v40-host.json`: eight contrasts eachO1/O2 for movement,3D boundary, dead early prefix, interactive dead transition, callback retarget, missing source, spawn and limbus. Debug/selectedAIS/audio endpoints are explicit fixtures; not full live HUD/AI/audio acceptance.
- `target-cleanup-v40-strict.json` plus private current-renderer logs: production owner and renderer binding pass strict ARM64/x86_64. Root independently reported combined APK both-ABI pass.

No emulator/ADB/app operations occurred. Root owns Xiaomi live validation. Do not mark T05/T06 fully verified from these host/source proofs alone.
