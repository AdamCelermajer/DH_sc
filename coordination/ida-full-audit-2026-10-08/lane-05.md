# Lane 05 — libDungeonHunter2.so #7464–#9329

Inventory dispositions cover exactly **1,866 records**. Semantic comparison is **not complete**: the report leaves 750 exact source correspondences unclear and 466 partial. The confirmed mismatches and disconnected paths below are static findings. Every runtime-acceptance cell is **not_run**.

Baseline requested by the coordinator: HEAD 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. This audit reads the already dirty working tree, not a clean checkout of that commit. Only lane-05.md and lane-05.csv were written. No game-source edit, build, test, emulator, installation or external message was performed.

## Exact coverage and evidence depth

- Library: libDungeonHunter2.so; assigned IDs 7464–9329 inclusive; CSV rows 1,866; unique IDs 1,866; missing IDs 0; duplicate IDs 0.
- All 1,866 exported pseudocode files are present and have decompile_status success. Failed-body dispositions: 0. This range contains no external-import body; 49 assembly-confirmed tail/this-adjustment thunks retain import/thunk dispositions.
- All body files were read in a systematic scan that records body hashes, direct symbolic calls, branch and indirect-call-expression counts. Subsystem source bodies and relevant caller/callee chains were inspected; the detailed confirmed paths below received full body and, where needed, assembly comparison. The scan is not claimed to prove each unresolved method semantically equivalent.
- The range has 1,706 distinct raw machine-code hashes, but 1,744 distinct pairs of raw code and exact exported body excluding the signature. Grouping requires both. Of 122 redundant pairs, 103 rows use clone; other specific thunk/native dispositions take precedence. Every clone remains a separate CSV row with its own xrefs/source candidate.
- CSV direct edges include 688 distinct incoming cross-range function pairs and 3,509 outgoing cross-range pairs. The full per-row local caller/callee lists are retained. Relocated data references are not reliably represented by the supplied xrefs export; raw vtable/RTTI words supply 774 slot references for 482 lane records.
- A source owner candidate is not exact-method proof. Candidate-only entries explicitly say the correspondence is unresolved. A symbol/file-name search miss is never labeled missing. There are 0 missing claims.
- The initial full-range pass hashed and rechecked 72 cited source files. Two changed during that report generation: native_quest_runtime_v76.cpp and level-world/CMakeLists.txt; their affected local paths were reread. The bounded post-pause recheck rehashed those 72 files, found only original_ui_session.cpp changed since the retained appendix, and added seven caller/consumer dependencies (79 total). F05-03 and row #9176 provenance were refreshed; other row dispositions were preserved. See both source-change sections. Hashes authenticate the cited versions and do not prove the pre-read history or unreviewed semantics.

Inventory functions.jsonl SHA-256: 3922efb894cf09cee9690708bc3976eaf6bc1372f7a584808ad9583ef3d8ff40. Each CSV row includes its original code hash and a normalized exported-body hash.

| Comparison disposition | Records |
|---|---:|
| matched | 24 |
| partial | 466 |
| unclear | 750 |
| disconnected | 188 |
| missing | 0 |
| import/thunk | 49 |
| clone | 103 |
| failed-body | 0 |
| out-of-scope | 286 |

The matched status covers only the stated defined native domain and inspected local behavior; it provides no runtime or full-gameplay acceptance. Out-of-scope rows are compiler-emitted STL allocation/copy/ordering helpers, with element lifetime reviewed at native caller rows. Android literal no-op methods are explicitly recognized; they are not called missing.

## Subsystem record coverage

| Subsystem | IDs | Records | Matched | Partial | Unclear | Disconnected | Other |
|---|---|---:|---:|---:|---:|---:|---:|
| script commands and scheduler | 7464–7556 | 93 | 6 | 67 | 4 | 0 | 16 |
| level savegame | 7557–7585 | 29 | 0 | 4 | 19 | 0 | 6 |
| player savegame | 7586–7725 | 140 | 0 | 11 | 98 | 0 | 31 |
| quest savegame and settings | 7726–7800 | 75 | 0 | 5 | 55 | 0 | 15 |
| physical owners | 7801–7868 | 68 | 7 | 0 | 52 | 0 | 9 |
| visual owner | 7869–7945 | 77 | 0 | 9 | 40 | 0 | 28 |
| animation controllers | 7946–8022 | 77 | 0 | 3 | 55 | 0 | 19 |
| anchors | 8023–8050 | 28 | 0 | 3 | 19 | 0 | 6 |
| conditions | 8051–8098 | 48 | 4 | 1 | 37 | 0 | 6 |
| game events | 8099–8133 | 35 | 0 | 4 | 23 | 0 | 8 |
| objectives | 8134–8353 | 220 | 0 | 25 | 138 | 1 | 56 |
| quest state machine | 8354–8424 | 71 | 0 | 5 | 50 | 2 | 14 |
| rewards | 8425–8474 | 50 | 0 | 11 | 30 | 0 | 9 |
| procedural generation and XML | 8475–8760 | 286 | 0 | 17 | 8 | 129 | 132 |
| animated FX | 8761–8855 | 95 | 0 | 5 | 57 | 0 | 33 |
| SWF animation | 8856–8906 | 51 | 7 | 1 | 30 | 0 | 13 |
| multiplayer | 8907–8986 | 80 | 0 | 0 | 13 | 56 | 11 |
| object search | 8987–9031 | 45 | 0 | 15 | 17 | 0 | 13 |
| PyData arrays and reflection | 9032–9329 | 298 | 0 | 280 | 5 | 0 | 13 |

