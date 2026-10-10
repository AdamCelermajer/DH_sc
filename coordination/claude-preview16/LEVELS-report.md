# LEVELS report (Preview 16 generality gate)

Branch `p16/levels` (worktree `DH_wt/p16levels`), built in `DH_wt/build-p16levels`. Deliverable: `coordination/claude-preview16/LEVEL-COVERAGE.md` (+ `LEVEL-COVERAGE.json`, screenshots in `level-shots/`).

## Result in one paragraph

Every fixed map (`data/scene/*.mlx`, 39 live files; 30 `x*_backup.mlx` are not loaded anywhere) loads through the existing generic loaders and 33 of them render their authored geometry with authored characters from a quiet hidden run; 3 render a blank view (118, 120, 125); 3 stop with a named error (103, 117: module node absent from the module .bdae; 108: no authored modules). 37 procedural `.rule.xml` maps cannot run in the Windows EXE. The canonical port (`port/level-loader`) already has the rule reader and the layout generator that matches 70 original runs; wiring it into `dh-foundation` and assembling the tiles is a medium item (LEVEL-COVERAGE.md gap 7). No run crashed. The 39 fixed maps produce 695 population notices; the largest groups are missing character profiles (375) and unknown activation conditions (255). The generality gate is therefore partially met: geometry and population load for any fixed map with no code change; instantiation of most authored classes still needs the systems listed as P1 in LEVEL-COVERAGE.md.

## Investigation (evidence)

- Level data format: `.mlx` = XML `Level` with `GameObject` children (LevelConfig, Module). Each Module references a module `.bdae` (geometry) plus `.mgp`/`.mvp` XML (gameplay/visual declarations: Character, AnimatedDecor, Door, containers, zones, SpawnPoint, Dummy, link). Read through `actor_definitions.cpp` (`load_actor_definitions`) which walks the module XML recursively (same rules as the game loader).
- Procedural `.rule.xml`: `RootRule` tree of `ForceBlock` nodes referencing `<list random="true">` entries via `#list[index]`; `<elem name gameplay visual>` module candidates. The canonical port (`port/level-loader/procedural_*_v1.hpp`) already reads these files and runs the layout generator; `dh-foundation` does not compile it.
- Level table: `levels_pyarray.bin` (Android `data/pydata`) decodes with `game-data/level_tables.cpp` (51 rows). The row fields (`Hub`, `IsRandom`, `MonsterLvl*`, `LevelName`, `MapName`, `LevelFile`) do not carry an act number, so the report's act column is inferred from the file-id band and marked unverified.
- Swamp-specific data in the EXE path: see LEVEL-COVERAGE.md "Gaps" items 13-19 (all file:line).
- Confirmed the EntryPoint heuristic: `001_swamp` first `_prim_EntryPoint` = (1090.75,-212.202,258), equal to `swamp.args --position`, so the first authored EntryPoint is a usable start for coverage runs.

## Changes (small, additive, with unique anchors)

1. `port/windows-foundation/features/levels/level_inventory.cpp` (new) + CMake target `level_inventory` (anchor `# P16 LEVELS:` in `CMakeLists.txt`). Read-only inventory: per level geometry (`load_level`), authored declarations and classes (`load_actor_definitions`), asset presence in package and Android roots (same `data/iphone/` alias as `level_manifest.cpp`), SpawnPoint/EntryPoint bounds, level table name (`game-data/level_tables.cpp`), procedural tag counts. Outputs JSON and markdown.
2. `port/windows-foundation/features/levels/level_class_registry.{hpp,cpp}` (new) + CMake library `foundation_level_classes` linked into `dh-foundation` and `level_inventory`. Maps each authored gametype to a status (consumed / library_only / ported_elsewhere / unsupported) and logs each non-consumed class once per process: `Unsupported class <X> status=<s> declarations=<n> (logged once)`. Hook: `main.cpp`, one line after the population load (anchor `P16 LEVELS`), plus one include line.
3. `port/windows-foundation/original_camera_config.cpp`: `levelConfig(..., required)`; the asset-aware `OriginalGameplayCamera::load` now accepts a level without LevelConfig (debug maps) using the header's InitPost defaults and logs `Level notice: no LevelConfig; original InitPost camera defaults used (unverified for this level)` once. Previously it threw `Selected level has no LevelConfig` for 20 of 39 fixed maps. The behaviour for maps with a LevelConfig is unchanged (the strict path is kept for `decode_original_camera_config`).

