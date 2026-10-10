# Lane 07: libDungeonHunter2.so records 11197-13062

Exactly 1866 assigned records, inclusive 11197-13062, addresses 0x4da830-0x54f77c. Unique IDs 1866; no gaps or duplicate IDs. All 1866 inventory records decompiled successfully. No failed-body or external import record occurs in this range. Internal thunks are distinct dispositions.

Disposition counts: clone=84, disconnected=1, import/thunk=284, matched=1, out-of-scope=139, partial=484, unclear=873. Runtime acceptance: not_run for all 1866.

Working tree read against README baseline HEAD 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. Only lane-07.md/lane-07.csv written. No builds/tests/emulator/install/external messages.

Complete manifest and structural disposition coverage is established. Full semantic completion is not claimed: unclear rows and partial families remain unresolved. The extractor read all 64,337 pseudocode lines across 1,866 bodies and ARM instruction summaries for all 1866 records. Substantive source comparisons are described below; candidate/address-reference source locations do not prove parity.

The xref pass read all 1,853,563 rows. Ordinary flow (type 21) excluded; code references to function entries and data/vtable refs retained. CSV excerpts bound incoming to 6/outgoing to 12/data to 6, while retaining totals. Indirect calls remain explicit and unresolved.

## Serialized records, readers and ownership

Records 11197-11603 cover generated Structs finalizers/destructors, arrays, readers and metadata initialization. Every reader row preserves its ordered readAs types/destinations and CString/array/call evidence. CSV destination numbers reflect the raw decompiler pointer expression; typed pointer arithmetic must be scaled before interpreting a byte offset. These give specific legal-input correspondences:

- CharacterProperties 0x4f2750 (11378) reads 224 ordered int words at offsets +4 through +896. port/game-data/data.cpp:28 loads 224 little-endian words per row and checks name/schema dimensions. Derived properties 11379-11421 tail-call that reader; they are individual thunk records.
- DesignSettings 0x4ee0d0 (11350) reads 43 four-byte fields, with int/float distinctions. port/game-data/design_settings.cpp:18 reads 43 raw words/172 bytes; its field-kind map agrees. Stream errors and partial mutation differ or remain unverified.
- ItemBase 0x4ec47c (11298) reads a CString, four words, one-byte Stackable and nine equip floats. Item-derived readers 11422-11434 add two words, a second CString and 20 words. port/game-data/items.cpp:33 consumes these fields; owning strings replace CustomFree allocations. Armor-derived thunks are separate.
- ItemPowerRef 0x4ecdc8 (11300) reads a signed byte, word, count, nested three-word AttrBonusEntry rows and five tail words. port/game-data/item_power_tables_v5.cpp:15 consumes the matching serialized layout, replacing 16-byte ARM vptr-bearing child objects with 12-byte serialized rows/native values. Its derived reader thunks do not add data.
- ClassFunc readers 11355-11366 have five ordered int reads. port/game-data/class_tables.cpp:95 traverses counted formulas with five ints per row. Source validation and transactional updates differ. Inventory/CharTemplate/StatAutoAssign candidates were left unclear because the formula decoder does not establish their individual readers.
- GameOption 0x4f213c (11372) reads seven words. port/engine-ui/game_option_table_v1.cpp:17 decodes seven words/28 bytes after the 43-word settings and five-word difficulty prefixes.
- OpenableContainer 0x4fdb90 has shape iibiSiii; DestructibleContainer 0x4fe220 has iiiiiiibiiSiiii; Door 0x4fe050 has Siii; TriggerObject 0x4fd448 has iSii. port/level-loader/game_object_arrays_owner_v81.cpp:27 explicitly traverses all eleven groups, publishing typed door/trigger/openable/destructible rows and retaining other groups raw. Raw retention is not proof of their gameplay consumers.
- Skill 0x4ebeb0 (11297) has the counted display list, three bool bytes, two CStrings and scalar tails. port/game-data/skill_tables.cpp:18 consumes those exact byte/word/string positions; snapshots and limits replace the legacy allocation/reread domain.
- FX readers 11346-11347 and 11599-11600 include packed bools, counted steps/sounds and CStrings. port/game-data/effects_tables.cpp:36 parses these sequences and verifies their schemas. The full effect-resource/runtime consumer path remains separate.
- v2Quest 0x504a94 (11558) includes counted prerequisites/objectives/rewards, two embedded objectives, repeatable byte, fourteen script strings and tail fields. port/game-data/quest_persistence_v51.cpp:27 follows that layout. It validates types and strips a trailing CString NUL; exact malformed-input/partial-publication equivalence is unverified.
- Condition stubs share operator plus two words; literal subtypes are internal thunks. port/level-world/native_conditions_table_v69.cpp:33 consumes count, actual native stub array, three-word rows and type. Bounds/seven-kind validation and assignment lifetime remain explicit limitations.
- Command readers 11455-11541 have address-keyed actions in port/level-loader/script_data_schemas_v52.inc. port/level-loader/script_manager_owner_v52.cpp:21 consumes word/byte/CString/int-array actions. load_commands at line 30 peeks kind, publishes command C1 before data reading, retains data, calls Init and retains failure prefixes. Execution/callback receivers cross into earlier lanes.

