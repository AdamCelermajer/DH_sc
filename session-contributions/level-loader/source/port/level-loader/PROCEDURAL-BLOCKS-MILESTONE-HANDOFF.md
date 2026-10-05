# Original MGX block interpretation milestone

Loader session: `01a108c2-0dd8-7842-bf2f-bc28dbbe94f5`.
Private worktree: `C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`.
No shared gameplay integration or factory/save ABI has been agreed.

## Implemented and compared

`procedural_blocks_v1` interprets retained MGX elements through original block
defaults, unit dimensions, integer room sizes, link declarations and accepted
exits. `ProceduralBlocksV1` prepares immutable block occurrences from the
retained `ProceduralSourcesV1` plan. The source plan and full original XML bytes
remain owned after ZIP/parser/facade destruction. A failed candidate preserves
the existing prepared plan. Separate visits retain their selected level identity.
These internal records are not persistent IDs or a shared runtime ABI.

Actual original ARM32 `Block::Block`, `MgxBlock::LoadFromXml`,
`Exit::LoadFromXml`, `StrToObj` and grid arithmetic execute on original XML
trees. Imported C/string/float services and allocation are explicit adapters.
The oracle supplies no block interpretation results. It observes return values
for every link declaration, including original rejections.

Host and ASan/UBSan comparisons cover all 250 `.mgx` files in the canonical
cache, 37 synthetic boundary cases, and 603 selected block occurrences across
all 35 procedural level rows. Their 1,073 accepted exit occurrences match the
original units, room dimensions, exit order, grid coordinates, height bits,
directions and link-type bytes. The authored selected occurrences contain zero
rejected links; synthetic cases verify original rejected links explicitly.

Original behavior retained:

- Units default to 3000; room dimensions default to one.
- Parsing starts at the first `GameObject`, then traverses unfiltered siblings.
- Accepted exits receive consecutive indices; rejected links consume no index.
- Directions compare without ASCII case; unknown names become `none`.
- Space compaction retains the old string length and stale suffix. For example,
  `a, b` becomes types `a` and `bb`.
- Comma splitting retains duplicates and empty entries. An empty or `0` final
  token rejects the entire exit, as does an absent direction.
- Position parsing takes three nonempty comma tokens, converts numeric prefixes,
  and leaves missing components zero. Grid projection uses float32 operations,
  ceiling, subtract-one and lower clamping; there is no upper clamp.

The XML snapshot now retains whether an element's immediate next sibling is
a non-element. This prevents the native MGX interpreter from hiding comments
or text encountered by the original unfiltered traversal.

## Explicit domain and gaps

Checked failures include original eight-exit capacity overflow, int32 overflow,
zero/nonfinite units, nonfinite/overflowing coordinates, absent active-exit
position, the original fixed position-buffer limit, and non-element siblings
in the traversal. These are reported rather than silently dropping objects.
They do not claim faithful recovery of original crashes or undefined behavior.

Numeric imported services use host CRT/libm models. The authored corpus and
listed boundary cases compare; historical Bionic parsing for arbitrary strings
has not been proven. XML caller policy remains distinct from parser diagnostics.

Room connection graph construction, sorted block-map insertion/duplicates,
typed rules, generated layout selection, collision/backtracking, generated
identities and selected MGP/MVP variants remain pending. A block occurrence is
a candidate definition, not a generated room.

Both Android ARM64 and x86_64 targets compile. This new block stage has not run
on Android. The installed private preview remains the prior map-only milestone;
its screenshots do not prove current typed blocks or mobs/chests rendering.
Chapter-1 acceptance and the complete generic-loader goal remain incomplete.

## Reproduction

Build the standalone `port/level-loader` project in the existing host,
host-sanitizers, android-arm64 and android-x86_64 build directories. Its target
`dh2_loader_procedural_blocks_probe` accepts length-prefixed XML on stdin or
`CACHE IDENTITY DEFINITION` arguments to prepare a retained source plan.

Run `tests/procedural_blocks_original.py` with `--engine`, `--dependency-root`,
`--cache` and `--out reports/procedural-blocks-original.json`. Run
`tests/procedural_blocks_differential.py` with `--original`, `--probe`, `--cache`,
`--inventory reports/canonical-level-inventory.json`, and `--out`. Repeat with
the sanitizer probe and `--sanitizers`. Windows drivers invoke WSL Ubuntu for
the host binaries. Then run `tools/capture_procedural_blocks_checkpoint.py`.

`reports/procedural-blocks-checkpoint.json` binds current source/helper hashes,
the original oracle, both host receipts, and the four libraries/probes.
Earlier checkpoints remain historical and must not be represented as current
source verification after the XML metadata/CMake changes.

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Canonical cache SHA-256:
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
Relevant original routines are captured in `reference/procedural-functions`;
the position converters are captured in `reference/string-conversions`.
