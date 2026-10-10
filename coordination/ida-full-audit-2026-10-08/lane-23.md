# Lane 23 — adversarial review of audit confidence

Read-only cross-cutting review, 2026-10-08. Eleven findings are recorded in `lane-23.csv`. This lane did not modify source or the existing checklist, build, run tests, install, launch an emulator, or send external messages. No integrated runtime acceptance is established.

The strongest concrete findings are six stale “missing” checklist rows across two active paths, an “implemented” row whose cited source does not satisfy its general criterion, and a demonstrated failure of raw byte hashes as semantic clone keys. The current Stage 17 ordering connection is present in source; its nested actor, script, container, resource and first-update closure remains a separate obligation.

## Scope and measured limits

This is a focused adversarial sample, not a review of every IDA body, callback, or source file. I read complete pseudocode for the ObjectBase serialization thunks/targets, the example destructor thunks, `DeleteAllSlotFiles`, `initSectionInfo`, the three Stage 17 native callbacks, `_LoadFinalInit`, the quest pack/save functions, and the FaeryList table/record readers. I inspected targeted regions of `_LoadProcess`, GameObject/Character binders, MenuManager initialization, and `AI_DoRangeAttack`; those larger bodies are not claimed fully reviewed. I followed the specific source owners described below. No global callback or failed-body closure count was measured by this lane.

Read-only aggregation of the earlier checklist found **1,156 rows, 549 distinct library/address anchors, all in `libDungeonHunter2.so`, and 1,156 runtime fields equal to `not_run`**. The native/DEX completion denominator from the full-audit brief is 37,813 records. The two figures count different things and cannot be used as whole-system parity percentages.

The checklist moved during review: its earlier snapshot had 534 implemented / 466 partial; the later snapshot had **535 implemented / 465 partial / 21 disconnected / 133 missing / 2 unclear**. LW-0066 changed from partial to implemented. The README later reflected 43 implemented / 5 partial among its 48 stage-flow checks. The six stale negatives described below remained unchanged when reread. Source and evidence hashes appear at the end.

## Findings

### 23-01 — raw byte identity is not semantic clone identity (P1)

IDA records #1583 at `0x33e0f0` and #1586 at `0x33e1e8` have the same exported `code_sha256`, `0ded1e40bf45877f3e1ebf69d486a944141c910157afc4f3813ea8d390a694ec`. Their eight bytes are the same: adjust R0 by 0x24, then branch to the immediately following function. Because the branch is relative to its location, one reaches **Deserialize at 0x33e0f8** and the other **Serialize at 0x33e1f0**. Assembly lines 51944–51947 / 52009–52012 and xrefs lines 62740 / 62825 prove the different destinations. Their target bodies respectively read visible/enabled fields and clear condition-tested flags, versus write those fields. The source preserves that distinction in `port/level-world/object_save_restore_v3.cpp:10-23`.

Another identical byte family includes the PhysicalWorld destructor thunk at `0x34bf7c` and Animator destructor thunk at `0x3663a8`, which reach distinct destructors. A representative hash cannot stand in for the other class's teardown.

At inspection, preliminary lane 01 classified #1586 as `clone` of #1583, while expressly recording `semantic_coverage=unfinished` and warning that equivalence/source parity was unproved. Its Markdown says IN PROGRESS. Preserve those unfinished qualifications; the clone label must not become completed semantic coverage at merge. Resolve final branch/data targets and receiver adjustment before grouping behavior.

### 23-02 — slot deletion “missing” claims omit the actual front path (P1)

CS-0041 / CS-0043 / CS-0045 search ApplicationSaveFilesOwner/SavegameFileGate and conclude no enumeration/deletion implementation exists. Original `Savegame::DeleteAllSlotFiles`, IDA #511 / `0x313fb0`, formats `dh2_%03u`, enumerates matching names and removes them (`pseudocode/0031/00313fb0.c:15-33`).

Current `port/game-data/campaign_profile_files_v1.cpp:58-87` implements directory enumeration, the same substring pattern and unlinking. `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp:748-752` calls it; line 968 binds it to `NativeEraseSaveSlot` with the front job-flush callback. This directly contradicts the scoped no-implementation evidence.

