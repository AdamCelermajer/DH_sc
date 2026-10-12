# B157: cave level-up after a profile restore (XP reset, no level, no FX, no points)

Owner: B157 Haiku session. Worktree `v217-levelup`, branch `v21.7-levelup` (base `b6b4d27a`). Scratch and runs are gitignored under `.local-inputs/v21.7/`.

## Summary

- Reproduced on the production path with a level-2 profile (`seeds/slot1`) through the real menu Continue and the real E transition (and, as a control, through a Continue into the cave with no transition). The first level-up after the session start silently reset XP to 0 and left the level at 2, with no level-up sound, no FX, and no attribute/skill points.
- Root cause (two defects in the Windows port, both reproduced in logs):
  1. A restored profile level is stored in the wrong property layer (`base` instead of `saved`), so the first LevelUp reloads the row, adds one level, and resolves to the same level.
  2. The XP carry above the threshold is computed after XP has been reset to 0, so XP is lost on every level-up.
- Fixed in `player_profile_properties.cpp` and `features/loot/runtime_death_rewards_v1.cpp`. Unit tests added. Full CTest: 182/182 pass. Runtime after-fix: the level-up happens on the first attempt, with FX, sound, points, and carry.
- Transition is not the trigger. The same failure happens with a Continue into the cave with no transition (R8b). The trigger is a profile restored at level 2 or higher.

## 1. Investigation (original behaviour), before implementing

**Visual evidence** (`.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`; ffmpeg 1 fps contact sheets in `.local-inputs/v21.7/ref/`):
- 6:06 to 6:14 (the user named 6:08 to 6:12): inventory screens only. I did not find a level-up in that window, so the user's timestamp is not confirmed by footage.
- 12:58 to 13:06: the Stats page reads `Warrior Lvl: 3`, `EXP: 45 / 300`, `Points left 2`, and the points go 2, 1, 0 as Strength is raised. The Skills page reads `Skill points left 2`. This is direct observation.
- Inference: a level-up to level 3 grants 2 attribute points and 2 skill points, and the level-3 to 4 threshold is 300 XP. Our port matches these values (see section 2).
- Not observed: the level-up light/FX frame and the exact frame of the level change. The footage sampled here does not show them.

**Logic evidence** (existing recovered-source notes and code; not re-derived from IDA in this session):
- `Character::LevelUp` (IDA 0x3beb88, see `port/level-world/player_progression_v1.cpp` comments and `coordination/v20.1/B106-report.md`): one level increment, then the carry above the threshold is added back (`player_progression_v1.cpp` `level_up()`, `progression_give_xp`). The carry is computed by the caller before LevelUp. LevelUp plays the `PlayerLevelUp` sound (B090) and FX set 135.
- Level model used by both the host reproduction and the Windows port (`runtime_death_rewards_v1.cpp` `level_up()`): `base` reloads from the CharacterDesign row on every LevelUp (level 1 for the Knight, `base[19]=256`), and each gained level is +256 in the `saved` layer.
- Threshold table: XP to next level is `resolved[34]`; the port gives 200 XP for level 2 to 3 (`51200`), 300 for level 3 to 4 (`76800`), and 400 for level 4 to 5 (`102400`).

**Expected behaviour to preserve:** crossing the threshold raises the level by one, plays the level-up sound and FX once, grants the attribute and skill points for that level, keeps the XP above the threshold, and works after any session start or restore.

**Uncertainties:** the per-level point grant and the carry rule come from the port's model and the level-3 screen; not separately recovered in this session. Whether the original's level-up FX appears on the Stats page at the same frame is not verified.

## 2. Reproduction (before), production path

Harness: hidden private desktop, `quiet_click_sync.ps1` (copied from the v21.4 verify tools), `run-r.ps1` in `.local-inputs/v21.7/tools/`. Saves are copies of `.local-inputs/v21.4/verify/seeds/*`. Assets read from `.local-inputs/windows-source-clock-v21.7`.

