# Preview 13 Group G: B013 hit and attack continuity (with B010/B012/B015 spot checks)

Status: **B013 remains OPEN.** No production source was changed. The only edit is a diagnostic probe mode in my own focused test file. The main finding is a mechanism on the normal path (almost every positive lizard hit carries the Injury outcome bit), not yet proven to differ from the original, because the live Injury inputs are not established.

Implementer: Preview 13 worker G. HEAD `f6b7f134`. Scratch: `.local-inputs/claude-preview13/g/`.

## 0. Setup notes

- The Preview 12 candidate folder is now named `.local-inputs/windows-source-clock-v19-preview-12/` (the `-candidate` suffix in the brief no longer exists). The EXE I ran has SHA256 `1d43942ecd52df7b86bdf77ba2e5cb306afc76944813f38ca7ce9e6ccb252672`, which matches `package-receipt.json` `exe_sha256`.
- Isolated run folder: `.local-inputs/claude-preview13/g/pkg/` (EXE copy, args files, fresh saves copied from the candidate, `assets`/`audio-assets`/`ui-assets` as junctions). The user's live `dh-foundation.exe` (PID 1 session) was not touched.
- Runs are **not deterministic** with identical args. Example: lizard hit at frame 42 gave `removed=0` in one run and `removed=7.69` in another. Judge each run from its own log, not by cross-run comparison.

## 1. Evidence

### 1a. Visual (normal EXE, Preview 12 candidate, observed in captured PPM/PNG frames)

Scenario: Knight (`KnightPlayerBase`, Longsword01) placed next to two Swamp lizards; `--combat-auto --enemy-ai`; attack held from frame 90 (`--attack-start-frame 90 --attack-frames 600`).

- Frame 15 and 18 (after a lizard hit at frame 14 while the Knight is idle, damage 8): the Knight stands in the idle/guard pose with the sword low. I saw **no clear hurt pose**. Inference only, because the Injured clip was not isolated.
- Frames 128, 132, 136 (mid-swing): Knight holds the sword raised in the swing/ready stance. 132 shows the `Miss` label. 136 shows a red `8` on the player (hit taken) and the Knight still in the swing pose. **No hurt pose seen, so the swing appears to continue through the hit.** This is an observation at 3 frames; a hurt pose lasting less than one capture interval could be missed.
- Frames 276, 282, 292: red `8` at 276. Knight stays in the swing/ready stance; 282 and 292 show `Miss` labels. No clear hurt pose. Inference only.
- Frames 172, 200, 240 (the run where lizard1 died at frame 167 was not reproduced): the lizard was still alive in these runs. **Enemy death mid-swing was not reproduced** in these runs, so B012 is not checked here.

### 1b. Logs (normal EXE)

`Damage frame=` lines from a run of the scenario above (attacker/target `-1` = player):

- 14: lizard1 -> player, removed 8.35 (player idle at that moment)
- 42/43: lizard2 -> player 0 (miss/dodge), lizard1 -> player 0
- 71: lizard2 -> player 8.77
- 106: player -> lizard1 17.20 (player attacking)
- 131: lizard1 -> player 7.87 (player attacking)
- 137: player -> lizard1 16.80
- 159: lizard2 -> player 7.93; 160 lizard1 -> player 0
- 167: player -> lizard1 13.5, lizard1 dies (`dead=1`)
- 188: lizard2 -> player 7.31; 276: lizard2 -> player 7.35
- 305: player -> lizard2, lizard2 dies (`dead=1`)

`Source player step FX` lines show the Knight's attack cadence continuing: frames 103, 132, 162, 210, 240, 270, 300, 348, 378, 408, ... (sequence 470/471/472 cycling, occurrence index increasing). The step at 181 and 319 is back at sequence 470 (first step), which could be a chain restart. It is not aligned with any player-side hit, so I did not attribute it to an interruption.

### 1c. Reference video (v1.0.3, `z_Zky7qQdYs`, 375–379 s)

