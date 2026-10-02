# Recovery status — updated 2026-10-02

**Source scripts in the Android APK:** The 730,019-byte source package now
initializes three exact recovered shared scripts inside the character preview.
Imports, budget/error rejection and recovery pass in the same process on Android
17 with both 4 KiB and 16 KiB pages. The same APK passes textured character
import, seeking, mixing, Play and Pause. Native game objects, real combat and full
source gameplay remain unfinished. See [APK source and evidence](../port/android-app/SCRIPT-INTEGRATION.md).
Earlier checkpoints below retain their original scope and binary identities.

**Structured field IDs:** Actual original static initialization and lookup bodies
establish 636 entries in 71 registrations and 693 lookup cases. Owned source
`GetPyStruct` uses the same map as `GetPyOID`; 1,272 alias queries and controlled
execution of three unchanged AI/skill/combat shared scripts pass on host, strict
sanitizers and both Android 17 page sizes. Array/constants/arithmetic/parse
regressions pass. Real combat, typed entity access, source APK integration and
full gameplay remain unfinished. See [scope](../port/pydata-names/STRUCT-FIELDS.md).
The earlier checkpoint entries below retain their original execution scope.

**Array name IDs:** Original readers decode 8,863 names in 71 tables from 35
files; 266 original ID lookups pass. Source ARM64/host matches every name and
those lookup cases. Owned `GetPyOID` imports pass all authored queries on host,
strict host sanitizers and both Android 17 page sizes; malformed/OOM imports
retain prior IDs. Updated constant, arithmetic and parse-corpus regressions pass.
Array-record count agreement, Structs fields, script execution and source
gameplay remain unfinished. See [scope](../port/pydata-names/README.md).

**Integer script constants:** Original ARM32 reader tracing covers all 27 constant
files; 26 load fully with 5,608 entries. Source ARM64/host decoding matches those
entries, and owned `GetPyCst` imports pass every authored Lua query on host and
both Android 17 page sizes. Strict host sanitizers pass, including allocation
failure rollback. The mixed sound file is rejected instead of reproducing the
original partial/misread import. Original map lookup and scripts are not executed.
Array IDs, structured data, includes and source gameplay remain unfinished.
See [scope](../port/pydata-constants/README.md).

**Source scripting runtime:** Official Lua 5.1.4 source plus an owned C wrapper
uses the original observed float32 number profile, builds for ARM64/x86_64 and
passes host selftests/sanitizers. All 504 original numeric arithmetic vectors
match source evaluation on host and both Android page sizes. The exact x86_64
runner passed standard library, memory/instruction budget and error recovery
tests on Android 17 with 4 KiB and 16 KiB pages. It parses 218 original scripts
plus the repaired override; the unchanged malformed sandworm fails as expected.
All staged inputs and runner hashes were verified. Game scripts were not
executed. Nine numeric callbacks pass 2,375 original ARM32/compiled ARM64/host
comparisons and integrated authored Lua assertions on host and both Android
test environments. The runtime passes strict sanitizers after a documented
table-key safety repair; preserved vendor bytes remain exact. Gameplay object
callbacks, APK integration and full gameplay remain unfinished.
See [scope and evidence](../port/lua-runtime/README.md).

**Cache script source:** 219 exact readable Lua source files (900,493 bytes)
are preserved from the complete cache: AI, skills, objects, levels and a test.
218 originals pass Lua 5.1.5 syntax; the sandworm script's missing parenthesis
is repaired in a separate one-character override. No script was executed.
The native scripting bridge and full game remain unfinished.
See [source and verification](../recovered/scripts/README.md).

**Script bindings:** Six original ARM32 registration callers now expose 175
distinct function names; 117 match globals read by recovered scripts. The trace
records 568 function/method requests, including inherited repeats. Binder
installation and standard library initialization are explicit stubs; native
callback installation in that trace is stubbed. Nine numeric callbacks now have
separately checked source implementations; script execution remains unfinished.
See [the trace](../reports/lua-registration-trace.json).