## Confirmed static mismatches

### F05-01 — LockCharacter loses case-insensitive All handling

Original #7507 Script_LockCharacter::Execute(bool, int) at 0x45dda0 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0045/0045dda0.c:1)) calls LC_API_STRCASECMP_0 at 0x45de38 and stores 1 in v2Controller.s_blocked when the target compares equal. Its vtable slot is _ZTV20Script_LockCharacter at 0x9689a8+0x10, corresponding to Execute virtual+8. The paired UnlockCharacter #7506 deliberately uses LC_API_STRCMP_0 at 0x45dcf4.

Current [port/android-native/app/src/main/cpp/source_campaign_script_execution_v96.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_script_execution_v96.cpp:391), SHA-256 1c9e57cdd9d71cbd1d0e46ef8d4b7d80d19b2425a4c6b8f53f3305b0461230aa combines Lock kind24 and Unlock kind25 and uses std::strcmp(name,"All") for both. Earlier world/environment/audio/AI/tutorial handlers exclude kind24, so they do not override the combined branch. The compiled command descriptor identifies kind24 as Script_LockCharacter.

Failing input path: an admitted LockCharacter target "all", "ALL" or another case variant takes the selected-object lookup branch in the current code; the original sets the process blocked global. No current asset occurrence or runtime reproduction was established. Disposition: partial; confidence high. [Assembly comparison](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/assembly-functions.asm:349387).

### F05-02 — Quest reward loop ignores the native false result

Original #8435 RewardList::Give(void) at 0x482920 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0048/00482920.c:1)) exits as soon as virtual+8 Give returns zero; the assembly at 0x482964 tests R0 and returns on zero. #8456 Reward_ConsumeLoot::Give(void) at 0x483034 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0048/00483034.c:1)) returns zero after both successful and unsuccessful consume attempts (vtable _ZTV18Reward_ConsumeLoot at 0x969a28+0x10). #8372 Quest::GiveRewards(void) at 0x48015c ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0048/0048015c.c:1)) delegates to RewardList.Give and then clears the quest reward-enable byte; #8396 Quest::UpdateCompleted(void) at 0x4814d8 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0048/004814d8.c:1)) calls GiveRewards before completion-transition/online handling.

The initial reviewed quest runtime (SHA-256 d21517b3ddbf0a676945031d7627be5d2e56ec66c4d05f7d5c922f937307c563) ignored the callback output given and skipped disabled rewards. The actual frame transport leaves given=false for type4 ConsumeLoot while returning operation-success true, confirming a connected caller/leaf mismatch at that snapshot.

During this audit, the file changed to SHA-256 6a20ba89baeef7ec66e67057fea0b24fee604760277e34a06f8e2a783c7f4762. The current [quest_reward_sequence_v108 helper](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/native_quest_runtime_v76.cpp:46) stops on source_result=false. The enabled [ConsumeLoot, Gold] path therefore now has the original short-circuit behavior in static source. The current [caller](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/native_quest_runtime_v76.cpp:237) still filters r.rewards by enabled8 before dispatch, whereas native RewardList visits every row and a disabled reward returns zero. A compiled disabled ConsumeLoot row before an enabled Gold row retains a different delivery sequence. Current asset incidence was not established.

Disposition: partial; confidence high for the initial mismatch and helper correction, with the disabled-row path still requiring exact parity acceptance. No tests or runtime were performed by this lane. The source-change flag prevents treating the earlier body review as an unchanged final snapshot.

### F05-03 — Process array missing-file failure policy differs

Original #9176 PyDataArrays::Load(void) at 0x4aa21c ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004aa21c.c:1)) opens one of 70 named files, logs a missing-open error, then advances this+0x38 and returns incomplete; stage70 returns complete. Its caller #3143 GSInit.Update at 0x3850c8 repeatedly calls it during startup stage3 until complete or a 49ms budget expires.

Current [port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp:91), SHA-256 fde21042364d703a50a51fb7785aaac3acd258e9d8519f7f55a4af80c3ec197a preserves the 70-file order but converts read failure into sticky failed_ without advancing stage38_. [port/android-native/app/src/main/cpp/original_ui_session.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_session.cpp:680), SHA-256 d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18 directly propagates that failure to process startup. Missing-file startup behavior is therefore different. This may be an intentional stricter failure policy; it is a partial parity result, not an absent loader claim. No missing-file runtime case was run. Confidence high for control-flow difference.

The post-pause caller chain remains explicit: [native_process_text_startup_v101.inc:14](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_process_text_startup_v101.inc:14) binds the arrays callback to OriginalUiSession; [source_process_trophies_v100.cpp:154](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_trophies_v100.cpp:154) fails GSInit stage3 if that callback returns false. Its fail helper at :104 makes startup failure sticky. This is distinct from the original Load return of zero meaning incomplete and causing another load within the timer loop.