Finalizers/destructors 11197-11259 reverse-destroy nested elements, free header-adjusted storage and, for finalize variants, clear fields. RAII/vector destruction alone does not prove identical child callback order or failed reread behavior. These individual rows remain unclear, thunk or verified instruction clone. Metadata constructor 11283 initializes reflection names/offsets and atexit cleanup; source schema tables do not prove its whole lifetime contract. Audio/AI and unpaired individual reader candidates remain unclear. Partial means a specific format/algorithm relationship exists, while whole-body parity remains open.

## L07-1: Japanese title formatting is disconnected

StringManager::parseEx 0x509aec (11636), directive ^t, calls Application::GetTitleString with a 32-byte title buffer. The exact IDA body is pseudocode/0050/00509aec.c. Its cross-lane callee Application::GetTitleString 0x3206d0 (1011), pseudocode/0032/003206d0.c, checks SavegameManager language and copies the literal Dark Quest 2 when language is 4. port/engine-ui/localization.cpp:73 instead rejects application language 4 inside parsed_string; line 160 repeats the rejection inside native_string.

Therefore a Japanese localized string reaching ^t cannot finish through these current source routes. This is a scoped title-formatting disconnection, with high confidence. Other parseEx directives have source implementations and required providers. This is not a claim that StringManager as a whole is missing.

Localization sources preserve nine-language/37-sheet metadata, 16-bit sheet count/length loading, symbol/pack lookup, color markers and number formatting, compiled by port/engine-ui/CMakeLists.txt. Source rejects embedded NUL/trailing bytes and counts beyond signed 32767; legacy preload/replacement/free/return semantics are partial. Application title/version calls are cross-lane edges. All runtime status remains not_run.

## L07-2: PropertyMap deliberately changes template semantics

PropertyMap::InitProperties 0x513d78 (11797), pseudocode/0051/00513d78.c, registers key template. port/level-world/canonical_property_map_v1.cpp:59 registers _templateName. PropertyMap::SetProperty 0x51387c (11792), pseudocode/0051/0051387c.c, invokes the descriptor FromString directly; current set_property at lines 129-139 routes a nonempty template-field string through set_template, adding descriptor cloning and LoadTemplate assertion behavior.

The source comments explicitly describe this as a modern editor-format compatibility correction. It may be desired for supplied assets; it is observable static divergence, so these records are partial with high confidence about the difference. No claim of a missing implementation is made.

GetPropertyMap 0x5134c0, SetTemplate 0x513fec, LoadDefaultProperties 0x5136ec and LoadOverridesFromXML 0x513a00 have counterparts in the same typed class/template map. LoadTemplate 0x51419c is an assertion body in the original and has a required provider in the port. Clone/save/descriptor destruction and actual subclass writers cross into TinyXML and object receiver lanes. Exact global nested-descriptor cleanup and all class factory closure remain unresolved.

## Compiled TinyXML and retained document identity

Records 11801-12014 have actual compiled TinyXML counterparts in port/level-loader/vendor/tinyxml, connected by port/level-loader/CMakeLists.txt:4 and XmlDocumentV1. Base downcasts return null; derived overrides are separate records. Exact ARM clones and thunks have independent dispositions.

TiXmlNode::Identify 0x519b10 (12006) selects declaration/comment/CDATA/element/unknown node kinds and sets parent/error behavior. Vendor tinyxmlparser.cpp has this identification/parser structure. Tree/clone/Attribute methods such as TiXmlElement::Attribute 0x515490 and TiXmlText::Clone 0x51756c have native counterparts. Whole fork/encoding/entity/error behavior is partial, not automatically matched from a common API name. TiXmlPrinterDH-specific bodies are unclear.

