# Lane 10 — Collada animation, particles, lights and mesh ownership

Audit date: 2026-10-08. Assigned library: libDungeonHunter2.so. Assigned function numbers: **16795–18660 inclusive**. Source snapshot marker supplied by the coordinator: HEAD 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. This review reads the existing dirty working tree.

## Coverage and claim limits

- **1,866 assigned records and 1,866 CSV rows; zero missing IDs and zero duplicate IDs.** First address 0x60ed80; last address 0x64a0e0.
- **1,864 successful pseudocode bodies** read in full and structurally inspected for effects/branches/calls; **2 failed bodies** inspected from assembly. Assembly sections and exported xrefs were read for every record.
- Body files total **1,974,735 bytes**. All 1,864 body files have CRLF line endings in this checkout; normalizing CRLF to LF matches their inventory pseudocode SHA-256. CSV also records actual raw file SHA-256. The initial raw-byte mismatch was line-ending conversion, not changed pseudocode.
- **1,322 distinct machine-code hashes**; 114 repeated-hash groups cover 658 records, including 544 additional identical-byte records. Every member remains individually listed. Exact-byte duplicates and compiler-labelled clones retain their own source family and underlying disposition. Thunk classification takes precedence over clone classification.
- Manual semantic comparison follows nontrivial representatives, source consumer gates and ownership boundaries below. **This does not establish individual semantic parity for every unexamined overload, virtual ABI, parameter shape, generalized resource branch or lifetime/reentrancy case.** Family comparisons and structural body extraction are reported explicitly; partial/unclear rows preserve that limitation.
- Scoped source read covers **1,796 C/C++/header/include files** across engine-animation, engine-skinning, engine-math, engine-resources, asset-payloads, scene-materials, level-world and Android native cpp directories. Captured reference/reports/tests/vendor/build and archived integration trees were excluded from current live-source candidates.
- No source edits, builds, tests or emulator runs. Only lane-10.md and lane-10.csv were written. **Every runtime_acceptance value is not_run**: integrated runtime acceptance for this exact working-tree range was not established.
- Current source byte hashes appear in each source-citing CSV row and the table below. Resampling the 33 source files hashed during review found **zero changes**. Files first cited later were hashed when producing the report; a change before their first hash is not ruled out.
- Confidence describes evidence for a disposition. High confidence in clone/thunk/failure/partial classification does not imply gameplay parity.

| Comparison status | Records |
|---|---:|
| unclear | 3 |
| clone | 492 |
| partial | 1136 |
| out-of-scope | 81 |
| matched | 12 |
| import/thunk | 140 |
| failed-body | 2 |

There are no source-absence claims classified missing and no name-only disconnected classifications. The 140 import/thunk rows are ABI thunks, not 140 imported external functions. The 81 out-of-scope rows are compiler/STL specializations; they do not establish correct consuming game-owner behavior.

## F1 — Original track factory exceeds connected sampler domains (partial)

IDA record 17013 at 0x611ae0, getAnimationTrackEx, dispatches full position type 1 and axes 2–4, quaternion type 5 and angle types 6–9, full scale type 10 and axes 11–13, plus light/material families. Transform specializations select char, short or float from output metadata. Direct callers 18060 (dynamic addAnimation), 18975 (resource postLoadProcess, outside this lane) and 19177 (static addAnimation, outside this lane) make these actual resource/compile paths.

Source **port/engine-animation/animation.cpp:147** admits dynamic types 1/5/10, position axes 2–4 and angle type 9. At :152 it rejects compressed accessor scales/offsets and non-float outputs; Player at :324 also rejects compression. Example **17145 / 0x615b08** reads signed char, applies scale/offset, interpolates and restores other components from a default. Assembly **0x615b3c LDRSB** confirms signed compression. This original data domain cannot pass the current parser gate.

Scale-axis applicator exists at **component_applicator.cpp:18**, but dynamic compilation excludes types 11–13. Float angle leaf exists at **angle_interpreter.cpp:25**; complete live types 6–8 and compressed angle closure remain unverified.

