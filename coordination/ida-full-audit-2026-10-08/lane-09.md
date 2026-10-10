# Lane 09: video, materials, textures, and Collada

Assigned range: libDungeonHunter2.so function records **14929–16794 inclusive**. The CSV contains **1,866 rows and 1,866 distinct IDs**, with zero missing or duplicated assigned IDs. All 1,866 exported decompilations succeeded, and all 1,866 corresponding pseudocode files and assembly sections were read. This lane has no external import or failed decompilation. The eight adjusting thunks have separate dispositions.

The source baseline is the README marker 75a7c2fe3261403e841ffbeb46734e9e4e84e75c and its already dirty working tree. The audit reads the working tree. Only lane-09.md and lane-09.csv were written. No source edits, builds, tests, installations, emulator launches, or external messages were performed.

**Source version:** initial review detected renderer drift from SHA-256 68df536883d56f0cd74f5b47052daafd07491faf5a75eec901790fd26afbe015 to bb979002f9b560f30796625bb844e25231381d6f3496b8cd78773d5e5516f69e. Those versions and anchors are historical. The bounded post-pause recheck now records current renderer SHA-256 de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca, cleanup/effect includes 1282/2312, camera invocation 3080, and clock declaration 616. Its exact 115-record and 14-source-file review scope is documented below. Source hash refresh does not establish whole-record parity.

**Inventory disposition is complete; semantic closure is not complete.** The unresolved records below must not be promoted to implemented or missing. Every integrated-runtime field is **not_run**. Historical component reports were discovery leads, and were not used as integrated gameplay acceptance.

| Comparison status | Records |
|---|---:|
| matched | 23 |
| partial | 122 |
| disconnected | 0 |
| missing | 0 |
| unclear | 1,295 |
| import/thunk | 8 |
| clone | 349 |
| failed-body | 0 |
| out-of-scope | 69 |

The clone count includes exact repeated bodies and separately inspected symbol-labelled compiler clones. Exact grouping requires both the exported machine-code SHA-256 and a normalized C body SHA-256. Normalization removes comments, whitespace, and the signature, while retaining declarations, operations, and referenced symbols. The appended table lists all group members, and the CSV preserves an individual record for each. Clone confidence describes the duplicate disposition, not source parity; underlying semantic status remains in the evidence field.

## Evidence and scope

Each CSV row includes library, function number/address, mangled and demangled name, decompilation status, body category, source anchor/hash or reason no exact owner was established, comparison/confidence, and runtime status. Evidence records original body path/hash, a body summary, branch count, assembly instruction count, and indirect-transfer count.

The complete exported call/tail graph was read for call/jump types 16–19. Individual caller/callee instruction sites are retained in CSV, including **4,355 outgoing and 1,127 incoming sites crossing this lane's range**. Decoding the little-endian words in vtables-and-rtti.json adds **1,600 vtable slot references**. Missing direct callers do not prove dead code. Indirect call sites and vtable membership identify leads; their concrete current owners remain unresolved unless traced below.

Source discovery began with rg and included active port .cpp/.hpp/.c/.h/.inc/.inl files. Production-owner claims exclude copied reference/test/report/vendor/build/handoff snapshots. compatibility/work source was also searched for exact class/name leads. original-functions.json labels are candidate leads only. Every cited source has a current hash in the appended table and exact source anchors in CSV. Related source in an unclear row is a candidate, not a semantic match. A search miss never supports a missing verdict here.

The 69 out-of-scope records are compiler-generated standard runtime/container/stream/allocator/algorithm helpers. That disposition is limited to reproducing the original ARM32 runtime ABI. Their bodies and xrefs remain recorded; game caller lifecycle is not declared equivalent. Containers that invoke original intrusive ownership and custom IDed collection comparators have separate unresolved dispositions.

## Compared paths and findings

### Batch append and clustering, 14929–14956

Original CBatchDriver::draw 14955 reaches thisAppendBatch 14954, which reaches spatialGridClustering 14947 and kdTreeClustering 14949. KD removal 14929 and nearest search 14941 reach lane 08 helpers, including 14897 and 14911. The grid path derives box centers, divides by cell extents, stringifies integer coordinates, and hashes them. The KD path mutates/searches the original tree. kMeansClustering 14948 rounds the requested count to a power of two, distributes means, refines/reseeds, and emits groups. Its absence of direct exported callers is not an unused-code verdict.

Current native_compile_scene_v111 in port/level-world/native_batch_compiler_v111.cpp visits selected scene nodes, resolves primitive materials, transforms attributes, remaps triangle vertices, and splits capacity-limited parts. source_campaign_batch_compilation_v111.cpp connects it to actual scene callbacks and GPU resource loans. This is a connected compiler. Original grid/KD grouping, append-buffer homogeneity, clone/remove, overflow rollback, and output-order parity remain unverified. The relevant records are partial or unclear.

### Driver and cleanup, 14957–15385

gameplay_camera_gpu_projection_v12.cpp reproduces 14969 orientation row swaps/sign changes and render-target-count guard. Its entry also performs the separate GL depth/flip conversion and clears the identity flag, so whole-record 14969 is partial. TextureDriverOptionsOwnerV1::set_option matches 14971 mutation before the option-100 disable callback on a bound valid driver. ActualDeviceProviderV54::renderer_type matches 15051 return 8 for the selected live GLES2 backend after owner/context/generation checks.

**Cleanup connection finding:** original 14990 IVideoDriver::removeUnused calls 14798 free2D textures, 16040 removeAllBatchBaker, 16022 clearUnusedInstances, 16025 remove renderers, 16246 clear placeholders, and 16309 remove unused textures, in that order. NativeDriverUnusedOwnerV50::clean_glitch in port/level-loader/native_driver_unused_v50.hpp expresses this sequence.

The reviewed production paths use another implementation. source_campaign_runtime_v61.cpp binds loading's cleanup and source_campaign_release_v88.cpp binds release's cleanup to clean_native_graphics_v50. model_renderer.cpp includes renderer_native_cleanup_v50.inc, whose body sweeps actual texture and buffer registries. renderer_native_cleanup_retention_v125.inc supplies retained geometry/draw ownership; UI/font/FX allocations remain in their RAII owners. No production invocation of NativeDriverUnusedOwnerV50 was found in the scoped active graph.

