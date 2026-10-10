# Lane 11 - libDungeonHunter2.so indices 18661-20526

Record coverage: **1,866 / 1,866**, each inclusive integer exactly once. Decompile successes: 1,866; failed bodies: 0. Every pseudocode file was read in full by the body/assembly/xref sweep, with all exported assembly blocks, direct call/tail/data references and raw vtable slots recorded per row. All 1,866 LF-normalized pseudocode SHA256 values match the inventory; raw CRLF-file hashes are recorded separately.

**Semantic closure is incomplete.** Individual comparison or subsystem dispositions cover 145 selected records. The remaining rows have body/assembly/xref dispositions with explicit unproved semantics, not parity claims. Matched means the stated logical behavior within a bounded valid input domain; it does not certify ARM ABI, all error/lifetime branches or integrated gameplay. Every runtime field is not_run.

README baseline: HEAD 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. Source evidence is the dirty working tree. Concurrent changes were detected in model_renderer.cpp and port/level-world/CMakeLists.txt; relevant selections were rechecked after the pause. The source ledger records initial and final hashes. Only lane-11.md and lane-11.csv were written; no game source edit, build, test, emulator, external message or other report edit was performed.

## Coverage counts

| Disposition | Records |
|---|---:|
|matched|37|
|partial|66|
|disconnected|1|
|missing|9|
|unclear|1256|
|import/thunk|220|
|clone|184|
|failed-body|0|
|out-of-scope|93|

| Subsystem | Records |
|---|---:|
|animation-accessor|49|
|batch-geometry|28|
|bres-file|53|
|bundled-jpeg|190|
|bundled-png|311|
|bundled-zlib|18|
|collada|296|
|device-core|179|
|morph|17|
|native-gui|174|
|particle|358|
|pvrtc|4|
|shared-string|10|
|skin|138|
|timeline|41|

Named virtual/nonvirtual thunks:203; additional attributes-thunk aliases:17. Unique code SHA256 values:1591; additional byte-identical records:275. Every record remains listed. Thunks certify adjustment/alias only; clones certify identical bytes only,without merging receiver/source contracts.


## F11-01 - Particle motion chooses the wrong random-direction branch

**Confirmed static divergence; high confidence.** #18912 at 0x653bb8 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0065/00653bb8.c)) chooses a random normalized direction only when all three configured Direction components equal zero. If any component is nonzero, it uses the authored vector, applies configured Euler variation and the current world matrix, then normalizes and multiplies by randomized Speed.

Assembly independently confirms the condition. The X equality comparison at 0x653dcc proceeds to Y/Z checks through 0x653dd8 BNE loc_653CC4. Y/Z comparisons at 0x653ccc and 0x653ce0 branch to authored handling at 0x653ddc on non-equality. RandVec at 0x653cf4 is reached only after all three equal-zero checks.

The current [port/engine-animation/particle_cloud_models_v1.cpp:45](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/particle_cloud_models_v1.cpp:45) tests all three components for **nonzero**. Zero vectors therefore stay zero and lose three random draws; all-nonzero authored vectors become random and consume three draws the original did not consume. Following particle spin/random state also changes.

The path is connected. [port/android-native/app/src/main/cpp/source_campaign_fx_v77.cpp:155](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_fx_v77.cpp:155) publishes CharacterAuthoredResourceFactoryV6 to the live CharacterMeshFxOwnerV4. [port/level-world/character_authored_resource_v6.cpp:107](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_authored_resource_v6.cpp:107) initializes parameters; [port/engine-animation/particle_resource_init_v2.cpp:20](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/particle_resource_init_v2.cpp:20) writes serialized Direction into registered model fields. [port/level-world/character_authored_resource_v6.cpp:128](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_authored_resource_v6.cpp:128) invokes the cloud whose [port/engine-animation/particle_cloud_runtime_v3.cpp:20](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/particle_cloud_runtime_v3.cpp:20) reaches this kernel.

Original mixin vtables at 0x97fc80 and 0x981ac8 contain #18912 at table offset +0xb8 and adjusting thunk #18913 at +0x244. #18881 at 0x6532cc ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0065/006532cc.c)) invokes that virtual initializer for new particles. A direct-xref list containing only the thunk is not unreachability evidence.

The condition and connected path are established statically. Actual asset Direction values and runtime effects were not measured; acceptance is not_run.

## F11-02 - Active billboard fails above 30 particles; wider correction is disconnected

**Confirmed conditional failure on a connected path; high confidence.** #18868 at 0x651eec ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0065/00651eec.c)) computes camera distances, bounds, optional local translation, then sorts the entire particle range. Its original body has no 30-particle rejection.

The active V6 resource admits capacity up to 16,383 at [port/level-world/character_authored_resource_v6.cpp:56](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_authored_resource_v6.cpp:56), then calls [port/level-world/character_authored_resource_v6.cpp:61](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_authored_resource_v6.cpp:61). V1 rejects n > 30 at [port/engine-animation/particle_billboard_v1.cpp:21](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/particle_billboard_v1.cpp:21). An admitted emitter reaching 31 active particles returns -1 through apply/cloud update to scene_frame, instead of the original sorting/render behavior.