### F05-04 — Generic array names loading omits the original size guard

Rows #9319 LootTable, #9321 ItemTypeList, #9323 NumProbArray, #9325 MerchantTable, #9327 SkillListTable and #9329 ProjectileTable call finalizeNames, consume the name count, then only allocate/read name strings when count equals the table size. The five interleaved skipNames rows #9320/#9322/#9324/#9326/#9328 are exact assembly branches to the matching readNames bodies; they do not independently skip a byte count.

Current [port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp:105), SHA-256 fde21042364d703a50a51fb7785aaac3acd258e9d8519f7f55a4af80c3ec197a clears and reads all bounded declared names without comparing declared_names to declared_rows. Its registration descriptors preserve grouped file order and native reader identities. A mismatched count therefore has different cursor/allocation/publication behavior. This is a malformed/mismatched-data domain difference; equality for the current packaged assets was not established in this lane. Disposition partial; confidence high for guard absence. #9329 Arrays::ProjectileTable::readNames(IStreamBase *) at 0x4b0600 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004b/004b0600.c:1)).

## Disconnected paths and unresolved closure

### F05-05 — Procedural generation has declarations and no established production implementation

Original external caller #5628 Level.GenerateRandomLevel at 0x3f07f8 destroys any prior generator, constructs a new one, LoadRuleFile, Generate, unconditionally SaveModularLevelToStream, destroys it and returns the generation result. This closes through #8567 rnd::RandomGenerator::Generate(int) at 0x488134 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0048/00488134.c:1)), #8722 rnd::RootRule::Impl::Generate(Array2d<rnd::Tile *> &) at 0x48e8ac ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0048/0048e8ac.c:1)), #8731 rnd::Path::Impl::OneStep(rnd::Tile *, rnd::Exit const*, rnd::Exit const*) at 0x48fd64 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0048/0048fd64.c:1)) and #8753 rnd::Tile::Unspawn(void) at 0x4918a8 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/004918a8.c:1)). Serialization closes through #8575 rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer &) at 0x488b84 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0048/00488b84.c:1)) to #8760 rnd::Tile::SaveAsModuleXML(TiXmlElement *, int) at 0x491d90 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/00491d90.c:1)) with Module/PropertyMap XML, position/name/MGP/MVP properties and recursive children. Generation failure still reaches serialization in the original caller.

Current [port/level-loader/stage_loader_v38_root_file.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/stage_loader_v38_root_file.hpp:150), SHA-256 dac89ad05bd96f5b7760e2e12f010480e59a050be87994c28df1daf184cf9f94 requires a generation callback, then selects generated stream assignment or the original filename backup branch. [port/level-loader/procedural_layout_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/procedural_layout_v1.hpp:27), SHA-256 87c423c06b3497b877fa43196ac55b2144ca3cce15ce594118be25fc81ba37c2, [port/level-loader/procedural_rules_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/procedural_rules_v1.hpp:38), SHA-256 c3d54e0bbf1c03fe221374ac6343657ee68066f0b70a37676854bb2c705a59b2 and [port/level-loader/procedural_instances_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/procedural_instances_v1.hpp:23), SHA-256 e599bdcde6ea4ba7c1a3b7967b86859b718647bab9cc8ded1fe9cd9e48cf6142 describe retained planning/constructor APIs; the corresponding procedural implementation .cpp files are absent from this working tree.

The source/file searches covered the full project outside Git internals, then checked the concrete production selection. [port/level-loader/CMakeLists.txt](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/CMakeLists.txt:12), SHA-256 a9c0f2066450d8380a002b64bcff88bcd8a69d1a9c4023a56e11d266c489b16d still lists the absent standalone implementation files. [port/level-world/CMakeLists.txt](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/CMakeLists.txt:746), SHA-256 78e9bae6ecf3c3dd4fe31ff187654440246ce92fb4741e85edb55d2d5b285608 explicitly selects fixed-map native transport without a procedural implementation. A declarative interface or historical report does not prove current execution. Nontrivial generation/serializer rows have disconnected dispositions; RNG declarations remain unclear. No claim is made that an entire historical subsystem never existed.

### F05-06 — Online callback/state-machine closure is not established

Original #8986 OnlineGameState::Init(int) at 0x4a04c0 ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004a04c0.c:1)) constructs a MultiplayerStateMachine and installs idle/ingame/create/search/join/lobby callbacks plus message/event handlers. #8984 OnlineGameState::Update(void) at 0x4a039c ([body](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004a039c.c:1)) conditionally delivers processEvents and processMessages based on GetOnline.byte5, then runs the state machine. CMsgRaisedEvent #8940, HandleScriptCmd #8961, quest-sync/loot/attack/spawn and join/leave handlers carry actual cross-boundary gameplay effects; their per-row call targets are in the CSV.

