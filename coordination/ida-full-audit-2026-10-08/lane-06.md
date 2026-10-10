# Lane 06 — generated PyData loading and lifecycle

Primary coverage: **1,867 / 1,867** `libDungeonHunter2.so` records, indices **9330–11196 inclusive**, one row per ID in `lane-06.csv`. The first address is `0x4b0798`; the last record begins at `0x4da7c8`. All **1,867** assigned pseudocode files were read, all have decompilation status `success`, and there are **0 failed bodies** and **0 external imports** in this partition. There are **152 exported tail thunks**, separately retained as `import/thunk`. Every CSV runtime status is **`not_run`**. Static and historical component evidence does not prove current integrated gameplay acceptance.

The baseline is the working tree described in README (`HEAD 75a7c2fe3261403e841ffbeb46734e9e4e84e75c`, already dirty). Only this report and its CSV were written. No source edits, builds, tests, emulator launches, installation or external messages occurred. The first-pass hash appendix is supplemented by the bounded post-pause recheck below. No drift was detected within either captured review interval. Post-pause source changes are identified separately; a hash cannot detect an edit before its first captured read.

## Coverage and method

| Inventory family | Records |
| --- | ---: |
| Arrays record readers | 70 |
| Arrays names readers | 65 |
| Arrays skipNames tail thunks | 32 |
| PyDataArrays | 6 |
| PyDataConstants | 9 |
| Structs lifecycle | 1,313 |
| Generated static metadata teardown | 343 |
| STLport container support | 28 |
| Global initializer | 1 |
| **Total** | **1,867** |

Every body was structurally inspected for allocations, field offsets, base calls, reset behavior, array cookies and virtual cleanup. Each CSV row records the input pseudocode/code hashes, an exact body hash, category, original calls/branches and relevant virtual-table slots, exact source location or an unresolved reason, confidence and runtime status. Extra columns `semantic_status` and `body_group_canonical` retain the source comparison behind a clone/thunk disposition.

Exact body groups use the UTF-8 pseudocode from its first opening brace, including local declarations and exact call/vtable operands. There are **1,205 distinct exact bodies**, **310 repeated-body groups**, and **662 repeated records** before giving thunks their separate classification. Every record still has its own CSV row. Identical relative branch machine bytes do not establish identical `skipNames` targets. The no-op group is backed by the original `BX LR`; deleting destructor wrappers and C1/C2 copies are retained with their addressed callees and owners.

All assigned table readers were followed through their instantiated vtables, with `read` at slot +12, into the actual Structs reader bodies. The recursive wire inspection covered **71 registered table roots** (one neighboring table reader lies outside the primary range) and **91 distinct row-reader bodies**. Bool/char consume one byte, shorts two, integer/float four raw bytes; strings and counted nested arrays were expanded. The only structural descriptor discrepancy is 06-F1. `Listeners` retains the actual float field as `f`. Other descriptors sometimes transport float bits as raw `i`; that is not a byte-consumption mismatch, and the projectile adapter reads those raw bits explicitly.

Assembly was inspected where name-count branching, indirect callback ABI, constant-load error phases, name bounds, empty bodies, destructor variants or reverse cookie cleanup mattered. Direct xrefs do not capture every indirect call or relocation-based vtable use. Virtual slots supplement those rows; a lack of direct callers is never taken as proof of missing behavior. Xref cells report total incoming counts, up to eight distinct incoming owners, every distinct named call/branch target and up to four relevant virtual slots. Large constructor registration lists are also traced through the fixed descriptors.

## Static findings

### 06-F1 — StatListTable describes a counted vector as two fixed words

**Path:** `character_properties_pyarray.bin` → Arrays::StatListTable::read **#9382, 0x4b40a8** → `_ZTVN7Structs12StatListListE` at **0x96b768** (runtime vtable **0x96b770**) → read slot +12 → Structs::StatListList::read **#11288, 0x4eacc0**, a cross-lane 07 edge. The row reader consumes an unsigned length, allocates `4 * length`, and reads exactly `length` integers. ARM instructions `0x4ead3c–0x4ead58` allocate from that decoded count and `0x4ead64–0x4eadd8` loop over it.