port/level-loader/xml_document_v1.cpp:80 bounds input and retains raw bytes, normalizes CR/CRLF only for the level-buffer route, parses once into the same retained native document, records parser diagnostics, projects unfiltered top-level node kinds and records actual native node identities. It rejects embedded NUL/depth/size excesses. Original LoadFromBuffer and current bounded document capture are not unrestricted equivalents. Immutable diagnostic snapshots do not themselves prove all callers respect backend D0 and borrowed node invalidation.

## Assets, scene utilities and actual batching

AssetManager::loadSceneNode 0x50a504 (11640) delegates to SceneManager::LoadScene. Retained/preloaded scene owners are present in port/level-world/scene_preload_owner_v81.cpp and scene_manager_map_owner_v2.cpp. AssetManager::loadTexture 0x50bdb4 changes retained texture refcount/priority through the original TextureManager; exact name map eviction/refcount closure is unclear. A resource-loader counterpart does not establish that contract.

BatchNodeCompiler::Compile 0x50dc84 (11705), Trim 0x50dbbc, MakeNoBatchVisible 0x50cfa8 and GOBatchSceneNode::updateSegmentVisibility 0x50d71c have actual counterparts in port/level-loader/level_batching_source_v96.cpp:53 and port/level-world/native_batch_resources_v110.cpp/native_batch_compiler_v111.cpp. The source compiler maps actual object/node identities, hides/restores names containing nobatch, compiles selected meshes, conditionally quantizes(false,true), flushes(true,false,false), transfers animator/material resources and trims work maps. Typed owner guards, budgets and failure domains differ; status is partial.

Scene utility records 11706-11751 include real traversal, materials, visibility/culling, projection, child removal and mesh copies. Debug-labelled methods are not blanket out-of-scope: they can call virtual methods or affect resource lifetime. Unpaired utility bodies are unclear.

## Navigation: real owners and limited algorithm parity

Records 12025-12291 cover floor/room/world/graph/object storage, mesh extraction, collision/height queries, flags, obstacle registries, validation, search and smoothing.

- PFWorld::_Init 0x522844 (12122) publishes the outer graph before allocating inner storage and stores initialized=1. PostLoad 0x5226cc (12121) only enters from 1 and stores 2 before room work. port/level-world/pf_native_storage_lifecycle_v106.hpp:44 preserves allocation/state prefix order; module_pf_room_v3.cpp:45 and floors.cpp:62 sew the same world room/floor storage.
- LoadRoom 0x523c14 (12155) publishes a room before mesh scanning, selects minimap/floor/exit names, loads floors and updates bounds. module_pf_room_v3.cpp:84 uses retained scene roots, actual mesh-name callbacks and floor clone/map owners. Generic floors::append rejects floortypes; the module_floor_append_v2.cpp path must be followed for actual retained property handling. That generic rejection alone is not evidence of a missing live feature.
- Room/world/floor collision queries use first-success iteration, bounds and type filters. navigation_world.cpp, navigation_motion.cpp, selector.cpp and published collision membership implement those legal-domain contracts with bounded arrays. Full overload/error/assert/profile parity is unverified.
- _SearchGraph 0x52cb90 (12278) has indirect goal/edge/node tests, sparse graph/list/priority queue work, repeated goal checks and an expansion limit. navigation_search.cpp:23 preserves repeated predicates, filtering, Dijkstra distances, SGI heap right-child tie order and external-prefix path insertion; navigation_world.cpp:51 supplies collision-to-node/direct-path/failed-cache selection. The five-argument test overload is distinct from the six-argument coordinate/object overload 0x52b560 (12270), which supplies direct-path/cache routing to FindPath/HasValidPath. Both native bodies and their actual caller signatures were inspected; they are not conflated.
- FindPath 0x52db48 (12284) drops old path, writes target, optionally records rolling PF_ProfileSearches timing, searches, smooths and calculates waypoint. navigation_path.cpp supplies drop/search/smooth/calc with the debug search-enabled provider. Original profiling timing, nonfinite/error branches and allocation/assert prefixes remain partial.
- Flying/swimming/obstacle setters and registries have numeric object counterparts in navigation_objects.cpp and avoidance/motion kernels. Actual script registration and every NPC caller are unresolved cross-lane closure obligations.

Game-specific sparse graph/AStar/search template bodies remain partial or unclear. Only ordinary STLport/toolchain helpers are out-of-scope for byte/ABI reconstruction. Their owning game/XML callers and child destruction are still recorded.

## Android, online, keyboard and Glitch GUI

