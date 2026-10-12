# Original procedural block-map and connection milestone

Loader session `01a108c2-0dd8-7842-bf2f-bc28dbbe94f5`; private checkout
`C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`.
Gameplay/factory/event/save integration remains awaiting main-session agreement.

`procedural_connections_v1` prepares an immutable ordered graph from the retained
`ProceduralBlocksV1` plan. It retains the block/source ownership chain and
selected level identity. Failed candidates leave the prior graph usable;
borrowed records survive ZIP/parser/facade destruction. Separate visits keep
their identity. Graph indices describe candidate definitions, not generated
rooms, active objects or persistent save identities.

## Original behavior recovered

Actual original `lstr::operator()`, `insert_unique`, red-black balancing/map
traversal and `Block::LinkToOtherBlocks` execute ARM32 instructions. They receive
blocks interpreted by the actual original MGX routines. Imported C/float and
allocator services are explicit adapters. Original `GetFiles` receipts supply
the file-listed inputs; the whole `RandomGenerator::LoadBlocks` coordinator
does not execute in this comparison.

Block names compare case-sensitively through `strcmp`; the first equal-name
occurrence remains selected. All source occurrences remain retained by the
native source plan, including excluded duplicates. Sorted unique-name map
order determines connection order. For each source block, the original loops
target block, source exit, target exit, source type and target type. A nonempty
source type must match the target string bytes and its direction must oppose
the target direction. Opposites are `[2,3,0,1,4]` for north/east/south/west/none.

Matching type pairs append separate references. Duplicate types therefore
produce repeated references and preserve their resulting selection weight.
The original allows a block to connect to its own other exits, and `none`
matches `none`. This stage applies no additional position, height, dimension,
collision or gameplay filter absent from the original connection routine.

## Evidence

- Original map/link execution covers all 21 authored MGX file-listed pools and
  four synthetic groups for ordering, duplicate names/types, self-connections,
  direction/type case, unknown directions, stale space suffixes, zero/empty
  tails and an empty map.
- Native comparisons cover every sorted key, insertion result and ordered exit
  reference, including repetitions. The 35 procedural level rows compare 603
  selected block occurrences and 10,550 connection-reference occurrences.
- Host and ASan/UBSan runs pass, including retained ownership and failed
  publication. A capacity failure after building 64 real connections preserves
  the complete previous graph. The native stage reports the offending block and
  exit instead of reproducing an original fixed-array overrun or truncating it.
- Android ARM64 and x86_64 builds pass. This new stage has no Android runtime or
  generated-level rendering proof. The installed private preview still proves
  only the earlier fixed-map rendering milestone.

The block oracle was refactored into a reusable CPU constructor without replacing
its original algorithms. Its 250 authored-file/37 boundary-case comparisons and
35 retained-plan comparisons were rerun and remain passing. The latest
connection checkpoint binds the refreshed block receipts as well.

## Reproduction and provenance

Build standalone `port/level-loader` in the existing `../build/host-xml`,
`host-sanitizers`, `android-arm64` and `android-x86_64` directories.
`dh2_loader_procedural_connections_probe` reads length-prefixed groups on stdin,
or accepts `CACHE IDENTITY DEFINITION` arguments for the complete retained chain.

Run `tests/procedural_connections_original.py` with `--engine`,
`--dependency-root`, `--cache`, `--original-lists
reports/procedural-file-lists-original.json` and `--out`. Run
`tests/procedural_connections_differential.py` with `--original`, `--probe`,
`--cache`, `--inventory reports/canonical-level-inventory.json` and `--out`.
Repeat with the sanitizer probe and `--sanitizers`. Windows drivers invoke WSL
Ubuntu for host probes. Refresh the block oracle/comparisons when its helper
sources change. Run `tools/capture_procedural_connections_checkpoint.py` last.

`reports/procedural-connections-checkpoint.json` binds current native sources,
oracle/helper scripts, original disassembly, documentation, both block and
connection receipts, and the four libraries and two probes per build variant.
Earlier checkpoints remain historical; their hashes must not be described as
current after this stage's CMake/helper changes.

Original engine SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Canonical cache SHA-256:
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
Map comparator/insertion captures are in `reference/procedural-map`;
the generator and connection captures are in `reference/procedural-functions`.

## Still required for the full loader

Typed rules, deterministic layout traversal/selection, collision/backtracking,
generated identities, selected MGP/MVP graphs and original serialization remain
pending. Numeric imported services use host models; arbitrary historical
Bionic numeric parsing is unproven. Main-owned factories, conditions, events,
save restoration and release services are still required for chapter-1 Swamp's
eligible mobs/chests and complete level lifecycle. The full goal stays active.
