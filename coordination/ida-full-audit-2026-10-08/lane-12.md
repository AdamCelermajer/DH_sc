# Lane 12 - static IDA/source audit

Scope: libDungeonHunter2.so inventory 20527-22393 inclusive (1,867 records). Working-tree source; baseline marker HEAD 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. This worker writes only lane-12.md and lane-12.csv. No source edits, builds, tests, emulator or external messages were performed.

This report provides **complete inventory disposition with bounded deep comparisons**, and explicitly **does not establish semantic parity for every nontrivial record**. The CSV review_depth column exposes this limit. The large unclear count must not be converted into either implemented or missing claims. Successful decompilation means a body was exported, not that its semantics are correct; two GL functions required assembly because pseudocode lost output-parameter branches.

## Coverage and evidence rules

- All 1,867 assigned pseudocode files were read and structurally classified; all 1,867 assembly blocks were indexed. Every row records code/body hashes, original names, source location or scoped unresolved reason, comparison, confidence and runtime acceptance.
- All 271 named adjustment thunks have verified assembly instruction shapes: 220 LDR/LDR/ADD/B and 51 SUB/B. Five local forwarding wrappers are distinguished, including JPEG allocator wrappers and glitch_free.
- The CSV retains 6,613 outgoing code edges resolved against the complete function inventory and 1,668 incoming non-flow references. Raw ARM32 little-endian vtable words supplied references for 1,020 assigned records. Direct-xref absence does not exclude virtual dispatch, exported-symbol use or asset/callback invocation.
- Clone status requires identical machine-byte SHA256 and the same demangled identity. Compiler [clone] labels alone are not enough. Each duplicate remains listed and references its canonical record; it inherits no source parity claim.
- Source candidates were read in the current port and followed through source calls, actual includes and build configuration. Test/report/reference/snapshot copies and similar names are not implementation evidence. No missing classification derives from a text-search miss.
- Confidence measures the static disposition, including concrete disconnection or uncertainty. Every integrated_runtime_acceptance cell is not_run; no acceptance was established for these exact current source hashes.

Comparison counts:

| Status | Rows |
|---|---:|
| clone | 22 |
| disconnected | 71 |
| import/thunk | 276 |
| matched | 1 |
| partial | 31 |
| unclear | 1466 |

Body categories:

| Category | Rows |
|---|---:|
| GL-output-parameter-pseudocode-artifact | 2 |
| abi-adjusting-thunk | 271 |
| attribute-serialization | 40 |
| bundled-JPEG-body | 69 |
| bundled-zlib-body | 35 |
| compiler-container-or-reference-helper | 93 |
| destructor-or-deleting-destructor | 205 |
| field-accessor-or-small-dispatch | 148 |
| leaf-accessor-or-forwarder | 420 |
| literal-no-op | 62 |
| local-forwarding-wrapper | 5 |
| static-initializer | 72 |
| subsystem-behavior | 445 |

Review depth:

| Depth | Rows |
|---|---:|
| assembly-adapter | 276 |
| deep-body-and-source | 37 |
| deep-original-body-source-unresolved | 58 |
| structural-body-xrefs-source-unresolved | 1496 |

## Compared paths and findings

### F12-01 - optional shader configuration fails in a live source branch

IDA #21546, 0x6e0a3c, ICodeShaderManager::initAdditionalConfig: config length begins at -1 (#21549). Missing file reaches assembly0x6e0b2c, checks warnOnce, logs at0x6e0b50, clears warnOnce at0x6e0b54 and returns through common file-system release0x6e0b20. It neither assigns nonnegative length nor fails shader creation. Present files replace owned text and change caret to newline.

Source port/engine-textures/texture_driver_fields_v1.cpp:4 reconstructs the present-file branch, but its missing-file branch requires ShaderConfigFileServicesV1::trace_missing and returns false when absent. Current live renderer_authored_effect_scene_v5.inc:136-141, included by model_renderer.cpp:2312, supplies only files.read and throws when initialize fails. renderer_native_batch_shader_v113.inc:42 likewise supplies read only. A pack lacking glsl.config therefore follows a different static result. Shipping pack contents and runtime triggering were not established. Status partial; high confidence in the stated branch mismatch.

### F12-02 - GL decompilation artifacts hide successful reflection