Contact sheet, 4 fps, 400 px tiles (`.local-inputs/claude-preview13/g/ref/sheet_375_379.png`). Observed: the Warrior keeps a sword swing through the sequence while damage numbers 15 and 17 appear above and around the Bog Moth. The HP bar shortens. At 376.5 the level-up tutorial appears. The numbers appear over the moth, so this clip does **not** isolate an incoming hit on the swinging hero. It does show the hero's swing continuing across hit numbers. Not a clean incoming-hit comparison.

### 1d. Logic (IDA, `pseudocode-all.c`)

- `Character::F_ApplyResult` (overload at `003b10b4`, lines 129649-130153). Structure, as read from the labels and gotos:
  - Reaction branches (Dodge `LABEL_92` -> Block `LABEL_94` -> Injure `LABEL_96` -> Knockback `LABEL_97` -> Stun/Fear `LABEL_23`) are inside `if (!(victim+52)())`.
  - **Idle conversion**: `if ((victim+40)() && CharStateMachine::SM_IsIdle(victim+319, 0))` (line 129886-7). Inside it, when the outcome has no bits in `0x16` (Dodge, Block, Injure), the code sets `0x10` if damage > 0 and `0x2` otherwise (lines 129890-129896). `(victim+40)()` is inferred to be "is player" from its use in the GOD-mode check (line 129768) and the dodge statistic (line 129901). This is an inference.
  - Outside the idle branch (`else` at line 130037), the existing outcome bits still drive the reactions. A busy victim therefore still gets Injure from calculated bit `0x10`.
- `CharStateMachine::SM_SetInjureState` (`003c5d84`, line 142766): if the shared Character timer `+0x14fc` is positive, return. Otherwise set it to 3000 ms and raise event 50010 (or force `_SetState(11, 50010)` when the mask flag `0x18000000` is set). The event handler `Character::CSM_Injured` (`003ad23c`) returns 1 (accept). The Interrupted handlers (`003ad244`) are for 50011-50013.
- `_F_CalculateResult` (`003b2638`, lines 130516-130580) sets the outcome bits:
  - bit 0x1 miss and 0x2 dodge from the roll; 0x4 block when mask `0x10`; **0x8 critical** when mask `0x20` (`CF__CalcCrit`, line 130560 `& 0xF7 | 8*…`); **0x10 Injury** when mask `0x80` (`CF__CalcHurt`, line 130578 `& 0xEF | 16*…`).
  - `CF__CalcHurt` (`003b0e8c`, line 129550+): with the attacker's property `135` > 0, value = `P135 + 5*P19(attacker) - P134(defender) - 5*P19(defender)`, and the result is `value > roll<<8`.
- Label correction: bit `0x8` is **critical**, not a "clean hit". The existing B013 report and the B013 test call `0x8` "clean" or "ordinary", which is misleading. Clean (non-critical, non-proc) hits are `0x0`.

### 1e. Calculation distribution (new probe, candidate calc kernel, authored Knight defender)

`b013_attack_continuity_v1_tests.exe <assets> histogram` (added in this pass), seeds 1-5000, lizard `Swamp_LizadMan_Type1` -> `KnightPlayerBase`, unsuppressed formula:

| outcome | count | meaning (inferred from bits) |
|---|---|---|
| 0x1 | 100 | miss |
| 0x10 | 198 | Injury |
| 0x18 | 51 | Injury + critical |
| 0x90 | 3741 | Push + Injury |
| 0x98 | 910 | Push + Injury + critical |

So 4900/5000 seeds (98%) have positive damage, and every non-miss result carries Injury (0x10). Input values: attacker `135=38400` (150.0), `19=5120` (20.0); defender `134=20736`, `19=256`. Plugging into `CF__CalcHurt` gives 125.0 against `roll<<8`. That is above 100% if the RNG yields 0-99 (not verified). This is consistent with the observed 100% rate.

