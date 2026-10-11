# B091: mobs passive after a scripted animation

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