This supports a **disconnected original-like helper with a narrower connected replacement**. The CSV gives 14990 partial because a live cleanup route exists. It does not assert that current cleanup wrongly frees a live allocation. Whole baker/material/shader/placeholder release parity and consumer authority remain unverified.

Assembly for 15164 confirms last-unit setTexture, active-unit update, temporary nearest-mipmap filter for min-filter index at most 1, glGenerateMipmap, re-read/restore of the filter, and dirty bit 2 unless flag 2. texture_mipmap_v1.cpp preserves these operations via callbacks for supported indices. TextureBindingOwnerV1::set_texture preserves 15163 slot-before-bind and same-receiver dirty checking; its offline creation continuation requires another binding.

texture_unbind_2d_v1 handles 15165's matching-slot clears before glDeleteTextures and resets name/flags/dirty/pending bits. The live effect path invokes it through renderer_effect_texture_v5.inc. Original 15165 also processes cube faces; the adapter rejects non-2D. initial_bind_impl_prefix reproduces a selected generation/binding prefix and rejects unsupported explicit-mip continuations. These are bounded partial comparisons.

Generalized framebuffer/renderbuffer/buffer clone/reload, soft polygon/quads, full feature init, and CNullDriver records have original body/vtable dispositions. Whole-record current dispatch/ownership parity remains unclear. Genuine empty methods in CNullDriver are distinguished from failed bodies and port stubs.

### Materials and shaders, 15386–16242

Many boost::enable_if return types prefix actual IMaterialParameters methods. They are classified by their body/owner, not dismissed as Boost runtime. Typed getters/setters/converters/serialize/drop/grab and IDed collection families remain unclear unless a specific comparison was established.

material_matrix_is_identity_v4 matches 15425 cached identity, epsilon bits 0x358637bd, separately rounded diagonal addition/subtraction, absolute off-diagonal tests, and success cache mutation. shader_uniform_partition_v4 matches 16237 stable global-prefix semantics 34 through 62 inclusive and returned count/order. Reflection 16145/16238/16239 has an explicit dictionary, bracket/digit normalization, uint8 numbered prefixes, and fallbacks in shader_reflection_v4.cpp. Its full original alias/character behavior and process-buffer/shared-name ownership remain partial.

**Material hash boundary:** 15626 skips global active indices and semantics 11/15, hashes bytes with base 13, and combines ordinary low-eight and texture low-twelve bits with pass masks. material_compare_v4.cpp carries these rules but requires one pass, hashes native-width texture identities, and defines native matrix padding. The original uses ARM32 pointer bytes, 68 matrix bytes, and mutable per-pass dirty masks. These differences prevent a whole-domain matched verdict for 15626–15628.

Parameter comparison 15722/15723 is reconstructed for checked retained native views. The connection is real: model_renderer.cpp includes renderer_authored_effect_scene_v5.inc at line 2312 (post-pause wrapper recheck). That included path creates ShaderProgramCollectionV4, refreshes EffectMaterialDirectoryV4, and invokes material equality/order for the transparent queue. Source behavior is therefore partial, not absent or presumed disconnected.

Original identity initializer 15767 switches runtime types 0–18, including matrix-pool release, texture/light drops, and scalar/color defaults. The live campaign light transport in source_campaign_batch_compilation_v111.cpp reproduces runtime-18 NULL cells and URL assignments. It is a specific subset of that whole initializer.

Original shader 16230 search/XML fallback and renderer-manager 16019 instance allocation/refcount/cache protocol are broader than the current effect program/directory interfaces. NativeBatchProgramV113 performs actual reflected typed GLES uniform submission, providing a concrete related path for 15171/15186/15226–15228. Complete original global/local directory, automatic transform/light, dirty-mask, and indirect parameter closure remains unverified.

### Texture manager, image formats, and codecs, 16243–16570

Original CTextureManager::getTexture 16335 has cross-lane callers including SceneManager 2083/2146, GSInit 3142/3143, Billboard 3231, SWF loader 6302, AssetManager 11666, CResFactory 18976, and gameswf load_file 25680. It normalizes/finds names, selects registered loaders, and reaches 16333/16332. The current authored effect path concretely reaches renderer_effect_texture_v5.inc and retains decoded/GPU owners. Its named cache-file route does not close every original caller's cache, placeholder, fallback, and exact receiver identity.

PVR 16525/16526 raw/BTEX recognition, kind/format mapping, file-size accounting, and complete mip-chain checks were compared with textures.cpp. Native code adds zero/large-dimension rejection and narrower twiddle/decode requirements. Decode supports nonmipped 2D PVRTC2/4; original 16529 also constructs generalized CImage objects and 16532 loads through virtual loadData. Those ownership/data paths remain partial.

**TGA domain finding:** original 16537 includes type-10 RLE and 16-bit paths. Current dh2_texture_open explicitly requires type 2 and 24/32-bit pixels, with checked origin conversion. This is established by accepting/rejecting source branches, not by missing symbol names.

ATC/BMP/DDS/JPG/PNG loaders, image writers, and general CImage blit/fill/scaling each retain a distinct CSV record and body. No complete corresponding current owner was established, so they are unclear. Original 16365 itself rejects DXT/ATC decompression; those original unsupported branches are not reported as a port omission.

Pixel-format sizing 16341–16347 is partly reconstructed by blood_texture_image_v1.cpp for format-14 RGBA and format-27 4×4/8-byte/minimum-32 blocks and mip shrink. Broader PFD/depth/cube domains remain unverified.

### OS, streaming, billboard, factories, and events, 16632–16794

OS log/print/RNG bodies were read separately from timers. 16642 assembly decrements the original stop counter; 16652 increments it. 16651 passes float bits in ARM R0, rebases time before storing speed, and clamps negative speed. 16647 uses gettimeofday epoch milliseconds. Current seeding/frame consumers use steady_clock. The presence of native clock providers does not establish original virtual speed/stop/last-time parity.

