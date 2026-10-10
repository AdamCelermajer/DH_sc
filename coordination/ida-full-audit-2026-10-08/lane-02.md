# Lane 02: partial semantic audit

Assigned: libDungeonHunter2.so function numbers 1866-3731. Date: 2026-10-08.
README source marker: HEAD 75a7c2fe3261403e841ffbeb46734e9e4e84e75c; current dirty working tree read.

This is **not a completed full semantic audit**. The CSV contains exactly 1866 disposition rows, one per assigned ID, with zero duplicates. An inventory-complete ledger does not mean every body was inspected. Failed decompilations: #2627, #3535, #3538; all three read in full assembly.

## Exact coverage

- Body inspected/dispositioned: 466 assigned records / 198 code hashes.
- Explicit complete pseudocode or failed-body assembly reads: 128 assigned records, overlapping structural groups.
- Structural reads: 212 ABI thunks / 82 hashes, 100 BX-LR no-ops / 1 hash, 40 constant returns / 2 hashes.
- Body inspection still unfinished: **1400 records**.
- Targeted source dispositions: 64, including two unresolved bounded post-pause edge comparisons; other source comparisons remain pending.
- Runtime: **not_run for all 1866 rows**. No build/test/install/emulator/runtime launch.
- Dispositions: {'import/thunk': 212, 'partial': 21, 'matched': 9, 'unclear': 1461, 'clone': 136, 'failed-body': 3, 'disconnected': 24}.
- Review states: {'structural_body_inspected': 352, 'body_inspected': 111, 'pending_body': 1400, 'assembly_body_inspected': 3}.

Explicitly read IDs: 1866-1893, 1939-1947, 1951, 1958-1960, 1962-1967, 1970, 1975-1980, 1982-1987, 1995, 1999-2004, 2418, 2627, 2996, 3372, 3382, 3393, 3403, 3417, 3425, 3428, 3433-3455, 3465-3466, 3468, 3470-3472, 3474-3475, 3477-3478, 3489-3490, 3493-3499, 3505-3508, 3510, 3532-3533, 3535, 3538.

Unfinished body IDs: 1894-1911, 1914-1925, 1927-1938, 1948-1950, 1952-1957, 1973-1974, 1981, 1988-1994, 1996-1998, 2005-2008, 2010-2028, 2033-2047, 2049-2151, 2156-2164, 2166, 2168, 2171, 2174-2177, 2180, 2182, 2184-2195, 2198, 2201, 2204, 2207, 2210, 2213, 2216, 2219-2226, 2241-2242, 2245-2254, 2258, 2262, 2265, 2268, 2271-2290, 2294, 2301-2354, 2363-2372, 2377-2392, 2397-2412, 2415-2417, 2419-2442, 2444-2451, 2453, 2455-2458, 2461-2465, 2467-2482, 2484, 2486, 2489-2501, 2503, 2505-2508, 2512, 2514, 2516, 2518, 2520, 2522, 2524-2525, 2529, 2532-2552, 2555, 2558, 2562, 2564, 2566, 2568, 2570, 2572, 2574, 2576, 2578, 2580-2583, 2586, 2589-2592, 2595-2596, 2598-2604, 2607-2626, 2628-2656, 2658, 2660, 2663, 2667, 2669-2671, 2674, 2676, 2678-2687, 2689-2693, 2698-2868, 2875-2877, 2879-2918, 2923-2932, 2934-2987, 2989-2995, 2997-2999, 3004-3034, 3036, 3038, 3042-3048, 3051, 3054-3059, 3061-3073, 3075, 3077-3078, 3089-3090, 3093-3103, 3106, 3108-3129, 3131-3143, 3147-3154, 3156-3158, 3161-3177, 3180-3188, 3193-3194, 3199-3205, 3208-3214, 3217-3222, 3226-3237, 3239, 3241-3246, 3248-3250, 3253, 3263, 3265, 3267, 3269-3276, 3278-3280, 3283, 3286, 3288-3294, 3296, 3298, 3300, 3302-3303, 3305, 3307, 3309-3310, 3312, 3315, 3318, 3320-3324, 3326-3327, 3329-3331, 3333, 3335-3341, 3344, 3346-3367, 3369, 3371, 3373-3381, 3383-3392, 3394, 3396-3398, 3400, 3402, 3404-3413, 3415-3416, 3420-3424, 3426-3427, 3429-3430, 3432, 3456-3464, 3467, 3469, 3473, 3476, 3479-3488, 3491-3492, 3500-3504, 3509, 3511-3520, 3524-3531, 3537, 3539-3541, 3543, 3547-3550, 3552, 3554-3559, 3561-3562, 3564, 3566-3570, 3573-3575, 3577, 3579-3585, 3587, 3589-3598, 3600, 3602-3606, 3608-3622, 3624, 3626-3627, 3629-3637, 3639, 3641-3642, 3644-3650, 3652, 3654-3655, 3660-3665, 3667, 3669-3670, 3672, 3674-3677, 3679-3682, 3686-3696, 3698, 3700-3702, 3704-3713, 3715, 3717-3723, 3725-3728, 3730.

CSV function_body_review and source_comparison_review distinguish body inspection from source parity and structural-only dispositions. Clone/import-thunk statuses do not establish owning behavior.

## Physics: reached source prefix omission