No change to the save format, the combat, population policy or map logic. No Swamp constants were removed (none were blocking loads).

## Tests

- `p14_build.ps1 -Name p16levels` builds clean (exit 0) after each change; the final build is repeated with `-Test` (log: `DH_wt/build-p16levels-test.log`; see the result line at the end of this report).
- Inventory run: `levels=76 table_ok=1` (39 fixed, 37 procedural; 30 backups excluded).
- Quiet batches (hidden, silent, parallel 8): batch1 (first pass, before the camera fix), batch2 (after the camera fix and the class log), batch3 (EntryPoint positions; the one used for the report). Logs and PPMs: `.local-inputs/claude-preview16/runs/batch{1,2,3}/`. batch2 log example with the one-time class log: `runs/batch2/001_swamp.log`.

## Evidence (look at these)

- `coordination/claude-preview16/level-shots/sheet-A.png`, `sheet-B.png`, `sheet-C.png` (12 maps each, EntryPoint start, 30 frames). Looked at: 001 swamp (teal, NPC and enemies visible), 003 darkwood (forest, bandits), 005 infected village, 009a abbey, 015 red desert hub, 031 fire temple (lava), 036 underworld, 038/039 dark temple, 105 alpha test (ice cubes), 129 roottroll (ice island), 130 lighthouse emissary (wood platform). Blank: 118, 120, 125.
- `runs/batch3/summary.json`: 36 exit 0, 3 exit 1 (103, 108, 117), 0 timeouts.

## Package files required

Runtime: the level loader reads `data/scene`, `data/3d/modules/**` (mgp/mvp/bdae), `data/3d/characters`, `data/3d/animateddecors`, `data/3d/gameobjects`, `data/3d/textures`, `data/3d/light`, `data/3d/skybox`, `data/3d/fx`, `data/3d/projectiles`, `data/3d/props`, `data/3d/cinematic`, `data/3d/camera`, `data/pydata`, `scripts`. The Android copies of these directories are staged at `.local-inputs/assets-extra/android/data/` (253 MB, 6040 files, `cp -n`, listed by directory above; the package itself is unchanged). For the EXE runs the overlay `.local-inputs/claude-preview16/levels-root` (package `assets` + Android `files/data` under `original-cache/data` and `data`, hard links) was used. The actor profile file `actor-profiles-v2.xml` is still the 26-actor Act-1 export (see gap 1).

## Verifier script

1. Inventory: `level_inventory.exe --root <levels-root> --package <package assets> --android <Android files> --table <Android files/data/pydata> --out inventory.json --md inventory.md` (in `DH_wt/build-p16levels`).
2. Batch: `runs/batch3/jobs.json` (39 jobs) via `port/windows-foundation/tools/quiet_run.ps1 -JobsFile ... -Parallel 8 -Summary ...`. Expected: 36 exit 0, 3 exit 1 with the messages in the table; `Unsupported class` lines once per class per log; `Level notice: no LevelConfig` in 20 logs.
3. Report: `coordination/claude-preview16/level-coverage/make_level_coverage.sh <inventory.md> <batch3 dir> <out dir>`.
4. Look at `level-shots/sheet-*.png` against the expectations above.

## Open risks / not verified

- Captures are the first authored EntryPoint, not the original entry rule. The real start depends on transition data not yet decoded.
- The LevelConfig fallback (camera defaults) is an inference; it is logged and documented but not checked against original footage.
- Act numbers are inferred from file ids, not from data.
- Positions in the runs are not gameplay-verified: no movement, combat or triggers were exercised outside the existing movement/combat code path.
- 103/117 are treated as content mismatch (the node is absent from the module .bdae); not confirmed against the original editor files.
- The class status table in `level_class_registry.cpp` describes code that exists, not original behaviour. `library_only` entries are not wired into the level runtime.
- `quiet_run.ps1` left `dh-foundation.exe` processes running after some batches (stopped by PID; only my worktree's processes were touched). Worth a look by the runner owner.
- Not done: procedural tile assembly on the Windows side (item 7; the layout generator already exists in port/level-loader), profile library (item 1), condition table (item 2), spawn policy (item 3), trigger/door/container wiring (items 4-5). These are the P1 gaps the other Preview 16 agents need.