**Streaming domain finding:** 16688 getAnimationBlock is called by particle initializers 18433/18846 and scene animator owners 19077/19153. It reuses 16660, obtains 16683, and transfers intrusive references via 16687/16678. grab 16685 can prepare next block 16684. 16681 obtains on-demand bytes via 16668 and mutates inner pointer relocation/state/refcounts.

16680 caches segments and 16673 evicts only original entries whose reference count equals 1; 16673 is also reached from Application::Init 1060. Assembly confirms budget/count mutation and release paths. Current dh2_animation_open accepts immutable in-memory state-0/state-1 views and rejects state above 1 or already-inner-relocated state 1. Player/compiled animation owns immutable bytes. This source does not reproduce lazy readers, circular blocks, or the cache eviction lifetime protocol. Whether every actual animator/particle consumer stays in the supported immutable domain remains unverified.

16704 CBillboardSceneNode::updateAbsolutePosition is camera/parent-dependent and includes orientation modes, normalized bases, inverse matrices, quaternion rotation, and recursive child updates. Generic scene transforms and particle billboards are related source, but no equivalent retained Collada billboard owner was established. Its adjusting destructor thunks 16699/16700/16702/16703 are separate from the destructor bodies. Quaternion 16695/16705 arithmetic matches math.cpp on valid nonaliasing inputs.

BRES getters 16728/16730/16731/16732–16744 root offsets and record strides agree with resources.cpp, including image 20, effect 116, emitter 144, and GNPS 232. These matched accessors are limited to valid opened immutable images and in-range indices. They do not establish original object lifetime or unchecked invalid-index behavior. Original 16726 retains database/texture identity and 16727 mutates root+8 material backlinks; immutable scene decoding does not itself reproduce those runtime effects.

Factory 16748–16760 dispatches exact CResFactory virtual methods. Current effect/mesh/modular-skin/emitter source provides partial producers; external URI, arrays, factory-null, pointer identity, and rollback closure remain unverified. The kind-1 nonrender geometry branch has an explicit authored_fx_nonrender_geometry_v32 source adapter.

Event 16725 search and 16761–16766 arithmetic/boundary/order/wrap behavior were compared with events.cpp. Original dispatch recomputes lag/group addresses and re-reads the track after each callback. Updates retain/drop the event manager around dispatch and publish the four-time cursor afterward. The current callback kernel reads immutable EventView.

Live callers exist. actor_playback.cpp copies the cursor and uses an event-generation guard. RetainedGameObjectVisualV1::update_callbacks_v21 takes an event-owner lease, checks removal during delivery, and can abort via exception. These adaptations provide a connected event route; full replacement/reentrancy/owner destruction equivalence remains partial. No runtime acceptance was run.

Empty CVirtualEx destructors 16767–16794 are genuine empty original bodies. They retain individual signatures/vtable references and exact duplicate dispositions.

## Remaining limits

The 1,295 unclear records still require complete per-branch/ownership/source comparison; the 122 partial records have explicit unverified domains. Clone records also retain an underlying semantic limitation. All pseudocode and assembly bodies were read and dispositioned, but the concentrated detailed comparisons above do not amount to semantic proof for every large unique driver/template/image body.

Unresolved families are generalized driver/framebuffer/buffer and null backend lifecycle; original batch grouping/overflow; complete typed material/global directory conversion/serialization/reference authority; loader registries/placeholders/codecs/writers/blit; shader search and automatic/indirect uniform commits; OS virtual timer/RNG/log owner mapping; streaming/circular cache release for actual consumers; Collada billboard and factory identity/external rollback; and event callback mutation/reentrancy/owner transfer.

No scope-negative verdict is based only on text matching. The narrower-domain findings cite actual source rejection branches or a traced alternative production route. This lane is not a gameplay-complete or parity-complete acceptance result.

## Repeated bodies and source hashes

The tables below are generated from the lane CSV and cited source snapshot. Every repeated-body member remains an individual row. Hash rechecks are source read-only.

### Exact repeated code/body groups

There are 299 repeated assigned records in 50 groups. One duplicate preserves its thunk disposition; the remaining exact duplicates have clone dispositions. Named compiler clones without these exact hashes are recorded separately in CSV.

