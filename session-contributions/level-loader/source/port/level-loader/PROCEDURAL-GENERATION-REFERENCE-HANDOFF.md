# Original procedural generation reference

The loader goal remains active. The visible private Android app currently
renders 16 fixed maps; Swamp mobs/chests and procedural map rendering are
still pending. Native generation now matches this reference, as recorded in
`PROCEDURAL-LAYOUT-MILESTONE-HANDOFF.md`. No shared engine boundary or gameplay
factory is agreed.

`tests/procedural_layout_original.py` executes the original ARM32
`RandomGenerator::Generate`, root selection, rule traversal, path/forced-room
selection, tile spawning, occupancy checks, backtracking and Array2d operations.
The original rule/list readers prepare the rule tree. Each original MGX file
is loaded through the actual derived MgxBlock constructor and XML reader;
original block-map insertion and exit linking prepare its connections.
Selection, collision, shuffle and placement answers are never hook fixtures.

File discovery comes from the independently verified original GetFiles
receipt. Block name, folder and target strings are caller inputs assigned
through the original string assignment instructions. FromFilename, the full
LoadRuleFile orchestration and serialization do not execute. Pool computation
is explicitly rejected by this fixture; all 35 supplied procedural definitions
have zero room-pool declarations. Allocation and imported byte/numeric/format
services are explicit adapters. Historical Bionic numeric parsing beyond the
previously tested input domain is not established.

The full receipt requests 35 authored definitions and four synthetic cases,
each with seeds 0 and 1. Every run records generation success, final RNG state,
ordered tile hierarchy, selected block/source, grid coordinates, height bits,
gameplay/visual list elements and counts of root attempts, placements,
unspawning and rule/path calls. A false original generation result is retained
as a no-layout run. A false rule reader result is separately recorded; the
previous caller audit establishes that Level::GenerateRandomLevel invokes
Generate even when LoadRuleFile returns false.

The three-room forced chain places three actual linked rooms. The repeated
same-block path fixture returns no layout because its two-exit/one-child
distribution has no choices; it does not establish a general repeated-room
rejection. Path can append a matching self-room candidate after filtering.
Root selection executes the original
shuffle even for list entries with zero or negative chance values. Swamp-return
references produce 11/12 rooms, and its troll cave produces 9/8 rooms for the
two seeds. These are original-execution references, not Android renders or
native-generator parity results.

`reference/procedural-generation` captures 35 original instruction bodies and
the exact 26,136-byte gDistributions table, with ELF/tool/source provenance.
The table's physical dimensions are 6 x 6 x 121 x 6 bytes. Its unused zero-index
subtables are not read by Step. Some other subtables lack a local sentinel;
the native implementation must preserve the bytes and explicitly reject an
unsafe table lookup rather than synthesize missing distributions. Direction
mismatch can grow the exit worklist, so all six bytes of each choice matter.
The original runtime rule child array has six pointers; tile children have
eight. Those capacities need checked native behavior.

Generation inspection also clarifies the constructor-only scalar projection:
runtime Impl word +0x28 is used as the Path step counter, not a tile pointer.
The existing constructor snapshot's `tile_present` label therefore must be
replaced before reusing it in a generator. It only proved the initial word was
zero. Word +0x40 remains a separately initialized zero word; its later meaning
must not be inferred from the snapshot's `progress` label. No shared/public
factory contract depends on these private constructor labels.

Reproduce from this isolated checkout using the bundled Windows Python:

```text
python port/level-loader/tests/procedural_layout_original.py --engine C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so --dependency-root C:/Users/adamc/.codex/worktrees/generic-level-loader/dependencies --cache C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --file-lists port/level-loader/reports/procedural-file-lists-original.json --rules port/level-loader/reports/procedural-rules-original.json --out port/level-loader/reports/procedural-layout-original.json --seeds 0,1
python port/level-loader/tools/capture_procedural_layout_checkpoint.py
```

The checkpoint verifies current helper hashes, source bytes from the canonical
ZIP, original ELF provenance, the two seeds, all four synthetic outcomes and
the returned tile hierarchies. It explicitly leaves native generation, full
orchestration, serialization, lifecycle, runtime objects and Android procedural
rendering unverified in this original-only receipt. The native layout
checkpoint separately binds the completed comparisons. Selected module
geometry and MGP/MVP still need to feed preparation, factories and the visible picker.