**Current animation checkpoint:** Checked skins and absolute poses now resolve
531 local skins and 333 of 342 player clips (327 animated). Float animation
values passed 4,705 original ARM32/compiled ARM64/host cases. The reconstructed
range timeline passed 400 sequences/4,000 updates and drives the source Android
preview, whose exact current APK passed advancing Play, stable Pause and visible
pose changes on Android 17 with both 4 KiB and 16 KiB pages. A separate original
mixing component passed 842 three-way cases and 24,000 sanitizer API calls;
Absolute layers now integrate normalization, vector mixing and quaternion blending
in the Android preview. The original scheduler, relative application and events
remain open. See [layers](../port/animation-layers/README.md),
[timeline](../port/animation-timeline/README.md),
[mixing](../port/animation-mixing/README.md) and
[Android runtime evidence](../port/android-app/layers-runtime-validation.json).
Full source-built gameplay remains unfinished.

**Original transition state:** The standalone animator transition projection
passed 400 sequences, 300 transition requests and 4,800 original ARM32/compiled
ARM64/host updates, including child selection order before normalization.
Another 100,000 valid/corrupted operations passed sanitizers. Child timelines
and callback effects were explicitly stubbed; this is not a complete scheduler
or Android integration. See [transition scope](../port/animation-transition/README.md).

**Original movement delta:** Explicit-position animation reset/calculation
passed 800 resets and 4,000 calculations against original ARM32 and compiled
ARM64/host source, including 1,600 repeated-timestamp checks. Another 100,000
valid/corrupted operations passed sanitizers. This standalone component has
no root-track sampling, scene movement or collision integration yet.
See [movement scope](../port/animation-motion/README.md).

**Original completion dispatch:** Standalone callback registration/checking
passed 384 setters, 768 checks and 144 controlled callback dispatches against
original ARM32 and compiled ARM64/host source, plus 10,000 sanitizer checks.
It preserves deferred notification and clearing after field changes during
callback. The actual character/game callback bodies and triggered events
remain unfinished. See [completion scope](../port/animation-completion/README.md).

**Original ending timing:** Extra-time calculation and ending notices passed
495 calculations, 1,980 filtered blender notices and 495 ordinary animator
notices against original ARM32 and compiled ARM64/host state. Another 20,000
calculation/notification calls passed sanitizers. Active lookup is explicitly
stubbed; game callback effects and Android integration remain open.
See [ending scope](../port/animation-ending/README.md).

**Complete-cache note:** a separately supplied local cache ZIP is valid and contains 6,833 files, including the previously truncated WAV. See [COMPLETE-CACHE.md](COMPLETE-CACHE.md). The original recovery counts below document the earlier ten-part partial input; newer module checks explicitly identify their full-cache scope.

This status combines this Git checkout with results from the separate earlier recovery package. A selected, hash-verified import makes the repaired Android Java source, reconstructed JNI component, recovery tools and reports browsable in Git. Later exact imports add 44 native pseudocode/index files, 721 raw Java/smali files, 3,625 native assembly files and 71 symbol records, each with per-file hashes. The compressed native bundles remain external. Exact copies of the 2,164 shader/configuration resources and 48 native DWARF/debug exports are now in Git with unchanged historical provenance; see [the text/debug import](RECOVERED-TEXT-AND-DEBUG.md). The archived pseudocode and assembly are not a compilable engine; the Android 17 gameplay APK still depends on the original ARM32 engine. See [the import ledger](RECOVERY-SOURCE-IMPORT.md).

## Final export accounting

| Library | Original distinct function starts | Starts attempted | Ghidra output functions | Pseudocode emitted | Failed exports |
| --- | ---: | ---: | ---: | ---: | ---: |
| `libDungeonHunter2.so` | 31,018 | 31,018 | 31,795 | 31,794 | 1 |
| `libStormGLOFT.so` | 1,498 | 1,498 | 3,333 | 3,332 | 1 |
| `libnativeinterface.so` | 10 | 10 | 31 | 31 | 0 |

Ghidra totals include heuristic functions and PLT thunks, so they exceed original named-start totals. Storm has 1,500 declared ranges but 1,498 distinct starts because two zero-sized division aliases share other bodies. Export completion is not semantic correctness. Emitted warnings and inferred types require review.

The engine's `ft_gzip_file_fill_output` caused the decompiler process to die. Storm's inferred PLT-area `__gnu_thumb1_case_uqi` timed out. Assembly evidence remains for both. All 6,488,258 executable bytes across the supplied libraries were preserved and round-tripped exactly. Every final UTF-8 byte index, shard reference and normalized ELF address passed verification.