Original #1874 (34c048) clears the old world, loads DebugSwitches, queries isTracingPhysicalWorld, creates a zero-gravity/sleeping b2World and registers four listeners. NativeWorld.load in port/level-world/physical_world.cpp:57 and Stage5 at port/level-loader/stage_loader_v46_physical.hpp:18 preserve this order for the original reached bounds. source_campaign_runtime_v61.cpp:599 passes the retained backend to that adapter. Original callers #5682 Level._LoadProcess and #6673 MenuMainMenu.SetupScene cross lanes; the menu-preview load branch was not fully compared.

#1883 ShouldCollide executes both owner tests before combining results. Original assembly 34c35c and 34c39c proves the second call runs even after a first rejection. Source physical_world.cpp:20 preserves that order, reverse peer/filter parameters and Box2D fallback for a null owner. #1876 instigator logic matches source physical_world.cpp:28, including static-mass branch and the same owner's virtual velocity query.

**Partial parity:** #1886 Remove, #1888 Result, #1891 Add and #1893 Persist all load/query DebugSwitches before owner checks. #1886 assembly resolves the misleading decompiler string to isTracingPhysicalWorld. Source dispatch at physical_world.cpp:92 and Result at :100 omit this prefix and expose no debug continuation. NativeWorld.Violation in physical_world.hpp:48 is empty, whereas #1889 consists of that load/query. These are scoped omissions on registered listeners. Actual campaign Level update invokes the same backend at source_campaign_runtime_v61.cpp:1352; Box2D listener registration supplies the callback path. Effects of omitted debug-file reads/failure handling remain unverified in gameplay.

Owner callback order, complementary instigator flags, fresh local point copies and Result(1,0) match. SayGoodbye #1878/#1880 are empty in original assembly and source. Multiple-inheritance and deleting-destructor ABI remain partial. Cross-lane dependencies: #1864 clear, #1370/#1371 DebugSwitches, #26059-26062 listener registration, #26083 b2World constructor and #26089 default filter.

## GameObject Lua: global and object method closure

Original #3428 (38d7ec) contains 86 registration calls, all extracted and inspected for name, bindFunction/bindMethod mode, callback and context. Original Character #4302, TriggerTrap #3797 and Door #5379 call it. Modern ScriptOwner skips initial method descriptors at the original null-binder gate (character_script_owner.cpp:129); returned objects separately install catalog methods through script_object_bridge.c:18.

The current NPC global paths for MarkAsFlying #3475, MarkAsSwimming #3474, IsFlying #3447 and IsSwimming #3446 are connected: character_script_session.cpp:193/:296 selects the source address; source_campaign_character_fsm_v101.cpp:738/:767 operates on the SAME actor.runtime.object; navigation_objects.cpp:31 changes/reads only flying bit 1 or swimming bit 2. Absent/nonboolean setter arguments do nothing. Original assembly preserves receiver R2, adds 1c8, then calls PFObject. Cross-lane PFObject #12161-12164 confirms those flags. This is a static global callback mapping; runtime is not_run.

**Disconnected object methods:** gameobject_lua_catalog.inc advertises those motion methods plus GetName, IsDead, target-list methods and other GameObject methods. Campaign NPC/player inputs in renderer_character_campaign_v62.inc:244/:255 inject SAME CharacterScriptObjects.services. script_object_bridge.c:10 captures self._this; script_runtime.c:534 dispatches that identity/address to the provider. CharacterScriptObjects.invoke at character_script_objects.cpp:54 accepts only GetID (38ebe4), GetTarget (3b6c7c) and GetPosition (38e700), then returns Unreconstructed scene Character method. Calling any other advertised method on a returned Character therefore reaches a concrete rejection. Exact authored asset demand for each method was not proved.

This rejection applies to inspected #3417 LOCK, #3425 UNLOCK, #3441 IsDecor, #3442 IsDoor, #3443 IsPlayer, #3444 IsCharacter, #3445 IsDead, #3448 IsInWater, #3449 IsOverAHole and #3454 GetName. Motion rows are partial because global callbacks connect while object methods reject. #3434 GetPosition and #3453 GetID match their static result/receiver projections, including identity as lightuserdata in script_runtime.c:49. Original GetSelf #3450 pushes UserData; its current NPC global has an explicit unsupported fallback.

Inspected target-list and Lua SetPosition callbacks also reach explicit unsupported globals in the current campaign. The scoped selector chain is renderer_character_campaign_v62.inc:244 -> projectile selector source_campaign_projectile_methods_v112.cpp:201 -> combat/audio selector source_campaign_combat_v115.cpp:312 -> previous/null -> character_script_session.cpp:348. The first selector covers two projectile addresses; the second covers combat and three audio addresses. It supplies none of the target-list/SetPosition handlers. Object methods reject through the provider above. Existing independent kernels do not establish callback closure.

## Target sorting: independent candidate mismatch

#3470 SetTargetListSorting (390548) accepts a numeric first argument, clears every current priority-queue result, then selects closest/frontal/no-sort. Assembly 3905a4-3905d0 proves clearing occurs before sorter stores. character_skill_native_v5.cpp:96 only assigns list_.sort; it retains list_.count and heap. character_skill_readonly_v6.cpp:10 includes that same implementation, and CharacterSkillNativeBindingsV6 delegates noncombat addresses to it.

The older development renderer instantiates that candidate at model_renderer.cpp:2667/:2724. This does not establish current campaign reach; current campaign target-list callbacks reject as described above. The candidate consequence is specific: Search -> SetTargetListSorting -> GetTargetListSize/Top can retain stale results, while the original list is empty. V5 also returns literal zero as the fifth Top value where original #3452 reads TargetInfo+10. Production of a nonzero original value was not audited, so that is a lead rather than a proven reached defect.

