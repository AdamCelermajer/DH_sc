# Original procedural primitives and source preparation

This later isolated milestone adds generator-local RNG, MGX file-list decoding
and retained rule/MGX source preparation. It does not generate a layout or
instantiate/restore runtime objects. The full loader goal remains active.

Original ARM comparisons pass 1,378 RNG/hash cases including 17,664 consecutive
values, and 32 file-list cases including all 21 authored lists. Optimized and
ASan/UBSan builds pass. No RNG sequence or file-list result is supplied by an
oracle hook: hooks supply exact integer division, immutable file bytes/storage
and observed vector writes while original instructions perform these operations.

`ProceduralSourcesV1::Borrow` retains identity, rule source/tree/diagnostic,
original folder and file-list bytes/entries, and file-listed MGX documents.
Original rules, lists, pools, transforms, exits, scripts and configuration remain
as source data. Duplicate listed occurrences remain visible. Missing required
dependencies fail explicitly; publication is transactional and borrows survive
owner/ZIP teardown and another visit. Sources are immutable snapshots; mutable
facades and their ZIP readers are used sequentially.

All 35 rule definitions pass source comparisons: 603 MGX block occurrences and
no missing dependency at this stage. Six malformed rule files retain parser
diagnostics; the original caller selects their retained root without checking
the buffer parser's return. Fifteen inputs rejected by strict Python XML are
compared with actual original ARM parser projections, including accepted
multiple-root documents. Other element/attribute trees compare independently
with original ZIP bytes; text-node/original MGX parser-wide parity is not claimed.

SWAMP_02 (`022_swamp2.rule.xml`) retains 42 documents, including 41 listed MGX
blocks. It remains distinct from the fixed nine-module SWAMP source set. Its
root rule/list overrides still need typed evaluation and selection; retaining
these 41 candidates does not mean 41 rooms will appear in the generated level.

Evidence and captures:

- `reports/procedural-random-differential.json` and sanitizer counterpart.
- `reports/procedural-file-lists-original.json`, differential and sanitizer counterparts.
- `reports/procedural-sources-host.json` and sanitizer counterpart.
- `reports/swamp2-procedural-sources.json` full retained source projection.
- `reference/procedural-functions/`: 157 symbol-bounded original routines.
- `reports/procedural-checkpoint.json`: later source/build/evidence binding.

Original entry points: NextInt `0x483a94`, GetInt `0x483ac8`, shuffle callback
`0x483af8`, Hash `0x483b14`, GetFiles `0x488904`, LoadBlocks `0x4892ec`,
LoadRuleFile `0x489508`, MgxBlock::FromFilename `0x48ad54`,
MgxBlock::LoadFromXmlStream `0x48aa84`. The RNG increments in 32 bits before
64-bit multiplication and remainder; changing that ordering changes layouts.
GetFiles emits `substr(0, LF-position-1)` for every LF and ignores the final
unterminated tail. These are recovered behaviors, not new cleanup decisions.

Reproduction with the isolated bundled Windows Python:

```
python port/level-loader/tests/procedural_random_differential.py --engine <original-libDungeonHunter2.so> --dependency-root ../dependencies --probe ../build/host-xml/dh2_loader_procedural_random_probe --out port/level-loader/reports/procedural-random-differential.json
python port/level-loader/tests/procedural_file_list_original.py --engine <original-libDungeonHunter2.so> --dependency-root ../dependencies --cache <canonical-cache.zip> --out port/level-loader/reports/procedural-file-lists-original.json
python port/level-loader/tests/procedural_file_list_differential.py --original port/level-loader/reports/procedural-file-lists-original.json --probe ../build/host-xml/dh2_loader_procedural_file_list_probe --out port/level-loader/reports/procedural-file-lists-differential.json
python port/level-loader/tests/procedural_sources_host.py --cache <canonical-cache.zip> --probe ../build/host-xml/dh2_loader_procedural_sources_probe --inventory port/level-loader/reports/canonical-level-inventory.json --original-lists port/level-loader/reports/procedural-file-lists-original.json --engine <original-libDungeonHunter2.so> --dependency-root ../dependencies --out port/level-loader/reports/procedural-sources-host.json --swamp2-out port/level-loader/reports/swamp2-procedural-sources.json
python port/level-loader/tools/capture_procedural_checkpoint.py
```

Build the standalone targets first as described in README. For sanitizer
comparisons, use `../build/host-sanitizers`, `--sanitizers` and separate receipt
paths. ARM64/x86_64 builds pass; this stage has not executed on Android. The
installed map-preview APK remains its earlier recorded milestone, separate
from the current procedural preparation libraries.

Next reconstruct typed block/exit links and rule/list/pool behavior, original
shuffle/selection consumption, path/backtracking/overlap constraints, and exact
generated MLX/stream serialization. Then assemble/render actual generated
placements. Main-owned object, condition, event and restore services remain
pending agreement through `INTERFACE-PROPOSAL.md`. Do not merge inherited
worktree changes or private preview app settings into the shared checkout.