IDA #21466, 0x6de9f8, CGLSLShader::linkProgram has decompile status success, but pseudocode shows only failure and return0. Assembly queries link status into a real stack output at0x6dea20, loads it at0x6dea24 and branches to success at0x6dea2c. Success enumerates attributes at0x6debe4-0x6dec78 and uniforms at0x6dece8-0x6dee20, maps type/semantic/sub-ID/location, owns shared strings, partitions global parameters and returns1.

#21479, 0x6df3c0, has a related output-parameter simplification: assembly checks actual compile status, persists compiled byte+0x34 on success and returns0 on an already-compiled second request. Source native_batch_program_v113.cpp:28-45 compiles, links and reflects actual GL output; shader_reflection_v4.cpp:275/283 maps and partitions uniforms. Those are implemented portions. Complete original code collection, reference identities and recreation contract remain unverified. Both rows are partial; neither is labeled a failed IDA export or an original routine that always fails.

### F12-03 - original shader aliases and qualified attributes exceed current handling

IDA #21352, 0x6dbb50, guessShaderVertexAttribute strips text through the first dot, lowercases and recognizes aliases including pos, vertices, normals, tangent/binormal families, diffuse/secondary colors, coord/texcoord and skin weight/index families. Original #21466 calls it at0x6dec08.

Current native_batch_program_v113.cpp:9-16 lowercases but has a narrower set and no dot stripping. At line44, initialize requires a recognized slot and throws on -1. The actual batch include uses these slots through renderer_native_batch_shader_v113.inc:18. Input.Position and pos are concrete static examples recognized by IDA and rejected by this source route. The fixed ProfileCOMMON effect renderer intentionally binds only Position/Color0/TexCoord0. Status partial. No shipping shader occurrence of a rejected alias was established.

### F12-04 - absolute texture sampling exists; relative/weighted closure is unresolved

IDA #21658, 0x6e3d78 copies five-float defaults, resets each channel cursor, carries interpolation AND across channels, routes types87-91 to offsetU/offsetV/rotation/scaleU/scaleV and leaves the public cursor unused. fx_texture_animation_v1.cpp:4-29 preserves this supported uncompressed absolute-track domain. authored_fx_mesh_graph_v32.cpp:81 routes the sample into dh2_fx_texture_matrix_v1.

Original #21648, 0x6e38c4 fixes center(.5,.5), converts degrees/180 using0x40490fe9, calls #21647 and writes a selected material parameter. character_fx_kernels_v1.cpp:23 preserves fixed-center term order, sign bits, layout and identity hint. Generic #21647 accepts arbitrary centers; the source helper represents the fixed-center caller domain.

Weighted/additive #21639/#21640/#21649 mutate all five input floats in place, sum from identity with scales1 and apply the result. Assembly0x6e3478-0x6e34d4 confirms input writes, so an ordinary pure weighted sum would not be adequate evidence. Relative helpers #21653/#21654 and accessor#21655 subtract a reference-time sample. dh2_fx_texture_delta_v1 exists at character_fx_kernels_v1.cpp:22; scoped production C++/hpp/inc search found only declaration and definition. The inspected texture_sample_v1 route calls absolute interpolation. The candidate relative scalar rows are disconnected; full relative/weighted ownership remains unresolved. No claim is made that all alternate routes are absent.

### F12-05 - bundled JPEG is present but the current UI provider is disabled

IDA #21804-#21876 contain real coefficient controllers, color conversion, Huffman machinery, DCT/IDCT and allocator adapters. Examples #21836, 0x6f0da4, and #21852, 0x6f2c60, are called by original jinit_master_decompress#19763. #21852 installs start_pass_huff_decoder and decode_mcu; it is not an empty stub.

port/engine-ui/CMakeLists.txt:18 and :30 define TU_CONFIG_LINK_TO_JPEGLIB=0 for headers/core. gameswf_edit_text_overlay_v1.cmake selects the active edit-text overlay. Its gameswf_impl.cpp:627 JPEG2/tag21 branch creates an empty bitmap under the disabled provider. Vendor base/jpeg.cpp:514-535 returns NULL from input/output creation in that configuration. This is disconnected JPEG provider behavior rather than a missing claim from a search miss. Original allocator shims#21873-#21876 are local forwarders. Other predecoded/native-image routes do not establish JPEG tag-decoding parity. Shipping JPEG-tag occurrence and runtime triggering are unverified.

### Arithmetic match and bounded source adapters