**Source:** `port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp:25` declares **`"ii"`**. `source_process_arrays_v101.cpp:111–112` executes the descriptor. The original serialized shape is **`[i]`**. Native rows expose two word fields instead of one vector field. A list length of zero or greater than one consumes a different byte count and shifts or rejects subsequent data. This is a confirmed static semantic discrepancy, `partial`, high confidence.

**Current input limit:** the bundled `port/android-native/app/src/main/assets/data/character_properties_pyarray.bin` is 401,608 bytes with SHA-256 `516ba82f631174d4c0a24708342549b5993c402f68c2c1dabd5ddc9eea138784`. It contains 448 CharacterTable rows, three stat assignment rows, then four stat-list rows whose payload starts at byte 401,576. Their lengths are all **one**, with values **150, 151, 152, 149**. Both programs therefore consume eight bytes per current stat-list row; this data coincidence hides the vector-type error. This was read-only asset inspection, not a game run or differential test. No current `group("StatListTable")` consumer was located in the scoped production C++ search, so impact on the bundled scenario is unestablished.

### 06-F2 — the names-count mismatch branch is omitted by the process loader

**Path:** PyDataArrays C1 **#9515, 0x4be550** registers grouped name callbacks. Reload **#9506, 0x4bd478** invokes them on the same advancing stream. For representative Arrays::CharSoundsTable::readNames **#9331, 0x4b079c**, assembly `0x4b0828–0x4b0834` returns after the count word unless `table.size == names_count`. Its preceding `finalizeNames` already removed old names. On equality, it allocates each length+1 string and adds NUL. The equality guard appears in **all 65 assigned readNames bodies**. The **32 skipNames records** are real tail branches to names readers; they also retain the target disposition.

**Source:** `source_process_arrays_v101.cpp:105–109` clears old names, reads the count, and unconditionally consumes every name. It does not compare `declared_names` with `declared_rows`. A mismatched count can expose ordinals outside the record table, and a grouped stream advances differently from the original. The reader rows are `partial`, high confidence; wrappers retain `import/thunk`. Valid equal-count behavior, appended NUL/CString lookup and first duplicate-name ordinal are otherwise preserved. Runtime status is `not_run`; no integrated mismatched-count acceptance is claimed.

### 06-F3 — constants missing-file startup policy differs

**Path:** GSInit::Update **0x3850c8** calls PyDataConstants::Load **#9519, 0x4c3708**, at `0x3853ac`. The original 27-phase switch opens the exact file, reloads and closes when present. A missing open logs an error, increments the stage at +0x20 and returns zero. Stage 27 returns one without increment. The common increment is at `0x4c3804–0x4c380c`; missing-file branches rejoin it.

**Source:** `original_ui_session.cpp:660–678` retains the exact file ordering and ready phase. However, read failure at line 671 through the latch at 670 permanently sets `process_constants_failed_v101`. `source_process_trophies_v100.cpp:151–155` then fails GSInit. The missing-file continue behavior is not preserved. This may be an intentional native failure policy; the static audit records the difference, not an observed gameplay failure. Status `partial`, high confidence, runtime `not_run`.

### 06-F4 — initialization is connected; dynamic replay and exact lifetime closure remain partial

**Original:** reload **#9506** dispatches arbitrary registered filenames in order. RegisterClassByName **#9511, 0x4bdccc** replaces a stored callback, and addFuncsForFile **#9514, 0x4be3e0** appends `(read, finalize)` pairs. Table readers destroy prior members, construct cookie-bearing rows and invoke a virtual reader. Structs finalizers distinguish count/pointer reset from destruction; some destroy nested elements in reverse order using virtual slot zero. Deleting destructors additionally free `this`.

