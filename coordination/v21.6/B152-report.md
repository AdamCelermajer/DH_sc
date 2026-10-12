# B152: a respawned Bogwomp stays standing when killed again

Branch `v21.6-respawn` (worktree `.claude/worktrees/v216-respawn`), base `488d9e82`. Not pushed. Tracker not edited.

Status in one line: root cause found in shared combat/lifecycle code, fixed generically, unit-tested, and reproduced and verified on the real combat-hit path in hidden-window runs (before/after frames). Reproduced only after a save/load world restore; a fresh session with a respawn did not reproduce it.

## 1. Answers to the three questions

| Question | Answer | Basis |
|---|---|---|
| (a) Any mob after a world restore or revive? | Yes, any respawnable authored monster that is restored dead and hidden (Limbus) and later respawns. The restore path is the F5/F9 world restore, which runs the same `rebind_after_restore` as the Swamp revisit and the death-checkpoint revive. | Runtime (F5 at 600, F9 at 700). Revisit and checkpoint revive: same code path, not run. |
| (b) Patrol-specific? | No. `patrol` does not appear in the port, the IDA export or the level data. The mechanism is in shared combat code (`ActorCombatRuntime`), not in any patrol/AI owner. The user's mob matches the respawning Bogwomp profile `Swamp_LizadMan_Type1_Respawn` (respawn 20000 ms, CharProperty 11 = 5120). | `grep` of the tree; probe scan of population respawn times. |
| (c) Also on a fresh un-restored session? | Not in the runs made here. A fresh session killed the same respawned lizard twice through real hits and the second death played normally (before build). The bug needs the dead-hidden state to be restored. | Runtime, `before-hit-liz05`. |

Honest gap for (b): I could not identify the exact bridge actor the user saw. The reproduction uses `_prim_LizTemplate_05` (profile `Swamp_LizadMan_Type1_Respawn`, the same profile family as the respawning lizards that the M2 run listed in the swamp corner area). The mechanism is generic, so the fix does not depend on the name.

## 2. Evidence

### Visual
- User screenshot `C:\Users\adamc\.t3\userdata\attachments\35094ffd-52b0-4a0f-acde-e02168ca5b33-2d9ff82e-b20e-426a-843b-788febdb92f4.png`: a bright-green lizard standing next to the hero on the boardwalk with a pile of skulls at its feet. The still cannot show which actor it is or its death state. Inference only: it is the standing-corpse symptom.
- Runtime, same inputs, before build vs after build (`.local-inputs/v21.6-respawn/verify/cases/`):
  - `before-restore-liz05-cap/f2010.png`, `f2080.png`: the lizard stays upright beside the hero while the hero keeps swinging (damage numbers "17" still shown).
  - `after-restore-liz05/f2010.png`: the lizard is in its death reaction (arms raised, staggering). `f2080.png`: the corpse has moved to the upper-left in its death pose.
  - Frames 1960, 1975, 1990, 2040 are in the same folders.
- Original video: not checked for a repeat death of a respawning Bogwomp. No timing claim is made.

### Logic (IDA export `libDungeonHunter2.so/pseudocode-all.c`, re-read this session)
- `CSDead::OnEvent` 0x3c4c3c, event 34 (death animation end): `SetPhysicalObject(nullptr)`; if not persistent, `CharTimers::TMR_Start(Despawn_Delay, 46)`; flags328 = 64.
- `CSDead::OnFocus` 0x3c4d50: entering Dead is the state that plays the death pose. Each new death enters Dead again.
- `CSLimbus::OnFocus` 0x3c2e58: flags328 = 0, vtbl+64, respawn timer (event 47) when byte +1328 is set and `GetRespawnDelay > 0`, `AI_ClearAllAggro`.
- `Character::Deserialize` 0x3a6210 (already in the port comments): a saved dead Character goes to Limbus without a new death clip.
- Expected behaviour: a respawned monster is a new live occurrence. Its next death enters Dead, plays the Died pose, releases its body at the animation end, and is hidden after `Despawn_Delay`.

### Port cause
- `actor_combat_runtime.cpp` `synchronize()`, dead branch: starts the Died pose only when `binding.pose != death && !binding.yielded`.
- `yield_pose()` sets `yielded = true`. It is the lifecycle hand-over. Called from `refresh_actor_combat_permissions()` (each frame) when a dead actor that still owns its death pose is not permitted (hidden) and its death is complete, and from the source-program transition.
- `yielded` was cleared by `start_pose()`, `kill()` (script kill only) and `death_event()`, but not when the actor was revived. The live branch of `synchronize()` cleared `restored_dead` ("revival is a fresh live occurrence") but not `yielded`. A hit death (`apply_calculated_hit` -> `handle_applied_hit` -> `synchronize`) therefore never restarted the pose.
- Consequence in `main.cpp` despawn owner: the second death is tracked in phase `dying`, but it waits for `retained_actor_pose(id)->current_ended()`. No pose exists, so the body is never released and the actor stays standing with HP 0.