#21438, 0x6dd9d0, projection fixup was compared with gameplay_camera_gpu_projection_v12.cpp:5 and original tail#14969, 0x5a8f88. Orthographic/perspective depth remap, NaN branch choice, identity-hint reset, bitwise Y sign flips, render-target-count guard and orientation order agree. The live model_renderer.cpp:3080 call remains present; its caller provenance was refreshed in the bounded post-pause recheck below. Matched concerns supplied-field arithmetic, not all driver producer identities or gameplay acceptance. model_renderer.cpp changed during review and is flagged in the CSV and hash table.

#21436 extension parsing maps the original space-terminated-token rule in blood_texture_image_v1.cpp:5, but only texture-consumed extensions were compared. #21487/#21497 shader construction maps eight copied chunks in shader_sources.cpp:24 into live GL compilation. Neither pure chunk parity nor a texture subset proves complete original collection/removal/recreation or bitmap behavior.

#20850 PS SParticle billboard corners map the documented contextbyte20==0 branch including spin quaternion and corner order. particle_billboard_v1.hpp:16 expressly limits that branch. Velocity orientation and #20851 GNPS pivot/spin layout remain unverified. #20852 startup sets numerous global templates; only SParticle UV order is directly reconstructed by the cited helper.

## Subsystem dispositions and unresolved work

- Legacy GUI#20528-#20606 includes parent event delivery, menus, serialization, wchar cursor/selection, key/mouse processing, wrapping and font ownership. Current GameSWF edit-text is a different object lineage. No same-owner equivalence was established; nontrivial methods remain unclear, not missing.
- Files/streams#20607-#20689 received structural coverage and deeper original-body review for limiting reads/async seeks, memory files, ZIP writer and stream serialization. Original CZipWriter#20667 writes stored method0 with CRC/central-directory ownership and is called by CBatchMesh::save#14190. Current ZipAssetPackV1 is a reader, not writer evidence. Stream paths include byte swapping, offsets/strides and driver allocation; a complete source counterpart remains unresolved.
- #20709 CColladaBinaryFileLoader::createMesh calls CResFileManager#18989, constructs scene#17369 and animator#16958, attaches/publishes the root, then returns null. Decoded vtable slot0x9878c8 proves virtual ownership. A null return must not erase these side effects. Factory/mesh/buffer/cache identity, rollback and teardown remain unclear.
- #20905 uses legacy68-byte scene particles with virtual emitter/affector dispatch. Box#22046, cylinder#22087, point#22201, ring#22238 and sphere#22321 use Randomizer::rand#16637, integer modulo/fmod placement, times and color interpolation. Affectors mutate that lineage. Current authored PS uses100-byte seeds and retained seeded models, so shared particle names are not matches. Animated-mesh#21956 and mesh#22165 emitt literally return0; they are recorded as zero bodies rather than invented missing emitters. Vtable slots0x98c8bc/0x98dcfc/0x98d024 show invocation can be virtual despite absent direct callers.
- Collision#20926/#20935/#20936 includes swept sphere triangle/edge roots, recursion depth<=5, ellipsoid scaling and optional gravity. Gameplay physics/navigation does not prove this selector API. FPS/Maya/collision-response and delete/fly/spline/rotation/texture animators, with serialization, remain unresolved.
- Procedural skybox/terrain/text/mesh bodies exist. Current GameplaySkyboxV24 is a loaded-mesh wrapper and does not prove six-texture CSkyBoxSceneNode#21138. Heightmap/LOD/normal/patch-neighbor and native GUI text paths remain unresolved.
- Extended generic animation vtables route cursor-aware keys/application through typed interpreters. #21598 has slot0x98b230 and marks grouped position/rotation/scale entries. Current binding represents a subset. Generic scalar/vector/visibility/weight virtual ownership is not closed; OR/add versus AND/blend and weighted input mutation must not be inferred from ordinary interpolation.
- Collada camera/corona and shadow volumes remain unclear. #22387 slot0x98f124 leads to dynamic-light iteration and #22386 buffer/silhouette creation. Ordinary projected character shadows or GL guards are not source substitutes.
- Bundled zlib#21769-#21803 has real bodies. ZIP source line137 invokes external zlib; native CMakeLists.txt:144 links z. This is a provider lead, not internal version/compressor/checksum-combine or byte-identical output verification. These rows remain unclear and are not mislabeled imports.
- All resolved direct cross-lane callee IDs/addresses and supplied vtable owners are retained in CSV. Indirect callback/asset reachability and complete source virtual identity remain unresolved. Inventory coverage must not be treated as whole-lane semantic closure.

