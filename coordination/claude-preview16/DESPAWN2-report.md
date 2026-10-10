# DESPAWN2 report (Preview 16, branch p16/despawn2, from p16/integrate 261f2e49)

Status: DONE for items 1, 2, 3, 5; item 4 defined and unit-tested, its in-EXE reload is refused by the existing checkpoint
policy (pre-existing, also on the baseline), so the restore is not EXE-verified. Gaps are listed in section 5.
Commits (not pushed): `efa34ce2` skeleton, `5c8d8532` clip hand-over, `6895f781` default follows the original, `fc8dc17d`
authored/respawn/reset, `dcb879ea` default-path tick, kill trigger, menu test fix.
ctest (build-p16despawn2, toolchain on PATH): all pass except `session_skill_binding` (allowed).

## 1. Investigation

### 1a. Why the Despawn clip never advanced (item 1)
- `ActorCombatRuntime::update` skips every dead actor in the combat session loop (`!a->alive()`), so a lifecycle state
  sequence on a dead actor never advanced.
- The runtime's `synchronize()` re-started the Death pose for any dead binding whose pose was not Death, so an
  interrupt/takeover was re-asserted on the next frame (`owns_pose` stayed true). This is the "combat runtime owns the pose"
  ownership bug. Confirmed by a temporary trace: the dead-actor branch was never reached while the death binding was owned.
- Fix: `ActorCombatRuntime::yield_pose` (takeover that marks the binding `yielded`); `synchronize` does not re-assert a
  yielded death pose; any new runtime pose clears the flag. The combat session's `play_actor_source_sequence` uses it.
  The session loop lets a non-generic lifecycle sequence advance on a dead actor (`lifecycleSequenceOnDead`).

### 1b. Original death -> despawn -> limbus -> respawn (item 2, decoded from pseudocode-all.c + registration-plan.json)
- Dead(12) event 46 -> Despawn(2) (plan row 12). Despawn(2) registers 34 and 64 -> Limbus(0). Limbus(0) registers 47 -> Spawn(1).
- `CSDead::OnEvent` 0x3c4c3c, event 34: `SetPhysicalObject(nullptr)`, then unless `(vtbl+40)` holds:
  `TMR_Start(CharacterDesign.Despawn_Delay, 46)`, flags328 = 64. `CSDead::OnFocus` 0x3c4d50 starts the same timer when the
  death animation is not pending. Despawn_Delay = 2000 ms (`design_pycst.bin`).
- `CSDespawn::OnFocus` 0x3c32fc: flags328 = 512 and **`SM_SetAnim(-1)`**. No state selects the AnimTable "Despawn" slot: the
  only `SM_SetAnim` callers with a table slot are Idle (+104) and Reviving (+108); every other state uses -1. So the original
  does NOT select the Despawn clip.
- `CSDespawn::OnBlur` 0x3c3794: +1328 = 1 (respawn flag); unless `CanRespawn` (or offline/vtbl+40), clears the local player's
  target; `ObjectBase::Delete` for `+5348 || IsSummoned` (CharType 5).
- `CSLimbus::OnBlur` 0x3c2be4: vtbl+64(actor,1), SetPosition/SetRotation to the initial anchor, Revive. `CSLimbus::OnFocus`
  0x3c2e58: flags328 = 0, vtbl+64, respawn timer 47 when +1328 and `GetRespawnDelay > 0`, AI_ClearAllAggro.
- `GetRespawnDelay` 0x3a4bac = `1000 * (CharProperty 11 >> 8)` (0 when the Character flag at +5249 is set). `CanRespawn`
  0x3a5248 needs property 11 > 0 and `GroupInfo::CanRespawn` 0x3d2a34 (group state; not modelled: no group table in this port).
- Fade: `VisualObject::StartFadeOut/UpdateFadeOut` are empty. Hide = vtbl+64 (Limbus) = population enabled off.
- XP/loot: the port grants them from the death event (`features/loot`); not changed.

### 1c. Reference corpse lifetime (item 5), `.local-inputs/reference-video/dh2-act1/...Part 1...mp4` (640x360, 30 fps)
- Contact sheets 420-436 s at 1 fps, 424-434 s at 2 fps, crops at 2 fps and 5 fps (`claude-preview16/despawn2/ref/`).
- Bog Moth dying around 423.5-424.5 s (XP popup "+4 EXP" 424.5-425.5 s). Body lying from ~425.0 s, still present at 426.3 s,
  gone by 426.5 s (5 fps crop, the camera pans so the body was tracked against ground markers). "8 GOLD" text appears ~426.5 s.
- Kill-to-vanish about 2.2-3.0 s (kill onset +/-0.5 s). No clear sinking or fade of the body is visible before it disappears.
- Port (default path, fixed step, summoned moth): death end at frame 214 (1.0 s death animation), hidden at frame 333
  (Despawn_Delay 2.0 s): kill-to-hidden 3.0 s. The lower end of the reference range is consistent with the 2 s delay; the
  reference does not show a visible Despawn clip, which agrees with 1b.

