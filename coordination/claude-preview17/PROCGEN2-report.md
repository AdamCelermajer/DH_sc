# PROCGEN2 report (Preview 17): procedural levels, second pass

Branch `p17/procgen2` (worktree `DH_wt/p17procgen2`, build `DH_wt/build-p17procgen2`), started from `p17/procgen` at 02d2611d. Not pushed. Builds with `p14_build.ps1 -Jobs 6`. The earlier pass is `coordination/claude-preview17/PROCGEN-report.md`.

Commits:
- 8b39e1fa: seed kept in a named v4 component (schema v5 removed).
- 54721257: layout locked to the original ARM oracle; 007 backup alias; probe and coverage diagnostics.
- a851c0ff: module Decor/AnimatedDecor (cave water) and the LevelConfig skybox for every level.
- baae8dd6: menu-start evidence, clearer source-scope error.
- Last commit: 121/122 folder message, decor/skybox tests, coverage table, evidence frames, this report.

## Result in one paragraph

The seed now lives in the character save as the named component `dh2.procgen.seed.v1` in the schema v4 component roster (no version bump; v5 reads are rejected; a pre-component v4 save loads with zero seeds). The generator is verified against the original ARM32 execution for the RNG (138 NextInt sequences, 139 between, 966 bounded, 135 hash) and for all 35 authored rules at seeds 0 and 1 (67 layouts plus 3 completed no-layout runs, every tile, the final RNG state and the call counters): 70 of 70 match. Module visuals now instantiate their Decor and AnimatedDecor children (the swamp cave water caustics are drawn; the witch cave grows from 10002 to 11406 triangles) and the LevelConfig skybox is loaded and drawn for fixed and generated levels. The menu start path runs the procedural witch cave once in the EXE (packaged startup, single-player recipe, fresh profile) and renders 100 frames with a clean shutdown. The fallbacks are explained: 017 and 018 and 027 are the original's own no-layout outcomes (the port reproduces the original's final state and counters on the oracle seeds), 121 and 122 are rejected by the original too, the 033 to 035 "14 to 22 triangles" were missing decor (not an asset limit), and 007's shipped backup has a different name that the fallback now finds. ctest: all pass except `session_skill_binding` (134 tests).

## 1. Save format (schema v4 component, no bump)

Investigation: `p17/transitions2` (b31b004f) keeps its entry record as an append-only tail component list (`CharacterComponent {id, bytes}`, `components` written after the visited list, `component_count` u32, read only if bytes remain). The v5 change on this branch had appended three u32 seeds after the visited list.

Implementation:
- `character_state.hpp/.cpp`, `save_store.cpp`: the transitions2 component hunks, applied verbatim (same text, so the two branches merge cleanly), schema stays 4.
- `features/procedural/procedural_level_v1.{hpp,cpp}`: `procedural_seed_component_id = "dh2.procgen.seed.v1"`, `saved_seeds()` (three little-endian u32, one per difficulty; a missing or malformed component reads as zeros), `store_saved_seeds()`; `session_seed` and `record_session_seed` use them.
- `tests/schema_v4_tests.cpp`, `tests/save_tests.cpp`: the base v4 layout tests now count the 4-byte component count (the tail is one byte longer); v5 is still rejected (schema 6 is the unknown-version case).

Tests (`procedural_level_v1`): round trip at schema 4 with the seed component, byte-exact re-save, a pre-component v4 file (count removed) loads with zero seeds, a schema 5 file is rejected and the destination is untouched, an unknown component is preserved next to the seed component, a malformed seed component reads as zero. `schema_v4_tests` and `save_tests` pass.

Seed source: `fresh_seed()` (time and counter) for a new profile; the original takes the realtime timer (`Application::LoadLevel`, savegame +92 + 4*difficulty, zero -> timer written back). Same role, different value (see gaps).

## 2. Floor water and skybox

Investigation (evidence):
- Reference: Part 1 at 850 to 945 s (`procgen2-shots/side-by-side-reference-878-vs-witch-decor-f99.png`, reference frame `reference-part1-878.png`): a shallow-water floor with bright caustics over stone, a high isometric camera; no visible sky in the enclosed cave.
- The witch cave module visuals (`data/3d/modules/swamp/mvp/*_swpcave_*_01.mvp`) declare `AnimatedDecor` `_anim_water_*` from `data/3D/AnimatedDecors/swamp_cave_watercaustics.bdae` (startanim idle, material `anim_caustics.tga`, additive, Standard_2 profile), plus `Decor` blood stains and props, and dust-mote effects.
- The port staged these declarations (`actor_definitions.cpp` visits mgp/mvp children) but never instantiated them ("Unsupported class AnimatedDecor ... ported_elsewhere" in `level_class_registry.cpp`); there was no water or caustic code.
- Skybox: the LevelConfig `skybox` XML attribute (fixed `.mlx` and the rule root copied into the generated LevelConfig; 019_03 and 029 declare `skybox_wind.bdae` on the rule) was never read for XML levels. The port's skybox reader is only the binary LevelConfig path.

