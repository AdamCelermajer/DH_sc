<!-- A partial canonical source adapter now consumes retained original XML and
actual owner-assigned signed handles. See CANONICAL-SOURCE-ADAPTER-HANDOFF.md.
This does not mean mobs/chests or complete runtime restoration are available. -->
<!-- Current map recovery adds original backup selection, exact reference
repairs and explicit static AnimatedDecor inspection. See MAP-RECOVERY-HANDOFF.md
for final Android, host and sanitizer verification receipts. -->
# Native generic level loader â€” active reconstruction

The full loader goal is blocked on canonical gameplay providers from the main
session. Swamp map geometry renders in the private emulator; runtime mobs/chests
remain unverified. Current source preparation follows the original backup route
after generation returns false and applies explicit, recorded repairs for two
confirmed Void Maze resource-name defects. The supplied cache is unchanged.

The map-recovery checkpoint verifies 344 native map/seed assembly cases, preserves
the 86 frozen original-mode cases, and checks cancellation/failure retention with
address/undefined-behavior sanitizers. See `MAP-RECOVERY-HANDOFF.md` and the new
recovery receipts for current rendering evidence and integration boundaries.
Older milestone receipts retain their original source hashes and results.
Work is isolated at
`C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`.

The starting snapshot contains 7,257 tracked/nonignored source files from the
live checkout, including uncommitted work. Per-file hashes are recorded in
`../baseline-snapshot.json` outside this repository root. The snapshot is not
claimed to be tree-wide atomic; no file changed during its individual read.
Build outputs and ignored `.local-inputs` were not imported.

The isolated app ID is `local.dh2.loader`. Its fresh AVD is `DH2_Loader_API37`,
emulator port 5590 (`emulator-5590`). Its window is visible by default, with on-demand
rendering and a picker for all 51 original definitions with generated seed 0/1
controls and known failure labels; `--headless` is optional. The other sessions' emulators were not modified. All future adb commands must explicitly target
the loader's verified serial. Shared renderer/CMake/gameplay integration remains
with the main session after an agreed handoff.

## Implemented preparation and evidence

- `tools/inventory_levels.py`: canonical-cache inventory, all 51 level rows and
  33 travel destinations, complete parsed attribute trees and unresolved refs.
  Its strict Python parser initially accepts 36 level definitions. The 15 other
  rows include parse failures; they are not 15 missing assets.
- `tests/xml_probe.cpp`: compiled native TinyXML candidate used against all
  1,664 cached MLX/MGP/MVP/rule.xml documents; nine parser errors, no probe failures.
- `tests/xml_original_probe.py`: executes the original ARM32 TinyXML constructor
  and parser. Heap, libc byte/string operations, C-locale Bionic tables, integer
  division and uncontended locks are explicit environment services.
- `reports/xml-original-comparison.json`: 1,664 comparisons covering every
  supplied MLX/MGP/MVP/rule.xml resource. First element trees/attributes
  match for all 1,664. Nine diagnostic-code differences remain; the receipt is
  explicitly INCOMPLETE. Text nodes and original level-caller policy are outside
  that comparison. Top-level siblings are retained by the production wrapper
  but are not compared by the first-root parser projection.
- `xml_document_v1`: candidate owned raw-byte/element-tree capture, immutable
  retained borrows and synchronous all-or-nothing publication. It records partial-tree/error evidence
  without granting permission to instantiate malformed content. Compilation and
  ownership tests pass, including multiple top-level elements. The mutable facade
  must be used sequentially; retained snapshots may outlive it.
- `resource_paths_v1`: the compiled-data path branch of original `Level::LoadFile`.
  The original tries the authored resource prefixed by empty, `data/`,
  `data/scene/` and `data/3d/modules/`. It removes the first occurrence of the
  first matching `old/`, `debug/`, `ps3/` or `iphone/` marker, in that priority;
  it does not remove all markers. Native attempts match six original ARM traces
  in `reports/resource-path-differential.json`. Uncompiled-data mode is pending.
- `fixed_sources_v1`: native preparation over the existing `ZipAssetPackV1`.
  Opening SWAMP retains 19 original XML documents, nine module links, complete
  declaration attributes and raw bytes: 50 Character declarations and five
  OpenableContainer declarations. Failed preparation preserves the previous
  source set; retained borrows survive both source-owner and ZIP-reader teardown.
  These are source declarations, not active mobs/chests or rendered objects.