| Canonical ID | Members | Machine SHA-256 | Normalized body SHA-256 | Every member ID |
|---|---:|---|---|---|
| 14958 | 157 | 379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f | 41b805ea7ac014e23556e98bb374702a08344268f92489a02f0880849394a1e4 | 14958, 14959, 15013, 15049, 15050, 15054, 15062, 15063, 15064, 15066, 15067, 15068, 15076, 15077, 15230, 15231, 15233, 15236, 15238, 15240, 15241, 15242, 15246, 15250, 15251, 15252, 15253, 15254, 15256, 15295, 15297, 15298, 15299, 15301, 15302, 15303, 15304, 15305, 15306, 15307, 15308, 15309, 15310, 15311, 15312, 15313, 15314, 15315, 15316, 15317, 15318, 15319, 15320, 15321, 15322, 15323, 15324, 15325, 15326, 15327, 15328, 15329, 15330, 15331, 15332, 15333, 15334, 15336, 15337, 15339, 15342, 15346, 15347, 15348, 15352, 15354, 15355, 15391, 15412, 15413, 15416, 15625, 15629, 15630, 15790, 15791, 15795, 15956, 16101, 16102, 16166, 16249, 16447, 16448, 16463, 16478, 16490, 16491, 16492, 16495, 16504, 16520, 16521, 16533, 16543, 16546, 16555, 16565, 16571, 16572, 16575, 16576, 16602, 16656, 16657, 16706, 16707, 16708, 16709, 16710, 16711, 16712, 16717, 16718, 16719, 16720, 16721, 16722, 16724, 16767, 16768, 16769, 16770, 16771, 16772, 16773, 16774, 16775, 16776, 16777, 16778, 16779, 16780, 16781, 16782, 16783, 16784, 16785, 16786, 16787, 16788, 16789, 16790, 16791, 16792, 16793, 16794 |
| 14965 | 46 | 6dcd5e75586fe6683156c0d559d4827b73bd48d501fa5781a1c6099aacd7c877 | 6be2e46a3d0765fd4d08def164c35b75afcc481db6fd11cc5118c4e2c20b634d | 14965, 15052, 15071, 15232, 15234, 15237, 15239, 15244, 15257, 15259, 15263, 15264, 15265, 15266, 15269, 15271, 15274, 15275, 15276, 15277, 15278, 15280, 15281, 15282, 15283, 15284, 15286, 15288, 15290, 15291, 15292, 15294, 15338, 15340, 15344, 15353, 15624, 16438, 16439, 16440, 16443, 16445, 16474, 16515, 16517, 16750 |
| 15051 | 2 | 3fc49adc7394262080e1e06db1aa4d983864b2d9b150c07306faf456deb4732e | e6c84c27804b2960643534641b9b2c714bddcc4af082d307590f213ab6e6c2a7 | 15051, 15055 |
| 15243 | 15 | 007f34a6c3441b0240da53f253e513b959105da2d8803255e1856aa48f48db47 | f58b7c3af621b52a2bb7dc67d4491f9ab6c6d16e3cfa1e46e670ff4f9a301fdc | 15243, 15245, 15247, 15248, 15249, 15255, 15267, 15268, 16441, 16444, 16469, 16473, 16493, 16513, 16516 |
| 15258 | 2 | b34bde848b9413dc767f6576ca23af07315ab29cc47e025ef5966d04f2dc5621 | 6be2e46a3d0765fd4d08def164c35b75afcc481db6fd11cc5118c4e2c20b634d | 15258, 15262 |
| 15270 | 2 | fb0581a28aa248731effd838725152b62f61f1166f05995b930e657deca25c81 | 0629c08bd3634b33291db01e1e8342886870bfb9c256cf97a0697b780d362b08 | 15270, 15279 |
| 15272 | 2 | d3e487b257f9877b4b2b201f812bf6f41ab2a9b7b43592e2cd6f4c54c49f658a | a7a37dc80e26a3ab73d0bdc7b322b777279cd0c5e546117b3e09521147f77262 | 15272, 15273 |
| 15289 | 2 | 4b0c5b35c8077d21a76ebbfa4b2d886bab3fa8815e786d91d05ed8cacc0e538c | 924d543b6ee2c1412440db99a9d68e46a56d27379d611bebb79a99889d907a08 | 15289, 15293 |
| 15296 | 3 | 0efcbe12bd7d505235ffab66b191e312b762871400d6f0870fd53b7c997bdd8d | 41b805ea7ac014e23556e98bb374702a08344268f92489a02f0880849394a1e4 | 15296, 15300, 15341 |
| 15345 | 2 | e2c7554dcd956172ec8462492a5edb51968a743be7ddb65c9c4bb0fcec292444 | 560ac812055960587a8e8106d1a5bf234d00242ccf91552d1d64f16ded8e2834 | 15345, 15351 |
| 15349 | 2 | e2c7554dcd956172ec8462492a5edb51968a743be7ddb65c9c4bb0fcec292444 | c1ed39aa614043a6a237daeb63bd44f7e760a0a6208ac674a9b13650d29346be | 15349, 15350 |
| 15389 | 4 | f4a3c5983d5d6327d02cbb7ccc65f06e0fd3bcef765b95aea7fde908f50cc313 | ed566037a6756b14d7af72c914319d0171e135aa3fccc1bd3391426d2ef4db97 | 15389, 15958, 16168, 16250 |
| 15390 | 4 | fdc1710f100ccf2b13937a27e7f23ccda5a1973826c8d8f3b7db09e6988fe8a4 | 6e8ee48e3eb541777e64315179bd9faa5a25b06c94cd4528fcf665429ebeabdc | 15390, 15955, 16165, 16248 |
| 15394 | 8 | fb96157746b77973c77cc293ccb5525fa4a0621801621b75be2aab03329d9c4e | 7bf05ab6eecf14345c9930af69d6442f3af2309dcf9f1e0dd48ed3fd1407a35d | 15394, 15397, 15961, 15962, 16171, 16172, 16254, 16255 |
| 15398 | 4 | 23161b9a0148d17f33959134347fc440745ead03b077d73811bcfbb39d175451 | f168f12465ed72216caf8017eec2aae7264b1ea41a95bb61046aba23e7887cf4 | 15398, 15963, 16173, 16256 |
| 15399 | 2 | 6e4505190638a2e197f2e8d3fd1e4879872715564ce44c7290221bda5f98d93c | bcee22ba307f8c1ca3a0f7a0c867785ac3154902244223193011ce1e351360af | 15399, 15400 |
| 15402 | 2 | 04666abbddc1bf0dae2a954766e410aef42d72ef4d381971004e4502c19fe73c | 62102cd9d0c22ede402ae6723d7e1011e855729e223bfe9b11df01363f8a53ad | 15402, 15403 |
| 15404 | 8 | 7c86806e7bbb6744b2922c440ef796c74d6840428e533dccb881eebb72f8a83c | 127b9be21abc76547ef75f022daaf93be22fad7bbce3bb6881576dc765a64df5 | 15404, 15405, 15967, 15968, 16177, 16178, 16258, 16259 |
| 15406 | 4 | 4eac897c57b0c1b024e122587efc3ff63ea45140ef208f533ffcf47a6d6c656c | ba2e76c5fd73f103f38dc14b92f15b371d613a8c1ddaef5bcf22a3b3412cfdd5 | 15406, 15969, 16179, 16260 |
| 15407 | 4 | b77067177e2cd4f7aab57202cca9d6a8fff0ef2551d1d0367b58c2cafdf2d8d0 | 921a62c845744468212978ea647700c62a23c5e26e21fa468499fb48123b14a8 | 15407, 15970, 16180, 16261 |
| 15408 | 2 | 720519b192934f710d422d7f2683b46c46e6eb474cd8073636d64c5b851cf0c1 | ac7dabfd57365ab789d839e3cd5322706040ec3a7c3c2fc2b4331b4bd0893750 | 15408, 15409 |
| 15410 | 2 | d025ebe130aff2792a423ff551fc3eee3803fc37ca17ff662a3a10f0d381260e | c87115b27401f1e1d44a88430c60b8a990e8e2c7be8b333d12cfed1c3dd8a70a | 15410, 15411 |
| 15414 | 2 | 05014d08d84ade3f746c6eab6e19483648e6c8fcdd891a7670ad82468df86613 | 52574fe462c9f4d3f2ed4dc3cea50fe43b811571312a51b7e1ace20c0c1a8403 | 15414, 15415 |
| 15419 | 2 | 3bb14bbb0a74c33f5129ea185803f49fbad1acb814cdb27b43ab6894ccbd9187 | a504d9d81a41ba85bd0cbf9141997353bce34d90a48cf9562479465a329cd0c8 | 15419, 15798 |
| 15420 | 2 | a1ca3ca572e21dbdeccadaafe50632430b8e8ec91ad0b9bc13dc5335a569071d | d7e3c062beb0443f704ad65c41e3e08596b20803befe7b536b10886eefb6a3cf | 15420, 15799 |
| 15423 | 3 | b5a06e316dd0be0d74d4e445fdc6f89e0101d8d8647d99531a40a78b4cd6bdb0 | db0fb5bb684e6f0f098444aa3961df3043de8c78f8d681949253caaa5afaea7e | 15423, 15640, 15802 |
| 15424 | 3 | 037a4af2824634ecd5685d468a45b5832c058a409c6f98ad0b8a599dd96b459e | 17120084519477bb8347d4646116768ad541fa06a3fade3e85f0fed3d3d6ff0b | 15424, 15641, 15803 |
| 15475 | 2 | 765ae85285d5a02dc525a945fc3ba029c88d7ca2f93838292b249de37594a784 | 9d990c5e602340beea1018ed792cbe3ebd9ae3dfcf6c99a993aa8b87bb965704 | 15475, 15478 |
| 15631 | 2 | d5067b070d30013b6c9a85dc6906da7038dde479912531cc19405b57fb2b5a82 | 35c1d28cb12726ce0fb3653e31cec1f0b76eed4055a9bb99d3aab76e77a40a26 | 15631, 15632 |
| 15792 | 2 | f784f97224e88b07c2539cb007a7a0f937f635d59f538c9f3e1d31af77ad9831 | 7c1308af95f1382e2027fb7268409ba5454c00c7454d44a788f638181d5fbaf9 | 15792, 15793 |
| 15953 | 2 | b55f617eb044b12cf93ce3ee6104032a02478ada35c6c13cadb070b23f2f146d | 588ec7fb5c51cc26eb970a3b2b7cd0b156e9a7ba2687ff18a5c2d5c101480e27 | 15953, 15954 |
| 15959 | 3 | 571551556fcc4f47799d62a78485b7c1f2f3914600619d6e94da6bcbfde3c458 | ea8975a5a4fab9a696cdafb22af17a68a057691be8124d5cce8e1df6e42a41e8 | 15959, 16174, 16257 |
| 15960 | 6 | 3791337f3a596b02ac6436ce2a5160571d9b77146297ad63597323e35ed8955f | 9882b3388bfb42e6a99a230270195429d8d8c11f41b8f77d369c13c05522285f | 15960, 15964, 16169, 16170, 16252, 16253 |
| 15965 | 2 | 4d859eeb2206f8a0c4e8c4b34c4af06fb8ff125b948e3aa950fdb1b9cd9b0790 | 097ab7c18453908458f28cccea88a73fa744d980c07290ecc8c709c8b78743ac | 15965, 15966 |
| 15971 | 6 | 9482f41bdb65959bc6590c1de13110314d82cc44a98a99d94bfcd29a1fb3bbac | 6ddd8ecb7b54c27ee35077f4533d8dfb12c6a66abf00c0e82a2ebb910b390ade | 15971, 15972, 16181, 16182, 16262, 16263 |
| 15973 | 2 | 7cf50de5345607bce2ab911c9e2d8a7cf3123239b280fb3a85bbd09413db29fb | 6aa29c5da18a25aa4dd7dee9c90c1c3d653644d0c76dc2124aa791ff535edcc0 | 15973, 15974 |
| 16099 | 2 | 68122bb366225543a13c3ba4016af9947cd8de7e10eb3e6dfbe655023d77d524 | b054aa70388d7a4bf2f8c47a53f5c435eae26ca61d914a491f9411ec9dcac37e | 16099, 16100 |
| 16175 | 2 | 1a04c2c6b7e80f1595bd50ac56f606f267b995b94d3ec59d36ef030f6b6da6c1 | ab1813a4bf59fa79f95c2de6cca7ed3d0a86bf482335c1a19bf98a07af530562 | 16175, 16176 |
| 16183 | 4 | 81e953ffd34a81c2907360287f4fca925c85440566baec0ba31c193ab3c6f247 | bed92ca76c963c974cd6d73a716d0b6fae35b4d39556121bba5a87c133ae9c55 | 16183, 16184, 16264, 16265 |
| 16442 | 3 | 687b83fbb1b1084c4b61325547b5f8fadfb354f0d8aa39fcef5ecb6ba6b86ed1 | d9015bc3e8341d1cdf7e2cc6c07f6efc70c01263a5781f3d0e04dd6a945a54d2 | 16442, 16471, 16514 |
| 16459 | 2 | 7bd5f9e001cddf884ed6ffa2fa1bc64916a76827a4c97391ae5fc6532d234693 | 43263ecabe28b7b1ebdbc04b5f3cc10ae949da7b108f166bf8da65517a0694dd | 16459, 16460 |
| 16475 | 2 | 7bd5f9e001cddf884ed6ffa2fa1bc64916a76827a4c97391ae5fc6532d234693 | b7c216c7f507a9aa7a0b1edcd98c69cd587a458b3c55118a9faf184ad87a8702 | 16475, 16476 |
| 16488 | 2 | 7bd5f9e001cddf884ed6ffa2fa1bc64916a76827a4c97391ae5fc6532d234693 | b54250dde6c32e95fb9a6f9e4b28f3b23e48981f4f1c8f190552533477c48fcb | 16488, 16489 |
| 16518 | 2 | 7bd5f9e001cddf884ed6ffa2fa1bc64916a76827a4c97391ae5fc6532d234693 | 5b9570a0862fb92afb8d19ae2e31b67754b2caa2a3192964585477b05aada589 | 16518, 16519 |
| 16544 | 2 | 7bd5f9e001cddf884ed6ffa2fa1bc64916a76827a4c97391ae5fc6532d234693 | 768180146dcac3538aaacc3caf579545708ae38ddea86eebb7c85bd315540dc0 | 16544, 16545 |
| 16553 | 2 | 7bd5f9e001cddf884ed6ffa2fa1bc64916a76827a4c97391ae5fc6532d234693 | 95f04150244853b59a1d57d2c42be876b1a3245a11d8d1047c4723548e341d74 | 16553, 16554 |
| 16563 | 2 | 7bd5f9e001cddf884ed6ffa2fa1bc64916a76827a4c97391ae5fc6532d234693 | 2ef590d05a7846fde489ea4b01b61625936ded4e1c38a97418ca0a736e37905c | 16563, 16564 |
| 16573 | 2 | 350ebe5de80d2a3496bbca96d0d6cfd3b881886e7e01f0d6ad6277028e2effde | 21aaba28d3e0290395bde3b8bd47abfb59347a9034a1da6466666564756c7fa3 | 16573, 16574 |
| 16658 | 2 | c7cc5f3eea1eb5bbf5f7eae9795a3d0d5fc8f25dd6b8a57c0c01088c66772b2a | 04b770ea1a8230106296beedb34f7446aeb5f367f669590866621222391fd2bf | 16658, 16659 |
| 16661 | 2 | a873b53ed05821cea8b6815af7f5d7995350ac2d14630aa4d06b7f243145903c | cc55e9b6ca741317c06118d300985659a45f7d3d43bd4934348682f880025e40 | 16661, 16662 |