Original #3468 radial and #3490 rectangle search support userdata/coordinate/relative origins and optional default/named backups. The old candidate exposes a restricted argument/filter domain. Full kernels and live consumers remain unfinished.

## Filesystem: bounded mapping

#1960 ApplyFilenameHacks splits at cwd substring plus one character, rewrites .tga to ata/3d/textures/basename, lowercases suffix and restores the original prefix. Assembly 34ea34 confirms the unusual literal ata/3d/textures/. swf_texture.cpp:4 preserves that projection; original_ui_session.cpp:464 and front_ui_session_v87.cpp:591 call it. Mapping remains partial because other virtual callers and malformed-input domains are not closed.

#1999 FileHandle selects rb/w+b/a+b, routes .savegame through RES_PATH, optionally lowercases and retains a CFile reference. #2002 tries resources then data fallback. level_root_cfs_filename_v52.cpp:22 supplies a path projection; its production caller connection was not found in the scoped port search. This is partial/unmapped, not a missing function claim.

#1944 decodeObfuscatedData assembly changes min(requested,4-offset) leading bytes for offset<=3, using decreasing negative deltas. #1979 read and #1980 peek call it based on pre-read offset/CFile flag; peek rewinds actual read count. A broad port search found no mapped owner/producer/consumer, so those rows remain unclear. A textual miss cannot establish missing behavior.

#1962 IFileStream.skip has misleading decompiler argument placement. Assembly preserves aligned uint64 amount from R2/R3, obtains virtual tell in R0/R1, uses ADDS/ADC into new R2/R3, then calls virtual seek. Modern ABI mapping is unresolved. File existence, backups/deletion, stream size/permissions/path/ownership bodies read here still require exact source comparison.

## Failed bodies: full assembly dispositions

- #2627 VoxSoundManager.StopMusic (36a1a0): current24==-1 returns; the JAVA_SOUNDS branch writes Save_Current_Music_ID=-1 before nativeStopSoundBig(current); normal branch calls Stop(current,fade); common stores current24=-1 and previous28=old. audio_level_gameplay_v67.cpp:71 matches normal control fields. audio_application_manager_v42.cpp:204 reaches the platform branch, but renderer_campaign_audio_services_v68.inc:89 supplies an explicit required JNI backend failure. Saved-music global mapping remains unresolved. native_menu_preview_backend_v121.inc:26 supplies a separate explicit failing platform endpoint. Original callers include #2647 PlayMusic, #2981 Lua.PlaySound, #6510 menu FS_StopMusic, #6664 menu Hide, #6934 SWF NativeStopMusic and #7532 Script_StopSound.
- #3535 SoundEmitterD1 (3953e0) and #3538 SoundEmitterD2 (395490): set derived vtables; if active39c call Vox.Stop(sound390,0); free string378; call GameObjectD2 #3403. They have different code hashes because relative branch/PIC encodings differ. canonical_sound_emitter_v32.cpp:14 delegates whole_destroy from catalog_auxiliary_v67.cpp:276; the actual stop-before-base teardown provider was not established. Both retain failed-body dispositions with this explicit unresolved mapping.

#3532 InitPost's sound-name-to-ID scan matches the catalog snapshot. #3533 Update was fully read; original Level38/faery/distance/active-byte/Stop3D/Play3D branches remain behind an unresolved modern whole_update service.

## Lower-level GameObject position

#3506 SetPosition (393db4) matches game_object_set_position_v2.cpp:5 and canonical_gameobject_graph_v68.cpp:137 for the reviewed body: same pinned base, attached-anchor deltas, alias-sensitive input rereads, point stores, AABB, physical pointer 2dc, visual pointer 2d8 and optional destination. This lower-level mapping does not repair disconnected Lua _SetPosition #3489.

#3498 Stop has a matching logical prefix at character_heading_owner_v1.cpp:54: drop path, destination=current, active/path flags zero, heading zero, then physical gate. Full body-backend and owning-caller comparison remains partial. Other rotation/path/target-position, GameObject InitPost/Update/destruction and subobject bodies read here need source comparison.

## Evidence limits and remaining systems

All 82 unique thunk adjustment bodies were read and compared with assembly structure: constant this subtraction or virtual-base offset load, then tail branch. Each CSV row retains its own tail operand and owning-function number. Identical ARM PC-relative branch bytes may resolve to different absolute targets, so source status is never inherited just from a digest. Three literal assembly groups give structural-only coverage for other owners; they do not accept lifecycle or reachability. Standard helper names are not classified as imports/out-of-scope merely from names.

xrefs.jsonl was indexed for resolved direct/tail callers/callees and data references. CSV contains complete resolved call ID sets, data-reference counts/samples and indirect-dispatch site counts. Internal labels are excluded as callees. An indirect-site count is not proof that targets are closed.

Unfinished IDs above include nontrivial input managers, rendering queues/passes/nodes, animation applicators/blenders/sets, audio, player/network/stat state, Lua manager/script bodies, trophies, game states, modules/decor, properties/helpers, zoning and triggers. None is declared implemented, missing or out-of-scope without a scoped body/source comparison. This lane has not passed the completion gate.

## Bounded post-pause recheck (2026-10-08)

