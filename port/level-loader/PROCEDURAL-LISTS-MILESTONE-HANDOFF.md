# Original procedural room-list declaration milestone

Loader session `01a108c2-0dd8-7842-bf2f-bc28dbbe94f5`; private checkout
`C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`.
The main-owned factory/condition/event/restore/release contract remains
unagreed. No shared integration or other session's emulator was changed.

## Behavior and ownership

`procedural_lists_v1` interprets the retained rule root and selected MGX block
keys into immutable list declarations. The source chain remains
`ProceduralSourcesV1 -> ProceduralBlocksV1 -> ProceduralConnectionsV1 ->
ProceduralListsV1`. Borrowed plans survive ZIP, parser and facade teardown;
separate visits retain distinct level identities. Failed publication preserves
the complete previous plan. These are internal preparation records, not runtime
objects, generated rooms, an agreed factory ABI or persistent save IDs.

Original `LoadListRules`, `ListRule::LoadFromXml`, `ListElem::LoadFromXml`,
`ValidBlock`, lowercase conversion, map lookup, unique insertion and red-black
operations execute actual ARM32 instructions. Known block-map keys are caller
inputs from the verified original file-list/block-map receipts. Original
membership results are observed, not supplied by hooks. Allocation and imported
C/float services are explicit environment adapters. This comparison does not
execute full `LoadRuleFile`, weighted list selection or layout generation.

The original uses exact `list` and `elem` tag filtering. List names are
lowercased; missing names become empty keys. Sorted unique-name order selects
the first declaration with each lowercase name. All declarations and ordered
elements are retained, including excluded duplicates. The native source tree
and raw bytes preserve authored case instead of being mutated by the reader.

`replacement` defaults true and otherwise requires exact `"true"`. The authored
`random` attribute is unread by this reader. Element block names, gameplay and
visual paths retain their authored bytes and default to empty. `chances`
defaults to 100 when the original integer query fails; valid numeric prefixes,
zero and negative values are preserved without invented normalization.

Original `ValidBlock` copies the query into a 512-byte stack buffer and
lowercases it before looking in the unchanged case-sensitive block map.
Thus `Room` can validate against key `room`, while a map containing only key
`Room` fails that lookup. This is distinct from changing the authored block
name or map key. Invalid references remain present and explicitly flagged;
the original feeds configurable assertions but does not remove the element.
The native reader rejects buffer overflow, integer overflow and unverified
non-ASCII lowercase inputs instead of reproducing unsafe behavior.

## Evidence and limits

- Original execution compares all 35 rule files and nine boundary cases:
  defaults, replacement versus random, duplicate lowercase names, filtered
  tags/comments, empty strings, invalid/case-sensitive map keys, numeric
  prefixes/int32 boundaries and the maximum safe validation buffer.
- Native comparisons cover every declaration, including excluded duplicates,
  sorted selection, all element strings/weights and observed membership
  results. Every one of the 35 procedural cache level rows prepares the retained
  chain: 171 list declarations and 479 list elements, with zero unavailable
  block-reference occurrences.
- Host and ASan/UBSan comparisons pass. Three explicit domain rejections are
  tested after partial candidate construction and retain the previous plan.
  Owner teardown, failed prepare and separate visit identity checks pass.
- Earlier MGX and connection comparisons were refreshed after the build/source
  wiring change: 250 authored MGX files, 603 selected block occurrences and
  10,550 ordered connection-reference occurrences remain passing.
- Android ARM64 and x86_64 compile/link successfully. This stage has no Android
  execution or generated-level rendering proof. The installed private preview
  still proves the earlier fixed-map rendering milestone.

Some rule files retain original XML diagnostics; matching list interpretation
does not turn them into clean parses or establish full rule-file success.
Host CRT numeric services do not prove arbitrary historical Bionic parsing.
Inspection also confirms EndPath dispatches to the base rule reader. The
handling of missing rule names and caller return values still needs execution
evidence before choosing nested-rule failure policy.

## Reproduction and provenance

Build standalone `port/level-loader` in the existing external directories
`../build/host-xml`, `host-sanitizers`, `android-arm64` and `android-x86_64`.
For example, from this checkout on Windows:

```text
wsl -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml -j2
C:\Users\adamc\AppData\Local\Android\Sdk\cmake\3.22.1\bin\cmake.exe --build C:\Users\adamc\.codex\worktrees\generic-level-loader\build\android-arm64 --parallel 2
```

`dh2_loader_procedural_lists_probe` reads length-prefixed key-list/XML groups
from stdin, or `CACHE IDENTITY DEFINITION` arguments for retained preparation.
Run `tests/procedural_lists_original.py` with `--engine`, `--dependency-root`,
`--cache`, `--connections reports/procedural-connections-original.json` and
`--out reports/procedural-lists-original.json`. Run
`tests/procedural_lists_differential.py` with `--original`, `--probe`, `--cache`,
`--inventory reports/canonical-level-inventory.json` and `--out`. Repeat with
the sanitizer probe and `--sanitizers`. Windows drivers use WSL Ubuntu for
host probes. Run `tools/capture_procedural_lists_checkpoint.py` last, after
refreshing block/connection host and sanitizer receipts against current builds.

`reports/procedural-lists-checkpoint.json` binds current native sources,
oracle/helper scripts, original disassembly, documentation, all three stages'
receipts and four libraries/three probes per build variant. Older checkpoints
remain historical after this stage's CMake and source changes.

Original engine SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Canonical cache SHA-256:
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
Original captures are in `reference/procedural-functions` and
`reference/procedural-map`. The rule-vtable inspection utility executes the
original constructors; its dispatch observation is preparatory evidence only.

## Remaining full-goal work

Nested rule and room-pool interpretation, deterministic traversal/selection,
collision/backtracking, generated placements/identities, selected MGP/MVP
graphs and original layout serialization remain pending. Main-owned factories,
conditions, events, restore and release services are still needed for the first
complete chapter-1 Swamp demonstration with eligible mobs/chests and lifecycle.
The full loader goal remains active.
