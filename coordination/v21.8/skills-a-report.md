# v21.8 skills-a: work-order item 5 (Knight/Rogue part) report

Branch `v21.8-skills-a` (worktree `.claude/worktrees/skills-a`, base `windows-foundation` @ 93f4823f). Scope: replace the 4-name whitelist with a data-driven use profile; implement Rogue ViciousStrike, Roundhouse, Quickness; verify Charge/GroundSlam/BashDown/JumpKick in a normal EXE run. Mage skills skipped.

## What changed

- `port/windows-foundation/features/generic_skills/runtime_skill_use_profile_v1.hpp` (new): `kRuntimeSkillUseProfilesV1`, one row per admitted (SkillTable name, script) pair: mana/apply class token, OnPre query (range, cone, sort, LookAt, none for GroundSlam), `sets_cooldown` (only Charge and GroundSlam call SetSkillCooldown in the Lua), Use-time target mode (requery / pre-selected single / pre-list), Use range/cone/sort (GroundSlam range = TempProp1), `rolls_objects` (ViciousStrike pops objects without rolling), Post ClearTarget, Charge shield variant. Each row is transcribed from `.local-inputs/assets-extra/android/data/scripts/skills/prince_*.luac` (plain Lua text despite the extension); the header lists per-row Lua line content.
- `runtime_skill_cast_coordinator_v1.cpp/.hpp`: `RuntimeWarriorSkillBranchV1` (4-branch enum) and the name whitelist are removed. Admission is `find_skill_use_profile_v1(name, script)`; every branch conditional now reads the profile. Behaviour for the four existing skills is unchanged except one intended correction: `SetSkillCooldown` timers start only for the authored sets_cooldown owners (Charge, GroundSlam). BashDown and JumpKick headers say "Cooldown: none/na", so no existing cooldown is lost.
- Dual-wield rows (SkillTable mask bit 0x800000): previously rejected with "Paired-hand ... not supported". Now they follow native `_SkillCombatRoll` (IDA 0x3b9fbc, audit `docs/ida-fidelity-audit-2026-10-11/character3.md` F2; port reference `port/level-world/character_skill_combat_v6.cpp`): one hit with the main hand if `HasMainHandWeapon`, then one with mask|0x4000000 if `HasOffHandWeapon`. The request carries `hand_facts_known`, `has_main_hand_weapon`, `has_off_hand_weapon`; `main.cpp` (hero skill key arm) fills them from `currentLocomotionItems()`. Inference: the audit note and the port kernel agree; I did not re-decompile 0x3b9fbc in this session.
- ViciousStrike and Roundhouse are both dual-wield rows, so both needed this branch.
- `runtime_skill_use_profile_v1_tests.cpp` + CMake `add_test(runtime_skill_use_profile_v1)` (new CTest target): checks six authored (name, script) rows exist in `skills_pyarraynames.bin`, their class tokens exist in `character_classes_pyarraynames.bin`, exact admission (Quickness and mage rows rejected), and the Lua-transcribed numbers.
- `runtime_skill_activation_session_v1_tests.cpp`: the Rogue end-to-end fixture is parameterized (`RogueSkillFixtureSpecV1`) and now also runs ViciousStrike and Roundhouse; checks the dual-wield result count (2 hits per target). Not runnable here (see gaps).
- `run_runtime_skill_activation_session_v1_tests.ps1`: added three link sources (`actor_combat_runtime.cpp`, `retained_sequence_playback.cpp`, `actor_state.cpp`); without them the runner fails to link.

## Per-skill status

Real runs: normal EXE (`dh-foundation.exe` built from this branch), hidden window (`quiet_click_sync.ps1` private desktop), staged v21.8 package as asset root, saves built from the H-report Rogue save and the afterwitch Knight save with the test tool (rank 1 bound to hotbar slot 0, level 9), `--skill-key-frame 150:2` (key 2 = slot 0), `--campaign-skip-frame` to pass the swamp intro. Logs and frames are under `C:/dh218/cases/<case>/` (run.log, f*.png). Lines quoted are from those logs.