Final citation validation time: 2026-10-08 15:19:54 UTC.

This follow-up is limited to the lane-02 obligations in post-pause-source-impact.md. It refreshes exactly 21 previously cited rows: 1960, 3434, 3438-3440, 3446-3447, 3450, 3452, 3465-3466, 3468, 3470-3472, 3474-3475, 3477-3478, 3489-3490. All prior comparison statuses are preserved. Two additional assigned bodies, #2418 and #2996, were read in full pseudocode and assembly for the process-design dependency only; both remain unclear. Thus 466 records / 198 hashes have body/structural inspection, with 1,400 bodies still unfinished. No new integrated acceptance is claimed; all runtime fields remain not_run.

The first-pass report fingerprints were CSV **77bb2fd546e52ba84e87afbbfaa8792dd53665bc52c9c9fb66d0584b821b3f15** and Markdown **28d6de5ba2b2df3c0f5a52deef44803da00d48c2108c3ebb27fc195017aca144**. CSV post_pause_prior_source_location retains the complete prior hash/line citation for each refreshed row; post_pause_recheck records this bounded scope. Only lane-02.csv and lane-02.md were written.

### Changed source versions and refreshed anchors

| Source | Prior lane-02 SHA-256 | Current rechecked SHA-256 | Refreshed scope |
|---|---|---|---|
| port/level-world/character_script_session.cpp | f1604fe5fca3eda25a0927c337111f96de73868c5f6a895c59222629b1692495 | 8ab0110f18e758f766e38213a411b0e978f5f5f2fef4f3702f4b148a763f630e | 173 HasPath; 193 motion; 296 motion registration; 328 HasPath selection; 341 GetPosition; 348 unsupported; 19/208/225/228-237/334 required/design wrappers |
| port/android-native/app/src/main/cpp/original_ui_session.cpp | c4db25ae30ec4bc60ffda88594b8e724c10564c4b1520ee229972894a5b9dc76 | d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18 | 462-476 texture caller; 94-95 owner registry; 680-703 arrays load/borrow; 1142-1168 publication/retirement |
| port/android-native/app/src/main/cpp/model_renderer.cpp | 68df536883d56f0cd74f5b47052daafd07491faf5a75eec901790fd26afbe015 | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca | 944-950 process lookup; 952-973 design constructor; 2667/2724 development skill route; 2217 source-boundary include |
| port/android-native/app/src/main/cpp/front_ui_session_v87.cpp | f88ceb1ed519625d059b20fefdffabf78ff65282d1b707760d8f0adb42b8b3ae | 3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638 | Additional drift: texture589-608, projection591, optional splash URI592-593 |
| port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp | 3c4ceb5d7cbe2210bd9edffddc9330c9e5521d4012a592472948f9b67f8f14ae | e4a9925f8f1673b83302165503ebe1ddfd4dc5dc6109015a0604110bbaad0391 | Additional drift: motion738-745, animation747-754, path refresh756-762, NPC binding764-770 |

A final citation validation found two additional changed sources in the same 21-row scope. Only the cited FrontUiSession texture and CampaignFsm motion/path/animation leaves were reread. The filename projection call and motion/path owner connection remain present; splash override after projection was not accepted as complete original texture-selection parity. This does not expand the recheck to other FrontUiSession or FSM behavior.

### Callback failure propagation

Current character_script_session.cpp:19 introduces required_fail; :208 tags actual motion provider failure, :225 tags animation provider failure, and :228-237/:332-334 wrap design delivery errors as DH2_SCRIPT_REQUIRED_SERVICE_FAILURE. Success/miss/arity are separate: GetPyOID's two-string guard can do nothing; a delivered member miss produces numeric -1; a failed lookup delivery uses required-failure tagging.

The live VM trampoline at script_runtime.c:89-92 increments required_failure_epoch before raising the Lua diagnostic. Source status at :441-453 and ScriptOwner initial virtual/file load at character_script_owner.cpp:110-115/:157-160 check that epoch, so catching the Lua diagnostic does not erase the native-service failure. This supports the revised failure contract. It does not extend to every callback: HasPath at session:173-176 still uses ordinary fail=1. The motion globals retain the same successful addresses, guards and actor state; previously identified object-method rejection remains unchanged. Unsupported target-list/GetSelf/SetPosition fallback is still present at session:348 and unsupported:188. The new design delegate handles 37f5fc, not those unrelated addresses.

The UI texture callback remains at original_ui_session.cpp:462-476, with the same filename projection at :464, cache lookup, read, decode and GPU delivery. #1960 remains partial. Changed movie/reset behavior was outside this filename recheck. The older development skill candidate remains at model_renderer.cpp:2667/:2724; source has not supplied new evidence to promote that candidate to current campaign target-list closure.

### #2996 GetPyOID -> #2418 AnimDict: new provider connection and limits

Original #2996 guards at least two exact string Values. Assembly 37f69c captures Application+30's current process arrays before extracting both strings; 37f728 calls #9508 PyDataArrays.GetOID, then 37f738 pushes the integer. #9508 finds class-name dispatch in its map and invokes the registered getter or returns -1. Original #2418 (364b80) scans exactly Arrays.AnimDict.size names in order, returning first match or -1. Its process registration data xrefs come from #9515/#9516.