### Source snapshot SHA-256

Hashes were captured after source read and rechecked at report writing. 1 cited source files changed during this review. CSV contains exact source anchors and current hashes.

| Source file | Current SHA-256 | Recheck |
|---|---|---|
| port/android-native/app/src/main/cpp/actual_device_provider_v54.hpp | 6eebffc3cbf48fa0c0302f4edba4b9317c77a9643e6456608246eab1ffd34d74 | unchanged |
| port/android-native/app/src/main/cpp/authored_shader_program.cpp | f7c0cc40d71f5dd00c802bf83b36afe1378d7c576c43e73a86dfb5da04d30c58 | unchanged |
| port/android-native/app/src/main/cpp/model_renderer.cpp | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca | initial drift recorded; post-pause selected wrapper/owner recheck stable |
| port/android-native/app/src/main/cpp/native_batch_material_v112.inc | 7788590e66a6062ba98d131117fb74336b13ab8c791d684f0d8e3e1f8fa69a79 | unchanged |
| port/android-native/app/src/main/cpp/native_batch_program_v113.cpp | 10b946d127afbf419fe06b784036d4021edba01a06f0edfe68d332ff5e274a4d | unchanged |
| port/android-native/app/src/main/cpp/renderer_authored_effect_scene_v5.inc | f1c22754d6a82ceb3bf85cbc4b9f05ce8cfcb64f00bd82fb322f993a0ac32a38 | unchanged |
| port/android-native/app/src/main/cpp/renderer_effect_material_directory_v4.inc | c226d2ca9f9a3943433c454373cfcea1802064653a3deedaae1307cc7a3e0206 | unchanged |
| port/android-native/app/src/main/cpp/renderer_effect_texture_v5.inc | a366fdc8df488c450197bde5512c68c18ed571f8e748b772daf4759d9f700391 | unchanged |
| port/android-native/app/src/main/cpp/renderer_native_cleanup_retention_v125.inc | c44882f80a0bac2253690d00d45c9fd94124a2636f4b4dacc42f68a4feb6161b | unchanged |
| port/android-native/app/src/main/cpp/renderer_native_cleanup_v50.inc | 54df48b41112553bf7bea64d5ead4701f67620e6acd512aa5464e11947896537 | unchanged |
| port/android-native/app/src/main/cpp/renderer_source_campaign_v55.inc | 398e3bf86b7b0ffe732afc8e9d4eeafed53a1c22ed64dbe4fb8b5260e031e87c | unchanged |
| port/android-native/app/src/main/cpp/source_campaign_batch_compilation_v111.cpp | 21a951d5e61c2767ee50f72e473d0c7a3c76f21ca5f78de0ebf7295254eb16f9 | unchanged |
| port/android-native/app/src/main/cpp/source_campaign_release_v88.cpp | d98ae5cebbd12db699de9c9e22d2bc2a621736e60624f6ffe7f7db43b3fb74b3 | unchanged |
| port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp | 6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e | unchanged |
| port/asset-payloads/payloads.cpp | ebc397d54978acd7da07f6f1c30a5fa7f232be34018af885527a734627a86c6d | unchanged |
| port/engine-animation/animation.cpp | 968352af9ffb1f1aacba6d6efb4f821beef933b96774abaff8077efea18e26f8 | unchanged |
| port/engine-animation/event_track.cpp | e7b70e88bbcbdc54ce28adbc78590fdc1005304cc8d017f9aab91149bf150792 | unchanged |
| port/engine-animation/events.cpp | 5f2b833865f68399c337fd5037e011fac70c6ec7a539acd02f0c159f874d0f35 | unchanged |
| port/engine-animation/particle_cloud_runtime_v1.cpp | 76a3accf7a3998d9bcfe0b6317549a2390e0a4fc3cd40331aea38a12293b854e | unchanged |
| port/engine-animation/particle_factory.cpp | 7b7afcbfb8d52e123a296cfad31e6e6d5d3b3e3265899f845df8e7247abf9bce | unchanged |
| port/engine-math/math.cpp | 04baafb69f3520bdee2a9097df8457903c9a3f04d3c1c66c5add870e06179ee6 | unchanged |
| port/engine-resources/resources.cpp | 5908520d2a2bdd318669c30e0a6863c023b05327366cc0c1cad782700c945b25 | unchanged |
| port/engine-skinning/visual_skin_owner_v6.cpp | 5e7e44c8a152cc705e7a4d11d02505622a745b73b9530d8d3e278ca635ce54e4 | unchanged |
| port/engine-textures/blood_texture_image_v1.cpp | eb26bbb9fef7880265eb6eedc6521187a40a2fe6dbe3874e3f41001e44ac12fb | unchanged |
| port/engine-textures/general_fx_texture_image_v2.cpp | 11b948496bb3e16e401b3d1bd0a482bd48e61f6b1c7828796599656572344503 | unchanged |
| port/engine-textures/texture_binding_owner_v1.cpp | 6a9dc09420f9675048d04909ebae9545ed2a7e7abb2e032d22a503523dd61419 | unchanged |
| port/engine-textures/texture_driver_fields_v1.hpp | 943d8971d69f4a900ddd79fecf94c97137ecadf81925881effa5309d42c9d143 | unchanged |
| port/engine-textures/texture_mipmap_v1.cpp | 8bbdb7a4e31fb3553aca77ec7df01ac557ccaa3f8aa35b7b92a87cb35e0bbbac | unchanged |
| port/engine-textures/texture_owner_v1.cpp | 192196f5f73082e86dc7acfc7ddd98524dfe5676de003a95c1242fc17e8a42f2 | unchanged |
| port/engine-textures/texture_unbind_v1.cpp | 1c0cc3e01ea24e9f68718dca2cdbf95c5fd87dce94c901ca6cf3b271abbae5d9 | unchanged |
| port/engine-textures/textures.cpp | 80264623322fd3262ceeebe1473711ebf5c9546a1f2028d0915c17d5572eb7cb | unchanged |
| port/level-loader/native_driver_unused_v50.hpp | 4795a614d8c19c65a4c666d24d0713fee071f72c56b34f95df88079d6e292d3b | unchanged |
| port/level-world/actor_playback.cpp | b897daf53fc901e065031e16961f8ac611363b03d31fdfbf13a82d7fe166a052 | unchanged |
| port/level-world/authored_fx_nonrender_geometry_v32.cpp | a912ddf16a1cd9dde9558f44d039a8ea4fc87fe708a281ebcbd596948b6f2f6d | unchanged |
| port/level-world/gameplay_camera_gpu_projection_v12.cpp | e51aa0771d7eb1066de9bc551657d6de5cc5e7757494d345735d58c652ca0e41 | unchanged |
| port/level-world/native_batch_compiler_v111.cpp | c26aad71e8f562130919b1ded16c93fb8228b9eb49cf7e1823bf6b2fae2d6ef3 | unchanged |
| port/level-world/retained_gameobject_visual_v1.cpp | 8b5bab7038393ac5b5d438a592bc1d88216b968b7f2295f6f80fe70667246701 | unchanged |
| port/scene-materials/effect_material_directory_v4.cpp | 33f46bcf778c739340db87fd992b5d0c0c2cda2d0e5f5591795c1a47f7b686c8 | unchanged |
| port/scene-materials/effect_render_pass_v4.cpp | eaca1eb604e4b1198603a6d4cebfae82d3364f2cf9e163c7d44f93491f600d29 | unchanged |
| port/scene-materials/material_compare_v4.cpp | aeab85d6ea22ce6a0a9fa44bd7c6ca5b6b03963e5b5503b7539f88b49ef60665 | unchanged |
| port/scene-materials/material_matrix_v4.hpp | 441a01b6d85f69b2e4680521be9185ef1c28ff248a85014d0434d4dccd00f163 | unchanged |
| port/scene-materials/scene.cpp | 5484076583d7fe9bab0085ed7ee14d31b7d43a3b30c08d7f2f936490f0f457a4 | unchanged |
| port/scene-materials/shader_program_collection_v4.cpp | 5beaab919c02a8bbc54baa77240a45acf3ad1b1b7ca4a7062ff1ef0c12dca431 | unchanged |
| port/scene-materials/shader_reflection_v4.cpp | 027ab73c484cbf598cd95bceadbddaa62c6e4c54fe9e85e7b03129e0331f048b | unchanged |
| port/scene-materials/shader_sources.cpp | f05874e30bba3a3b1c8cec643e5fda6bcba4f31e9b935eae8e94c9872b41cde1 | unchanged |

