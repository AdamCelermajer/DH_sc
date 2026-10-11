# LIFECYCLE report (p16/lifecycle): admitted physical transitions for lifecycle states under AI

Status: PARTIAL. The blocker is removed: PreSpawn17 -> Spawn1 -> Idle3 now run under `--enemy-ai` for the pool lizard and moth,
and the native bodies are created and imported each frame. NOT done: engagement of the spawned lizard (not seen),
death/despawn, the authored LizardMan_Intro replay under AI, the derived moth profile (Swamp_Moth_Minions), and the
regression batches beyond ctest.

## 1. Evidence

### Visual (reference `.local-inputs/claude-preview16/spawn/video/intro-sheet-224-228.png`, v1.0.3)
- Observed in the contact sheet: two green lizards enter over the top wall edge (frames 1-3), one arcs onto the floor
  (frames 2-5), both stand near the centre (frames 6-9); HUD and "Combat Tutorial" appear around frames 10-11.
- Inference, not observed: the arc is `lizardman_spawn_jump`. 4 fps cannot give timing.

### Logic (IDA `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
- CSPreSpawn::OnFocus (0x3c688c): flags328 = 0x1300 (4864); ANIM_Set to the PreSpawn/Limbus anim; SetPhysicalObject(null);
  DisableCollisions. Animation speed 0 when the anim row is -1.
- CSPreSpawn::OnBlur (0x3c67d4): Revive; EnableCollisions.
- CSSpawn::OnFocus (address not read; pseudocode line 141534): flags328 = 577 (0x241); keeps bit 0x2000 when coming from 17;
  ANIM_Set spawn clip; AI_SetTarget(null); CancelSneaking; StartFadeIn on the visual.
- CSSpawn::OnBlur (line 141352): if !(flags328 & 0x2000) -> Character::InitPhysicalObject.
- CSIdle::OnFocus (line 141365): flags328 = 9088 (0x2380) unless byte 1336; ANIM_Set idle.
- None of PreSpawn, Spawn or Idle pins the body (no pin calls in these handlers).

### Logic (port)
- `OriginalActorLifecycle::change` (original_actor_lifecycle.cpp) runs the blur-side native effects (init_physical for
  previous 1, set_enabled/revive/collisions for previous 17) before the state publish, and the focus-side ones
  (set_flags, select_state_animation, remove_physical, set_collisions) after it. This matches the IDA order above.

## 2. Expected behaviour and the implementation

Expected: PreSpawn17 (hidden, no body) -> Spawn1 (spawn clip, body created when leaving Spawn) -> Idle3 (combat enabled),
with the same observable order as the source. The consumer must admit these as ordinary Blur -> publish -> Focus receipts,
not refuse them. The native body effects stay in the lifecycle services, which already run them in source order, so
each effect is applied once.

Changes (branch `p16/lifecycle`, commit 22c5fb80 and this report):
- `combat_session.hpp`
  - `CombatSessionSourceSequencePolicy::lifecycle_to_state` (default -1; 1, 3 or 17 for legacy lifecycle programs).
  - `select_actor_state_leaf(..., std::int32_t lifecycle_to_state=-1)` and `play_actor_state_sequence(..., lifecycle_to_state=-1)`.
- `combat_session.cpp`
  - `select_actor_state_leaf`: with a bound handler and a target, it runs `actor_transition` before the selection and
    the after_change stage after success. Without a target it still refuses (old message kept).
  - `play_actor_source_sequence` legacy branch: admitted when `lifecycle_to_state` is 1, 3 or 17; receipt to_state is that target.
  - `live_original_state`: with a bound handler the published World fact is canonical (before the lifecycle overlay).
  - `facts()`: under a handler the lifecycle overlay does not overwrite the published state.
  - `set_actor_original_state`: under a handler it publishes the state (idempotent with the transition).
  - `set_motion_phase_handler`: no longer refuses while samples are queued (see 4).
- `features/physics/session_actor_transition_v1.cpp`: blur/focus/suffix cases for 1 and 17. Focus 17 writes flags 0x1300,
  Focus 1 writes 0x241, the others validate and return. Each case has a source comment.
- `main.cpp` (lifecycle `select_state_animation`): passes `request.state` to the three calls.

Not changed: death 12 and Limbus 0. `put_limbus` (state 0) still has no transition receipt (no select call), so the
consumer's 0 recipe is not exercised.

## 3. Tests (real output)

- `p14_build.ps1 -Name p16lifecycle` exit 0.
- `p14_build.ps1 -Name p16lifecycle -Test` (ctest): 114 of 115 pass. The only failure is `session_skill_binding`
  (the known junction issue in worktrees).