No missing implementation count is inferred from unresolved rows. Runtime and integration work remain untouched.

## Source hashes and concurrent changes

The table records current working-tree hashes, not git blobs. Hash rechecks covered2,680 non-snapshot port code/config files. Two files changed during review: model_renderer.cpp (cited projection/include caller; current call re-read and current hash used) and unrelated tools/menu_options_audit.cpp (not a parity comparator). Concurrent source changes prevent a single immutable-tree claim. Other cited files were stable. Supplemental vendor/cmake hashes were captured after their first reads and rechecked before export.

| File | First recorded SHA256 | Export SHA256 | Changed |
|---|---|---|---|
| port/engine-textures/texture_driver_fields_v1.cpp | 2209fd3f3fae3c170e56e6aae2318c107f74ab520d4cef922b4d8a4c2f720f51 | 2209fd3f3fae3c170e56e6aae2318c107f74ab520d4cef922b4d8a4c2f720f51 | no |
| port/engine-textures/texture_driver_fields_v1.hpp | 943d8971d69f4a900ddd79fecf94c97137ecadf81925881effa5309d42c9d143 | 943d8971d69f4a900ddd79fecf94c97137ecadf81925881effa5309d42c9d143 | no |
| port/engine-textures/blood_texture_image_v1.cpp | eb26bbb9fef7880265eb6eedc6521187a40a2fe6dbe3874e3f41001e44ac12fb | eb26bbb9fef7880265eb6eedc6521187a40a2fe6dbe3874e3f41001e44ac12fb | no |
| port/scene-materials/shader_sources.cpp | f05874e30bba3a3b1c8cec643e5fda6bcba4f31e9b935eae8e94c9872b41cde1 | f05874e30bba3a3b1c8cec643e5fda6bcba4f31e9b935eae8e94c9872b41cde1 | no |
| port/scene-materials/shader_reflection_v4.cpp | 027ab73c484cbf598cd95bceadbddaa62c6e4c54fe9e85e7b03129e0331f048b | 027ab73c484cbf598cd95bceadbddaa62c6e4c54fe9e85e7b03129e0331f048b | no |
| port/scene-materials/shader_program_collection_v4.cpp | 5beaab919c02a8bbc54baa77240a45acf3ad1b1b7ca4a7062ff1ef0c12dca431 | 5beaab919c02a8bbc54baa77240a45acf3ad1b1b7ca4a7062ff1ef0c12dca431 | no |
| port/android-native/app/src/main/cpp/native_batch_program_v113.cpp | 10b946d127afbf419fe06b784036d4021edba01a06f0edfe68d332ff5e274a4d | 10b946d127afbf419fe06b784036d4021edba01a06f0edfe68d332ff5e274a4d | no |
| port/android-native/app/src/main/cpp/renderer_native_batch_shader_v113.inc | db5594805bb526b7d23b8f39ba6a41c2289d5d599fb502d25eb8b9e0ed6c538a | db5594805bb526b7d23b8f39ba6a41c2289d5d599fb502d25eb8b9e0ed6c538a | no |
| port/android-native/app/src/main/cpp/renderer_authored_effect_scene_v5.inc | f1c22754d6a82ceb3bf85cbc4b9f05ce8cfcb64f00bd82fb322f993a0ac32a38 | f1c22754d6a82ceb3bf85cbc4b9f05ce8cfcb64f00bd82fb322f993a0ac32a38 | no |
| port/android-native/app/src/main/cpp/renderer_effect_program_connection_v4.inc | 5d72ceb4002fd8b7a791ba2ba3ffd0262bebb66a1ad76b614265d0b81efcea58 | 5d72ceb4002fd8b7a791ba2ba3ffd0262bebb66a1ad76b614265d0b81efcea58 | no |
| port/android-native/app/src/main/cpp/renderer_effect_draw_v4.inc | 0dfc459a90f238dc505bed1706688008d3a1824922b6295c7a3fe0cfd182e317 | 0dfc459a90f238dc505bed1706688008d3a1824922b6295c7a3fe0cfd182e317 | no |
| port/android-native/app/src/main/cpp/authored_shader_program.cpp | f7c0cc40d71f5dd00c802bf83b36afe1378d7c576c43e73a86dfb5da04d30c58 | f7c0cc40d71f5dd00c802bf83b36afe1378d7c576c43e73a86dfb5da04d30c58 | no |
| port/android-native/app/src/main/cpp/model_renderer.cpp | 68df536883d56f0cd74f5b47052daafd07491faf5a75eec901790fd26afbe015 | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca | yes |
| port/android-native/app/src/main/cpp/CMakeLists.txt | ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45 | ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45 | no |
| port/engine-animation/particle_billboard_v1.cpp | 665a84f44d7a8692d585210151107d63aba702c354553ba510f43e8d6f3f8054 | 665a84f44d7a8692d585210151107d63aba702c354553ba510f43e8d6f3f8054 | no |
| port/engine-animation/particle_billboard_v1.hpp | f80a3d4557478353a308b9d16f480bf04255059a363dad3b8c990ed1f82937a6 | f80a3d4557478353a308b9d16f480bf04255059a363dad3b8c990ed1f82937a6 | no |
| port/engine-animation/particle_resource_init_v32.cpp | d1689290a55ac75e8b294e3e2410850391e39e8d2795dc128cb0a6cb8aad7b54 | d1689290a55ac75e8b294e3e2410850391e39e8d2795dc128cb0a6cb8aad7b54 | no |
| port/level-world/character_authored_resource_v32.cpp | 11076a12633ce6a0e70253fdc0b4ff66fe4ccb233e4c15321d4c3da781db1d27 | 11076a12633ce6a0e70253fdc0b4ff66fe4ccb233e4c15321d4c3da781db1d27 | no |
| port/level-world/character_fx_kernels_v1.cpp | 74292efbf3136dcd7aca614e9879107d1273f7b79821d67bd71065cbf34f5e6d | 74292efbf3136dcd7aca614e9879107d1273f7b79821d67bd71065cbf34f5e6d | no |
| port/level-world/fx_texture_animation_v1.cpp | 4ddcc9d2e384af51d8827643cd7ad26971b221726b1a93ebc402fb56812db9fc | 4ddcc9d2e384af51d8827643cd7ad26971b221726b1a93ebc402fb56812db9fc | no |
| port/level-world/authored_fx_mesh_graph_v32.cpp | 6a7ccc060e29ad4944da1a495ede9faae976c22820a40d13b309c815b231e6b4 | 6a7ccc060e29ad4944da1a495ede9faae976c22820a40d13b309c815b231e6b4 | no |
| port/engine-animation/animation.cpp | 968352af9ffb1f1aacba6d6efb4f821beef933b96774abaff8077efea18e26f8 | 968352af9ffb1f1aacba6d6efb4f821beef933b96774abaff8077efea18e26f8 | no |
| port/level-world/gameplay_camera_gpu_projection_v12.cpp | e51aa0771d7eb1066de9bc551657d6de5cc5e7757494d345735d58c652ca0e41 | e51aa0771d7eb1066de9bc551657d6de5cc5e7757494d345735d58c652ca0e41 | no |
| port/level-world/gameplay_skybox_v24.cpp | 3f4ee50209f70892f38c324b1584d230a6eb43477ad378d455ad7e5a37a3ed79 | 3f4ee50209f70892f38c324b1584d230a6eb43477ad378d455ad7e5a37a3ed79 | no |
| port/engine-ui/CMakeLists.txt | 92f5bd733e492cdd71f72b6d8f3e91bf565e9b453cc60d023ccf43cd8cc9f625 | 92f5bd733e492cdd71f72b6d8f3e91bf565e9b453cc60d023ccf43cd8cc9f625 | no |
| port/engine-ui/gameswf_edit_text_overlay_v1.cmake | 406284f2020472b60cbc4e2fe9eaffc3725cb5b2f4b95bd61e9321bd3cc1df23 | 406284f2020472b60cbc4e2fe9eaffc3725cb5b2f4b95bd61e9321bd3cc1df23 | no |
| port/engine-ui/overlays/edit-text-v1/gameswf_impl.cpp | 5c1e95229a4c3025c090f0fcf8ce4e76df3ba95c220a99069f9902f7d13e3e48 | 5c1e95229a4c3025c090f0fcf8ce4e76df3ba95c220a99069f9902f7d13e3e48 | no |
| port/engine-ui/vendor/gameswf1714/base/jpeg.cpp | 1d3ba9f2b2b75108f5b24bcee0da823cadf7f1b2471e81cf033229474a686ec3 | 1d3ba9f2b2b75108f5b24bcee0da823cadf7f1b2471e81cf033229474a686ec3 | no |
| port/asset-payloads/zip_asset_pack_v1.cpp | 38322384ea3114f1ea3aad0ee0211bb6f583dfaf4a76ae642645fe208a9b17df | 38322384ea3114f1ea3aad0ee0211bb6f583dfaf4a76ae642645fe208a9b17df | no |
| port/android-native/tools/menu_options_audit.cpp | e7c8ff53ab486db6fd3289008952dc3ab2ed0d979f7e6b40a7aec43ebb779877 | 2b12500b72acf37502379e55130e77c5c00553bee6eacc08036c62e15a681347 | yes |