## 2. Implementation

### Item 1: clip path (opt-in) and runtime hand-over
- `features/despawn`: entering Despawn (`play_clip`) always happens at Despawn_Delay; with a clip the actor waits for its
  completion (poll), without one it goes to Limbus at once (hide), like the source.
- `actor_combat_runtime.{hpp,cpp}`: `yield_pose`, `Binding::yielded`, `synchronize` guard, cleared in `start_pose`.
- `combat_session.cpp`: takeover uses `yield_pose`; dead lifecycle sequences advance (non-generic only).
- `main.cpp`: `--despawn-clip` (default off) plays the actor's Despawn sequence; default = original (no clip).
- Clock: `despawnCarryMs` carries the fraction of each frame, so 1/60 s frames count exactly (previously rounded to 17 ms).

### Item 2: authored monsters on the default path
- `despawnTick` runs in every run (it was gated on the lifecycle flag, so the default path was never covered).
- Every dead, enabled, non-pool population actor is tracked. Lifecycle actors use `OriginalActorLifecycle` as before.
  Authored monsters without a lifecycle record use the same owner with population effects: body release (native body
  remove_physical when `--original-native-bodies`), hide (`population.set_enabled(false)`), physical flags off.
- `--kill-test NAME@FRAME` (debug, default off): sets an authored actor to 0 HP; the runtime takes the death pose.

### Item 3: respawn (event 47)
- Respawn delay per actor = `1000 * (resolved[11] >> 8)` (`OriginalCombatProperties::sheets`). Summoned actors never respawn.
- Owner phase `awaiting_respawn`: after Limbus, a respawnable non-summoned actor waits the delay, then `respawn` revives it at
  its anchor (population shown, health reset, idle, physical back on).
- Authored anchor: `despawnHome` is captured at the first tick the actor is alive (before any death).

### Item 4: transient state and restore rule
- Owner, records, carry, anchors, lifecycle records, lifecycle physical/collision/flags/idle maps are transient (never in the
  save). Reload/restore (`R` and `--reload-frame`) now clears them, and `SpawnPoolV1::free_all()` re-declares every pool slot
  free (stable IDs kept), as the pool header states.
- Corpses after a restore: the rebuilt world's state (no timers survive). A dead actor is not re-tracked until it is alive and
  dies again.

## 3. Isolated tests
- `actor_combat_runtime_tests`: `yielded_death_pose_not_reasserted` (death pose taken, hand-over releases it, no re-assert over
  3 updates, unbound yield is a no-op).
- `combat_session_motion_phase_tests`: dead-actor lifecycle sequence (player fixture) completes exactly once after the hand-over.
- `despawn_after_death_v1_tests`: no-clip ordering (enter Despawn, hide, release slot), non-summoned no-clip, respawn
  scheduling (fires exactly at 5000 ms, once), summoned never respawns, clip path schedules respawn after completion.
- `spawn_character_v1_tests`: `free_all` frees busy/failed slots and keeps the stable IDs.
- `character_menu_tests`: see section 5 item 8.
- ctest: all pass except `session_skill_binding` (allowed).

## 4. Integrated runtime verification (EXE `build-p16despawn2`, `--fixed-step` 1/60, quiet hidden runs)
Scratch: `.local-inputs/claude-preview16/despawn2/` (`base.args` is the d520 scenario; `mk.sh`/`run.sh`).
- **Summoned moth, default (`full`)**: `DESPAWN tracked ... clip=none respawn_ms=0 frame=152`; `death end ... frame=214`;
  `delay expired ... state=Despawn clip=none hidden frame=333`; `complete ... slot=released frame=333`; the reuse
  `SPAWN ok ... actor=...768` at frame 470. Clean shutdown.
- **Summoned moth, `--despawn-clip` (`fullclip`)**: `delay expired ... state=Despawn frame=333`; `Lifecycle whole sequence
  finished ... state=0` and `DESPAWN complete ... state=Limbus slot=released frame=455` (122 frames = Moth_Despawn length).
- **Pixel evidence** (`v300/v340/v400/v430/v452/v462.ppm`, no re-spawn): the corpse is present at 300 and 400, moves during
  the clip (340, 430), and at 462 (after hide) the crop is pixel-identical to a no-summon control (`ctl462.ppm`) apart from the
  player and gold text (`diff462.png`). The object seen there in both runs is static scenery.
- **Authored moths, default (`kill`)**: `_prim_MothTemplate_02` and `_01` killed at frame 60: tracked, `death end` at 121,
  `hidden` at 240; `Population final enabled=31` (33 before, the two hidden).