Online records 12292-12309 contain real login/logout/score/state/network callbacks; complete same-owner source closure was not established. Keyboard records 12015-12024 have event/string-buffer ownership and EventManager dispatch; current edit-text/SWF/Android alternatives do not establish original registration/lifetime. These are unclear, not missing.

Android records 12310-12461 include ADevice, application state/input/pause/update/render, JNI resources/device/media/purchase/music and NVThread. Short Java dispatch bodies are actual wrappers, not imports. GLMediaPlayer_nativeInit 0x531ca4 caches methods/signatures; GLResLoader_nativeInit 0x5324c0 caches resource methods; DungeonHunter2_nativeInit 0x5325c4 caches background/browser/trophy/device/purchase methods. JNI_OnLoad 0x53224c forwards VM to Vox/NVThread and returns JNI 1.4.

port/android-native/app/src/main/cpp/native_app.cpp has the connected com_example_dh2_NativeBridge surface. It does not prove each original global mEnv/cached ID/class/global-ref/thread lifecycle equivalent. NVThreadGetCurrentJNIEnv 0x533d20 uses pthread TLS/AttachCurrentThread; SpawnProc 0x533e4c frees spawn data, gets env, calls user callback and detaches. These remain unclear.

NotifyTrophy 0x532b6c (12407) invokes the cached Java callback. port/android-native/app/src/main/cpp/source_android_notify_trophy_v114.cpp:8 reconstructs DEX persistence into the 100-line androidTrophy.dat, including caught exceptions and id==100 behavior. Native file behavior is partial mapping; JNI ownership is different. The DEX call is a lane20/callback-lane edge.

Glitch records 12462-13062 contain factory/process heap and GUI element layout/hit testing/focus/children, environment fonts/factories/XML serialization, file dialogs, bitmap/TT fonts, images/faders/list boxes/menus/mesh viewers/message boxes/modal screens/scroll bars/skins/spin boxes. All have body/ARM/xref evidence. Nontrivial widget bodies include virtual events, timers, image scanline parsing, drawing and attribute serialization. Current SWF/native UI alternatives do not prove absence or non-use. Exact current widget/factory/heap behavior is unclear where no semantic counterpart was established; virtual adjustment thunks retain separate target dispositions.

## Explicit remaining limitations

Every unresolved record is named in lane-07.csv. The large unresolved families are generated metadata/nested teardown, complete AssetManager refcount/eviction, unpaired scene utilities, online callbacks, keyboard registration, legacy JNI methods/classes/thread ownership and Glitch widgets/factory/process heap. Partial readers do not prove malformed-input/failed reread/error return/virtual teardown/live consumer equivalence. Partial navigation/batching/property paths do not prove all indirect calls, overloads, profiling/debug/failure/retirement orders.

All-body structural reading is not an exhaustive human semantic comparison of every long GUI or metadata body. Family/address source references are candidate evidence, not blanket implemented labels. No source absence claim was made and no textual miss was promoted to missing. Existing checkpoint labels/test reports were not used as integrated gameplay acceptance.

The full semantic completion gate remains open. This lane supplies exactly all assigned record dispositions and substantive family comparisons, with the limitations above. Source hashes below identify cited working-tree content and changes detected between review snapshots; later changes require revalidation.

## Source fingerprints