- Level-2 profile: `seeds/slot1` (profile `ada`, `stats.level=2`, XP raw 17408 = 68, skill points 1, HP max 169.195).
- Level-1 control: `seeds/presave` (level 1, XP 6).
- Transition: menu Continue (`--menu-actions`), then `--player-to _prim_ExitLevelZone@120` and `--transition-key 180` (real E, scripted through SemanticInput). Continue for `slot1` lands in the cave (B143 resumes the profile level); E out to the swamp, then `--player-to _prim_ExitLevelZone@420`, `--transition-key 480` (E) back to the cave (R4 path, cave arrival after a Swamp E travel).
- XP: not from real kills. Real Tab/Space attacks in these runs produced 0 to 2 kills and never reached the threshold within the run budget (R1b: 2 kills, 6 to 22 XP; R4: 41 combat text, 0 kills; R5: `--kill-test` marks the monster dead but awards no XP in this build). XP is granted by a new TEST-ONLY aid, `--test-xp FRAME:AMOUNT`, which calls `RuntimeSessionDeathRewardsV1::award_experience`, the same reward path quest rewards use (`award_player_xp`, then `level_up`, then `on_level_up` for FX and sound).
- Logs: `Test XP award frame=... amount=... ok=... xp=... level=... statPoints=... skillPoints=... resolvedLevelRaw=... resolvedXpRaw=... resolvedXpNeededRaw=...`, `Level up sound LevelUp ...`, `Level up presentation frame=... level=... fx=135:level_up:played|failed`.

Before results, pre-fix logic (`R7b`, E path, slot1; `R8b`, Continue into cave, slot1; `R9`, E path, presave level 1; with `R7c`, a trace build of the same logic):

| Run | Award (frame, amount) | XP before to after | Level | Sound/FX | Points (stat/skill) |
|---|---|---|---|---|---|
| R7b (E, level 2) | 940 (+60, XP 188 to 248, threshold 200) | 248 to **0** | **2** (unchanged) | none | unchanged, 0/1 |
| R7b (E, level 2) | 1180 (+60, XP 180 to 240) | 240 to **0** | 3 | played | 2/2 |
| R8b (Continue cave, no E, level 2) | 940 (+60, XP 188 to 248) | 248 to **0** | **2** (unchanged) | none | unchanged, 0/1 |
| R9 (E, presave level 1) | 880 (XP 82 to 142, threshold 100) | 142 to **0** | 1 to 2 | played | +2/+1 |
| R9 (E, presave level 1) | 1120 | to **0** | 2 to 3 | played | +2/+1 |

R7c trace of the first failed attempt (R7c `B157 level_up` lines, pre-fix):
```
start           base19=512 saved19=0   gear19=0 resolved19=512 resolved33=63488 resolved34=51200
after-prefix    base19=512 saved19=256 gear19=0 resolved19=768 resolved33=0     resolved34=51200
after-row-reload base19=256 saved19=256 gear19=0 resolved19=768 resolved33=0     resolved34=51200
before-resolve  base19=256 saved19=256 gear19=0 resolved19=512 resolved33=0     resolved34=51200
after-resolve   base19=256 saved19=256 gear19=0 resolved19=512 resolved33=0     resolved34=51200
```
(`start` is XP 248 raw-scaled as 63488 = 248 XP; the carry was 48 but is lost.)

R9 (level 1, control) takes the same route and levels on the first attempt: `start base19=256 saved19=0 resolved19=256`, after-prefix `saved19=256`, `resolved19=512`.

## 3. Root cause

**A. Restored level stored in the wrong property layer.**
- `port/windows-foundation/original_combat_properties.cpp` (line ~43) and `original_actor_properties.cpp` (line ~57) write the `level_raw` option into `sheets.base[19]`. The player's profile level is passed this way (`propertyOptions.level_raw = state.stats.level*256`, `main.cpp`).
- `port/windows-foundation/player_profile_properties.cpp` projected the profile by keeping that value in base: `candidate.sheets.base[19]=prepared.sheets.base[19]`.
- The LevelUp model needs base = row level and the gained levels in `saved`. A restored level-2 hero had base 512 and saved 0. The first LevelUp reloads base from the row (256), adds +256 to saved (0 to 256), and recalculates to 512. The level is unchanged, nothing is announced, and no points are granted. The write-back makes the layout consistent, which is why the next LevelUp succeeds (R7b at 1180).
- A fresh character (level 1 with base = row) and a level-1 restore (R9) are consistent from the start, so they level on the first attempt.

