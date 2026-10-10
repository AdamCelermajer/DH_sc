# Lane 18: libStormGLOFT.so records 0–1863

Read-only audit of the working-tree reconstruction. Baseline marker from the audit README: 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. Produced only lane-18.md and lane-18.csv. No source edits, builds, tests or emulator runs occurred.

## Coverage and limits

**1,864/1,864 assigned inventory records classified; zero missing IDs and zero duplicate IDs.** Original decompilation statuses: **1,862 success and 2 failed**. There are 1,351 explicitly marked forwarding thunks and one PLT resolver, leaving 512 other records including the two failed bodies.

| Comparison disposition | Records |
|---|---:|
| import/thunk | 1,352 |
| out-of-scope | 452 |
| missing | 35 |
| partial | 15 |
| unclear | 3 |
| clone | 3 |
| failed-body | 2 |
| matched | 2 |
| **Total** | **1,864** |

Every integrated-runtime field is **not_run**. Existing texture corpus reports and historical Storm loading/hook logs do not establish current-source integrated acceptance. Inventory disposition and source-boundary comparison do **not** prove full internal compiler or codec equivalence.

The CSV retains each original number/address/name, code and pseudocode hashes, original decompilation status, body category, assembly line, source citation or scoped reason, confidence and runtime disposition. Its review_depth distinguishes body/source comparison, body/boundary classification, thunk assembly review and failed-body assembly/role review. In-process Mesa/glslopt helpers have no selected source implementation to compare; their internal algorithms were classified through body features and actual call/data role rather than declared equivalent to the GLES driver.

The CSV enumerates exported xrefs between inventory starts and named data references. **1,338 outward and 3,514 inward lane-18/lane-19 reference edges** are retained as distinct source/target/type triples. Their IDA code/data type remains explicit: calls, jumps, fallthrough and data-address references are not all direct calls. Indirect linker calls and generated trampolines need the concrete closure evidence below.

All cited source hashes matched at the initial hash capture and again at report generation; no cited source changed during that interval. The first hash capture followed initial source reads, so changes before capture cannot be excluded. Dirty working-tree files, rather than HEAD contents, are the source reviewed.

## Source scope and negative-claim evidence

S1 enumerated 3,087 rg-visible authored/source files under port, compatibility, reference and tools using C/C++/headers/ASM/Python/Java/CMake/Gradle extensions, excluding vendor, test, report, build, out, reference, staged and generated directories. Searches for glslopt_, hook_export_function, my_glShaderSource, storm_native, InlineHook, libStormGLOFT, explicit DXT/ATC/ETC decoder names, compressed-format names and the malformed endif token found six Storm packaging/patch-tool references and no authored codec or shader-repair implementation in that scope. Supplemental searches and reads covered the actual texture API, native GLES code, compatibility C/ASM, CMake and every current shader-pack member. Ignored artifacts and vendor/staged/generated copies are not asserted to be selected implementations.

The positive native boundary is [NativeBridge.java:5](../../port/android-native/app/src/main/java/com/example/dh2/NativeBridge.java), which loads dh2_native, and [native CMakeLists.txt:49](../../port/android-native/app/src/main/cpp/CMakeLists.txt), which selects reconstruction modules. [authored_shader_program.cpp:53](../../port/android-native/app/src/main/cpp/authored_shader_program.cpp) and [native_batch_program_v113.cpp:31](../../port/android-native/app/src/main/cpp/native_batch_program_v113.cpp) submit authored source chunks to actual GLES. [shader_sources.cpp:24](../../port/scene-materials/shader_sources.cpp) produces those chunks. [textures.hpp:7](../../port/engine-textures/textures.hpp) admits PVRTC2/4 and TGA; [textures.cpp:59](../../port/engine-textures/textures.cpp) rejects other PVR payload types. [native_app.cpp:279](../../port/android-native/app/src/main/cpp/native_app.cpp) shows checked decoding followed by RGBA upload.

These implementation bodies establish the replacement boundary and support limits. A symbol-search miss alone is not evidence of missing behavior.

## Findings and concrete paths

### F18-01: private-linker ABI has a concrete compatibility replacement

IDA #1356 hook_import_function at **0x371E4** treats dlopen's result as the old private soinfo layout, reads fields 43–51, resolves the ELF hash chain and replaces relocation type 22. #1456 hook_inline_function at **0x43894** loads handle+0x8C and adds the fixed engine offset. Assembly confirms the original load at **0x438E4**.