Concrete owned paths include LangSheetList finalize **#10572, 0x4ceb00**, v2QuestScripts finalize **#10649, 0x4d0238**, ItemPowerRef finalize **#10964, 0x4d573c**, and DialogStepList finalize **#11196, 0x4da7c8**. v2Quest destruction **#10651, 0x4d04dc** destroys five nested cookie arrays, then its scripts and objective subobjects. Exact pointer/count offsets, cookie strides, resets and addressed calls are retained per record. Pseudocode same-name destructor calls are resolved by the actual xrefs rather than treated as literal recursion.

**Source connection:** `OriginalUiSession::Impl` owns `SourceProcessArraysV101` at `original_ui_session.cpp:123`. `native_process_text_startup_v101.inc:12–14` binds constants/array loading into GSInit. `original_ui_session.cpp:680–698` loads and lends that same process owner. The C1's **142 record/name registrations for 71 groups** and **142 class/member getter registrations** are represented by native descriptors and compiled member names. GetOID **#9508, 0x4bd640** has a counterpart at `source_process_arrays_v101.cpp:75–89`: unknown class returns -1; known classes yield a compiled member or table-name ordinal; names retain first-match semantics. Duplicate ColladaFile type registrations use the same original getter and metadata, so first-versus-last insertion in the fixed native descriptor set is not itself a demonstrated current discrepancy.

Native ownership is implemented with strings, nested vectors, maps and shared/unique owners. The public process arrays API offers staged initialization and borrows. No arbitrary-file replay, callback re-registration, original per-Structs finalize API, cookie/vtable identity or exact reverse virtual teardown closure was established in the reviewed current code. These paths are `partial` when the producer schema was traced, and `unclear` when the nominal type has no connected replacement. A token miss is not proof of absence: a renamed or flattened representation may implement the behavior. Script execution has separate owners and is not deemed missing because a generated Structs data destructor lacks a same-named native class.

The scoped search read **2,567 current authored files** under game-data, level-world, level-loader, script-runtime, Android main C++, engine-ui and engine-audio, with `.cpp/.hpp/.h/.c/.inc` extensions and excluding reference, reports, tests and vendor snapshots. It covered **329 nominal Structs types**, of which **85** had an exact token lead; individual unclear rows record their type and token-file count. DataReloaderManager and reloader-mutating behavior were also searched in production port code. The conclusion is unestablished closure, not blanket missing implementation.

## Positive comparisons and cross-boundary edges

- GetConstant **#9535, 0x4c4bdc** maps to `port/script-runtime/script_constants.cpp:60`. It preserves signed values, two-level const lookup and miss → zero. Lua `_GetPyCst` calls it at `0x37f480`; character, trigger and audio callers also cross this boundary. These bindings provide static evidence only.
- GetConstantName **#9534, 0x4c4b08** maps to `script_constants.cpp:74`, returning the first ordered key for a repeated value and the empty literal on miss. NativeGetNextDialogMessage's call at `0x450158` crosses this boundary. `source_script_ui_world_v97.cpp:49–54` gets DialogStyles from a retained design owner. Whole process-versus-campaign constants owner identity is not proven by this individual helper comparison.
- Constants reload **#9539, 0x4c540c** maps to `script_constants.cpp:38`, preserving merge/overwrite, prior entries, partial assignments and name-stop semantics. `original_ui_session.cpp:672` supplies the Debug load/GetSwitch prefix. The native short-stream reject and allocation bounds are explicit differences from original stale-stack behavior. Destruction uses the retained constants unique_ptr deleter at `original_ui_session.cpp:97`.
- SpawnGroups table **#9452, 0x4b9060** reaches row **#11290, 0x4eb244**, wire `bi[iii]`. `source_campaign_spawn_groups_v108.cpp:70–81` borrows that process owner for names, definitions and choices, with a lifetime pin.
- ProjectileTable **#9460, 0x4b9ab0** reaches row **#11349, 0x4edbb8**. `projectile_process_rows_v112.hpp:8–59` selects raw fields from the process owner and pins it in callbacks. The adapter transports packed bool/char widths and raw float bits.
- Additional cross-lane row edges include v2Quest **#11558, 0x504a94**, v2CondAnd **#11569, 0x505970**, SoundAutoGen **#11526, 0x502af4**, and DialogStepList **#11273, 0x4dca68**. Their actual wire reader bodies were followed without duplicating their primary row ownership.
- Static `__tcf_*` helpers free original global Structs metadata strings registered by the PyDataStructs constructor. For example **#10057, 0x4c8bdc** is registered at `0x4ea5a4/0x4ea5b0`. Relevant native compiled member metadata uses `constexpr const char*`, with no identical STLport heap block to release. The helper ABI is `out-of-scope`; metadata names and consumers remain audited. The 28 STLport support records are likewise delegated to native standard containers, not called missing.

