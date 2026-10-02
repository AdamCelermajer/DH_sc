# Work required for a playable modern Android port

The current development target is Android 17 in official emulators; no Fold7 or other physical device was tested for this update. The owner's earlier Fold7 / One UI 8.5 target remains historical context. The supplied engine is ARM32. A complete native ARM64 restoration still requires the engine and its dependencies to be rebuilt from validated source, or a separately validated runtime translation approach. This repository concentrates on source recovery and evidence for reconstruction.

| Workstream | Evidence available | Required result |
| --- | --- | --- |
| Engine class reconstruction | Named methods, vtables, relocation targets, disassembly, pseudocode and original build filenames | Valid C++ declarations/layouts and implementations with behavioral tests, including indirect calls and lifetime rules |
| Engine algorithms | Address-indexed function bodies and exact instruction bytes | Subsystem implementations checked against original ARM32 execution or controlled gameplay captures |
| C++ runtime | STLport/debug types, libgcc records and import inventory | Consistent modern compiler/STL ownership, allocation, exception and ABI rules |
| JNI and reflection | 46 DEX declarations; native imports/exports and call sites | Exact Java member-name contracts, pointer-safe native handles, thread attachment and exception handling |
| Rendering | Native symbols/imports, shader source and binary mesh/texture signatures | Tested graphics initialization, texture/mesh loaders, shaders, surfaces, resize/suspend and GPU behavior |
| Asset pipeline | XML configurations; BRES/BTEX/PVR evidence and original file hashes | Complete cache, decoded resource schemas, format readers and reproduced scene behavior |
| Storage | Original paths and level/resource references | Storage-picker import, consistent app-owned paths, reliable startup without obsolete downloads |
| Audio | WAV/VoxN signatures and native/audio call evidence | Format decoding, playback, suspend/resume and correct resource lifetime |
| Network/vendor services | Java billing/licensing layers and Storm symbols | Studio-approved replacement or restoration of unavailable endpoints without silently changing gameplay contracts |
| Packaging | Compilable isolated JNI source and Android SDK/NDK reproduction steps | Fully rebuilt ARM64 engine/support libraries, current Android lifecycle and installation, tested save handling |
| Game validation | Exact original binary and reproducible recovered evidence | Launch/menu/new-game/loading/combat/quests/save/load tests on device, followed by extended gameplay |

## Suggested reconstruction order

Binary-specific obstacles: the engine imports old Bionic globals (`__sF`, `_ctype_`, `_tolower_tab_`, `_toupper_tab_`), `libstdc++` and ARM `__aeabi` helpers. Modern source should use public libc APIs and a consistent current C++ toolchain. Its graphics calls include GLES2 shader/VBO/FBO operations despite a GLES1 dependency string. Storm contains `HookArm`/`HookThumb`/HookZz and shader-optimizer/Mesa symbols, with `mprotect`, `cacheflush` and dynamic symbol imports. Isolate the ARM-specific patching layer before porting it; this is distinct from gameplay reconstruction.

1. Retain the now-complete supplied cache and exact APK with their hashes; seek the cleanest available original APK from the rights holder. Compare supplied save-restoration additions before deciding which behaviors belong in a studio build.
2. Use the original source-file/symbol index to ask for archived engine headers, SDK projects, export tools, shader build pipelines and resource schemas. Even incomplete headers or tools can remove substantial uncertainty.
3. Build a controlled original ARM32 execution/reference environment for routine-level and gameplay traces. Static inspection alone cannot validate an entire game.
4. Audit every JNI/reflection member name and restore the Java/native contract. Compilation repairs may change names or expose existing bytecode/decompiler ambiguities.
5. Reconstruct foundational engine types and their ownership/layout rules, then isolate resource loading, rendering, scene graph, input, audio and game-state systems. Validate each against the original before migrating callers.
6. Replace address-sized assumptions with reviewed handles/types and rebuild all native components under one ARM64 toolchain. Do not preserve ARM32 object layouts by blindly widening fields.
7. Integrate app-owned cache import and Android lifecycle handling. Test permissions, surface loss, suspend/resume, audio interruption and saves on current Android emulators; test device-specific display behavior if a physical target is requested later.
8. Run actual gameplay validation before labeling the result a restored game or offering it as a release build.

The current workspace supplies a substantial recovery base, reconstructed native support, engine math covering 20 original function starts, and resource loading covering 35 complete bodies plus the whole-buffer BRES initializer branch. Math passed 21,477 original-ARM32/compiled-ARM64 comparisons. Resources passed 10,449 reader comparisons and every fixup in all 2,901 recovered BRES images. The resource component preserves four-byte serialized offsets and resolves separate native pointers instead of widening fields in place.

`port/asset-payloads` now adds interleaved type-0 mesh decoding, deferred buffers contained in complete files, raw animation keys and original typed keyframe search. The full-cache audit covers 10,924 meshes and 890,301 time keys; 26 complete animation bodies pass 271,970 original-ARM32/compiled-ARM64 comparisons. A separate reader checks the embedded mesh bytes in all nine type-1 geometry records, but their opaque prefix and spline semantics remain unresolved; the normal mesh API preserves the original runtime's rejection of type 1. Local-space OBJ and raw animation JSON export are available, and 1,149 meshes also pass host-versus-executed-ARM64 decoding checks.

Checked image/material views, static scene traversal, a host WebGL preview and a source-built Android candle renderer now exist. The next engine work is runtime scene/controller behavior, animation application, skinning and ownership across loading and rendering. Bulk allocation and external/split-file loading also require reconstruction. One textured primitive is not complete game rendering or gameplay; these components do not provide a completed engine replacement, playable source-built APK or original studio C++ project. The separately tested Android 17 gameplay APK still runs the original ARM32 engine through a compatibility translator.
