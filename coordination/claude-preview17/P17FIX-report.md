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