## Disposition counts

| CSV comparison_status | Records |
| --- | ---: |
| clone | 554 |
| import/thunk | 152 |
| matched | 5 |
| out-of-scope | 369 |
| partial | 407 |
| unclear | 380 |
| **Total** | **1,867** |

| Underlying semantic_status | Records |
| --- | ---: |
| matched | 160 |
| out-of-scope | 372 |
| partial | 731 |
| unclear | 604 |
| **Total** | **1,867** |

Clone/thunk rows preserve their underlying semantic disposition. No row is `missing` or `disconnected`, because those stronger negative claims were not established. No row is `failed-body`, because all assigned bodies decompiled successfully. Coverage read-back found **zero missing IDs and zero duplicate IDs** within this lane.

## Cited current source hashes and stability

| Repository-relative source file | SHA-256 |
| --- | --- |
| `port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp` | `fde21042364d703a50a51fb7785aaac3acd258e9d8519f7f55a4af80c3ec197a` |
| `port/android-native/app/src/main/cpp/source_process_arrays_v101.hpp` | `bd8f964640fa9964536a62fa8e6c932988ee03cbe0124981523b7004ec7ebf5d` |
| `port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp` | `a663cf0ae3dacbbafb1882774954877472b057b5ab6b3ea9f3453dc2cd607fb0` |
| `port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp` | `5b22a0749372f06d2020a8644aa83aac396c8712ea2d65d500950988ed804c7e` |
| `port/android-native/app/src/main/cpp/source_process_pydata_files_v101.hpp` | `2f7680294723e055d84b79b0cc55e95d371858ce267e9a696e157604de41982e` |
| `port/android-native/app/src/main/cpp/original_ui_session.cpp` | `d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18` |
| `port/android-native/app/src/main/cpp/native_process_text_startup_v101.inc` | `9d45986fca434ed751a0a07078c8f0acd7bde432ba44389a79b476ebf61fbc5d` |
| `port/android-native/app/src/main/cpp/source_process_trophies_v100.cpp` | `65dfa0fb2dc11d3b5689caa69a2d36ce89b48ba092dfc0036f784c91b5543fdc` |
| `port/script-runtime/script_constants.cpp` | `36cdc15228be9b7289a6536e6f915393f59ca918349f478300e08c1a36fdabde` |
| `port/script-runtime/script_constants.hpp` | `c81d952c14c5baa920d23ba094386eb56c8f50cea3e86439bee754576f8b9cfa` |
| `port/android-native/app/src/main/cpp/source_campaign_spawn_groups_v108.cpp` | `2af83b7815411985d49ed3b9f37b552f8284048a44640861aa97add52250e134` |
| `port/android-native/app/src/main/cpp/projectile_process_rows_v112.hpp` | `1f8db808906fdc90f25c7905875da621badee5270b0e41bb6a07b8e24611c8d7` |
| `port/android-native/app/src/main/cpp/source_script_ui_world_v97.cpp` | `e8b1bff049b815b4dcf7daf4d15b6595c2cb3d55de5f4ca10e2a314749452fd2` |

