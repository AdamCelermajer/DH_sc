# P16 MERGEFIX report (merge of p16/questui into p16/integrate, worktree DH_wt/p16int)

Scope: finish the in-progress merge of `p16/questui` into `p16/integrate` (HEAD e6877ba6, which already had
`p16/map`). Both features must survive: Quest Log tab and Map tab, distinct and reachable. Nothing pushed,
no branch switch, no abort, other worktrees untouched.

## Commits
- `64641453` Merge p16/questui into p16/integrate (conflict resolution below).
- Follow-up commit (this report + one added check in character_menu_tests.cpp).

## Tab order (decision)
Quest Log = 5th tab, Map = 6th tab: `Tab { stats, equipment, skills, faery, quest, map }`.
Evidence: the authored CharacterMenuTabs buttons sit at x 432.4 (btnQuestLogTab) and 484.3 (btnMapTab) in the
sheet space (`original_art.cpp` hit zones); QUESTUI-report says Quest is the 5th of six tabs; MAP-report says Map is the
6th tab (icon 6). Both captured frames show the same order (see Runtime verification). IDA was not re-read for the
tab order in this session; the decision rests on the authored button positions and the two reports.

## Conflict resolution (per file)
- `CMakeLists.txt` (1 hunk, whole-file in the raw diff): the questui side has CR/LF-mixed endings. Ignoring CR, its
  only change is 11 added lines (quest runtime sources, the quest banner presenter test). Three-way merge with
  normalized endings gave a clean result; both sides' entries kept (`foundation_map_visit`, `foundation_runtime_quests`).
- `features/character_menu/character_menu.hpp`: enum union `Tab {stats,equipment,skills,faery,quest,map}`,
  `Action {..., faery, quest, map, map_legend, map_reset_zoom, close}`.
- `character_menu.cpp` (2 hunks): select() accepts quest and map; release() maps `Action::quest` and `Action::map`.
- `source_composition.hpp` (3 hunks): pages array size 6, slot 4 = quest, slot 5 = map (fallback index 6 is out of
  range, as before); action switch handles quest, map, and the map controls.
- `original_art.cpp` (1 hunk): `art4` stays the Map page (HEAD); the questui quest chrome is renamed `art5`;
  `original_menu_art` maps quest to art5 and map to art4; the hit-zone list has stats, equipment, skills, faery,
  quest (x 369-432), map (x 421-484), close, map_legend, map_reset_zoom.
- `export_art.py` (3 hunks): the HEAD (map) generator structure is kept, with the quest variant added: panels =
  [sheet, inventory, skills, None(faery), MapSheet, None(quest)] so the indices match art3/art4/art5; the tab-name
  tuple, zone list and `original_menu_art` match `original_art.cpp`. The questui raise-SystemExit regeneration guard
  is kept (regeneration stays disabled; the committed art is the hand-spliced union).
- `main.cpp` (3 hunks): both sides' includes, options (`--map-page-frame`, `--map-legend`, `--map-zoom`,
  `--quest-page-frame`) kept. Also fixed a questui typo `'NL'` (multi-character literal) to `'\n'` in the quest
  reload diagnostic.

## Implementation check
- Build: `p14_build.ps1 -Name p16int` exit 0 (dh-foundation.exe built from the merge commit).
- Tests: ctest 129/130 with the llvm-mingw bin on PATH. The only failure is `session_skill_binding` (known junction
  issue). Without the toolchain bin on PATH (plain bash ctest), `winmm_pump_priority_v1` and
  `container_open_script_v1` exit 0xc0000135 (DLL not found); with PATH set they pass. Environment, not code.
- `character_menu` (extended in the follow-up commit): the Quest Log tab release selects `Tab::quest` through its
  own hit zone; the Map tab release (existing check) selects `Tab::map`, so the two stay distinct. PASS.
- `.local-inputs` junction still in place (a symbolic link to DH_sc/.local-inputs); no real directory created.

## Runtime verification (real EXE, quiet_run hidden desktop, 2 parallel jobs, both exit 0)
Build: `DH_wt/build-p16int/dh-foundation.exe`. Args and jobs: `.local-inputs/claude-preview16/mergefix/`
(`map.args`, `quest.args`, `jobs.json`, `summary.json`); logs and frames in `.../mergefix/out/`.
- Quest job (`quest.args`, fresh start, debug accept row 53 at frame 60, debug kill 8 Moths at frame 100, page
  frame 150, 220 frames): log `Quest banner kind=QUEST COMPLETED row=53 xp=20 gold=150`, then
  `Character menu Quest Log selected frame=150`. Frame `out/quest.png` (direct look): "Quest Journal" header, the 5th
  tab highlighted in red, "Quest Log" heading, an Assigned row "Prison Break" (selected, bronze bar), a "COMPLETED"
  header, the right pane with no visible text. Matches the QUESTUI-report run.
- Map job (`map.args`, fresh start, map page frame 150, 170 frames): log `Map room visited frame=0 ... modules=0 total=1`,
  `Map room zones ... modules=9`, `Character menu Map selected frame=150 ... zones=9`. Frame `out/map.png` (direct
  look): "Map" title, the 6th tab highlighted in red, the parchment over the map rectangle with one dark-slate room
  diamond and the cyan player icon at its bottom edge, "Show legend" and "Reset zoom" buttons at the bottom, back arrow.
  A translucent grey bar crosses the top of the parchment; I did not identify its source in this session.

## Gaps (carried over, not changed here)
1. Quest Log: Completed rows, detail text, SIDE QUEST tag and MAKE ACTIVE are not visible in the captured frame.
2. Map: level title not drawn; "Unexplored area" caption has no symbol; only the local-player marker is produced.
3. Hit zones: quest (x 369-432) and map (x 421-484) overlap by about 11 px, as the existing tab zones do; in the
   overlap the later zone (map) answers. Not checked against the original hit shapes.
4. The grey bar on the Map parchment (see above) is unexplained.
5. `export_art.py` regeneration stays disabled (shared hit-only style reader drops solid batches); the art is the
   hand-spliced union. Exporter drift (`tools/export_hud_geometry.py`) is unchanged.

## Placeholders
None added in this merge.

## Package files required
Unchanged from MAP-report and QUESTUI-report: quest table `v2quests_pyarray.bin` / `...names.bin` under
`original-cache/data/pydata/` (quests-run/assets for the quest job); Swamp rc3 assets for the map job.

## Verifier script
`powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16int -Test`, then
`tools/quiet_run.ps1 -JobsFile .local-inputs/claude-preview16/mergefix/jobs.json -Parallel 2`. Expected: both exit 0;
`Character menu Quest Log selected frame=150`; `Character menu Map selected frame=150`; frames as described above.