The setter wrapper **17795 / 0x623d78** preserves value in R1, moves target R2 to R0 and calls vtable slot 0xa4. Pseudocode omits the argument. Source component_applicator.cpp:18 writes the complete vector and sets position dirty 8 or scale dirty 2; raw live receiver/singleton equivalence remains partial. Empty retrieve/destructor bodies are retained as genuine no-ops or clones, with no invented replacement behavior.

The CSV records narrowly matched standalone kernels for slerp 17052, weighted quaternion blend 17053, adjacent quaternion interpolation 17054 and adjacent float3 position/scale interpolation 17892/17896. These valid-input/adjacent-key comparisons do not upgrade the generalized factory or runtime owner.

## F2 — Material, light and signed additive animation closure remains partial

The range contains **380 material parameter track records**, covering float arrays, RGB/RGBA and component variants. For example **17645 / 0x621748** weights RGB bytes, forces alpha 255 and converts the material parameter using ID from applicator metadata. **material_color.cpp:24/:43/:56** and **material_color_v3.cpp:6** provide selected uchar alpha/color kernels.

A connected selected domain exists in **port/level-world/character_authored_resource_v32.cpp:115**, which accepts diffuse-color uchar4/uchar1 tracks with exact target/property/layout constraints. This does not establish arbitrary float2/3/4, RGB/component SetParam live adapters.

**17033 / 0x6126c8** interpolates adjacent uchar3 light colors. Source **scene-materials/scene.cpp:77** parses static SLight descriptors; **animation.cpp:147** excludes light animation channels. The animated live light receiver remains unverified.

**17062 / 0x613378** additive quaternion blend processes positive weights through identity-to-value slerp, negative weights through inverse quaternion, then ordered quaternion products. **animation_blend.cpp:50** exposes weighted slerp. A complete signed additive owner was not established in the scoped source. This is distinct from the matched weighted blend kernel.

## F3 — Database, animator and mutable dynamic-set ownership remains partial

**17365 / 0x61b2f4 constructNode** handles cameras/controllers/geometry/lights, old and GNPS emitters, coronas, forces and modular skins through factory vtable dispatch, applies TRS/visibility/name, recursively attaches children and drops references. It calls outside this lane to **16753 constructModularSkin** and **14630 culling**.

**scene.cpp:157** handles local graph/TRS/visibility/meshes plus camera/light projections; :163 ignores other instance tags. Dedicated retained particle/modular resource owners provide selected additional branches elsewhere. Ignored tags in this one loader are therefore not alone proof of whole-project absence.

Source **scene.cpp:166/:176** rejects external geometry/materials. Original **17407 / 0x61cca8 constructMaterial** temporarily resolves directory/path state, calls the factory and restores directory/references. Reader/factory/resource identity, external resolution, rollback and shared drop ordering are not established as equivalent.

**16958 / 0x60fb54 constructAnimator** triggers on animation/image/events libraries, adds animator-bearing tracks and calls **16957 / 0x60fab8 setEventsTrack**, which drops the old manager, creates a 24-byte manager and installs callback/user/cursor. Source Player/EventTrack owns copied BRES storage and selected events. Named-time leaves **16961–16963** match final-match behavior for valid formats against **events.cpp:60**. Dispatcher **16964** remains partial because original unknown format returns 0, whereas source validation returns -1.

**18062 / 0x62f61c compile** builds first-seen union, mismatch pruning, per-library mode 1/2 bindings/defaults, then calls **19180 CompileInternal** outside the lane. **animation.cpp:214 compile_dynamic** reconstructs selected transform domains. **18041 / 0x62e204 overwriteAnimationLibraryBindings** replaces a retained library and patches its row/segment tables in place. Source RegistrationSet append/default at **animation_registration.cpp:6** does not prove all overwrite/remove/default-index/lifetime semantics of records 18050–18071.

## F4 — Generalized GNPS and forces exceed current live particle domains (partial)

**18190 / 0x632b70 PWind::apply GNPS** and **18192 / 0x632f6c SParticle-labelled overload** implement point/directional vectors, exponential falloff and turbulence from three lrand48 calls, with position effects. Assembly **0x632cf8** advances 0x9c. A type name is not enough to infer storage width.

Actual source **particle_force_scene_v2.cpp:38** accepts gravity type 0 and deflector type 2, throwing for other types. **particle_bound_forces_v4.cpp:54** routes those two kernels. The wind live domain is unavailable by explicit source rejection, independently of textual search.