CSV coverage: 1,867 rows; IDs20527-22393; missing assigned IDs0; duplicate IDs0; failed exported decompilations0; runtime not_run1,867.

CSV SHA256: 13cbf559a0c786b319949bbb985dc5fe0ea5f9989a0cb95a4b1d6c11cc50b971.


The initial report-integrity check observed another model_renderer.cpp change. The then-cited caller read was captured with SHA256 c35716faf0f364342244e6f0b974717e7fa39713c6c086c1ef9ae23fbbb9a45b and call line3065. This is an explicitly changing source file; the report makes no immutable-tree claim. The standalone projection kernel comparator remains stable.

## Bounded post-pause recheck - 2026-10-08 15:14 UTC

Scope: the #21438 projection comparator and renderer provenance requested by post-pause-source-impact.md. The standalone kernel was reread against original #21438 at0x6dd9d0 and orientation tail#14969 at0x5a8f88. Its SHA256 is unchanged: e51aa0771d7eb1066de9bc551657d6de5cc5e7757494d345735d58c652ca0e41. Depth conversion, NaN branch selection, identity hint reset, bitwise Y sign flip, target-count guard and orientation order remain the previously compared arithmetic. The matched disposition is retained for this standalone supplied-field kernel.

The current renderer call is model_renderer.cpp:3080, with renderer SHA256 de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca. Its surrounding code still copies view.projection and supplies GpuProjectionFieldsV12{0,0,0}, then copies the resulting matrix into submitted_projection_v113. This confirms the existing live call; it does not prove original driver-owner identity or exercise the nonzero orientation/flip/target-count branches. Renderer hash drift alone does not establish an arithmetic regression or disconnection.