## Post-pause bounded renderer recheck, 2026-10-08

This section supersedes the earlier renderer provenance following post-pause-source-impact.md. Completed at 2026-10-08T18:16:57+03:00. It updates exactly **115** renderer-citing records: **105 unclear and 10 clone**. All comparison statuses, confidence values, original body/xref evidence, coverage totals and runtime not_run fields are retained. No new missing or matched verdict follows from the refreshed include/owner graph. Only lane-09.csv and lane-09.md were changed.

**Exact CSV subset:** 14957–14968; 14970; 14972–14989; 14991–15012; 15014–15050; 15052–15053; 16632–16646; 16648–16655. Of these, 92 rows cite the existing projection candidate at renderer:3080 and 23 cite the clock declaration at renderer:616. Their complete function-specific implementation/ABI/lifecycle comparisons remain unresolved. Original 14969/14971/14990/15051/16647 and every other lane row were outside the CSV mutation subset.

**Reviewed renderer subset:** header ownership declaration 331–339, existing steady_clock declaration 616, cleanup include neighborhood 1282, geometry-owner includes 2235/2236, authored-effect/batch-shader includes 2312/2313, batch-GPU include and owner selection around 2976, and the campaign projection-call neighborhood around 3080. Selected included bodies and two existing loading/release bindings were read only to check wrapper selection and retained owner transport. Newly added process-design delegates, movie interception/font reset, whole timer parity, shader algebra, unsupported parameter families, and broader original driver/material bodies were not re-audited.

