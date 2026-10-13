# act1rogue report: can ACT 1 be played start to finish as the Rogue?

Branch `v21.8-act1rogue`, worktree `.claude/worktrees/act1rogue`, base `windows-foundation` 586efa0c. Subst `R:`. Build dir `C:/dh218/act1rogue-build` (Release, llvm-mingw, `dh-foundation` target). Runner: `coordination/v21.8/act1rogue-run-rogue.ps1` (copy of the v21.7 `run-case.ps1` that uses the v21.8 staged package and `C:/dh218/seeds`; its `--audio-table` path was fixed during this session, see section 4). Case folders: `C:/dh218/act1rogue/cases/` (outside the repo).

## Verdict

**Not verified end to end. ACT 1 has NOT been shown playable as the Rogue.** This session reached only the first Swamp segment. The lizard fight, the Bogwitch Cavern (Witch), the Mud Lord and the Darkwood load were not reached in a real run. The Rogue's damage output to monsters has not been observed at all (no damage or death event in any run). The full CTest is green (202/202 on the untouched base, which this branch has not changed).

## 1. Segment results

| # | Segment | Result | Evidence |
|---|---|---|---|
| 1 | Menu: Single Player, Continue a Rogue profile (`rogue-jumpkick` seed, class RoguePlayerBase) | WORKED | r01 log line 2 and 7: `class: RoguePlayerBase`; `Frontend launched same CharacterState slot=0 class=RoguePlayerBase`. Frames in r03 `f1700.png` show the Rogue in the Swamp with two daggers. |
| 2 | Swamp intro cinematic (Swamp_Intro, "The Boglands - Ancient Prison") | PARTIAL | r01 `f1300.png` shows the cinematic with the SKIP button. Enter (VK 13) at 16 to 20 s ends it (r03/r04: `cinematic=0` from frame ~1190). Enter presses at 8 to 20 s (r02) did not get past the main menu, so the timing is fragile. |
| 3 | Rogue movement and HUD (hotbar 1 to 5, Faery, potion) | PARTIAL | r03 `f1700.png`: HP/MP bars, hotbar slots 1 and 3 empty, slot 2 has an icon, `5 Potion: 5`, `4 Faery`. Movement was only tested through `--player-to` (teleport), not walking. Which skills are on the hotbar was not checked. |
| 4 | Rogue basic attacks (Space, dual daggers) | WORKED at the input and animation level; damage NOT verified | r04: 7 Space presses gave 7 `attack.pressed` input edges (`STATE_TRACE edge=input ... semantic=1,1`). r05 (30 s of Space at the start position): 186 `Source combo boundary` lines. The clip is `prince_dual_pre_combo_01.bdae` (4-step dual combo). The Knight control (k01, afterwitch seed) uses `prince_1hand_pre_combo_01.bdae`. So the Rogue plays its own dual-wield combo. |
| 5 | Lizard ambush and lizard intro (LizardMan_Intro, LizardMan_Ambush1) | DID NOT WORK (not verified) | r06: `--player-to -2900,-100,156@900` (inside `_prim_TriggerZone_LizManIntro`) triggers the intro (`cinematic=1` for 24 traces), then `Tab` and Space. Lizards are listed as targetable (`Swamp_LizadMan_Type1 targetable=1`, STATE_TRACE in r06). Input edges arrive, but there are 0 combo boundaries and 0 damage or death lines. Cause not identified. |
| 6 | Witch cave and Witch fight (B154) | NOT RUN | Needs a Rogue run from the `afterwitch` path or the witch-cave transition. |
| 7 | Mud Lord (B155, B158, B159) | NOT RUN | The Knight-only kill is in `coordination/v21.8/mudlord3-report.md`. A Rogue run would need the hero walked to the relocated boss (B158). |
| 8 | Darkwood load (`003_darkwood.mlx`) | NOT RUN | The Knight load was shown in mudlord3 runs m3f4 and m3f5. |

## 2. Ranked blockers