The helper at [port/engine-animation/particle_billboard_v32.cpp:19](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/particle_billboard_v32.cpp:19) admits 16,383 and its V32 resource source calls it. However [port/engine-animation/CMakeLists.txt:8](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/CMakeLists.txt:8) omits particle_billboard_v32.cpp and particle_resource_init_v32.cpp; [port/level-world/CMakeLists.txt:298](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/CMakeLists.txt:298) selects V5/V6 factories and omits character_authored_resource_v32.cpp. The actual campaign factory is V6, as F11-01 shows.

The scoped production caller/target search found V32 only in its own definitions/headers and the unselected V32 resource. The live V6-to-V1 call and explicit target lists are positive evidence of the disconnect. The wider correction is classified disconnected; the active implementation is partial.

The world target list changed concurrently and is flagged in the hash ledger. These selections/omissions were rechecked after resuming and remained the same. Actual authored counts above 30 remain an asset/runtime question. No source reconnection or runtime experiment was performed.

## F11-03 - Original cylinder and spin/direction branches are rejected

**Confirmed partial domains; asset reachability unresolved.** #18784 at 0x64e750 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0064/0064e750.c)) constructs box type 0, sphere type 1, and cylinder type 2. #20225 at 0x69e1bc ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0069/0069e1bc.c)) draws cylinder height, angle and radius and combines the domain basis/center. Active [port/level-world/character_authored_resource_v6.cpp:52](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_authored_resource_v6.cpp:52) admits only types 0/1. [port/engine-animation/particle_resource_init_v2.cpp:12](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/particle_resource_init_v2.cpp:12) also requires DirectionMode 1 and SpinAxisType 0.

#18782 at 0x64e3c8 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0064/0064e3c8.c)) supports supplied axis or velocity axis with positive SpinAxisVariation by drawing three angles, rotating XY/YZ/XZ, then normalizing. The source [port/engine-animation/particle_cloud_models_v1.cpp:37](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/particle_cloud_models_v1.cpp:37) rejects positive variation for nonzero axis types and rejects axis types above 2. The original treats nonzero axis types other than 2 as supplied-axis selection.

#18933 at 0x656450 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0065/00656450.c)) includes direction modes 0/1/2 and multiple spin-axis initialization branches. The connected V2 initializer rejects the wider cases before production. V32 admits SpinAxisType 0/2 but is disconnected as F11-02 shows.

Specific life/size/spin-apply/motion-apply kernels have matched valid-domain rows. That does not close the wider factory/update domain. Original #18786 at 0x64e830 also samples per-particle texture/color animation with time mapping; the cloud uses [port/engine-animation/particle_cloud_models_v1.cpp:65](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/particle_cloud_models_v1.cpp:65). V6 separately samples material color, which is real support but does not prove the complete original per-particle branch.

## F11-04 - Morph controller construction/consumption is absent on the traced path

**Missing connected capability; high confidence about the rejecting path.** #18667 at 0x64a6e4 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0064/0064a6e4.c)) maps source/target buffers and accumulates weighted morph vertices. #18668 at 0x64afec ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0064/0064afec.c)) calls it during rendering preparation, and it reaches cross-lane weighted-vertex helper #18649 at 0x649b3c.

The current [port/scene-materials/scene.cpp:169](C:/Users/adamc/Desktop/workspace/DH_sc/port/scene-materials/scene.cpp:169) rejects controller type != 0 before geometry/consumer construction. [port/engine-skinning/skinning.cpp:59](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-skinning/skinning.cpp:59) repeats the skin-only condition. The retained visual loads controllers through that skin consumer.

The scoped search covered morph/controller/weighted-vertex behavior in current scene/material, animation, skinning, retained-world and Android renderer code. The missing claim concerns the explicitly rejected type-1 consumer on this actual path; it does not rely on a literal CMorphingMesh symbol miss. Actual gameplay use of morph controllers was not established.

## F11-05 - BRES complete-file support does not close original external/split modes

**Partial file/lifecycle coverage.** #20150 at 0x69a40c ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0069/0069a40c.c)) performs in-place pointer relocation, supports two high-bit-selected ExternalFilePtr slots and split/deferred chunks. #20152 at 0x69a880 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0069/0069a880.c)) reads chunk tables and can load a dependent resource through #18990. #18975 at 0x658c90 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0065/00658c90.c)) post-processes external resources/material/effect/controller/emitter references after manager construction/cache.

[port/engine-resources/resources.cpp:159](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-resources/resources.cpp:159) rejects relocated inputs, external_base != 0 and noncanonical header/fixup layouts. [port/asset-payloads/payloads.cpp:26](C:/Users/adamc/Desktop/workspace/DH_sc/port/asset-payloads/payloads.cpp:26) borrows deferred bytes from complete-file storage and requires a zero cached pointer. [port/level-world/scene_preload_owner_v81.cpp:5](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/scene_preload_owner_v81.cpp:5) supplies admitted retained construction/rollback.

This is connected admitted complete-file support. It does not establish mutable onDemand cache, pointer-slot, segmented-reader, canonical filename-cache or post-load lifecycle equivalence.

A false callback finding was avoided: #18989 at 0x65a98c ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0065/0065a98c.c)) returns null for the async flag and otherwise delegates get. The original overload ignores its supplied callback. A source callback-name miss would not demonstrate a defect for that overload.