Current callback session:237/:334 -> script_design_bindings.c:11 -> CharacterGameDesign.Snapshot.lookup at character_game_design.cpp:51 delegates registered groups outside the six local tables to its retained process_design. AnimDict appears in registered_names.inc:132. Renderer model_renderer.cpp:944-950 delegates to SourceProcessArrays.get_member_id; :967-972 borrows the current published process owner and passes the shared owner into initialize. Snapshot :99 retains it; :88 rejects reload while a Borrow exists. Current campaign selection also uses that constructor through renderer_source_boundary_v61.inc:25-43; model_renderer.cpp:2217 includes this current source, and create_source_world_v55 at boundary:63-66 passes the retained design into WorldScriptContext.

The process registry is weak under a mutex (original_ui_session.cpp:94-95), publication is :1167, borrow is :699-703 and teardown retires only the same instance at :1144-1145. A Borrow and Snapshot pin the selected process owner; CharacterScriptSession.Impl owns that Borrow and resets ScriptOwner before destroying its dependencies (:70-99), while ScriptOwner.Session closes the VM before context teardown (:63-70). This provides a static finalizer lifetime argument for the selected instance.

Borrow does **not** require arrays.ready(). AnimDict records are stage 29 and names stage 64; get_member_id at source_process_arrays_v101.cpp:75-89 delivers -1 for zero declared rows, requires loaded names for positive rows and uses a first-emplace name index. Class registration records the original getter 364b80 (source_process_array_registration_v101.hpp:219). A failed renderer borrow still constructs the no-provider snapshot; a later lookup of known extra group then fails delivery. renderer_source_boundary_v61.inc caches the design on first selection or reuses live_script_world.design. Therefore startup timing, cached no-provider recovery and instance replacement/reload need further evidence: original GetPyOID reads the current Application-owned process instance each invocation, whereas the modern Snapshot pins the instance chosen at construction.

Original #9350 readNames populates only when name count equals Arrays.AnimDict.size, while current names load/lookup requires separate review of mismatch/partial-load states. This bounded pass did not accept every AnimDict record/name failure branch, every ANIM_AddAnimDictToSet consumer, owner replacement or authored registration timing. #2418 and #2996 remain unclear despite the concrete new connection.

### Scope fingerprints

All entries below were hashed and rechecked for this follow-up. Changed during this bounded recheck: none. New dependency files had no prior lane-02 hash; their hashes establish this observation, not a reconstructed historical patch. Other first-pass source hashes below retain their original scope.

| Rechecked source/dependency | SHA-256 |
|---|---|
| port/android-native/app/src/main/cpp/front_ui_session_v87.cpp | 3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638 |
| port/android-native/app/src/main/cpp/model_renderer.cpp | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca |
| port/android-native/app/src/main/cpp/original_ui_session.cpp | d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18 |
| port/android-native/app/src/main/cpp/original_ui_session.hpp | 4efa3ebdd7cb260492ad7be7817737a59fdd0c742b3149812f5141617b16388f |
| port/android-native/app/src/main/cpp/renderer_source_boundary_v61.inc | a2434c103faa4b25721377cff87b06f32d013ba2e5c582790bcc2ff4b4ec8064 |
| port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp | e4a9925f8f1673b83302165503ebe1ddfd4dc5dc6109015a0604110bbaad0391 |
| port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp | a663cf0ae3dacbbafb1882774954877472b057b5ab6b3ea9f3453dc2cd607fb0 |
| port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp | fde21042364d703a50a51fb7785aaac3acd258e9d8519f7f55a4af80c3ec197a |
| port/android-native/app/src/main/cpp/source_process_arrays_v101.hpp | bd8f964640fa9964536a62fa8e6c932988ee03cbe0124981523b7004ec7ebf5d |
| port/android-native/app/src/main/cpp/source_process_pydata_files_v101.hpp | 2f7680294723e055d84b79b0cc55e95d371858ce267e9a696e157604de41982e |
| port/level-world/character_game_design.cpp | 51444c7524394eba045744a2c7ddca90c5b2b8bb2ccbb8650aef99ef418bb8c8 |
| port/level-world/character_game_design.hpp | 7c640ec5c3abd50117d6fd33d43322516819f8462d77126bef13b91f181e705e |
| port/level-world/character_script_owner.cpp | c47182cc49c542411fc0f2316ea7d832f81dba13ad1f12d0de9051cf908eb3ff |
| port/level-world/character_script_owner_bindings.inc | 0a428c0126d314d664a4af687619e0ce22de1ed6c9d1a56b1fb056fc0f814f14 |
| port/level-world/character_script_session.cpp | 8ab0110f18e758f766e38213a411b0e978f5f5f2fef4f3702f4b148a763f630e |
| port/level-world/reference/character-game-design/registered_names.inc | b0405c5c439a864cbf6a6f5291517922fb9d36ba58a0bd79cb7ef909ea41348b |
| port/script-runtime/script_design_bindings.c | d3fa2b17d974aec16a130ab9f6ffd547e89f5cc24b26f18e98f970ad8b931e0a |
| port/script-runtime/script_runtime.c | 571f4009dfac62b4251d9e4555f6b3cf343166a29d8f65ab92d5bc9d55391b9b |

## Source snapshot hashes

First-pass hashes were captured after initial source inspection and stable in that first interval. The five changed citations in this table are refreshed to the current bounded recheck above; prior versions are preserved there. Changes before first capture cannot be excluded. Historical reports/reference/source-snapshot copies were not accepted as current implementation.