Current [port/level-world/source_online_loading_owner_v55.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/source_online_loading_owner_v55.hpp:11), SHA-256 e96ed204bda5782ff5b113444742cfa9d20464ca3f9367d4a0171e4457d83860 exposes the retained COnline byte4/5 facet and explicitly does not claim transport queues/mutexes. [port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp:947), SHA-256 6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e supplies failing OnlineGameState34 services; [port/android-native/app/src/main/cpp/source_campaign_quest_frame_v108.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_quest_frame_v108.inc:112), SHA-256 df1ae38cf517fd619381e58a9721fda6c7e29600537460cd7ed7c1c4990b080f supplies failing online quest message leaves. [port/android-native/app/src/main/cpp/native_process_startup_v119.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_process_startup_v119.inc:328), SHA-256 256c0b9ec77a720ad60d507309727428337aeb44d72fd1fba6b689cd9598cffa excludes main-menu GameCenter effects under the current offline policy.

Scoped searches inspected online callbacks, COnline facets, front-menu entrypoints and quest/script network services across active source. They establish the missing production connection for nontrivial callback paths; they do not prove absence merely because an original class name is not present. Android return-only GameCenter API leaves and literal state helpers retain unclear/clone dispositions rather than missing/disconnected effects. Online backend integration and runtime acceptance remain unresolved.

### Other unresolved methods and branches

- Script manager loading and scheduling have current native owners; command factories, Data storage and actor/UI/AI/audio/environment leaves were traced. Only the specifically matched local methods receive matched. Full message-queue/AS receiver equivalence and network wire paths remain partial/unclear.
- Level/player/quest save owners reproduce section tags, object payload sizes, PlayerCharacter_0 selection, saved skill/faery/state widths and actual retained object dispatch. Native per-method file gates, every deletion variant, assertions, online gating and complete cold/malformed restore/teardown parity remain unverified.
- Physical controls and pin/filter leaves have inspected matches. Derived contact callbacks map to current canonical POCharacter/POItem/POProjectile/POZone owners; every virtual collision branch and native body-lifetime prefix is not proven.
- Retained visual resources/pose/bounds/modular skins have source implementations. Positive shadow/xray/material-rim branches explicitly require providers in retained_gameobject_visual_v1.cpp; empty constructor vectors do not prove the positive domains. Exact VisualObject methods, AnimSetManager cache/allocator/synchronized paths and callback retirement remain partial/unclear.
- AnchorBase/Forward owner behavior is present. AnchorGroup global registration, once-per-tick averaging and registry lifetime have no established exact current owner; Vector3DFList ordering and allocation are similarly unresolved. These are unclear, not missing-by-search.
- Current Condition/Event/Objective/Quest owners preserve selected literal conditions, retained stubs, event registration, saved quantities, markers and script transitions where examined. Full diagnostic formatting, all factory/deleting variants, all native virtual alternatives and source boundary lifetime behavior remain unverified.
- FX library, typed instance and resource-backed runtime owners exist. Full callback sequencing/reentry, pool deletion and positive resource/rendering domains remain partial. SWFAnim tooltip/HUD banks are actual generation-bound movie owners; generic multi-bank manager coverage remains incomplete.
- PyData process loader has exact grouped registration/native address metadata and compiled Structs member names. Its current retained rows/strings are successors rather than proven original independently callable global finalize/virtual8 cleanup/reverse D1/null-store order. Constructor/destructor/map order and every native table receiver remain partial/unclear.

Every unresolved record is individually named in the CSV. The 750 unclear rows and 466 partial rows require further exact source correspondence/deep comparison before semantic completeness can be claimed. Direct xrefs cannot resolve every virtual, function-pointer, lazy singleton or relocated data edge; vtables and inspected source bindings narrow that limitation but do not remove it. Historical static/host/Android receipts were not treated as current integrated gameplay acceptance.

## Source changes observed during review

Two cited files changed after the final citation snapshot and while the report was being generated. This lane made no source edits. The local affected paths were reread, and final hashes below replaced stale CSV/source citations.

| File | Reviewed SHA-256 | Reread SHA-256 | Effect on findings |
|---|---|---|---|
| native_quest_runtime_v76.cpp | d21517b3ddbf0a676945031d7627be5d2e56ec66c4d05f7d5c922f937307c563 | 6a20ba89baeef7ec66e67057fea0b24fee604760277e34a06f8e2a783c7f4762 | New reward sequence helper stops on source false; enabled ConsumeLoot sequence corrected statically. Disabled-row prefilter remains different. Full file semantics were not re-proven after the mutation. |
| level-world/CMakeLists.txt | 7f0e6314dd7e2e7afde9fce9a579d5f75e917b2c3f039cabf15f8ef78ec434b4 | 78e9bae6ecf3c3dd4fe31ff187654440246ce92fb4741e85edb55d2d5b285608 | Current production selection still explicitly excludes procedural implementation. New source/test target entries are outside this lane's actions and acceptance evidence. |

## Bounded post-pause recheck — process array owner and consumer

Scope: the source-impact report cutoff was **2026-10-08 17:32:11 Asia/Jerusalem**. This resumed pass reread F05-03 native #9176 and its original #3143 caller, current array failure forwarding, and the newly shared process-array design provider. Final source fingerprint snapshot: **2026-10-08 15:18:37 UTC**. It did not repeat the full 1,866-body comparison or promote existing partial/unclear rows. No build, test, emulator or game-source edit occurred.