The source mapping does not establish complete parity: the port imposes slot/directory/regular-file restrictions, and its front flush guard rejects gameplay attachment. Those are bounded domain/lifetime questions. They are different from absence of slot deletion, and no runtime acceptance was found.

### 23-03 — quest serializer “missing” claims omit the campaign writer (P1)

CS-0131 / CS-0132 / CS-0133 cite the load-only `quest_savegame_v1` API and claim there is no quest serializer or save callback. IDA #7706 / `0x469454` chooses regular versus volatile QuestSavegame from the low mode bit (mask 1); #7758 / `0x46c6fc` saves three difficulty tiers; #7757 / `0x46c658` packs count, quest records and current/primary/act words; #7756 / `0x46c584` writes each index and delegates the body to `Quest::_saveQuestData`.

`port/level-world/player_save_collections_writer_v45.cpp:16-30` implements that collection shape, order, per-record callback and mode selection. `renderer_campaign_save_bindings_v45.inc:53-62` registers QEST on the campaign profile with the retained regular/volatile Save collections and `services_.quest_save_data`.

A serializer and bound section registration therefore exist. This lane has not closed the final `quest_save_data` producer, all same-owner identities, null-slot behavior, or integrated save/reload. Reclassify the absence claim and investigate those actual remaining edges.

### 23-04 — CS-0009 “implemented” exceeds its cited key contract (P2)

The criterion is that unequal prefix-related section names stay distinct and identical keys update. IDA #529 / `0x315904`, `initSectionInfo`, accepts a CString key and creates or updates its map entry, including reader, writer and context.

The row itself says the cited Level cache supports only INFO/OBJS fixed slots. `level_savegame_cache_v1.cpp:6-7,24-27` rejects other names and stores a fields pointer. The alternate `campaign_save_profile_v45.cpp:19-21` does use a map, but requires four-byte tags and registers writers. Neither cited route demonstrates general arbitrary prefix-related CString keys or both callback metadata records.

This is insufficient cited evidence for the stated “implemented” criterion; it is not a whole-project absence finding. Narrow the check to its supported tags or keep the general contract partial/unclear while following all Savegame owners.

### 23-05 — Stage 17 sequencing does not close its derived callbacks (P1)

LW-0066's updated narrow source classification is supportable. `003f6990.c:460-473` calls `_LoadFinalInit` in state 17 and `_LoadCharStates` in state 18; source `lifecycle_v36.cpp:27-28` preserves that order. `source_campaign_runtime_v61.cpp:545` installs the deferred transport. `renderer_character_campaign_v62.inc:392` installs the concrete object-loading input provider. The deferred body in `source_campaign_object_loading_deferred_v106.cpp:15-27,31-42` ultimately reaches `source_object_loading_v95.hpp:70-91`.