## F11-06 - Software equations are supported; hardware/proxy lifecycle is unproved

#19580 at 0x66fe34 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0066/0066fe34.c)) computes joint world * inverse-bind * bind-shape under a dirty cache. #19581 at 0x66ff48 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0066/0066ff48.c)) writes weighted positions and optional normals, stops at first zero weight, and writes zero for unweighted vertices.

[port/engine-skinning/skinning.cpp:40](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-skinning/skinning.cpp:40) and [port/engine-skinning/skinning.cpp:126](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-skinning/skinning.cpp:126) preserve those equations in the admitted domain of 1-4 influences and at most 256 joints. [port/level-world/retained_gameobject_visual_v1.cpp:149](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/retained_gameobject_visual_v1.cpp:149) connects both to retained mesh storage. [port/engine-skinning/skin_pose_cache_v32.cpp:17](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-skinning/skin_pose_cache_v32.cpp:17) caches using joint-world byte comparisons.

The original range also contains hardware matrix, dual-quaternion and texture techniques, availability checks, pointer caches and mapped proxy buffers. Full source closure for hardware selection/shader representations, dirty-bit identity and proxy-buffer teardown was not established. Those rows remain unclear/partial; a CPU kernel match cannot prove that larger path.

## F11-07 - Bundled codecs, system z replacement, and missing image decoding

Indices 19631-19648 contain 18 zlib bodies; 19649-19838 contain 190 IJG/JPEG bodies; 19839-20149 contain 311 libpng bodies. They are embedded implementations, not imports. Actual branch aliases are disposed as thunks separately.

Original inflate #19642 at 0x672f48 receives calls from CZipReader::openFile #14081, GameSWF inflate_wrapper #23599 and inflater #25385, plus PNG code. Current [port/asset-payloads/zip_asset_pack_v1.cpp:137](C:/Users/adamc/Desktop/workspace/DH_sc/port/asset-payloads/zip_asset_pack_v1.cpp:137) uses raw system-z deflate and [port/engine-ui/CMakeLists.txt:38](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/CMakeLists.txt:38) links system z. This proves a live replacement consumer, not exact legacy version/state/malformed-input/allocation/error parity.

Original JPG loader #16499 at 0x604be0 calls jpeg_CreateDecompress, jpeg_read_header #19729, jpeg_start_decompress #19738 and jpeg_read_scanlines #19734 before CImage construction. Original PNG loader #16509 at 0x6050dc reaches png_create_read_struct #19950, png_read_info #19944, png_read_update_info #19943 and png_read_image #19941 through the real CImage path. The two loader bodies were inspected outside this lane to establish these edges.

Current [port/engine-ui/CMakeLists.txt:30](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/CMakeLists.txt:30) disables JPEG/PNG. The disabled JPEG wrapper begins at [port/engine-ui/vendor/gameswf1714/base/jpeg.cpp:514](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/gameswf1714/base/jpeg.cpp:514) and returns null from its input factory. Active [port/android-native/app/src/main/cpp/renderer_model_texture_budget_v40.inc:64](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/renderer_model_texture_budget_v40.inc:64) reaches [port/engine-textures/textures.cpp:59](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-textures/textures.cpp:59). Its parser accepts PVR or true-color TGA; JPEG/PNG signatures fail the TGA checks.

Scoped current C/C++/Java/Kotlin search covered BitmapFactory, ImageDecoder, decodeByteArray, stbi_load, JPEG/PNG creation APIs and codec flags. No alternate decoder on this path was established. The missing claim is anchored in active upload, explicit parser rejection and disabled codec code. Internal metadata/transform/encoder/writer functions remain unresolved because their runtime demand was not established; they are not all declared missing.

## F11-08 - Native GUI/device/init/shared-string closure remains open

#20359 at 0x6a574c ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/006a/006a574c.c)) dispatches native widget construction through GUI-environment virtual slots. #20403 at 0x6a6eac ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/006a/006a6eac.c)) handles focus/press/release and Enter/Space/Escape before parent event 5. #20431 at 0x6a7ed8 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/006a/006a7ed8.c)) toggles checked state and parent event 7. #20509 at 0x6ad02c ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/006a/006ad02c.c)) traverses submenus and dispatches event 18 to a parent or alternate event parent. Raw vtables contain these substantive methods despite absent direct caller xrefs.

Current [port/android-native/app/src/main/cpp/native_app.cpp:69](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_app.cpp:69) uses GameSWF sessions. Scoped current production C/C++/Java/Kotlin and active target-list inspection did not establish a native-widget/factory equivalent or original gameplay demand. These rows remain unclear; a name search miss is not missing/unused proof.

Original init #20310 executes registered callbacks forward at first init; exit #20309 executes them in reverse at last exit; #20354 allocates/cleans the interned shared-string heap. Registration/refcount/identity remain substantive unresolved paths. Constructors whose actual bodies only assign float 0.5 are separately disposed as static constant artifacts. Original Android postKeyEvent #20254/postMouseEvent #20255 are empty; no event-delivery implementation is inferred for those bodies.

## Cross-lane edges and unresolved list

All exported direct call/tail references and raw vtable slots are retained per row. Inspected edges include:

- Particle #18912 -> #18075 Rand, #18215 RandVec, #18146/#18147/#18148 Euler rotations and #2307 normalize; spin #18782 reaches the same family.
- BRES #18989 callers include #16896-16899 database constructors, #17299/#17300 animator constructors, #17373/#17374 scene constructors, #17388/#17389 node constructors and #20709 binary loader. #18975 reaches database/effect/controller/node getters in lane 10.
- Skin #19581 -> #14848 vertex streams and #14876/#14878 map; morph #18667 -> #18649 weighted vertex and map.
- Inflate #19642 receives ZIP/SWF #14081/#23599/#25385 and reaches #21769 adler32, #21772 crc32, #21793 inflate_fast. JPG/PNG loader chains originate at #16499/#16509.
- PVRTC #20235 receives original pixel_format decompressor #16365; source texture_decode reaches its reconstructed decoder.
- Quantized-buffer #20335 receives CBatchMesh::quantizeComponents #14179 and maps vertex buffers plus local quantizeScaleOffset helpers. Receiver/stride/ownership equivalence is unproved.

**Remaining work:** 1,256 unclear rows need specific function-level semantic closure. The 66 partial rows retain omitted domains or lifetime/ABI uncertainty. The 9 missing rows refer to the rejected morph/image decoder paths above. The disconnected row refers to the wider billboard source.

General indirect target/registration order, synchronized animator behavior, packed animation scale/offset branches, hardware skin/proxy buffers, native GUI demand, BRES external/split modes, and asset-specific particle/codec/morph reachability remain open. Helper dispositions do not certify custom element ownership or caller behavior. Historical helper audit claims were not used as integrated acceptance. Runtime is not_run throughout.

This is exact record coverage with incomplete semantic parity. The whole audit must not be called complete on this inventory sweep alone.

## Snapshot and exporter hash notes

After resuming, HEAD remained 75a7c2fe3261403e841ffbeb46734e9e4e84e75c on branch reconstruction/native-textures-android. The final source hash check detected no further changes after the cited snapshot.

All raw pseudocode file hashes differ from inventory hashes because the Windows exporter writes CRLF while hashing an LF string. export_in_ida.py lines 67-69 establish that mechanism. All 1,866 LF-normalized hashes match inventory: zero normalized content disagreements. Each CSV row includes its actual raw-file SHA256 alongside the inventory pseudocode SHA256.

Concurrent changes were detected in model_renderer.cpp and port/level-world/CMakeLists.txt; the renderer changed again across the pause. The source ledger records review-start and final hashes. The active V6 factory and V1 billboard selection were rechecked after resuming; no V32 production selection was introduced. Every source citation above refers to that file's final hash in the ledger.


## Repeated identical body groups

All assigned records remain individually listed in CSV. Equal machine bytes do not prove semantic/source parity.

