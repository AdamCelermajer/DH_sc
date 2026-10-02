# Dungeon Hunter 2: reconstruction findings, current state and continuation guide

**Source Lua checkpoint:** Official Lua 5.1.4 C/header/license bytes are now
vendored with a pinned archive/per-file manifest. The owned C runtime uses the
observed original float32/int32 profile through a separate configuration header.
504 original arithmetic outputs match host and Android source evaluation;
13 original numeric push/read cases verify float32 storage. The runtime builds
for ARM64/x86_64; host selftests/sanitizers pass. The exact x86_64 runner passed
on Android 17 with 4 KiB and 16 KiB pages, parsing 218 originals and the separate
sandworm override, with the unchanged malformed original rejected as expected.
Authored library/budget/error-recovery snippets passed; no game script was
executed. Original registration callers expose 175 distinct function names,
117 matching script global reads. Binder installation remains stubbed in that
trace. Nine numeric callbacks now have source implementations passing 2,375
original ARM32/compiled ARM64/host comparisons, 54,000 sanitizer calls and
authored Lua assertions on both Android 17 page sizes. Strict runtime sanitizers
pass after a separately documented table-key overflow/conversion repair applied
only to a generated build copy. Vendor bytes remain exact. Next implement
remaining native callbacks, checked PyData tables and include/object
lifetimes before executing original AI/skill behavior. The source APK has not
yet incorporated this runtime. See [runtime scope](port/lua-runtime/README.md)
and [registration evidence](reports/lua-registration-trace.json).

**Readable game scripts:** The complete cache's 219 `.luac` files are plaintext
source, not bytecode. All 900,493 original bytes are now preserved with archive
member hashes. 218 originals pass Lua 5.1.5 syntax, and a separate one-character
sandworm override passes. Original files remain exact. Engine API integration,
script execution and full source gameplay remain unfinished.
See [source scope](recovered/scripts/README.md).

**Original ending timing:** Extra-time and pending fields passed 495 calculations,
1,980 filtered blender notices and 495 ordinary animator notices against original
ARM32 and compiled ARM64/host source. 20,000 sanitizer calculation/notification
calls passed. Active lookup is a controlled stub; object lookup, actual game
callback effects and Android integration remain open.
See [ending evidence](port/animation-ending/README.md).

**Original completion dispatch:** Callback registration/checking passed 384
setters, 768 checks and 144 controlled dispatches against original ARM32 and
compiled ARM64/host source. Deferred notification, field changes during callback
and the final pending clear passed, as did 10,000 sanitizer checks. Original
game callbacks and Android integration remain open.
See [completion evidence](port/animation-completion/README.md).

**Original animation movement:** Explicit-position movement delta/reset passed
800 resets, 4,000 calculations and 100,000 sanitizer operations. Source preserves
the original repeated-timestamp behavior and position history. It is standalone;
root sampling, scene movement, collisions and source gameplay remain unfinished.
See [movement evidence](port/animation-motion/README.md).

**Original transition projection:** The original two-child transition request
and fade/update state passed 300 requests and 4,800 updates against original
ARM32 and compiled ARM64/host source. Active-child dispatch order passed,
with child updates/getters and callback checking explicitly stubbed.
100,000 safety operations passed. This is standalone; original object ownership,
callback effects, pose application and full gameplay remain unfinished.
See [scope and exact evidence](port/animation-transition/README.md).

**Two-motion source preview:** Checked absolute layers now combine the warrior's
dual walk and Dark Queen scene 03a motion. The exact updated APK passed imports,
midpoint seek, 0/50/100% mix, advancing Play and stable Pause on Android 17 with
both 4 KiB and 16 KiB pages. Visually inspected screenshots show distinct textured
poses. Host endpoint/palette and 3,000 damaged-input probes passed.
See [layer scope](port/animation-layers/README.md) and
[current runtime evidence](port/android-app/layers-runtime-validation.json).
Original transition scheduling and full source-built gameplay remain unfinished.

**Original timeline update:** The source Android app now uses reconstructed
range-clock arithmetic for animation playback, seeking and resume. All 400
original ARM32/compiled ARM64/host sequences matched (4,000 updates and 400 jumps);
50,000 sanitizer updates passed. The exact updated APK passed advancing Play,
stable Pause and visible pose changes on Android 17 with 4 KiB and 16 KiB pages.
See [the timeline component](port/animation-timeline/README.md) and
[Android runtime evidence](port/android-app/timeline-runtime-validation.json).
Full source-built gameplay remains unfinished.

**Player animation audit:** 333 of 342 clips produce checked source poses on
the warrior model, including 327 animated clips. Negative key times are now
supported. The current source APK passed dual-walk controls on Android 17 with 4 KiB
pages and a position-component cutscene on 16 KiB pages. Earlier builds
passed walk and negative-start aura checks. See the
[pose audit](port/animation-pose/README.md) for rejected formats and bindings.
The complete source game remains unfinished.

