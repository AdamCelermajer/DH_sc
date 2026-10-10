# B037 report: skill admission while Space is held; attack release

Status: source-logic fix implemented; isolated same-Session tests pass. Reference-video evidence was NOT gathered in this pass. Not integrated into the main EXE and not verified in a live window. Do not claim B037 closed.

## 1. Evidence

### Logic (IDA: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
- `CSAttack::OnInit` (0x3c8284, line 144574): `50005 -> state 6` is registered with no guard. `50001 -> state 4` is guarded by `Character::CSM_StoppedAttacking`.
- `Character::CSM_StoppedAttacking` (0x3ad280, line 127022): `return (a4==5) ? *(byte*)(this+1090) : 0;`. Its listing (`LDRBEQ R0,[R0,#0x442]`) is the only read of offset 1090.
- Writer of +1090: `Character+968` is the embedded `CharAI`, so `CharAI+122 == Character+1090`. `CharAI::_OnAnimStepBegin_Attack` (0x3d4044, line 152492) and `_OnAnimStepEnd_Attack` (0x3d3e44, line 152396) write `+121` (= Character+1089, read by `CSM_Interrupted` case 5) and `+122`. Inference: +122 is the per-step "last / no further input" flag. It matches the port's `AttackState64::last`.
- Port mirror: `level-world/character_ai_attack.cpp:41` `if(continued&&s.last) return 0;` matches the source melee gate. `features/combo_chain/INTEGRATION.md`: pre and recovery steps set `last=1` and ignore input.
- Skill gate: `CharAI::AI_IsSkillUsable` (0x3d8358) rejects only when a skill is in use (unless Character+1312 & 0x8000) or casting. It has no attack-state test. Taken from the lead doc, not re-derived line by line.

### Port state before the fix
- `features/generic_skills/runtime_skill_cast_coordinator_v1.cpp` `begin_skill_cast_v1` rejected any action other than idle/moving. Held Space keeps the player in `attacking`, so every skill press was rejected.
- Release: the port never leaves a swing on release. `CombatSession::update` only skips `command_request` when `input.attack` is false. The swing ends at a group boundary when no continued input exists (`features/combo_chain/INTEGRATION.md`). The source instead leaves Attack immediately on release when +1090 (`last`) is set.

### Video (reference)
- NOT examined. No frames were extracted for B037 and no claim is made about the original visual sequence. Still needed: a mid-swing skill press and a release during pre/recovery in `.local-inputs/reference-video/dh2-act1/`, checking (a) the skill is accepted during the swing and (b) whether the swing is cut at release. The verifier/fidelity agent should do this.

## 2. Expected behaviour and implementation

Expected, from source logic: a skill pressed during a held-Space swing is admitted; the swing is cut by the skill (attack-departure cooldown once); the still-held Space does not start a second attack until the skill's Post completes. Skill rejected while casting, while using a skill, and when dead. Release leaves Attack only when `last` (+1090) is set.

Implemented:
1. Admission (`begin_skill_cast_v1`): the idle/moving gate is replaced. Rejected: `casting` (skill in progress); `hurt` (kept as before, flagged as unverified against source Injured admission). Dead is still rejected by the existing `alive()` check. The existing unfinished-occurrence check still applies. `attacking` and `moving` are admitted.
2. `previous_action` for an admitted skill is `idle` when the swing was active, so a failed sequence start does not restore `attacking` with no attack owner.
3. `CombatSession::play_actor_source_sequence` interrupt path (existing `s.combat->interrupt(id)` line): also clears `sourceAttack.continued` and `.last`, so the interrupted swing leaves no stale continuation. The interrupt already starts the attack-departure cooldown (`CombatSystem::interrupt`, `CooldownTiming::attack_departure`), matching the source OnBlur timer42 behaviour.
4. New `CombatSession::Impl::depart_source_attack` (accepted departure, used by the release path): cancels the retained cursor, clears `sourceAction`, clears continued/last, interrupts the runtime pose and calls `combat->interrupt` once. No-op for non-combo or inactive swings.
5. Release guard in `CombatSession::update`: when `input.attack` is false, the player is `attacking`, the swing is a source combo swing with `sourceAction`, and `sourceAttack.last != 0`, call `depart_source_attack`. A release with `last == 0` is ignored and the swing runs to its step end.
6. No input buffering added; `main.cpp` is untouched. The press edge is still a one-shot `begin_skill_cast_v1` call. Admission now succeeds during attack, so the reported case needs no buffer. A press while casting is still dropped. No source evidence was found for a buffer, so none was added.

## 3. Changes
- `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`: `begin_skill_cast_v1` admission block (anchor: `if (actor->action == CharacterAction::casting)`) and the `active.previous_action` hunk (anchor: `active.target_query = target;`).
- `port/windows-foundation/combat_session.cpp` (shared with B004/B029; three surgical hunks):
  - Impl: new `depart_source_attack` method, inserted before `bool command_request(` (line ~889).
  - Impl `update()`: release guard, inserted after `if(input.attack){...command_request...}` (line ~1458).
  - `play_actor_source_sequence`: two-line reset after `s.combat->interrupt(id);` (line ~2025).