**B. XP carry lost on every LevelUp.**
- `runtime_death_rewards_v1.cpp` `level_up()` computed `carry = resolved[33] - resolved[34]` after the prefix had set XP to 0 (`dh2_property_set_int(&view,33,0)`). The result is always negative, so XP after LevelUp is 0. The host reproduction (`player_progression_v1.cpp`) computes the carry before LevelUp.

**Why the user saw "after the transition".** The restore is the trigger, not the E travel. The Continue path (R8b) fails the same way. The user's session was a restored level-2 profile.

**Not reproduced: the user's third symptom** ("the next level-up produced nothing, still level 2, no attribute or skill point"). In the pre-fix runs, the first silent attempt reset XP to 0, and the next LevelUp after enough XP worked (R7b at 1180). A possible explanation is that the reset made the next visible LevelUp need a full threshold again. This is not verified.

## 4. Changes (worktree branch `v21.7-levelup`)

- `port/windows-foundation/player_profile_properties.cpp`: the projection stores the restored level as the delta in `saved` (`saved[19] = resolved[19] - authored[19]`), keeping `base[19]` at the CharacterDesign row level. It uses `resolved[19]` (what the validator checks) so re-projecting a projected sheet keeps the level.
- `port/windows-foundation/features/loot/runtime_death_rewards_v1.cpp`: `level_up()` reads the carry before the prefix reset.
- `port/windows-foundation/tests/player_profile_properties_tests.cpp`: new restored-level-2 block (base = row, saved = 256, resolved = 512, XP kept, repeat projection unchanged).
- `port/windows-foundation/features/loot/runtime_death_rewards_v1_tests.cpp`: the level-up fixture stages XP 10 above the threshold and asserts the carry survives (`10 <= XP < new threshold`).
- `port/windows-foundation/main.cpp`: TEST-ONLY `--test-xp FRAME:AMOUNT` (option field, parser, one per-frame block next to the `--kill-test` loop, `Test XP award ...` log line). Inert unless passed; not used in any packaged args. This is the only `main.cpp` change.

## 5. Tests

- Focused: `player_profile_properties_tests` prints four PASS lines including the new level-2 check (`KnightPlayerBase restored level-2 base/saved delta PASS`). `runtime_death_rewards_v1_tests` exits 0 with the carry assertion.
- Full CTest (`V:/.local-inputs/v21.7/build`, Release, llvm-mingw clang++ 23.1.3, CMake 3.22.1 + Ninja, `V:` subst of the worktree): **`100% tests passed, 0 tests failed out of 182`** (log `.local-inputs/v21.7/ctest-final.log`, run from PowerShell).
- Environment notes for the record: earlier runs failed for missing `.local-inputs` folders (the worktree had none) and `0xc0000135` when test exes were launched from Git Bash. I linked the required inputs as junctions to the primary's read-only folders, and the final run was started from PowerShell. The junctions are removed with `rmdir` before the worktree is deleted (see section 8).

## 6. Runtime verification

After fix (same production path as the before runs; the after-fix binary has the same two source files as the fixed tree):

| Run | Award | XP before to after | Level | Sound/FX | Points (stat/skill) |
|---|---|---|---|---|---|
| R10 (E, level 2) | 940 (+60, XP 248 threshold 200) | 248 to **48** | **3** on first attempt | played (`fx=135:level_up:played`) | 2/2 |
| R10 (E, level 2) | 1240 (+60, XP 348 threshold 300) | 348 to **48** | 4 | played | 4/3 |
| R11 (Continue cave, level 2) | 940 | 248 to **48** | **3** on first attempt | played | 2/2 |