Original gravity includes point/falloff behavior; **particle_cloud_models_v1.cpp:56** rejects point_mode and positive falloff. Deflector kernels and retained force matrix owners exist at **particle_deflector_v1.cpp:16/:35** and particle_bound_forces_v4.cpp. Standalone **18145** friction coefficient matches its 0/1 endpoints, 0.07/0.035 thresholds and log/exp transition. Complete proxy/serialization/priority sorting/raw representation remains unproven.

Original GNPS uses **156-byte records**. **18234** size initialization writes current +96 and target +100 with symmetric variation; **18252** life initialization writes age +88 and life +92. Current kernels project **100-byte ParticleSeed100** fields and selected old particle behavior.

**18349 / 0x63beec** samples normalized-life motion and variation curves and RandVec. **18351 / 0x63c388** samples size curves. **18355 / 0x63c5a8** handles animated UV/color with sin/cos. **18366 / 0x63d390** handles spin curves or motion-facing branches. Assembly confirms accessor calls and 0x9c stride. **particle_cloud_runtime_v3.cpp:11** routes selected life/size/motion/spin kernels and scene-color fallback **particle_cloud_models_v1.cpp:65**; its simpler :32–:40 kernels do not establish generalized GNPS parity.

**18490 / 0x642e58 initParticleSystem** is a 740-line body: parameter setters/factory callees were traced and body structurally read. Its generalized emitter/spin/vertex/output/shared-buffer choices exceed **particle_resource_init_v32.cpp:10**, which requires box/sphere, Direction mode 1 and spin axes 0/2. The live selected resource connection is explicit at **character_authored_resource_v32.cpp:106/:116**, including generation/cloud/force/render owners.

**18075 / 0x62fe78 Rand** matches the source seeded recurrence. Assembly **0x62fecc–0x62fed8** constructs double bits 0x41dfffffffc00000, exactly 2147483647. Rounded pseudocode “2147483650” does not justify a denominator mismatch. **18339 / 0x63b44c hashString** matches signed-byte hash arithmetic against **particle_factory.cpp:31**, excluding temporary string allocator lifetime.

## F5 — Morph controller/process-buffer owner is unestablished (partial)

**16954 constructMorph** reaches original factory construction. **18649 / 0x649b3c setWeightedVertex** leaves destination untouched for weight 0, copies for weight 1 and otherwise assigns weighted source vectors, with packed/strided variants. It is called by **18667 / 0x64a6e4 morph** outside the lane. Process-buffer/init/morph/prepare methods **18665–18668** continue into lane 11.

Actual source **scene.cpp:169** rejects controller type !=0 and **skinning.cpp:54** loads skin. The scoped source did not establish a complete morph runtime; explicit parser rejection demonstrates the unavailable live domain. This is partial, not a text-search-only missing claim. Original **18556 modular clone** and **18639 morph clone** return null; no original allocating clone path is invented.

## F6 — Static light descriptors exist; full Collada object closure is unresolved (partial)

**18505 / 0x6444ec** and 18506 initialize color multiplied by intensity/255, type-dependent ambient/diffuse/attenuation/spot state, and native doLightRecalc outside the lane. **scene.cpp:77** retains local SLight descriptors.

Original **18095 / 0x62ff80** external-light default strips a fragment prefix, asks the live SceneManager and checks node type. Source **scene.cpp:79** rejects external authored lights. Separate native light-set support does not alone prove this factory/UID/recalculation/animation receiver path.

## F7 — Mesh and modular selection exist; native buffer packing remains partial

**18531/18532 CMesh constructors** request on-demand payloads and vertex/index buffers through factory configuration. **asset-payloads/payloads.cpp:97** and **authored_fx_mesh_graph_v4.cpp:59** supply decoded primitives and material bindings. **18549 / 0x646730 render** calls native setMaterial/drawMeshBuffer with original streams/attribute maps; **renderer_authored_effect_scene_v5.inc:139** provides current native draw transport. Cache/clone/on-demand/drop/stream/native object parity remains unresolved.