No changes were detected in the captured source hash comparison interval. Original code/export hashes and exact-body hashes are recorded per CSV row. Source file paths and numbers are current working-tree citations, not frozen-manifest assertions.

## Remaining limits

This lane establishes exact inventory coverage and a static comparison, not whole-game completion. Exact lifecycle closure for every generated derived Structs type, live reload/callback replacement, allocator/vtable identity and reverse destructor effects remains unresolved per row. Error and missing-file branches differ from native bounds/failure policies. The packaged stat-list input conceals one descriptor discrepancy. No runtime work was run, and historical fixtures were not promoted to integrated acceptance. The coordinator must apply the full audit completion gate across all lanes.

## Bounded post-pause recheck — 2026-10-08

Following `post-pause-source-impact.md`, **eleven existing CSV rows** were refreshed: **9350, 9444, 9508, 9517–9520, 9523–9525 and 9539**. Coverage remains **1,867 records**, with zero missing or duplicate IDs. Original-body hashes, comparison/semantic dispositions and all `not_run` runtime fields remain unchanged. Other rows were not semantically rerun in this bounded pass.

### Constants owner, load and cleanup

The eight constants/initializer rows now cite `original_ui_session.cpp` SHA-256 **`d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18`**, superseding the first-pass **`c4db25ae30ec4bc60ffda88594b8e724c10564c4b1520ee229972894a5b9dc76`** snapshot. Current owner construction is at :114, its stage counter at :120, and the deleter at :97 calls the unchanged native constants destructor. #9517/#9518 remain partial/clone-partial because the original DataReloaderManager relationship is unestablished.

**06-F3 remains supported.** Current load :660–678 returns the saved failure at :663. Its lambda at :670 latches asset failure (:671), Debug failure (:672) and negative parser status (:677). Original #9519 **0x4c3708** missing-open branches log and rejoin the increment at **0x4c3804**. The new array provider does not change this local failure-policy difference.

The unchanged parser at `script_constants.cpp:38` writes into THIS UI map at `original_ui_session.cpp:675`, with Debug prefix :672. #9539 remains matched for its bounded parser/prefix comparison. CharacterGameDesign's new provider handles **kind 1 array/member lookups**; kind 0 reads its separate snapshot constants at `character_game_design.cpp:54`. This change does not establish process/campaign constant-map identity. #9523–9525 retain allocator/vtable qualifications; UI teardown :1142 conditionally retires the array publisher independently of the constants unique_ptr. #9520 remains a compiler global initializer disposition, distinct from array publication.

### GetPyOID → process GetOID → AnimDict

**Original:** LuaScript::_GetPyOID **#2996, 0x37f5fc** captures the process arrays global, passes both authored strings, and calls GetOID at **0x37f728**. GetOID **#9508, 0x4bd640** selects the registered class callback. AnimDict getter **#2418, 0x364b80** scans loaded names in ordinal order and returns -1 for no rows or a missing member. The original wrapper/getter edges cross into lane 02; downstream RegisterAnim behavior remains a separate lane 03 obligation.

**Current source connection:**

1. `original_ui_session.cpp:123` owns the process arrays. Successful initialize publishes a weak reference under a mutex at :1167; lender :699–703 locks the same reference into a shared owner.
2. `model_renderer.cpp:969–972` supplies that exact pointer, `process_array_design_lookup_v101` (:944–950), and a shared lifetime pin to CharacterGameDesign::initialize. Failed borrow retains the existing no-provider construction path.
3. `character_game_design.cpp:51–64` delegates known original kind 1 groups outside its six local registrations. `registered_names.inc:132` includes **AnimDict**. The snapshot stores provider/owner at :99 before publication at :118. Unknown classes retain the registry's successful -1; known extra classes without a provider fail delivery.
4. `character_script_session.cpp:334` associates original callback **0x37f5fc** with `design_oid` (:237). Its query wrapper (:233) calls the two-string/value projection in `script_design_bindings.c:10–31`, then the snapshot delegate and process `get_member_id` at `source_process_arrays_v101.cpp:75–89`. A successful miss is numeric -1; delivery failure becomes `DH2_SCRIPT_REQUIRED_SERVICE_FAILURE`.
5. AnimDict data **#9444, 0x4b85a4** reaches ColladaFile::read **#11268, 0x4dc424** through its virtual read slot. The descriptor retains shape `S` at `source_process_array_registration_v101.hpp:139`, loading at **phase 29** (`source_process_pydata_files_v101.hpp:64`). Names load at **phase 64** (:99), via skipNames **#9351, 0x4b212c** → audited readNames **#9350, 0x4b1f94**. The connected names path still has **06-F2's count-mismatch difference**.