**2026-10-02 continuation:** Current source modules now include checked textures,
materials, scenes, static draw commands, software skin controllers, and
[original-instruction-checked float animation calculations](port/animation-values/README.md).
The [source Android preview](port/android-app/README.md) displays a textured
18-bone warrior and a selectable 27-track walk on Android 17 x86_64 with
both 4 KiB and 16 KiB pages. Its exact build and runtime records are under
`port/android-app/`; an emulator test script is included. This remains an
absolute-key diagnostic preview with no source-built gameplay. The older
dated snapshot below must be read alongside these current module reports.

**Snapshot date:** 2026-10-01, Asia/Jerusalem. **Repository reviewed:** [Noamcelermajer/DH_sc](https://github.com/Noamcelermajer/DH_sc), through [ded2115ee60048666e5fd146f80693613e70dadb](https://github.com/Noamcelermajer/DH_sc/commit/ded2115ee60048666e5fd146f80693613e70dadb).

This report consolidates the source-recovery and native-engine reconstruction work, and records the independently maintained APK compatibility work where it affects the handoff. It is a dated evidence snapshot; later commits, release notes and device reports may supersede individual status entries. In particular, the later [verified source import](docs/RECOVERY-SOURCE-IMPORT.md) makes the repaired Java/JNI source and recovery tools browsable in Git; statements below that those paths are absent describe the earlier audited commit. Test numbers below come from the existing reports. This documentation update did not rerun the engine tests or build an APK.

## 1. Goal and present result

The owner's goal is a complete, reproducible source tree that can be reviewed by the rights holder and used to rebuild Dungeon Hunter 2 for current Android. Earlier work considered a Galaxy Z Fold7; the current work validates on official Android 17 emulators and does not require or perform a Fold7 test. For the native reconstruction route, completion means an ARM64 engine built from reconstructed source, with the game systems and assets integrated and behavior tested on Android. The final engine must run without translating the supplied ARM32 engine binary. Original source text, comments and the exact historical studio project cannot be recovered merely by decompiling a binary; a faithful reconstructed implementation is the practical target. See [the current Android 17 results](docs/ANDROID-17-EMULATOR-RESULTS.md) before using the historical compatibility status below.

**That goal is not complete.** We have a substantial binary/source evidence base, buildable Java and JNI support work in the earlier recovery package, and three published C++ engine components. Math, resource readers/BRES access, and animation accessor/search routines have recorded comparisons against original ARM32 instructions. Mesh payloads can be decoded and exported. Images/materials, scene/controller integration, rendering, most engine services and gameplay still need implementation and validation.

A separate route preserves the original ARM32 engine and runs it inside an ARM64 compatibility runtime. It has published Fold7 test APKs and device diagnostics. It is useful both as a nearer-term restoration attempt and as a possible source of reference behavior. Its existence does not establish a complete native-source rebuild.

## 2. Parallel work and preservation boundary

The owner explicitly requested that this handoff preserve the other agent's APK rebuilding work.

| Track | Purpose | Current work locations | Current result |
| --- | --- | --- | --- |
| Native source reconstruction | Recover evidence and implement reviewed ARM64 source module by module | `port/engine-math/`, `port/engine-resources/`, `port/asset-payloads/`, associated `reports/` and recovery package | Buildable components and isolated behavior checks; no integrated native game |
| APK compatibility | Host original ARM32 libraries with ZettaBridge/Dynarmic and repair specific platform/binary failures | `compatibility/`, `compatibility-work-test*.zip`, `unpack_compatibility.py`, Fold7 release assets | Test 5 published; successful loading/gameplay after its latest repair still unverified |
| Android emulator testing | Establish a repeatable launcher test environment for the compatibility APK | `compatibility/work/fold7-build/ANDROID_TESTING.md` and its smoke runner/results | Emulator booted; Test 5 installation failed; game was not launched |

This change adds only `RECONSTRUCTION-HANDOFF.md`. It does not change code, build inputs, signing fixtures, runtime bundles, compatibility snapshots, existing documentation or releases.

Continuation rules for reconstruction contributors:

1. Use a separate checkout or worktree and a reconstruction branch. Fetch the latest remote before committing; preserve concurrent commits by merging or rebasing deliberately.
2. Keep initial changes within a new or existing reconstruction module and its own evidence/tests. Treat the compatibility track as independently maintained; coordinate before modifying its files or shared entry documents.
3. Stage explicit paths and inspect the complete staged diff. Do not commit another agent's uncommitted files, replace the repository with an older local snapshot, force-push, or regenerate compatibility ZIPs from a reconstruction checkout.
4. Keep binary identities separate. The reconstruction harnesses require the exact original engine hash listed below. Test 5 intentionally patches that engine and must not be substituted as their oracle.
5. Share useful layouts, traces and findings through narrowly scoped commits. Integrating reconstructed functions into the translator or APK is a separate change requiring boundary and device checks.

These are collaboration instructions for this handoff, not a replacement for the compatibility agent's own build plan.

## 3. Where the work actually lives

The GitHub tree at the reviewed commit contains the newer engine checkpoints, their selected reference assembly and reports, and the compatibility work. It does **not** contain the full earlier recovery tree. In particular, `recovered/`, recovery-root `tools/`, `port/android-java/` and `port/nativeinterface/` are absent from that GitHub snapshot. Some older overview links describe logical paths inside the downloaded recovery package; they should not be interpreted as existing GitHub directories.

| Material | Available location / start point |
| --- | --- |
| Math source, original-address mapping, reference assembly and tests | [Math module](port/engine-math/README.md) |
| Readers, checked BRES views, table access and inspector | [Resource module](port/engine-resources/README.md) |
| Mesh/animation views, exporters, layouts and selected original assembly | [Payload module](port/asset-payloads/README.md), [format tables](port/asset-payloads/FORMATS.md), [sample exports](port/asset-payloads/examples/README.md) |
| Consolidated reconstruction state and findings | [Status](docs/STATUS.md), [findings](docs/FINDINGS.md), [porting plan](docs/PORTING.md) |
| Compatibility source, test history and device procedure | [Compatibility overview](compatibility/README.md), [build instructions](compatibility/BUILDING.md), [Test 5 diagnosis](compatibility/work/fold7-build/TEST5.md) |
| Compatibility complete snapshots and APKs | [Releases](https://github.com/Noamcelermajer/DH_sc/releases), root `compatibility-work-test*.zip` and [unpacker](unpack_compatibility.py) |
| Full earlier Java/smali/pseudocode/DWARF/XML/shader recovery, JNI source and recovery tools | [Existing recovery folder](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW), beginning with `START-HERE.txt` |
| Exact supplied APK and ten supplied cache parts | Recovery folder's `Original-Inputs`; not committed as reconstruction source |

The earlier recovery folder contains `Dungeon-Hunter-2-Source-Recovery.zip`, `assembly.tar.gz`, `symbols.tar.gz`, `Component-Validation-Artifacts.zip`, instructions and checksum records. Its published source/validation ZIPs were updated through the math milestone. The subsequent resource and payload milestones are on GitHub; use current GitHub files for those components rather than assuming the older ZIP contains them. No Drive content was changed or reverified during this report update.

Large game art/audio, original APK/ELFs and toolchain binaries are separate inputs, not evidence that a GitHub-only clone is already a complete game project. Preserve [RIGHTS.md](RIGHTS.md) and applicable notices; recovery does not confer a new license for the game.

## 4. Input identity and recovery findings

### 4.1 Exact binary reference

| Input | SHA-256 |
| --- | --- |
| Supplied APK | `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200` |
| Original `libDungeonHunter2.so` | `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80` |
| Original `libStormGLOFT.so` | `be6beaab782944e5ce39cca8e850fd03de654e4236c9329621043adcbee291e1` |
| Original `libnativeinterface.so` | `180b582cbb7e7004c94fe771f8c8013dad4c74a9317ee4baff08c542da4ff0da` |

The package is `com.gameloft.android.GAND.GloftD2SS`. The examined libraries are ELF32 ARM; the APK's principal native directory is `armeabi-v7a`. The original manifest declares minSdk 8, no explicit targetSdk and launcher `Zirconia_DRM`. Newer compile-SDK metadata and save-restoration/support classes also exist in the supplied APK. Its filename does not establish untouched studio-release provenance. Hashes identify the actual analyzed input, without assuming who added later changes.

### 4.2 Native metadata is unusually useful, but incomplete as source

The main engine preserves 31,021 unique named function symbols representing 31,018 physical starts/ranges, 54,463 relocations, 1,628 vtables, 30,992 unwind-index entries and 867 build-source filenames. Named gameplay groups include character behavior, player management, inventory/loot, levels, quests and savegames. They provide a map for reconstruction, not recovered field layouts or implementation text.

All **6,488,258 executable bytes** across the three native libraries were retained and round-tripped against their ELFs. That includes executable gaps and support-library bytes whose decoding is uncertain. Exact byte preservation is stronger than losing those regions, but does not prove their semantics have been understood.

| Library | Distinct original starts attempted | Ghidra output functions | Pseudocode emitted | Failed exports |
| --- | ---: | ---: | ---: | ---: |
| Main engine | 31,018 | 31,795 | 31,794 | 1 |
| Storm support | 1,498 | 3,333 | 3,332 | 1 |
| Small JNI support | 10 | 31 | 31 | 0 |

Ghidra totals include heuristic functions and PLT thunks. Storm's 1,500 declared ranges include two aliases that share other starts. The engine failure is `ft_gzip_file_fill_output`; Storm's inferred PLT-area `__gnu_thumb1_case_uqi` timed out. Their assembly/bytes remain available. Generated pseudocode has warnings, inferred types and unresolved indirect calls and is not a compilable replacement engine.

DWARF exports contain 11,621 type records and 32 compilation units, categorized by paths as seven licensing/online glue, 22 STLport and three libgcc units. No gameplay compilation unit was identified. The debug records help with support/runtime declarations; they do not restore the engine's gameplay source.

A concrete decompiler error affects 16 of 19 vector/quaternion pseudocode bodies: imported floating-point helpers were incorrectly marked as non-returning, truncating control flow. Complete assembly was used for the math reconstruction. Future decompilation should correct helper metadata/calling conventions and regenerate affected analysis; do not treat the archived pseudocode as authoritative when it contradicts the instructions.

### 4.3 Java and JNI recovery

The earlier recovery accounts for all **357 DEX classes, 2,432 defined methods and 46 native declarations**, with Java exports and exact smali retained. JADX synthesizes additional resource files; its raw output initially fails to compile in vendor billing code.

The separate repaired Java tree has 288 source files compiling to 365 class files under Java 17/Android SDK 35, with zero errors and 15 legacy warnings. D8 produced a 571,752-byte DEX and preserved all 46 declared native contracts. Focused vendor/Gameloft Base64, CRC and installer-integrity tests address identified decompiler damage. This does not validate all calls from native code into Java: `GetFieldID`, `GetMethodID`, reflection, obfuscated names, callbacks, threads and exception handling still require an audit.

The reconstructed small JNI library preserves ten exports, including four JNI methods, and builds for ARM64 with 16 KiB load alignment. Host semantic/JVM JNI tests and recorded original-ARM32 comparisons cover SHA-1, passphrase and license-file routines. Original undefined behavior, including a UTF-8/`wcslen` mismatch, is replaced by documented bounded behavior; arbitrary allocator-slack reads are not claimed equivalent. Original licensing decisions were preserved, not newly bypassed. This support component does not establish working vendor services or a complete engine.

The compatibility phone report's 36 dynamically registered native methods and the recovery inventory's 46 declared native methods describe different inventories; neither number alone verifies the full Java/native contract.

### 4.4 Cache and readable game data

The source-recovery input consists of ten 30 MiB parts totaling **314,572,800 bytes**. It is a truncated ZIP prefix, ending inside `m_world_map.wav`. At least **368,651 compressed bytes** are missing to complete that file, followed by an unknown archive tail and the missing central directory. Original total file count is unknown.

Local-header recovery validated **5,839 complete files and 241 directories**, totaling 509,330,036 uncompressed bytes. Exported entries passed their decompressed-length and CRC-32 checks; incomplete entries were excluded. The partial corpus is useful, but cannot establish that all levels/audio/assets are present. This describes the ten-part recovery input, not necessarily the complete archive already imported on the owner's phone. Later emulator download failures likewise do not prove a defect in the phone's cache.

| Format identified from bytes | Recovered count | Interpretation |
| --- | ---: | --- |
| BRES `.bdae` | 2,901 | Binary scene/resource containers; decoded in increasing depth by the C++ modules |
| `.mgp`, `.mgx`, `.mvp`, `.mvx`, `.mlx` XML | 629 / 250 / 598 / 253 / 331 | Gameplay/visual modules, links, transforms and level placement |
| Other XML | 45 | Light sets, tweakers and other configuration |
| `.tga` files containing `BTEXpvr` | 234 | Eight-byte wrapper followed by a legacy PVR header; not ordinary TGA |
| Actual TGA / PNG | 8 / 121 | Conventional image formats |
| Complete WAV / VoxN / MP4 | 188 / 12 / 3 | Audio and intro video; VoxN decoding remains unresolved |
| `shaders.pak` | 1 | ZIP with 32 GLSL sources and two configuration files |

There are **2,164 exact recovered text/shader files** with provenance hashes. Three original XML documents contain duplicate attributes and are preserved unchanged. The old parser's first/last-attribute behavior needs evidence before selecting a compatibility policy. References to `.max` editor files and alternate `data/iphone/3D/` paths do not establish that those editor sources are present; inventory path references instead of guessing replacements.

## 5. Implemented engine components and validation

| Component | Implemented scope | Recorded result | Evidence |
| --- | --- | --- | --- |
| Math | 20 original vector/quaternion/matrix function starts | 21,477 original ARM32 versus compiled ARM64 comparisons; zero mismatches; 1,704 original instruction addresses exercised | [Module](port/engine-math/README.md), [report](reports/engine-math-validation.json) |
| Readers / BRES | 35 complete bodies: 11 memory-reader, eight subfile-reader and 16 database-accessor methods; whole-buffer initializer branch | 10,449 reader comparisons; all 2,901 BRES files; 2,577,206 fixups, 5,154,412 native pointer values and 27,812 root pointers checked; zero mismatches | [Module](port/engine-resources/README.md), [comparison report](reports/engine-resources-validation.json), [build report](reports/engine-resources-build.json) |
| Animation access/search | 18 complete accessor bodies plus eight complete typed-search bodies | 271,970 original ARM32 versus compiled ARM64 comparisons; zero mismatches; 456 original instruction addresses exercised | [Module](port/asset-payloads/README.md), [ARM report](reports/asset-payloads-arm-validation.json) |
| Mesh/raw-animation decoding | Immutable checked views, immediate/deferred bytes inside complete files, local OBJ and raw-key JSON export | 10,924 meshes; 1,641,664 vertices; 1,088,422 triangles; 890,301 time keys decoded; nine unsupported type-1 geometry records listed | [Corpus report](reports/asset-payloads-cache-validation.json), [layouts](port/asset-payloads/FORMATS.md) |
| Target mesh execution | Host versus executed compiled ARM64 decoder | 25,682 comparisons on 1,149 meshes; zero mismatches | [ARM64 mesh report](reports/asset-payloads-arm64-mesh-validation.json) |
| Safety / builds | Host and NDK r29 ARM64 components | Resource 10,000 corruption/truncation probes; payload 5,000 probes; ASan/UBSan pass with leak detection disabled | Module READMEs and build reports |

These comparison categories have different scopes and must not be added together as a measure of whole-game correctness. Likewise, function-start counts cannot give a meaningful overall completion percentage: caller integration, inlined behavior, ownership, rendering and gameplay remain large unknowns.

### 5.1 Math conventions that must survive integration

Vector rotations use degrees; quaternion Euler/angle-axis construction uses radians. Quaternion multiplication has reversed Hamilton-product operand order. The two matrix-returning forms intentionally produce transposed orientations relative to each other. Float/double rounding, original normalization branches, interpolation thresholds and selected zero/NaN behavior are preserved. A generic library substitution can silently change these conventions.

The tests execute original instructions and compiled ARM64 code in Unicorn. Imported ARM arithmetic/libm calls use a controlled host dependency model; historical Android libm has not been validated. Non-NaN floats are compared by bits and NaNs by classification. Instruction-address coverage is not exhaustive input/path coverage. See the math README before changing optimization flags or replacing operations.

### 5.2 BRES and reader rules

BRES serialized references occupy four bytes. The fixup table identifies pointer-field locations; its first entry is the header's own table pointer and has special relocation handling. Writing eight-byte pointers into those fields corrupts adjacent data. The port retains immutable serialized bytes and resolves separate native pointers, including tests with ARM64 addresses above 4 GiB.

The root exposes 13 resource libraries. Relevant count/pointer pairs are image `0x4c/0x50`, effect `0x54/0x58`, material `0x5c/0x60`, geometry `0x68/0x6c`, controller `0x70/0x74` and animation `0x24/0x28`. Their serialized strides are 20, 116, 36, 16, 12 and 32 bytes respectively. These sizes must not be treated as ARM64 object layouts.

Only whole-buffer BRES relocation is implemented. External bases, split allocations and original runtime ownership remain unfinished. Readers preserve original seek/callback quirks, including negative cursor states, ignored seek failures and subfile asynchronous advancement by requested rather than actual read count. Unsafe copies are deliberately rejected. Buffers/readers/names are borrowed; their lifetimes must be managed by callers.

### 5.3 Mesh and animation rules

All decoded type-0 meshes use interleaved buffers. Actual attribute types are float and unsigned byte. Primitive mapping `[6,4,3,1,2]` contains engine enums, not GL constants; observed primitive buffers are triangle lists. OBJ output retains local coordinates, material names, normals, first UV set and winding, without scene transforms or textures.

`prince_modular.bdae` is the sole recovered file with the deferred mesh-buffer form: 173 meshes use 16-byte on-demand records. The decoder resolves their complete-file byte offsets. It does not recreate file-loading callbacks, cached GPU buffers, allocation or reference counts.

Animation entries use signed offsets relative to the **pointer word itself**, independently of BRES base-relative fixups. The code decodes embedded and deferred segments contained in the complete file. Compressed key-time getters use double precision while search uses float precision and integer truncation, allowing millisecond differences. Typed search retains the original equality, boundary, interpolation and sampler-zero type-selection behavior.

Raw keys are not finished playback. Channel/track semantics, default/scale variants, value interpolation/application, generic cached dispatch, clips, skeletal controllers and skinning remain unresolved. Four additional mesh/relocation routines supplied layout evidence; their complete GPU/ownership behavior was not reconstructed or claimed as equivalent. The ARM64 mesh test compares two implementations of the new decoder, not the original GPU constructor.

## 6. Compatibility APK state at the snapshot

The latest published test is [v1.0.2-fold7-test5](https://github.com/Noamcelermajer/DH_sc/releases/tag/v1.0.2-fold7-test5). It uses ZettaBridge/Dynarmic to execute the ARM32 engine, with targeted Storm/Java/platform repairs. Its runtime page-mapping assumptions remain separate from the 16 KiB load alignment of the reconstructed modules; alignment alone does not establish 16 KiB runtime support.

| Milestone | Observed evidence / change | Remaining limit |
| --- | --- | --- |
| Native startup | Two Storm uses of obsolete private linker fields were reproduced and repaired; phone reports confirm 4096-byte pages, native loading and JNI startup | All runtime boundaries and sustained execution still need testing |
| Test 2 | Repaired old MediaStore `*` column query | Later phone evidence reached the cinematic and fairy loading screen, then failed |
| Test 3 / Test 4 | Original-engine directory-path bounds abort reproduced; Test 4 adds a narrow file guard | Test 4 phone report confirms guard execution, then a different failure |
| Test 5 | Original `CFileSystem::open` misclassified POSIX absolute paths and duplicated the cache root when reopening the Prince model; five ARM instructions add leading-`/` recognition | 13 path cases, five earlier file cases, native/package checks pass; phone load/gameplay retest still required |
| Emulator supplement | API 30 x86_64 AVD booted with software CPU/SwiftShader; APK install returned package-service `Broken pipe (32)` | Game was not launched; Android 16/Fold7 rendering was not reproduced |

Test 4 diagnostics strongly implicate model input during deferred loading. The saved register/stack data is imprecise and is not a fully unwound exact-instruction backtrace. An earlier `GL_INVALID_ENUM` also remains open, but its existence alone does not establish that graphics caused this crash. Read [Test 5](compatibility/work/fold7-build/TEST5.md) and [bridge assessment](compatibility/work/fold7-build/BRIDGE-ASSESSMENT.md) for those distinctions.

Test 5 is the first compatibility build to patch the engine binary itself; statements about byte-identical engines apply historically to Tests 1–4. Its released APK hash is `e6b81ec649e25bb32c6ec7f3f477d5ef1c2a79b7af43b7ca7643518c5f5e1b8d`. The APK, work ZIP and checksum asset are retained at the release above. No new APK is provided by this report.

Historically, the compatibility agent's next discriminator was a same-settings Fold7 retest and diagnostic export. The current Android 17 emulator work has since fixed the wrapper's repeated model path and reached short saved-level movement on the 4 KiB image. Russian menus, attack/save behavior, sustained gameplay and 16 KiB host pages remain unresolved or unverified. The [current result](docs/ANDROID-17-EMULATOR-RESULTS.md) supersedes that older device-test plan. Older sections in compatibility documents retain historical status; consult the newest test-specific section before drawing conclusions.

## 7. Reconstruction roadmap and acceptance gates

The next bounded deliverable is **checked image/material decoding and a minimal renderer**, starting from the working mesh views. Scene/controller reconstruction can follow or accompany it in a separate module. A rendering milestone should advance the engine source route while leaving compatibility fixes independently reviewable.

| Priority / milestone | Work to implement | Evidence needed to call it complete |
| --- | --- | --- |
| R1: images/textures | Trace image references and BTEX/PVR/TGA parsing; recover texture descriptions, mip levels, flags, alpha/orientation and byte lengths | Every available texture classified; checked headers/data ranges; explicit unsupported cases; pixel or upload comparisons for supported formats |
| R2: materials/effects | Decode nested material/effect records, image bindings, samplers, shader parameters and vertex attribute mappings | Real mesh material names resolve to reviewed effects/images; state mapping and representative outputs checked against original behavior |
| R3: minimal renderer | Use existing mesh APIs, decoded textures/materials and recovered GLSL; add reviewed transform, GPU buffer and resource lifetimes | Candle flame and menu-swamp examples draw on an actual Android GLES context, with recorded screenshots/logs, correct winding/UV/alpha and resize/resume checks |
| R4: scenes/controllers/animation | Decode hierarchy/transforms, cameras/lights, controller/skeleton/skin data, clips and track application | Static scene and animated character behavior match controlled reference captures; key timing, binding, transforms and lifetimes tested |
| R5: unresolved resource cases | Nine type-1 geometries, separate-stream or other absent formats, external/split resource loading and shared ownership | Individual fixtures/regressions for each implemented case; remaining exceptions explicitly listed |
| R6: platform integration | Audit JNI/reflection names; integrate input, storage import, audio/video, threading and Android lifecycle | Clean ARM64 app startup and asset loading on device; pointers/threads/callbacks safe; folding, interruption and save paths exercised |
| R7: game systems | Reconstruct configuration/Lua integration, characters, movement/combat, items, quests, level transitions and persistence | Menu-to-game loop and representative progression/save/load cases checked against original behavior |
| R8: restoration release | Complete asset manifest, deterministic build recipe, source/dependency notices, production packaging and sustained testing | Independent rebuild plus the device acceptance matrix below; documented remaining service/feature limitations |

These are planned milestones, not completed features or estimates. Complete-cache acquisition and rights-holder tooling inquiries can proceed alongside the bounded implementation work. The missing archive tail need not block a renderer prototype using intact fixtures, but it blocks a claim of complete restoration.

### 7.1 Concrete starting symbols for the next contributor

The following entry points were identified in the original engine's function index. Addresses are **ELF virtual addresses for the exact original hash**, not process addresses or offsets into a patched APK. They are starting points for investigation, not newly reconstructed routines.

| Investigation | Original symbol (signature abbreviated) | ELF VA |
| --- | --- | --- |
| PVR description | `glitch::video::CImageLoaderPVR::loadTextureHeader(IReadFile*, STextureDesc&) const` | `0x00605b10` |
| PVR upload/data | `glitch::video::CImageLoaderPVR::loadTextureData(...) const` | `0x0060623c` |
| Image dispatch | `glitch::video::CTextureManager::createImageFromFile(IReadFile*)` | `0x005e913c` |
| File texture loading | `glitch::video::CTextureManager::loadTextureFromFile(...)` | `0x005ecba4` |
| Material record construction | `glitch::collada::CColladaFactory::createMaterial(...)` | `0x006323d0` |
| Attribute/material binding | `glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(...)` | `0x00634520` |
| Effect/material renderer | `glitch::collada::CColladaFactory::createMaterialRenderer(..., char const*, char const*, ...)` | `0x00636c8c` |

The PVR routines have 1,056 and 184 recorded bytes; image dispatch has 92. Recover the complete reachable dependencies rather than assuming a short top-level routine contains the whole texture implementation. The function index also identifies ATC and DDS loader paths. Their presence is evidence of engine capabilities, not evidence that those formats occur in this recovered corpus.

Recommended first sequence:

1. Classify the 234 BTEX/PVR files and eight actual TGA files by bytes, not `.tga` extension. Trace the complete PVR header/data path and `STextureDesc` field accesses; document what each flag actually controls.
2. Add a checked immutable texture view/exporter consistent with existing BRES views. Retain four-byte serialized offsets; use native handles separately. Declare fixture, format and allocation limits explicitly.
3. Resolve image/effect/material references and primitive material names using their original strings and hash logic. Do not guess channel enums, blend states or normalized byte meanings.
4. Differential-test pure parsing/state-selection routines with original ARM execution where possible. For GPU paths, capture actual API calls/output separately and state that limit; host self-consistency checks alone are not original behavior checks.
5. Draw an intact small fixture using the existing `dh2_mesh_open`, `dh2_mesh_attribute`, `dh2_mesh_primitive`, `dh2_attribute_read` and `dh2_index_read` interfaces. Begin with candle flame, then menu swamp, then the Prince's deferred-buffer case. These are component fixtures, not a playable game milestone.
6. Commit source, original-address/hash mappings, assembly, reproduction commands, test reports and unsupported cases together. Update this roadmap only after a milestone has reviewable evidence.

## 8. Reproduce the current component work

Use a clean reconstruction checkout. Inputs and generated reports belong in a separate workspace; do not overwrite committed reports merely to run a smoke test. These commands operate on the published C++ components, not the compatibility APK build.

```sh
git clone https://github.com/Noamcelermajer/DH_sc.git DH_sc-reconstruction
cd DH_sc-reconstruction
git switch -c reconstruction/next-module

python -m venv /absolute/path/to/dh2-venv
. /absolute/path/to/dh2-venv/bin/activate
python -m pip install -r port/engine-math/requirements.txt
python -m pip install -r port/engine-resources/requirements.txt
python -m pip install -r port/asset-payloads/requirements.txt

python port/engine-math/build.py --ndk /absolute/path/to/android-ndk-r29
python port/engine-resources/build.py --ndk /absolute/path/to/android-ndk-r29
python port/asset-payloads/build.py --ndk /absolute/path/to/android-ndk-r29
```

Recorded reconstruction dependencies include Python 3.12, a host C++17 compiler, NDK r29, pyelftools 0.33, Capstone 5.0.9 and Unicorn 2.1.4. Consult each module's requirements and README for its exact harness assumptions. ARM64 binaries are executed in a controlled emulator for comparisons; successful compilation does not run them on a phone.

The exact original ELF and intact cache fixtures must be supplied separately. For the full corpus, recover the ten numbered parts with the recovery-package tools; their expected partial-archive exit code is 2. Do not accept a differently patched engine as the reference.

```sh
python port/engine-math/tests/differential.py \
  --original /absolute/path/to/original/libDungeonHunter2.so \
  --report /absolute/path/to/new-results/math-validation.json

python port/engine-resources/tests/differential.py \
  --original /absolute/path/to/original/libDungeonHunter2.so \
  --assets /absolute/path/to/cache/files \
  --report /absolute/path/to/new-results/resource-validation.json

cd port/asset-payloads
python tests/audit_cache.py --cache /absolute/path/to/cache/files \
  --report /absolute/path/to/new-results/cache-validation.json
python tests/differential.py --original /absolute/path/to/original/libDungeonHunter2.so \
  --cache /absolute/path/to/cache/files \
  --report /absolute/path/to/new-results/arm-validation.json
python tests/arm64_mesh.py --cache /absolute/path/to/cache/files \
  --report /absolute/path/to/new-results/arm64-mesh-validation.json
python tools/export_assets.py /absolute/path/to/candle_flame.bdae \
  --obj /absolute/path/to/new-results/candle-flame.obj \
  --animation-json /absolute/path/to/new-results/candle-flame.animations.json
```

Sanitizer commands and corpus-selection options are in the module READMEs. Leak detection was disabled for recorded ASan/UBSan input probes due to the execution environment; those results are not leak tests. The payload build/harness reuses sibling resource code, so preserve that sibling directory layout.

For full earlier evidence, extract the recovery source ZIP into its own directory. Place `assembly.tar.gz` and `symbols.tar.gz` next to its `recovered/native/bundles/manifest.json`, then run `python tools/unpack_native.py` and `python tools/verify_recovery.py` **there**. Those root recovery tools are not present in the reviewed GitHub checkout. Use its `docs/REPRODUCING.md` for APKTool 2.12.1, JADX 1.5.6, Ghidra 11.0.3 and Java 17 reproduction steps. Copy/merge only needed reconstruction material through a reviewed branch; do not overlay that older tree onto the compatibility checkout.

For compatibility builds or emulator work, follow the compatibility agent's [build guide](compatibility/BUILDING.md), [Test 5 notes](compatibility/work/fold7-build/TEST5.md) and [Android testing guide](compatibility/work/fold7-build/ANDROID_TESTING.md). Their ZettaBridge/Dynarmic/runtime dependencies and historical path layout are a separate recipe.

## 9. Unresolved work and final acceptance

Outstanding engineering includes coherent native classes and virtual dispatch, string/STL/refcount ownership, modern libc/platform boundaries, full asset schemas, GPU lifetimes, scene and animation application, audio/VoxN, Lua/configuration behavior, gameplay and save formats. The engine imports old Bionic globals and ARM helpers; Storm uses ARM-specific hooks. Rebuild these boundaries deliberately under one modern toolchain rather than widening ARM32 structures or importing their private platform assumptions.

The complete cache is still needed. A rights-holder handoff should request archived headers, engine projects, resource schemas, asset export tools and shader pipelines, using the preserved source filenames/subsystem names to focus the search. Studio decisions are also needed for unavailable licensing, billing, online or multiplayer services; no working service restoration is asserted by this work.

| Completion gate | Required evidence |
| --- | --- |
| Reproducible native source build | Clean independent checkout builds ARM64 engine/support code and APK with pinned dependencies, known asset inputs and documented rights; source route executes without original-engine translation |
| Startup/loading | Repeated cold/warm startup, intro-to-menu/game transition and character/level loading on SM-F966B |
| Gameplay/progression | Movement, combat, inventory/equipment, quests and area transitions checked against controlled reference behavior |
| Persistence | Save, force-stop, restart and reload retain expected progress; existing-save compatibility or conversion is explicit |
| Graphics/input | Correct shader/texture/alpha/transform behavior and touch alignment on folded/unfolded displays; context loss/resize tested |
| Lifecycle/audio | Background/resume, screen lock, interruptions, audio and video-to-game transition tested |
| Stability | At least a 30-minute recorded session with memory/frame timing, followed by save/reload; investigate remaining crashes or corruption |
| Scope of support | Explicit device/Android/service/multiplayer coverage and any unsupported asset cases; no generalization from one device alone |

Every further milestone should preserve the exact input hashes, original symbol/address/range mappings, source version, commands, modelled dependencies, known safe deviations and measured results. Component checks and full-game checks should remain separately identifiable. Until the integration and device gates pass, this is a reconstruction project with verified components and a separate experimental compatibility APK, not a completed source restoration.