Consequence on the candidate normal path: every non-miss lizard hit reaches `start_injure_reaction`. The 3000 ms gate limits the reaction to about one per 3 s per actor, and each admitted reaction moves the player to hurt and restarts the attack cursor. This is the mechanism that matches "ordinary hits stun/reset attacks".

Whether the original does the same depends on the live values (the Knight's stats in the save, the lizard's authored level and RNG range), which I could not establish. If the original has the same inputs, its Injury would also interrupt the swing about every 3 s; the reference footage I looked at does not isolate an incoming hit, so it cannot decide.

### 1f. Normal-path routing (important for the owner)

- Normal EXE: `ActorCombatRuntime::consume_source_marker` -> `CombatSystem::consume_marker` (`combat_system.cpp:121-165`, `resolve_damage_with_outcomes`) -> `handle_applied_hit` -> `start_injure_reaction`.
- `CombatSession::apply_source_result` (`combat_session.cpp:1576`) is **not** called by `main.cpp` or the marker path. The B013 focused test uses it. So the B013 test does not prove the normal-EXE behaviour. This matches the brief's "normal-EXE behaviour is not closed".

## 2. Expected behaviour

- Busy victim (attacking): a clean hit does not change the attack pose, target, or clock. A calculated Injury (0x10) follows the 3000 ms gate and interrupts (IDA: event 50010 with `CSM_Injured` returning 1).
- Idle player victim: a positive-damage hit with no Dodge/Block/Injure bit becomes Injury (IDA lines 129886-129896). The candidate does not do this (see 3).
- Dodge/Block: the original raises dodge/block state events. The candidate has no dodge/block reaction and does not reset the attack on those results. Dodge/block seeds were not found in 1..49999 for the tested loadouts.
- Lethal: death path (already present).
- Push (0x80): owned by B035, not changed here.

## 3. Divergences found and decision

1. **Idle player clean hit (real divergence, not fixed).** The original converts a positive-damage, no-reaction-bit hit on an idle player to Injury (IDA 129886-129896, `vtable+40` inferred as "is player"). The candidate does not, so an idle Knight takes no flinch unless the calc already set 0x10 (which is 98% for lizards, so the lizard case is masked). This divergence lives in the normal path (`CombatSystem::consume_marker`, `combat_system.cpp` around line 150-165), not in `apply_source_result`. A correct patch needs the "is player" and "idle" facts at that point. **I did not implement it** because it adds stuns and the root/owner must weigh it against the user's complaint.
2. **Injury frequency (open, not a Session edit).** Per 1e, the candidate's Injury chance for lizard hits is at or above 100% for the authored sheets. The Session/runtime gating is faithful to IDA (3000 ms gate, event 50010). The root question is whether the live Knight/lizard property values (the save's `19`/`134`, the lizard `135`) and the RNG range match the original. That belongs to the calc/data owners, not to a Session edit.
3. **Label error.** `0x8` is critical, not clean. The B013 test's "ordinary hit" case accepts 0x0 or 0x8, so it does not isolate a no-critical clean hit (the no-proc case is `0x0`). A note only, not changed.

No change to `combat_session.cpp` or `actor_combat_runtime.cpp`. I did not find a Session-level divergence that explains the reported symptom alone.

## 4. Changes

- `port/windows-foundation/features/combat/b013_attack_continuity_v1_tests.cpp`: added `#include <map>`, global `histogramMode`, a histogram probe block in `run()` after `const auto seedCategory=...` (prints the outcome distribution and the attacker/defender property values for seeds 1-5000), and a `histogram` argument in `main`. The default run is unchanged. Normal test output is the same as before (`PASS`).
- New files (scratch only, git-ignored under `.local-inputs/`): `claude-preview13/g/` (package copy, captures, tools, logs). `G-report.md` is this file.
- Shared files touched: none. Not edited: `docs/BUGS-AND-IMPLEMENTATION.md`, `main.cpp`, `CMakeLists.txt`.

## 5. Tests

Commands and results:

