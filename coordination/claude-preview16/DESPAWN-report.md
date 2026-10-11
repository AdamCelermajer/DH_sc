# DESPAWN report (Preview 16, branch p16/despawn, from p16/integrate 2d703432)

Status: PARTIAL. Verified in the real EXE: death -> body release -> Despawn_Delay -> hide -> pool slot released and reused;
barrel moth summon (rolls) end to end with spawn/idle/engage; plant/zombie summon variants refuse with a logged reason.
NOT verified: the Despawn clip plays (gated off, see gap 1); visual proof that the corpse disappears (inconclusive frames).
Commits (not pushed): `0bf1da05` (despawn owner, lifecycle state 2, derivation, glue), `18f083bb` (hide path verified,
admission refusal). ctest: 130/131 pass; only `session_skill_binding` fails (allowed).

## 1. Investigation

### 1a. Death -> despawn state machine (IDA + registration plan)
- Sources: `pseudocode-all.c`; `port/level-world/reference/character-monster-state-ownership/registration-plan.json`
  (SM_RegisterEvent records from each state's OnInit, original addresses).
- `CSDead::OnEvent` 0x3c4c3c: event 34 (death anim end): `SetPhysicalObject(nullptr)`; unless `(vtbl+40)(character)`,
  `TMR_Start(CharacterDesign.Despawn_Delay, event 46)`, flags328 = 64.
- `CSDead::OnFocus` 0x3c4d50: flags328 = 577; the same timer when the death anim is not pending.
- Registered transitions: Dead(12) --event 46--> Despawn(2) (registration-plan, state 12 row). Despawn(2) registers
  events 34 and 64 -> Limbus(0). Limbus(0) registers 47 -> Spawn(1) (respawn timer).
- `CSDespawn::OnFocus` 0x3c32fc: flags328 = 512, `SM_SetAnim(-1)` (the Despawn clip is AnimTable `Despawn`).
- `CSDespawn::OnBlur` 0x3c3794: sets +1328; if `!Character::CanRespawn` (0x3a5248: property 11 > 0 and the group permits)
  clears the local player's target and `ObjectBase::Delete` for `+5348 || IsSummoned` (CharType 5). Otherwise the actor
  goes to Limbus with the respawn timer.
- `Despawn_Delay` = 2000 (`design_pycst.bin`, CharacterDesign). The earlier "unresolved event-46 owner" is resolved by the
  registration plan.
- Fade-out: `VisualObject::StartFadeOut/UpdateFadeOut` are empty in this build; no fade is applied (unchanged from PROFILES).

### 1b. Despawn clip data
- Minion AnimTable 44 (`MothMinion`) Despawn = sequence 443 `Moth_Despawn` (`moth_despawn_01.bdae`), loop=0, 1 step
  (observed in the EXE log). Present in `windows-source-clock-v19-preview-15-rc3/assets`.

### 1c. Reference video (Part 1, after kills) - INCONCLUSIVE
- `.local-inputs/reference-video/dh2-act1/...Part 1...mp4`, 1 fps contact sheets 300-480 s and 430-470 s
  (`.local-inputs/claude-preview16/despawn/ref/`). Bog Moth fights are visible around 420-430 s (labelled "Bog Moth"); the
  Bogwomp kill is around 460 s. At thumbnail size no lying body is identifiable and no vanishing body is seen. A corpse check
  needs full-resolution frames 2-3 s after a Bog Moth kill. Not done.

## 2. Implementation
- `features/despawn/despawn_after_death_v1.{hpp,cpp}` (new): phases dying -> corpse (body released, timer) -> despawning
  (clip) -> done. Event 46 expiry: with a clip, start the Despawn state; without one, hide now. Completion: summoned actors
  release their pool slot; others stay hidden. Transient (`clear()`), duplicate/invalid transitions refused, owner failures
  keep the record for a retry.
- `features/despawn/despawn_after_death_v1_tests.cpp` (new, ctest `despawn_after_death_v1`): summoned clip path, timer
  exact at 2000 ms (125 x 16 ms), no-clip hide, non-summoned no slot release, failure/retry, zero delay, clear.
- `original_actor_lifecycle.{hpp,cpp}`: target state 2 (Despawn; flags 0x200; select state animation), `despawn(id)`
  (Idle only), `release_body(id)` (remove_physical, the CSDead event 34 release), Despawn clip end -> Limbus in
  `animation_finished`.
- `features/physics/session_actor_transition_v1.cpp`: state 2 cases (blur, focus prefix, suffix).
- `combat_session.cpp`: `set_actor_original_state` accepts 2; the source-sequence transition gate admits 2.
- `features/spawn/actor_profile_derivation_v1.cpp`: derived profiles and melee actors publish the `Despawn` state when the
  AnimTable has it (authored XML omits it). `actor_profile_derivation_v1_tests.cpp`: authored comparison skips `Despawn`
  when the authored file has none (the derived clip is still produced).
- `main.cpp` (all `P16 DESPAWN` / `P16 CONTAINERS2` hunks):
  - `despawnTick` (frame loop, after the combat update): tracks dead Idle lifecycle actors (`!alive`), releases the body when
    the death pose has ended (`retained_actor_pose(id)->current_ended()`), feeds ms from the frame dt, polls completion.
  - `Despawn_Delay` read from `design_pycst.bin` through `dh2_script_constants_get` (not hard-coded).
  - Lifecycle `select_state_animation` branch for state 2 (sequence path, not wired live; see gap 1).
  - `--container-script DECL=SCRIPT` (debug, default off): overrides a declaration's OnOpen script (used for plant/zombie).
  - Admission glue: an unadmittable container summon profile is refused with its reason and logged
    (`SPAWN admission refused profile=... reason=...`), not a startup abort (previously `throw` killed the run).
    `profile_available` reports `profile admission refused: <reason>`.
- CMake: `foundation_despawn` library, `dh-foundation` link, test `despawn_after_death_v1`.

## 3. Isolated tests
- `despawn_after_death_v1_tests`: PASS (ctest `despawn_after_death_v1`).
- `actor_profile_derivation_v1_tests`: PASS with the Despawn publication (authored comparison unchanged apart from Despawn).
- ctest full: 130/131; `session_skill_binding` fails (allowed).

## 4. Integrated runtime verification (EXE `build-p16despawn`, quiet/hidden/silent)
Assets: `.local-inputs/claude-preview16/despawn/assets-run` (full copy of the rc3 assets; the old `profiles/assets-run`
is missing v2quests data, so it cannot start this integrate build). Note: these runs have no `--fixed-step`, so the despawn
timer uses wall-clock dt (the 2000 ms is in ms, the frame count varies).

### 4a. Barrel summon end to end (container interact, `--interact-at ..._46@22`, attack flags off unless noted)
Barrel `_prim_DestructibleContainer_05_321_364_22_41_21_187_21_46`, script `moth_spawn_container` (GetRand(0,100) < 25):
| combat seed | roll | outcome |
|---|---|---|
| 4 | 1 | summon, `SPAWN ok ... Swamp_Moth_Minions ... actor=...768 busy=1/2` |
| 3 | 9 | summon |
| 6 | 16 | summon |
| 5 | 26 | no summon (`summon_calls=0`) |
| 7 | 70 | no summon (`summon_calls=0`) |
Earlier c7 run (before the profile merge) also showed 1, 9, 16 summon and 26, 47, 70, 86, 90 not summon; the merged EXE
re-checks 9, 16, 26, 70, 1.
- Lifecycle for the spawned moth (seed 4, `e2e/s4-f240.log`): `17 -> 1 (Spawn) -> 3 (Idle)`,
  `Lifecycle whole sequence finished ... state=3`, `Lifecycle final ... state=3 enabled=1 physical=1`.
- Engagement: with `--enemy-ai` (`eng/e6.log`), the spawned moth hits the player (`Damage attacker=...768 target=<player>`
  at frames 99 and 200). Without AI it does not engage (no `--enemy-ai` in the summon runs).
- Frames: `e2e/s4-f45.png`, `s4-f90.png`, `s4-f160.png`, `s4-f240.png`. The moth is not clearly visible at this scale in
  those frames (a winged shape at the top edge in f90/f160); the arc is not verified frame by frame. The "New Quest: Muck
  Fly" banner is from the quest system.

### 4b. Plant and zombie variants (`--container-script`, seed 3 barrel, 60 frames)
- `plant_spawn_container2` (`var/plant2.log`): derives `EarthTemple_BigSpider`, `EarthTemple_SmallPlant`,
  `EarthTemple_SmallPlantMinion`, `EarthTemple_Spider`; each refused: `Original visual unavailable ... spider_green.bdae` /
  `small_plant.bdae`. The OnOpen Summon line: `outcome=refused reason=profile not admitted: EarthTemple_Spider (profile
  admission refused: ...)`. Run completes (`Rendered frames=60; clean shutdown`).
- `zombie_spawn_container` (`var/zombie.log`): `InfectedBurned` refused (`infected/burned.bdae` absent). Same logged reason.
- `plant_spawn_container` (`var/plant.log`): summons the moth (`spawned=1`), as the source script does.

### 4c. Automatic despawn after death (summoned moth, `dsp/d520.log`, attack flags on)
Sequence in the log (EXE `build-p16despawn`, last run):
- `DESPAWN tracked actor=...768 summoned=1 clip=none frame=154` (dead, lifecycle Idle).
- `DESPAWN death end actor=...768 body=released delay_ms=2000 clip=none frame=212` (death pose ended; body released).
- `DESPAWN delay expired actor=...768 clip=none hidden frame=299` then `DESPAWN complete actor=...768 slot=released frame=299`.
- Second spawn `--spawn-test Swamp_Moth_Minions@...@470`: `SPAWN ok ... actor=...768 ... busy=1/2` (the released slot and
  its stable ID are reused; `Lifecycle final` 768 `state=3`).
- `Rendered frames=520; clean shutdown`, no `Foundation error`.
- Frames `dsp/c200.png` (before expiry) and `dsp/c340.png` (after): the moth is not identifiable in either frame, so the
  visual disappearance is NOT confirmed by eye. Evidence is the log and lifecycle state.

## 5. Gaps (honest)
1. **Despawn clip does not play.** The lifecycle reaches state 2 and the Despawn sequence (Moth_Despawn, non-looping) is
   started, but the combat runtime keeps pose ownership of the dead actor (`owns_pose`), so the sequence is never advanced
   and its completion never fires. Forcing the takeover (interrupting the runtime) cancels the playback ("Retained sequence
   is not active"), so that was reverted. The clip path is kept behind `kDespawnClipPlaybackWired = false` (main.cpp). Until
   fixed the actor is hidden at the delay with no clip (the no-clip path). Fix belongs in the combat session's pose ownership
   for dead actors (runtime death action vs. state sequence).
2. Visual confirmation of the corpse disappearing is inconclusive (frames, see 4c); reference-video corpse behaviour not
   established (1c).
3. Scope: only lifecycle-managed actors are despawned (pool slots, declared, lifecycle spawns). Authored monsters in the
   default path (no lifecycle) keep their corpses. Making every actor lifecycle-managed is a larger change, not done.
4. Respawnable actors (CanRespawn, property 11 > 0): the source sends them to Limbus with a respawn timer (event 47); here they
   are hidden and not respawned (`can_respawn` is never set; no respawn timer bound).
5. Save/restore: the despawn owner is transient but is not cleared on GameSave load/reset (no `despawnOwner.clear()` wired).
   The spawn pool is also not reset on load in this build. Not tested.
6. Timing uses wall-clock dt in these runs (no `--fixed-step`), so the 2000 ms is ~125 frames at 16 ms but not exact per frame.
7. Container summon targets for plant/zombie are refused: their visuals are not in the Swamp package (see 4b). Whether they
   exist in the Android device folder was not checked.
8. Plant/zombie variant summon is refused, not spawned: a successful spawn of those variants is not demonstrated.
9. Summon arc (spawn motion) not verified frame by frame.

## Placeholders
- None (no art or HUD was added). The `--container-script` and `kDespawnClipPlaybackWired` switches are debug/gating only.

## Package files required
- None new. `moth_despawn_01.bdae` and `moth_urn_spawn.bdae` are already in the rc3 assets
  (`original-cache/data/3d/characters/moth/animations/`).

## Verifier script
- Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16despawn -Jobs 6`.
- Isolated: `build-p16despawn/despawn_after_death_v1_tests.exe` (expect `despawn_after_death_v1 OK`).
- Summon: jobs in `.local-inputs/claude-preview16/despawn/{base,atk,final}` (seeds 3/4/5/6/7); expect `SPAWN ok` for seeds 3,
  4, 6 and `summon_calls=0` for 5, 7.
- Despawn: `.local-inputs/claude-preview16/despawn/dsp/d520.args`; expect the `DESPAWN tracked/death end/delay expired/complete`
  lines and a reused `SPAWN ok ... actor=...768` after frame 470.
- Refusals: `.local-inputs/claude-preview16/despawn/var/{plant2,zombie}.args`; expect `SPAWN admission refused` and
  `outcome=refused reason=profile not admitted`.