- `reports/fixed-sources-level-coverage.json`: all 51 table rows attempted.
  Sixteen source sets prepare; 29 require procedural generation and six are
  parser-blocked. No row is marked as rendered or gameplay-supported.
- `procedural_random_v1`: original generator-local RNG, signed interval behavior,
  shuffle callback and byte hash. Original ARM execution matches 1,378 cases,
  including 17,664 consecutive sequence values. Optimized and ASan/UBSan probes
  pass; global gameplay RNG and full procedural layouts are outside this check.
- `procedural_file_list_v1`: original `GetFiles` byte tokenization, including
  unconditional pre-LF character removal, NUL termination and ignored final
  unterminated tails. Original ARM observations match all 21 authored MGX file
  lists plus 11 edge cases, also under ASan/UBSan.
- `procedural_sources_v1`: retains original rule roots, file-list bytes and every
  file-listed MGX block occurrence. All 35 rule definitions compare with source
  data (603 block occurrences), including ownership after teardown, failed
  publication and a separate visit. Swamp return visit `SWAMP_02` retains 42
  documents and 41 listed MGX blocks separately from fixed `SWAMP`. Its original
  rules/scripts/configuration stay intact; no layout or selected MGP/MVP graph
  is generated yet. Source comparisons and sanitizer checks pass; both Android
  ABIs build, with Android execution of this stage still unverified.
  Original `LoadRuleFile` and `MgxBlock::LoadFromXmlStream` ignore the buffer
  parser's return before selecting their first matching root. This stage follows
  that root-selection policy while retaining diagnostics. Six malformed rule
  sources still report errors. Fifteen sources rejected by strict Python XML
  (including multiple-root inputs) are compared through the original ARM parser;
  fixed-source preparation's earlier strict error gate remains unchanged.
- `procedural_blocks_v1`: original MGX units, room dimensions, link declaration
  results and accepted exits. Actual original ARM routines match all 250 cache
  MGX files plus 37 boundary cases. Native retained plans compare all 603 block
  occurrences in 35 procedural rows, including 1,073 exits, with source ownership,
  failed publication and separate visits checked under ASan/UBSan. Original
  stale suffixes after space compaction, comma duplicates, final empty/zero
  rejection, unknown directions and float32 grid projection remain intact.
  These are candidate blocks, not generated rooms. Generation remains
  pending; both Android ABIs compile but this stage has no Android execution
  proof. See `PROCEDURAL-BLOCKS-MILESTONE-HANDOFF.md` and
  `reports/procedural-blocks-checkpoint.json` for that historical milestone;
  the connection checkpoint binds later refreshed block receipts.
- `procedural_connections_v1`: original case-sensitive block-map ordering,
  first duplicate selection and ordered exit connections. Actual ARM comparator,
  map insertion/balancing/traversal and link routines compare all 21 authored
  block pools. Native retained plans compare all 35 procedural rows, including
  10,550 ordered connection references across 603 selected block occurrences.
  Duplicate type matches and self-connections remain intact. Host and sanitizer
  checks include ownership and transactional rejection after exceeding the
  original 64-reference capacity. Both Android ABIs build; this stage has no
  Android execution or generated-layout proof. See
  `PROCEDURAL-CONNECTIONS-MILESTONE-HANDOFF.md` and
  `reports/procedural-connections-checkpoint.json` for that historical milestone;
  the list checkpoint binds refreshed block and connection comparisons.
- `procedural_lists_v1`: original room-list declarations, default/explicit
  replacement, authored element order, weights and gameplay/visual paths.
  The reader lowercases list names and ValidBlock query text, while retaining
  authored block names. The first equal lowercase list name stays selected;
  every excluded duplicate declaration remains retained. Actual original ARM
  list/element readers and map membership/insertion compare 35 rule files and
  nine boundary cases. All 35 procedural level rows compare 171 declarations
  and 479 elements, with zero unavailable block references. Host and sanitizer
  checks include ownership, separate visits and failed-candidate publication.
  Both Android ABIs build; weighted selection and layout execution remain
  pending. See `PROCEDURAL-LISTS-MILESTONE-HANDOFF.md` and
  `reports/procedural-lists-checkpoint.json` for that historical milestone;
  the rule checkpoint binds refreshed MGX, connection and list comparisons.