[storm_import_fix.c:72](../../compatibility/work/fold7-build/storm_import_fix.c) obtains an anchor from the exact companion JNI_OnLoad, validates the ELF header/machine and walks PT_DYNAMIC/DT_SYMTAB/DT_STRTAB/DT_JMPREL. It replaces matching R_ARM_JUMP_SLOT imports and installs additional fopen/puts guards. [storm_bias_fix.S:11](../../compatibility/work/fold7-build/storm_bias_fix.S) resolves the pinned bias from exported JNI_OnLoad, preserves registers and rejoins original hook logic. [patch_storm.py:48](../../compatibility/work/fold7-build/patch_storm.py) installs the import-entry branch; the same patcher verifies and replaces the load at 0x438E4.

**Disposition: partial, high confidence.** These are source replacements for two compatibility boundaries with deliberate ABI/guard changes. The remaining hook machinery remains original ARM32 binary; native reconstruction does not select it. Current-snapshot compatibility acceptance is unverified.

### F18-02: registration closes through exact companion-engine callbacks and owner state

#1355 **0x3713C JNI_OnLoad** calls export discovery for Storm with library prefix libDungeonHunter2, inline-hooks companion offset **0x530C50**, then overrides exactly the companion **glShaderSource** and **glGetString** imports. Assembly supplies the library-prefix R1 and inline arguments lost by pseudocode. It returns JNI 1.6 and makes no RegisterNatives call.

Three concrete .data descriptors exist: custom_hook_info_nativeSetPhone **0xDE020**, custom_hook_info_nativeResize **0xDE100**, and custom_hook_info_nativeGetWidth **0xDE1E0**. #1376 discovers descriptors through #1386/#1387/#1388 symbol-table parsing; #1396 resolves and installs trampolines; #1381 retains installed-hook records.

| Storm callback | Companion target | Actual behavior |
|---|---|---|
| #1377 0x395A4 storm_nativeSetPhone | DH #12417 0x532E78 nativeSetPhone | Stores actual dimensions in my_width/my_height, invokes original with orig_width/orig_height |
| #1378 0x39690 storm_nativeResize | DH #12359 0x53117C nativeResize | Invokes original with orig_width/orig_height |
| #1379 0x39754 storm_nativeGetWidth | DH #12392 0x531A1C nativeGetWidth | Returns orig_width rather than invoking original JNI body |

Storm .data stores **orig_width=1280.0 and orig_height=720.0**. #1397/#1398 restore/reinstall target bytes around original calls; callback ownership is the descriptor/trampoline state.

The generated inline callback is #1455 **0x43834 RegisterHook**, attached through #1453/#1470/#1462. It writes my_width/my_height into saved r1/r2. Companion **DH #12352 appInit at 0x530BA8** contains **0x530C50 STM R3,{R1,R2}**, committing those dimensions to g_windowDimensions before createDevice. The callback changes values written by that instruction; this is an interior instruction edge, not another named function.

[native_app.cpp:290](../../port/android-native/app/src/main/cpp/native_app.cpp) stores actual native surface dimensions and publishes the root viewport. The checked surface accessor at line 302 feeds camera/draw owners. [swf_gpu.cpp:481](../../port/android-native/app/src/main/cpp/swf_gpu.cpp) applies actual clipped/flipped UI subrects. **Dimension callbacks are partial replacements; address patch machinery is out-of-scope for the selected native source.** No old trampoline/descriptor-owner parity or runtime acceptance is implied.

### F18-03: GLSL optimizer is replaced at the selected GLES boundary, with current input grounding

#1357 **0x3732C my_glShaderSource** joins counted or NUL-terminated chunks, repairs malformed "#endif]" to "#endif", queries GL_SHADER_TYPE 0x8B4F, runs glslopt target 1/options 0, submits optimized single-string output on success and joined source on failure, then frees/cleans context. Assembly verifies omitted parameters and both GLES calls. Optimized bytes are copied before glslopt_shader_delete, establishing their required lifetime.

The optimizer boundary crosses lane 19:

| Lane-18 thunk | Actual implementation |
|---|---|
| #11 initialize | #1915 0x77218 glslopt_initialize |
| #23 optimize | #1919 0x772E4 glslopt_optimize |
| #24 status | #1923 0x77A62 glslopt_get_status |
| #25 output | #1924 0x77A6A glslopt_get_output |
| #28 delete | #1921 0x77A18 glslopt_shader_delete |
| #32 cleanup | #1917 0x772B4 glslopt_cleanup |
| #1061 Mesa compile | #2002 0x7E3D0 _mesa_glsl_compile_shader |

