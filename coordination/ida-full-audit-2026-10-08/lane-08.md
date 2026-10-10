# Lane 08: Glitch GUI, IO, scene and video static audit

This report accounts for **1,866 / 1,866 assigned records**, indices **13063–14928**, addresses **0x54f78c–0x5a4374** in libDungeonHunter2.so. All 1,866 have success pseudocode; there are **zero failed/unsupported decompilations** and zero external-import records in this range. There are **1,655 distinct exported code-byte hashes**, **126 ABI adjustor thunks**, and **61 symbols labelled compiler clones**. Every assigned ID has its own CSV row; no ID is missing or duplicated.

The inventory and body disposition pass is complete. **The semantic parity audit is not complete:** 1288 rows remain unclear, and selected source comparisons do not establish every generic virtual owner, branch, lifecycle or asset consumer. No unresolved name was classified missing. This lane must not be used to call the full-game audit or gameplay acceptance complete.

The working tree was read with baseline marker HEAD **75a7c2fe3261403e841ffbeb46734e9e4e84e75c**, following the README's existing dirty-tree warning. No source edits, builds, implementation tests, emulator runs, installs or external messages were made. Only lane-08.csv and this file were written. **Runtime acceptance is not_run for every row.**

## Accounting and evidence method

| Comparison status | Rows |
|---|---:|
| matched | 7 |
| partial | 41 |
| disconnected | 12 |
| missing | 0 |
| unclear | 1288 |
| import/thunk | 126 |
| clone | 98 |
| failed-body | 0 |
| out-of-scope | 294 |
| **Total** | **1866** |

| Inventory band | Rows | Content |
|---|---:|---|
| 13063–13409 | 347 | GUI sprite banks, static text, tabs/table/toolbar, TrueType glyph/font/face/library, windows |
| 13410–13781 | 372 | Quaternion primitive, typed CAttributes conversions, serialization and ownership |
| 13782–14113 | 332 | Filesystem, memory/read/write files, PAK, XML readers/writer, ZIP and texture string utilities |
| 14114–14345 | 232 | Batch mesh/node, billboard, cameras/frustum and empty scene nodes |
| 14346–14694 | 349 | Lights, mesh nodes, octree/triangle selectors, SceneManager, ISceneNode fields/ownership/transforms |
| 14695–14928 | 234 | Animator/shadow/animated mesh, mesh operations, 2D driver, lights, primitive/vertex/buffer/batch helpers |

All **2,101,078 bytes of assigned pseudocode** were loaded, split into complete bodies, hashed and structurally inspected for statements, branches/loops, calls and virtual dispatch. The CSV gives every body file/hash, statement evidence and assembly instruction count. **632 bodies exceed 30 pseudocode lines**; these were not all proved against complete source owners. **67 records** have a targeted source-body or explicit related-owner comparison. Automated body indexing and token hits are not deep semantic parity evidence.

The scoped source search read **2,774 primary port C/C++/header/include/CMake files**, excluding generated reports, tests, historical references, vendor snapshots and build directories. Each row records its class/function token and candidate-hit count. Alternate source APIs were read, including TinyXML capture, ZIP packs, SWF fonts and retained scene/batch/light ownership. Name or address absence was never used as proof of absent behavior. Dynamically supplied providers and external consumers remain unresolved.

The full native xrefs export was scanned for direct calls and tails to named entries; intra-function conditional branches were excluded. Raw vtable words from all **1,628 vtable/RTTI records** were also decoded because the xrefs export is emitted from function items and omits many static-data/vtable incoming references. A zero direct-caller count does not mean a virtual/exported record is unused. All 126 labelled thunks have an exported direct/tail target.

**98 rows** are grouped only where both the entire code-byte hash and complete extracted pseudocode-body hash match a listed lane representative. Every duplicate keeps its own ID/name/source uncertainty/xrefs. Equal bytes alone cannot establish equivalence when PC-relative globals differ. Compiler clone labels with different bodies remain ordinary/helper records with compiler_clone_label=yes. Empty base virtual [14114 / 0x578920](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0057/00578920.c) is distinct from CBatchMeshExt<GameObject>::loadSegmentExtraData at **0x50cf00**.

## Findings requiring integration review

### F08-01 — Camera caller omits original parallel-up fallback