- `powershell -File port/windows-foundation/features/combat/run_b013_attack_continuity_v1_tests.ps1 -AssetRoot <preview-12 assets> -BuildRoot .local-inputs/claude-preview13/g/b013-build`: **PASS** (Knight and Rogue: ordinary/miss preserve the attack, Injure and lethal interrupt, Push gap exposed, duplicate does not reroll). Note: `dodge` and `block` report `unavailable` (no seed in 1..49999).
- `b013_attack_continuity_v1_tests.exe <assets> histogram`: distribution in 1e.
- Batch runner `g/run_all.ps1` (log `g/runners.log`):
  - `run_b013`: PASS
  - `run_b035_session_push_effect`: PASS
  - `run_b035_state10_native`: FAIL to compile (pre-existing at HEAD, see section 7)
  - `run_runtime_player_combo_chain`: FAIL first (runner expects `libfoundation_data.a` etc. in BuildRoot); PASS after copying the six `.a` libraries into the build folder (section 7)
  - `run_runtime_player_incoming_animation_bank`: same, PASS after the library copy
  - `run_runtime_player_profile_attack_bank`: PASS
  - `run_target_facing_regression`: PASS
  - `run_target_retention_regression`: PASS
- `g/run_fix.ps1` (log `g/runners2.log`): the three runners re-run after the library copy.
- Not run: `run_auto_target_marker_v1_tests.ps1`, `run_b012_death_mid_swing_v1_tests.ps1`, `run_object_of_interest_owner_v1_tests.ps1`, `run_b033_targetless_attack_admission_v1.ps1`. They use their own default build folders, which other workers may own; I did not reuse them. None of these depend on any change made here.
- Mental check, `features/combo_chain/combat_session_combo_tests.cpp` (93 lines): it drives the live marker path (`session.update`, marker counts, `damage receipts`). I made no change to `combat_session.cpp`, so its outcome is unchanged. It assumes the Knight's combo continues through the target's marker hits; if any of those hits carried an Injury bit (as 1e suggests for lizards), that assumption would fail, so it is worth watching if the Injury gate changes.

## 6. Other bugs (quick check)

- **B010** (player faces target): runner `run_target_facing_regression_tests` PASS (Session-level, admitted attack rotates toward a moving Lizard). In the normal EXE, the captured Knight appears to face the adjacent lizard in frames 128-292. This is inference only. Status: **NOT-VERIFIED in the normal EXE** (runner PASS).
- **B012** (death mid-swing): enemy death was **not reproduced** in the captured runs (lizards survived to 240 in three runs; one earlier run killed lizard1 at 167 but the run was not reproducible). The focused B012 runner was not run (default folder). Status: **NOT-VERIFIED**.
- **B015** (pause/resume freeze): not exercised here. Status: **NOT-VERIFIED**. The EXE has `--pause-close-frame` / `--pause-page-frame` options that could be used; not done in this pass.

## 7. Runner results after library fix

From `g/runners2.log`:

- `run_runtime_player_combo_chain_v1_tests.ps1`: **PASS** (actual ItemTable stances 0/2/3; source Attack/AttackStatic roots; same-session groups 0/1/2 with target retained while offscreen).
- `run_runtime_player_incoming_animation_bank_v1_tests.ps1`: **PASS** (Warrior/Rogue/Mage incoming roots and clips; Injured Type2 choices; ordinary no-stance Died Type0; no Knight fallback).
- `run_b035_state10_native_tests.ps1`: **FAIL to compile**: `error: no member named 'knockback_read_gate528' in 'dh::foundation::physics::SessionActorTransitionProvidersV1'`. The fixture `features/combat/b035_native_test_fixture_v1.hpp:90` sets this member, but `features/physics/session_actor_transition_v1.hpp` at HEAD `f6b7f134` does not declare it. This is a pre-existing break at HEAD (the header is unchanged in the working tree), not caused by this pass. Route to the B035 owner.

## 8. Package files required

B013 runtime data (all present in the Preview 12 package `windows-source-clock-v19-preview-12`; first 16 hex digits of SHA256):