Those implementations reach lane-18 glcpp scanner/parser #1489/#1529, preprocess #1557, allocator/hash support #1560–1634, AST/HIR #1635–1731 and builtin IR #1732–1863. These bodies are compiler internals, not missing gameplay scripting callbacks.

Current source submits the authored plan directly at authored_shader_program.cpp:53 and native_batch_program_v113.cpp:31. The actual [shaders.pak](../../port/android-native/app/src/main/assets/shaders/shaders.pak) is **44,194 bytes**, SHA256 **365a4d3c432454c44208ebb484c7a472a3a4534a0c5c9e77a7a90f3b87b1b5c0**, with **34 ZIP members**. All 34 member bodies were read and none contains the malformed endif token, including GameSWFVS, GameSWFFS and GameSWFFS_blend. No current asset failure at that repair branch was established.

**#1357 is partial** because optimizer/rewrite behavior differs. Its unselected in-process Mesa/glslopt dependencies are **out-of-scope by the positive compiler boundary**; no compiler algorithm/precision equivalence is asserted. The complete live authored program set remains unverified at runtime.

### F18-04: fixed renderer/capability identity is an unselected compatibility override

#1358 **0x37C04** returns ARM, Mali-T604, GLES 2.0, ETC1/Mali binary extensions and GLSL ES 1.00 for selected GL enums, forwarding unknown values. [native_app.cpp:259](../../port/android-native/app/src/main/cpp/native_app.cpp) queries actual current-context renderer/version. **Disposition: out-of-scope compatibility override**, not a missing native device implementation. Driver-sensitive behavior still requires runtime acceptance.

### F18-05: PVRTC arithmetic has concrete source counterparts, with a restricted domain

#1363 **0x381DC** routes PVRTC2/4 and RGB/RGBA through #1400 **0x3B398**, stripping alpha bytes for RGB before #1364 upload. #1400 has Morton addressing, opaque/transparent endpoint decode, checkerboard 2bpp modulation, 4bpp interpolation/punchthrough and RGBA output. Its transparent-B branch expands **A blue** while leaving B blue unreplicated.

[pvrtc.cpp:23](../../port/engine-textures/pvrtc.cpp) preserves that endpoint behavior. Its checked entry at line 56 adds encoded-size/output-capacity/nonzero/power-of-two limits. [textures.cpp:92](../../port/engine-textures/textures.cpp) decodes borrowed views and sets alpha 255 for nonalpha textures. The selected source uploads RGBA rather than allocating a separate RGB representation.

#1401 **0x3BF40** nonzero power-of-two predicate matches textures.cpp's predicate. #1405 **0x3C890** interpolation matches pvrtc.cpp's interpolate arithmetic on valid endpoint/coordinate inputs, including RGB shifts 1/2, alpha shift 0/1 and final replication. **Only those two helpers are matched statically. #1400/#1363 are partial**, with full Storm input-domain and ownership equivalence unresolved. #1407/#1408 have analogous restricted PVRTC block Morton addressing, not full exported texture twiddle/detwiddle parity.

Historical [cache-audit.json](../../port/engine-textures/reports/cache-audit.json) describes 242 decoded images: 17 PVRTC2, 217 PVRTC4 and 8 TGA. It explicitly has gpu_execution=false and original_pixel_equivalence=false. This is selected-format context, not current-source Storm equivalence or integration acceptance.

### F18-06: portable DXT/ATC/ETC1 fallback is absent; current gameplay relevance remains unverified

#1368 **0x388E4** dispatches PVRTC 0x8C00–03, DXT 0x83F0–03, ATC 0x87EE/0x8C92/0x8C93 and ETC1 0x8D64. #1365/#1411–1420 decode DXT1/3/5; #1366/#1421–1427 decode ATC with explicit/interpolated alpha; #1367/#1451/#1443 and their unpack helpers decode ETC1. #1402–1404 supply the separate PowerVR ETC family. Allocation → decode → #1364 upload → free is explicit.

The source Format enum/open/decode bodies admit only PVRTC/TGA and reject other PVR payload types. Together with S1's scoped source search, **35 rows are missing specifically portable fallback/decoder behavior**. The classification includes supporting palette/alpha/unpack helpers; it does not count 35 independent gameplay failures.

