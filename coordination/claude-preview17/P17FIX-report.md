# P17FIX report (Preview 17 fixes on the merged EXE)

Worktree `DH_wt/p17fix`, branch `p17/fix` (started from `p17/integrate`, commit `435d1e4e` = Script_EnterSafeZone / LeaveSafeZone non-blocking). Not pushed; no other worktree touched.

## Investigation
- (in progress) Review of `435d1e4e`: ok on read-through (kind 16/17 in the support switch and in the Execute switch; provider bound in CampaignHost; music switch via set_level_music_safe_zone).

## Implementation
- (pending)

## Isolated tests
- (pending)

## Integrated runtime verification (real EXE, quiet_run, rc3 package)
1. Regression check walk-over pickup / level-up (p16int vs p17fix): (pending)
2. Red door fire (`_prim_Swamp2Door_FIRE`): (pending)
3. Save/reload with doors in one run: (pending)
4. Quest tab frame (Assigned row): (pending)
5. ctest: (pending)

## Gaps
- (pending)

## Placeholders
- (pending)

## Package files required
- rc3 package (`windows-source-clock-v19-preview-15-rc3`).

## Verifier script
- (pending)

## Regression check 1 (walk-over pickup, level-up): findings so far
- Review of `435d1e4e`: no defect found (kinds 16/17 non-blocking; provider bound in CampaignHost; music switch via set_level_music_safe_zone).
- Same args on p16int (`build-p16int`) and p17fix: outcomes are deterministic per EXE (two repeats identical), but differ between the EXEs.
- Population: p16int `calls=20`, 34 visuals; p17fix `calls=21`, 35 visuals. The extra actor is `_prim_MothTemplate_13` (Condition unknown in p16, admitted by the CONDITIONS evaluator in p17). Its extra draw shifts the combat stream: hits/misses differ from frame 14 on, so the kill frame moves (148 -> 181 in some seeds) and the fight can end without a kill (levelup recipe: no kill by frame 150).
- Drops: on the kill, `Source death reward` shows `spawned=1` on p16 seed 1 and `spawned=0` on p17 seed 1. Drop-roll sample over combat seeds 1-20: p16int 7/19 kills drop, p17fix 2/18 (not significant at this size; see the drop-rate note below).
- Walk-over pickup proven on the merged EXE with combat seed 10 and seed 11 (no key): `World item pickup ... reason=walkover` (`Potion0` at frame 181, `PlateBoots01` at frame 182) in `DH_sc/.local-inputs/claude-preview17/p17fix/reg/sweep-walk-{10,11}/walk.log`.

## Regression check 1 result: expected population change, recipes re-derived
Evidence folder: `DH_sc/.local-inputs/claude-preview17/p17fix/reg/` (args, logs, `png/`).
- Same walk and level-up args on `build-p16int` and `build-p17fix`. Repeat runs of one EXE are identical (`p16int-walk-a/b`, `p17fix-walk-a/b`), so the difference is the EXE, not timing.
- Population: p16int `calls=20`, visuals 34, `_prim_MothTemplate_13` not instantiated (`Condition policy returned unknown`). p17fix `calls=21`, visuals 35, `MothTemplate_13` admitted by the CONDITIONS evaluator. That one extra draw shifts the combat stream: first hit/miss differs at frame 44, so the kill moves (148 or 181) or does not happen by the recipe's end (`levelup150` seed 19: no kill by 150).
- Recipe seed was the only thing wrong, not the code: on p17fix, seed 19 gives no kill by frame 150, but seeds 2, 3, 5, 6, 7, 8, 9, 11, 12 kill and present the level-up.
- Drops: the victim `_prim_Monster_3_05_313_356_14_33_13_179_13_38` has a loot table on both EXEs; `Source death reward ... selected= spawned=` now logs the roll (commit after 435d1e4e). Over combat seeds 1-20: p16int 7 of 19 kills dropped an item; p17fix 2 of 18 kills dropped (seeds 10, 11). Not statistically separable at this size (p about 0.07); recorded as an open point, not claimed as a regression.
- XP is delivered on both EXEs (`xp=8`, level 1 kill; the level-up save reaches `level=2`).

### Walk-over pickup (no key) on the merged EXE: PROVEN
- Recipe: `walk` args (`--move-segment 165:195:1,0`, `195:225:-1,0`), `--combat-seed 10`, no Space/E input for the pickup.
- Log `reg/sweep-walk-10/walk.log`: `Source death reward frame=148 ... selected=1 spawned=1`, then `World item pickup frame=181 item=1 id=Potion0 reason=walkover picked=1 stacks=4->5`.
- Seed 11 also: `PlateBoots01 reason=walkover` at frame 182 (`sweep-walk-11`).

### Level-up presentation (white placeholder column) on the merged EXE: PROVEN
- Recipe: `levelup150` args with `--combat-seed 2` (kill at 143, the `character-xp-high.save` fixture), capture at frame 163.
- Log `reg/lu-pl-s2-163/levelup.log`: `Level up presentation frame=143 ... fx=135:level_up:played text=LEVEL UP!`.
- Frame `reg/png/lu-pl-s2-163.png` (viewed): tall white column with bloom from above the hero to the feet, the `LEVELUP_PLACEHOLDER_COLUMN` window (white phase 100-430 ms after play; capture is +20 frames, about 320 ms).
- Seed 3 also presents (frame 111, `reg/lu-pl-s3-131`).

## Red door fire (task 2): partial, not fixed
Evidence: `DH_sc/.local-inputs/claude-preview17/p17fix/fire/` (`d-30.png`, `door2/d-120.png`, `f-60/120/200/240.png`); reference `claude-preview16/ref-wall-of-fire-0759.png`.
- Observed (direct): in the red door cutscene (`door2` args, `DH_FX_DEBUG=1`) the cutscene starts, the camera target `_prim_Waypoint_BlueDoor` resolves (`Camera named target ... found=1 ok=1`), and the line "That just leads deeper into the bogs, Player." plays (matches swamp.english 61). The thin red band on the floor at the left edge (`door2/d-30`) is the only flame visible; the reference wall of fire is not reproduced.
- Model facts (BRES strings, `go_door_fire_red.bdae`): clips `closed`, `opened`, `close`, `idleclosed`, `idleopened`, `open`; flame skin `_mesh_fire-mesh-skin` on `Bone1..4`; bones `_bone_01..03`; emitter `GL_PCloud01-emitter` with birth-rate track; material `Standard_20`/`Standard_202`, additive, `fx_doorfire.tga`.
- Runtime: the red door plays clip `idleclosed`, looped (sourceMs cycles 0..about 750 ms, `DH_FX_DEBUG` log). Violet door plays `open` clamped at its end. Log: `Animation channel skipped (target is not a scene node) type=28 target=GL_PCloud01-emitter`.
- Not decided: whether the 750 ms loop has the authored tall flame, whether bones are posed (rest vs animated), the birth-rate sample, and the colour. These need the BRES decoder; not done.
- Diagnostics added (no gameplay effect): `DH_FX_DEBUG=1` prints the embedded FX clip and sample time; `Camera named target` log in the named_target provider; `Embedded FX ... no active clip` notice.

## Remaining tasks, status
- (3) save/reload with doors in one run: NOT run this session.
- (4) Quest tab frame (Assigned row): NOT checked this session.
- (5) ctest (`ctest -j 6`, build-p17fix, llvm-mingw on PATH): 146 of 147 pass; only `session_skill_binding` fails (allowed). Log: `DH_sc/.local-inputs/claude-preview17/p17fix/ctest.txt`.