### Bounded connection results

- **Cleanup selection remains the native sweep.** Current renderer:1282 includes renderer_native_cleanup_v50.inc. That unchanged body still validates the current context, retains allocation names from active ownership directories, and sweeps actual texture/buffer registries. Existing source_campaign_runtime_v61.cpp:586 and source_campaign_release_v88.cpp:392 bind actual loading/release receivers to clean_native_graphics_v50. The scoped active-source search again finds NativeDriverUnusedOwnerV50 only in its separate helper definition; the earlier narrower connected cleanup finding remains bounded and is not upgraded into a defect in these 115 rows.
- **Cleanup retention refers to allocation owners.** renderer_native_cleanup_retention_v125.inc:4–25 retains base Draw diffuse/alpha/vertex/index names and native_shader attributes/sampler names. Its geometry helper traverses meshes, compiled batches, actor/object batches/private images and skybox draws. renderer_menu_geometry_v123.inc:4–9 applies this to source_geometry_gpu_v64 and menu_preview_gpu_v123. The include exists at current renderer:2236. These observations check the owner directory used by the sweep, not every owner publication, failed allocation rollback or teardown interleaving.
- **Material comparison remains selected and retained.** Current renderer:2312 includes the unchanged authored effect scene. Its existing :165–174 program creation transfers the shared program owner; :183 copies part.retention into source_retention; :206 refreshes the actual reflected comparison directory; :230/:235 invoke equality/order. The included profile/material adapter and effect texture owner are unchanged. This confirms the previously claimed connected native adapter; it does not extend the single-pass/typed-domain comparison to the 115 unrelated unresolved original records.
- **Batch GPU owner transport remains selected.** Current renderer:2313/2976 includes the shader/GPU adapters. Draw at renderer:331–339 stores weak source mesh identity plus shared NativeBatchGpuStateV113. native_batch_program_v113.hpp:35–42 gives that state shared program/material/light owners and attribute/sampler name arrays. renderer_native_batch_shader_v113.inc:48–59 installs those same owners. renderer_source_geometry_v64.inc:39–60 stores weak world/level and shared program cache; :83–88 guards actual world/level/context generation. renderer_native_batch_gpu_v111.inc:3–26 checks campaign/current generation, records the reached GPU prefix and then prepares streams; :29–37 releases shader attributes and draw buffers before removing the map entry and cache-only programs. This rechecks selected owner/lifetime boundaries without proving all original IVideoDriver ownership callbacks or error/reentrancy branches.
- **Projection/clock citations remain candidates.** The current campaign projection wrapper at renderer:3080 still copies the retained view projection and passes the local GpuProjectionFieldsV12 zero tuple. The clock declaration at renderer:616 remains steady_clock. Include presence and these candidate anchors provide no new match for original log/RNG/virtual timer/base-driver records.

