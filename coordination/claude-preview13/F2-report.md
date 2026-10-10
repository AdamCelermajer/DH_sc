# Preview 13 wave 2 (F2): B035 push / knockback port into the repo

Status: **B035 push logic is ported and its focused tests pass in the repo.** The push is **not yet wired into the normal EXE** (main.cpp has no binder call), so the EXE verifier script does not exist yet. Tracker row B035 stays open. Repo HEAD at start: `2bba336c`. No commit made.

## 1. Evidence

### Logic (proposal, verified against HEAD)
- The private proposal `.local-inputs/b035-state10-proposal/` (full copies, CRLF) differs from repo HEAD only by the intended state10 changes, plus the hit-effect observer in `ActorCombatRuntime`. I generated per-file diffs HEAD -> proposal (LF-normalised) and applied them with `patch` (dry run clean, no fuzz). Shared files were not overwritten, so the B013 worker's edits are not clobbered.
- Melee path: the observer hook sits in `ActorCombatRuntime::handle_applied_hit` (`actor_combat_runtime.cpp`), which both the calculated melee path (`apply_calculated_hit`, line ~197) and the marker path (`consume_marker`, line ~328) call. So the normal melee marker and explicit `apply_source_result` hits go through the same before/after-effects gate.
- Visual (reference video): not re-examined in this wave. The F-report 377 s example is a tutorial overlay, not a hit. Push distance/speed from video remains unmeasured.

### Calculation risk (from G-report probe, not re-run here)
- G's histogram (`b013_attack_continuity_v1_tests.exe <assets> histogram`, seeds 1-5000, lizard vs authored Knight) gives 0x90 in 3741 and 0x98 in 910 of 5000 seeds, i.e. about **93% of positive lizard hits carry the push bit 0x80**. If the push-bit derivation is wrong, wiring the push into the EXE would make lizards push on most hits, which looks like the "every hit pushes" bug the user reported. **Check this before wiring main.**

## 2. Expected behaviour (preserved by the tests)
- Push-bearing result (0x98 Lizard, 0x88 controlled) on a Player in Idle (state 3): Injury 11 first, then KnockedBack state 10 (GreatKnockedBack when mask 0x100000), then back to Idle 3 at sequence end. Movement follows the authored MoveGO path through the native floor/obstacle bodies.
- Clean hit (0x0) and critical hit (0x8): no Push, no KnockedBack, hero position unchanged, RNG untouched.
- Duplicate delivery: no second push, no RNG replay.

## 3. Changes (all in `port/windows-foundation/`, LF line endings kept)
Applied proposal hunks (per-file `patch`, not full-file copies):
- `actor_combat_runtime.hpp/.cpp`: `CombatRuntimeHitEffectStage`, `CombatRuntimeHitEffectObserver`, `set_hit_effect_observer`, `observe_hit_effect`, occurrence counter, before/after calls in `handle_applied_hit`, failure latch cleared in reset. **My adjustment:** the proposal's early `if(!result.applied)return true;` was removed and the observer is gated on `result.applied` instead, so non-applied results follow the HEAD flow exactly when no observer is bound (HEAD's `start_injure_reaction` runs for them too). All observer and latch lines are gated on `result.applied&&hit_effect_observer_`.
- `actor_state.cpp`, `character_state.hpp`: `CharacterAction::knocked_back` accepted in the validity switch and added to the enum (the only switches on it have `default:`; checked).
- `combat_session.hpp/.cpp`: `CombatSessionSourceSequencePolicy` gains `source_other_actor`, `source_knockback_great`, `source_direct_transition`; `bind_source_hit_effect_handler`; `actor_transition(...)` takes the incoming policy; state 10 handling in source-sequence begin / cancel / finish. **My addition:** `facts()` maps `CharacterAction::knocked_back` to `original_state=10` (the proposal left it to the `default:` branch, which logged "Unresolved ... producer" and kept the stale state).
- `features/combat/b035_session_push_effect_v1.hpp/.cpp`: `bind_session_push_effect_v1` (declared and defined; the undefined symbol from the F-report is resolved), AiProps IsBoss gate in the push sink, policy with `original_state=10`.
- `features/physics/session_actor_transition_v1.hpp/.cpp`: `knockback_read_gate528`, `knockback_write_gate528`, `knockback_controller_lock`, `knockback_look_at_cancel_sneaking` owners (the physics-header declarations the fixture needs), state 10 cases in Blur/Focus/suffix.

