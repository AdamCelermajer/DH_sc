# Integrated retained source pipeline

This milestone connects the verified cached ZIP/file traversal to the actual
FixedSourcesV1 preparation used by fixed maps and the generated-map inspection
adapter. The private Android preview now uses this connected path. It still
renders map geometry only; mobs, chests and campaign restoration are incomplete.

## Change

`fixed_sources_v1.cpp` discovers module links from the selected element
callbacks of `CachedLevelFileV1` / `LevelFileWalkV1`, instead of a separate
manual root-child scan. Acquired source borrows outlive file completion. Parser
or callback failure is latched, explicitly discarded, and fails preparation;
an existing published snapshot is preserved. URI sharing, cycle checks, exact
root recognition and source ownership remain explicit.

Generated inspection roots retain their original module-plan owner and raw
derived bytes, then use the same level-buffer normalization and selected-child
traversal. This does not establish full original PropertyMap serialization,
template selection, class construction or original asynchronous file opening.

`tests/fixed_sources_probe.cpp` checks the level-buffer route for every retained
source. Coverage labels describe the fixed-source probe's scope; its rule
generation blockers are not claims that the separate generator is missing.
`tools/build_preview.py` accepts explicit current fixed/procedural coverage
receipts when preparing the original 51-entry Android map catalog.

## Verified current state

- Host and address/undefined-sanitized builds prepare all 16 fixed definitions,
  assemble all 16 maps and compare 2,331 declaration occurrences independently
  against source XML, including nested data, module context and f32 projections.
- Both builds attempt all 35 procedural definitions at seeds 0 and 1. They
  assemble 66 runs; three original runs return no layout; VOID_MAZE_03 seed 1
  encounters the missing `vm011_corner_voidmaze_ne_00_01.mgp` dependency.
- Both standalone Android ABI libraries and the private APK compile.
- The installed APK renders DARKWOOD (24 modules / 936 draws), SWAMP_02 seed 0
  (11 modules / 518 draws) and SWAMP (9 modules / 384 draws) on the visible
  DH2_Loader_API37 / emulator-5590. Native frame logs and screenshots retain
  `gameplay=0 objects=0` / `mobs/chests pending` as explicit limits.
- SWAMP reload works; a missing-definition load keeps the previous rendered
  map pixels unchanged. The emulator is left showing SWAMP module 0. Its window
  size and the other chats' emulators are untouched.

Current receipts use the `source-pipeline-*` prefix. Earlier checkpoints and
screenshots are historical; they should not be presented as current APK proof.
`reports/source-pipeline-checkpoint.json` binds sources, host/sanitizer semantic
results, static-library members, packaged binaries, original cache, current APK
and current rendered evidence. It deliberately records full_loader_verified=false.

## Reproduction

Run the commands from this private worktree. Python is the bundled Windows
runtime; the host binaries are built in sibling `build/host-xml` and
`build/host-sanitizers` through WSL Ubuntu. Coverage scripts expose their exact
arguments with `--help`.

1. Build the host/sanitizer targets and run `tests/fixed_sources_coverage.py`,
   then `tests/fixed_map_coverage.py`, then `tests/fixed_declarations_host.py`
   against the matching current probe binaries and canonical cache.
2. Run Windows `tests/procedural_map_coverage.py` for both matching WSL probes,
   original inventory, cache and distinct current output paths.
3. Build the private APK with `tools/build_preview.py --fixed-coverage
   reports/source-pipeline-fixed-maps-host.json --procedural-coverage
   reports/source-pipeline-procedural-maps-host.json` (paths relative to loader
   directory, or pass absolute paths).
4. Run `tools/preview_device.py install`, then
   `tools/audit_source_pipeline_preview.py`. The audit checks the private AVD
   before every explicitly targeted device command; it force-stops only the
   private application for each selected-map test.
5. Build both standalone Android library configurations, then run
   `tools/capture_source_pipeline_checkpoint.py` to verify receipt closure.

For the coverage commands, use these concrete arguments from the private
worktree. Replace `LABEL` / `BUILD` with `host` / `host-xml`, then
`sanitizers` / `host-sanitizers`; all outputs stay in this loader directory.

```text
wsl.exe -d Ubuntu -- python3 /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/fixed_sources_coverage.py --cache /mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --inventory /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/canonical-level-inventory.json --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/BUILD/dh2_loader_fixed_sources_probe --out /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/source-pipeline-fixed-sources-LABEL.json
wsl.exe -d Ubuntu -- python3 /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/fixed_map_coverage.py --cache /mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --source-coverage /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/source-pipeline-fixed-sources-LABEL.json --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/BUILD/dh2_loader_fixed_map_probe --out /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/source-pipeline-fixed-maps-LABEL.json
wsl.exe -d Ubuntu -- python3 /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/fixed_declarations_host.py --cache /mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --map-coverage /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/source-pipeline-fixed-maps-LABEL.json --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/BUILD/dh2_loader_fixed_declarations_probe --out /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/source-pipeline-fixed-declarations-LABEL.json
C:\Users\adamc\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe port\level-loader\tests\procedural_map_coverage.py --cache C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip --inventory port\level-loader\reports\canonical-level-inventory.json --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/BUILD/dh2_loader_procedural_map_probe --out port\level-loader\reports\source-pipeline-procedural-maps-LABEL.json
```

After receipt closure, `tools/freeze_source_pipeline_handoff.py` creates a new
checkpoint-addressed ZIP containing the current source/contract slice, loader
verification sources, reports and observed screenshots. It verifies every
archived digest and refuses to overwrite an existing frozen bundle. The ZIP
requires the existing isolated reconstruction baseline plus original cache/ELF;
it is not a standalone source release or a complete gameplay implementation.

## Next boundary

The main session explicitly requested concrete typed context requirements.
`OWNER-CONTEXT-REQUIREMENTS.md` supplies retained source inputs, canonical
object/module output requirements and per-operation lifetime/ordering needs.
It names the current canonical interfaces inspected and their stated limits.
The main accepted the retained source/candidate/object borrow direction and
confirmed the generic gameplay factory, class template/property adapter,
containers, generic conditions/events and complete world restoration are
unavailable. It plans extraction of its existing canonical actor composition.
No callable shared ABI, shared checkout, main gameplay owner, menu files or
other device was modified by this milestone.

Next joint acceptance remains chapter-1 SWAMP with actual condition-eligible
Character and OpenableContainer resources instantiated and visibly placed by
the canonical owner services. Authored declarations or geometry frames cannot
substitute for that acceptance.