**Partial; high static confidence; runtime not_run.** Unverified path: live procedural/menu or authored camera view → look-at with viewing direction parallel/nearly parallel to up.

[14332 / 0x583280](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0058/00583280.c) normalizes direction and up; if abs(dot) lies within about 1e-6 of 1, it adds **0.5 to up.x** before calling [14331 / 0x582e64](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0058/00582e64.c), then refreshes frustum state. [port/level-world/gameplay_camera_runtime_v11.cpp:86](../../port/level-world/gameplay_camera_runtime_v11.cpp) and [port/level-world/gameplay_camera_factory_v16.cpp:52](../../port/level-world/gameplay_camera_factory_v16.cpp) call [port/level-world/gameplay_camera_matrix_v8.cpp:7](../../port/level-world/gameplay_camera_matrix_v8.cpp) with unchanged up. Exactly parallel inputs produce a zero side vector and degenerate source view basis. The primitive itself corresponds; its local match does not cover the caller guard. Actual supplied-asset runtime impact was not exercised.

### F08-02 — Light parent visibility leaves effective flag stale

**Partial; high static confidence; runtime not_run.** Unverified path: Scene parent visibility callback → active LightPoint node inherited visibility handler.

[14607 / 0x596e34](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0059/00596e34.c) writes parent121, recomputes flags11c bit1 as **local120 && parent121**, and propagates changed effective visibility to children. Concrete CLightSceneNode vtable at **0x975b88**, slot **+0x108**, points to **0x596e34**, proving the inherited target.

[port/android-native/app/src/main/cpp/source_campaign_light_environment_v113.cpp:117](../../port/android-native/app/src/main/cpp/source_campaign_light_environment_v113.cpp) forwards the actual retained root callback to [port/level-world/native_scene_lights_v113.hpp:50](../../port/level-world/native_scene_lights_v113.hpp). This stores only parent121_; flags11c_ is not recomputed. A visible source light can remain effectively visible after false parent notification. Procedural-camera, Module and batch alternatives do recompute their flags; this finding concerns the light owner. Rendering/registration consequences remain unverified.

### F08-03 — Procedural camera position setter differs in flags and cache timing

**Partial; high static confidence; runtime not_run.** Unverified path: procedural/menu set-position → later flag or cached absolute-transform observation.

[14627 / 0x59712c](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0059/0059712c.c) copies relative XYZ, ORs flags11c with **8**, and leaves the world cache lazy. [port/level-world/gameplay_camera_factory_v16.cpp:48](../../port/level-world/gameplay_camera_factory_v16.cpp) ORs **0x40** and immediately calls scene::update_world. [port/level-world/module_static_scene_v2.cpp:151](../../port/level-world/module_static_scene_v2.cpp) uses 8 but also eagerly recomputes its selected root. Other Character dispatch has the expected bit8 store; this is a concrete variant/order difference, not absence of all position behavior.

### F08-04 — MemoryReader has static correspondence without established live dispatch

**Disconnected; bounded source integration uncertainty; runtime not_run.** Unverified path: original CFileSystem memory-file factory / IReadFile virtual completion → current gameplay consumer.

IDA **13842, 13843, 13845–13852, 13856** correspond to [port/engine-resources/resources.cpp:26](../../port/engine-resources/resources.cpp): accessors, signed upper-bound-only seek, synchronous callback(amount, amount==0, this, user), and failed-seek reading at the old cursor. Source guards invalid/null/negative-cursor copies the original memcpy attempts. Scoped non-test searches found definitions/export wrappers and LimitReader composition, without a live original memory-file factory/virtual dispatch owner in active Android source. This is an implementation with unestablished active linkage; possible external consumers are not ruled out. Earlier component evidence was not promoted to integrated runtime acceptance.

### F08-05 — Generic CFileSystem open maps only to disconnected filename projection

**Disconnected; runtime not_run.** Unverified path: FileSystemWin32::_FileHandle / CReadFile / CWriteFile → CFileSystem.open → fopen / CFile ownership.