**18614 / 0x648fd0 setCategoryModule** release/construct/retain/install/drop has selected counterpart at **visual_skin_selection_v6.cpp:20**. Downstream **18610 / 0x6483c8 updateBuffer** groups material/technique and coalesces/reallocates vertex/index streams. **visual_skin_owner_v6.cpp:67** explicitly stores retained draw_modules and makes no original GPU packing claim. CPU skinning at **skinning.cpp:114** and retained draws establish selected rendering, not original coalescing/process/reallocation semantics.

## F8 — Both failed bodies have distinct assembly dispositions

- **18631 / 0x6494e8 C1:** assembly 0x649510/0x649518 initializes IReferenceCounted virtual subobject +0x17c and refs +0x180; 0x64955c calls **19326 CSkinnedMeshSceneNode base constructor**; 0x649574/0x649578 installs final vptrs. Caller **18161 factory** is in this lane.
- **18632 / 0x649594 C2:** 0x6495a4 passes VTT+4; 0x6495c8 calls **19326**; 0x6495d8/0x6495ec reads construction-vtable offsets -0x1c/-0xc and installs supplied vptrs. Caller **2185 BaseMeshSceneNode wrapper** is outside this lane.
- Current retained modular owner is **visual_skin_owner_v6.cpp:57**. Exact construction-vtable/live-object identity parity is unproven. Both CSV rows retain failed-body, source limits and cross-lane edges.

## F9 — Effect/profile factories and parameter binding have selected support (partial)

Records 18169 (SEffectList), 18174/18175 (material renderer manager parameter lookup/binding), 18178/18180 (material creation/setter), and 18206/18208–18210 (profile shader/material renderer creation) were compared to the actual effect pass, program cache and uniform reflection source. Original 18174 resolves shared string IDs and balances references; 18175 resolves the parameter definition then forwards its ID to the typed binding overload. Source **effect_render_pass_v4.cpp:13**, **shader_program_collection_v4.cpp:7** and **shader_reflection_v4.cpp:275** provide connected selected effect/program/reflection behavior. The pass decoder at :19 rejects multiple passes and :23 rejects stencil/sample-coverage/polygon-offset states. Exact original temporary parameter IDs, every profile, alias ownership and generalized shader-factory behavior remain partial.

Record 16952 fetches the original corona material, retains/swaps it and drops the old material. Source scene.cpp:163 skips corona tag 11; a complete live corona receiver was not established. This is partial based on the reached source gate, with no whole-project text-only missing claim.

Vector rotation records 18146–18148 have actual arithmetic source counterparts at engine-math/math.cpp:78/:81/:84. Float-rounded sin/cos, center subtraction, ordered plane products and restore order were inspected. Their rows remain partial because raw return ABI and all partial-overlap aliases/owners were not established.

## Cross-lane edges and unresolved work

CSV records every exported entry-point call/jump to another function and inbound entry call/jump, with actual IDs/addresses. Zero direct inbound xrefs explicitly does not exclude virtual/indirect callers. Relevant edges include 17013 <- 18975/19177; 17365 -> 16753/14630; 18062 -> 19180; 18549 <- 2161/2162/2163; 18631 <- 18161 -> 19326; 18632 <- 2185 -> 19326; 18649 <- 18667. Imported libc/soft-float/allocator callees are listed without assigning extra primary rows outside this range.

Cross-lane bodies were inspected beyond label/xref discovery: 18975/0x658c90 L142–149 writes the factory result into each 32-byte animation row; 19177/0x6601fc checks compatibility/URI and extra morph-index/material-property before requesting the handler, returning -1 on failure. 19180/0x6605ac builds start/end/length tables from each database. 19326/0x666b3c calls the mesh-node constructor, installs VTT fields, initializes the identity matrix and culling 2. 18161 allocates a 0x184-byte modular node with identity TRS; 2185 installs its wrapper VTT and render flag +380. 18667/0x64a6e4 maps input/output buffers and calls weighted-vector handling for position and an optional secondary attribute. Those original body observations support the source-domain comparisons above; they do not establish unresolved owner equivalence.

Unresolved domains: raw virtual receiver/singleton identity; compressed tracks, live scale axes, angle types 6–8, light animation and arbitrary SetParam; signed additive quaternion owner; mutable animation overwrite/remove/default library ownership; external resource resolution and rollback; 156-byte GNPS curves versus current 100-byte kernels; wind, point gravity, falloff, force priority and serialization; generalized vertex/baker/shared-buffer init; mesh cache/clone/stream lifetime; modular GPU coalescing/reallocation; morph process buffers/runtime; both failed constructor identities.