## Source and test results

- Original Android recovery: all 357 classes, 2,432 defined methods and 46 native declarations; Java and exact smali preserved.
- Repaired Java: 288 source files compile to 365 class files, with zero errors and 15 legacy warnings under Java 17 / Android SDK 35. D8 produces a 571,752-byte DEX. All 46 declared native contracts match the original exactly.
- Current SDK stub check: the imported 288 Java files also compile with JDK 25 (`--release 17`) against Android SDK 37.0 into 364 class files, with 15 legacy warnings. Build Tools 35.0.0 D8 produces a 572,124-byte DEX and all 46 native declarations still match. This is a source/DEX check, not an Android 17 app run; see [`java-port-api37-validation.json`](../reports/java-port-api37-validation.json).
- Reconstructed JNI source: all ten original function exports, including four JNI entry points. Host semantic and real JVM JNI tests pass. Original ARM32 instruction differential tests pass for the recorded SHA-1, passphrase and license-file cases. NDK r29 ARM64 compilation passes; load segments align to 16 KiB. Intentional safe differences from original undefined behavior are documented.
- Cache recovery: 5,839 complete files and 241 directories validated; 2,164 original text/shader files exported with SHA-256 provenance. Twelve recovery regression tests pass.
- Focused Java tests: vendor Base64 16,384 cases; Gameloft Base64 3,072 plus malformed vectors; CRC 1,296 checks; 30 installer/cache-integrity branches. These address identified decompiler damage, not whole-game behavior.
- All 2,164 text/shader hashes and all 288 repaired Java source hashes were independently rechecked.
- The external recovery source ZIP and both native evidence bundles passed the published SHA-256 checks; the source ZIP passed all 3,388 member CRC checks. The full export accounting was rerun from the restored external archive. A selected 333-file Java/JNI/tools/report import into Git has per-file source and checkout hashes in [`reports/recovery-source-import.json`](../reports/recovery-source-import.json).
- Reconstructed engine math: C++ for 20 original function starts (vector/quaternion plus matrix rotation), host and NDK r29 ARM64 builds, 16 KiB load alignment. 21,477 instruction differential comparisons pass; all 1,704 original instruction addresses in these ranges execute during tests. Original `__aeabi`/libm imports use a controlled host dependency model; historical Android libm and device integration remain unverified. See `port/engine-math/README.md` and `reports/engine-math-validation.json`.
- Reconstructed resources: C++ for 35 complete memory/subfile-reader and Collada-accessor bodies, plus the whole-buffer BRES relocation branch. Host and NDK r29 ARM64 builds pass. 10,449 stream comparisons and all 2,901 recovered BRES files pass original-ARM32/compiled-ARM64 checks, including 2,577,206 fixups, 5,154,412 native pointer values and 27,812 library/scene pointers. All 331 addresses in the complete bodies execute. Host ASan/UBSan input probes pass with leak detection disabled. See `port/engine-resources/README.md` and `reports/engine-resources-validation.json`.
- Mesh and animation reconstruction: immutable C++ views decode all 10,924 type-0 meshes, 1,641,664 vertices, 1,088,422 triangles and 890,301 animation time keys across the 2,901 recovered BRES files. The Prince's 173 meshes use a separately traced deferred-buffer form. Twenty-six complete animation accessor/typed-search bodies pass 271,970 original-ARM32/compiled-ARM64 comparisons with zero mismatches and all 456 instruction addresses executed. A separate host/ARM64 mesh check passes 25,682 comparisons on 1,149 meshes; it does not execute original GPU constructors. Host/ARM64 builds and 5,000 host ASan/UBSan probes pass. Small real OBJ/animation JSON exports are included. See `port/asset-payloads/README.md` and the four `reports/asset-payloads-*.json` files.
- External texture component: checked views classify all 363 external TGA/PNG files in the complete cache. A new PVRTC1 decoder produces RGBA8 for all 234 BTEX images (17 at 2bpp; 217 at 4bpp); every RGBA byte matched the official PowerVR software decoder in the recorded comparison. Synthetic pixel, modulation, bounds and canary checks pass. The revised component cross-builds for Android ARM64 with 16 KiB-aligned load segments and runs in the x86_64 source renderer on Android 17; ARM64 runtime execution and original-engine GPU upload remain untested. See [`port/texture-assets/README.md`](../port/texture-assets/README.md), [`reference-validation.json`](../port/texture-assets/reference-validation.json) and [`arm64-build-validation.json`](../port/texture-assets/arm64-build-validation.json).
- Image/material component: immutable checked views cover 3,662 image, 3,873 effect and 4,329 material records across all 2,904 BRES files in the complete cache. Nested checks cover 60,906 effect parameters, 49,902 material parameters and 9,625 effect image references; 7,708 local material/effect-group image-index set comparisons match. The revised component cross-builds for Android ARM64 with 16 KiB-aligned load segments. The Android 17 source renderer resolves one real diffuse image link, while original game material behavior remains untested. See [`port/material-bindings/README.md`](../port/material-bindings/README.md), [`validation.json`](../port/material-bindings/validation.json) and [`arm64-build.json`](../port/material-bindings/arm64-build.json).
- Scene hierarchy component: checked C++ views traverse 1,563 visual scenes and 21,472 nodes across all 2,904 BRES files in the complete cache, composing static world matrices from an instruction-supported parent × child convention. They classify 11,648 instances and resolve 10,403 of 10,444 local type-3 geometry references; the other 41 are recorded as unresolved within their file. Host and NDK r29 ARM64 builds pass with 16 KiB load alignment, as do 16 focused safety and ordering checks. Runtime animation, other instance payloads, rendering and gameplay remain unimplemented. See [`port/scene-payloads/README.md`](../port/scene-payloads/README.md) and [`validation.json`](../port/scene-payloads/validation.json).
- Host renderer preview: the checked BRES/mesh readers extracted four candle vertices, six indices and UVs. Flat and textured original unlit GLSL pairs from `shaders.pak` compiled and drew the primitive in Chrome WebGL1. Textured mode resolved the BRES diffuse image index, decoded `env_crypt.tga` PVRTC pixels and uploaded RGBA8 to WebGL. Each draw returned 53,185 non-background pixels; the textured draw had 6,460 distinct RGB colors. Shader/state selection is diagnostic rather than original material behavior; scene transforms, lighting, animation and Android execution are omitted. See [`port/renderer-preview/README.md`](../port/renderer-preview/README.md).
- Android development emulator: official Android 9 x86 AVD ran a locally signed ARM32 guest with the complete cache. Test 6's source-level repeated-root file-open retry reached the animated main menu and accepted Start game and Single Player without duplicating the cache. Level loading then hit repeated thread-creation failures and an ARM-translation memory abort; gameplay did not start. No phone or Fold7 device test was performed for this checkpoint. See [`EMULATOR-TEST-2026-10-02.md`](EMULATOR-TEST-2026-10-02.md).
- Android 9 thread diagnostic: a separate one-instruction branch from the failed worker start to the game's existing synchronous `Savegame::UpdateJobs()` path eliminated the 3,188 observed thread-creation failures and reached a saved 3D level. A short touch input visibly changed player/combat state, and the process remained alive about 30 seconds later. This isolated Test 8 check does not validate Android 17, save persistence or long sessions; see [`compatibility/thread-exhaustion/validation.json`](../compatibility/thread-exhaustion/validation.json).
- Android 17 source runtime: a native x86_64 smoke executable built from the reconstructed PVRTC decoder and BRES/material readers passed on the official API 37 16 KiB-page emulator. Synthetic 2bpp/4bpp and material checks passed, as did real-cache atlas texture and candle BRES reads, with matching on-device hashes. This checks isolated source functions, not the original engine or game. See [`port/android-smoke/runtime-validation.json`](../port/android-smoke/runtime-validation.json).
- Android 17 source app: a signed APK built from the checked C++ BRES/mesh/material/PVRTC modules plus new OpenGL ES 2.0 code installed on the API 37.2 x86_64 16 KiB-page emulator and rendered a real candle mesh with its matching 1024×1024 texture. The same path also passed on the 4 KiB-page Android 17 image. Both x86_64 and ARM64 libraries have 16 KiB-aligned load segments, but ARM64 runtime and gameplay have not been tested. See [`port/android-app/README.md`](../port/android-app/README.md) and [`runtime-validation.json`](../port/android-app/runtime-validation.json).
- Android 17 standalone compatibility packages: the first locally signed ARM64 Test 7 wrapper bundled the exact complete cache and guest APK and provisioned 6,834 files including one generated options file on both API 37 images. On 4 KiB x86_64, that first build rejected the ARM64 plugin proxy. Later builds fixed the plugin namespace, bundled ARM32 sysroot aliases, changed the save-job branch, and repaired a duplicated model-cache path. Test 9 reached a saved 3D level but cropped its menu again on cold restart. Test 10 fixes the engine's initial landscape size and subsequent GL viewport. A clean install of its final default-fit APK on API 37.0 with 4 KiB pages provisioned the full cache, showed a full-width character menu, entered the saved Swamps 3D level, responded to attack taps with a nearby moth and health bar disappearing, and moved the player/camera after joystick input. The exact final APK returned through pause to an updated save menu, then survived force-stop/cold relaunch with the new timestamp, reentered the saved level, and moved again. Long sessions, complete save semantics, level transitions and physical devices remain unverified. The standard wrapper retains the 16 KiB guard; a strict translator experiment on API 37.2 with 16 KiB pages reached a full-width interactive title and character menu, loaded a saved Swamps 3D level on one uninterrupted rerun, moved the player/camera after joystick input, and returned through pause to a menu with an updated last-save timestamp. Additional exact-package cold relaunches with both default and graphics-preservation settings visibly reached the saved-character menu with the previous save time after a slow loading phase. The default-setting run reentered the saved Swamps 3D level and moved the player/camera again. See [`ANDROID-17-EMULATOR-RESULTS.md`](ANDROID-17-EMULATOR-RESULTS.md) and [`STANDALONE-TEST10-VIEWPORT.md`](../compatibility/work/fold7-build/STANDALONE-TEST10-VIEWPORT.md).
- Android 17 game wrapper: the published ARM64 Test 5 wrapper installed and showed its launcher on official API 37 16 KiB and 4 KiB-page x86_64 AVDs. Its page-size guard stopped the 16 KiB image. On the 4 KiB image, direct loading of its AArch64 proxy failed in an x86_64 plugin process before engine startup. The ARM32 Test 7 direct guest was rejected by both AVDs with `INSTALL_FAILED_NO_MATCHING_ABIS`. See [`ANDROID-17-EMULATOR-RESULTS.md`](ANDROID-17-EMULATOR-RESULTS.md).

