# Preview 13 wave 3 (N): B035 push probability, wiring status and verifier

Status: **push derivation verified against IDA; live-input push probability is 0 in the port; EXE wiring NOT done** (blocked on owners listed in section 3). B035 stays open. Repo HEAD at start `5db15994`. Only repo edits: `port/windows-foundation/CMakeLists.txt` (test registration, section 4). No `main.cpp` change. No commit. Scratch: `.local-inputs/claude-preview13/N/`.

## 1. Evidence: which result bits mean Push

### Logic (IDA, `pseudocode-all.c`)
- `Character::_F_CalculateResult` (`003b2638`, lines 130516-130640). Mask bits drive outcome bits:
  - `0x200` -> `CF__CalcPush(att, def, roll<<8, mode 0)`; `0x400` -> mode 2. The roll `Random::GetRandom(100)` is drawn whenever either bit is set (before any gate). Result stored as `outcome |= v28<<7`, i.e. **outcome 0x80 = Push**.
  - `0x80` -> `CF__CalcHurt` -> outcome `0x10` = Injury (0x100 mode 2).
  - `0x20`/`0x40` -> `CF__CalcCrit` -> outcome `0x8` = critical. `0x10` -> block `0x4`. Miss/dodge (`0x1`/`0x2`) skip the rest (`goto LABEL_32`).
- `CF__CalcPush` (`003b0d78`, lines 129501-129530):
  - mode 0: `P137(att) > 0` required, else false. Then `P137 + 5*P19(att) - P136(def) - 5*P19(def) > roll<<8`.
  - mode 2: same with `P183(att)` and `P136(def)`.
- Outcome-bit names: `port/game-data/combat_result.hpp:9` lists bits 0..8 as miss, dodge, block, critical, hurt, fear, stun, push, slow. So 0x98 = push+hurt+critical, 0x88 = push+critical (controlled), 0x90 = push+hurt.

### Port derivation (matches IDA)
- `port/game-data/combat_result.cpp:55` (`dh2_combat_result`): the push roll uses `mask&(3<<9)`, `status(..., field 1, magic=(mask&(1<<9))==0)`, `status()` (line 18-31) computes `P137+5*P19a-P136d-5*P19d > roll` with the normal/magical/resist tables `{135,137,...}`, `{182,183,...}`, `{134,136,...}`, and sets bit `1<<7`. Mode choice (0x200 vs 0x400) and the 0x80 bit position reproduce IDA. Roll and status order match. No discrepancy found in the derivation.
- The lizard melee mask `0x22aab5` has bit `0x200`, so the push roll is drawn for every non-miss lizard hit; the push bit is only set when the status test is true.

### Live-like probability (new probe, `.local-inputs/claude-preview13/N/`)
Probe = `b013_attack_continuity_v1_tests` histogram-live (seeds 1..5000, mask `0x22aab5`, `resolve_result` = production calc, enemy at its CharacterTable base level as `main.cpp` does). I added a print of P136/P137 in a scratch copy only (the repo file is unchanged).

| Knight level | defender P136 (push resist) | lizard | lizard P137 / P19 | push threshold (P137+5*P19a-P136-5*P19d) | seeds with outcome 0x80 |
|---|---|---|---|---|---|
| 1 | 25600 | Type1 | 1280 / 256 | -24320 | **0 / 5000** |
| 1 | 25600 | Type2 | 1280 / 256 | -24320 | **0 / 5000** |
| 1 | 25600 | Type3 | 14080 / 256 | -11520 | **0 / 5000** |
| 1 | 25600 | Swamp_MovieMoth | 1280 / 256 | -24320 | **0 / 5000** |
| 5 | 29696 | Type1 / Type3 | 1280 / 14080 | negative | **0 / 5000** |
| 20 | 45056 | Type1 / Type3 | 1280 / 14080 | negative | **0 / 5000** |

Level-20 lizard (the B013 `histogram` mode and the B035 state10 fixture, `enemy.propertyOptions={20*256}`): lizard P137=25600, P19=5120 gives threshold about 24320, so push in ~93% of seeds: `0x90` 3741, `0x98` 910 (matches G). That override is not the live EXE: `main.cpp` sets no enemy level, so live lizards are level 1.0 (`actor-profiles-v2.xml` gives `Level=256 LevelMin=256 LevelMax=768`).