**Local failure finding retained.** The current loader bytes are unchanged (SHA-256 fde21042364d703a50a51fb7785aaac3acd258e9d8519f7f55a4af80c3ec197a). OriginalUiSession now carries d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18, superseding the earlier c4db25ae30ec4bc60ffda88594b8e724c10564c4b1520ee229972894a5b9dc76 snapshot. Its current :680–693 wrapper still returns false at :691 after load_stage failure. The newly shared owner changes consumer closure and leaves the missing-open/stage-increment difference intact. CSV #9176 remains **partial**, confidence **high**, runtime **not_run**; only its caller/provenance evidence was updated. All other 1,865 rows are unchanged.

**New consumer connection recorded separately.** [OriginalUiSession:123](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_session.cpp:123) retains a shared SourceProcessArraysV101 object; successful initialize publishes a weak process reference at :1167. The process borrower at :699–703 locks and returns that object. [model_renderer.cpp:967](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/model_renderer.cpp:967) passes its exact raw pointer plus the shared pin into CharacterGameDesign.initialize. The delegate at :944–950 forwards kind-1 group/key lookup to SourceProcessArraysV101.get_member_id. [character_game_design.cpp:49](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_game_design.cpp:49) retains the provider owner in the snapshot; :59–63 delegates known original registration groups outside its six local registrations, propagating provider failure, and :99 moves the owner pin into the next snapshot. Therefore a blanket claim that the process-array lookup has no design consumer would be stale. This bounded reread is not semantic parity proof for out-of-lane GetOID #9508, AnimDict #2418/#9350/#9444, or the complete Lua/VM chain.

**Readiness and lifecycle limits remain.** Publication occurs during UI initialize rather than after stage70. The borrower and renderer construction path do not check ready(); [source_process_arrays_v101.hpp:40](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_arrays_v101.hpp:40) defines ready as stage70 without sticky failure. get_member_id uses per-group members_loaded/names_loaded checks, with a zero-row -1 success branch, rather than a global load-completion check. If process borrow fails, renderer continues with an empty optional provider; extra known registrations then fail delivery. These observations do not establish the actual ordering of live design construction and startup loading. [OriginalUiSession:1144](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_session.cpp:1144) conditionally retires only its own published weak reference; a retained snapshot can still pin the arrays object. The design snapshot reload guard at :88 rejects live borrowers, and [character_game_design.hpp:70](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_game_design.hpp:70) requires the Borrow to survive VM close/finalizers. Exact live-owner selection, group readiness at use, array reload/failure recovery, teardown ordering and actual VM finalizer pin retention remain unverified. No runtime acceptance is inferred.

The 72 retained appendix hashes were rechecked; only the UI session fingerprint differed. Seven additional files below record this bounded caller/consumer review. Source hashes observed before the resumed detailed rereads remained stable through final rehash. These dependency citations do not increase completed semantic coverage.

## Source SHA-256 appendix

CSV citations carry the reviewed source hashes. This appendix includes every CSV-cited file and additional finding/caller evidence. All 79 entries were rehashed during the bounded post-pause recheck; the initial two mutations and the later UI-session replacement are documented above. Rehashing untouched files does not repeat their semantic comparison.