- **Authored lizards, long run (`kill4`, 1460 frames)**: `_prim_LizTemplate_09` and `_10` have respawn 20 s (property 11).
  `respawn scheduled ... after_ms=20000 frame=228`, `respawned ... frame=1428` (= 228 + 1200). The respawned actor is at
  `HP=47.5/47.5 action=0`, population enabled 21 (19 + 2). Its position is -11979.1,15765.3 against authored -11984.2,15747.7
  (19 units off: the anchor is captured at the first tick, after the first frames' settle). Not pixel-checked.
- **Regression A/B (default path, `ab/`)**: wave-2 jobs smoke-R, b004-R, kxp-130-R, eq-bag-R, base (`baseline-dh.exe`, the
  pre-change EXE) vs new, same assets. b004-R and kxp-130-R identical md5. smoke-R and eq-bag-R differ, and the baseline run
  against itself also differs for smoke-R (`base-smoke-R` vs `base2-smoke-R`): these jobs are wall-clock nondeterministic
  (no `--fixed-step`), as the wave-2 report recorded. No despawn was tracked in any of the four jobs.
- **Reload**: `--reload-frame` is refused in the standard configuration (`Reload checkpoint rejected ...: Campaign
  lifecycle/controller providers are not persisted`). The baseline is refused the same way. Removing the controller policy
  stops the run for other reasons (movement/body plan requirements). So the reset is code-reviewed, not EXE-exercised.

## 5. Gaps (honest)
1. **Despawn clip is not on by default.** The decoded original selects no Despawn clip (CSDespawn `SM_SetAnim(-1)`), and the
   reference corpse vanishes without a visible despawn animation. The clip path works (section 4) and is opt-in. Decision for
   the user: keep the clip off (matches the source and the reference) or enable it (`--despawn-clip`).
2. **Authored monsters have no lifecycle record on the default path.** A full `OriginalActorLifecycle` record would make
   `lifecycleRegistered` true, and the campaign checkpoint policy refuses any checkpoint with lifecycle providers (`combat_session.cpp`
   line ~2196). So authored monsters use the same owner with population effects; Limbus/state-2 side effects (OnBlur flags,
   +1328) are implemented as owner decisions, not as state callbacks.
3. **Respawn is not the source's Spawn sequence.** The source plays Spawn (Limbus -> Spawn -> Idle) after event 47; this port
   revives and shows the actor at once. The CanRespawn group permission (`GroupInfo`) is not modelled (every respawnable actor
   respawns). Respawn position is 19 units off the authored placement (section 4).
4. **Save/load restore not EXE-verified.** The reset code is in place; reload is refused in the standard configuration (also on
   the baseline). The restore rule is unit-tested only for the pool (`free_all`) and the owner (`clear`, existing test).
5. **Reference corpse timing precision is about +/-0.5 s** (640x360 source; kill onset is not sharp). The 3.0 s port value
   (1.0 s death animation + 2.0 s) is inside the reference's 2.2-3.0 s range but not confirmed exactly.
6. **Loot/XP timing:** the port grants XP and loot at the death event (matches the reference XP popup). The reference's "8 GOLD"
   text appears ~2 s after the kill, near the vanish time. Not established whether the original drops loot at despawn; not changed.
7. **Authored visual hide not pixel-verified** (the authored moths are far from the camera). The population flag and the render
   gate (`if(!actor.enabled) continue`) are the evidence.
8. **character_menu test (pre-existing failure on p16/integrate):** the MAPFIX nearest-centre rule uses the contour centroid
   (Quest 395.0, Map 452.65 in sheet units), so the overlap midpoint is 423.8. The assertion sampled x=424 for Quest, which is
   0.35 nearer the Map centre. Corrected the Quest sample to x=420 (Map sample 430 is unchanged). The hit logic is not changed.

## Placeholders
- None (no art or HUD added). Debug triggers only: `--kill-test NAME@FRAME`, `--despawn-clip` (opt-in, not a placeholder).

## Package files required
- None new. Runs use `.local-inputs/windows-source-clock-v19-preview-15-rc3/assets` and the `despawn/assets-run` copy.

## Verifier script
- Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16despawn2 -Test -Jobs 6`.
- Owner tests: `build-p16despawn2/despawn_after_death_v1_tests.exe` (expect `despawn_after_death_v1 OK`).
- Default summon: `.local-inputs/claude-preview16/despawn2/full.json` (expect `clip=none`, `hidden frame=333`, `complete ... slot=released`).
- Clip: `fullclip.json` (expect `clip=Despawn`, `state=Despawn frame=333`, `complete ... state=Limbus ... frame=455`).
- Authored kill: `kill.json` (expect `DESPAWN tracked ... lifecycle=0` for `_prim_MothTemplate_02`, `hidden ... frame=240`).
- Respawn: `kill4.json` (1460 frames; expect `respawned ... frame=1428`).
- Visual corpse: `v300..v462.ppm` vs `ctl462.ppm`; diff with `ffmpeg -i v462.ppm -i ctl462.ppm -filter_complex blend=all_mode=difference`.