- `procedural_rules_v1`: original nested RootRule/Path/ForceBlock/EndPath reader
  state, per-node results, list lookup/indexing, path lengths and room-pool
  declarations with postorder FillSizes. Actual ARM constructors, virtual
  readers, map lookups and pool routines compare 35 authored definitions and
  14 safe boundary cases; five unsafe domains are guarded explicitly. Native
  host and sanitizer checks compare 236 authored rule nodes, including 38 false
  node results and six false root results. No false result is silently promoted
  to original success. All authored list/block references resolve; authored
  pools are absent, with pool fields covered by synthetic original execution.
  Owner teardown, separate visits and failed publication pass; ARM64/x86_64
  build. See `PROCEDURAL-RULES-MILESTONE-HANDOFF.md` and
  `reports/procedural-rules-checkpoint.json` for that historical reader milestone;
  the instance checkpoint binds later refreshed reader comparisons.
- `procedural_instances_v1`: initial runtime RootRule/Path/ForceBlock/EndPath
  state and exact local-RNG effects, with the original source graph and visit
  identity retained. Actual ARM constructors compare 35 authored definitions
  plus 15 boundary cases. Five seeds and both parent-presence fixtures compare
  2,185 authored instances and 230 observed original random calls. IDs with no
  matching pool keep length 1 without consuming random state; zero-ID Paths
  use the original half-open interval. Root length is zero. Exit restrictions
  use exact lowercase cardinal names only on non-root instances with a parent.
  Actual Level::GenerateRandomLevel caller instructions also prove unconditional
  generation/serialization after the loader boundary and return of the generation
  result in 32 service-fixture cases. This is a caller control-flow check; full
  LoadRuleFile, graph registration, generated traversal/placement and serialization
  bodies are not established. Host/sanitizer comparisons and both Android builds
  pass. See `PROCEDURAL-INSTANCES-MILESTONE-HANDOFF.md` and
  `reports/procedural-instances-checkpoint.json` for that historical milestone.
- `procedural_layout_v1`: exact original distribution bytes, forward shuffles,
  root/list selection, rule/path traversal, footprints, float32 heights, list
  consumption/restoration and backtracking. All 70 authored runs and eight
  synthetic runs match the executed original ARM hierarchy, source selections,
  coordinates, height bits, gameplay/visual metadata, final RNG and seven event
  counters. Cache-facade comparisons cover all 70 authored runs. Repeated
  generation preserves source lists; missing inputs and unsupported pools
  preserve prior output. Retained results keep the source graph and visit
  identity after teardown. ASan/UBSan checks and both Android builds pass.
  See `PROCEDURAL-LAYOUT-MILESTONE-HANDOFF.md`; procedural rendering and
  generated module assembly remain unverified.
- `fixed_map_v1`: retained native geometry/material/navigation assembly over
  those source sets and the existing scene, BRES and floor kernels. All 16
  prepared fixed definitions assemble; the other 35 retain their source blockers
  in `reports/fixed-map-level-coverage.json`. Swamp has nine authored modules,
  369 map mesh instances, 16 navigation floors and 626 navigation triangles.
  Module placements are checked against the original XML. Visual modules with
  no floor are allowed, matching the original room loader.
- `fixed_declarations_v1`: an internal index of each authored declaration
  occurrence, retaining its complete source tree, module context and checked
  float32 authored transforms. Reused files retain distinct occurrences without
  changing authored names or assigning persistence IDs. All 2,331 occurrences
  in the 16 assembled fixed definitions match independent original-XML reads
  (`reports/fixed-declarations-host.json`). Missing and empty properties remain
  distinct; `template` and `_templateName` are not combined. Translated positions
  are authored projections. Original loading checks `IsGameObject()` before
  module offset addition; only `LevelConfig` gets immediate `InitPost` there.
  The earlier `InitPre` description was incorrect; see
  `reports/original-loader-vtables.json`. Class defaults, activation and final runtime positions still
  require the main-owned services. This is not an agreed factory/save ABI.
  The same 16-level comparison passes ASan/UBSan with leak detection and retained
  ownership checks (`reports/fixed-declarations-sanitizers.json`). The new
  declaration target builds for both Android ABIs; execution on Android remains
  unverified. `reports/declarations-checkpoint.json` binds this later milestone.