Packaged names/data streams each contain **1,447 rows**, consuming exactly 42,616 and 104,357 bytes. Static ordinal **1328** is `swampking_idle`, paired with `data/3D/characters/swampking/animations/swampking_idle.bdae`. The fingerprints below pin these inputs. This is static data inspection, not execution of GetPyOID, RegisterAnim or an animation-resource consumer.

### Timing and ownership limits

Neither lender nor renderer adapter enforces `ready()`. For array classes, `get_member_id` returns delivered -1 when `declared_rows == 0` (:83), and fails delivery for nonempty rows without names (:84). Complete process readiness is phase 70 with no failed latch. Publication precedes these load phases; a loan alone does not prove group readiness.

However, **the source startup path loads arrays before normal main-menu entry**. GSInit phase 3 waits for array completion (`source_process_trophies_v100.cpp:139–160`) before phase 4. Main-menu entry is phase 14 (:138); startup completes at phase 15 without failure (`source_process_trophies_v100.hpp:80`). `native_process_startup_v119.inc:270–292` installs this entry/readiness check, and the native frame gate at `native_app.cpp:330–338` returns while startup is incomplete. **No reviewed evidence establishes that Stage 17 GetPyOID actually runs before AnimDict names phase 64.** Early publication is not classified as a defect or disconnect. The legacy JNI loadWorld entry (:641–652) has no equivalent direct guard; its existence does not establish the actual Stage 17 boss initialization order. Selected initialization/restart timing remains unverified at runtime.

Provider lifetime has a concrete pin: CharacterScriptSession::Impl retains its design Borrow at :23 and resets ScriptOwner first in its destructor (:99). ScriptOwner::Session closes the VM at `character_script_owner.cpp:63–71` while that Borrow and its snapshot-owned process provider remain alive. CharacterGameDesign rejects replacement while a Borrow exists (:88), and failed initialization preserves the prior snapshot. UI teardown retires the publisher only when it still names that session's arrays (`original_ui_session.cpp:1144–1145`); existing snapshots retain their shared loan without automatically rebinding to a later session. This supports storage lifetime through ordinary VM close, while reentrancy, arbitrary live reload and original global identity across replacement remain unverified.

**Disposition boundary:** #9508 remains matched for its local ordinal/miss helper. The new character consumer is statically connected with the timing limits above. #9350/#9444 remain partial for existing count/reload/ABI qualifications. No row is promoted to gameplay acceptance or changed to disconnected/missing. Only this report and its CSV were updated; game source, builds, tests and emulator were untouched.

### Recheck source/input fingerprints

The files below were read and rehashed during this bounded pass. No drift was detected between captured reads and report update.