**Record coverage is complete for this lane. Full semantic parity and gameplay acceptance are not established for the unresolved domains.**

## Repeated identical-byte bodies

The largest groups are below. Complete membership is explicitly listed per CSV row by body_sha256 and identical_body_primary. Equality is original machine-code bytes, not normalized symbol/text similarity. Compiler-labelled clones with distinct bytes retain that distinction.

| SHA-256 | First assigned ID | Members |
|---|---:|---:|
| 379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f | 16795 | 63 |
| e7ecfdacaee34dd6f39b726dc7b5ffcc3adf55e2c7037d1df0d3033e53eb0543 | 17801 | 52 |
| 3e8a4d187fd09086abbee6fdcaed9e6195f6b5b8b3b55f27497a024e2d6b3d88 | 16838 | 26 |
| ba8ba744167a2aa74704cd56546aca85b93663f7d47a86ccc6d6d4d4a690d646 | 17237 | 26 |
| ee0724f4e95b9d2428a132e61278f4709cb3e2137ad2676bd32b535c9ee25e27 | 17713 | 24 |
| 86a9fb7e0d09e208a9ecd2deb026be65e2be489e28e1e9b55c738167a63d6d3f | 17418 | 23 |
| 5ca98cec000b405798801727b8bca53d6dc9bcb75e9651ab764222e394351a6f | 16865 | 12 |
| 0247b8149cd8443b11f1ccdffa1ee217fa5a4037627b719cf11f93445ea7b8db | 16878 | 12 |
| 424f60c1602173553ffae84c460ff89535e6aece13f9ca02a4a00e787804613c | 17596 | 12 |
| 1ba23be83fb2f7bacb9aa58898067a75156a26dc2a9dc94490374e5366b27b6f | 17692 | 12 |
| 06fc797d9829df4c3ad795586996a0d7396a61dd027ef8eae5cc3c573e7243d4 | 17700 | 12 |
| 705b7734f4666818e8402c1879d90d2874d87ebbefa22943a419b7b4f2eb86b5 | 16828 | 11 |

## Current cited source hashes

