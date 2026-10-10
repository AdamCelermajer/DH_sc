# Preview 13 Group F report: B035 incoming push (knockback) integration

Status: **investigation complete, integration NOT done.** No repo source, test or tracker file was edited.
The blocker is larger than a surgical wiring edit (see "Findings"). The root and the B035 owner
(`/root/b035_state10_runtime_sol`) need to decide the port order before a main.cpp hunk makes sense.

Repo HEAD at start: `f6b7f134`. Working tree had only tracker/report modifications from others; no code
files were dirty.

## 1. Evidence

### Visual (reference video)
- Video: `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`.
- Extracted 2 fps contact sheet from 376 s for 8 s (`.local-inputs/claude-preview13/F/sheet_376_384.png`).
- **Observed:** 376-384 s is a Level-Up tutorial overlay ("When you gain a level..." / "To assign the points...").
  The hero stands still on the swamp boardwalk; no enemy hit, no push, no reaction clip is visible.
- **Conclusion:** the brief's "incoming-hit example at ~377 s" does not match this v1.0.3 video at that time.
  I did not find the knockback example. This is not verified. Next step: locate a Lizard hit by
  scanning for the health bar drop / red damage numbers, or ask the user for a timestamp.
- No frames of a push were examined, so no claim about push distance or speed is made from video.

### Logic (source/tracker)
- Tracker row B035 (`docs/BUGS-AND-IMPLEMENTATION.md`, line 70): actual Lizard 0x98 emits one Push request;
  CharAnimTable row 48 field 8 / field 16 hold the two-clip MoveGO sequences; main state10 transition,
  collision/motion owner and OnFocus/OnBlur effects remain unbound.
- `features/combat/b035-push-hit-effect-consumer-v1.md` is **0 bytes** in the tree (empty feature report).
- In-repo feature (`features/combat/b035_session_push_effect_v1.{hpp,cpp}`):
  - `capture_session_push_admission_v1` captures the pre-hit state gate (target Player and Idle state 3).
  - `consume_session_push_result_v1` consumes an applied result with outcome bit 0x80, once, and fails closed on
    missing outcome/mask bits or a missing sink.
  - `build_session_push_animation_bank_v1` builds the two roots (`KnockedBack` field 16, `GreatKnockedBack` field 8)
    from the explicit CharAnimTable row, stance and stanced mask.
  - `play_session_push_animation_v1` starts the retained source sequence on the same Session.
- Caller path in the normal EXE: `CombatSession::apply_source_result` (`combat_session.cpp:1576`) is called only from
  `runtime_session_projectile_v1.cpp:430`, `celest_source_use_v1.cpp:198`, `hotty_character_cast_v1.cpp:138` and
  `runtime_skill_cast_coordinator_v1.cpp:1142`. Melee damage from a Lizard runs inside the Session marker path
  (`ActorCombatRuntime::apply_calculated_hit`, `actor_combat_runtime.cpp:176`), which has no push observer hook.

### Findings that change the plan
1. **The in-repo state10 focused test cannot compile.** `features/combat/b035_state10_native_tests.cpp:174` and `:212`
   call `bind_session_push_effect_v1`, which is defined nowhere in the repo (grep over `port/windows-foundation`).
   `run_b035_state10_native_tests.ps1` would fail at compile time at HEAD.
2. **The binder needs a hit-effect observer hook that is not in the repo.** The private binder
   (`.local-inputs/b035-state10-private-source/.../b035_session_push_effect_v1.cpp:13`) calls
   `session.bind_source_hit_effect_handler(...)` with `CombatRuntimeHitEffectStage::before_effects` and the
   after-effects stage. Neither symbol exists in the repo (grep: zero hits).
3. **The full state10 implementation lives only in a private proposal.**
   - `.local-inputs/b035-state10-proposal/state10-proposed.diff` (299 lines) touches `actor_state.cpp`,
     `character_state.hpp` (adds `CharacterAction::knocked_back`), `combat_session.cpp/.hpp`,
     `session_actor_transition_v1.cpp/.hpp` (gate528, controller lock, LookAt/CancelSneaking owners).
   - Full copies of `actor_combat_runtime.cpp/.hpp`, `combat_session.cpp/.hpp`, `session_actor_transition_v1.cpp/.hpp`,
     `b035_session_push_effect_v1.cpp/.hpp` are in the same folder. `actor_combat_runtime` and the hit-effect hook
     are not in the diff text, so the diff and the full copies are inconsistent. Treat the full copies as the
     reference, and re-diff against HEAD before use.
4. **Private evidence (not in repo, not re-run by me):** `.local-inputs/b035-state10-private-test/run.log` reports PASS for
   ordinary 0x8 no-op, natural Lizard 0x98 and controlled 0x88 each dispatching the Push once, no RNG replay,
   failed-effect replay blocked, and lethal/interrupted cases. Relevant numbers: natural Lizard
   body-motion-xy 109-114 scene units, authored-xy 415-439. The run.log is evidence for the proposal only.