The before and after for the first attempt is the same award at frame 940 (R7b/R8b show XP 0 and level 2; R10/R11 show XP 48 and level 3).

**Frames (Skills page, full-size PPM read as PNG, same run recipe; `--skills-page-frame 960`, `--capture-frames 970`, about 30 frames after the first attempt at 940):**

| Build | Run | Frame 970 Skills page, "Skill points left" | Level-gated skill icons |
|---|---|---|---|
| Pre-fix (`frozen/prefix.exe`) | R14 (E path, slot1) | **1** (level 2; XP reset to 0 by the silent attempt) | still locked (chain overlays) |
| Post-fix (`build/dh-foundation.exe`) | R15 (E path, slot1) | **2** (level 3 at 940; FX/sound/points granted) | unlocked |

Images: `.local-inputs/v21.7/cases/R14-prefix-970/f970.png` and `R15-postfix-970/f970.png`. Logs for the same runs: R14 `Test XP award frame=940 ... xp=0 level=2`; R15 `Test XP award frame=940 ... xp=48 level=3 statPoints=2 skillPoints=2`. An earlier pair captured at frame 1340 (`R12`, `R13`) shows the same skill point difference at a later time (pre-fix 2 after the delayed level-up at 1180, post-fix 2).

## 7. Remaining gaps (honest)

- Real-input kills did not reach the threshold, so XP came from `--test-xp` through the quest reward path (same award function, not the kill path). The kill-to-XP path itself was not exercised at runtime in this session; it is covered by the existing unit test for death-reward level-up.
- The user's third symptom (the next level-up gives nothing) is not reproduced; see section 3.
- Original footage did not show the level-up moment in the sampled windows (6:06 to 6:14 inventory; 12:58 to 13:06 Stats). The level-up FX/sound against the original is not visually verified.
- `Level up presentation ... actor=18446744073709551615`: the player id passed to the presentation callback prints as the invalid id, but the FX plays through the registered anchor. Not investigated further.
- Level-1 post-fix runtime was not repeated (the unit test and the full CTest cover the level-1 projection; R9 is pre-fix only).
- Direct-launch (`Play-swamp`) and the Stats page capture were not run.

## 8. Artifacts and cleanup

- Cases: `.local-inputs/v21.7/cases/` (R1 to R13; logs `run.log`, `injector.txt`, `launcher.txt`, `physical-inputs.txt`, `run.args`, `exe-sha256.txt`).
- Frozen EXEs: `.local-inputs/v21.7/frozen/baseline-head.exe` (HEAD logic, no `--test-xp`; SHA256 `db4a5b21...`), `frozen/prefix.exe` (pre-fix logic + `--test-xp`; SHA256 `a1b8a765...`), the post-fix build (`build/dh-foundation.exe`; SHA256 `e0760...` at R10/R11, `24b06f41...` after the final relink of the same sources; the difference is the PE link timestamp, and the two source files were verified identical with `diff`).
- Reference contact sheets: `.local-inputs/v21.7/ref/levelup-366-374.png`, `levelup-778-786.png`.
- Junctions created in the worktree's gitignored `.local-inputs` (to primary read-only inputs): `design-settings`, `player-loot-v7`, `windows-shared-assets`, `claude-preview15`, `claude-preview16`, `v20`, `windows-source-clock-v20`, `windows-source-clock-v21.6`, `windows-source-clock-v19-preview-9`, `-12`, `-14-rc1`, `-15-rc3`, `-2`, `-3`, `windows-population-assets`, `windows-encounter-source`, `windows-melee-bindings`, `windows-equipment-assets`, `windows-camera-assets`, `windows-main-frontend-v1`, `windows-foundation-assets`, `windows-foundation-build`, `windows-skill-cast-session-test`, `actors`, `publication`, `assets-extra`, `audio-v34`, `items-discovery`, `loot-world-v8`, `character-*` (as listed by the tests), and others. Remove every junction with `cmd /c rmdir` (no `/s`) before deleting the worktree.