**Unverified path:** an ETC1/DXT/ATC encoded payload supplied through a future portable loader lacks Storm's software fallback. **No current gameplay failure is proven:** original JNI installs only shader/string import hooks; my_glCompressedTexImage2D has no incoming inventory-function xref and all three custom_hook_info descriptors are dimension hooks. External activation of exported helpers remains possible. Full asset-consumer reachability for these formats is unresolved.

The ETC1 compression/optimizer utilities #1428/#1429/#1435–1437/#1444–1450 are **out-of-scope encoder behavior**, because the selected source API is a reader and no compression consumer is selected.

### F18-07: optional 16-bit uploads and forced-origin clipping differ

#1364 **0x382FC** conditionally converts byte RGB/RGBA to RGB565/RGBA4444, uploads, then frees scratch. #1369/#1370 supply the conversion masks. It samples glGetError before upload and later logs that sampled value. Native source selects byte channels at native_app.cpp:279 and [swf_gpu.cpp:104](../../port/android-native/app/src/main/cpp/swf_gpu.cpp). Ordinary upload functionality is present; the optional policy/conversion branch is unselected. **#1364 partial; optional standalone conversion/low-memory helpers out-of-scope.**

#1371/#1372 force viewport/scissor origin zero. Native root resize uses zero, but SWF clipping carries actual clipped/flipped subrect coordinates at swf_gpu.cpp:481. Both rows remain partial; indiscriminately applying this override to native subrects would change clipping.

### F18-08: thunks, clones, fragments and failures retain separate dispositions

#0 **0x31ECC** is a PLT resolver (saved LR, GOT setup, indirect PC load), despite JUMPOUT(0) pseudocode. #1–1345 and six later wrappers (#1497/#1499/#1527/#1558/#1574/#1737) are exact forwarders. Their actual targets/import records and cross-lane xrefs are recorded; no missing-gameplay labels are inferred from ABI helpers.

Verified duplicate groups retain every row:

| Canonical record | Clone record | Verified body and code hash |
|---|---|---|
| #1638 0x4EDD4 | #1730 0x5A07C | Empty destructor callback; c7dfbb7d02759eacb64dbc916c1bb6f21eabaff1c1032ea5c9176abf7fd28df8 |
| #1663 0x52160 | #1664 0x52166 | Same indirect virtual dispatch at vtable+4; 77bb8296f7de909082af2fad92870279820ea00f9e5b732cce8a0f5f36fcaa03 |
| #1811 0x6979C | #1822 0x698EC | Same GLSL availability predicate/offsets; 6403147055d8fc2f6360d3306924bc300576d8c523ee720d924c0ce6cc87a53d |

Do not merge #1801 **0x4EE3C** and #1802 **0x5993C** on their identical raw hash 0784f7cc7fce7752d6c9855a5e3c89a639193ae1f181bf9ce85fa65ee4f3d1f3. Their four-byte STRB/MOVS ranges continue into different next bodies: prototype_string and ast_fully_specified_type::has_qualifiers. #1800 **0x499C8** is another inherited-state fragment. All three are **unclear**; success decompilation does not establish a valid standalone function.

| Failed record | Disposition and assembly evidence |
|---|---|
| #1780 0x65848 | Decompiler returned no cfunc. Full 28-line Thumb assembly selects type-dependent IR opcode 0x40/0x56, takes callback #1761 and calls #1852. Builtin compiler role established; arithmetic parity unresolved. |
| #1788 0x66F6C | Decompiler timed out after 60 seconds. 2,111-line Thumb assembly and #1736's inverse registration identify mat4 inverse builtin IR construction, including SubFactor temporaries and final division. Complete expression parity unresolved. |

Both failed-body records preserve failed decompilation status. Assembly role evidence does not relabel them successfully reconstructed.

## Outstanding work

1. Current-source integrated runtime acceptance: all 1,864 records remain not_run.
2. External activation and exact cache/resource consumer for exported DXT/ATC/ETC1 helpers; export presence does not establish gameplay reachability.
3. Full Storm PVRTC/current-source equivalence for tiny/unsafe/nonpower-of-two/overflow inputs and RGB/RGBA representation differences.
4. Precision/output equivalence of glslopt versus the actual driver on all live selected authored shaders. The 34-member pack proves only absence of the repair trigger.
5. Complete expression reconstruction of both failed compiler helpers.
6. Correct standalone boundaries/ABI for #1800–1802.
7. If future scope requires byte-identical glslopt output or full compatibility-library source, reopen the compiler/trampoline out-of-scope rows. This audit establishes selected-source boundaries, not mathematical equivalence of every library helper.
8. No old C++ runtime/libc/toolchain behavior was exhaustively emulated. Dynamic and generated calls remain limited by available static exports.