Sequence in the restore run (`before-restore-liz05b`, `before-restore-liz05-cap`):
1. Hit at frame 237 (real combat hit, not a script kill): Died pose, body released at 286, hidden at 403, respawn scheduled (20000 ms).
2. F5 at 600 saves the hidden, dead lizard.
3. F9 at 700 restores it: `DESPAWN restored ... state=Limbus clip=not-replayed`, terminal death pose held while hidden. The yield on the hidden corpse is inferred from the code path and the final state, not logged.
4. Respawn at 1889 (`DESPAWN respawned`).
5. Hit at 1968: `DESPAWN tracked`, no `DESPAWN death end`. Final actor line: `HP=0/47.5 action=0` (Idle), `Native body final physical=1`. The body stays in the world.

## 3. Expected behaviour and fix
- Expected: every new death of a respawned or restored monster plays Died, releases its body at the animation end, and is hidden after `Despawn_Delay`.
- Fix (generic, no names): a revived actor is a fresh live occurrence, so the lifecycle hand-over belongs to the previous dead occurrence. `ActorCombatRuntime::synchronize()` now clears `yielded` in the live branch, next to the existing `restored_dead` reset.
- Not changed: the lifecycle hand-over itself, the despawn owner, saves, and the permission rules.

## 4. Changes (commit on `v21.6-respawn`)
1. `port/windows-foundation/actor_combat_runtime.cpp`: clear `yielded` on a live occurrence (the fix, 3 lines).
2. `port/windows-foundation/tests/actor_combat_runtime_tests.cpp`: new `yielded_actor_revived_new_death()` (2 variants x 3 death producers: a death that ran to its end, and a restored dead terminal; the producers are script kill with update, a lethal `apply_calculated_hit`, and `death_event`). Registered in `main()`.
3. `port/windows-foundation/main.cpp`, `--kill-test` (debug aid): it used to set HP 0 and the dead action without `source_dead`, so the actor was not dead for the runtime or the despawn owner (no death pose, no despawn). It now calls `CombatSession::script_kill_actor` (`kill_actor_state` + runtime death), the same path as a scripted KillActor. This changes the debug aid's behaviour; the old form did not produce a death.

## 5. Tests
- `actor_combat_runtime_tests` before the fix: FAIL (`Respawned actor's next death did not start its death pose`). After the fix: PASS (`actor combat runtime tests passed`).
- Full CTest, build `R:/.local-inputs/v21.6-respawn/build` (llvm-mingw clang++ 20261006, CMake 3.22.1 + Ninja, Release, `subst R:`): **179/179 passed** (`.local-inputs/v21.6-respawn/ctest.log`, 524 s).

## 6. Runtime-verified vs unit-only

| Item | Status |
|---|---|
| Respawned lizard, second death after an F5/F9 world restore, real combat hit: before build fails (body stays, action Idle, physical), after build plays Died and releases the body | Runtime (hidden window, isolated copy of the package seed) |
| Same respawn, fresh session, second real-hit death | Runtime, passes on the before build (no bug seen) |
| Restored-terminal + hand-over + revive + kill (variant 1 of the unit test) and normal death + hand-over + revive | Unit only (fails before, passes after) |
| Swamp revisit (B146 arrival restore), death-checkpoint revive | Not run. Same `rebind_after_restore` path (inferred) |
| Lifecycle-record actors (`actorLifecycle` records, e.g. Limbus/Idle lifecycle) | Not run |
| Original repeat-death footage | Not checked |

Runs (all in `.local-inputs/v21.6-respawn/verify/cases/`; frozen EXEs in `.local-inputs/v21.6-respawn/frozen/`):
- `before-restore-liz05b`, `before-restore-liz05-cap` (before build, `b152-before-killtest-only.exe`, SHA256 `1f9024dfc96fc13cc1e565ecb0d2dc3de5d74f67632e1f572605fd3235319244`): bug reproduced.
- `after-restore-liz05` (after build, `b152-after.exe`, SHA256 `3cc868b895aa8d30edbc6c2a1306de3472eed32e61610ce8639899b4df6a547f`): fixed.
- `before-hit-liz05` (before build, fresh, no restore): second death normal.
- Command shape: `run-case.ps1 -Position -5780,8880,262 -Extra --target-frame 150 --attack-start-frame 160 --attack-frames 1900 [--save-frame 600 --load-frame 700]`. The seed is the v21.4 `pkg` seed (copies only); the package is `windows-source-clock-v21.5` assets. The shipped v21.5 EXE (SHA256 `185CACAD...`) lacks the `--kill-test` change but has the same combat code as the before build.

## 7. Remaining gaps (honest)
- The exact bridge "patrol" Bogwomp was not identified; the reproduction actor is `_prim_LizTemplate_05`, profile `Swamp_LizadMan_Type1_Respawn`.
- The restore trigger is inferred for revisit and death-checkpoint revive. Not run.
- The yield on the hidden restored corpse is inferred from code and final state; no log line was added.
- Stale `yielded` can still suppress a death in the same frame as a revive (the clear happens in the next live `synchronize`). The reproduction does not hit this window.
- The despawn owner still waits forever when a dead actor never starts a death pose (no timeout). That is the reason the body stays visible, and it is not changed here.
- Lifecycle-record actors (`actorLifecycle`) use the same `ActorCombatRuntime` path; not tested.
- Original footage of a repeat death was not compared.