| Current source file | SHA-256 |
|---|---|
| port/android-native/app/src/main/cpp/front_ui_session_v87.cpp | 3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638 |
| port/android-native/app/src/main/cpp/model_renderer.cpp | de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca |
| port/android-native/app/src/main/cpp/native_menu_preview_backend_v121.inc | cb1335e046d22dcf2cc1444c2a25671c37099dc1e56ced70eeb5824a89f1aef6 |
| port/android-native/app/src/main/cpp/original_ui_session.cpp | d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18 |
| port/android-native/app/src/main/cpp/renderer_campaign_audio_services_v68.inc | 0e48f0cbe55988af38417824352a0a62c2dc2d33f54851af32f4aad654d995b2 |
| port/android-native/app/src/main/cpp/renderer_campaign_music_v101.inc | 10d213ffaea509af49ecc053016de62314ad93cf0b6faa53b38c96aea110e99c |
| port/android-native/app/src/main/cpp/renderer_campaign_native_lenders_v89.inc | 1c1c91ca264ee39fbe436b79cbf482845b0277bb53d2c04fc9d7654cd1ec779b |
| port/android-native/app/src/main/cpp/renderer_character_campaign_v62.inc | ad287e056ae6117d991da925e80576433ec8f7292867e8098cf2a58e347cf27b |
| port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp | e4a9925f8f1673b83302165503ebe1ddfd4dc5dc6109015a0604110bbaad0391 |
| port/android-native/app/src/main/cpp/source_campaign_combat_v115.cpp | 7878934f401b04f73f3a6192126fc41be5c7eb88c6cd727c58372244164d68e6 |
| port/android-native/app/src/main/cpp/source_campaign_projectile_methods_v112.cpp | 23da61e1ebba79accb94dfbbf6f62b389a53fa7d9474234a6ea9007978f918ac |
| port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp | 6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e |
| port/engine-audio/audio_level_gameplay_v67.cpp | 62a6a9fe276421b5d0b67a449f94a67192be25aff9fa7a45923d040ec51087c5 |
| port/engine-audio/integration-v42/audio_application_manager_v42.cpp | ea8a49468fff92373579569eeaaf8df6de766a601f759af052ca00bc44c1f043 |
| port/level-loader/catalog_auxiliary_v67.cpp | 0f2dcb1de0026a4ca4fc1ff99f36c8dbacb1403d1b06e08e6b36e65f3049efdc |
| port/level-loader/level_root_cfs_filename_v52.cpp | c5afff048df0629784936b938b9dc528bb8695603263676e1be3627a44158270 |
| port/level-loader/stage_loader_v46_physical.hpp | b017855e9f855ea7fdf25c0d20354145a248d8e9661add3999c3c082d3db7173 |
| port/level-world/CMakeLists.txt | 7f0e6314dd7e2e7afde9fce9a579d5f75e917b2c3f039cabf15f8ef78ec434b4 |
| port/level-world/canonical_gameobject_graph_v68.cpp | 3dd92e560401c2b72263d18def093397dc5710cff8f6fe90eebdd60420441c19 |
| port/level-world/canonical_sound_emitter_v32.cpp | 10d8f14925b7b3b81ce2559906c02a53ed5697b069f468664e45a9b462abd7fc |
| port/level-world/character_heading_owner_v1.cpp | 9cd7ba70a2abe7ed359f3447687a1b4a66fb826bc9e33fe3cd30dbb77debe958 |
| port/level-world/character_script_objects.cpp | 07d4a63b7068243fcff6f307426ecdaae673ab03438fe2982600c494d99f9e0f |
| port/level-world/character_script_objects.hpp | 7ffed627f489fe8758eaa8dfc589b359387ead9d5befceff9ea04355e8585c3c |
| port/level-world/character_script_owner.cpp | c47182cc49c542411fc0f2316ea7d832f81dba13ad1f12d0de9051cf908eb3ff |
| port/level-world/character_script_owner_bindings.inc | 0a428c0126d314d664a4af687619e0ce22de1ed6c9d1a56b1fb056fc0f814f14 |
| port/level-world/character_script_session.cpp | 8ab0110f18e758f766e38213a411b0e978f5f5f2fef4f3702f4b148a763f630e |
| port/level-world/character_skill_native_v5.cpp | 98237f63987e656d42b5d4ea0bec53152c9148928483b190819a60b0ac72815f |
| port/level-world/character_skill_native_v6.cpp | 5624c5844a3e0bda43b75e5d1b86973ebb0732332ea9167707c33de71220d2dc |
| port/level-world/character_skill_readonly_v6.cpp | e06c819bf2aff694027baf8bad0be0f6ceea538f575e333b920585f5923a7c16 |
| port/level-world/character_spatial_bindings.cpp | b347cf19c1888f9ac1046891bd3b3b1f4b3879f32882e1557bf46f6ae94c681f |
| port/level-world/character_world_skill_execution_v6.cpp | 0114462889be70e984b78432ba7d934bbf0fb879bcc5ee942312a037b9d0fc83 |
| port/level-world/game_object_set_position_v2.cpp | 4054b95cab71f550215eecd4f579f54f29943d83fc58c8a5b694b59723d1dde2 |
| port/level-world/gameobject_lua_catalog.inc | 1652c4df384846be323c91c33d482992d21e6bc3cfd4cecb8105b5286155a586 |
| port/level-world/gameobject_lua_representation.cpp | 9855469c0259c284f6955b794f38e10b5526bf2b69d97c958e41568b755b14e0 |
| port/level-world/navigation_objects.cpp | daa5d8c5d117b0ebbfcc7c363259c66c58d6e29ce89776044bdd0a9ad4cedbc9 |
| port/level-world/physical_world.cpp | 104dd9049d1ca95ed2bfdcfb6a83748d0a34f9e6447c0c1ec015d26bcc5c0ba4 |
| port/level-world/physical_world.hpp | 870d89be3afb6b0eb12d3af3b84dbcdd795afabde209126611b0682dd097f517 |
| port/scene-materials/swf_texture.cpp | 5e5fb0a1c31c7a79d212db678a4df71be84f40bdb1e3ecde192a009599801b85 |
| port/script-runtime/script_object_bridge.c | e8bd39d180c86dfa7e6fa0396351010475ac330284372e4607cd2d3e595cf74c |
| port/script-runtime/script_runtime.c | 571f4009dfac62b4251d9e4555f6b3cf343166a29d8f65ab92d5bc9d55391b9b |