| Current source file | SHA-256 |
|---|---|
| [port/android-native/app/src/main/cpp/native_process_startup_v119.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_process_startup_v119.inc:1) | 256c0b9ec77a720ad60d507309727428337aeb44d72fd1fba6b689cd9598cffa |
| [port/android-native/app/src/main/cpp/original_ui_item_tooltips_v104.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_item_tooltips_v104.inc:1) | 74695b027ce04f24cbf68177921f0536b327ee68d8b585baf796768e705c08a6 |
| [port/android-native/app/src/main/cpp/original_ui_session.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_session.cpp:1) | d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18 |
| [port/android-native/app/src/main/cpp/source_campaign_quest_frame_v108.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_quest_frame_v108.inc:1) | df1ae38cf517fd619381e58a9721fda6c7e29600537460cd7ed7c1c4990b080f |
| [port/android-native/app/src/main/cpp/source_campaign_quests_v76.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_quests_v76.cpp:1) | 469738ed2ed3fbeedb9c187a72d7e8c5aa823d771e41a6c976a7d13af90530bd |
| [port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp:1) | 6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e |
| [port/android-native/app/src/main/cpp/source_campaign_script_ai_v118.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_script_ai_v118.cpp:1) | 04f6e9c0423f3b7cdb26c927ce9ac212cbc17c3d97cb9de68818ff8f461f5d11 |
| [port/android-native/app/src/main/cpp/source_campaign_script_audio_v117.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_script_audio_v117.cpp:1) | 95bf7d6093d73d4cee4b2009789d9a6327edb84e035286502dff6321f074d99d |
| [port/android-native/app/src/main/cpp/source_campaign_script_environment_v120.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_script_environment_v120.cpp:1) | 95247d1cff6d675c89bcb97bd33142c9a6e47bf78f43556ceddc632126bad742 |
| [port/android-native/app/src/main/cpp/source_campaign_script_execution_v96.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_script_execution_v96.cpp:1) | 1c9e57cdd9d71cbd1d0e46ef8d4b7d80d19b2425a4c6b8f53f3305b0461230aa |
| [port/android-native/app/src/main/cpp/source_campaign_script_tutorial_control_v118.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_script_tutorial_control_v118.cpp:1) | 9b509ebbb9a46b063586215b1753c74e1f7af23260abf0334fa848103ee15711 |
| [port/android-native/app/src/main/cpp/source_campaign_script_tutorial_v118.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_script_tutorial_v118.cpp:1) | 28a10d2db9cf53309806f329cb2000b3eafc21ff7d5b49a5e577ae2dd69ba9ea |
| [port/android-native/app/src/main/cpp/source_campaign_script_world_v117.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_campaign_script_world_v117.cpp:1) | 360950f101a0971192b7d474fdc52e9db2adfea4e3efbb174a0ebc77bca98aa1 |
| [port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp:1) | a663cf0ae3dacbbafb1882774954877472b057b5ab6b3ea9f3453dc2cd607fb0 |
| [port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp:1) | fde21042364d703a50a51fb7785aaac3acd258e9d8519f7f55a4af80c3ec197a |
| [port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp:1) | 5b22a0749372f06d2020a8644aa83aac396c8712ea2d65d500950988ed804c7e |
| [port/android-native/app/src/main/cpp/source_process_pydata_files_v101.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_pydata_files_v101.hpp:1) | 2f7680294723e055d84b79b0cc55e95d371858ce267e9a696e157604de41982e |
| [port/engine-skinning/visual_skin_owner_v6.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-skinning/visual_skin_owner_v6.cpp:1) | 5e7e44c8a152cc705e7a4d11d02505622a745b73b9530d8d3e278ca635ce54e4 |
| [port/engine-ui/menu_status_messages_v26.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/menu_status_messages_v26.cpp:1) | 92505893df698221f21f9817505d14e4059d46c0bbaa5610d37b2066cce66d48 |
| [port/engine-ui/owned_hud_settings_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/owned_hud_settings_v1.cpp:1) | fec0cf75356b8c45883bfbb829af2ca7e17c6478437bdcac7edb7a53adb16eef |
| [port/engine-ui/swf_anim_tooltip_v104.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_anim_tooltip_v104.cpp:1) | c70595a4988eedcbaee8064717a6dbc473d4ab95f7db85b681953f4f588cc0f3 |
| [port/game-data/campaign_profile_files_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/campaign_profile_files_v1.cpp:1) | 1df2eec19d42342bdc65ee6c6f58d09f31ae5836b3927ca9382b5900f4617b89 |
| [port/game-data/player_save_load_owner_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/player_save_load_owner_v1.cpp:1) | 4ca2359647ada8cfe9ef66600787b78490b30d6a20e5bb5442524e390e934e6b |
| [port/game-data/player_save_quest_sync_v3.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/player_save_quest_sync_v3.cpp:1) | b4884c06e9a68caa60e20649789dabb93c1c2ab60c691ce799538cbcc042059d |
| [port/game-data/player_save_state_fields_v45.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/player_save_state_fields_v45.cpp:1) | b31fd83694f9009c5d451bd93b8c69e75a7dbbe63892c179793ac10c77aa024d |
| [port/game-data/player_save_write_owner_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/player_save_write_owner_v1.cpp:1) | 7983a1638f49177eaed024918ca385f4525beef2e40b787d03472d7ccb96f5ac |
| [port/game-data/player_savegame_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/player_savegame_v1.cpp:1) | ca713e12bd25307bca7b24c95714cec45bda1e198fdbe70e952315659ef91d43 |
| [port/game-data/quest_savegame_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/quest_savegame_v1.cpp:1) | cd4e231fbc172e0d681fa316406e842d7959d47312cb3b97a31fcc9839e57749 |
| [port/level-loader/CMakeLists.txt](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/CMakeLists.txt:1) | a9c0f2066450d8380a002b64bcff88bcd8a69d1a9c4023a56e11d266c489b16d |
| [port/level-loader/game_event_manager_v50.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/game_event_manager_v50.cpp:1) | 52cf18233e3d7820082bb85a0e2fb3c428ac124f73c519be80f519a892b16657 |
| [port/level-loader/game_event_runtime_v75.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/game_event_runtime_v75.cpp:1) | 72ae02a51b05a1ff0c1c618c49a7675c887c74f6fe1794f21fd22f0c9fb5cd27 |
| [port/level-loader/procedural_instances_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/procedural_instances_v1.hpp:1) | e599bdcde6ea4ba7c1a3b7967b86859b718647bab9cc8ded1fe9cd9e48cf6142 |
| [port/level-loader/procedural_layout_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/procedural_layout_v1.hpp:1) | 87c423c06b3497b877fa43196ac55b2144ca3cce15ce594118be25fc81ba37c2 |
| [port/level-loader/procedural_random_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/procedural_random_v1.hpp:1) | b46a6792189e0b1b5667d2eb34ab728dac5054e2bf711925ec5315c3ddbefe63 |
| [port/level-loader/procedural_rules_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/procedural_rules_v1.hpp:1) | c3d54e0bbf1c03fe221374ac6343657ee68066f0b70a37676854bb2c705a59b2 |
| [port/level-loader/procedural_sources_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/procedural_sources_v1.hpp:1) | ccd7f4c63e5cf4901a4b3fe908024c6de9e7741f9abf2a0429f2a3cb0402ddb9 |
| [port/level-loader/script_execution_control_v96.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/script_execution_control_v96.cpp:1) | e55b1d62bff36abd087a3ce8d8e5bf1ff53ae6924b8a66c10b86b45f773b5830 |
| [port/level-loader/script_manager_execution_v96.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/script_manager_execution_v96.cpp:1) | e2d26215cf9a29dbe823547ec9e5d651b66715a3c74c977fcf114833ba4d9763 |
| [port/level-loader/script_manager_owner_v52.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/script_manager_owner_v52.cpp:1) | 263c7ac802be1a4747c81b904db0c37824aea031434aef965350b4305c825b42 |
| [port/level-loader/stage_loader_v38_root_file.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/stage_loader_v38_root_file.hpp:1) | dac89ad05bd96f5b7760e2e12f010480e59a050be87994c28df1daf184cf9f94 |
| [port/level-loader/vendor/tinyxml/tinyxml.h](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-loader/vendor/tinyxml/tinyxml.h:1) | 492ebfc96ca285fa63f50225d94883fab70776293c32cd3c6695096aa6cc74ae |
| [port/level-world/CMakeLists.txt](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/CMakeLists.txt:1) | 78e9bae6ecf3c3dd4fe31ff187654440246ce92fb4741e85edb55d2d5b285608 |
| [port/level-world/base_index_animation_controller_v21.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/base_index_animation_controller_v21.cpp:1) | b424ee80875cca81baf938ec6c187f4bb6e96dafdc459a789f3cd9ad9de704a0 |
| [port/level-world/base_named_animation_controller_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/base_named_animation_controller_v1.cpp:1) | c5ce416da97b92874843c0f85b3594e96dcb3a6e56132fe2f3809b9a8c4c14a6 |
| [port/level-world/camera_anchor_owner_v75.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/camera_anchor_owner_v75.cpp:1) | 18d59408139f9659e96d2ed3c53af1be00cd28eb02cde5edff4394a53aeed0fa |
| [port/level-world/canonical_podecor_body_v49.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/canonical_podecor_body_v49.cpp:1) | 61435a7b355d3cec6ae93d15916e0c00eac41dcf8b6ee6bbd2de61151aa4ab5a |
| [port/level-world/canonical_projectile_physical_v112.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/canonical_projectile_physical_v112.cpp:1) | 74e59cef9782014d2eabd56f64c8966e4a9db25fe6de555fdfcbc1c9e23457c6 |
| [port/level-world/canonical_zone_physical_v82.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/canonical_zone_physical_v82.cpp:1) | 9f25b852836dc408786f1ac6f774f22e97beaeccbd87b753e653d5fb4396faba |
| [port/level-world/character_animation_instance.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_animation_instance.cpp:1) | 0f6dcb6d4c6cbcee389bce5437eaa0b925febffb521a05ee6f875182b45d285f |
| [port/level-world/character_fx_state_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_fx_state_v1.cpp:1) | 9524b8a833b249dad57d182fb42b7ba4b2b2cae76cd1ffcdbfbde6920e1d8fb1 |
| [port/level-world/character_generic_animation_registration_v62.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_generic_animation_registration_v62.cpp:1) | 474b90b08d0feaa05dd04cb44481cc910bc085172aeeaa2e3ece67bbdbeca1b9 |
| [port/level-world/character_mesh_fx_owner_v4.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_mesh_fx_owner_v4.cpp:1) | 9646837a1e87db48b4e10107cec58e950e5e80648287ec07bfbfefe30aa73754 |
| [port/level-world/character_target_search.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_target_search.cpp:1) | d019fe062f96977a7b00ff6b15a5db291071f643652ad74c51c19c23ce7177d4 |
| [port/level-world/character_world_physical_character_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_world_physical_character_v1.cpp:1) | 87235c55739165a8765b84e702fbcf37adc1f4f68b700e3ab83928ac5592971f |
| [port/level-world/gameobject_target_list_owner_v107.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/gameobject_target_list_owner_v107.hpp:1) | 1dd7de64c6fdef982c8252c030c3c0fd092040de2055f2b52bb25328341fae41 |
| [port/level-world/level_savegame_objects_v2.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/level_savegame_objects_v2.cpp:1) | 26fb0d7f719827d71b9cd893c9b69552ce2e545723315508e2b9b47872198ffe |
| [port/level-world/level_savegame_owner_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/level_savegame_owner_v1.cpp:1) | 4da4371a1a8d36b0a14b0d186f6f1a3f2ef8f8fff89517a47876d592584a246d |
| [port/level-world/native_conditions_runtime_v69.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/native_conditions_runtime_v69.cpp:1) | 644204680a967f33d3af984a92e5249d4b780f596ed20d6dc16a3e96f9dc3a71 |
| [port/level-world/native_physical_filter_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/native_physical_filter_v1.cpp:1) | abc13b1563bbf0f9100378e78506e39f085fae2c4fc49165cb08463a5d562c11 |
| [port/level-world/native_quest_runtime_v76.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/native_quest_runtime_v76.cpp:1) | 6a20ba89baeef7ec66e67057fea0b24fee604760277e34a06f8e2a783c7f4762 |
| [port/level-world/objective_gathering_registration_v11.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/objective_gathering_registration_v11.cpp:1) | c9e136d8720ac25ccbe7b026469d59e77b03e93acf6b768c488aaedf96b90fd3 |
| [port/level-world/physical_controls.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/physical_controls.cpp:1) | 104b8b409e40eb1e733388d8977b7c0dbe333dfd7b61fc9d17aef67dca77aea6 |
| [port/level-world/physical_lifecycle.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/physical_lifecycle.cpp:1) | 717afdf187fb884c44d1eb0dba0b2f957fcda8c0a613ccb3efd852808746ca3b |
| [port/level-world/player_save_collections_writer_v45.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/player_save_collections_writer_v45.cpp:1) | 1e5710e3e5db721017708b93b95dd8bbe7cea29f7bdf3cc45f350d4ed00dea02 |
| [port/level-world/player_save_inventory_writer_v45.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/player_save_inventory_writer_v45.cpp:1) | 84e4f7e5b86dde86b01b8e025f963a3d0a246bf2c1e633a1a39deb8694d02af2 |
| [port/level-world/player_save_metadata_writer_v45.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/player_save_metadata_writer_v45.cpp:1) | 754872925f81e15ca76d7d45a565976cd2fedc3490660944d7a4baaffae811b4 |
| [port/level-world/quest_talk_marker_v76.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/quest_talk_marker_v76.cpp:1) | 3ae0f51905ec0a86207e5e25b37ad341c9ca09f3d886c5f5eb06132d9ca5107a |
| [port/level-world/retained_gameobject_visual_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/retained_gameobject_visual_v1.cpp:1) | 8b5bab7038393ac5b5d438a592bc1d88216b968b7f2295f6f80fe70667246701 |
| [port/level-world/source_online_loading_owner_v55.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/source_online_loading_owner_v55.hpp:1) | e96ed204bda5782ff5b113444742cfa9d20464ca3f9367d4a0171e4457d83860 |
| [port/level-world/visual_anim_controller_owner_v4.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/visual_anim_controller_owner_v4.cpp:1) | 8aa3d0259938e8c8e2003be1f491c58c7cc6c14ce1350fed3ec10554da82c975 |
| [port/level-world/visual_fx_manager_libraries_v63.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/visual_fx_manager_libraries_v63.cpp:1) | de3d5ba697b2f263bd25b3ef5149f93b3aac1cdd1d5bf223d32ab89d95a5aa63 |
| [port/level-world/world_item_physical_contact_v4.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/world_item_physical_contact_v4.cpp:1) | 84952f04e7b18bb5f614aad57cb0e8ff3a684e228dae98b8221623266f67b839 |