### Answer to the risk question
- At live-like inputs the true push probability for an authored Swamp lizard hitting the authored Knight is **0** in every case tested: it is not "rare", it is zero, because the Knight's push resist P136 (25600 = 100.0 at level 1) exceeds the lizard's P137 (1280..14080) plus its level term.
- The level scaling does not change that: the original lizard level is `clamp(player level, 1, 3)`, so at most P19a = 768. Even then the threshold stays negative for Type1/Type3 at level 1-3 Knights unless P136 is far lower than the port's value.
- Clean `0x0` and critical `0x8` cannot carry `0x80`: the bit is only set when the status test is true, and the port matches IDA's bit placement. So the normal path would not push on clean or critical hits.
- **Critical unverified input:** the Knight's P136 = 25600 comes from the port's resolved property sheet (`resolved[136]`), not from an IDA trace or the original property data. If the original Knight's push resist is far lower (under about 1280 for a level-1 Knight), the calc would push on a meaningful fraction of hits. Verify P136 against the original property row before any push is enabled. This is the only thing that could reproduce the "every hit pushes" bug with this formula.
- The tracker's B035 claim "actual Lizard 0x98 emits one Push request" rests on the level-20 override, so it is not a live-path fact. The state10 test "natural" 0x98 case (`b035_state10_native_tests.cpp:109`, `:400`) is a level-20 case; relabel it, do not treat it as live evidence.

### Visual evidence
Not re-examined in this wave (no hit clip in the reference video was found; F-report and J-report). Push distance/speed from video remain unmeasured.

## 2. Push-bit derivation: changes
None. The derivation matches IDA; no fix needed. Probe-only scratch file was created and deleted from the repo tree.

## 3. Wiring into the normal EXE (task 2): NOT done
Binder (`bind_session_push_effect_v1`) must not be bound yet. Binding it would make the Knight's state10 transitions fail: `features/physics/session_actor_transition_v1.cpp` returns "KnockBack suffix requires actual gate/controller/look/sneak owners" when the owners are missing (line ~291), and "KnockBack Blur requires the actual controller lock owner" (line ~179). Owners the normal EXE still lacks (`main.cpp` consumer config at ~2226-2280):
1. `knockback_read_gate528` / `knockback_write_gate528`: trivial, use `contextFor(id).gate528`.
2. `knockback_controller_lock`: no player controller-lock owner in main. Needs a decision and a player input consumer that honours it (not a no-op).
3. `knockback_look_at_cancel_sneaking`: LookAt attacker via `motor->setDesiredHeading` (`actor_movement.hpp:64`); CancelSneaking has a diagnostic no-op already (`main.cpp:1540`).
4. Push bank: `build_session_push_animation_bank_v1` needs the player's CharAnimTable row, stance and stanced mask, and its clips must be appended to the player's visual config **before** `next->initialize` (`main.cpp:965`). The skill animation tables are block-local (`main.cpp:761`); no knockback row owner exists.
5. Sink: `play_session_push_animation_v1` with `animationServices` (`main.cpp:1506`) is available.

Main hunks needed (for the integration owner): ~60 lines across main (bank append before initialize, binder call after the session binds, consumer owners) plus a feature file. Not done because items 2 and 4 are new owners, and I could not build the EXE to test them (ninja/cmake on the shared dir is forbidden).

`main.cpp` syntax check (real compile line from `ninja -t commands dh-foundation.exe`, `-fsyntax-only`, no `-o`, toolchain on PATH): **exit 0** at HEAD `5db15994` (log `N/main_syntax.log`).

## 4. CMake (task 2 part): minimal test registration
`port/windows-foundation/CMakeLists.txt` after `combat_session_combo` (~line 561): `target_sources(foundation_data PRIVATE features/combat/b035_session_push_effect_v1.cpp)`, `add_executable(b035_session_push_effect_v1_tests ...)` linked to `foundation_data`, `add_test(NAME b035_session_push_effect_v1 ...)` with the `windows-shared-assets` root (same as other combat tests). Its sources are all in `foundation_data` (`CMakeLists.txt:52` has `skill_animation_program.cpp`); the binder has no physics dependency. **Not configured or built** (cmake/ninja forbidden on the shared dir); the root must build target `b035_session_push_effect_v1_tests` to confirm.
- `b035_state10_native_tests` is **not** registered: it needs ~20 standalone sources including `session_actor_transition_v1.cpp` (in `foundation_runtime_enemy`), `playable_actor_bodies.cpp` and `original_actor_physical.cpp`. I did not verify a link set without a configure.

## 5. Verifier script (task 3): NOT available
- No EXE option produces a push. The binder is not bound, so no `--combat-*` argument line can show a push. Do not run a push verifier against the current candidate.
- Forced-result option: **not implemented**. Recommended design (for the integration owner): a development flag, default off, e.g. `--dev-force-push-first-hit`, that ORs `0x80` into the first applied enemy-to-player `source_outcomes` in `CombatSystem::consume_marker` (`combat_system.cpp:164`) or in the binder observer. Both are shared files; that edit belongs to the owner.
- Expected observable once wired: log `original_state=10` / `source_outcomes=0x98` (or forced 0x90) on the first player hit; hero displacement per frame over about 1 s (authored path, not into walls); control run with no flag and the same seed shows zero displacement and no KnockedBack state.
- Clean-hit control available now: the lizard hits in the current EXE never push (binder unbound), so a run of the G args (`coordination/claude-preview13/G-report.md` section 10) gives the zero-displacement control. Not re-run here.