Implementation:
- `level_manifest.{hpp,cpp}`: `ModulePlacement.visualPath` (the module's `mvp`); `decode_module_decor()` returns the Decor and AnimatedDecor children with a dae and xrefobject (visible="0" skipped) with transform = module placement x the declaration's own TRS; `decode_level_skybox()` reads the LevelConfig `skybox` attribute.
- `assembled_level.cpp`: after the module loop, each visible module's visual document is decoded and its decor merged through the same BRES decoder and `merge()` as module geometry (no room zone). The skybox is loaded as a whole scene into `OriginalScene::skybox` (a shared pointer, so the transition reload and the menu start carry it too). Missing visual, decor or skybox files are named notices.
- `main.cpp`: `bindMaterials` binds the skybox textures; the frame draws the skybox before the render queue at the eye with depth writes off (`SourceMaterialPass.depthWrite=false`), so the level and actors draw over it.

Isolated tests (`procedural_level_v1_tests`): decor keeps AnimatedDecor/Decor with a dae, skips hidden, dae-less and other gametypes; decor placement is module x declaration translation (105,200,0 and 100,207,0); the LevelConfig skybox attribute is read; a level without LevelConfig has none.

Integrated verification (quiet, hidden, silent; `.local-inputs/claude-preview17/procgen2/`):
- Witch cave, seed 1234 (`witch-decor/run1.log`): `Decor instantiated: 15`, `Level triangles=11406` (was 10002), exit 0, 100 frames. Frames `procgen2-shots/witch-decor-f99.png`: the floor now shows the caustic water texture; the reference shows the same kind of caustic floor but brighter, with the camera higher. The caustic animation (startanim idle) is not played (gap).
- Wind temple, seed 1234 (`wind-sky/run1.log`): `Skybox loaded: data/3D/skybox/skybox_wind.bdae triangles=12`; `procgen2-shots/wind-temple-skybox-f99.png` shows the sky texture behind the level. Not compared with a reference frame (no wind-temple reference sampled).
- Fixed Swamp (`swamp-sky/run1.log`): skybox `skybox_swamp.bdae` loaded; decor 38 declarations; `Level triangles=30263` (was 24738 with the same assets before the decor change); `procgen2-shots/swamp-fixed-water-f99.png` shows the caustic water on the swamp floor.
- Voidmaze: decor is the reason for its geometry (see 4).

## 3. Layout fidelity (original ARM oracle)

Investigation: the canonical checkout (`generic-level-loader`) holds the executed-original receipts `reports/procedural-layout-original.json` (35 authored rules at seeds 0 and 1, plus fixtures; ARM32 execution of `RandomGenerator::Generate` and the rule traversal under unicorn) and `reports/procedural-random-differential.json` (the original `rnd::RandomGenerator` primitives). The RNG was also checked by reading IDA: `NextInt` `279470273 * (state+1) % 0xFFFFFFFB` (`00483a94`), `Hash` 5381/33 (`00483b14`), `GetInt` (`00483ac8`), `operator()` (`00483af8`), `Generate` sets the state to the seed (`00488134`), stage 7 of `Level::_LoadProcess` (`003f6990`).

Implementation: the port's generator is unchanged; `generate_layout()` returns a plain `LayoutRecord` through the same chain as `generate_level()`.

Locked (`procedural_layout_fidelity_tests`, ctest): `features/procedural/reference/original-layout-seeds-0-1.tsv` (607 rows exported by `export-layout-oracle.js`) and `original-rng-differential.tsv` (exported by `export-rng-oracle.js`). Checks: RNG `next` (first 16 values), `between` and `bounded` (value and state after the call), `hash`; layouts: success, final RNG state, all seven call counters, tile count, and every tile (source index, name, grid, exact height bits, list element, children). Result: RNG 1,378 rows; layouts 67 generated + 3 no-layout, 70 of 70 matched.

Witch cave against the reference video: the original oracle at seed 0 gives 7 tiles: deadend_s, corner_nw, corner_se, t_nse, deadend_n, straight_we, bossroom_w. Tile vocabulary and room types match the reference (corridors with water, corners and T junctions, a stone boss platform with chest and bogwomp). The first cave pass in the reference has an unknown seed, so exact layout is not comparable; the structure is consistent.

Spawn and exits (generated witch cave): spawn `203.611,557.532,37.06` (grounded at 100.41 after the floor snap); exits `_prim_ExitLevelZone_001` and `_prim_ExitLevelZone_42` to `001_swamp.mlx` entry 4 (the second gated by `IsAfter_Swamp_KillWitch2`, stub true).

## 4. Failures

- **017 (20/64) and 018 (58/64)**: `layout: generation produced no tiles`. The original generator returns no layout for these seeds too: the oracle has 017 seed 0 as a completed no-layout run (generated=false, final state 3146929004, 26 RNG calls, 33 placements, 33 unspawns, 15 rule calls) and the port returns exactly those values. The failing seeds for 017 and 018 are listed with `procedural_coverage <root> 64 <rule>` (018: seeds 15838, 118785, 348436, 403869, 411788, 506816). The port has no work cap that could produce this silently (`tick()` throws a named error after one million steps). Both rules ship backups (`x17_*`, `x18_*`), so the fallback is the designed path. Whether the original fails on exactly these other seeds is not verified (oracle seeds 0 and 1 only; 018 succeeds on both).
- **027 (0/64)**: the original fails on seeds 0 and 1 too (generated=false; final state 2499713547 and 2922999758, root attempt 1, 4 and 13 RNG calls). The rule forces a fixed chain (monument objective at the root, a T junction with an east path of length 4 that must reach the `#end` boss room, plus `#start` and `#path[3]` on the other branch), and the backtracking has no admissible layout for the icy blocks. It falls back to `x27_icy_cavern_02_backup.mlx` (present, renders). "Never" for the original is not verified beyond seeds 0 and 1.
- **121 and 122 (legacy)**: `folder` is on `RootRule`, and `RootRule` has no `name` attribute. The original `Rule::LoadFromXml` returns 0 without a `name`, so `LoadRuleFile` fails, and the backup (`x21_debug_test2_backup.mlx`, `x22_debug_test_backup.mlx`) does not exist; the original's stage-7 loop would retry a missing file. The port rejects them with a named error: `Rule ... has no folder on its rules element (legacy RootRule folder format)`. These are debug maps.
- **033 to 035 (voidmaze, 14 to 22 triangles)**: the layouts match the original (9, 11 and 7 tiles at seed 0). The triangles came from the missing decor: the tiles' geometry is in the Decor children of their module visuals (`void_maze` module nodes alone are 1 to 2 triangles each; the whole `void_maze.bdae` is 41 triangles). With decor instantiated: 13696, 16392 and 10548 triangles. Visual fidelity against a reference is not verified (no voidmaze frames sampled).
- **007 backup name**: the IDA stage-7 rule gives `x07_crypt_01_BACKUP.mlx` (the level's rule name with `x` in front, cut at the first `.`, plus `_BACKUP.mlx`); the shipped file is `x07_crypt_backup.mlx` (the only one of the 30 backups that drops the `_01`). Fix: `procedural_backup_candidates()` tries the IDA name and then the name without a trailing `_NN`; `resolve_procedural_backup()` returns the first present. The coverage table and the EXE fallback use it. 007 never needs the fallback (64/64 generate).

## 5. Menu start path, run once in the EXE

What it took (documented, the packaged `startup.args` plus these changes):
- `--level data/scene/002_swamp_witchcave.rule.xml` (the menu start defaults to Swamp only when no level is given).
- `--source-root-scopes data/scene/002_swamp_witchcave.rule.xml`: the packaged startup passes `original-cache/data/scene/001_swamp.mlx`, which builds the Swamp source root against the witch cave's population and fails (`matches 0 declaration hosts`). For a generated level the rule path is the source root (the generated document is served there), and it builds (`modules=7`). `--original-floor-motion` and `--original-heading-rotation` are kept.
- `--skip-boot`: without it the intro movie waits for a touch and the run never reaches the menu frames (quiet runs).
- `--menu-actions "menu_MainMenu.btn_MENU_SINGLE_PLAYER|text:Hero|menu_EnterName.buttons.btn_Accept|menu_SelectClass.btn_Confirm|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER"` (Knight; the old recipe's `btn_right` picked the Rogue).
- `--assets` and `--menu-assets` absolute (the packaged relative `assets` resolves against the working directory: `Asset root is not an existing directory: assets` in one run, and a silent stall in another).
- A run-local asset root (`procgen2/asset-root3`, hard links of `levels-root` minus `data/pydata/loot_table_pyarray.bin`): the overlay copy of the loot table does not match the menu creation owner's receipt (`Asset differs from the exact RuntimeCreationSourceOwner receipt`); the rc3 package does not ship that file. Junctions are rejected by the asset catalog (`Asset path escapes asset root`).

Run (`menu-final/run1.log`, exit 0): profile created in slot 0 (fresh seed), `Session start ... level=data/scene/002_swamp_witchcave.rule.xml`, `Procedural level generated rule=... seed=2653015743 tiles=7 modules=7 unplaced=0 signature=7730f0b6d49e1640`, `Decor instantiated: 15`, `Level triangles=11406`, exit zones `_prim_ExitLevelZone_001` and `_42` to Swamp, `Source root constructor scopes modules=7`, `Source floor diagnostic rooms=7 floors=10`, 100 frames, clean shutdown. Frame: `procgen2-shots/menu-start-procedural-f100.png` (the Knight in the generated cave, fresh profile HUD).

Control: the packaged rc3 EXE with its own startup and the same menu actions into Swamp completes (`rc3-start`, exit 0, `Source root scopes modules=9`); our build with the Swamp flags completes too (`cmp-ours`, `ov-ours`).

## Gaps (explicit)

1. Original layout beyond seeds 0 and 1 is not verified: no ARM oracle run is possible here (no Python interpreter; the canonical oracle needs it). 017 and 018 failing seeds and 027 "never" are the port's faithful outcomes on seeds 0 and 1, not verified for the rest.
2. Caustic animation (`startanim idle`, scrolling caustic texture) is not played; the water is the authored static pose with additive blending. The reference is brighter.
3. Camera: the generated witch cave uses the default follow camera, while the reference (and the fixed Swamp view) is a high isometric view. The generated LevelConfig carries no camera file; the original's camera source for this area is not established.
4. Skybox: witch cave declares none (the reference shows an enclosed cave, no sky), wind temple's sky is drawn but not compared with a reference frame; Swamp's skybox is loaded but not seen from the top-down camera.
5. Decor effects: `AnimatedDecor` dust motes (`swamp_caveentrance_effect.bdae`, 2-triangle quads) are drawn as static meshes; not verified visually. `Decor` blood stains are drawn (authored).
6. Seed: `fresh_seed()` (time and counter) instead of the realtime timer; the first seed of a new profile differs from the original's value.
7. Menu start: the packaged startup's Swamp-specific source-root flags must be replaced by the rule path for a generated level (the product should derive this, not the startup file). Not changed here.
8. The old P17 coverage numbers referred to seeds `s*7919`; `PROCGEN2-coverage.tsv` uses the same seeds (to keep the comparison); the oracle uses seeds 0 and 1.
9. Level notices `Excluded exported module/navigation/collision helper instances` and the other source notices are unchanged.

## Placeholders

- `snap_to_floor` (P17 PROCGEN): an approximation; the original placement rule is not recovered.
- `fresh_seed()`: time and counter source, not the original realtime call.
- `--transition-confirm` and `IsAfter_*` stubs (TRANSITIONS): unchanged.
- Decor animation: authored pose only (no `startanim` playback).

## Package files required

- `.local-inputs/claude-preview16/levels-root` (unchanged): the overlay `data/3d/AnimatedDecors/*.bdae`, `data/3d/skybox/*.bdae` and the module visual documents are already there.
- The overlay's `data/pydata/loot_table_pyarray.bin` does not match the menu creation owner receipt; the menu start run uses `procgen2/asset-root3` (hard links, file excluded). The package rc3 does not carry the file.

## Verifier script

1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p17procgen2 -Jobs 6` (the `-Test` run leaves the summary to `ctest`).
2. ctest (llvm-mingw on PATH): only `session_skill_binding` fails (134 tests).
3. Isolated: `build-p17procgen2/procedural_level_v1_tests.exe <levels-root>` and `procedural_layout_fidelity_tests.exe <levels-root>` print `OK`.
4. Coverage: `build-p17procgen2/procedural_coverage.exe <levels-root> 64 [rule-substring]` (per-seed lines for the substring); the committed table is `coordination/claude-preview17/PROCGEN2-coverage.tsv`.
5. Module probe: `build-p17procgen2/procedural_module_probe.exe <module .bdae> <node>...` (triangles and materials).
6. Quiet EXE runs (`port/windows-foundation/tools/quiet_run.ps1 -JobsFile <job>`): `witch-decor` (`--start-mode swamp --level <rule> --procedural-seed 1234`), `wind-sky`, `swamp-sky`, and `menu-final` (packaged startup with the changes listed in section 5). Expect `Level triangles=11406` for the witch cave, `Skybox loaded` for wind and Swamp, `Rendered frames=100; clean shutdown`.
