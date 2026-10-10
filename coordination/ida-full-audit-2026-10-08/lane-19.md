# Lane 19 â€” Storm IDA records and source boundary audit

Scope: **libStormGLOFT.so indices 1864â€“3727 inclusive; 1,864 records**. Working-tree HEAD: **75a7c2fe3261403e841ffbeb46734e9e4e84e75c**; the dirty tree was preserved. CSV contains every assigned ID once, including thunks, clones and failed decompilations.

This is complete record coverage plus static subsystem/boundary review, **not 1,864 proofs of semantic parity**. Nontrivial internals without source parity evidence remain unclear. All integrated runtime acceptance entries are **not_run**. No game-source edit, build, test, install, emulator or new runtime run was performed.

## Coverage and method

| Coverage | Count |
| --- | ---: |
| Assigned rows / unique IDs | 1,864 / 1,864 |
| Missing / duplicate IDs | 0 / 0 |
| Successful pseudocode bodies | 1,740 |
| Failed decompilations | 124 |
| Failures proven external import slots | 124 |
| Failed executable bodies | 0 |
| Original ELF code hashes compared | 1,740; zero mismatches |
| LF-normalized pseudocode hashes compared | 1,740; zero mismatches |
| Integrated runtime acceptance | zero; all 1,864 not_run |

Original Storm input SHA-256: be6beaab782944e5ce39cca8e850fd03de654e4236c9329621043adcbee291e1. Executable hashes were checked by mapping the original ELF PT_LOAD ranges and concatenating each function's exported chunks. Pseudocode files are CRLF; index hashes were computed before Windows text-mode writing. LF normalization reproduces all expected digests.

All successful bodies were loaded, hash checked and structurally inspected for operations, branches/loops, named expressions, literal/data references, indirect dispatch and body identity. CSV retains body evidence and all function-target incoming/outgoing xrefs, including non-code references and cross-lane owners. Manual semantic comparison followed the shipping shader boundary, allocation/destruction/reflection consumers, HookZz entry/trampoline/VM/TLS ownership, and fragile assembly. A structural census is **not a line-by-line proof of every Mesa optimization pass**; these unresolved rows are explicit.

Clone grouping requires identical original code bytes **and normalized pseudocode body**, preserving resolved literals/identifiers. Code hash alone is insufficient: #3117/#3119 have different targets despite equal bytes, and #3330/#3333/#3364/#3368/#3430/#3433 return different strings through equal PC-relative encodings. The synthetic zero import slots remain individual imports. Identical virtual-dispatch bodies do not prove identical dynamic receiver/owner behavior. CSV identifies each clone representative and body hash.

## Subsystem coverage

| Records | Inspected behavior |
| --- | --- |
| 1864â€“1914 | Mesa builtin type/variable publication, generated lexer/token conversion/buffer lifecycle and accessors |
| 1915â€“1934 | glslopt context/shader ownership, optimization, status/output/log, descriptor/statistic APIs |
| 1935â€“2415 | Parse state/version/extensions, AST/type/symbol table/IR construction and cloning, constant expressions, visitors |
| 2416â€“2492 | GLSL/Metal emission, names/precision/loops/assignments, string buffer growth |
| 2493â€“2686 | IR debug/rvalue/stat visitors, atomic/uniform/interface/varying linkage and layout |
| 2687â€“3045 | Loop/unroll/lowering, constant/copy propagation, dead code/inlining, algebraic/swizzle/vector rewrites |
| 3046â€“3070 | S-expression utility and Mesa shader/context defaults |
| 3071â€“3279 | HookZz allocation/hook owners, ARM/Thumb readers/writers/relocators, trampolines, VM/TLS |
| 3280â€“3513 | GAbi++/STL allocation/exceptions/typeinfo/guards, ARM arithmetic and unwind |
| 3514â€“3597 | 84 Thumb/ARM branch veneers; each target retained |
| 3598â€“3721 | 124 synthetic external slots; imports.json exact identity and failure reason |
| 3722â€“3727 | Added lexer/printer fragments, precision visitor, qsort comparator, recovered Thumb relocation |