| Skill | Status | Evidence |
|---|---|---|
| Knight BashDown | WORKED | Real run `bashdown-1`: `Source skill key=2 slot=0 frame=150 ... phase=1 skill=BashDown`; `Source player step FX ... sequence=347 ... dispatched=1 sets=164`; `Audio step source ... sequence=347 ... sound=190 status=1` (1 = dispatched); frame f162 shows the sword strike pose (seed overlay "Quest Completed" covers part of the frame). |
| Knight GroundSlam | WORKED (cast, animation, FX, sound submission) | `groundslam-1`: phase=1 skill=GroundSlam; FX `sequence=350 ... sets=173 dispatched=1`; sound `sequence=350 ... sound=185 status=1`; f170 shows the overhead strike pose. Hit application not logged. |
| Knight Charge | WORKED (cast, animation, FX, sound submission) | `charge-1`: phase=1 skill=Charge MP=21.5; FX `sequence=349 ... sets=172 dispatched=1`; sound `sequence=349 ... sound=48 status=1`; f170 shows the charge burst and sword pose. Hit application not logged (matrix "no hit seen" still open). |
| Rogue JumpKick | WORKED (cast, animation, FX, sound submission) | `jumpkick-1`: phase=1 skill=JumpKick MP=28.25 (from 40.25); FX `sequence=521 ... sets=160 dispatched=1`; sound `sequence=521 ... sound=125 status=1`; f162 shows the kick pose. Hit application not logged. |
| Rogue ViciousStrike | PARTIAL | `vicious-4`: phase=1 skill=ViciousStrike MP=24.25 (from 40.25); FX `sequence=528 ... sets=163 dispatched=1`; sound `sequence=528 ... sound=225 status=1`; f156/f170 show the dual-dagger spin (distinct from idle). Not verified: target damage (no hit line logged; the unit fixture that asserts two hits per target cannot run here), the stun side effect (the Lua applies `_Stun` class, the port does not reproduce stun tails). |
| Rogue Roundhouse | PARTIAL | `roundhouse-2`: phase=1 skill=Roundhouse MP=20.25 (from 40.25); FX `sequence=525 ... sets=162 dispatched=1`; no sound by design (Lua sound line commented out; log `sound=-1 status=1`); f162 shows the kick/spin pose. Not verified: hits (same as ViciousStrike). |
| Rogue Quickness | DID NOT WORK | `quickness-1`: `Source skill key=2 slot=0 ... phase=0 skill=Quickness ... diagnostic=No complete generic same-session Use arm is implemented for this authored source skill script`. Quickness is a timed buff (CreateBuff/ApplyBuff of `Skill_Rogue_Quickness_buff`, duration SnS_Duration or 4000+1000*level, FX `spell_dh2_prince_rogue_quickness`), and no generic timed-buff owner for the player sheet exists in the port. Left out of the table on purpose (unit test asserts it stays out). |

Mana numbers are the observed MP after the key press (the prefix is committed before the Use); I did not cross-check them against the class projection.

## Tests

- CTest (full run on the final commit, `C:/dh218/skills-a-build`): 186/188 passing. The 2 failures are `private_save_transport_win32` and `source_level_config`. Both fail on the baseline too, and both need inputs absent from the primary `.local-inputs` (`publication/checkpoint/...` assets and `windows-camera-assets`). Baseline at 93f4823f, before the worktree junctions for `windows-equipment-assets`, `windows-melee-bindings`, `actors` and `windows-toolchain` were added: 172/187, with 15 failures. Most of those were the same missing-input class; `frame_pacer` was a timing flake. New test `runtime_skill_use_profile_v1` passes.
- Run hygiene: `ctest` started through PowerShell `Start-Process` stalled repeatedly with no test child running. The same suite started from bash completed normally, so use bash for ctest in this environment.

## Evidence per AGENTS.md (investigate before implementing)

- Visual reference: NOT compared against the original footage in this session. The coverage matrix and the Lua headers were used instead. Real-run frames are the only visual evidence (see above), and they show the animation pose and the FX dispatch, not an original-video match.
- Logic reference: the Lua sources (`prince_rogue_vicious_strike`, `prince_rogue_roundhouse`, `prince_rogue_quickness` and the four whitelisted scripts) for targeting, mana, cooldown, Use, Post and sound. The dual-wield branch comes from the IDA audit note (`character3.md` F2, IDA 0x3b9fbc), cross-checked against the port's Android kernel `character_skill_combat_v6.cpp`. I did not re-decompile 0x3b9fbc in this session.
- Uncertainties: the hit side (damage on the targets) of ViciousStrike and Roundhouse was not logged in the EXE runs; the stun effect is not reproduced; mana numbers are the observed MP after the key press and were not cross-checked against the class projection; Quickness's buff row (`Skill_Rogue_Quickness_buff`) effect was not decoded.
- Verification: unit (CTest table test, syntax-checked activation fixture), real EXE (animation, FX set, sound-cue status) as listed above.
- `runtime_skill_activation_session_v1_tests` (PowerShell runner) compiles and links with the runner fix, but the binary crashes (access violation, rc 0xC0000005) or exits with "unsupported character schema version" / "Asset root is not an existing directory" depending on the launch environment. The same crash occurs at HEAD (checked with HEAD copies of the three coordinator/test files), so this is a pre-existing failure and not caused by this change. The new ViciousStrike and Roundhouse fixture rows are therefore unit-unverified.

## Gaps and next steps

1. Quickness needs a generic timed-buff owner (apply class-row props to the player's sheet for the buff duration, remove on expiry, FX). Not started.
2. Stun/knockback/DOT tails of ViciousStrike and Roundhouse results are not reproduced by the port (documented in the header).
3. Hit application for ViciousStrike and Roundhouse is unverified in the EXE (no damage log line; needs a seed with enemies in front, or a damage log). The one-hand dual-wield path (only main or only off hand) has no unit test.
4. The activation session runner needs its crash/schema failure fixed before its fixtures count as unit evidence.
5. Environment: the worktree `.local-inputs` junctions to the primary were removed at the end of this run (`cmd /c rmdir`); recreate them the same way before any build or test.