- `visual_transform_v1`: original near-zero scale handling, three-component
  scaling and Euler-to-quaternion argument order. The original ARM transform
  and synchronization routines match 138 checked cases, with zero observed
  quaternion difference (`reports/visual-transform-differential.json`). This
  comparison does not establish all GameObject initialization side effects.
- `user_properties_v1`: original LF/key/value/percent-quote behavior, kept
  separate from gameplay property parsing. Original ARM parsing matches 89
  checked cases, including four raw Swamp BDAE property strings. Raw strings and
  decoded floor metadata remain retained. A geometry-only bridge clears those
  tags in a temporary scene before the existing floor adapter, then applies the
  decoded flags before graph construction; authoritative source data is intact.
- Swamp map assembly passes repeated preparation and retained-resource checks,
  plus ASan/UBSan with leak detection. The map module and probes build for both
  Android ABIs. These assembly tests establish geometry/navigation; the separate
  preview receipt below establishes map mesh rendering. Runtime mob/chest
  creation and gameplay remain unverified.
- Swamp source preparation and retained ownership also pass an ASan/UBSan
  build with leak detection and halt-on-error enabled. The standalone loader,
  cache library and probes build for Android ARM64 and x86_64 at API 23.
  No loader APK or emulator rendering result is verified by those builds.

The vendor reference is TinyXML 2.6.2 from the official SourceForge archive,
verified archive SHA256
`ac6bb9501c6f50cc922d22f26b02fab168db47521be5e845b83d3451a3e1d512`.
The original game's exact TinyXML version is not established. Original parser
comparison is required; a library name/version is not parity evidence.

## Reproduction

From this checkout with the bundled Windows Python, invoke:

```
python port/level-loader/tools/inventory_levels.py --cache <canonical-cache.zip> --output port/level-loader/reports/canonical-level-inventory.json
```

Standalone native candidate builds in WSL:

```
cmake -S port/level-loader -B ../build/host-xml -DCMAKE_BUILD_TYPE=Release
cmake --build ../build/host-xml -j 2
python3 port/level-loader/tests/run_xml_audit.py --phase native --cache <canonical-cache.zip> --probe ../build/host-xml/dh2_loader_xml_probe
```

On Windows, private dependencies live in `../dependencies`; set PYTHONPATH to
that absolute directory, then run:

```
python port/level-loader/tests/run_xml_audit.py --phase original --cache <canonical-cache.zip> --engine <original-libDungeonHunter2.so>
```