5. **Melee path differs from the state10 test.** The state10 normal-marker case calls `session.request_actor_attack`
   and expects the Session melee marker to reach the push sink. The HEAD melee marker path has no sink call, so
   the proposal's hook is required for the natural Lizard path.
6. **The bound fixture is test-only.** `b035_native_test_fixture_v1.hpp` builds floor, obstacles,
   navigation, body bindings, the transition consumer and a motion phase handler itself. Main has its own
   `nativeBodies`/`nativeWorld` (`main.cpp:2288`, motion handler at `main.cpp:2764`), but I did not verify that main
   binds the session actor-transition consumer (`make_session_actor_transition_consumer_v1`). A grep hit in main.cpp
   matched one of several patterns; I did not confirm which.

## 2. Expected behaviour (preserve; from the tracker and the proposal's evidence)
- A Lizard hit that carries outcome 0x80 (Push) on a player in Player/Idle (state 3) runs Injury 11 first, then
  Push state 10 (KnockedBack, or GreatKnockedBack when mask bit 0x100000 is set), then Idle 3 at sequence end.
- A clean hit (0x8) does not push. A duplicate delivery does not push or replay RNG.
- The body moves along the authored MoveGO path through the native floor/obstacle bodies, not through walls.
- Visible result for the verifier: the hero slides back over roughly the authored distance with a KnockedBack pose.
  Exact distance and speed still need a reference-video measurement (not done).

## 3. Changes
- None. No source, test, runner, CMake, tracker or main.cpp file was modified.
- Created: `.local-inputs/claude-preview13/F/sheet_376_384.png` (scratch, git-ignored).
- Created: this report.

## 4. Tests
- Not run: no build, no focused runner. I did not compile anything, because the in-repo state10 test fails at compile
  time (see Finding 1) and no code changed.
- Static checks run (grep over `port/windows-foundation`):
  - `bind_session_push_effect_v1`: defined nowhere; used only in `b035_state10_native_tests.cpp`.
  - `bind_source_hit_effect_handler` and `CombatRuntimeHitEffectStage`: zero hits in the repo.
  - `apply_source_result(` callers: 4 production paths listed above, none is melee.
- Regression runners (`run_b012/b013/b033`, `run_target_retention_regression_tests.ps1`,
  `run_target_facing_regression_tests.ps1`, `combat_session_combo`): **not re-run.** Nothing changed, so there is
  nothing new to regress. Re-run them after the proposal is ported.

## 5. Uncertainties / not verified
- The 377 s reference example: not found (376-384 s is a tutorial). Push distance and speed from video are unmeasured.
- The feature md is empty, so the expected-behaviour text above comes from the tracker and the private log, not
  from a written feature report.
- Whether main.cpp already binds the session actor-transition consumer. Unverified.
- Package files required (for the eventual fix; none checked yet):
  - `data/animations_dictionary_pyarraynames.bin`, `original-cache/data/pydata/animations_dictionary_pyarray.bin`,
    `original-cache/data/pydata/animations_pyarray.bin`, `.../animations_pyarraynames.bin`, `.../animations_pystructnames.bin`,
    `data/animations_pycst.bin`, `original-cache/data/pydata/loot_table_pyarray.bin`, `.../loot_table_pyarraynames.bin`,
    `.../loot_table_pystructnames.bin`, `actor-profiles-v2.xml`, `original-melee-bindings.xml` (all under `assets/`),
    and `original-cache/data/3d/modules/swamp/swamp.bdae`. SHA256 not computed. Presence in the Preview 12 candidate
    package not checked.

## 6. Recommended next steps (for root / B035 owner)
1. Port the proposal into the repo as one owned change: `actor_combat_runtime.{hpp,cpp}`, `combat_session.{hpp,cpp}`,
   `session_actor_transition_v1.{hpp,cpp}`, `actor_state.cpp`, `character_state.hpp`, and the binder in
   `b035_session_push_effect_v1.{hpp,cpp}`. Coordinate with G (B013) first, because `combat_session.cpp` is shared.
2. Make `b035_state10_native_tests` compile at HEAD, then run `run_b035_state10_native_tests.ps1` and the B012/B013/B033
   runners and the target retention/facing runners.
3. Only then wire main.cpp: bind the push observer and the push bank for the player after `initialize`, and bind the
   transition consumer if main does not already. Keep the hunk small and anchored.
4. Write the B035 feature md (currently 0 bytes) with the video-measured distance once the 377 s example is located.

## Verifier script
**No valid EXE arguments exist yet.** The option list (`main.cpp` parser) has `--combat-*` options (for example
`--combat-auto`, `--combat-player`, `--combat-sequence`, `--combat-source-combo`), but none sets up a Lizard push,
and the push path is not in the integrated EXE. Do not run a verifier against the current candidate and expect a push.
Once steps 1-3 above are done, the verifier should:
- run an isolated save in a fresh output folder, with a Swamp Lizard placed in melee range of the Knight;
- capture log lines for `original_state=10` and `source_outcomes=0x98`, plus frames at 60 fps around the hit;
- expect one push dispatch, the hero moved along the authored path, and a clean-hit control with no movement.
The exact argument line is to be written by the implementer of step 3.
