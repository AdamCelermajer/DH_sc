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
