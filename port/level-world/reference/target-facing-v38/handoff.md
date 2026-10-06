# Shared target/facing V38

This package closes two audited source discrepancies. It does not certify every skill or live camera/FX alignment.

1. `WorldTargetFrameServicesV2.close_range` was not supplied in the renderer. Any true Character.CanRangeAttack path therefore failed before choosing close/ranged/out-of-range AIS events. New whole `character_close_range_v38` preserves the captured3d63d8 branch: explicit target else same current AI40; null return without services; Handle.AsCharacter; typef4==0; interaction8; Character virtual128 parameters; actual GetTargetPosition for both actors; 3D distance; debug load/query order; strict minimum-square comparison. Nonenemy/noncharacter branches require the existing genuine interaction provider. Signed32 MUL wrapping is retained; no radius invention.
2. Normal FSM `prince_look_service` used object_groups raw position and fixed actor ID, while skill Lua used the source-registered target owner. New `control_services_v38()` exposes that SAME existing source control endpoint so the normal focus caller can use selected node/cache position too. It updates desired Character/controller heading, not Euler directly and not camera follow. Existing GameObject rotation update remains the producer of actual EulerZ used in search/visual orientation.

## Exact root integration

- Add `port/level-world/character_close_range_v38.cpp` to level-world. Existing geometry TU now calls it; rebuild all consumers of its additive header.
- Include `renderer_target_facing_v38.inc` after complete PlayerSkillsRuntime declaration and before prince_look_service.
- Replace only the body of `prince_look_service` with `return player_target_facing_service_v38(request,response);`. Preserve its signature.
- In Character service `look_at`, construct ControllerCommandState32.owner and direct `dh2_character_control` owner from `player_skills_runtime->world->player_object->identity`, with guarded genuine readiness. Do not retain hardcoded0x100000001. Keep the original command-vs-direct branch and original controller flags. Early initialization without the actual registered receiver must fail named required readiness rather than use raw placement fallback.
- In bind_target_frame assign `services.close_range=player_target_close_range_service_v38;` before constructing the frame owner.
- No change to AI40/current target, AI44/last target, Character14a4 OOI, life/death, Save, camera, skill target lists, state timer or FX clocks.

## Verified evidence rows

| Behavior | Evidence | Acceptance boundary |
|---|---|---|
| Close-range arithmetic / strict boundary / 3D / signed overflow |160 original ARM3d63d8 captures, native sanitized replay; total183 checks | Handle/type/interaction/range/debug endpoints are explicit fixtures; original GetTargetPosition and arithmetic execute. |
| Mandatory close-source failure prefixes |10 injected callback failures retain output and exact diagnostic | Host coordinator; no silent successful missing callback. |
| Ranged resolved projectile shortcut | Same World fixture uses actual borrowed224 properties, no inventory access | Synthetic source facts; actual app ranged weapon not exercised. |
| Moving/cardinal target facing | Same registered target cache changes in4 directions, Character/controller desired angle match sourceLookAt | Native fixture13 checks underO1/O2ASAN+UBSAN; not live visual proof. |
| Removed/missing actor | Same registry remove then source position endpoint rejects | No arbitrary origin fallback. |
| Live normal melee/targeted Bash | Prior root accepted reports fx-target-v28 and recent nonlethal checkpoints | Historical separate APK proof; no new runtime acceptance from this package. |
| Dead target handling | Existing `_UpdateTarget` source clears noninteractive dead monster before range; surviving interactive dead friend uses original event0xa, post-callback alive store. Existing registered frame tests cover this. | Whole lethal CtrlKill/loot/XP pipeline remains separate; no new death publisher. |
| Search direction | Existing `character_world_target_pose_v2` borrows actual EulerZ174, not desired heading178; tests exercise attack and skill snapshot search | No forced instant facing; existing source rotation rates remain. |
| Targeted vs area/directional cast | Preserve Lua/source script choice of LookAt and target list. Do not promote PreSearch to AI40 or force area casts toward arbitrary enemy | No all-script or all-skills acceptance claim. |
| FX anchors and sword trail | Existing V28 anchor rotation uses actual retained visual root absolute transform; source GetTargetPosition remains separate from enemy HUD head anchor. Source warm pool resets scale, source root/node transforms apply once | No new camera-angle/slope/live sword-trail proof inV38. Host math correctness cannot establish visual parity. |

## Still required for checklist closure

Root must compile its actual forwarding integration and run only after host guard/lease: moving selected enemy, retarget, removed/dead selected enemy, normal melee + actual ranged owner, supported targeted and directional/area skill at several actual headings/camera angles. Record do_skill + targetHP/outcome + actual GPU draw resource/anchor and no required-prefix error. A dodge/block is a legitimate no-change result. Do not change RNG or force learned skills. All45 authored effect assets being parseable does not prove all86 skills execute or align.

Source captures used: character-target-update/helpers/reference/original-functions.asm3d63d8; existing character-attack geometry original proof; CharacterWorldTargetOwnerV1 source control endpoint. No emulator, app lifecycle or resource stress was performed. Camera focus/follow remains a distinct source system and is not changed.