[13831 / 0x56dc40](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0056/0056dc40.c) has direct callers **1999 / 0x34fc18**, **2005 / 0x35003c**, **13910 / 0x5704cc**, **13926 / 0x57095c**, and existFile **13832**. [port/level-loader/level_root_cfs_filename_v52.cpp:4](../../port/level-loader/level_root_cfs_filename_v52.cpp) preserves ./ stripping, working-directory substring testing followed by advancement **from the start**, mapping/slashes and colon absolute test. It has no non-test caller in the scoped search and omits physical fopen/CFile creation/obfuscation ownership. Active resource routing uses [port/android-native/app/src/main/cpp/source_campaign_file_io_v65.hpp:48](../../port/android-native/app/src/main/cpp/source_campaign_file_io_v65.hpp) and mounted ZIP. An alternate cache route does not establish generic open parity.

### F08-06 — ZIP parser has a different local-header/central-directory contract

**Partial; high static confidence; runtime not_run.** Unverified path: original CZipReader construction on local headers without a valid complete central directory.

[14101 / 0x577db4](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0057/00577db4.c) sequentially scans local headers and stores payload positions; constructor callers are **14102, 14103, 14106, 14107**. [port/asset-payloads/zip_asset_pack_v1.cpp:48](../../port/asset-payloads/zip_asset_pack_v1.cpp) requires EOCD/central directory, validates bounded names/flags/methods, later matches local headers and verifies CRC. Cache support is present, but rejection/partial-stream behavior differs. This does not prove that full-cache gameplay fails. The key preparation in [14109 / 0x578520](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0057/00578520.c) has a live counterpart at [port/scene-materials/swf_texture.cpp:19](../../port/scene-materials/swf_texture.cpp) through OriginalUiAssets; generic configurable archive/index ownership is partial.

### F08-07 — Generic compile and alternate projection domains are narrower in source

**Partial; runtime not_run.** Unverified compilation path: booltrue, nonzero point, null destination, split-choice callback or reentry.

[14558 / 0x58fd10](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0058/0058fd10.c) installs driver callbacks, initializes buffers, collects nodes and runs before/draw/after/finalize/setup/release. [14560 / 0x58ff10](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0058/0058ff10.c) stores requested point/driver byte then delegates. [port/android-native/app/src/main/cpp/source_campaign_batch_compilation_v111.cpp:40](../../port/android-native/app/src/main/cpp/source_campaign_batch_compilation_v111.cpp) requires existing same-owner mesh/node, boolfalse, zero point, nullptr split callback and no reentry; [port/level-world/native_batch_compiler_v111.cpp:109](../../port/level-world/native_batch_compiler_v111.cpp) supplies retained triangle/material/segment/GPU compilation. IDA **14183 / 0x57bc78** transparent partition corresponds locally at source line197; generic compile parity remains unestablished.

Unverified camera path: orthographic or far-infinity mode → rebuild. IDA **14330 / 0x582d90**, **14334 / 0x58359c**, **14335 / 0x58364c** contain these branches. Source retains authored kind but live camera views always build finite perspective. Actual asset reachability of alternate modes was not established.

## Assembly and cross-lane evidence

- **Time sentinel:** [14514 / 0x58b9f0](../../.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0058/0058b9f0.c) displays -123460 in pseudocode. Assembly **0x58b9f8–0x58ba00** forms **0xc7f12000 = -123456.0f**, correctly used at [port/level-world/gameobject_scene_root_registry_v1.cpp:157](../../port/level-world/gameobject_scene_root_registry_v1.cpp). Store-before-traversal and optimized virtual18 versus recursive root14 distinction are present; source guards unsupported time/reentry/failure domains. The decompiler decimal was not reported as a defect.
- **Absolute transformation:** **14657 / 0x597c60** tests parent20/self5e, calls absolute38/relative40, multiplies, ORs120 and clears50. Assembly **0x597d34** copies **65 bytes**, including identity byte. Source 16-float graph caches need separate identity/flags. Relevant callers beyond the lane are **2159 / 0x35a940**, **2160 / 0x35a974**, **2253 / 0x35c27c**, **2271 / 0x35c51c**, **16704 / 0x60cf88**. Module/retained selected flags were traced; generic recursion/cache ownership is partial.
- **Light C1:** **14357 / 0x5840f0** is used by LightBase::InitPost **6077 / 0x40b0c0**, GSViewer **3220 / 0x3872f8**, DefaultSceneNodeFactory **20738 / 0x6b9f30**. CLight **14822 / 0x59fe08**, allocate **14823 / 0x59feb8** map to retained color/attenuation/radius/cutoff/ref fields. Clone/detached matrix-pool ownership is unresolved.
- **Visibility:** **14442 / 0x5890a8** is only byte289=1 and matches the retained registry cell. Cross-lane **2281 / 0x35d168** RootSceneNode::onAnimate and **7895 / 0x470e18** VisualObject::SetModularSkin route notification. F08-02 remains separate.
- **Teardown:** **14467 / 0x589888** is called by game SceneManager.clear **2099 / 0x354468**. removeAll **14673 / 0x5987e8** is used by ISceneNode and collada root destructors **19033 / 0x65c074**, **19035 / 0x65c260**. Retained parent-drop receipts and native D1 were read; generic shadow-target and child migration/reentry parity remains partial.
- **GUI/font:** StaticText.breakText **13115 / 0x551204** is reached by update-position/wordwrap/font/draw/text **13116–13120**. UTF decoder **13339 / 0x55bd58** is called by iterate **13340 / 0x55be04**. TT glyph lookup/cache/draw uses FreeType/per-glyph textures. Related current SWF resolver/geometry/text owners were read and not declared identical Glitch GUI implementations.