## 6. Standalone runners (task 4)
See the runner table in section 7. All runs at HEAD `5db15994`, against the preview-12 assets, compiled from source per runner script; the shared prebuilt libs are only used where a runner links them. Failures were classified as harness (argument or missing library copy) or code.

## 7. Runner results
Every `port/windows-foundation/features/{combat,generic_skills,combo_chain}/run_*.ps1` script that I could launch. Logs: `N/runners/`, `N/runners2/`.

Runners that take `-AssetRoot`/`-BuildRoot` (own build folder, preview-12 assets):

| runner | result | note |
|---|---|---|
| run_auto_target_marker_v1_tests | PASS (exit 0) | |
| run_b012_death_mid_swing_v1_tests | PASS (exit 0) | `PASS lethal mid-swing` |
| run_b013_attack_continuity_v1_tests | PASS (exit 0) | |
| run_b035_session_push_effect_v1_tests | PASS (exit 0) | |
| run_b035_state10_native_tests | PASS (exit 0) | natural 0x98 case uses the level-20 lizard override (section 1) |
| run_object_of_interest_owner_v1_tests | PASS (exit 0) | |
| run_runtime_player_profile_attack_bank_v1_tests | PASS (exit 0) | |
| run_target_facing_regression_tests | PASS (exit 0) | |
| run_target_retention_regression_tests | PASS (exit 0) | |
| run_runtime_player_combo_chain_v1_tests | PASS (exit 0) on retry | first try was harness: runner needs the six `.a` libs incl. `physics-backend/libdh2_box2d_201.a` in BuildRoot; copied and re-run |
| run_runtime_player_incoming_animation_bank_v1_tests | PASS (exit 0) on retry | same harness fix |

Runners that accept no `-AssetRoot` (only `-Compiler`/no args), run with their default paths and build folders (not my BuildRoot):

| runner | result | note |
|---|---|---|
| run_b033_targetless_attack_admission_v1 | PASS (exit 0) | F2 saw FAIL at HEAD; F2 may have used other args. Not reconciled. |
| run_post_skill_attack_regression_v1 | PASS (exit 0) | F2 saw FAIL at HEAD. Not reconciled. |
| run_runtime_skill_activation_session_v1_tests | PASS (exit 0) | F2 saw FAIL at HEAD. Not reconciled. |
| run_b037_attack_skill_admission_v1 | **FAIL (exit 1)** | exe prints `B037: swing departure left stale combo continuation; continued=0 last=1` (log `.local-inputs/b037-attack-skill-admission-v1/build-174376`). Same failure as F2's HEAD baseline. |
| run_pc_gameplay_hud_v1_tests, run_pc_skill_hud_projection_v1_tests, run_pc_skill_input_binding_v1_tests, run_runtime_skills_text_v1_tests, run_runtime_skill_activation_v1_tests, run_runtime_skill_animation_bank_rank0_v1_tests, run_runtime_skill_mana_v1_tests, run_runtime_skill_progression_v1_tests, run_runtime_tests, run_tests | PASS (exit 0) | defaults |

Not run: `features/combo_chain/combat_session_combo_tests.cpp` (no `run_` script; F2 ran it standalone: PASS). `run_b035_state10` and the B035 session runner were run at HEAD and pass, so the binder code itself is not regressed.

Baseline note: my runs are on `5db15994` (repo files unchanged apart from the CMake registration, which does not affect the standalone runners). F2 reported segfaults for b012, target_facing and combo_chain against the stale shared `libfoundation_data.a`; those runners PASS in this run, so the segfault did not reproduce here (I did not inspect which archive they linked). The b037 failure is a real assertion, not a harness problem; I did not diagnose it further (outside the push scope).

## 8. Package files required
None. This wave adds no runtime asset. The push path needs the same Preview 12 package files listed by F2 (`original-cache/data/pydata/*`, `data/animations_*`, `actor-profiles-v2.xml`, `original-melee-bindings.xml`, `original-cache/data/3d/modules/swamp/swamp.bdae`), all present in `windows-source-clock-v19-preview-12` (loaded successfully by the runners). SHA256 not computed.

## 9. Uncertainties and not verified
- Knight P136 (push resist) value vs the original property data: the single critical input. Not verified.
- Lizard P137 at monster level 2-3 (original level scaling): not measured. Doesn't change the zero result unless P136 is much lower.
- Idle/Player identity of the push gate and the controller lock semantics: not verified against the original input code.
- Nothing from the video. No EXE run. No push observed anywhere.
