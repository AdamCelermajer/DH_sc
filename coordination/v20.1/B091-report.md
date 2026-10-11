# B091: mobs passive after a scripted animation

## 2026-10-11 resumed investigation before additional behavior changes (B091 + B094 + B080)

The user explicitly identifies B094 as the **brown boss lizard**, not Celeste/Rene. The old fix094 report's identity premise and its INTENDED verdict are invalid for this report. Shot-10/13 show the large brown mace-carrying actor without a ring/plate; a still alone does not prove continuous passivity. This actor matches the captain model and room in the reference.

Visual evidence inspected first: original P1 v1.0.3 `z_Zky7qQdYs`, 462–478 s sampled at 2 fps (`fix091/audit/captain462-478.png`). At 466–470.5 the captain approaches a captive, strikes/fells it, then HUD returns. At 474–477.5 the brown actor has the **Bogwomp Chief** plate and red ring and exchanges blows with the hero. Troll sequence 349–363 s at 2 fps (`troll349-363.png`) shows entry/roar, HUD return, TRUNGUS ring/plate and hits. No original SKIP press occurs in these excerpts.

Logic evidence: `Script_PlayActorAnim::Execute` 0x45e890 skips its body when skip=1 and otherwise calls `SM_SetAnimState(...,0,1)`; setter 0x3c1aa8 enters state14 and writes idle-after-stop byte+69. `CSAnim::OnFocus` 0x3c324c writes flags8768 and selects its animation; `CSAnim::OnUpdate` 0x3c1a3c returns Idle when character animation-playing byte+5248 is false and +1345 is true. `Script_SpawnCharacter::Execute` 0x45f400 enters Spawn even when skipped; `CSSpawn::OnEvent` 0x3c0b04 admits physical interaction at marker40 `is_interactive`. `ScriptManager::SkipScript` 0x460530 sets the skip request/flushes messages and does not cancel character animators. Existing source-adapter kind31 calls `set_idle(id,!skip)` for allowed live actors; EndScriptedCutScene's All selector includes the captain.

Actual production reproduction uses compiled bank + authored TriggerZone contact, no campaign-command injection; isolated copy of the earlier privately generated post-opening m7-skip save. Fresh-player must remain enabled because main.cpp:1284 only loads that direct profile before combat bootstrap under that option. Initial unsaved/direct runs overlapped Swamp_Intro and were discarded as combat evidence. On valid seeded contact, pre-fix captain played ends with lifecycle3 but remains in scripted animation14; skipped at frame80 ends lifecycle1, physical0, HP260 despite End's ordinary session Idle, while the rendered actor tracks the player. This exposes two owners disagreeing: `set_idle` updates CombatSession only, leaving the lifecycle's Spawn state/body gate unchanged; skip also erases running scripted completion timers.

Expected minimal reusable fix: preserve already-playing actor clip completion across skip (new skipped PlayActorAnim remains a no-op), and synchronize a lifecycle actor's explicit non-waiting Idle through the lifecycle owner before ordinary session selection. Never revive dead/limbus/pre-spawn actors merely because a timer ends. Existing B091 no-chain completion callback supplies the source CSAnim return to Idle for live gameplay actors.

Verification defined before coding: host tests for no early release, chained/replacement clips, one completion, callback failure, and completion surviving skip; lifecycle test for skipped Spawn->Idle physical admission and unchanged authored anchor; compiled-bank real EXE runs captain played, early/late skip, troll skipped/played; require plate/ring plus legitimate attacks/damage, and captain legitimate kill->death->corpse removal if reachable. Full ctest with toolchain DLL PATH. Do not rerun an unchanged passing long scene.

Uncertainties: video v1.0.3 versus IDA APK export version; SKIP timing only source-backed; exact shot actor placement may be a different Type2/3 instance. Full route traversal is separate from isolated normal authored-contact fixtures. New runtime result and hash remain pending below.