## Cited working-tree hashes

| File | SHA256 |
|---|---|
| port/android-native/app/src/main/java/com/example/dh2/NativeBridge.java | 02e95bd3476dfb64cab894089caf47c4180fc61dc18a3953d3dfcb69fbdb0109 |
| port/android-native/app/src/main/cpp/CMakeLists.txt | ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45 |
| port/android-native/app/src/main/cpp/authored_shader_program.cpp | f7c0cc40d71f5dd00c802bf83b36afe1378d7c576c43e73a86dfb5da04d30c58 |
| port/android-native/app/src/main/cpp/native_batch_program_v113.cpp | 10b946d127afbf419fe06b784036d4021edba01a06f0edfe68d332ff5e274a4d |
| port/android-native/app/src/main/cpp/native_app.cpp | a24f348fafea17e488bf5b3cdf97f832651818f45e78077a59b3a6f5407ddef3 |
| port/android-native/app/src/main/cpp/swf_gpu.cpp | 140ef2708958cb5adb5a4f0d7e484c3fea742a9bcdb34c58078c603bd52013cd |
| port/scene-materials/shader_sources.cpp | f05874e30bba3a3b1c8cec643e5fda6bcba4f31e9b935eae8e94c9872b41cde1 |
| port/engine-textures/pvrtc.cpp | dd88a34faff7b5741da77be72a49c0668e75f5aa57c146a3ea7c51817228d575 |
| port/engine-textures/textures.cpp | 80264623322fd3262ceeebe1473711ebf5c9546a1f2028d0915c17d5572eb7cb |
| port/engine-textures/textures.hpp | 806ba7487bda7dda7de8e8f7b8c22f3165cd9ee46cadb635f4f0bf8ccf34cb61 |
| compatibility/work/fold7-build/storm_import_fix.c | 57e5e91b4269dd4c670b1f2205b57f22483f4820458c53d32102c2d7c1472b27 |
| compatibility/work/fold7-build/storm_bias_fix.S | 0a1b353820d886cbce855448c40eefb97d6b71b43e390e07daa76ea98905b134 |
| compatibility/work/fold7-build/patch_storm.py | 43215644520531e32cf47b6cf87f20e9ddcd0dd1e01017f5c3155c106a12b47b |
| port/android-native/app/src/main/assets/shaders/shaders.pak | 365a4d3c432454c44208ebb484c7a472a3a4534a0c5c9e77a7a90f3b87b1b5c0 |
| port/engine-textures/reports/cache-audit.json | bc7634bab8be45b562fd47a41d33b6d1ad60e2cd83a42380070b7d1529b212b7 |

## IDA inputs

All CSV pseudocode/assembly/xref paths are relative to .local-inputs/ida-apk-export-2026-10-07/libraries/libStormGLOFT.so/.

| Export | SHA256 |
|---|---|
| functions.jsonl | 1f9dd13c36983c0b103fa96a0bd3af5071153d6d719e2591c69eff544bfa2ea6 |
| pseudocode-all.c | 59256a5f65232cd019d3f21b0879ea610656cff958f0084d062b1ff5509d5b70 |
| assembly-functions.asm | 51efa100cb61aee4fe1fa5c89b5a5f0827ec966e756148250a420b09cb2cb8f5 |
| xrefs.jsonl | 3658327c67ad268a88a290c27cb4bdbf31ee3c84719a72a3d75d93b150d1ce98 |

Companion reads are references only, not duplicate primary coverage: libDungeonHunter2.so assembly-functions.asm:568349 for instruction 0x530C50 and pseudocode/0053/00530ba8.c, 0053117c.c, 00531a1c.c and 00532e78.c.

| Companion pseudocode file | SHA256 |
|---|---|
| 00530ba8.c | 96c0760b7ab768a26653781eff1fbf3e48e3f5ca3ae44c8116f53ef5165e171e |
| 0053117c.c | 7a58be4edb0984120ffc1cade52667ce14a994601830e7576cba76b4c9787b31 |
| 00531a1c.c | 3af3ae1db8f8b6ca08a2b757b399c20a3288e986d60d65cddcc043829ec161e7 |
| 00532e78.c | 6850e133c7073d8afc6ea556056ff14f5b6411f46ac51957eefbeda588ac4b06 |