- New: `port/windows-foundation/features/generic_skills/run_b037_attack_skill_admission_v1.ps1`. Generates a private copy of the canonical session test, enables `sourceCombo` in that copy (the production profile does this, `main.cpp:887`), injects the B037 probe at the B003 anchor, then compiles and runs it. The canonical test file is not modified.
- Edited `port/windows-foundation/features/generic_skills/run_post_skill_attack_regression_v1.ps1`: one line. The anchor count used `String.Split(string)`, which under Windows PowerShell 5.1 splits on characters and failed with "anchor missing or ambiguous". Changed to `-split [regex]::Escape(...)`. Otherwise unchanged.
- `main.cpp`: not touched, so no syntax check was needed.

## 4. Tests
Windows PowerShell 5.1 with the brief's toolchain.

- `run_b037_attack_skill_admission_v1.ps1` (new): EXIT=0
  ```
  PASS B037 mid-swing skill admitted: generation=2
  PASS B037 held Space during skill: no second attack before Post, completed once
  PASS B037 dead and hurt rejections hold; casting rejection holds
  PASS B037 release in strike window (last==0) did not stop the swing
  PASS B037 release in recovery window (last!=0) left Attack at once
  ```
  Covers: held Space starts a swing; skill pressed mid-swing accepted (`prepared_pending_use`, `action==casting`); `sourceAttack.continued==0 && last==0` after departure; a second skill while casting rejected ("casting"); 240 frames of still-held Space during the skill with no attack restarts and the skill completing; dead rejected ("dead"); hurt rejected ("Hurt"); release on a strike frame leaves the swing running; release on a recovery frame leaves Attack on the same frame; swing ends without further commands.
- `run_post_skill_attack_regression_v1.ps1` (B003): EXIT=0 on the final code. PASS: PC visual mapping regression; B003 BashDown held-through-skill, released/neutral, then Space; B003 coordinator/Post, Space reacquisition, released attack completion.
- `run_runtime_skill_activation_session_v1_tests.ps1`: EXIT=0 on the final code. PASS: PC visual mapping regression; linked coordinator fixture (Warrior slots, Rogue slot, progression, restore, Use/Post, duplicate suppression, interruption, checkpoint).
- Failures seen during development, all fixed, all in my probe or runner rather than production: (a) runner anchor count under PS5.1 (see Changes); (b) the first probe run lacked `sourceCombo`, so there was no combo state; fixed by enabling it in the private copy; (c) a probe `continue` skipped `advance_after_session_update`, which trips "Skill cooldown clock must observe each successful Session update exactly once"; fixed in the probe.
- NOT run: `combat_session_combo` (`features/combo_chain/combat_session_combo_tests.cpp`). It is a CMake target. A standalone link against the shared archives still fails on script-VM/FSM symbols (`dh2_script_vm_bind_source_values`, `dh2_character_native_fsm_get_integer`), so it needs the integrated build. RISK: mode 2 asserts "Release discarded previously accepted source continuation" must not happen. If the release lands on a recovery frame (`last != 0`), the new release guard ends the swing and that check may fail. Root must run `ctest -R combat_session_combo` in the integrated build and reconcile with the source 50001 guard if it fails.
- NOT run: a negative control with the pre-fix gate. By code reading, the first probe check (mid-swing admission) would fail under the old idle/moving gate.

## 5. Uncertainties / not verified
- Reference video: no frames examined. Skill-during-swing and release-cut behaviour are inferred from IDA, not observed.
- Hurt: the source table accepts hurt (Injured) unguarded. Left rejected and flagged in code. Decide separately.
- Post-skill resumption: the probe does not assert whether a still-held Space restarts the swing after the skill's Post. The B003 path (fresh Space after neutral) passes.
- Attack-departure cooldown: `CombatSystem::interrupt` starts it once; the test does not assert the timer count.
- `last` semantics: the port's `AttackState64::last` is mapped to +1090 via the CharAI+122 offset and the melee gate. The step-begin/step-end writes were not traced line-by-line against the port's combo boundaries. The verifier should confirm `last` is 1 exactly in the pre/recovery windows.
- The release guard applies only to source combo swings (`sourceCombo`). The production player profile sets it (`main.cpp:887`); legacy/diagnostic attacks keep the old release path.
- Integrated checks for root/verifier: (1) `combat_session_combo` via CMake (see risk above); (2) launch the EXE with a fresh save, hold Space, press skill key 2 mid-swing, and confirm the log shows the skill accepted, the swing cut once, and no second attack until Post; (3) hold Space, release during a swing, and confirm the swing leaves Attack only at a recovery frame.
