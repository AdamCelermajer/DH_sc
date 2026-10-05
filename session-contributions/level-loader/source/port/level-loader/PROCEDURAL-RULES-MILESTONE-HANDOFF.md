# Original nested-rule and room-pool reader milestone

Loader session `01a108c2-0dd8-7842-bf2f-bc28dbbe94f5`; private checkout
`C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`.
Main-session gameplay/factory/condition/event/restore/release integration still
awaits an agreed contract. No shared integration or other session's emulator was modified.

## Retained state and original semantics

`procedural_rules_v1` captures original nested reader state over retained
lists, MGX blocks, connections and XML. Raw attributes and source identities
remain available even when the original reader leaves a field at its default.
Lists referenced by index remain owned. Borrowed plans survive all source,
ZIP, parser and facade teardown; failed candidates preserve the previous plan
and source identity. Separate visits remain separate. Internal preparation
success means a reader snapshot exists, not that the original reader returned
true or a playable/generated level exists.

Actual original ARM RootRule/rule constructors, virtual LoadFromXml readers,
NewRule, ValidBlock/ValidList/GetList, Hash, LoadRoomPools/RPElem and FillSizes
execute. Previously verified original list readers prepare the actual list
map. Known block keys are inputs from the verified original map receipts;
membership results are observed, not injected. Imported integer/search/C
services and allocation are explicit adapters. Full LoadRuleFile orchestration,
pool random allocation and layout execution are outside this comparison.

The first exact `RootRule` top-level child is selected. Nested Path, ForceBlock
and EndPath tags compare without ASCII case sensitivity, in unfiltered child
order. Missing `name` returns false before name/exit/id/child parsing. Path
extras are read before that base check, so a missing-name Path can retain its
length and dontGoBack while leaving its children unvisited. False child results
propagate, but remaining siblings still read. The complete partial tree and
every per-node result remain visible.

Names beginning with `#` select lists and optional signed indexes through the
original substring/atoi behavior. ValidList lowercases its temporary lookup;
GetList uses the unchanged query. Uppercase queries can therefore validate but
remain unresolved. Explicit block names and validation results remain separate.
Path lengths preserve the numeric prefixes/defaults; dontGoBack uses an integer
query, so `"true"` keeps the constructor default rather than acting as a parsed
boolean. ForceBlock reads connectFrom but discards its value, retaining direction
4. No new direction behavior is invented.

Top-level exact `pool` and `elem` tags retain pool size, ID hashes, recursion
flags and original minimum/maximum/current fields. Matching successful rules
fill the first matching hash in each pool after children have read; repeated
IDs therefore follow original postorder overwrites. Negative size bounds leave
the default -1 fields. ComputeSizeOfRules, which uses imported global rand,
does not execute here; no substitute random allocation is supplied.

## Evidence and explicit limits

- Original readers return for all 35 authored definitions and 14 safe boundary
  cases. Five other synthetic cases stop at checked guards before a nonadvancing
  comma loop, null child dereference, seventeenth-child write, unfiltered
  non-element child or null atoi input. Those guards are not successful original
  calls. Native candidates reject the same unsafe inputs transactionally.
- Host and ASan/UBSan compare every selected rule field, validation/resolution,
  ordered child, per-node return and unallocated pool field. The authored rows
  contain 236 rule nodes, including 38 false node results. Root results are
  29 true and six false. Authored list/block references all resolve. There are no
  authored room pools; synthetic cases verify pool declarations, duplicate IDs,
  postorder updates, negative lengths and recursion/default handling.
- False root results occur in 019_light_house_01/02/03, 022_swamp2,
  029_wind_temple and 033_voidmaze_01. Only the last also reports original XML
  error 9. These results are preserved separately from parser diagnostics.
  Caller handling remains unverified; they are not silently relabeled clean
  loads or declared permanently unsupported levels.
- Four further native domain checks cover integer overflow, lookup buffer
  overflow, a later non-element sibling and a first text child. The XML wrapper
  now records first-child node kind as well as next-sibling kind so filtered
  element storage cannot hide original unfiltered traversal hazards.
- Ownership, separate visits, failed preparation and source immutability pass.
  Updated host/sanitizer MGX, connection and list comparisons pass. The XML
  ownership CTest passes in both builds after the wrapper change.
- Android ARM64/x86_64 compile/link. No new Android runtime, generated layout or
  mob/chest proof is claimed. The installed private preview remains the earlier
  fixed-map milestone. Arbitrary historical Bionic numeric parsing is unproven.

## Reproduction and provenance

Build standalone `port/level-loader` using the existing external
`../build/host-xml`, `host-sanitizers`, `android-arm64` and `android-x86_64`
directories. Example Windows commands:

```text
wsl -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml -j2
C:\Users\adamc\AppData\Local\Android\Sdk\cmake\3.22.1\bin\cmake.exe --build C:\Users\adamc\.codex\worktrees\generic-level-loader\build\android-arm64 --parallel 2
```

`dh2_loader_procedural_rules_probe` reads length-prefixed block-key/XML groups on
stdin or accepts `CACHE IDENTITY DEFINITION` to prepare the retained chain.
Run `tests/procedural_rules_original.py` with `--engine`, `--dependency-root`,
`--cache`, `--lists reports/procedural-lists-original.json` and `--out
reports/procedural-rules-original.json`. Run
`tests/procedural_rules_differential.py` with `--original`, `--probe`, `--cache`,
`--inventory reports/canonical-level-inventory.json` and `--out`. Repeat with
the sanitizer probe and `--sanitizers`. Windows drivers invoke WSL Ubuntu for
host probes. Refresh MGX/connection/list comparisons after source/build changes.
Run `tools/capture_procedural_rules_checkpoint.py` last.

`reports/procedural-rules-checkpoint.json` binds current native sources,
oracle/helper scripts, disassembly, documentation, all four stages' receipts,
and the four libraries/four probes per build variant. Older checkpoints remain
historical after this CMake/XML-wrapper change.

Original engine SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Canonical cache SHA-256:
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
Original captures are in `reference/procedural-functions` and
`reference/procedural-map`. `tools/inspect_rule_receipt.py` prints bounded
summaries or a selected original rule tree for review.

## Remaining full-goal work

Trace original caller handling of reader failure, execute deterministic rule
traversal/selection, pool allocation and collision/backtracking, reconstruct
generated placements/identities and selected MGP/MVP graphs, and verify layout
serialization. Main-owned factories, conditions, events, restore/release and
runtime object rendering remain needed for chapter-1 Swamp's eligible mobs and
chests and the complete level lifecycle. The full loader goal remains active.
