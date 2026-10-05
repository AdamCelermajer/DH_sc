# Native procedural layout milestone

The goal is active. Native generation now matches executed original ARM layouts
for all 35 supplied procedural definitions at seeds 0 and 1, plus four small
fixtures. The visible Android preview still offers 16 fixed maps. Generated
module geometry/MGP/MVP assembly and procedural rendering are pending. Swamp
mobs/chests still require the main-owned factory/template/condition services.

`procedural_layout_v1` uses the exact original distribution table, original
forward Fisher-Yates ordering and the verified local RNG. Root selection,
direction restrictions, nested rules, forced rooms, paths, occupancy, float32
height arithmetic, duplicate connection/list entries and backtracking retain
their original behavior. Spawn removes the first matching non-replacement
list entry by room name and gameplay; visual is not compared. Unspawn restores
the selected entry at the list's end. Chance fields remain retained metadata;
this generation branch does not convert them into probability weights.

The generator copies its mutable list state. Repeated generation from the same
retained sources returns the same layout and counters. It does not mutate
authored lists or publish partial checked failures. A completed original
no-layout result returns API success with `generated=false`, an empty tile
graph and the observed final RNG/counters. Callers must check both statuses
before publishing a level. Unsupported room pools, unsafe distribution lookups,
capacity excesses, invalid coordinates and bounded-work excesses fail explicitly.
All supplied procedural definitions contain zero room-pool declarations.

The facade retains the rule/list/block graph, original documents and visit
identity in the result. Tile/source indices are internal occurrence indices,
not persistence IDs. This private preparation interface is not the proposed
shared factory/event/restoration ABI. There are no condition, quest, loot,
AI, animation or save implementations in this generator.

`tests/procedural_layout_probe.cpp` compares ordered selected tiles, parent/child
hierarchies, room source indices, grid coordinates, exact height bits,
gameplay/visual selections and chances, generation success and final random
state. It also compares seven observed event counters: random calls, root
attempts, placements, unspawns, Step calls, Path calls and base OneStep calls.
It checks repeated generation, transactional missing-input/pool rejection and
source ownership after ZIP/facade teardown.

Current receipts:

- `reports/procedural-layout-native.json`: all 78 input-based comparisons pass.
- `reports/procedural-layout-native-facade.json`: all 70 authored cache-facade
  comparisons pass.
- `reports/procedural-layout-native-sanitizers.json`: all 78 comparisons pass
  with ASan leak detection and ASan/UBSan halt-on-error enabled.
- `reports/procedural-layout-native-checkpoint.json`: source, reference, probe,
  library, configuration and receipt hashes for host, sanitized host, Android
  ARM64 and Android x86_64 builds.

The authored runs comprise 67 successful layouts and three original no-layout
results, totaling 533 selected rooms, 1,365 random calls, 1,185 placements and
652 unspawns. The no-layout results are red-desert cave 02 seed 0 and icy cavern
02 seeds 0 and 1. Six original rule roots return false while original Generate
still executes; these results remain separately recorded. This is faithful
original generation, not a claim that every original definition/seed succeeds.

Reproduce from this isolated checkout with the bundled Windows Python. The
host probe lives in the separate WSL build directory; pass its Linux path:

```text
wsl.exe -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml -j 4
python port/level-loader/tests/procedural_layout_differential.py --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml/dh2_loader_procedural_layout_probe --cache C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --original port/level-loader/reports/procedural-layout-original.json --out port/level-loader/reports/procedural-layout-native.json
python port/level-loader/tests/procedural_layout_differential.py --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml/dh2_loader_procedural_layout_probe --cache C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --original port/level-loader/reports/procedural-layout-original.json --out port/level-loader/reports/procedural-layout-native-facade.json --facade
python port/level-loader/tests/procedural_layout_differential.py --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-sanitizers/dh2_loader_procedural_layout_probe --cache C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --original port/level-loader/reports/procedural-layout-original.json --out port/level-loader/reports/procedural-layout-native-sanitizers.json
python port/level-loader/tools/capture_procedural_layout_checkpoint.py
python port/level-loader/tools/capture_procedural_layout_native_checkpoint.py
```

The constructor-only word at +0x28 is now known to be the integer Path step
counter; the generator initializes its own typed counter to zero. It does not
reuse the earlier snapshot's `tile_present` label or infer a meaning for +0x40.
Those historical private labels still need cleanup before wider reuse.

The next stage is original selected module/placement serialization policy,
resource selection, transforms and map assembly. Android procedural execution,
generated map rendering, original serialization, complete loading orchestration,
room-pool allocation, repeated whole-level load/unload, runtime objects and
gameplay remain unverified. Main integration remains pending the explicit
factory/event/restoration agreement. No shared files or other emulators changed.