### Recheck fingerprints

All 14 files below were hashed at review and again before report update. **Zero changes during this resumed bounded recheck** were detected. Files without an initial lane fingerprint are newly fingerprinted owner-dependency observations, not retrospective no-change claims. Original 45-file snapshot entries outside this selected set are not a claim of fresh semantic review.

| Selected source file | SHA-256 at review and final recheck |
|---|---|
| port/android-native/app/src/main/cpp/model_renderer.cpp | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca |
| port/android-native/app/src/main/cpp/native_batch_program_v113.hpp | dc905f4aeac127346d53a28961cb6826186ceb5bd03695d0bd3c42f1b15bcd7e |
| port/android-native/app/src/main/cpp/renderer_authored_effect_scene_v5.inc | f1c22754d6a82ceb3bf85cbc4b9f05ce8cfcb64f00bd82fb322f993a0ac32a38 |
| port/android-native/app/src/main/cpp/renderer_effect_material_directory_v4.inc | c226d2ca9f9a3943433c454373cfcea1802064653a3deedaae1307cc7a3e0206 |
| port/android-native/app/src/main/cpp/renderer_effect_texture_v5.inc | a366fdc8df488c450197bde5512c68c18ed571f8e748b772daf4759d9f700391 |
| port/android-native/app/src/main/cpp/renderer_menu_geometry_v123.inc | cec9a3ecceff4b531daad1d14a62ed5c2b4983fba081feccd662527d15c17960 |
| port/android-native/app/src/main/cpp/renderer_native_batch_gpu_v111.inc | d1fcf4462c81f1afe5400e09bd4aa822e80ddaa77844073f8d9b4d2103daec00 |
| port/android-native/app/src/main/cpp/renderer_native_batch_shader_v113.inc | db5594805bb526b7d23b8f39ba6a41c2289d5d599fb502d25eb8b9e0ed6c538a |
| port/android-native/app/src/main/cpp/renderer_native_cleanup_retention_v125.inc | c44882f80a0bac2253690d00d45c9fd94124a2636f4b4dacc42f68a4feb6161b |
| port/android-native/app/src/main/cpp/renderer_native_cleanup_v50.inc | 54df48b41112553bf7bea64d5ead4701f67620e6acd512aa5464e11947896537 |
| port/android-native/app/src/main/cpp/renderer_source_geometry_v64.inc | c3715999f5979ef9f6b16fe8cbdbba62d56dd9430eb5b81b18ee15f7dbaa60fa |
| port/android-native/app/src/main/cpp/source_campaign_release_v88.cpp | d98ae5cebbd12db699de9c9e22d2bc2a621736e60624f6ffe7f7db43b3fb74b3 |
| port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp | 6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e |
| port/level-loader/native_driver_unused_v50.hpp | 4795a614d8c19c65a4c666d24d0713fee071f72c56b34f95df88079d6e292d3b |

**Report coverage after update:** 1,866 unique assigned IDs, zero missing/duplicate IDs, unchanged overall dispositions (23 matched, 122 partial, 1,295 unclear, 349 clone, 8 thunk, 69 out-of-scope), and 1,866 not_run runtime fields. Source provenance refresh and this bounded wrapper inspection leave semantic closure incomplete.
