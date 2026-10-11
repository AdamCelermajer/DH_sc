# B080 / B084: mobs not targetable after a cutscene (skipped or not)

Branch fix/b080 (from windows-foundation 2a8faf0f), build `fix080` (-Jobs 2). Scratch/evidence: `.local-inputs/claude-v20.1/fix080/` (job.args, run.log, sheet.png per run; tools in `fix080-tools/`).
Baseline EXE = fix080 build of windows-foundation before the change, `fix080/exe-base.exe`; fixed EXE `fix080/exe-fix.exe`.

## Evidence
- Screenshots shot-2/shot-4 (observed): hero in the moth/lizard room (medallion floor, bone pile), Rene bottom right, a green lizard-type creature next to the hero (shot-2), two flying moths over a corpse (shot-4). The same layout is reproduced by our run m6 (frames 2820-3000).
- Video P1 z_Zky7qQdYs 150-250 s (sheet `vid/sheetA.jpg`, observed): after the opening the Combat Tutorial with SKIP shown, lizards at the top-left of the room, Rene bottom right. The MothIntro trigger does not fire in the inspected video (earlier B068 finding, unchanged), so the post-intro moth behaviour has NO direct video evidence.
- IDA (not re-decoded this session; taken from the B068/B071/v20 reports): SkipScript/ExecuteScript pass the skip flag to every command; Script_PlayActorAnim and PlayAnimByName do nothing when skipping; MoveActor 0x45f0a8 uses Cmd_WarpTo 0x405318 when skipping; Script_PutCharacterInIdle 0x45d590 wait=!skip then SM_SetIdleState 0x3c1a00. The original has no "animation-only" actor class: every Monster-class actor spawned by a script ends in state 3 with the monster AI.

## Reproduction (real EXE, quiet runs, fresh private saves, route = opening skipped at 400, CombatTuto skipped at 2250, MothIntro skipped at 2800, walking the real route west; attack harness from 3000)
- Skip audit, lifecycle end state, opening played vs skipped: identical (troll state 0, lizards 17, moths 17, the same lines in `o-skip`/`o-full` of v20cine and `o7-full`). No difference in targetable/AI/visibility/scripted flag/positions between the skip fast-forward and the played end for the opening.
- Lizards (LizardMan_Intro + CombatTuto): after the opening skip and the CombatTuto skip both lizards are real combatants (state 3, physical, collisions) and are hit/killed (`l2-skip`, `l4-skip`: Damage lines both ways). NOT reproduced. Lizard 2 stays at its spawn point until the hero comes near, in played and skipped runs alike (not a skip effect).
- Moths: after MothIntro (skipped `m6-skip`, played) FakeMoth_01/02 are lifecycle state 3 but log `Animation-only final ... profile=Swamp_MovieMoth`: untargetable, no attacks, no AI. The skipped and the played path are the same, so skip is not the cause (reproduces B084/B080 for the moths).

## Root cause
main.cpp admits every `auto_spawn=0` declared character whose profile is not in the authored combat list as an animation-only presentation actor ("script visual actor admitted without combat policy"). That was meant for cutscene props (opening troll, serpent actors) but also catches monsters the script spawns for gameplay: `_prim_Monster_FakeMoth_01/02` (profile Swamp_MovieMoth, which has real combat stats: B013 J-report). They end the script in Idle (PutCharacterInIdle, LookActor LocalPlayer, Unlock) but can never be targeted or fight.

## Fix (general, data-driven)
- `collect_script_gameplay_actors(runtime)` (features/campaign_host/actor_clip_manifest.*): names spawned by Script_SpawnCharacter (kind 30) and never retired by Script_PutCharacterInLimbus (kind 29) in any loaded script. A retired actor is a cutscene prop (opening troll), the rest are gameplay monsters.
- main.cpp (one hunk, `B080`): such actors skip the animation-only branch and get the existing derived combat policy (`derive_enemy_combat_policy_v1`, same as other declared monsters). No name or map list.
- Diagnostic harness: `--campaign-skip-frame` accepts a comma list (A,B,C) to press SKIP in several scenes of one run.
- Test: `campaign_host` (new asserts: moths are gameplay actors, `_prim_ActorTroll` is not). ctest: all pass except session_skill_binding (known); winmm_pump_priority_v1 / startup_loading_art_v1 failed only when run with my shell PATH (0xc0000135) and pass with the llvm-mingw bin on PATH.

## After (fixed EXE)
- `m7-skip` (opening, CombatTuto and MothIntro all skipped): moths show the "Bog Moth" plate and target ring, take hits and attack; `Lifecycle final` moths state 0 (dead). `m7-play` (opening+CombatTuto skipped, MothIntro played): 16 Damage lines involving the moths, same.
- `o7-full` opening played 5000 frames: lifecycle identical to before, no errors, opening frames normal (cage, moth, Rene/Celeste, end). Not re-run: lizard/troll/serpent scenes (their actors are unchanged: authored profiles or retired/limbo props).

## Observed / inferred / not verified
- Observed: moths were untargetable and passive in skipped and played runs before; targetable and fighting after; lizards targetable after skip in my runs; opening skip vs played end state identical.
- Inferred: that the intro moths should become ordinary hostile monsters after MothIntro (script ends in Idle + look at player, profile has combat stats, user expectation); the shot-2 creature is a lizard-type mob, whose untargetability I could NOT reproduce, so the lizard/"troll" part of B080 is not confirmed fixed. Possible cause not excluded: skip pressed in a different phase or a different route than my harness.
- Not verified: original post-MothIntro behaviour in video (trigger not in the footage); TrollReturn (`_prim_Monster_53_03_001`) skipped then fought (the seeded fixture overlaps the opening, route too long); Serpent/Witch/Camp/Captain skip paths (no actor-state difference expected from code review: fast-forward runs the same commands with skip=1).
- Remaining skip-path notes from code audit (no behaviour change made): `fast_forward` clears `actor_clips_` but not `held_clip_actors_` (cleared later by set_idle(!wait)/MoveActor/SetActorPosition); MarkCharacterAsScripted is stored only, no AI consumer.

Package files required: none.