The main CSV xref display is bounded and marks long lists, while the additional all_exported_direct_tail_callers, all_exported_direct_tail_callees and all_exported_vtable_slots fields enumerate every exported direct/tail callsite and decoded slot. Cross-lane row disposition is explicit; runtime indirect closure remains unverified. Among the bounded display, **1041 rows** have an out-of-lane edge among listed references. This is not the count of all indirect runtime edges. Full edge fields identify **1043 rows** with at least one exported out-of-lane caller/callee. Static call edges do not establish live runtime receivers.

## Unresolved families and limits

| Family | Remaining uncertainty |
|---|---|
| GUI/font/window/table | Generic hierarchy, input/focus/events, wrapping, serialization, glyph/texture lifetime, GUI factory owner mappings |
| Typed attributes | Name/index lookup, conversion/defaults, numeric/vector/matrix/quaternion/string/binary/enum/pointer/texture/light storage and destruction; TweakAttributesV90 callbacks are not complete CAttributes |
| Files/PAK/XML | Archive priority/fallback, obfuscation map, physical IO/clone lifetime, UTF16/UTF32 BOM reader state, writer and node/attribute streaming APIs; TinyXML is a distinct API |
| Batch/billboard/SceneManager | Registration queues, clipping/culling, shadow targets/stats, split/segment callbacks and all lifecycle/variant domains |
| Triangle/mesh utilities | Retained XYZ/triangle domain does not close octree queries, format/map/unmap, mesh copy/weld/tangents/colors/normals/planar UV/transform |
| Video streams/buffer/2D | GPU admission/upload/release does not prove nested-map/access/stale-CPU sync, stream aliases/conversions, serialized attributes or 2D material/texture lifetime |
| Animator/shadow/math/spatial | Empty/duplicate rows have explicit dispositions; containing virtual owners/callback order and unmapped numeric/KdTree uses remain unresolved |

Vtable slots prove possible targets, not actual runtime instances. No full GUI/scene asset census was performed; selected camera/cache/retained ownership paths were followed. Historical source snapshots/tests are not authoritative current-source matches. No runtime receipt was accepted as integrated execution of these exact mappings. The unresolved rows/families prevent claiming semantic or whole-game completion.

Assigned pseudocode SHA256 check: **1866 / 1866** LF-normalized UTF-8 bodies match indexed content; normalized-content mismatches: **none**. All **1866 raw file hashes** differ from indexed string hashes because Windows writes CRLF while the exporter hashes the LF string before writing. The CSV adds actual-byte pseudocode_file_sha256; this verified newline distinction is not a decompilation failure or source change. Cited source changes between targeted mapping and report completion: **none detected**. Hashes were captured with the mapping and checked at report end; they do not recover an earlier revision changed before initial hashing. Completion timestamp: **2026-10-08T14:27:08.774928+00:00**.

## Current source hash ledger

CSV citations include exact line and hash. Every source path mentioned as a candidate lead is separately hashed in source_location_files_current_sha256 (**71 distinct files**); candidate hashes are identity evidence, not semantic matches. Related source files read/cited in this report are included below. Hashes identify current working-tree bytes including pre-existing uncommitted content.