## Still unresolved

- Native pseudocode does not compile into a replacement engine. Original gameplay class fields, ownership, ABI and indirect-call behavior still require reconstruction.
- Sixteen of nineteen vector/quaternion pseudocode bodies incorrectly stop at floating-point helpers marked as non-returning. The new math module uses complete assembly; the archived pseudocode has not been regenerated with corrected helper metadata.
- JNI/reflection calls into non-native Java members remain unverified, including JADX-renamed obfuscated fields/helper methods.
- The earlier ten-part cache input ended partway through `m_world_map.wav`; the separately supplied complete local cache resolves that input gap and is bundled in the experimental Android 17 compatibility APK. It is not committed as source.
- BRES container access, interleaved type-0 meshes, inspected embedded meshes in all nine type-1 records, deferred bytes inside complete files, raw animation keys/search, external PVRTC pixels, checked image/material references, scene hierarchy traversal and static world matrices are reconstructed. The nine type-1 prefix words are opaque and the original runtime rejects type 1; spline semantics, full effect/material behavior, runtime transform updates/controllers/skinning, track-specific animation application, split/external loading, ownership, game rendering and a complete native engine build remain unfinished.
- The reconstructed components have run in the x86_64 Android 17 emulator, but no physical ARM64 device execution or complete source-built gameplay engine has been demonstrated. The compatibility APK has short integrated menu/load/attack/movement evidence; full quest, level-transition, save/reload and extended gameplay tests remain open.
- The supplied APK contains later save-restoration/support additions; its untouched studio-release provenance is not established.

The isolated compiled artifacts included with a Drive handoff are validation outputs, not a playable game APK. The exact input APK and cache parts are kept separately from recovered source.
