# Original runtime rule initialization and caller policy

Private loader checkout: `C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`.
The full loader goal remains active. Shared gameplay/factory/condition/event/
restore/release integration still awaits agreement. This stage does not modify
the shared checkout, menu session, or any emulator. The user's visible private
preview remains available for inspection.

## Implemented state

`procedural_instances_v1` reconstructs initial state of one runtime rule
constructor. It accepts a retained rule source and child-index path, an explicit
parent-presence input, and the verified generator-local random state. It retains
the full ZIP/MGX/list/rule graph and visit identity when given the rule facade's
borrow. Pure owned source plans support isolated fixtures. Failed source/path
initialization preserves both the prior instance and the random state.

RootRule starts with length zero and copies its explicit block names. ForceBlock
starts with length one and copies its names. EndPath starts with length one.
Path with ID zero calls original GetInt over its authored half-open length
interval; equal/reversed bounds do not advance the RNG. A nonzero-ID Path uses
the current size from the first matching pool element, or retains length one if
no pool matches, without a random call. Pool values can be -1; they are not
silently clamped. This stage does not run pool random allocation.

The base constructor uses exact lowercase north/east/south/west exit restrictions
only when it has a parent. RootRule ignores parent/exit restrictions. Unknown or
uppercase names leave no restriction. Child count, tile presence and progress
start at zero; Path direction starts at 4. Read-result flags do not gate these
constructors. No lazy child graph, tile, placement or traversal is fabricated.

## Original caller evidence

Original `Level::GenerateRandomLevel` at `0x3f07f8` calls LoadRuleFile at
`0x3f084c`, then unconditionally Generate at `0x3f0858` and serialization at
`0x3f0868`. It returns the generation result and destroys/clears its generator.
The LoadRuleFile tail combines the root reader result at `0x489814`; a false
root result forces the combined return false, but the Level caller does not
branch on that return.

Thirty-two original ARM caller executions cover load/generation/serialization
returns 0 and 1, seeds 0 and UINT32_MAX, and existing/absent old generators.
The constructor, loader, generator, serializer and destructor bodies are explicit
recorded service fixtures in this caller test. Allocation/free are environment
services. It verifies caller branching, argument routing, return and cleanup,
not those substituted bodies. Full LoadRuleFile execution remains unverified.

This explains why the six false root-reader trees, including SWAMP_02, must
remain available to the original generation path. It does not prove that every
such tree can produce a valid layout.

## Verification and reproduction

Actual original XML/list/rule readers, runtime constructors, GetApp, pool Find,
string comparison/copies, GetInt and NextInt execute ARM32. Imported C services,
heap allocation and exact integer quotient/remainder are explicit adapters;
constructor fields and random results are never supplied by a hook.

All 35 authored definitions and 15 safe boundary fixtures pass. Five boundary
seeds and parent-present/absent fixtures compare 2,185 authored instances over
236 source nodes, including 230 observed original random calls. Extra fixtures
cover all cardinal directions, uppercase/unknown exits, full int32 interval
width, equal/reversed bounds, partial readers and pool/default ID behavior.
Fixture preorder is a test invocation order, not original generated traversal.

Host and ASan/UBSan compare every observed initial field and before/after random
state. Every procedural inventory row prepares through the retained source chain
after facade/ZIP teardown, checks separate visits and retains source ownership.
The backing MGX/connection/list/rule comparisons are refreshed after CMake changes.
ARM64/x86_64 compile/link; this new stage has no Android execution proof.

Build the existing `../build/host-xml`, `host-sanitizers`, `android-arm64` and
`android-x86_64` directories with `cmake --build <directory> --parallel 2`.
Run original oracle from this checkout:

```text
python port/level-loader/tests/procedural_instances_original.py --engine C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so --dependency-root C:/Users/adamc/.codex/worktrees/generic-level-loader/dependencies --cache C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --rules port/level-loader/reports/procedural-rules-original.json --out port/level-loader/reports/procedural-instances-original.json
python port/level-loader/tests/procedural_instances_differential.py --original port/level-loader/reports/procedural-instances-original.json --probe ../build/host-xml/dh2_loader_procedural_instances_probe --cache C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --inventory port/level-loader/reports/canonical-level-inventory.json --out port/level-loader/reports/procedural-instances-host.json
```

Repeat the second command with the `host-sanitizers` probe, `--sanitizers`, and
`reports/procedural-instances-sanitizers.json`. Windows drivers invoke WSL for
host probes. The probe also accepts `CACHE IDENTITY DEFINITION`, or length-prefixed
block-key/XML fixture groups on stdin. It uses the same five seed fixtures as
the oracle. Run `tools/capture_procedural_instances_checkpoint.py` last.

`reports/procedural-instances-checkpoint.json` binds current sources, reused
oracle helpers, original disassembly, all five stages' receipts, and libraries/
probes from all four builds. Previous checkpoints remain historical.
Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Canonical cache SHA-256:
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.

## Remaining full goal

Implement lazy runtime rule graph registration, pool allocation, weighted list/
block selection, original collision/backtracking and placements/serialization;
then assemble selected MGP/MVP graphs through the same native map path. Complete
main-owned factory/condition/event/restoration integration, render chapter-1
Swamp's eligible mobs/chests, and verify full lifecycle/transition/resource
behavior and every supplied fixed/procedural level. Initial constructor parity
and caller control flow do not satisfy complete layout or whole-level acceptance.