| Code SHA256 | Record numbers |
|---|---|
|379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f|18684, 18690, 18698, 18702, 18704, 18708, 18718, 18724, 18939, 18944, 18945, 18946, 18994, 19070, 19100, 19133, 19134, 19149, 19187, 19238, 19240, 19242, 19327, 19328, 19332, 19348, 19352, 19353, 19374, 19376, 19377, 19400, 19403, 19404, 19407, 19430, 19590, 19592, 19593, 19598, 19716, 19817, 19829, 20033, 20153, 20154, 20156, 20158, 20160, 20162, 20164, 20166, 20186, 20236, 20247, 20248, 20249, 20250, 20254, 20255, 20256, 20257, 20259, 20261, 20262, 20264, 20278, 20284, 20294, 20296, 20312, 20313, 20314, 20315, 20356, 20364, 20384, 20417|
|e20961135846c524c556124f20a02a725366faffb19600565e5edcd246e9e20b|18714, 18720|
|aadcc991add2df8aca7f4886b704db94448f301a766e968820aedbc6fde3955b|18715, 18721|
|aa851e22e3c0c8cac54d3868afca7162c50d67e73ae5844340db50cefdb5a8f6|18716, 18722|
|cada6afba07a3e9d2f18d816239a74b2e68d3a2e715c73373e9e1e8ea07837ee|18717, 18723|
|6aa6e05cf158a4cc1081697d904dff0d8f476e3c878574f13ba6767fbd3b1b5e|18719, 18725|
|02da354b9ff7906a3fd8777685b1249044054db02e6a4ae796b8fd9898b7511e|18741, 18743, 18745, 18751, 18753, 18755, 18757|
|90f5106ab87567f0598ad6b90617ed2a9e529cc87b88479636f6cbcd5bf2ff00|18749, 18763, 18837, 18903, 18907, 18911|
|b91a7a728857ca485c19e29ce2ff7efe76c05d2b3e2b5d28cda1fd38d7d06248|18777, 18780|
|16b0c4fcc69a9daf1bded409c1237314471a9f3b8ef8b40f12e6b3851eb7d3fb|18796, 18798, 18800, 18802, 18804, 18806, 18808|
|bd1e68b1ab298cde8f7e6b8e7418a9a428fddc0ca77da054e1c5c05d7d714cd3|18816, 18827, 18882, 18893|
|4b810a2d45dd36dfe07ecbb4fea869b1fcec8eb5119f7d3bffd85606270e9f25|18817, 18828, 18883, 18894|
|d4475a13162033f0491ac7bc5a53ae0d756c4ac1d435cf31843d8ae307d458ba|18818, 18829, 18884, 18895|
|0f5b42a24c46a32877dbda7db1ad80e4d3a1df27126965361542888c5c25910e|18819, 18830, 18885, 18896|
|1d0ba545cf4461d0d3ae217e8cc117e13fb6e6f20b04f4930d6ddc62ea6909d1|18820, 18831, 18886, 18897|
|38694bc3437cb8c75d04fe17f995ea9fe7b1623a2324d1d586030b86f727ab88|18821, 18832, 18887, 18898|
|1dfe7d4299e302bd12b10de50451e0a20acd08cef99d6d5438307ed292817854|18822, 18833, 18888, 18899|
|8b97579ab00a1c7dff3ec2da309db4fcac75f9413ef7dab05371be86463e63ce|18823, 18834, 18889, 18900, 19218, 19221|
|691c5ddb063af36a37e0b43c3e9109db3f4d9fd5270c3f8c049dc38a66d36bdc|18824, 18835, 18890, 18901|
|1219ffd69b9fcfa79f4dd507a1d0bfd31e5377942a23dbeb381e0974b2d31964|18826, 18892|
|542011c423a510fa1a11ed7cb3367977f50d009517ea3fb6b4ab4e78ab55c506|18869, 18871|
|b62d182a8b4eadca5b26aaca133eaa43a141f1fe374d758ef57d0b05c3ba4f02|18873, 18875|
|90869a62790dbdc274c8b0a430db29f378378db020522e5c3acd65dcf3fdb413|18905, 18909|
|f22e86c6848307ad4ac59216e6d3d1b129cede4ffdb7ab0ec403a8d90f6af48b|18928, 18931|
|092174ac38e8c45c165bd1fe37e63e38838f97f47f9e8d20aa08ebd619fca6c0|18947, 18948|
|6dcd5e75586fe6683156c0d559d4827b73bd48d501fa5781a1c6099aacd7c877|18949, 19071, 19121, 19331, 19382, 19405, 19490, 19523, 19548, 19591, 19914, 19915, 19917, 19918, 20159, 20253, 20263, 20297, 20298, 20299|
|34b97ea285a49248d21f74e46f26848d3f2c1a90e381ca2c3fe219705331487e|18999, 19058, 19074, 19104, 19150, 19210, 19243, 19282, 19324, 19358, 19369, 19379, 19385, 19408, 19432, 19487, 19493, 19528, 19551, 19570, 19584, 19599, 20151, 20197, 20265, 20285, 20300, 20306, 20316, 20320, 20344, 20355, 20365, 20380, 20414, 20442, 20470, 20520|
|c2a9ddce47351fffdafaaed33bb2e92f451777a12993fcc3733fbc7e54ecd098|19069, 19103, 19148, 19201|
|fa04763cc015275f81b1d864426513948ce8bfb90e0889b5098b558301847cdd|19081, 19083, 19109, 19111, 19159, 19161, 19219, 19222, 19254, 19257, 19419, 19421|
|76dd300acc743c2c8e0a3a152a9cd5935b7e2b399f0836a2938b09c32f1e5287|19115, 19117|
|db5f80338df5dd5fb373b3e05e0741d53805bbc8b7ffbee570f04ae630350732|19122, 19123|
|0ded1e40bf45877f3e1ebf69d486a944141c910157afc4f3813ea8d390a694ec|19197, 19199, 19202, 19204, 19206, 19208|
|3cbbe34b62127ab8ffe7c69208d69465bf4c109daf11e46aee2574eace58cb4d|19237, 19239, 19241, 19244, 19246|
|8c7ef5fe7e8dd102f56ac7fcd7e180c0f95d4555811abeb2cabf484a19bf9138|19253, 19256|
|14e29d8546e16c2862278bddeb80beb41b4943971c5e1bf329c9fce4245d8913|19270, 20161|
|4ad30257073db1d7f6a88f7b5a4494510a68e07477bff36efd7d44c38d0dd5db|19329, 19349|
|d3e487b257f9877b4b2b201f812bf6f41ab2a9b7b43592e2cd6f4c54c49f658a|19333, 20173, 20196|
|df583123d5a6c4eef4235fe2b14fd8948009e6b0a1b5af6123b12b9fe139d8d9|19336, 19429|
|f68520e7644c47739b5b74691da29778feb6b6b8fdeed6ee79f647d2e3ded16e|19401, 19644, 19841, 19842, 19843, 19844|
|ea60a4bb964e206271f3ac5ae383badc75f50a9553113300fc34f823dde5b446|19488, 19489, 19521, 19522|
|f4bfd9dce3ac30a288ffc4f87ef48effc1ba54989f531403c4b48c8a2578f25b|19491, 19524, 19549, 19569|
|8958b19019c78c296207fcd0de4477b24ad8dae0390b9680571a6c63a4e3cbe1|19492, 19525, 19550|
|c037ca5b9370b3630b6d9734768e3f5190890b9bbb15a597780e07497bfda410|19535, 19556|
|d4cda14ab933c3057b8f98d694aca5de348c03257212db23d774067dbb483c2c|19546, 19547|
|7a72d5a43a95f2c76bd0fd5d4caaf3304067351116330e6ba68e3ae2e505e99d|19565, 19566|
|007f34a6c3441b0240da53f253e513b959105da2d8803255e1856aa48f48db47|19567, 19568, 20165, 20246, 20251, 20252|
|360e42c8d5fdec86ec8ac1e7dee4036c011d5d765363a8ab68e21cde9f86df6b|19597, 20163|
|9d2f43e37bc993d1579cb9e87e7f62ab643359789ce5f3d3715b19215beb4699|19948, 20098|
|49f6588984b7c99c20c82f1e3c57050aaa8c28057443b24762b1cc47ca0eed32|19957, 19958|
|2eb021a7b4c06ce008e7fb86284df193a1be8a586419c1975868e6b3d780e3aa|19988, 19989|
|e3e4c9df5dd09b0bbc4549fc8fea06d7e76fbcd5384f1ab1fec62380449bec7a|20168, 20169|
|a9d1d290ee13e0df6b723e3517caa7d5526cea57b5dd4cc45f65fb6d53845f9e|20175, 20182|
|a6857e4a4726555222ec3dd39dd62b12a175e99c3b9ca62c326f55842f3fb966|20184, 20187, 20295|
|63f2ca0fb9f045e43f40d5b9256bf31522277477f1dcb0837b83dd6997ec8f64|20191, 20195|
|4f128083d1b7b4751bdc301e3e674c506c6626c8cbdfae912f5c70ca93e15fae|20279, 20280|
|2dad11aa48e6304fbaf88fdd34385d0a50f03d81ce600cf5ff55988312b8c9ff|20357, 20358|
|3b07b28787aed58fa3f08267bbdcdcc0353b4dfb58a34a18bedfe4eae9532000|20362, 20363|
|9e14f32ce06979797608bb3ca93a3e99460a9d660fbc6bcfb2a190b5ab93322b|20374, 20411|
|f2f0091c38e9d760beba62e243d85c46ab2c7b329c2bceb7d4f98bbd7951f5b9|20376, 20412|
|777571d8b4f6398f189c8b9c0f4584cedaf099528ecd11c965328e341e1a8c75|20382, 20415, 20474|
|f7422c4693f2390a65eef484bc1691fe155ba7f2b49107cab7c8168b47f55a81|20383, 20416, 20475|
|a1b5b1dab996b308b82d813c5f8055c55cbdfe13e7ac4c03408b296e06db7a48|20394, 20422, 20449, 20488|
|236cb65f0355b46071ceb10e3ec03bdac3eaf6144ab4fb76b6dff35af6966337|20395, 20423, 20450, 20489|
|cc0fd7e68594548209965a888cb6dec97a78293fe563e66e8c3b776c97b4b4d4|20401, 20433, 20455, 20497|
|7f8245341b58ec6ec3da51cba0a5beb6ffd1906cb3681ba0b2bdc007a5e7f37c|20402, 20434, 20456, 20498|
|daeeefbc1ab7b9e02443b6028e2e6062a014e7520eb27ba9afb75b5964a28c92|20428, 20494|
|afb96fc44c9826f15b71580129992a005054bcf66f3fcbaffeb5e79cf0965928|20429, 20495|