| Source file | SHA-256 |
|---|---|
| port/android-native/app/src/main/cpp/renderer_authored_effect_scene_v5.inc | f1c22754d6a82ceb3bf85cbc4b9f05ce8cfcb64f00bd82fb322f993a0ac32a38 |
| port/asset-payloads/payloads.cpp | ebc397d54978acd7da07f6f1c30a5fa7f232be34018af885527a734627a86c6d |
| port/engine-animation/angle_interpreter.cpp | 018a29c34536fce1831e9f78aee41e1c58584875fa522a22e05fc89fd84ab45f |
| port/engine-animation/animation.cpp | 968352af9ffb1f1aacba6d6efb4f821beef933b96774abaff8077efea18e26f8 |
| port/engine-animation/animation_blend.cpp | 5066e683add9d92643f350aa91819f694a7b2be82201785be40cc7812536d2ac |
| port/engine-animation/animation_registration.cpp | 74365756322d081aa171ca54802fbf5a94488f1e29a9b9a0260ff0bb2514f0ae |
| port/engine-animation/component_applicator.cpp | 9abca1a630447b10b7ea871f66af1e523511af5328fd03189e8be13dd1bb57bd |
| port/engine-animation/event_track.cpp | e7b70e88bbcbdc54ce28adbc78590fdc1005304cc8d017f9aab91149bf150792 |
| port/engine-animation/events.cpp | 5f2b833865f68399c337fd5037e011fac70c6ec7a539acd02f0c159f874d0f35 |
| port/engine-animation/material_color.cpp | 240bab4957913bbd41082d7b8ab5d13d804e09c8bbaa0e98a7bd7c511e92ef51 |
| port/engine-animation/material_color_v3.cpp | 41deb9b53027cb815919fc0bace3a338b4d8b2afb6a7c29f2ccda5577ac5f2eb |
| port/engine-animation/particle_billboard_v32.cpp | 61433cab652840a91c95ae365345f2dcba46b2acbe48a000e6d01228b5194caa |
| port/engine-animation/particle_bound_forces_v4.cpp | 9d9f1dc4c7b05a2f0b3117d31fb5bd072dc4cb238f50ae487dc3edf16816aa68 |
| port/engine-animation/particle_cloud_models_v1.cpp | 575ee1aa9c64832ceec3a84c4e71a6990474605a790d84f807a9cea82787d747 |
| port/engine-animation/particle_cloud_models_v1.hpp | ac90e53d9a6b9c9eb44a68f3017776adac375dd18642894e7c7698e02b731c7b |
| port/engine-animation/particle_cloud_runtime_v3.cpp | f4b1c102bf7c6c5f2fdbe2c5d78e7f851baa40b29616e7e58626160aaae3f2ba |
| port/engine-animation/particle_deflector_v1.cpp | f42482ebb0dc064e32c9c87691d6cbe69d4350502aaffb4dea5d6559ad921fff |
| port/engine-animation/particle_emission.cpp | 2da6f88c0d8ffc306e7e557fe92887b8a42a621d809c37f38f05f6a7573d776f |
| port/engine-animation/particle_factory.cpp | 7b7afcbfb8d52e123a296cfad31e6e6d5d3b3e3265899f845df8e7247abf9bce |
| port/engine-animation/particle_force_scene_v2.cpp | 09c33c0c578fa3c1424c812d6ed97c5ae836480e40ee9be6743ad10385a04c58 |
| port/engine-animation/particle_random_v1.cpp | 10841976082e5959fc2c72e7ea4e7799c9c64bd012038887a6b700644423e3c0 |
| port/engine-animation/particle_resource_init_v32.cpp | d1689290a55ac75e8b294e3e2410850391e39e8d2795dc128cb0a6cb8aad7b54 |
| port/engine-math/math.cpp | 04baafb69f3520bdee2a9097df8457903c9a3f04d3c1c66c5add870e06179ee6 |
| port/engine-resources/resources.cpp | 5908520d2a2bdd318669c30e0a6863c023b05327366cc0c1cad782700c945b25 |
| port/engine-skinning/skinning.cpp | ec5bdda6fbb982856d28f2aa288e16663935628648937971c9026689fec7fd90 |
| port/engine-skinning/visual_skin_owner_v6.cpp | 5e7e44c8a152cc705e7a4d11d02505622a745b73b9530d8d3e278ca635ce54e4 |
| port/engine-skinning/visual_skin_selection_v6.cpp | 5fd4136601821c8cdc1ab5bacd1f17f30d0aaedbe708dbd03a82de5ccd9d968d |
| port/level-world/authored_fx_mesh_graph_v4.cpp | bc7add361c9bf3149276db2f6e271b985d279c6bf86e6d441cbdf5905a558b7c |
| port/level-world/character_authored_resource_v32.cpp | 11076a12633ce6a0e70253fdc0b4ff66fe4ccb233e4c15321d4c3da781db1d27 |
| port/scene-materials/effect_render_pass_v4.cpp | eaca1eb604e4b1198603a6d4cebfae82d3364f2cf9e163c7d44f93491f600d29 |
| port/scene-materials/scene.cpp | 5484076583d7fe9bab0085ed7ee14d31b7d43a3b30c08d7f2f936490f0f457a4 |
| port/scene-materials/shader_program_collection_v4.cpp | 5beaab919c02a8bbc54baa77240a45acf3ad1b1b7ca4a7062ff1ef0c12dca431 |
| port/scene-materials/shader_reflection_v4.cpp | 027ab73c484cbf598cd95bceadbddaa62c6e4c54fe9e85e7b03129e0331f048b |

## Evidence reproduction

Inputs: .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/functions.jsonl, per-record pseudocode, assembly-functions.asm and xrefs.jsonl. CSV includes pseudocode path/effect line, raw pseudocode hash, exact code hash, assembly instruction count, all exported direct entry edges, source location/hash, disposition, underlying clone/thunk comparison, confidence and runtime status.

Historical inventory labels and historical stage/component checks were not accepted as proof of this working-tree semantic parity or integrated runtime acceptance. The selected matched kernels are scoped static comparisons; no runtime work was performed.