- port/android-native/app/src/main/cpp/native_app.cpp sha256=a24f348fafea17e488bf5b3cdf97f832651818f45e78077a59b3a6f5407ddef3; unchanged since citation baseline
- port/android-native/app/src/main/cpp/source_android_notify_trophy_v114.cpp sha256=a7268bae57de6fe090a40dac19332f42aa08884c4fa26507b2462391be503ba8; unchanged since citation baseline
- port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp sha256=5b22a0749372f06d2020a8644aa83aac396c8712ea2d65d500950988ed804c7e; snapshot recorded at report production
- port/engine-audio/audio_listener_rows_v38.cpp sha256=b6a2c3e47dba2ddad6f1bb3bf6f13b08527cf5c4038f7efd3bc2822e34869712; snapshot recorded at report production
- port/engine-ui/game_option_table_v1.cpp sha256=8ffd204eba23339675b635c415deb9cfa9f397d45fe264cbfc3142fc31f7debf; snapshot recorded at report production
- port/engine-ui/localization.cpp sha256=354f52a6193a48abee7eca37f7a807438060229eef60bb4d0828ff08b99e7d69; unchanged since citation baseline
- port/engine-ui/localization_parse_ex_v1.cpp sha256=6499289c94411aee561f6bfa27f0431386c9adf0948cd27fe9ec7c21e2d7cbfd; unchanged since citation baseline
- port/game-data/ai.cpp sha256=e36de5efc51972186aa9a480e55da72b1353fd4c05bbca2211441685881b2e01; snapshot recorded at report production
- port/game-data/animation_tables.cpp sha256=edf5d8fc71d225e975a4b7459a5ef2169ea3a84a8964b39290cfa9d91abc4b67; snapshot recorded at report production
- port/game-data/class_tables.cpp sha256=3960dbf2d35b895c3baa137cecf67dacd6fafb368352f74c89af4cc2f90d1595; unchanged since citation baseline
- port/game-data/data.cpp sha256=f7350be46f850c30a7d8f328adf48e03c91c1888c1f9df5c091077d75431642f; unchanged since citation baseline
- port/game-data/design_settings.cpp sha256=68b641c45a23b7d4ba55686a39506de470f0b2605b8c6f7293479d548ffb4dc0; unchanged since citation baseline
- port/game-data/effects_tables.cpp sha256=3c067f603ce7dada9f1d61664d32ed9f7e9215ab52ea888c2a915c5e58647c72; unchanged since citation baseline
- port/game-data/faery_tables.cpp sha256=ac646932288e65764ba7f66fe352966de734c56f6737df5f7bc18b47d91b1a79; snapshot recorded at report production
- port/game-data/item_power_tables_v5.cpp sha256=c391a2b2b7800306ccbb7c5bebf5f7d595e71a7224dd8c004f864efe37a580d6; snapshot recorded at report production
- port/game-data/items.cpp sha256=094bf9c52b35f65ba2829dbeee25cae68040271074c9a92a6d4478cfa4b53cfb; unchanged since citation baseline
- port/game-data/level_tables.cpp sha256=43e3e130983b6ec3fd9dc9b1c5db1894012143851b21a4ef92993e9392e24ca4; snapshot recorded at report production
- port/game-data/loot_tables_v2.cpp sha256=ba72e90ac1003fe2e931024b3a21a99231048e8788d449794d80fffbc609892a; snapshot recorded at report production
- port/game-data/quest_persistence_v51.cpp sha256=9577455afa63fd407824eaae908de06da366572d71f6841b2d18d01ac899423c; unchanged since citation baseline
- port/game-data/skill_tables.cpp sha256=85695e3a274dae96bb92abcfdd12bdfb24b4d9f910f0026a777a39ce6c5534dd; unchanged since citation baseline
- port/game-data/world_map_profile_table_v59.cpp sha256=ef17c0b10de120df54904775dfbe7645b75c14321acd1172dc0d3b6235f94a6e; snapshot recorded at report production
- port/level-loader/game_object_arrays_owner_v81.cpp sha256=edc343eaabaf22ba61643b4fa3c6d87f77e6e9671b8c26e3b00c3e785a73a31a; unchanged since citation baseline
- port/level-loader/level_batching_source_v96.cpp sha256=a71503fd5c9fdeba8c7df0bb8a4f24e3cc8bc5a178c262ad5e64b1baf0b40169; unchanged since citation baseline
- port/level-loader/script_data_schemas_v52.inc sha256=fbdb3d7b6480937b3b93817fca9f258829bd2fd234940f503f020c12f4e811a2; unchanged since citation baseline
- port/level-loader/script_manager_owner_v52.cpp sha256=263c7ac802be1a4747c81b904db0c37824aea031434aef965350b4305c825b42; unchanged since citation baseline
- port/level-loader/vendor/tinyxml/tinyxml.cpp sha256=9ee7ceb204f2f30fe6eb92294999a68437a872c70b0cbf69dc74ee1ce94b1cac; unchanged since citation baseline
- port/level-loader/vendor/tinyxml/tinyxml.h sha256=492ebfc96ca285fa63f50225d94883fab70776293c32cd3c6695096aa6cc74ae; unchanged since citation baseline
- port/level-loader/vendor/tinyxml/tinyxmlparser.cpp sha256=1dd6068847ee18697e559b7876136bcaaf3c9eca58c28863320e8a21c8f4aeb5; unchanged since citation baseline
- port/level-world/canonical_property_map_v1.cpp sha256=d2f32d25e001815237b74e1203a5927d912c8f13e744e41db177c0c9e26d4a72; unchanged since citation baseline
- port/level-world/character_combat_sound_tables_v2.cpp sha256=87f331f7f309afbe6ae77e0d54c000684a21075d37a06c8f2d658c1aa98c595c; snapshot recorded at report production
- port/level-world/floors.cpp sha256=632a85da3036037e1fa2ccec708d08966cccb5e7dbee09553844d10d12b9d344; unchanged since citation baseline
- port/level-world/module_pf_room_v3.cpp sha256=e17aa032895867792e075903dc1436482bac45ceafb2905ed7d5fc20913ecfde; unchanged since citation baseline
- port/level-world/native_batch_compiler_v111.cpp sha256=c26aad71e8f562130919b1ded16c93fb8228b9eb49cf7e1823bf6b2fae2d6ef3; unchanged since citation baseline
- port/level-world/native_batch_resources_v110.cpp sha256=79768da924ef977a1d206293421bb667c302958691c33868da7f43831a9354dc; unchanged since citation baseline
- port/level-world/native_conditions_table_v69.cpp sha256=05fd4783412da7af63e5055e2857bc389bb2691f3d22788a0ed1acf6b69167e6; unchanged since citation baseline
- port/level-world/navigation.cpp sha256=cc20d5df2f8b6b1501754b33ea16e5aacd3c35d916880ca868343994c12fdc91; unchanged since citation baseline
- port/level-world/navigation_avoidance.cpp sha256=d4d7d7de179858e702d2b3fd3b059f196456f51de5171208cffd16f63e568429; unchanged since citation baseline
- port/level-world/navigation_motion.cpp sha256=9d87dcb28d981a83d278f098807a9deb65e4b4491b49938fa3bdca87d1c1509f; unchanged since citation baseline
- port/level-world/navigation_objects.cpp sha256=daa5d8c5d117b0ebbfcc7c363259c66c58d6e29ce89776044bdd0a9ad4cedbc9; unchanged since citation baseline
- port/level-world/navigation_path.cpp sha256=4200d57fe3224e9bf423cdca11b34d22a080c868f57b1c7d68416a735b10ca36; unchanged since citation baseline
- port/level-world/navigation_search.cpp sha256=468666c49030bdfed67074356e1130579d651b8bfca5a3f35e2cdae79debdaa0; unchanged since citation baseline
- port/level-world/navigation_world.cpp sha256=0799b088893b4c774a56541acbb4593a4bb7c7dc343452f8ab136c3574025f0d; unchanged since citation baseline
- port/level-world/pf_flush_services_v1.hpp sha256=10d009e61ef7dbe1621bee87735717fd5c8a9890579d2e4309eff3935675f5e0; unchanged since citation baseline
- port/level-world/pf_native_storage_lifecycle_v106.hpp sha256=c761cf87a23d50e0ffb138efc03636e3711e01d158d5262415da198a2b3c4a8a; unchanged since citation baseline
- port/level-world/retained_visual_draw_v27.cpp sha256=de0204d33929500015b61321e14e97cba4b5171427e855842201f03837bf1cb2; snapshot recorded at report production
- port/level-world/scene_manager_map_owner_v2.cpp sha256=827c6ffcc927464fa33165b17f4d620d95608c6d552dc7eee3097cf5c6225963; snapshot recorded at report production
- port/level-world/scene_preload_owner_v81.cpp sha256=3e810580afc0159a8cd3cf23171cb31fbacae09f115d7ce53f70fbacf18a4c0d; unchanged since citation baseline
- port/level-world/selector.cpp sha256=312fa4adfa811188cb4df66b893826ecaafc795deba21d5ef38f21e3f984e4d5; snapshot recorded at report production
- port/level-world/trophy_manager_owner_v1.cpp sha256=0bcdb075cfbacb38db12293fbe6565c8ce9e545c9b4b3b5144949fb0aae2a0ef; snapshot recorded at report production
- port/level-world/visual_fx_tables.cpp sha256=cee00a2235e1f3ee7d21052c052687e406722b7522913021978e40f47d3d82db; snapshot recorded at report production
- port/scene-materials/scene.cpp sha256=5484076583d7fe9bab0085ed7ee14d31b7d43a3b30c08d7f2f936490f0f457a4; unchanged since citation baseline