## Source hash ledger

Every inline source citation refers to the final SHA256 below. Changed files were rechecked for the cited active build/factory selections.

| Source | Review-start SHA256 | Final SHA256 | Changed |
|---|---|---|---|
| port/android-native/app/src/main/cpp/CMakeLists.txt | ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45 | ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45 | no |
| port/android-native/app/src/main/cpp/actual_device_android_v54.cpp | d9d4de58a7dd1b1a416cb0bebb2ed7714c4e30712506069ac8722fa626ced5b6 | d9d4de58a7dd1b1a416cb0bebb2ed7714c4e30712506069ac8722fa626ced5b6 | no |
| port/android-native/app/src/main/cpp/model_renderer.cpp | 68df536883d56f0cd74f5b47052daafd07491faf5a75eec901790fd26afbe015 | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca | yes |
| port/android-native/app/src/main/cpp/native_app.cpp | a24f348fafea17e488bf5b3cdf97f832651818f45e78077a59b3a6f5407ddef3 | a24f348fafea17e488bf5b3cdf97f832651818f45e78077a59b3a6f5407ddef3 | no |
| port/android-native/app/src/main/cpp/renderer_combat_fx_runtime_v4.inc | 474a97258420fbd2a9bb35864b6136042854fad0e8895e46a13baab3f0bcfbd3 | 474a97258420fbd2a9bb35864b6136042854fad0e8895e46a13baab3f0bcfbd3 | no |
| port/android-native/app/src/main/cpp/renderer_model_texture_budget_v40.inc | c4ff74181e39bdf1f5b4f7900f6d4c3021adca0e73b2f901c0803071d2218354 | c4ff74181e39bdf1f5b4f7900f6d4c3021adca0e73b2f901c0803071d2218354 | no |
| port/android-native/app/src/main/cpp/source_campaign_fx_v77.cpp | 2b956db85dae6d6f7f8d2710d2c5b732084aea83115ff3230cf39c30974c8a8a | 2b956db85dae6d6f7f8d2710d2c5b732084aea83115ff3230cf39c30974c8a8a | no |
| port/asset-payloads/payloads.cpp | ebc397d54978acd7da07f6f1c30a5fa7f232be34018af885527a734627a86c6d | ebc397d54978acd7da07f6f1c30a5fa7f232be34018af885527a734627a86c6d | no |
| port/asset-payloads/zip_asset_pack_v1.cpp | 38322384ea3114f1ea3aad0ee0211bb6f583dfaf4a76ae642645fe208a9b17df | 38322384ea3114f1ea3aad0ee0211bb6f583dfaf4a76ae642645fe208a9b17df | no |
| port/engine-animation/CMakeLists.txt | 016b5d5aee4a5dfb1c3d40a116883b0c54e4e4db56e84b42ba7b453b8ff353df | 016b5d5aee4a5dfb1c3d40a116883b0c54e4e4db56e84b42ba7b453b8ff353df | no |
| port/engine-animation/animation.cpp | 968352af9ffb1f1aacba6d6efb4f821beef933b96774abaff8077efea18e26f8 | 968352af9ffb1f1aacba6d6efb4f821beef933b96774abaff8077efea18e26f8 | no |
| port/engine-animation/animation_blend.cpp | 5066e683add9d92643f350aa91819f694a7b2be82201785be40cc7812536d2ac | 5066e683add9d92643f350aa91819f694a7b2be82201785be40cc7812536d2ac | no |
| port/engine-animation/particle_billboard_v1.cpp | 665a84f44d7a8692d585210151107d63aba702c354553ba510f43e8d6f3f8054 | 665a84f44d7a8692d585210151107d63aba702c354553ba510f43e8d6f3f8054 | no |
| port/engine-animation/particle_billboard_v32.cpp | 61433cab652840a91c95ae365345f2dcba46b2acbe48a000e6d01228b5194caa | 61433cab652840a91c95ae365345f2dcba46b2acbe48a000e6d01228b5194caa | no |
| port/engine-animation/particle_bound_forces_v4.cpp | 9d9f1dc4c7b05a2f0b3117d31fb5bd072dc4cb238f50ae487dc3edf16816aa68 | 9d9f1dc4c7b05a2f0b3117d31fb5bd072dc4cb238f50ae487dc3edf16816aa68 | no |
| port/engine-animation/particle_box_v2.cpp | 3e31bb713f0d1f26ba5fe4004f31a7a99d9c8a10e342c6e409ca143c1f36ee6a | 3e31bb713f0d1f26ba5fe4004f31a7a99d9c8a10e342c6e409ca143c1f36ee6a | no |
| port/engine-animation/particle_cloud_models_v1.cpp | 575ee1aa9c64832ceec3a84c4e71a6990474605a790d84f807a9cea82787d747 | 575ee1aa9c64832ceec3a84c4e71a6990474605a790d84f807a9cea82787d747 | no |
| port/engine-animation/particle_cloud_runtime_v1.cpp | 76a3accf7a3998d9bcfe0b6317549a2390e0a4fc3cd40331aea38a12293b854e | 76a3accf7a3998d9bcfe0b6317549a2390e0a4fc3cd40331aea38a12293b854e | no |
| port/engine-animation/particle_cloud_runtime_v3.cpp | f4b1c102bf7c6c5f2fdbe2c5d78e7f851baa40b29616e7e58626160aaae3f2ba | f4b1c102bf7c6c5f2fdbe2c5d78e7f851baa40b29616e7e58626160aaae3f2ba | no |
| port/engine-animation/particle_emission.cpp | 2da6f88c0d8ffc306e7e557fe92887b8a42a621d809c37f38f05f6a7573d776f | 2da6f88c0d8ffc306e7e557fe92887b8a42a621d809c37f38f05f6a7573d776f | no |
| port/engine-animation/particle_factory.cpp | 7b7afcbfb8d52e123a296cfad31e6e6d5d3b3e3265899f845df8e7247abf9bce | 7b7afcbfb8d52e123a296cfad31e6e6d5d3b3e3265899f845df8e7247abf9bce | no |
| port/engine-animation/particle_resource_init_v2.cpp | 2006759c93a39d11d55d159cdcebf415c536d47c249af4521a565e44b9a1d1f1 | 2006759c93a39d11d55d159cdcebf415c536d47c249af4521a565e44b9a1d1f1 | no |
| port/engine-animation/particle_resource_init_v32.cpp | d1689290a55ac75e8b294e3e2410850391e39e8d2795dc128cb0a6cb8aad7b54 | d1689290a55ac75e8b294e3e2410850391e39e8d2795dc128cb0a6cb8aad7b54 | no |
| port/engine-animation/particle_scalar_animation_v6.cpp | e122a17a0e2b7ea4bb7913def1335698017b37628a4a508330dd3c580431d058 | e122a17a0e2b7ea4bb7913def1335698017b37628a4a508330dd3c580431d058 | no |
| port/engine-resources/resources.cpp | 5908520d2a2bdd318669c30e0a6863c023b05327366cc0c1cad782700c945b25 | 5908520d2a2bdd318669c30e0a6863c023b05327366cc0c1cad782700c945b25 | no |
| port/engine-skinning/CMakeLists.txt | 534120140d1fec822a50168537f52c9325e8ce00ce6ed23d2aebc2fe658d79c5 | 534120140d1fec822a50168537f52c9325e8ce00ce6ed23d2aebc2fe658d79c5 | no |
| port/engine-skinning/skin_pose_cache_v32.cpp | ba56e46c1f8a9be55ba1201d95a74abf9a5cb43f99d4a92f0fc807d3b54157ed | ba56e46c1f8a9be55ba1201d95a74abf9a5cb43f99d4a92f0fc807d3b54157ed | no |
| port/engine-skinning/skinning.cpp | ec5bdda6fbb982856d28f2aa288e16663935628648937971c9026689fec7fd90 | ec5bdda6fbb982856d28f2aa288e16663935628648937971c9026689fec7fd90 | no |
| port/engine-skinning/visual_skin_owner_v6.cpp | 5e7e44c8a152cc705e7a4d11d02505622a745b73b9530d8d3e278ca635ce54e4 | 5e7e44c8a152cc705e7a4d11d02505622a745b73b9530d8d3e278ca635ce54e4 | no |
| port/engine-textures/pvrtc.cpp | dd88a34faff7b5741da77be72a49c0668e75f5aa57c146a3ea7c51817228d575 | dd88a34faff7b5741da77be72a49c0668e75f5aa57c146a3ea7c51817228d575 | no |
| port/engine-textures/textures.cpp | 80264623322fd3262ceeebe1473711ebf5c9546a1f2028d0915c17d5572eb7cb | 80264623322fd3262ceeebe1473711ebf5c9546a1f2028d0915c17d5572eb7cb | no |
| port/engine-ui/CMakeLists.txt | 92f5bd733e492cdd71f72b6d8f3e91bf565e9b453cc60d023ccf43cd8cc9f625 | 92f5bd733e492cdd71f72b6d8f3e91bf565e9b453cc60d023ccf43cd8cc9f625 | no |
| port/engine-ui/vendor/gameswf1714/base/jpeg.cpp | 1d3ba9f2b2b75108f5b24bcee0da823cadf7f1b2471e81cf033229474a686ec3 | 1d3ba9f2b2b75108f5b24bcee0da823cadf7f1b2471e81cf033229474a686ec3 | no |
| port/engine-ui/vendor/gameswf1714/base/png_helper.cpp | f67833f51b1dd669a6408e14d4c3f1f5b7c3125b66f0a67b086b45d84af5ad11 | f67833f51b1dd669a6408e14d4c3f1f5b7c3125b66f0a67b086b45d84af5ad11 | no |
| port/engine-ui/vendor/gameswf1714/base/zlib_adapter.cpp | 38645cc90857938d674925a6dc2020f282d6995c887e8a30ffba69a0cfd0f850 | 38645cc90857938d674925a6dc2020f282d6995c887e8a30ffba69a0cfd0f850 | no |
| port/level-world/CMakeLists.txt | 7f0e6314dd7e2e7afde9fce9a579d5f75e917b2c3f039cabf15f8ef78ec434b4 | 78e9bae6ecf3c3dd4fe31ff187654440246ce92fb4741e85edb55d2d5b285608 | yes |
| port/level-world/base_named_animation_controller_v1.cpp | c5ce416da97b92874843c0f85b3594e96dcb3a6e56132fe2f3809b9a8c4c14a6 | c5ce416da97b92874843c0f85b3594e96dcb3a6e56132fe2f3809b9a8c4c14a6 | no |
| port/level-world/character_authored_resource_v32.cpp | 11076a12633ce6a0e70253fdc0b4ff66fe4ccb233e4c15321d4c3da781db1d27 | 11076a12633ce6a0e70253fdc0b4ff66fe4ccb233e4c15321d4c3da781db1d27 | no |
| port/level-world/character_authored_resource_v6.cpp | 390c7a3044be5189eb2949f7e2754723ef0bbbfc8ceca7aa1e36c360b0ee9c2a | 390c7a3044be5189eb2949f7e2754723ef0bbbfc8ceca7aa1e36c360b0ee9c2a | no |
| port/level-world/retained_gameobject_visual_v1.cpp | 8b5bab7038393ac5b5d438a592bc1d88216b968b7f2295f6f80fe70667246701 | 8b5bab7038393ac5b5d438a592bc1d88216b968b7f2295f6f80fe70667246701 | no |
| port/level-world/scene_preload_owner_v81.cpp | 3e810580afc0159a8cd3cf23171cb31fbacae09f115d7ce53f70fbacf18a4c0d | 3e810580afc0159a8cd3cf23171cb31fbacae09f115d7ce53f70fbacf18a4c0d | no |
| port/level-world/visual_timeline.cpp | 9a6ffdec646fc927ce82199a193df7d1d14ba2f738464e312efead143b4899ca | 9a6ffdec646fc927ce82199a193df7d1d14ba2f738464e312efead143b4899ca | no |
| port/scene-materials/scene.cpp | 5484076583d7fe9bab0085ed7ee14d31b7d43a3b30c08d7f2f936490f0f457a4 | 5484076583d7fe9bab0085ed7ee14d31b7d43a3b30c08d7f2f936490f0f457a4 | no |
