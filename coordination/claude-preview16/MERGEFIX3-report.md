# MERGEFIX3 report (Preview 16): finish merge of p15/patch (Preview 15.2 fixes B053-B067) into p16/integrate

Worktree `DH_wt/p16int`, branch `p16/integrate`. No abort, no branch switch, no push, other worktrees untouched.
Build `DH_wt/build-p16int/dh-foundation.exe`.

## Commits
- `55a8a810` Merge p15/patch (Preview 15.2 fixes) into p16/integrate (conflict resolution below).
- This report (separate commit).

## Conflict resolution
- `features/character_menu/export_art.py` (2 hunks): kept the p15 side in both. `solid_hit_styles(tag,at,ident,shape_code=2)`
  and the hit-only solid style `kind 0x40, bitmap_id 1`. The HEAD `kind 0` variant is the one the QUESTUI guard
  (c3560c22) blames for dropping solid batches. The generated art was NOT regenerated (hand-spliced union unchanged).
- `main.cpp` (6 hunks):
  1. Scripted move: HEAD `MoveSegmentV1` + `--move-from-frame` kept (P16 walk checks use it). The p15 tuple-based
     `--move-segment` branch (auto-merged, duplicate) removed.
  2. Pre-update block: HEAD despawn owner kept, `FramePacer` (B066) added after it.
  3. Move application: HEAD form kept (honours `moveFromFrame` and `moveSegments` struct).
  4. Combat update: HEAD SPACEBTN block kept; the B066 `DH_PROBE("combatSession.update")` wrap and the
     `FramePerf` sim_update mark added.
  5. Pickup: B063 walk-over (`WorldItemContactTrackerV1::begin_contacts`, moving gate, `runWorldItemPickup("walkover")`)
     kept. No E/Space pickup key exists in the merged code. `--pickup-frame` stays as a scripted test hook.
  6. Map visits block (HEAD) kept; `FramePerf fx_prepare` mark added.
- Duplicate walk-over removed: P16 CONTEXT had its own `WorldItemContactTrackerV1` (`features/loot/world_item_contact_v1.*`,
  same class name and namespace as B063's in `world_drop_rules_v1.hpp`). Both implemented the same walk-over contact, so the
  P16 version, its include, its declaration, its CMake entry and its test file are deleted (recoverable from `e2f03f03`).
  B063's tracker (shipped, with its ctest block) is the one kept. Space stays the P16 context dispatcher (containers, NPC
  talk, enemy priority); the dispatcher does not pick up items.
- `CMakeLists.txt`: the P16 `world_item_contact_v1.cpp` entry removed.

## Implementation check
- Build `p14_build.ps1 -Name p16int -Jobs 6` exit 0 (first try; no compile fixes needed).
- ctest (direct run, llvm-mingw bin on PATH): 139 of 140 pass. Only `session_skill_binding` fails (allowed).
- `.local-inputs` junction intact (no real directory created).

## Integrated runtime verification (real EXE, quiet_run hidden and silent, rc3 package)
Args, logs, summaries: `DH_sc/.local-inputs/claude-preview16/mergefix3/` (per job folder). Frames as PNG: `.../mergefix3/png/`.

| Item | Run | Evidence |
|---|---|---|
| Walk-over pickup, no key | `walk` (B063 seed-1234 kill, walk segments 165-195, 225) | `World item target frame=148 item=1 id=ClothGloves01`; `World item pickup frame=181 ... reason=walkover picked=1 stacks=4->5` |
| Space does not pick up | `nopick` (same fight, walk segments removed, stands still by the item; Space at 200 and 230 plus the run's own presses) | Items still on the ground (`World item draws frame=249 count=2 store=2`); no `World item pickup` line; Space at 200 = `Context button ... type=8 actor=1` (enemy) |
| Space opens chest | `space-inrange` (player 100 units from `_prim_OpenableContainer_2`, Space 20:3) | `Context container ... status=accepted state=2->3 frame=20`; `Container opened ... loot=227`; `loot ... status=ok` |
| Space breaks barrel | `space-barrel` (player 120 units from destructible) | `accepted state=2->3`; `Container opened ... loot=9`; `status=ok` |
| Enemy priority | `space-enemy` (enemy and barrel in range) | `Context button frame=20 ... type=8 use=1 actor=1`; no container line |
| Boot title | `boot` (startup.args, press at 6 s, capture at 4 s) | Frame `png/boot-title.png` viewed: title key art, "Touch the screen to continue" |
| Loading art | `boot2`, `load3` (press, save, `--loading-capture`) | NOT verified: the loading screen was never entered in these runs (main menu idles) |
| Equipment page | `equip` (swamp start, `--equipment-page-frame 60`) | `png/equip.png` viewed: 2nd tab selected, equipment slots and character |
| Level-up placeholder column | `levelup150`/`176` (character-xp-high save, level 2 at frame 143, `fx=135:level_up:played`), `lu153`, `lu160` | `png/lu153.png` viewed: white column behind the player. Frames 150 and 176 do not show it clearly |
| Quest Log tab | `quest` (debug accept row 53, kill 8 Moths, page frame 150) | `png/quest.png` viewed: Quest Journal, 5th tab highlighted, "Prison Break" and "COMPLETED: Bogwalker's Delight 2" |
| Map tab | `map` (page frame 150) | `png/map.png` viewed: 6th tab highlighted, "The Boglands", map room, Show legend / Reset zoom. Log: `Map room visited ... modules=1`, `Character menu Map selected frame=150` |
| Opening plays | `open520`, `open1500` (`base-prod3` + `GameStartOnly`, auto-tap 300) | `png/open520.png` viewed: SKIP and the "Movement Tutorial" caption box over the scene. `png/open1500.png` viewed: in-game HUD. Log captions: "The Boglands - Ancient Prison" (frame 2), "Movement Tutorial" (307, 391, 475); script commands through frame 1440; exit 0 |

Not checked: the full intro end (OPENING2 expected the end near frame 3946; this batch stopped at 1500); the Space pressed-state art; the
chest-tutorial zone run; the Quest Log completed-row detail text. The `walk`, `nopick`, `space-barrel` and `space-enemy` frames were not
viewed; the verdicts come from their logs.

## Notes
- `startup.args` (rc3) uses relative `--assets assets` and `--menu-assets assets`. They resolve against the args file directory, so
  configs must sit in the package root, or the asset paths must be made absolute (done for these runs). Not a merge bug.
- Boot-only runs (start-mode menu) never exit on their own; quiet_run times them out. Expected, not a failure.

## Gaps (carried over)
1. Loading screen art not captured in this merge (see table).
2. Level-up column is visible only at some frames of the FX window (about frame 153); not checked at every frame.
3. Map: grey bar over the parchment (MERGEFIX report gap 4) unchanged.

## Placeholders
None added by this merge. P16 duplicate tracker removed (not a placeholder).

## Package files required
rc3 package (`windows-source-clock-v19-preview-15-rc3`) and the `quests-run/assets` quest table, as in MERGEFIX/QUESTUI reports.

## Verifier script
`powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16int -Jobs 6 -Test`, then
`port/windows-foundation/tools/quiet_run.ps1 -JobsFile .local-inputs/claude-preview16/mergefix3/jobs.json -Parallel 4`
(job generator: `DH_wt/mergefix3_tmp/mkjobs.py`, scratch outside the repo).