- `assets/data/animations_dictionary_pyarraynames.bin` e5276ddf59cea816
- `assets/original-cache/data/pydata/animations_dictionary_pyarray.bin` faee362a0cbf3fdc
- `assets/original-cache/data/pydata/animations_pyarray.bin` 4c9af9a5cbbea556
- `assets/original-cache/data/pydata/animations_pyarraynames.bin` 987e08269dfe8e89
- `assets/original-cache/data/pydata/animations_pystructnames.bin` d4e19b0e688c51b2
- `assets/data/animations_pycst.bin` dfac957b74e79c29 (note: under `data/`, not `original-cache/data/pydata/`)
- `assets/original-cache/data/pydata/loot_table_pyarray.bin` 4dd85c8656d60c38
- `assets/original-cache/data/pydata/loot_table_pyarraynames.bin` 8ea5bc26e47d3b9a
- `assets/original-cache/data/pydata/loot_table_pystructnames.bin` ed6721327d6779e4
- `assets/actor-profiles-v2.xml` a8abf163da310c0a
- `assets/original-melee-bindings.xml` bc29f6ef2ce13f0c

No new asset is needed by this group (no change to runtime content).

## 9. Uncertainties and what the root/verifier must check

- Whether the live Injury rate matches the original. Check: for a Knight at the save's level, count `0x10` in the outcome for lizard hits (use the histogram mode with the live save's property values, or add a trace of outcomes to the normal path). The original's rate is not measured here.
- Whether an Injury (admitted by the 3000 ms gate) actually produces the hurt pose and interrupts the swing on the normal path. Not directly observed. Capture a dense frame sequence (every 2-3 frames) around a player hit while the Knight is attacking, and look for a hurt pose.
- The idle-player flinch divergence (section 3.1) is a candidate patch, not implemented. Decide whether to implement it in `CombatSystem::consume_marker`.
- Dodge/block reactions: not implemented in the candidate; no dodge/block seed found for the tested loadouts.
- Nondeterministic runs: repeated runs with the same arguments differ. A verifier must check the log of the run it inspects.

## 10. Verifier script

Base: copy of the candidate `swamp.args` with the fight options appended (file `g/pkg/run1.args`). Relevant lines:

```
--position
-6472.72,18982.2,24
--frames 1500
--fixed-step 0.016666667
--combat-auto
--enemy-ai
--target-frame 60
--attack-start-frame 90
--attack-frames 600
```

Run from `g/pkg` (assets are junctions to the Preview 12 package): `dh-foundation.exe --startup-config run1.args`.

Capture and check:

- `Damage frame=` lines: player-hit entries (`target=18446744073709551615`) show `removed` values in the 7-9 range from the lizards (frames 14, 71, 131, 159, 188, 276 in the run I logged); player attacks show `removed` 13-17.
- `Source player step FX frame=` lines: sequence 470-472 continues through player-hit frames (no break at 131/159/188 except the 181/319 returns to sequence 470).
- Frames: `--frames 136 --capture x.ppm` shows the Knight still in the swing pose with a red `8`.
- Expected observable for B013 (not yet met): a hurt pose must appear only on an admitted Injury, and at most once per 3000 ms. Clean-hit frames should show the swing.

Tools used (in `g/tools/`): `ppm2png.py` (PPM to PNG), `crop_sheet.py` (cropped contact sheet). Run with `C:/Users/adamc/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe -I`.

## Summary

- B013: OPEN. Found the normal-path mechanism (98% of positive lizard hits carry Injury, admitted every 3 s), plus an idle-player flinch divergence (IDA lines 129886-129896) that is not implemented. Visual: swing continuity observed in the captures (not a hurt pose). Reference footage did not isolate an incoming hit.
- B010: runner PASS; normal EXE NOT-VERIFIED. B012: NOT-VERIFIED (no enemy death capture). B015: NOT-VERIFIED.
- Only edit: a histogram probe in `b013_attack_continuity_v1_tests.cpp`. No `combat_session.cpp` change.