Branch fix/b091 (from v20.1/integrate), build fix091 (-Jobs 2). Scratch: `.local-inputs/claude-v20.1/fix091/` (mk2.py, addcmd2.py, run.ps1; runs c1/c2/r1).

## Evidence
- Screenshots shot-10 and shot-13 (observed): a brown wolf-like big creature (shot-13 with a mace) next to/near the hero, no target ring, no attack. Model matches `lizardman_big.bdae`: profiles Swamp_LizadMan_Type2/Type3 (propertyRow 362/363, aiName LizadManLarge). The declared `_prim_Monster_05_325` (Swamp_LizadMan_Type3, Limbus, auto_spawn=0) is spawned by script LizCaptain_Intro (trigger BigLizIntro, boardwalk ~(-6400,9600)).
- IDA (read this session): Script_PlayActorAnim::Execute (0x47... in pseudocode-all.c ~247632) calls CharStateMachine::SM_SetAnimState(sm, clip+2, 0, 1): enters CSAnim (state 14) with flag byte +69 = 1. CSAnim::OnUpdate (0x3c1a3c) calls SM_SetIdleState once the clip is over when that flag is set (CSAnim::OnEvent does the same on event 34 for flag +68). So in the original any actor that plays a scripted clip returns to Idle (state 3, AI) by itself. LizCaptain_Intro has Spawn, PlayActorAnim x4, Unlock and NO PutCharacterInIdle for the captain, so the original relies on this.
- Video: not checked for this creature this session (no footage located for the LizCaptain intro); not verified.

## Reproduction (real EXE, quiet, isolated, harness `--campaign-command LizCaptain_Intro:i:frame` with the intro commands, no skip of this script)
- Before (exe-91, run c2/run-old.log): `Lifecycle final _prim_Monster_05_325 state=1 physical=0` (Spawn state, never Idle): untargetable and passive. This is the shot-10/13 symptom. Same mechanism applies to every script-spawned monster that has no PutCharacterInIdle after its clips (our host kept the actor in Spawn/CSAnim, only PutCharacterInIdle moved it on).
- Lizard/troll/moth ordinary paths from B080 unchanged (those scripts end with PutCharacterInIdle).

## Root cause
The host ignored CSAnim::OnUpdate: when a scripted actor clip ends (advance_actor_clips, no chained clip) nothing returned the actor to Idle.

## Fix (general, small)
- `campaign_host.hpp/.cpp`: new service `scripted_clip_finished(id, released, err)` called when a scripted clip with no chained clip ends; if released, the held cutscene pose is dropped.
- `main.cpp` (one hunk, B091): the owner applies it only to gameplay actors (`scriptGameplayActors` from B080: spawned by script, never retired to Limbus) that are alive and in a state other than Idle/Limbus/PreSpawn: `set_actor_state(id,3)` (lifecycle put_idle). Cutscene props (opening troll, serpent) are untouched.

## After
Run c2 with the fixed exe: `[campaign] scripted clip finished: gameplay actor _prim_Monster_05_325 state=14 -> Idle`, lifecycle 1 -> 3, `Lifecycle final ... state=3 enabled=1 physical=1 collisions=1` (targetable, combat enabled). Build exit 0.

## Observed / inferred / not verified
- Observed: before, captain stuck in state 1; after, state 3 physical. Screens r1 sheet: lizards and moths fine on the normal route (opening skipped, CombatTuto, MothIntro skipped).
- Inferred: shot-10/13 creature is a big lizardman (Type3 captain or similar Limbus-spawned) stuck after its intro; matches the symptom, not proven from the screenshot itself.
- Not verified: no unit test added (CampaignHost has no host-level test harness for clips); ctest not run this session; no visual attack/target-ring frame of the captain after the fix; other scripts with spawn+clip (Ambush, Camp, Serpent) not individually replayed; video comparison missing.

Package files required: none.