Original `_LoadFinalInit` (#5612 / `0x3efca4`) walks the actual ObjectManager map, resolves each handle/GameObject and calls **derived virtual +0x58**. The source has the corresponding live-derived dispatch. That outer connection says nothing by itself about every positive derived callback.

The historical receipts illustrate the distinction: `.local-inputs/dh2-stage17-error-logcat.txt:34131` fails in OpenableContainer's `GameObject::Update`; later `dh2-loading-progress-logcat.txt:3437` fails at SwampKing's `MarkAsFlying`. Both failures occurred within correctly ordered Stage 17. Current `canonical_destructible_container_v16.cpp:45,68` and `renderer_campaign_noncharacter_v69.inc:1000-1033` do supply the InitFinal/update/PODecor paths, so an asserted current container source absence would also be overconfident. Actual class selection/execution remains unverified.

### 23-06 — installed globals and resource PASS do not prove callable support (P1)

Original GameObject/Character binders at `0x38d7ec` and `0x3b56bc` register MarkAsFlying, RegisterAnim/RegisterActorAnim and GetPropHP/GetHP. The Lua **GetHP alias targets 0x3b6cd8**, returning three HP values, rather than native `Character::GetHP` at `0x3bd110`. Name-only matching would conflate different interfaces.

The source catalogs contain those names, but `character_script_session.cpp:187-190,337-349` installs explicit failing closures for unsupported bindings and records `installed=true`. A registered global can therefore fail exactly as the historical Stage 17 receipt shows. Current source has concrete adapters: `character_script_session.cpp:192-249,285-297,328` and `source_campaign_character_fsm_v101.cpp:721-752` route motion, dictionary registration and resolved HP to the actor.

The actual authored `swampking_core.luac` calls MarkAsFlying at line 546, then seven RegisterAnim calls through lines 230–237 / 550. Its first OnUpdate calls GetPropHP at lines 634–637. The positive closure depends on all of these callbacks, aliases, dictionaries, ownership and HP arithmetic guards. No current integrated execution through first update was found.

The existing resource audit reports 219 script files and 721 checks. Its hashes include the resource loader and old renderer snapshot; it does not hash/test these CharacterScriptSession/CampaignFsm callback bodies. Its resource success remains useful within that scope.

### 23-07 — neighboring data and pre-World ownership failures remain regression obligations (P1)

The historical startup failures were not confined to a missing stage dispatcher: V120 failed compiled Structs metadata (`act1-startup-v120-live.log:12`); V124 failed FaeryTable string length 234881024 at offset 0x55 (`act1-coordinator-v124-live.log:52`); V126 failed InfoHUD operation 3 (`act1-coordinator-v126-live.log:243`).

IDA `Arrays::FaeryListTable::read` (#9490 / `0x4bc12c`) allocates 12-byte records and invokes their virtual reader. `Structs::FaeryList::read` (#11287 / `0x4eab9c`) reads a count followed by an int32 vector. Current `source_process_array_registration_v101.hpp:47-50` correctly distinguishes preceding `[i]` FaeryListTable from following `iiiiSii` FaeryTable in the same file. `source_process_arrays_v101.cpp:54-72,103-127` keeps compiled member metadata and stream shape independent and enforces EOF. This explains why reviewing only the reported FaeryTable fault would omit the preceding group's causal dependency.

Original `MenuManager::Init` (#6736 / `0x42f304`) phase 4 calls InfoHUDManager/HUDControls initCachedChars (`0042f304.c:262-268`). Current `native_menu_postmovie_v62.inc:36-42` now reads saved options from process Application settings before a World exists. These current repairs are present. They remain concrete regression seeds for adjacent serialized groups, member getters, loaded-row consumers and exact owner lifetime; component EOF receipts do not establish full startup.

### 23-08 — 1,156 valid citations do not measure full audit closure (P1)

The earlier checklist's 549 unique anchors do not directly anchor the MarkAsFlying/RegisterAnim/GetPropHP callbacks or the two FaeryList readers above. They may cite some of them indirectly; indirect mention is not a measured per-record body disposition.

`validate_checklist.py:90-161` validates row identity, function/address names, file existence and line bounds. It does not compare semantics, resolve actual callers, inspect ABI/ownership, inspect whether registered closures are supported, or authenticate runtime receipts. I read this validator and did not execute it.

The current primary artifacts inspected were explicitly preliminary. Lane 01 emitted 1,866 rows with 352 import/thunk, 252 clone and 1,262 unclear at that read, but expressly left its internal semantic review unfinished. Lane 13 emitted 1,866 rows with 26 import/thunk, 160 clone and 1,680 unclear. These are versioned snapshots, not final audit totals. No unsupported completion assertion was found in their inspected status text.

The merge must require all 37,813 unique records and independently account for unfinished body, virtual, callback and data closure. A comparison-status label alone cannot complete a row whose own semantic field says unfinished.

### 23-09 — failed extern records are symbol-specific interfaces (P2)

IDA #31370 / `0xa3642c` is failed `__imp_asinf`, with null pseudocode and the hash of four zero bytes. `segments.json` places it in the nonexecutable extern segment; `imports.json` identifies asinf. Executable PLT #1 / `0x30dd88` reaches it by GOT transfer. The exported zero-byte group has **354 distinct extern records**.

Current `port/engine-math/math.cpp:229` calls platform `::asinf`. Lane 01 appropriately dispositions the PLT as external import/thunk. A missing native body/source-symbol search would be false evidence here, while one extern representative does not prove all of those distinct libc/GLES/ABI provider obligations resolve. Keep failed .text native bodies separate and review their assembly/edges.

### 23-10 — scope requires an exact branch and reachability proof (P2)

Current `source_campaign_character_fsm_v101.cpp:175-178` expressly fails generic NPC range redirect and network-send continuation. Original `CharAI::AI_DoRangeAttack` (#4850 / `0x3d076c`) has live receiver/state/target branches. No mandatory Act 1 encounter selecting that generic redirect was established in this sample.

The SwampKing asset has an OnTargetInRangedRange callback at lines 594–611 and phase-2 “ranged” behavior at 256–260, but that callback calls **DoSkill(2)**. Its textual ranged label does not prove it reaches the generic AI_DoRangeAttack failure site. Equally, choosing a melee player does not establish that all NPC ranged dependencies are irrelevant.

For the full binary audit, offline milestone exclusions must retain a disposition and precise rationale for each original branch. Trace actual class/table equipment predicates and callback targets before calling a positive branch mandatory, unreachable or accepted. Do not turn an error string into a reachability claim.

### 23-11 — the audited evidence set changed during review (P2)

`checklist.csv` changed from SHA-256 `022c1bcd0c401adac86b1d9a8b8e7ee9a35d91d103c7beef160323eb93e95103` to `656c5300d736236103a9e179c15e7852a219d0b6c152d00004dac8a425dedf89`. Its LW-0066 revision accounts for the +1 implemented / −1 partial shift. The original Stage17 evidence remained fixed, and no reviewed game source hash changed at final rehash.

The delivery tracker also changed during reading: observed hashes included `d73d24015d2333b0afaf73d91ab7a24d443fe562712ad8c6f308dbb880e5687c`, `913ea40bdfbcb05d16d58520c500f969f3fa44d1082cbb3b4686a97a476bc6c5`, and the appendix snapshot. This lane did not edit either file. Current tracker claims source-fixed/compiled callbacks while keeping gameplay unaccepted. Historical failure receipts cannot be attributed to the later source/build without replay.

Freeze or hash the evidence accepted at merge, recompute aggregates from that version, and record stale receipts separately from present source state.

## Remaining uncertainties

The final generic key/reader metadata contract, slot-delete gameplay job ownership/domain differences, QEST leaf body/provider identity, every admitted Stage 17 derived receiver, complete script dependency/callback closure, all neighbors of the sampled data streams, target platform import ABI/provider resolution, and mandatory generic ranged-path selection are not established here. Current integrated boot-to-Swamp/first-update/gameplay/save-reload acceptance is **not_run** for this lane. The scope of these unresolved questions is narrower than “missing from the whole port,” and broader than stage ordering.

## Hashes of cited source and evidence

These hashes identify the last comparison snapshot. They are not APK identity or runtime acceptance. The game source entries were stable between their hash snapshots and final rehash; moving checklist/tracker evidence is described above. The initial earlier checklist hash is retained there.

| Path | SHA-256 |
|---|---|
| `port/level-world/character_script_session.cpp` | `f1604fe5fca3eda25a0927c337111f96de73868c5f6a895c59222629b1692495` |
| `port/level-world/character_script_session.hpp` | `c29d1a90ed5648e942db68820afe0fc3c370bdbb9272f9ca8e426870e2a55633` |
| `port/level-world/character_script_owner_bindings.inc` | `0a428c0126d314d664a4af687619e0ce22de1ed6c9d1a56b1fb056fc0f814f14` |
| `port/level-world/character_script_assets_v1.cpp` | `cdf3aed71af8f2fddf50b68a10588688f4f4f2c422ac635df062f23c0f6a49b9` |
| `port/level-world/object_save_restore_v3.cpp` | `5b6080f6f9a564059bd717e7e76847251888a61e0655db4ade1d1803684663ad` |
| `port/level-world/player_save_collections_writer_v45.cpp` | `1e5710e3e5db721017708b93b95dd8bbe7cea29f7bdf3cc45f350d4ed00dea02` |
| `port/level-world/campaign_save_profile_v45.cpp` | `9e1743ef8967d0c3695ba09f33a1c7198a906a8c89681e6b7bb7c070a8424ba7` |
| `port/level-world/level_savegame_cache_v1.cpp` | `8c85d364b762f4725d21db51013333a34d3f95ac4c903e309e11189ab70f5d9e` |
| `port/level-world/canonical_destructible_container_v16.cpp` | `071ef899e0a5c1b7c60c6e4d6bb09b85b4292dca3365afca380f77e8dfa8441f` |
| `port/level-loader/source_object_loading_v95.hpp` | `0917143f5d717b3c09edc223274efb92b20b3409a06150942bc3388c1b8fde24` |
| `port/game-data/campaign_profile_files_v1.cpp` | `1df2eec19d42342bdc65ee6c6f58d09f31ae5836b3927ca9382b5900f4617b89` |
| `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp` | `f88ceb1ed519625d059b20fefdffabf78ff65282d1b707760d8f0adb42b8b3ae` |
| `port/android-native/app/src/main/cpp/renderer_campaign_save_bindings_v45.inc` | `5b47c4256bb7825fb244e877d9a835e8717955e7e13a10b5f25f564efbbf4e97` |
| `port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp` | `3c4ceb5d7cbe2210bd9edffddc9330c9e5521d4012a592472948f9b67f8f14ae` |
| `port/android-native/app/src/main/cpp/source_campaign_object_loading_deferred_v106.cpp` | `2c9093c06ffaa82563a0d51abf0c1d6393e7664bb45d3ac170febe418af453f2` |
| `port/android-native/app/src/main/cpp/renderer_campaign_noncharacter_v69.inc` | `f0dbe3cfa4bbe61bb1ee2c9266704b4b264c32629a2006aaea6547a004c04fa4` |
| `port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp` | `a663cf0ae3dacbbafb1882774954877472b057b5ab6b3ea9f3453dc2cd607fb0` |
| `port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp` | `fde21042364d703a50a51fb7785aaac3acd258e9d8519f7f55a4af80c3ec197a` |
| `port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp` | `5b22a0749372f06d2020a8644aa83aac396c8712ea2d65d500950988ed804c7e` |
| `port/android-native/app/src/main/cpp/native_menu_postmovie_v62.inc` | `d4fc4c9121b32df67f8fe04d897f7f4f44dcebe5a161acd4f7231cd05217cac3` |
| `port/android-native/app/src/main/cpp/model_renderer.cpp` | `68df536883d56f0cd74f5b47052daafd07491faf5a75eec901790fd26afbe015` |
| `port/level-world/character_hit_v6.cpp` | `d50a7963f8cf44d13cc2e0d54e239f15dc9f32ad82d5c8983760766ca88cd924` |
| `docs/act1-ida-audit/checklist.csv` | `656c5300d736236103a9e179c15e7852a219d0b6c152d00004dac8a425dedf89` |
| `docs/act1-ida-audit/validate_checklist.py` | `50a82097e9a851a4512b3db24384ad3da343a0ca195f48568cf67d00f6e34110` |
| `docs/ACT1-DELIVERY-TRACKER-2026-10-06.md` | `c75ec2ea5474cc628b8f5cf7bd3169109a9a9fbc2a96e95be189ffd6392b7b14` |
| `.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/ai/swampking_core.luac` | `c6ea4676f14bfa5340a7c3f5160490202742781326b779bb85f6542649c9781b` |
| `port/level-loader/lifecycle_v36.cpp` | `b56e4567a9add617b66e51fb14c1a8499cd5a620593c0d646ca14ec1d369523d` |
| `port/engine-math/math.cpp` | `04baafb69f3520bdee2a9097df8457903c9a3f04d3c1c66c5add870e06179ee6` |
| `port/android-native/app/src/main/cpp/renderer_character_campaign_v62.inc` | `ad287e056ae6117d991da925e80576433ec8f7292867e8098cf2a58e347cf27b` |
| `port/android-native/app/src/main/cpp/renderer_npc_attack_frame_v1.inc` | `674e06ae87697e20ec2dee5ccbcfa573364233073d9f97be8dd6a99f2b061030` |
| `docs/act1-ida-audit/README.md` | `a9d81f797808606a707a2415269129d7a880cb02a7e7701d092ebb8b934f36ba` |
| `port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp` | `6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e` |
