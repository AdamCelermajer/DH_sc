# Target-retention regression fix (B037 vs B004/B029 interaction)

Status: fixed in `combat_session.cpp` (B037 hunk only). All requested runners exit 0 on the current tree. No ninja/cmake run on the shared build dir. Nothing committed.

## 1. Cause

The failure came from the B037 release guard, not from the B004/B029 OOI owner (the OOI files are not in the runner's source list).

The first failing check is `target_retention_regression_tests.cpp:220-221`, "Space key-up or source attack completion cleared a living selected target". A Tab-selected (sticky) Lizard target is cleared when the Space release path runs.

Call chain on release (lldb breakpoints on a private `-g -O0` build of the same sources, `.local-inputs/retention-debug-b037/lldb1.log`):

```
CombatSession::update (combat_session.cpp:1468)
  -> Impl::depart_source_attack          (B037 release guard)
     -> ActorCombatRuntime::interrupt   (actor_combat_runtime.cpp:299)
        -> depart -> release             (actor_combat_runtime.cpp:101, 120)
           -> CombatSystem::interrupt    (combat_system.cpp:219-225)
              owner->target_id = invalid_actor_id   <-- unconditional clear
     -> CombatSystem::interrupt (again)
```

`CombatSystem::interrupt` clears `target_id` unconditionally. The B037 departure called it on release, so a living sticky target was wiped.

## 2. Which side is right (original behaviour)

- `CSAttack::OnBlur` (IDA pseudocode-all.c:141806) only starts the AttackDelay timer (timer 42) and returns the pinned object. It does not clear the target.
- The target is cleared only by the source clear rules:
  - `CharAI::_ClearNonStickyTarget` (0x3d8d70, pseudocode 156264): `if(!*(this+75)) { AI_SetTarget(null); AI_SyncLastTarget }`. It clears only when the target is not sticky.
  - It is called from the attack step-end code (pseudocode 152458/152469).
  - The port mirror is the `attack_anim_clear_nonsticky` operation (combat_session.cpp ~430), which keeps a sticky player target and otherwise calls `set_source_target(invalid,false)` plus `sync_source_last_target`.
  - The BashDown Post `ClearTarget` (skill rule) clears the target explicitly.

So the retention test's expectation is correct: a sticky living target survives release, and a non-sticky target is cleared with the last-target sync (the later check at test line 326-330 expects exactly that after a post-skill implicit swing).

The B037 intent is unaffected: the interrupt-and-cooldown and the `last`-gated release are kept.

## 3. Change

File: `port/windows-foundation/combat_session.cpp`, only the `Impl::depart_source_attack` body (new in B037; the other two B037 hunks are untouched).

- Capture `target_id` before the interrupts.
- Keep the existing body: cancel cursor, clear `sourceAction`/events and `continued`/`last`, `runtime->interrupt`, `combat->interrupt` (cooldown once).
- After the interrupts:
  - sticky player target: restore `target_id`, return.
  - otherwise: `set_source_target(id, invalid, false)` and `sync_source_last_target(id)`, the same nonsticky rule as the completion path.

Two iterations were needed:
1. Restoring the target after the interrupts only: still failed. The capture was taken after `runtime->interrupt`, which had already cleared it (confirmed by lldb). Moved the capture to the top.
2. Still failed at test line 330 for a non-sticky post-skill swing that must clear to `invalid` with last synced. Replaced the unconditional restore with the nonsticky rule. This passes.

## 4. Evidence

- Baseline (HEAD + B037 edits, before fix): `run_target_retention_regression_tests.ps1` FAIL at line 221 (`Space key-up ... cleared a living selected target`).
- After iteration 1: FAIL at line 330 (`Implicit attack release lost source last-target/OOI candidate state: current=2 last=2 lastKnown=1 ooiKnown=1 ooi=2 searches=3 marker=2`).
- After final fix: PASS.

## 5. Results (real output, exit codes)

Each log is in `.local-inputs/retention-debug-b037/runs/`.

| Runner | Exit | Key output |
|---|---|---|
| run_target_retention_regression_tests.ps1 | 0 | `PASS source target/OOI retention across Space release; actual BashDown Pre/Use/Post cleared current/last while independent OOI survived, decayed/requeried at 500 ms ...` |
| run_target_facing_regression_tests.ps1 | 0 | `PASS KnightPlayerBase -> ...`, `PASS RoguePlayerBase -> ...` (2 PASS) |
| run_auto_target_marker_v1_tests.ps1 | 0 | `PASS original target-marker candidate precedence, null/self fallback, and unfiltered source identity` |
| run_object_of_interest_owner_v1_tests.ps1 | 0 | 21 PASS, ends `OOI owner and marker policy tests passed` |
| features/generic_skills/run_b037_attack_skill_admission_v1.ps1 | 0 | 6 PASS: mid-swing skill admitted; held Space no second attack; dead/hurt/casting rejections; release strike window (last==0) keeps swing; release recovery window (last!=0) leaves Attack at once |
| features/generic_skills/run_post_skill_attack_regression_v1.ps1 | 0 | 3 PASS (PC visual mapping; B003 BashDown held/released/Space; coordinator/Post/reacquisition) |
| features/generic_skills/run_runtime_skill_activation_session_v1_tests.ps1 | 0 | 2 PASS (PC visual mapping; linked coordinator fixture) |
| features/combat/run_b013_attack_continuity_v1_tests.ps1 | 0 | 1 PASS (no-proc hit/miss keeps attack; Injury/lethal interrupt; duplicate delivery) |
| features/combat/run_b033_targetless_attack_admission_v1.ps1 | 0 | 3 PASS |
| features/combat/run_b012_death_mid_swing_v1_tests.ps1 | 0 | 1 PASS (lethal mid-swing) |
| features/combat/run_runtime_player_combo_chain_v1_tests.ps1 | 0 | 1 PASS (ItemTable stances, same-session groups) |

Not run: `combat_session_combo` (CMake/ctest target). It needs the integrated build, as already noted in the B037 report. The B037 report's risk remains: mode 2 "Release discarded previously accepted source continuation" could be affected by the release guard when a release lands on a recovery frame. Root must run `ctest -R combat_session_combo` in the integrated build.

## 6. Uncertainties and risks

- Sticky branch does not re-check eligibility (the completion path does). The per-frame check in `update` (combat_session.cpp ~1447) still clears an ineligible target on the following frame.
- `play_actor_source_sequence` interrupt path (B037 hunk 3) still calls `combat->interrupt`, which clears the target unconditionally (pre-existing at HEAD). A mid-swing skill admitted by B037 now reaches this path during an active swing. For a skill whose source does not ClearTarget, the target may be cleared where the source would keep it. Not changed here (out of the retention scope); flag for the B037 owner and the integrated check.
- The retention test passes; the video/visual behaviour of "release keeps a sticky target and clears a non-sticky one" is not video-verified in this pass.
- Debug artifacts (lldb logs, private debug exe) are in `.local-inputs/retention-debug-b037/` (git-ignored). No shared build dir touched; no commits.