## Repeated assigned code hashes

Byte equality is not necessarily absolute-target semantic equality. Every CSV row carries full hash/group members and each thunk its own assembly target. ARM PC-relative branches can have equal bytes and different destinations; no source disposition is inherited blindly.

| SHA-256 | Function numbers |
|---|---|
| 1bf3afd592c37ac585c67f2c3d233e3fd9b9d9bddf46c7b306ce93bf5371daea | 1866, 1870 |
| fa04763cc015275f81b1d864426513948ce8bfb90e0889b5098b558301847cdd | 1867, 1871, 1882, 2452, 2454, 2483, 2485, 2502, 2504, 2554, 2557, 2585, 2588, 3060, 3074, 3076, 3247, 3325, 3328, 3332, 3334, 3395, 3542, 3578, 3607, 3643, 3678, 3703, 3724 |
| 982e4d439db97b660345b346393a13679d4ab5509bbd982a30207acfafa8d479 | 1877, 1879 |
| 379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f | 1878, 1880, 1912, 1913, 1926, 1961, 1968, 1971, 1972, 2029, 2030, 2031, 2413, 2414, 2466, 2530, 2531, 2597, 2605, 2606, 2657, 2659, 2661, 2662, 2664, 2665, 2666, 2668, 2672, 2673, 2675, 2869, 2870, 2871, 2872, 2873, 2874, 2878, 2919, 2920, 2921, 2922, 2933, 2988, 3000, 3003, 3079, 3080, 3081, 3082, 3083, 3084, 3085, 3086, 3087, 3088, 3091, 3092, 3104, 3105, 3107, 3130, 3144, 3145, 3146, 3155, 3159, 3160, 3178, 3179, 3189, 3190, 3191, 3192, 3195, 3196, 3197, 3198, 3206, 3207, 3215, 3216, 3223, 3254, 3255, 3256, 3257, 3342, 3418, 3419, 3431, 3521, 3544, 3545, 3546, 3560, 3571, 3576, 3656, 3683 |
| 34b97ea285a49248d21f74e46f26848d3f2c1a90e381ca2c3fe219705331487e | 1881, 1907, 1920, 2364, 2405, 2447, 2467, 2492, 2536, 2582, 2596, 3180 |
| 765ae85285d5a02dc525a945fc3ba029c88d7ca2f93838292b249de37594a784 | 1885, 1887, 1890, 1892, 3282, 3285, 3311, 3314 |
| 2024c33cedfc7efe60e4be8aec134a81e22d0a7d94a331eae04722014ffbe384 | 1896, 1897 |
| 8849833261a368bb6be6b96be89b529a7d05eaa08925918dea537dc68e3a51bb | 1910, 1911 |
| 68a639b2dffac301817461b8b1e1e2fbca60e4b4db136a34b582411c8b7d17ba | 1914, 2658 |
| d144307b46fa61bf436910ba39ae43bde10820bea91e464c821d18dacfed531d | 1942, 1943 |
| 007f34a6c3441b0240da53f253e513b959105da2d8803255e1856aa48f48db47 | 1969, 2443, 2677, 2688, 2694, 2695, 2696, 2697, 3001, 3002, 3035, 3037, 3039, 3041, 3225, 3259, 3343, 3345, 3522, 3523, 3628, 3657, 3658, 3659, 3684, 3685 |
| 6dcd5e75586fe6683156c0d559d4827b73bd48d501fa5781a1c6099aacd7c877 | 2009, 2243, 2244, 2527, 3040, 3224, 3251, 3252, 3258, 3260, 3261, 3262, 3414, 3572 |
| bd40b9083ee7d6ba18f4e9c0775b49777349eae8c6714f0598e0fdeaabd36767 | 2044, 2046 |
| 045aa6297d96a7dec203f9709a0d94d8652a30324120bca39d8abf71ec1c414f | 2169, 2172, 2178, 2255 |
| faa1ef5a6f0c3b7a1efb0f8bdef1aea63072055fa817aa198ce897b17ddd50f5 | 2170, 2173, 2179, 2256 |
| 314642571ec97276339d7908dbdf2642bcf59eeae3c2b4647100f000504440af | 2181, 2183 |
| 129db75274f3e211f4d299a3492aab450af7fc6928a8e89b3eb734ae8986af49 | 2196, 2199, 2202, 2266, 2291 |
| 954b70ef54de3f3b1c3e6903103da96fe47e2f25c40219b001f14e5a791280db | 2197, 2200, 2203, 2267, 2292 |
| a5a34b090ae5feddf73411d2c1180aa70beccabc849298131051b7f318586ca8 | 2205, 2227 |
| 3ec72437face90620275012d5319d0c3dd1e1c73043c6c48abfbe98fa549ef37 | 2206, 2228 |
| daeeefbc1ab7b9e02443b6028e2e6062a014e7520eb27ba9afb75b5964a28c92 | 2211, 2269, 3052 |
| afb96fc44c9826f15b71580129992a005054bcf66f3fcbaffeb5e79cf0965928 | 2212, 2270, 3053 |
| e940440167811e0705b620b7f92226ea4b48799b64670ec3955a4e2bfeeb5c27 | 2214, 2217 |
| c964e37490c2249c004a4f11c13ae6e7d4a92451afdf86c5679609473c7dd6bc | 2215, 2218 |
| 1c5a59316e364a3eb8556df0131d55a9de2ba12413ac4fb7621f859ffc563924 | 2257, 2261, 2293 |
| 29b6c3bca1962db2d7fc0af5b896e0fd28dc33686624c716955bce2802ed80e9 | 2263, 3049 |
| 918b6aa699462435e1601f0b5100e1643fa02d3d793596f218b3dbb7d25c1266 | 2264, 3050 |
| a6b4c55e0dd9cd89b022b838c897c7c2e9eb60b9c692e42e564253d693f28b7d | 2303, 2304, 2377, 2378 |
| c1746b5ed3a54e764b261a7cacd55c3778b599ac0a5fe79778db80b431f576f8 | 2397, 2398 |
| 601a7c9f2f41b13dc1b05ea744928714045635a974ca72364bad298980ef9ee1 | 2446, 2491 |
| 90f5106ab87567f0598ad6b90617ed2a9e529cc87b88479636f6cbcd5bf2ff00 | 2487, 2559, 3287, 3316 |
| 0ded1e40bf45877f3e1ebf69d486a944141c910157afc4f3813ea8d390a694ec | 2511, 2513, 2515, 2517, 2519, 2521, 2523, 2526, 2528, 3238, 3240, 3264, 3266, 3268, 3277, 3295, 3297, 3299, 3301, 3306, 3308, 3317, 3319, 3368, 3370, 3399, 3401, 3534, 3536, 3551, 3553, 3563, 3565, 3586, 3588, 3599, 3601, 3623, 3625, 3638, 3640, 3651, 3653, 3666, 3668, 3671, 3673, 3697, 3699, 3714, 3716, 3729, 3731 |
| 8b97579ab00a1c7dff3ec2da309db4fcac75f9413ef7dab05371be86463e63ce | 2553, 2556 |
| 3cbbe34b62127ab8ffe7c69208d69465bf4c109daf11e46aee2574eace58cb4d | 2561, 2563, 2565, 2567, 2569, 2571, 2573, 2575, 2577, 2579 |
| 9d0e58610b19ff896be43c4f6463d3651bbbefa31d7a866a74085842ebdd0bbe | 2566, 2570 |
| 8c7ef5fe7e8dd102f56ac7fcd7e180c0f95d4555811abeb2cabf484a19bf9138 | 2584, 2587 |
| 4b652a22ab3461b88e8762041019b4eaef2135a8cd5f9be5434d294988c89b59 | 2607, 3131 |
| d5067b070d30013b6c9a85dc6906da7038dde479912531cc19405b57fb2b5a82 | 2669, 2670, 2671, 2678 |
| f794a96f7ae6cca84512e07d84d5433bb44a5caedd5c59ce72b1fa2cdf86e291 | 2679, 2682, 2685 |
| f784f97224e88b07c2539cb007a7a0f937f635d59f538c9f3e1d31af77ad9831 | 2680, 2683, 2686 |
| adb591a7f69f280d3116791af6e43c0d420e841d275885417c7192bd9524cbb5 | 2867, 2868, 3089, 3090, 3187, 3188 |
| d7e6d4a66bb7f9c458f501f67a687c64f78090566bc07781ce603c639ec2f800 | 2898, 2899 |
| 6143299c415a779d14d46483b05772d81c6b595bc189b4499503e30275041932 | 3034, 3349 |
| 7dacf1801d93c81fdc0b91850ff950c71de2e972dff198461c38db25df91d5e3 | 3102, 3103 |
| ec3d360cf2cc193223dde4cfaa2045151149c40e4b1d642309cb0145fc9be50b | 3128, 3129 |
| b21af60d2be89cea70b7f1ce33c267f89b678e7ab440305259947ff034d6ae2b | 3176, 3177 |
| f68520e7644c47739b5b74691da29778feb6b6b8fdeed6ee79f647d2e3ded16e | 3193, 3194 |
| a5d1ebb661b3004e8aa73d2371157e025609734a594fa060d11bec5372cee31b | 3213, 3214 |
| 08704a31f2291bb8db23aed8122250105ded856aa313815b8f32047e8bd76549 | 3230, 3263 |
| 44e1a323f7aff4195cb07711e94e6be0766f9a3a713073c63c89346e92172cf0 | 3354, 3525 |
| 60926e9f7d0bc8ab20dc66a4f665de32e52c56400960f338d9f13233eb8e4fc8 | 3408, 3409 |
| 3c52ebb4ca40c57c62cc2a46624dcc8361b018e96ef6e2fb7636ae82810cf597 | 3558, 3620 |