## F1 â€” shipping shader preprocessing/output boundary is partial

Original cross-lane path: JNI_OnLoad #1355 at 0x3713c installs #1357 my_glShaderSource at 0x3732c through #1356 hook_import_function. #1357 calls lane19 glslopt initialization/optimization/cleanup through PLT #11/#23/#32. It queries shader stage, assembles chunks using explicit length or strlen, repairs #endif], initializes optimizer **target 1**, and optimizes with **options 0**. It submits **one** optimized string on success, or the repaired assembled input on failure. On success it copies the output before deleting the optimizer shader and frees the submitted copy. Assembly at 0x3738c/0x37a30 confirms target/options; 0x37abc and 0x37af4 confirm both source-submission branches. Pseudocode had dropped several arguments.

IDA bodies: [#1357](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libStormGLOFT.so/pseudocode/0003/0003732c.c), [#1919 optimizer](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libStormGLOFT.so/pseudocode/0007/000772e4.c), [#1917 cleanup](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libStormGLOFT.so/pseudocode/0007/000772b4.c). Exact assembly start lines and body hashes are in CSV.

Current [shader_sources.cpp:24](C:/Users/adamc/Desktop/workspace/DH_sc/port/scene-materials/shader_sources.cpp:24) builds eight chunks including GLES2/highp/bias/config/caller/body. [authored_shader_program.cpp:53](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/authored_shader_program.cpp:53) and [native_batch_program_v113.cpp:31](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_batch_program_v113.cpp:31) submit these directly, invoke the GLES driver compiler, and check GL_COMPILE_STATUS. Failure reports the driver log and releases GPU owners. They have no same Mesa IR optimizer object or output at this boundary.

Disposition: **#1919 partial**. Direct-driver compilation may produce acceptable shaders, but equivalence of repair, optimized output, precision/rewrites and behavior/performance over every shader/configuration is unproved. No #endif] was found in the scoped authored source/assets examined, so there is **no demonstrated failing asset here**. That literal miss does not prove the repair unnecessary; retained ZIP shader payloads were not exhaustively decoded.

## F2 â€” native UI and material-batch owners are connected

UI program creation is called by [native_app.cpp:236](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_app.cpp:236); [authored_shader_program.cpp:66](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/authored_shader_program.cpp:66) owns compile/link/failure cleanup.

Material path: [renderer_native_batch_gpu_v111.inc:25](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/renderer_native_batch_gpu_v111.inc:25) calls prepare_native_batch_shader_v113, whose [line 48](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/renderer_native_batch_shader_v113.inc:48) constructs NativeBatchProgramV113 and initializes it before publishing the shared program to the same source-geometry GPU cache. Current [model_renderer.cpp:2313](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/model_renderer.cpp:2313) includes the shader path and [model_renderer.cpp:2976](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/model_renderer.cpp:2976) includes the GPU consumer. These current anchors were reread during the bounded post-pause recheck; renderer SHA-256 is de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca. Programs submit through [renderer_source_geometry_v64.inc:288](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/renderer_source_geometry_v64.inc:288), and the cache releases program owners at that file's line 78. [CMakeLists.txt:183](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/CMakeLists.txt:183) compiles the program implementation.

Driver attribute/uniform reflection at [native_batch_program_v113.cpp:39](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_batch_program_v113.cpp:39) feeds material/sampler bindings and submission; [shader_reflection_v4.cpp:5](C:/Users/adamc/Desktop/workspace/DH_sc/port/scene-materials/shader_reflection_v4.cpp:5) owns semantic mapping. Optimizer getters #1931â€“#1933 read seven fields at stride 28 from optimizer arrays, a **different producer**. Similar reflection purpose does not establish those API contracts. A cpp/hpp-only search initially missed .inc callers; the broader scan/include chain resolved it. **No disconnected program finding is retained.**

## F3 â€” legacy HookZz source parity remains unresolved

#3084 ZzBuildHook allocates/initializes a hook entry, builds/registers a trampoline and publishes its original invoke address. #3086 finds the exact owner, rejects enabled state, sets its flag and activates the trampoline. #3107 selects pre/post/replace/one-instruction/DBI construction. #3246â€“#3248 create/pop TLS call frames, invoke stored pre/post callbacks, route continuation/return addresses and free frames. The callback owner is a HookZz entry/TLS frame, not a gameplay entity or script registry. Direct xrefs do not resolve these indirect targets.

The shared native target [CMakeLists.txt:49](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/CMakeLists.txt:49), GLES linkage at line 144, and ARM64/x86_64 filters [build.gradle.kts:66](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/build.gradle.kts:66) do not reconstruct/link HookZz. Compatibility [patch_game.py:73](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/patch_game.py:73) retains the original ARM32 Storm binary. [patch_storm.py:21](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/patch_storm.py:21) patches the separate lane18 hook_inline_function private-linker bias access and replaces the lane18 import-hook entry using [storm_import_fix.c:79](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/storm_import_fix.c:79). These are not replacements for the 209 assigned HookZz bodies.

No direct incoming calls to exported #3088 ZzHook, #3089 ZzHookPrePost, #3090 ZzHookReplace, #3091 DBI, or #3092 one-instruction API appear in supplied Storm target xrefs. Their internal helper/PLT closure is in CSV. Dynamic external callers remain possible, so the disposition is **unclear**, not missing or proof of unreachability.

Original contracts/risks if invoked: #3108 ZzHookGOT and #3109 ZzDisableHookGOT return 1 without a GOT operation; #3118 reports RX allocation support unconditionally; #3232 returns1 without a trampoline free. #3262 hardcodes 4096 in Thumb assembly. #3264 aligns protection with it; #3271 snapshots aligned pages, changes writable/executable protections, copies and munmaps scratch while ignoring protection returns and always returning1. These are original helper behaviors, not demonstrated current native-source defects or accepted modern-device behavior.

## F4 â€” successful pseudocode can lose execution behavior

ctx_save/ctx_restore #3241â€“#3244: ELF values 0xaadd9/0xaae15 are Thumb-tagged, sizes 60/56. Raw bytes 38 d0 4d e2 and 04 00 9d e4 are consistent with ARM stack allocation and postindexed LDR; continuations save/restore registers. IDA splits each symbol-sized range into a 4-byte start plus remainder. Mode/symbol inconsistency is unverified and may originate in supplied metadata, not only IDA. All four retain **unclear** rows.

#3727 at 0xa86d8 exactly covers the 508-byte ELF symbol but pseudocode has const-write warning/JUMPOUT. Assembly dispatches GetTHUMBInsnType to LDR/ADR/B/BLBLX rewrites #3156â€“#3166, copies original instruction bytes on fallback, records emitted spans and advances indices. Switch-table bytes near 0xa8740 remain misrepresented as instructions; complete relocator correctness was not proved.

#2574 validate_ir_tree is genuinely BX LR/no-op. Conversely #3485/#3487/#3489 have empty pseudocode but actual FLDMIAX/VLDM VFP loads; they are runtime helpers, not no-op clones. #3497 carries a positive-SP unwind continuation warning. These distinctions are in individual evidence cells.

## F5 â€” added boundaries and callbacks have explicit dispositions

#3722 at 0x76ba0 is a four-byte register-dependent lexer fragment before #1878; #3724 at 0x8abec and #3725 at 0x8dc90 are printer fragments. Listings/data references do not establish independent callable functions. Each retains its assigned row and unclear status.

#3723 at 0x77eb4 is a precision visitor supplied by #1938 sub_77B98 to visit_tree. #3726 at 0x926dc is the qsort comparator supplied by #2645 sub_92354 inside #2644 varying-location assignment. Pseudocode shows these callback pointers despite target xrefs omitting incoming calls. Comparator assembly 0x9270c negates strcmp, which pseudocode loses; two explicit slots compare location difference. Native reflection uses driver locations and a different producer/owner; replacement sorting parity remains unclear.

## Imports, excluded dependencies and runtime

All 124 failed rows #3598â€“#3721 match imports.json by exact synthetic address/name and fail because special segments cannot be decompiled. They are import slots, not 124 unavailable executable implementations. Individual PLT/GOT/data references are retained. There are 127 successful explicit thunk bodies, including 84 interworking veneers #3514â€“#3597.

Out-of-scope is narrow: original C++/ARM runtime dependency bodies are owned by the modern NDK target; Metal emission requires target 3 while shipping wrapper target 1/native GLES excludes that application path; standalone compiler main/usage has no Android shared-library CLI consumer. This does not mean libc/libm/STL/driver edge semantics were tested or binary behavior preserved.

Historical [native-load-test.log:15](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/native-load-test.log:15) and [native-probe-test5.log:34](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/native-probe-test5.log:34) show original Storm loading, inert-VM JNI_OnLoad and hook installation. [phone-test4 log:194](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/phone-test4/dh2-logcat.txt:194) shows loading/lane18 custom hooks. These different binaries/routes do not accept reconstructed lane19 optimizer/relocator bodies. **All CSV runtime entries remain not_run.**

## Search scope and unresolved work

Implementation search covered port/compatibility/tools and then full authored working-tree code/build material, with .git/.local-inputs and historical reports/reference/integration snapshots/vendor material excluded from live-source results. Broadened scans included .inc. Seeds included glslopt/_mesa_glsl/HookZz/Zz/zz_thumb/Storm/import hooks, GLES submissions and native program owners. CMake/Gradle inclusion, actual shader callers/planning, caches, reflection/submission/destruction and compatibility patch construction were read. IDA callers/PLT/callbacks were followed across lane18. **No textual miss alone caused a missing claim.**

All unclear rows are individually named in CSV. Unresolved: per-body Mesa lexer/parser/IR/constant-evaluation/link/layout/optimization/lowering equivalence and compiler-cache ownership; every shader/configuration through #1357 repair/optimized-output; exact optimizer metadata versus driver reflection; HookZz external reachability and indirect callback ownership; ARM32 relocation/VM/TLS failure and rollback; mode/boundary #3241â€“#3244/#3722â€“#3725; comparator #3726 replacement ordering. Exhaustive ZIP shader-payload review, source-built integrated acceptance and modern-page/device behavior were not established. This lane does not certify gameplay or every compiler pass branch.


## Bounded post-pause F2 recheck — 2026-10-08

The source-impact sweep identified stale Markdown caller provenance. This recheck supersedes the former renderer anchors 2298/2961 and the historical c35716fa/bb979002 fingerprints with current anchors **2313/2976** and SHA-256 **de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca**. Both include statements were reread, along with the included material-batch call at GPU line 25, shader program construction/initialization at line 48, same-cache submission at source-geometry line 288, and cache release at line 78. The shader/GPU/source-geometry include files and NativeBatchProgramV113.cpp retain their previous hashes. Source binding remains present.

This refresh does not resolve F1: native source still submits assembled chunks directly to the driver, while original Storm repairs/optimizes source before submission. **#1919 remains partial; unresolved per-body comparisons remain unclear; all runtime acceptance remains not_run.** CSV has no model_renderer.cpp citations to replace, and the directly cited compilation boundaries retain their hashes, so lane-19.csv is unchanged. This was an anchor/owner connection reread, not renewed per-pass Mesa or HookZz equivalence review. No game source, build, test or emulator was changed or run.

## Exact disposition counts

| Status | Count |
| --- | ---: |
| matched | 0 |
| partial | 1 |
| disconnected | 0 |
| missing | 0 |
| unclear | 1238 |
| import/thunk | 251 |
| clone | 137 |
| failed-body | 0 |
| out-of-scope | 237 |

Unique cross-lane function-target xref edges retained: 4852. All incoming/outgoing function targets and explicit callback pointers are in CSV; indirect targets remain unresolved.

## Source snapshot hashes

Source changed during captured hash window: port/android-native/app/src/main/cpp/model_renderer.cpp. This window does not establish immutability throughout earlier reads.

| File | SHA-256 |
| --- | --- |
| [port/android-native/app/src/main/cpp/CMakeLists.txt](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/CMakeLists.txt) | ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45 |
| [port/android-native/app/build.gradle.kts](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/build.gradle.kts) | fd359e37a3c27feb6a3b4275f1cd0a15e947ff83d66153b13cb10ed5ab5a370a |
| [port/android-native/app/src/main/cpp/authored_shader_program.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/authored_shader_program.cpp) | f7c0cc40d71f5dd00c802bf83b36afe1378d7c576c43e73a86dfb5da04d30c58 |
| [port/android-native/app/src/main/cpp/native_app.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_app.cpp) | a24f348fafea17e488bf5b3cdf97f832651818f45e78077a59b3a6f5407ddef3 |
| [port/android-native/app/src/main/cpp/native_batch_program_v113.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_batch_program_v113.cpp) | 10b946d127afbf419fe06b784036d4021edba01a06f0edfe68d332ff5e274a4d |
| [port/android-native/app/src/main/cpp/native_batch_program_v113.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_batch_program_v113.hpp) | dc905f4aeac127346d53a28961cb6826186ceb5bd03695d0bd3c42f1b15bcd7e |
| [port/android-native/app/src/main/cpp/model_renderer.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/model_renderer.cpp) | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca |
| [port/android-native/app/src/main/cpp/renderer_native_batch_shader_v113.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/renderer_native_batch_shader_v113.inc) | db5594805bb526b7d23b8f39ba6a41c2289d5d599fb502d25eb8b9e0ed6c538a |
| [port/android-native/app/src/main/cpp/renderer_native_batch_gpu_v111.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/renderer_native_batch_gpu_v111.inc) | d1fcf4462c81f1afe5400e09bd4aa822e80ddaa77844073f8d9b4d2103daec00 |
| [port/android-native/app/src/main/cpp/renderer_source_geometry_v64.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/renderer_source_geometry_v64.inc) | c3715999f5979ef9f6b16fe8cbdbba62d56dd9430eb5b81b18ee15f7dbaa60fa |
| [port/scene-materials/shader_sources.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/scene-materials/shader_sources.cpp) | f05874e30bba3a3b1c8cec643e5fda6bcba4f31e9b935eae8e94c9872b41cde1 |
| [port/scene-materials/shader_reflection_v4.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/scene-materials/shader_reflection_v4.cpp) | 027ab73c484cbf598cd95bceadbddaa62c6e4c54fe9e85e7b03129e0331f048b |
| [compatibility/work/fold7-build/patch_storm.py](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/patch_storm.py) | 43215644520531e32cf47b6cf87f20e9ddcd0dd1e01017f5c3155c106a12b47b |
| [compatibility/work/fold7-build/storm_import_fix.c](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/storm_import_fix.c) | 57e5e91b4269dd4c670b1f2205b57f22483f4820458c53d32102c2d7c1472b27 |
| [compatibility/work/fold7-build/storm_bias_fix.S](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/storm_bias_fix.S) | 0a1b353820d886cbce855448c40eefb97d6b71b43e390e07daa76ea98905b134 |
| [compatibility/work/fold7-build/patch_game.py](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/patch_game.py) | 16133e6ceb99b88f3f9f42d79ee6736ff7ca8061420d0deec5ed6569342c575c |
| [compatibility/work/fold7-build/native-load-test.log](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/native-load-test.log) | 2bab2c40565dfd19789133919a86559597e98c332f160d2fd268637f31ed0a89 |
| [compatibility/work/fold7-build/native-probe-test5.log](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/native-probe-test5.log) | d5f309eb2b392e9615df7abd827e8fc6feb09ea0e9e9ace0608140a77b4187d8 |
| [compatibility/work/fold7-build/phone-test4/dh2-logcat.txt](C:/Users/adamc/Desktop/workspace/DH_sc/compatibility/work/fold7-build/phone-test4/dh2-logcat.txt) | c37d01782f83350e56c7f9127d148e240e652f9ef2542f8e74d93b82827ab2db |

| IDA artifact | SHA-256 |
| --- | --- |
| functions.jsonl | 1f9dd13c36983c0b103fa96a0bd3af5071153d6d719e2591c69eff544bfa2ea6 |
| xrefs.jsonl | 3658327c67ad268a88a290c27cb4bdbf31ee3c84719a72a3d75d93b150d1ce98 |
| imports.json | eae763841f89a092a33ebc4202d2458671f815fd2e30a24d7777ad970a8f8e00 |
| decompile-failures.jsonl | fad75b7dab396d97d5be03ccee9832fb52c3f897c2f42c28940d60624d0880c8 |
| assembly-functions.asm | 51efa100cb61aee4fe1fa5c89b5a5f0827ec966e756148250a420b09cb2cb8f5 |
| elf-symbols.jsonl | d0a41956e9517296bb454416fee542c632218bf5dc9d2e22d521e66686afcc91 |
| segments.json | 4f3cb1736a107167da607a36ce0a99d93a168d2c4cd8c4810d76f9809a985440 |

## Identical-body representatives

Every clone remains an individual CSV row. Grouping requires identical bytes plus normalized body preserving resolved literals/identifiers; neither dynamic owner nor source parity is inherited.

| Representative | Assigned duplicate IDs | Normalized body SHA-256 |
| --- | --- | --- |
| #1500 | #1885 | 2538f0529d3b684b3559a15b64d78d9c39f51032443bcbd68c8bbbb797230414 |
| #1506 | #1891 | 7154d9378e1781e2322170f251d4714e7d9fb323b79022e23d332f3aa0747f68 |
| #1513 | #1898 | 989bc2e9214a96c3e2fe9e31a708849644540c17675b4f3462ee57c261c5acb9 |
| #1516 | #1901 | bc585b628b91fbbae97f5ad8bbad2cd575e5c7f1f03182bee25b893307b8fe6f |
| #1517 | #1902 | a156e9dad52c35fc9e4f6feeee783d15a18cf7c7a0fa98c5ee933e04d4164734 |
| #1521 | #1906 | 99fd35f311cf2685f3b7baf2cbfe9d8f8b841eaaeaa067812b2580a3d5735c48 |
| #1523 | #1908 | de72ddf403bcd02d45b9b5bfa7854dad54006c5a73132be177697af42d2304e2 |
| #1638 | #1936, #1937, #2006, #2023, #2145, #2308, #2324, #2342, #2418, #2454, #2488, #2521, #2522, #2574, #2608, #2620, #2628, #2666, #2752, #2755, #2756, #2757, #2758, #2759, #2760, #2761, #2762, #2763, #2765, #2769, #2770, #2771, #2772, #2862, #2871, #2879, #2956, #2965, #2996, #3027, #3068, #3305, #3356, #3428 | 41b805ea7ac014e23556e98bb374702a08344268f92489a02f0880849394a1e4 |
| #1662 | #2064, #2065, #2066, #2067, #2148, #2149, #2266, #2267, #2300, #2302, #2307, #2313, #2887, #2905, #2914, #3056, #3057, #3061, #3064, #3286, #3306, #3437 | 71450af6462dcee295e9d88cab76ae5d6aaaa239e871f4da75ac1c52d55f0e6f |
| #1761 | #2900, #2924, #2951, #2969, #2970, #2971, #2973, #2976, #2993, #3000, #3012, #3013, #3058, #3059, #3063, #3065, #3108, #3109, #3118, #3232, #3348 | 31a0a6ccfb9540f2a97c417020675fa158c6f93b308a917eedd94ebab1e6c62e |
| #2122 | #2287, #2290 | b5ae2a16870e4e82c96f3c3e2b6eac96f8c9ad155cdb3fc5c0b9d05d4a8dcdb5 |
| #2146 | #2320 | 457455376a30a42709c24dd7a14dad57ba716c7d519001c72211eef3ec6eba8e |
| #2283 | #2284 | e2f4c0440f69ef425b2f16a9ea23c331c36695024c061d04ae6f90806c64827c |
| #2347 | #2348, #2349, #2369, #2371, #2373, #2375, #2377, #2379 | e62f81ecac7b6d988b3216dd1485291c35b636a0644bf2320f4641fd44c02224 |
| #2370 | #2372, #2374, #2376, #2378, #2380 | 6f933ff4a2b56486dc72e06f11cd67459b683481703adadcf44281a1c7346033 |
| #2426 | #2465 | 6849ec7cd6e8674d29f2b8f8dae51ac569918dc21b8fed0fbd770e36776fe96d |
| #2527 | #2529 | 25ef5a6e381f3201f555247c0d3632a39995c7a0ee2890edeee7e07363a56a7c |
| #2532 | #2533, #2534, #2535 | 6f50b671b4d46e15d1f68b80609ec73859f7dfabfb4ce769d0b08ce423dfd50e |
| #2536 | #2537 | 94e49682c5ecb21f0c845aec0287faa6db1c114e3285eca7ef93414473506710 |
| #2538 | #2540 | 3cf770a36f83ad2977ae981cd092e26306d6cf7bf6e0a5caf0ac95624bdb9c4e |
| #2543 | #2544, #2545, #2546 | e493fe83945c17862270f7729ae890292e5fc0981e359728bc457bdf2914406a |
| #2547 | #2548 | 94e49682c5ecb21f0c845aec0287faa6db1c114e3285eca7ef93414473506710 |
| #2549 | #2551 | 9ed26c4c1dd51bb27649d4306c38126dadada247c906a6ca0e5b14c05c97f529 |
| #2554 | #2555, #2556, #2557 | 671fbc1b47b18fa0218a4ff852abf78020ef33c41ace222c825126dc82b1c1cc |
| #2559 | #2563, #2565 | 6946056d7a8662ca10963a8c8bc9908b7fcc3f131ac7958a17d5d800fd5d0b55 |
| #2560 | #2724, #2726 | d4d70105a5e46daa22510feaab0955bad07d19b6ff8cc74c26702cc9b4f98cf4 |
| #2561 | #2564 | 27b131920113c395ff737c134410a7f094faa2d2e4335f77b351641fd7343f13 |
| #2721 | #2777 | bc346877f9f164b35e2ed3411f4c9f79aed4818fa2d3814814f07c87196ef419 |
| #3011 | #3372 | 131b57e98cea8f81686a02216d123de12cfaed2bab80ceb46f0693b58a11bcdb |
| #3039 | #3041 | 2ce3faa26def6fcaf2ebfbdb65b12a2a7e9e624e8e57c5aa79fffe83ba4d9566 |
| #3142 | #3151 | 75a414e477b60828cefd9e8ee3cc75bbaa05b7d1d7dceedc3190ef2234f0041a |
| #3146 | #3155 | ebdec5911ac956ac96af443eeea0bf9d3dfd6aa948fbee01a8e99b5db105a97a |
| #3464 | #3472 | 41b805ea7ac014e23556e98bb374702a08344268f92489a02f0880849394a1e4 |