1. **No Rogue damage observed at all.** Attacks run (segment 4), but no lizard, witch or boss damage, death or XP was logged for the Rogue. The first real-run check still to do: a lizard in front of the hero, Tab to target it, Space until `Damage` and `dead=1` appear. This is the blocker for every later segment.
2. **Stale or misleading combat policy message for non-Knight classes.** `main.cpp` lines 1143 to 1153 give every non-Knight playable class `animationOnly=true` and print `outgoing attacks await their own source pipeline`. The run shows that the Rogue still plays combos, through the player's own melee bank (`Source player melee bank class=RoguePlayerBase ... off=370`). So the message is wrong, or the animation-only profile is only a label for the combat session. The combat-session guards (`combat_session.cpp` lines 906, 922, 958: "Animation-only actor rejects combat commands") would block a Rogue who needs a real combat profile. Whether these guards apply to the player must be checked before any Rogue damage fix. Investigation first, then decide.
3. **Lizard fight path unknown** (segment 5). Attacks give 0 combo boundaries in r06 but 186 in r05. The difference is the position (lizard intro area vs start) and the intro-trigger state. Not investigated.
4. **Hotbar skill assignment on the Rogue seed unknown.** Slots 1 and 3 are empty in `f1700.png`. The skill list of the `rogue-*` seeds was not checked. The four Rogue skills in the task (JumpKick, ViciousStrike, Roundhouse, Quickness) were not cast in a real run here.
5. **Harness gaps (not game bugs).** (a) `--player-to ZONE_NAME@FRAME` with a trigger-zone name did not move the hero (r05); coordinates (`x,y,z@frame`) work. (b) The `--audio-table` path in the copied template was wrong (`audio-assets\data\sounds\sounds_pyarray.bin`); the file is `audio-assets\sounds_pyarray.bin`. Fixed in the runner (section 4). Runs before the fix (r01 to r06) had no audio, so no sound evidence exists for them.
6. **Rogue run is slow and fragile.** r02 (2,400 frames, SKIP presses at 8 to 20 s) hit the 300 s timeout while still on the main menu. The runs that worked used 1,500 to 4,200 frames with 400 to 800 s timeouts. Input timing decides whether the menu or the intro is reached.

## 3. Test aids used

- `--player-to x,y,z@frame` (teleport the hero): r05 (failed with a zone name), r06 (worked with coordinates). This is a test aid, not the production path. Walking was not tested.
- Real keys only otherwise: Enter (13) for SKIP, Space (32) for attack, Tab (9) for target. No `--test-player-hp`, no `--test-xp`, no god mode.

## 4. Files and changes

- Code changes on the branch: none so far (the base is unchanged). The CTest baseline is the final result.
- Added: `coordination/v21.8/act1rogue-report.md` (this file), `coordination/v21.8/act1rogue-run-rogue.ps1` (the runner; its audio path is `$audio\sounds_pyarray.bin`).
- Outside the repo: `C:/dh218/act1rogue-build.sh`, `C:/dh218/act1rogue-full.sh`, `C:/dh218/act1rogue/tools/run-rogue.ps1`, cases in `C:/dh218/act1rogue/cases/` (r01 to r07, k01).
- Frames viewed for this report: r01 `f1300.png` (intro cinematic, SKIP), r03 `f1700.png` (Rogue with two daggers, hotbar, potion 5), r04 `f2600.png` (dual-dagger pose), r06 `f2600.png` (hero after the intro teleport, dagger flash, no lizard in view).

## 5. Tests

- Build: `dh-foundation` and all tests built from `R:/port/windows-foundation` (`act1rogue-fullbuild.log`).
- Full CTest: **100% passed, 202 of 202** (`C:/dh218/act1rogue-ctest-baseline.log`, 522 s). Nothing changed in the source, so this is the final count. Note: the mudlord3 report recorded 197/199 on its base with two pre-existing failures (`original_melee_bindings` count drift and the Rogue session identity in `runtime_skill_activation_session_v1`); neither failed in this worktree.

## 6. Next steps (in order)

1. Find a lizard position (the `LizardMan_Intro` or `Swamp_LizadMan_Type1` spawn near the start or along the path) and run Tab + Space until `Damage` and `dead=1` appear. This settles blocker 1.
2. Decide blocker 2 from the code: does `combat_session.cpp`'s animation-only guard apply to the player? Test with a unit test before any change.
3. Then run the segments in order: Witch cave (`afterwitch` path), Mud Lord (copy the mudlord3 `run-c-follow300.ps1` with the Rogue profile), Darkwood load.

Status of the commit: see the git log on `v21.8-act1rogue` (commit made after this report).