Inputs are bound to canonical ZIP SHA256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`
and original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

Native source preparation and ownership validation:

```
ctest --test-dir ../build/host-xml --output-on-failure
python3 port/level-loader/tests/resource_path_differential.py --original port/level-loader/reports/resource-path-original.json --probe ../build/host-xml/dh2_loader_resource_path_probe --out port/level-loader/reports/resource-path-differential.json
python3 port/level-loader/tests/fixed_sources_host.py --cache <canonical-cache.zip> --probe ../build/host-xml/dh2_loader_fixed_sources_probe --out port/level-loader/reports/swamp-fixed-sources-host.json
python3 port/level-loader/tests/fixed_sources_coverage.py --cache <canonical-cache.zip> --probe ../build/host-xml/dh2_loader_fixed_sources_probe --inventory port/level-loader/reports/canonical-level-inventory.json --out port/level-loader/reports/fixed-sources-level-coverage.json
python3 port/level-loader/tests/fixed_map_host.py --cache <canonical-cache.zip> --probe ../build/host-xml/dh2_loader_fixed_map_probe --out port/level-loader/reports/swamp-fixed-map-host.json
python3 port/level-loader/tests/fixed_map_coverage.py --cache <canonical-cache.zip> --probe ../build/host-xml/dh2_loader_fixed_map_probe --source-coverage port/level-loader/reports/fixed-sources-level-coverage.json --out port/level-loader/reports/fixed-map-level-coverage.json
python3 port/level-loader/tests/fixed_declarations_host.py --cache <canonical-cache.zip> --probe ../build/host-xml/dh2_loader_fixed_declarations_probe --map-coverage port/level-loader/reports/fixed-map-level-coverage.json --out port/level-loader/reports/fixed-declarations-host.json --swamp-out port/level-loader/reports/swamp-fixed-declarations.json
```

The sanitizer build uses a separate `../build/host-sanitizers` directory with
`-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined` and
`-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined`. Run the ownership and
Swamp probe with `ASAN_OPTIONS=detect_leaks=1:halt_on_error=1` and
`UBSAN_OPTIONS=halt_on_error=1`. Its receipt is
`reports/swamp-fixed-sources-sanitizers.json`.

Android standalone builds use NDK 29.0.14206865's `android.toolchain.cmake`,
Ninja, `ANDROID_PLATFORM=android-23` and respectively `ANDROID_ABI=arm64-v8a`
or `ANDROID_ABI=x86_64`, in `../build/android-arm64` and
`../build/android-x86_64`. Build them with `cmake --build <directory> --parallel 2`.
`tools/capture_checkpoint.py` records source, binary and receipt hashes for
this preparation milestone; future changes require a new checkpoint capture.
`tools/capture_map_checkpoint.py` records the later geometry/navigation
milestone, including reused kernel sources, all four builds and current receipts.
Checkpoints bind the source and binaries at their recorded milestone; older
map/preview checkpoints must not be presented as hashes of later declaration
changes. The installed preview remains the separately recorded map milestone.
Original transform/property comparisons use their respective
`tests/*_differential.py` scripts with `--engine`, `--dependency-root`, `--probe`
and `--out`; the property comparison also requires `--cache`.

The standalone source graph is not the proposed runtime level-description or
factory ABI. It shares XML bytes, not runtime module identities, and does not
evaluate conditions, resolve gameplay templates, create
objects or restore saves. Rules and alternate module selection remain explicit
unsupported cases rather than fallback layouts. XML error codes remain those
of the reference candidate. The original error table at `0x99b07c` includes an
extra allocation error and a different duplicate-attribute diagnostic; those
differences have not been hidden by a passing receipt.

Procedural preparation is a separate internal stage: original root/file-list
sources, typed MGX exits, ordered connections, typed room lists, nested rule
reader state, unallocated room-pool declarations and runtime constructor state
are retained. The procedural level caller's false-load branching is verified;
full loading orchestration, pool allocation, generated module assembly
and placement serialization remain
pending. `PROCEDURAL-MILESTONE-HANDOFF.md` records the earlier source stage;
`PROCEDURAL-BLOCKS-MILESTONE-HANDOFF.md` records the later block interpretation.
`PROCEDURAL-CONNECTIONS-MILESTONE-HANDOFF.md` records the subsequent map/link stage.
`PROCEDURAL-LISTS-MILESTONE-HANDOFF.md` records the room-list declaration stage.
`PROCEDURAL-RULES-MILESTONE-HANDOFF.md` records the nested-rule/pool reader stage.
`PROCEDURAL-INSTANCES-MILESTONE-HANDOFF.md` records runtime constructor state and
the original procedural level caller's control flow.

The map stage is also internal, not an agreed runtime factory/save ABI. It
retains original visibility and conditions; conditional module selection is
unsupported rather than flattened. Its generated scene IDs are assembly IDs,
not campaign persistence keys. Eight unclassified Swamp geometry instances
remain explicit in the receipt. Scene lights/effects and the full original
helper-node rendering policy still need integration. Navigation query scratch
is mutable: use a retained map sequentially, not from concurrent path searches.

## Remaining scope

Agree the factory/event/condition/restoration boundary in
`INTERFACE-PROPOSAL.md`; complete original parser/level-caller semantics and
resource-path policy; complete original map policy and all declared object
types; finish original complete PropertyMap serialization/default behavior;
connect real engine services;
verify Swamp mobs/chests and complete scene rendering, then every supplied level; validate lifecycle,
failure and ownership behavior; deliver the reviewed integration handoff.

PowerShell-started handles remain stalled. Direct `cmd.exe` commands with
`login=false` work for native builds, reads and audits. Existing handles were
retained; no shared process or service was restarted.

## Isolated Android map preview

`android/loader_preview.cpp` and the private `LoaderPreviewActivity` read the
same canonical ZIP through native descriptor-backed loading. The private app
CMake selects only this preview target. Original textures, material bindings,
vertex colors, texture matrices, alpha thresholds and blending feed an
inspection shader. Camera and shader are adapter choices; original lights,
effects, helper visibility and complete scene fidelity are not established.
Eight unclassified Swamp geometry instances remain retained but are not drawn
as ordinary map mesh nodes. Gameplay objects are not fabricated for this preview.

`reports/swamp-map-preview.json` records 15 checks: initial Swamp rendering,
nine focused module captures, three repeated loads, failed-load retention and
Home/resume context recreation. The latter two compare the map viewport
pixel-for-pixel. The native frame submits 369 mesh instances, 384 draws,
24,676 triangles and three textures. Overview and per-module PNGs are in
`reports/`. Both Android ABIs build; runtime checks used x86_64 only.

`reports/fixed-map-preview-coverage.json` attempts all 51 rows and records a
submitted frame/screenshot for each of the 16 assembled fixed definitions.
`reports/fixed-map-preview-pixels.json` verifies the visible identity and
nonblank map viewport in all 16 screenshots. The 35 source blockers remain
explicit. In particular `SWAMP_02` uses `022_swamp2.rule.xml` and requires the
procedural generator; it is not supported by the fixed Swamp preview.

From this private checkout with bundled Windows Python:

```
python port/level-loader/tools/build_preview.py
python port/level-loader/tools/start_preview_emulator.py
python port/level-loader/tools/preview_device.py state
python port/level-loader/tools/preview_device.py install
python port/level-loader/tools/audit_preview.py
python port/level-loader/tools/audit_fixed_preview_coverage.py
python port/level-loader/tools/verify_preview_pixels.py
python port/level-loader/tools/capture_preview_checkpoint.py
```

Start the AVD once; the launch tool refuses occupied ports. Installation and
the audit verify `DH2_Loader_API37` and explicitly use `emulator-5590` for every
device operation. The audit launches/stops only `local.dh2.loader`. These tools
deliberately assert this private checkout path. Do not transplant the preview
app settings into the main session's integration build.

The first acceptance is still incomplete: runtime Characters and
OpenableContainer, conditions, template resolution and main-owned event/save
services must be connected and rendered. The preview does not establish
chapter progression or the return visit to SWAMP_02.

The visible preview's picker, three-map switching checks, selected-map reload,
module focus and zoom are recorded in `VISIBLE-PREVIEW-HANDOFF.md` and
`reports/visible-preview-checkpoint.json`. It is left open for user inspection.

Original procedural generation now has a separate ARM execution reference in
`PROCEDURAL-GENERATION-REFERENCE-HANDOFF.md`. It runs original Generate over
actual rule/MGX readers and connections, records selected layouts and original
no-layout results, and captures the distribution table and placement bodies.
Native generation now matches these references, as recorded in
`PROCEDURAL-LAYOUT-MILESTONE-HANDOFF.md`. Later module projection, generated map
assembly and selectable private Android inspection are recorded in
`PROCEDURAL-MODULES-MILESTONE-HANDOFF.md`. Earlier stage receipts and descriptions
above are historical; use the module checkpoint for current source/build hashes
and visible map evidence. Complete runtime mobs/chests remain unverified.

## Canonical cached source composition

See [CANONICAL-CACHED-FILE-HANDOFF.md](CANONICAL-CACHED-FILE-HANDOFF.md) for the verified connected-owner snapshot and exact SWAMP LevelConfig first failure. Cached XML now delivers retained canonical factory attempts; actual module/class/scene/condition/restoration providers still gate live readiness.

## Same retained Level fields and current GSLevel global

CanonicalLevelContextV1 now supplies the one game-Level identity, shared config/music fields and original constructor word150 through the exact KillLevel16 view. CanonicalGSLevelGlobalSlotV1 borrows the authoritative retained GSLevel::s_level global without automatic publication. See CANONICAL-LEVEL-CONTEXT-HANDOFF.md for interface mapping, constructor-prefix scope, ownership and verification. Full Level constructor/initialization, runtime wiring and SWAMP acceptance remain pending.