The declaration chain remains model_renderer.cpp:287 -> gameplay_camera_application_v23.hpp:5 -> gameplay_camera_gpu_projection_v12.hpp; level-world/CMakeLists.txt:86 still lists the kernel source. The related renderer_authored_effect_scene_v5.inc and renderer_native_batch_shader_v113.inc includes remain at model_renderer.cpp:2312/2313 with unchanged include-file hashes. These include observations refresh provenance only; the prior shader findings and incomplete owner closure are not upgraded.

| Rechecked file | SHA256 | Result |
|---|---|---|
| port/android-native/app/src/main/cpp/model_renderer.cpp | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca | caller snapshot refreshed |
| port/level-world/gameplay_camera_gpu_projection_v12.cpp | e51aa0771d7eb1066de9bc551657d6de5cc5e7757494d345735d58c652ca0e41 | read for bounded comparator/include chain |
| port/level-world/gameplay_camera_gpu_projection_v12.hpp | e3b69e47c1801a235effb63a977578ec2c8d62209a7b6412a8f394e44107c3af | read for bounded comparator/include chain |
| port/level-world/gameplay_camera_application_v23.hpp | d68dd9a241a37b5ce4ce539604ed0a17fc0339b1b7e2eb888ce95799fe25c698 | read for bounded comparator/include chain |
| port/level-world/CMakeLists.txt | 78e9bae6ecf3c3dd4fe31ff187654440246ce92fb4741e85edb55d2d5b285608 | read for bounded comparator/include chain |
| port/android-native/app/src/main/cpp/renderer_authored_effect_scene_v5.inc | f1c22754d6a82ceb3bf85cbc4b9f05ce8cfcb64f00bd82fb322f993a0ac32a38 | read for bounded comparator/include chain |
| port/android-native/app/src/main/cpp/renderer_native_batch_shader_v113.inc | db5594805bb526b7d23b8f39ba6a41c2289d5d599fb502d25eb8b9e0ed6c538a | read for bounded comparator/include chain |

Only CSV row21438 and this lane Markdown were updated. Coverage remains1,867 rows with no missing/duplicate assigned IDs; all dispositions and all not_run runtime cells are unchanged. No game source, builds, tests or emulator were changed/run.