Test edits (mine):
- `features/combat/b035_state10_native_tests.cpp`: hero position captured before `apply_source_result`; non-push branch asserts position unchanged and no KnockedBack (+7 lines); new case `clean-hit-no-push` (0x0, seed 2 found by the existing search).
- `features/combat/b035_session_push_effect_v1_tests.cpp`: one expectation changed from `CharacterAction::idle` to `knocked_back` and `original_actor_state==10` (the old test predated the state10 decision; the new state is the intended one).

Not changed: `main.cpp`, `CMakeLists.txt`, `docs/*`, `tests/combat_session_combo_tests.cpp`. (`main.cpp` shows a diff from another worker; not mine.)

### Package files required
The state10 path loads only files the Preview 12 package already has. The runners load them successfully from `.local-inputs/windows-source-clock-v19-preview-12/assets` (presence confirmed by successful load; SHA256 not computed): `original-cache/data/pydata/*` (animations, loot, character properties), `data/animations_*`, `actor-profiles-v2.xml`, `original-melee-bindings.xml`, `original-cache/data/3d/modules/swamp/swamp.bdae`. No new asset is needed.

## 4. Tests (commands and real results)
Runner used for state10 and session push: `port/windows-foundation/features/combat/run_b035_state10_native_tests.ps1 -AssetRoot <preview-12 assets> -BuildRoot .local-inputs/claude-preview13/F2/b035-build`
- `run_b035_state10_native_tests.ps1`: **EXIT 0, PASS** ("PASS B035 exact source-result controls: ordinary 0x8 no-op; natural 0x98 and controlled 0x88 dispatch once ..."). Cases: normal melee 0x98, failed post-hit effect, ordinary 0x8, clean 0x0, natural 0x98, controlled 0x88, great, direct, interrupted, lethal. Push cases report `moved-scene-units=114.104` (natural), `112.677` (controlled), floor-z 255->255, `sink-calls=1 consumed=1`; non-push cases `sink-calls=0 consumed=0 hero-position-unchanged=1`.
- `run_b035_session_push_effect_v1_tests.ps1`: **EXIT 0, PASS** (after the one expectation change above).
- `features/physics/session_actor_transition_v1_tests.cpp` and `tests/combat_session_actor_transition_tests.cpp` (standalone, changed sources compiled from source, `-Wno-unused-value -Dfinite=_finite`, preview-12 assets): **both exit 0, PASS**.
- `features/combo_chain/combat_session_combo_tests.cpp` (the CMake target `combat_session_combo`; the brief's `tests/` path is this file), standalone with patched `combat_session.cpp` + `actor_combat_runtime.cpp` from source: **exit 0, PASS** (live held three swings, release, blocked; save continuation resets).
- `main.cpp` syntax check with the real ninja compile line (`-fsyntax-only`, no `-o`, from `ninja -t commands dh-foundation.exe`): **exit 0**. Main was not edited; this checks the changed headers.

Standalone combat/generic_skills runners (batch log `F2/runners-all.log`; individual logs `F2/fail2-*.log`, `F2/head-*.log`):

| runner | my tree | HEAD baseline | note |
|---|---|---|---|
| run_b013_attack_continuity_v1_tests | PASS | - | |
| run_b035_session_push_effect_v1_tests | PASS | - | |
| run_b035_state10_native_tests | PASS | - | |
| run_auto_target_marker_v1_tests | PASS | - | |
| run_object_of_interest_owner_v1_tests | PASS | - | |
| run_target_retention_regression_tests | PASS | - | |
| run_runtime_player_profile_attack_bank_v1_tests | PASS | - | |
| run_runtime_player_incoming_animation_bank_v1_tests | PASS | - | needs `-BuildRoot` with the six `.a` copied |
| run_runtime_skill_activation_v1_tests, _mana_, _progression_, _skills_text_, run_runtime_tests, run_tests, run_pc_gameplay_hud, run_pc_skill_input_binding, run_runtime_skill_animation_bank_rank0 | PASS/EXIT 0 | - | |
| run_pc_skill_hud_projection_v1_tests | PASS | - | takes no `-AssetRoot` |
| run_b033_targetless_attack_admission_v1 | FAIL | **FAIL** | pre-existing at HEAD |
| run_b037_attack_skill_admission_v1 | FAIL | **FAIL** | pre-existing at HEAD |
| run_post_skill_attack_regression_v1 | FAIL | **FAIL** | pre-existing at HEAD |
| run_runtime_skill_activation_session_v1_tests | FAIL | **FAIL** | pre-existing at HEAD |
| run_b012_death_mid_swing_v1_tests | segfault (-1073741819) | PASS | stale library, see 5 |
| run_target_facing_regression_tests | segfault | PASS | stale library, see 5 |
| run_runtime_player_combo_chain_v1_tests | segfault | PASS (G) | stale library, see 5 |

The four HEAD-failing runners are not caused by this change: they fail the same way on a `git archive HEAD` copy (exit 1). Their logs were not diagnosed further.

## 5. Stale library (important for the root)
`.local-inputs/windows-foundation-build/libfoundation_data.a` contains a HEAD-built `ActorCombatRuntime`. The runners for b012, target_facing and combo_chain link this archive, while `combat_session.cpp` (now changed, new `ActorCombatRuntime` members) is compiled from source. The class layout differs, so they segfault. Compiling `actor_combat_runtime.cpp` into those runners makes them pass (b012: `PASS lethal mid-swing ...`; target facing: both PASS; combo chain: PASS). I did not rebuild the shared library. **The root's integrated ninja build fixes this**; the standalone runners need either a rebuilt archive or `actor_combat_runtime.cpp` added to their source lists.

## 6. Not done
- **Main wiring (task 3): not done.** The EXE does not call `bind_session_push_effect_v1`, so no push happens in the normal EXE. The owners needed in `main.cpp` (the `make_session_actor_transition_consumer_v1` config near lines 2261-2280) are: `knockback_read_gate528`/`write` (can use `contextFor(id).gate528`), `knockback_controller_lock` (main has no controller-lock owner; `idleSuppressed` is the nearest analogue and needs a decision), and `knockback_look_at_cancel_sneaking` (LookAt attacker plus CancelSneaking; main has a `cancel_sneaking` diagnostic stub near line 1540). A player push bank must also be built (the test builds it at lines 25-73 from the same tables main already loads). This is more than a surgical hunk; it belongs in a feature file (e.g. `features/combat/b035_push_runtime_v1.cpp`) called from a 3-5 line main hunk. Until then the EXE behaviour is unchanged, because the observer is only set when the binder is called.
- **CMake (task 1 item): not done.** No ctest target exists for either B035 runner, and the state10 runner compiles about 20 sources outside any library. I did not add a target; the root can decide.
- **Verifier script (task 5): not available.** No EXE argument line can produce a Lizard push on the player yet, because the binder is not wired. Do not expect a push from the current candidate.
- Video evidence for push distance/speed is not measured.

## 7. Uncertainties and risks
- Push-bit frequency (see Evidence, about 93%). Highest risk for the user's "every hit pushes" complaint.
- `features/combat/b035-push-hit-effect-consumer-v1.md` is still 0 bytes.
- The melee-marker clean/critical control is not isolated: the marker path does not reproduce the search's outcome for the same seed (seed 2, searched as 0x0, gave 0x90 on the marker path). I removed that marker control rather than guess a seed. The same hook is covered through the explicit `apply_source_result` path. The calc owner should look at this marker/search mismatch.