| Source path | SHA256 |
|---|---|
| port/android-native/app/src/main/cpp/model_renderer.cpp | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca |
| port/android-native/app/src/main/cpp/native_menu_preview_v121.cpp | c4b5b58f21dec3e0e95861c2b6c486c120aed1f12e9557ad0c4a248e24af73d6 |
| port/android-native/app/src/main/cpp/original_ui_assets.cpp | 6e801156f9ba1ee668dbbf2495966836dad6c8c64401050d5d35253cfa85c490 |
| port/android-native/app/src/main/cpp/renderer_buffer_budget_v41.inc | 3e19b3629518fe048e66518cb39dba5c4b8fe3ae277ad423fc1a241bdbf405be |
| port/android-native/app/src/main/cpp/source_campaign_batch_compilation_v111.cpp | 21a951d5e61c2767ee50f72e473d0c7a3c76f21ca5f78de0ebf7295254eb16f9 |
| port/android-native/app/src/main/cpp/source_campaign_file_io_v65.hpp | 7be253e16e3b89e4c6fc7b31fe5f66d87033c0a6f4289f187d34ac1cb9d17b7d |
| port/android-native/app/src/main/cpp/source_campaign_light_environment_v113.cpp | 8ace4939555692bc5bc37b1af3b7acc7e94f9695baf231ebf37a196934ab898a |
| port/asset-payloads/zip_asset_pack_v1.cpp | 38322384ea3114f1ea3aad0ee0211bb6f583dfaf4a76ae642645fe208a9b17df |
| port/engine-math/math.cpp | 04baafb69f3520bdee2a9097df8457903c9a3f04d3c1c66c5add870e06179ee6 |
| port/engine-resources/resources.cpp | 5908520d2a2bdd318669c30e0a6863c023b05327366cc0c1cad782700c945b25 |
| port/engine-ui/swf_font_geometry.cpp | db2c47657bb2227a017db5570a1d1cd097678f442ff0687f5cb319b3336d410a |
| port/engine-ui/swf_font_resolver.cpp | 1876dfb38e04eb4f31243d59d64a746ff5a3d3d62a8b1f0d44a72174d720e96a |
| port/level-loader/level_root_cfs_filename_v52.cpp | c5afff048df0629784936b938b9dc528bb8695603263676e1be3627a44158270 |
| port/level-loader/player_light_tweaker_owner_v90.cpp | db5804415fbcdf4b5b544be90dd1153c981639c95c0bbd6d8858423e8d8ed443 |
| port/level-loader/player_light_tweaker_owner_v90.hpp | 0490b9a4fe9ba761dde1f94d6f1a958aac3d3c3a767e858dd75f73956c8d9428 |
| port/level-loader/xml_document_v1.cpp | 1f66fee7d930ebd8f0e20585bb65bae869ddc9dd537d45b2b255b68c22169361 |
| port/level-world/canonical_character_candidate_v60.cpp | a44e80d17c6bbd0749e1145a8024f2e66e43ec0074c2403603b7c14513027e55 |
| port/level-world/character_particle_fx_resource_v2.hpp | a4703e0f75d3de8ca940f2bf838ec00c71f3ae5ff44f848a979597f8788b97c3 |
| port/level-world/character_same_scene_animator_v6.cpp | 8a65600fd518e7a7407f6571b9ac54f69710100329469765b531f010dcb68eee |
| port/level-world/gameobject_scene_root_registry_v1.cpp | fe048f354648d492e332f35ac90d0a1f9bb1b22b5e8ce075c0a477a4942750f8 |
| port/level-world/gameobject_scene_root_registry_v1.hpp | b7dcb68c6f980a2784fba1da868423dce49aac3d141d96153f39df5ecdbdee78 |
| port/level-world/gameplay_camera_factory_v16.cpp | ee21b8de401e6dfdd7c483cf83a9689dda5dc900aa3e0e7926fc3af0d9bd9862 |
| port/level-world/gameplay_camera_matrix_v8.cpp | 393cba426de1b48ac2b9f83cc6f6d8c8d5b1e5dc917fc7cdeb48efb78dcb7d00 |
| port/level-world/gameplay_camera_runtime_v11.cpp | 7ce61e3425f2609fcbb5b6053f840678ec8c34718e6fce4ba1ff330b149ca09a |
| port/level-world/gameplay_camera_scene_v3.cpp | c49e9143301e19981287625b1cfc0eafcbff7636dbd50b3ca1a9625c3382a425 |
| port/level-world/module_static_scene_v2.cpp | 4f669e3d8584791c08d1d28ba794d12809b0de23e02762ac7850ba99d78165d3 |
| port/level-world/native_batch_compiler_v111.cpp | c26aad71e8f562130919b1ded16c93fb8228b9eb49cf7e1823bf6b2fae2d6ef3 |
| port/level-world/native_batch_resources_v110.cpp | 79768da924ef977a1d206293421bb667c302958691c33868da7f43831a9354dc |
| port/level-world/native_scene_lights_v113.cpp | aeb9e3075040e91dc1bbc2c6983f121d22630d3a918dc87292c039bae2baf918 |
| port/level-world/native_scene_lights_v113.hpp | ac2a0764ce1baf2f009a98eb2afda8fbe4f23c01b8894ade71106766ed1f5a70 |
| port/level-world/retained_gameobject_visual_v1.cpp | 8b5bab7038393ac5b5d438a592bc1d88216b968b7f2295f6f80fe70667246701 |
| port/level-world/retained_gameobject_visual_v1.hpp | 704ab3a257f81f131f8aa04e3ad5c3da7b64fceb98235f72901854fdc5ce909c |
| port/level-world/retained_visual_child_v91.cpp | b9e9e3e0ce7bb8e98eceb9a536878a402e90626799965703dc8b220d54a5c09e |
| port/level-world/scene_manager_map_owner_v2.cpp | 827c6ffcc927464fa33165b17f4d620d95608c6d552dc7eee3097cf5c6225963 |
| port/scene-materials/scene.cpp | 5484076583d7fe9bab0085ed7ee14d31b7d43a3b30c08d7f2f936490f0f457a4 |
| port/scene-materials/scene.hpp | 5db567907f9938f023aa3a100365f0be413981f653d17b839d45ccd56f81641e |
| port/scene-materials/swf_texture.cpp | 5e5fb0a1c31c7a79d212db678a4df71be84f40bdb1e3ecde192a009599801b85 |