| Current source/input | SHA-256 |
| --- | --- |
| `port/android-native/app/src/main/cpp/original_ui_session.cpp` | `d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18` |
| `port/android-native/app/src/main/cpp/original_ui_session.hpp` | `4efa3ebdd7cb260492ad7be7817737a59fdd0c742b3149812f5141617b16388f` |
| `port/android-native/app/src/main/cpp/model_renderer.cpp` | `de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca` |
| `port/level-world/character_game_design.cpp` | `51444c7524394eba045744a2c7ddca90c5b2b8bb2ccbb8650aef99ef418bb8c8` |
| `port/level-world/character_game_design.hpp` | `7c640ec5c3abd50117d6fd33d43322516819f8462d77126bef13b91f181e705e` |
| `port/level-world/character_script_session.cpp` | `8ab0110f18e758f766e38213a411b0e978f5f5f2fef4f3702f4b148a763f630e` |
| `port/level-world/character_script_session.hpp` | `c29d1a90ed5648e942db68820afe0fc3c370bdbb9272f9ca8e426870e2a55633` |
| `port/level-world/reference/character-game-design/registered_names.inc` | `b0405c5c439a864cbf6a6f5291517922fb9d36ba58a0bd79cb7ef909ea41348b` |
| `port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp` | `fde21042364d703a50a51fb7785aaac3acd258e9d8519f7f55a4af80c3ec197a` |
| `port/android-native/app/src/main/cpp/source_process_arrays_v101.hpp` | `bd8f964640fa9964536a62fa8e6c932988ee03cbe0124981523b7004ec7ebf5d` |
| `port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp` | `a663cf0ae3dacbbafb1882774954877472b057b5ab6b3ea9f3453dc2cd607fb0` |
| `port/android-native/app/src/main/cpp/source_process_pydata_files_v101.hpp` | `2f7680294723e055d84b79b0cc55e95d371858ce267e9a696e157604de41982e` |
| `port/android-native/app/src/main/cpp/native_process_text_startup_v101.inc` | `9d45986fca434ed751a0a07078c8f0acd7bde432ba44389a79b476ebf61fbc5d` |
| `port/script-runtime/script_constants.cpp` | `36cdc15228be9b7289a6536e6f915393f59ca918349f478300e08c1a36fdabde` |
| `port/script-runtime/script_design_bindings.h` | `0f7377a43a4efdfc05817901ad4dcc5344b9a1e30fbcb5de57e50f393173efd9` |
| `port/script-runtime/script_design_bindings.c` | `d3fa2b17d974aec16a130ab9f6ffd547e89f5cc24b26f18e98f970ad8b931e0a` |
| `port/level-world/character_script_owner.cpp` | `c47182cc49c542411fc0f2316ea7d832f81dba13ad1f12d0de9051cf908eb3ff` |
| `port/android-native/app/src/main/cpp/native_app.cpp` | `a24f348fafea17e488bf5b3cdf97f832651818f45e78077a59b3a6f5407ddef3` |
| `port/android-native/app/src/main/cpp/native_process_startup_v119.inc` | `256c0b9ec77a720ad60d507309727428337aeb44d72fd1fba6b689cd9598cffa` |
| `port/android-native/app/src/main/cpp/source_process_trophies_v100.cpp` | `65dfa0fb2dc11d3b5689caa69a2d36ce89b48ba092dfc0036f784c91b5543fdc` |
| `port/android-native/app/src/main/assets/data/animations_dictionary_pyarraynames.bin` | `e5276ddf59cea81681a64da89db78378248dddb10fc3dea1c21f49abb19f9fe3` |
| `port/android-native/app/src/main/assets/data/animations_dictionary_pyarray.bin` | `faee362a0cbf3fdcf3b09e2b957482c87f90ce25bc68048d1dfc7c742838fca7` |
| `port/android-native/app/src/main/cpp/source_process_trophies_v100.hpp` | `3bf64a7c8353401b8d3b24caedbd933e33d724de9071d75716915c7e5e52d3b2` |
| `port/script-runtime/script_constants.hpp` | `c81d952c14c5baa920d23ba094386eb56c8f50cea3e86439bee754576f8b9cfa` |

The lookup chain and ordinary VM teardown were reread. Every transitive animation/resource leaf, Java launch variant, live process reload and finalizer reentrancy was not recertified. This refresh supersedes stale local owner citations without extending static coverage into runtime acceptance.