### Additional bounded-recheck dependencies

| Reviewed source file | SHA-256 |
|---|---|
| [port/android-native/app/src/main/cpp/model_renderer.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/model_renderer.cpp:1) | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca |
| [port/android-native/app/src/main/cpp/native_process_text_startup_v101.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_process_text_startup_v101.inc:1) | 9d45986fca434ed751a0a07078c8f0acd7bde432ba44389a79b476ebf61fbc5d |
| [port/android-native/app/src/main/cpp/original_ui_session.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_session.hpp:1) | 4efa3ebdd7cb260492ad7be7817737a59fdd0c742b3149812f5141617b16388f |
| [port/android-native/app/src/main/cpp/source_process_arrays_v101.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_arrays_v101.hpp:1) | bd8f964640fa9964536a62fa8e6c932988ee03cbe0124981523b7004ec7ebf5d |
| [port/android-native/app/src/main/cpp/source_process_trophies_v100.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/source_process_trophies_v100.cpp:1) | 65dfa0fb2dc11d3b5689caa69a2d36ce89b48ba092dfc0036f784c91b5543fdc |
| [port/level-world/character_game_design.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_game_design.cpp:1) | 51444c7524394eba045744a2c7ddca90c5b2b8bb2ccbb8650aef99ef418bb8c8 |
| [port/level-world/character_game_design.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/level-world/character_game_design.hpp:1) | 7c640ec5c3abd50117d6fd33d43322516819f8462d77126bef13b91f181e705e |

## Output integrity

The CSV schema provides library, function number/address, mangled and demangled name, decompilation status, category, full supplied local caller/callee xrefs plus raw vtable slots, exact source location or explicit unresolved-source reason, comparison disposition, body evidence, confidence and integrated runtime acceptance. Extra columns carry code/body/source hashes and subsystem. All runtime cells are not_run.

This lane satisfies record-accounting output coverage; it does not satisfy the entire audit completion gate or prove complete IDA-to-source semantic parity.