- Quiet spawn run, 240 frames, AI on (`r1.args`, job `s1`): exit 0, "Rendered frames=240; clean shutdown", no
  "Foundation error".
  - `Lifecycle state actor=...768 previous=17 state=1 enabled=1`, then `SPAWN ok template=Swamp_LizadMan_Type1 ... clip=source_spawn_state`.
  - `Lifecycle state actor=...770 previous=17 state=1`, `SPAWN ok template=Swamp_Moth_Type1`.
  - Final: both reached `state=3 enabled=1 physical=1 collisions=1` by frame 240.
  - `Source physics import actor=...` lines for the spawned lizard and moth (21 lines) show their bodies moving each frame.
- Before this change the same run gave "SPAWN failed-state ... Legacy lifecycle source program has no admitted physical transition recipe".

## 4. Core change to review (combat_session.cpp, `set_motion_phase_handler`)

The per-frame rebind in main.cpp refused while `pendingMotion` was non-empty. A spawn's `begin`/`select` runs between frames
and queues root samples outside an update, so the frame's rebind failed with "Motion phase binding requires idle current
Session, handler and no pending samples". The guard was dropped; `updating`, `transitionDelivering` and the restore states
still block it. The motion-phase tests (`combat_session_motion_phase_tests`) and reconstructible tests pass under ctest.

## 5. Observations (quiet frames, not yet a fight)

- Batch `fbatch` (frames 42, 44, 46, 48, 50, 56, 64, 80, 120, 200), contact sheet `lifecycle/sheet-spawn.png`
  (frames 42 to 80, 2x down-scale), looked at. The authored Bogwomp (the lizard the player fights) is at the top, with the
  HUD. The spawned lizard stands at the bottom left from about frame 42, and a second one appears near frame 80 at the
  right. I did not see either arc or walk toward the player. The arc is not visible at this sampling.
- Spawned lizard at -6700 (closer, run `e1`, 120 frames): still `state=1` at frame 120 (Spawn clip not finished), so it
  takes longer than 80 frames to finish Spawn. In the 240-frame run it reached state 3 by frame 240.
- No "Source target" or enemy-AI engagement line appears for the spawned lizards. Engagement is gated by the enemy
  controller's view radius and the sight check (`features/enemy_ai/runtime_enemy_controller_v1.cpp`, ~line 494). I did
  not establish whether the lizards are out of radius or the search rejects them. That is open.
- `--combat-ai-gate ... =Limbus` only feeds the legacy `--combat-auto` diagnostic AI (combat_session.cpp ~1390); it does not gate `--enemy-ai`.

## 6. Not done (work for the next owner)

1. Engagement: confirm why the spawned lizard does not aggro. Check `inside_view` radii, `eligible_target` and
   `can_see` for a state 3 pool actor; then capture frames to see an attack and a hit. Do not assume the radius is the cause.
2. Death and despawn: no death/despawn path verified. Death 12 goes through combat; Limbus 0 has no receipt.
3. Authored `LizardMan_Intro` replay under AI (`--source-command`): not run.
4. Moth profile derivation for `Swamp_Moth_Minions` (CharacterTable row 370, AnimTable 44, no actor profile): not done.
   The spawn job still prints "profile not admitted".
5. Regression batches (smoke, B037, combat quiet batches): not run. Only ctest.
6. Frame captures of the spawn animation compared with the reference sheet, at the frame-by-frame level: only the
   contact sheet above was looked at. The arc is not confirmed.

## 7. Verifier script

Scratch folder: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview16/lifecycle/`.
- `r1.args` is the spawn-test args (AI on, 240 frames). Build the job and run it:
  `./mkjob.sh s1 r1.args 240 && ./run.sh s1` (quiet, hidden, silent).
- Expected in `s1.log`: `Lifecycle state ... previous=17 state=1` (x2), `SPAWN ok template=Swamp_LizadMan_Type1`,
  `SPAWN ok template=Swamp_Moth_Type1`, `SPAWN rejected template=Swamp_Moth_Minions ... profile not admitted`,
  `Source physics import` lines, `Rendered frames=240; clean shutdown`, and no "Foundation error".
- `Lifecycle final` for the two pool lizard/moth actors: `state=3 enabled=1 physical=1 collisions=1`.
- Frame captures: `./mkjob.sh f50 r1.args 50` then run; convert with `ppm2png.py`; compare with the reference sheet.
- Known to fail until item 4 of section 6 is done: the spawn of `Swamp_Moth_Minions`.

## 8. Open risks

- A rejected (failed) lifecycle selection after the Blur leaves a pending receipt. The next transition is then refused.
  This is the same behaviour as the generic path and is not new, but it is not re-tested for lifecycle.
- `live_original_state` and `facts()` now prefer the published fact under a handler. Generic (skill/knockback) paths
  go through the same code. The ctests pass, but a quiet regression of combat batches is still needed.
- The spawn clip duration is longer than 80 frames here. The 240-frame run is the reference for "becomes Idle".