Changed baseline source files: none detected.

## Raw identical-byte groups (not automatically semantic clones)

- code_sha256=9680d5a6b87f7a4292cbefd99cc48fe047c38a3e06418ed24c83b77c8983c876; records=11470,11933.
- code_sha256=e5e94a4281673bf204c3b0971ab160580e938f41cc554c7d50600ebf35dcd92c; records=11516,11518.
- code_sha256=fe5d976f12453812b97d188d1bc6872469e1ab7c77f4c3bdfceec9ac06b5f242; records=11550,11557.
- code_sha256=d306661edd8f13977bb0008a720e2708cb6194182d9f23e4e13309db966438d5; records=11563,11572,11577,11584,11591.
- code_sha256=5a02c365b4ffed8d77f69a09e7ebda6df65706d1fbb8631df5ecce54c66de6d1; records=11564,11573,11578,11585,11592.
- code_sha256=c23828df9bd56cae75f9be5d2affa69b69729f3ee95f49403ce411dd09a82a0d; records=11565,11574,11579,11586,11593.
- code_sha256=3850df701aee999be332c89b6215d0e76f2a99b9eb8596561d685867ab98a624; records=11580,11587,11594.
- code_sha256=cac29cf834710a6f287bd0ce740a5234b6b3b54ddbabc6beeeed73b2939fb2d5; records=11581,11588,11595.
- code_sha256=5c45d1a355482b4b0b9837ab40d26e5029e0de3d164ee130164c33a653956cf4; records=11582,11589.
- code_sha256=379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f; records=11638,11679,11680,11682,11686,11687,11708,11801,11814,11815,11816,11817,11818,11819,11821,11822,11823,11824,11975,11976,12018,12019,12025,12026,12032,12033,12110,12156,12292,12330,12331,12332,12355,12356,12357,12366,12367,12368,12464,12465,12523,12689,12690,12694,12696,12697,12741,12771,12883,12926,12927,12931,12961,12984,13019.
- code_sha256=045aa6297d96a7dec203f9709a0d94d8652a30324120bca39d8abf71ec1c414f; records=11689,12875,12915,12942.
- code_sha256=faa1ef5a6f0c3b7a1efb0f8bdef1aea63072055fa817aa198ce897b17ddd50f5; records=11690,12876,12916,12943.
- code_sha256=007f34a6c3441b0240da53f253e513b959105da2d8803255e1856aa48f48db47; records=11717,11848,11849,12046,12048,12333.
- code_sha256=9529704f8d2b5bfe8f1e9171d2e42cf4ffec97c722bd1e195ea0578b75c835ee; records=11764,11768.
- code_sha256=34b97ea285a49248d21f74e46f26848d3f2c1a90e381ca2c3fe219705331487e; records=11799,11850,11974,11984,12185,12285,12287,12288,12289,12290,12291,12300,12466,12551,12665,12711,12740,12767,12810,12860,12881,12903,12928,12956,13001,13038.
- code_sha256=6dcd5e75586fe6683156c0d559d4827b73bd48d501fa5781a1c6099aacd7c877; records=11802,11803,11804,11805,11806,11807,11808,11809,11810,11811,11812,11813,12365,12450,12687,12693,12695.
- code_sha256=adb591a7f69f280d3116791af6e43c0d420e841d275885417c7192bd9524cbb5; records=12016,12017.
- code_sha256=68a639b2dffac301817461b8b1e1e2fbca60e4b4db136a34b582411c8b7d17ba; records=12034,12036,12038.
- code_sha256=fdc1710f100ccf2b13937a27e7f23ccda5a1973826c8d8f3b7db09e6988fe8a4; records=12035,12039.
- code_sha256=9bfa0b0a7d6bdb9e0c33a0660fd40744052398e20ea35287c2b8db36da924e97; records=12075,12083.
- code_sha256=665ee4f4259cad0bc523b17094ca194e2d8820dec594f3bcf749a0c4cad51c33; records=12076,12084.
- code_sha256=b576532d6b53caa8f2bad02c29ba3ab863085da18345cfa566a53ed64c9154b0; records=12114,12116.
- code_sha256=98bb792172dff96ad2b6197bbf2e02cfaea39c9d760a998d2dbc8dd723edcfe9; records=12133,12140.
- code_sha256=9879104513590f286b79b500a9e4bbe63b2458d901b2a0d33737f1a2b787403f; records=12134,12141.
- code_sha256=73c7c3fb1c89478469e227b09c54da50f67abd72a9af58d2da768445a49b65f9; records=12136,12143.
- code_sha256=e5b12b14082da539ea3383730535c453133d45e7997a76c237df844bc56be267; records=12180,12183.
- code_sha256=4a197440c5abb259234e1a4dc146e361341faeb09d8b5a9e9588411cbd35d233; records=12245,12275.
- code_sha256=08a752174092790ff061ec8f7c70004cc7a62be30bd40257cbfc8170ab5313fe; records=12246,12276.
- code_sha256=f3288546307267a22f797d11ad049cf993a093cb681027aec8ee7e5f2e789a4c; records=12293,12294.
- code_sha256=b93040e87a184cd2bd198af1f63f3c76deef26b86fe382ea78e07bfeccd27d6b; records=12295,12296.
- code_sha256=ec95cdfc08e608b104887732ce7d994f9517ade60117b6ce1a429af7f962ade8; records=12370,12406.
- code_sha256=b7b60cd054c43d699a5424dc02a80154b55ee14a92479b05961cb8c616eda1c6; records=12387,12423.
- code_sha256=05caaa5518dc3ec488fa719e1c2d0f5b23d16f39d15bda0af1e61fd03a562f11; records=12391,12424.
- code_sha256=5efa27140b5e572a81b41c5ff8163e7ff02e6e10cbaa382ffdd48e397233a838; records=12405,12446.
- code_sha256=2bfcdb4723daff0eb57fbb8fb6b61b460ee494ab11dce33df0db5d80df9b25f2; records=12408,12449.
- code_sha256=dafb633c766d19fc0f39dfb1aba9364d2b4d741ed3d7e6e8a70a8673177ac947; records=12462,12463.
- code_sha256=3f9e9d7248f7a6ed5f09e6fd36610bc2a4fb8ff13fc18e4d9df464cda862a770; records=12517,12887.
- code_sha256=765ae85285d5a02dc525a945fc3ba029c88d7ca2f93838292b249de37594a784; records=12532,12543,12545,12604,12624,12626.
- code_sha256=daeeefbc1ab7b9e02443b6028e2e6062a014e7520eb27ba9afb75b5964a28c92; records=12636,12782.
- code_sha256=afb96fc44c9826f15b71580129992a005054bcf66f3fcbaffeb5e79cf0965928; records=12637,12783.
- code_sha256=a1b5b1dab996b308b82d813c5f8055c55cbdfe13e7ac4c03408b296e06db7a48; records=12670,12746,12776,12833,12862,12889,12966,13048.
- code_sha256=236cb65f0355b46071ceb10e3ec03bdac3eaf6144ab4fb76b6dff35af6966337; records=12671,12747,12777,12834,12863,12890,12967,13049.
- code_sha256=cc0fd7e68594548209965a888cb6dec97a78293fe563e66e8c3b776c97b4b4d4; records=12674,12753,12786,12839,12872,12895,12974,13054.
- code_sha256=7f8245341b58ec6ec3da51cba0a5beb6ffd1906cb3681ba0b2bdc007a5e7f37c; records=12675,12754,12787,12840,12873,12896,12975,13055.
- code_sha256=caf9ecb434891f282b7fc815074a1d9d7cd42be55b9e16925d76c0b5e9fb1c13; records=12686,12983.
- code_sha256=0efcbe12bd7d505235ffab66b191e312b762871400d6f0870fd53b7c997bdd8d; records=12688,12691.
- code_sha256=20bfc9fc7704d2f1c302cb4beb3f5d30c2a3e68508a77481fd9c85b3989a6675; records=12698,12699.
- code_sha256=c3d8dd417a3d369a931b36849140485a6c92ef4c3800386a0205b8982ff410bb; records=12796,13032.
- code_sha256=777571d8b4f6398f189c8b9c0f4584cedaf099528ecd11c965328e341e1a8c75; records=12816,12959.
- code_sha256=f7422c4693f2390a65eef484bc1691fe155ba7f2b49107cab7c8168b47f55a81; records=12817,12960.
- code_sha256=72eb508bacd1f9b1403d5742c44d4b1246104a58b221df4d635e0ab8194d5435; records=12868,12909,12937.
- code_sha256=a624a058c8fe2ea83788704ad45dfa757b2b260032efb79d1c31d3226ec334bf; records=12869,12910,12938.

## Subsystem counts

- android-jni-platform: 152
- assets-batching-scene-utilities: 115
- glitch-factory-heap-gui: 601
- keyboard: 10
- localization: 33
- navigation: 267
- online: 18
- property-map: 49
- serialized-structs: 407
- tinyxml: 214

Semantic clone check: exact bytes plus normalized ARM operands are required. PC-relative calls/thunks with different external targets are not grouped as semantic clones; all raw-byte members still remain listed.