## Bounded post-pause renderer citation recheck

Rechecked at **2026-10-08T15:14:58.960742+00:00**, following the lane-08 obligation in post-pause-source-impact.md. Scope is exactly **14779–14780 and 14784–14786**. These five rows retain **unclear / low confidence / not_run**. Only their current renderer candidate anchors, candidate hash and bounded explanatory citations were refreshed; no whole-row parity or gameplay acceptance was established.

Current renderer SHA256 is **de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca**. The five previously recorded candidate fingerprints **bb979002f9b560f30796625bb844e25231381d6f3496b8cd78773d5e5516f69e** are historical and superseded for this bounded source read. The renderer hash was stable between candidate inspection and report write. Other source versions and the initial source-ledger completion timestamp remain the earlier audit snapshot.

| IDA rows | Current model_renderer.cpp candidate anchors | Bounded conclusion |
|---|---|---|
| 14779 / 0x59dc1c; 14780 / 0x59dea4 | visual scale:2088; placement scale:2090; retained root scale:2149 | Current Character/root/bounds scale handling is a candidate; original generic mapped-buffer position/normal mutation, grab/drop iteration and mesh box publication remain unestablished. |
| 14784 / 0x59ee60 | rendered actor point/bounds:3520; matrix composition:3522; submit:3524 | Original optional-box overload delegates to 14783/0x59e728. Rendered point/bounds accumulation is not evidence of the same in-place mapped stream/normal owner contract. |
| 14785 / 0x59ee64 | draw iterator/submit:3493; world draw matrix:3504; actor points:3520; actor matrix/submit:3522/3524 | Original transforms and releases every mesh buffer then unions/publishes mesh bounds. Current draw composition does not establish that complete destructive IMesh operation. |
| 14786 / 0x59efac | world draw matrix:3504; actor matrix/submit:3522/3524 | Original no-box wrapper delegates to 14783 with NULL box; generic streamed mutation and map/unmap ownership remain unresolved. |

These are refreshed **candidate locations**, not exact reconstructed-body matches. No source edits, builds, implementation tests, emulator runs or new runtime claims were made. Coverage remains **1,866 unique assigned rows**, with the earlier comparison-status counts unchanged.
