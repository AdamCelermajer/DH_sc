# V47 actual main provider bindings for V46

This audit reads the current shared main sources and original ELF; it changes only this reference/report packet. It does not claim that the app is running any provider. File hashes in the source snapshot define the examined version. Parent's private coherent successor already contains several interface migrations still absent from the shared main snapshot.

| Dependency | Current shared main | Exact adoption action |
|---|---|---|
| Actual state130 | uint32 and LoadingFieldsV26 borrow exist | Reuse same completed C1; never `WorldScriptContext.source_level_load_phase` |
| Actual134/138/13c | Still uintptr in shared constructor header | Adopt reviewed original scalar uint32 header and rebuild all C1/context consumers coherently; no reinterpret cast |
| Logical NAME versus raw filename | NativeLevelConnectionV25 still compares request.identity==arguments.name | Adopt reviewed separation: logicalSWAMP, rawworlds/001_swamp.mlx, verified cacheURI distinct; keep seed equality |
| GS frame fields | NativeGSRuntimeV27 exposes const fields only | Adopt parentV45 constructed-only frame_fields accessor and checked frame, without a second field copy |
| Actual current Level | NativeGSLevelGlobalsV27.s_level and runtime.current already exist | Use runtime.current/renderer borrow_current_native_level_v27; actual empty global stays empty |
| GS→loader Update binding | Shared runtime has no loading frame composition | Adopt parent's checked V49 sibling connection; reject caller-supplied level_update/borrow/load conflicts |

The renderer's current Crypt demo explicitly uses unpublished native C1 when `native_gs_launch_v27=false`. `borrow_current_native_level_v27` therefore reads a genuine empty global on that route. Bind the loader into a genuine GS construction route with real level owners; do not publish the demo C1 as a fallback. Source-only inspection is not a campaign launch producer.

## Per-call `_LoadProcess` prefix

`SourceLoadProcessPrefixV46.actual_provider_owner` should pin an independent Application/provider lifetime. The before_dispatch callback must capture the completed actual C1 borrow and reread its state130 at original query/log points. Its uint32 argument is an entry observation, not permission to introduce a saved phase authority.

1. Read actual `handle_cheats_inGame` byte9f640a. Its sole native owner/borrow is missing from shared ApplicationServicesOwnerV5 and NativeLevelApplicationV25; add a genuine original static field facet to the existing Application/global owner. An unavailable borrow is failure, not false. A genuine zero skips all fast-travel work.
2. If nonzero, call existing actual current-Level accessor. A genuine empty global skips the player branch. If nonempty, call the same live `PlayerManagerOwnerV1::get_local_player(0,true,record,error)`. Main already retains its real manager through RendererPlayerManagerBootV25.actual_manager, including ownership transfer; require that actual pointer/lifetime instead of allocating a second manager. Observe the actual record.character660; null skips Character/save/UI work.
3. For a real Character, implement/bind original Character.SG_UnlockAllFastTravels3bba70. It reads actual Save14e8; a genuine null Save is source no-op. Otherwise it calls PlayerSavegame466384. The current PlayerSavegameV1 owns the correct three bitsets17c but exposes only a const getter and load/write serialization. Add a narrow genuine `unlock_all_fast_travels(actual FastTravelList size provider,error)` producer to that SAME save owner. Original loops over three tiers and sets indices below `Arrays::FastTravelList::size`9a65d8, rereading the real count, preserving all other bits. It does not set all64 unconditionally, write a file, or initialize a substitute Save. Bound invalid counts above64 explicitly rather than inventing success.
4. Still after Character call, borrow actual MenuWorldMap via its sole menu owner and set original byte12c=1. Dedicated MenuWorldMap facet/byte borrow is currently absent. This store occurs even when actual Character.Save14e8 was null; the prior source Character no-op still completed.
5. Borrow the ONE selected actual DebugSwitches and DebugFileServices24; existing `dh2_character_debug_load/get` kernels implement the genuine missing-file domain. WorldScriptContext already owns these and native_gslevel_menu_v27 uses them. Do not create another debug map for loader. Call load then get exact `IsDisplayLoadingStepName` and preserve error statuses. Existing-file parser returns-3; that is an actual missing body, not false/absent-file success.
6. If that actual switch is nonzero, call recovered DBG_GetLoadingStepName3ef18c with freshly read actual state130, then actual MenuDebug.GetInstance/SetText42a08c. Dedicated MenuDebug receiver/body is absent. The original helper indexes its literal table for nonnegative input without an upper guard; use source table evidence and a bounded invalid-domain guard, never substitute lifecycle service strings. Original case37 produces a non-label pointer/string in the captured source fixture; do not guess corrected names.
7. Execute original `_DEBUG_OUT324114` loading-state delivery. It is not BX LR (first instruction pushes arguments); whole logging body/provider is still required. Use the actual platform logging owner/format/body when reconstructed, preserving its position after optional MenuDebug and before stage dispatch. Profiling3136b4/3136b8 and ShowMemoryStats31041c are independently proven BX LR and require no fabricated service.

For source debug-only stages3/16/19, bind `debug_only_stage` to the same actual Debug owner. Original DebugSwitches.GetInstance330740 calls load337888 then returns the same global instance, so the existing load/get kernel pair is a real reusable body adapter in its accepted missing-file domain. Call get exact `isTracingLevel_Loading`, discard the result, leave130 unchanged, then let the real dispatcher increment it.

## Stage0 through6

| Stage | V46/existing typed binding | What is available | What must be supplied |
|---|---|---|---|
|0|`borrow_stage0_level_v46(level,fields,error)`|Completed C1 aliases134/138/13c/144 after scalar migration|Actual Application/global facet +CleanGlitch and Vox StopAll bodies |
|0|`Stage0ApplicationBorrowV46{appLease,staticLease,appIdentity,&bigI,&bigV,&byteb4}`|Original symbols g_bigI9f6890/g_bigV9f688c identified|Sole actual words/Appbyteb4; ApplicationServicesOwnerV5 currently lacks them |
|0|`Stage0ServicesV46.clean_glitch(view,error)`|Original31f55c device chain established|Actual globalApp+10 ->owner+10 ->virtualac body; renderer.clear/reset/deactivate is different |
|0|`stop_all_sounds(soundIdentity,500,error)` +same Vox pin|Audio mixer has stop_all and actual VoxPlay3D transport exists|Whole Vox369990 receiver/bookkeeping/fade500 body, not just mixer command |
|1|Existing LifecycleV36 default increment|Original no additional stage body|No success stub or extra provider |
|2|External stage_body2 with actual tracing callback|Real debug kernel, roster/stack/name lookup/push/hide operations|Fresh MenuManager.GetInstance before each UnloadMenu2,1,3 and whole unload body |
|3|V46 debug_only_stage|Existing actual debug kernel accepted domain|Singleton binding only; actual missing-file/parser errors retained |
|4|External stage_body4 +actual debug and DebugModules|Existing `dh2_fx_debug_module_get(...,"AnimatedFX")` body in missing-file domain|Actual VisualFXManager fields and _BuildAnimLibrary/_BuildAnimFXDict/_BuildAnimFXSets bodies |
|5|`stage5_physical_body_v46(actual,physicalAlias,debugBorrow)`|Genuine NativeWorld.load backend/listeners; actual tracing kernels|Alias to sole Application+44 world, runtime-thread and source-stage ownership |
|6|External stage_body6|Actual PM map/GetLocalPlayer and current C1.rawfield194|Actual PM6c9 byte +Update378fb4, distinct GameEventManager C1/Load/owned lease and real GameEvent tables |

Stage0 is wired by assigning its returned body to `inputs.external.stage_body[0]`; stage5 analogously fills5. Each returns complete only after the authentic reached suffix completes. The dispatcher alone increments130 and publishes the scalar tail/UI. Neither callback may increment130 or invent readiness. Genuine required body failure retains the reached source prefix and fails before subsequent stages.

Stage2's `UnloadMenu(int)` is substantially different from GS's MenuBase.Hide and stack Pop. Original42d4bc: id1 first clears actual MenuCharMenu_Map icons; obtains the real MultiMenuManager MenuFX file slot134+id*4; walks original MenuManager registry64..68; for each matching render pointer clears menu+4, conditionally calls deleting vslot4 when actual byte7d is set, and erases the registry entry; finally gets actual FlashAnimManager, ResetScanForAnims for that MenuFX, and calls MultiMenuManager.UnloadSWFFile(id). Current MenuStackMenuV1 has no original byte7d/delete ownership projection, so this needs an actual menu-owner extension/body, preserving synchronous registry rereads and resource pins. Three fresh GetInstance calls are required by stage2.

Stage4 queries exact tracing key first, then actual VisualFXManager.BuildLibraries496bd8. BuildLibraries separately loads Debug and GetModule("AnimatedFX"); its enabled path reaches _BuildAnimLibrary496ba4, which only builds when list28==list2c, sets actual byte4=1, invokes _BuildAnimFXDict495364 and _BuildAnimFXSets496954. Current WorldScriptContext.effects/PreloadBacking/effect_queue and CharacterMeshFxOwner PreCache are useful metadata/queue dependencies but do not implement that body. Missing-file GetModule defaults1 genuinely, so hardcoding the module off would skip required source construction.

Stage5 should alias the existing actual native physics receiver once main establishes it as Application+44. The renderer already has one `actor_world` NativeWorld; it currently loads computed bounds during initialize_native_actor and clears it during GL reset/deactivate. Loader must not allocate a parallel world, reset that live world concurrently, or replay source5 on GL restoration. Main must reconcile its existing demo/GL lifecycle with the original Application physics lifecycle before production binding. V46 calls exact [-2000,-2000,+2000,+2000], which is original34c048 input, while actual existing NativeWorld implements clear/newbackend and listener registration.

Stage6 needs a genuine PM source-body addition, not an adapter to get_local_player/num_players/development adoption: shared PlayerManagerOwnerV1 has no byte6c9 or Update method/operation. Original stage sets actual6c9=1 BEFORE Update, then allocates12 and constructs a distinct GameEventManager479d08, writes actual Level194, then Load479e5c. This GameEventManager is a vector of GameEvent pointers and is not Level.events or Application.events14 EventManagerOwnerV12. Current field194 is a raw identity only; main must retain the actual event owner inside the sole Level's active lifetime. A temporary stage/provider pin cannot disappear at source-finished while194 still points to it. Load resizes from actual Arrays event count and constructs32byte GameEvents with AssignPyData/ReInit before each vector publication; quest/event data binding and later Compile31 remain main-owned bodies. Loader merely sequences and preserves failed prefixes.

## Ownership and verification before adoption

Use independent provider leases with weak references to any World/GS parent that owns the frame; synchronous callbacks lock and validate the actual World. Source frame/loading/GS are sibling-owned. Never capture a strong GS inside callbacks retained by its own Level/World, copy a registry, create a second debug/current/physics owner, or use renderer.source_level_load_phase as loading authority.

Before calling a whole production loading path, verify completed actual C1 and pointer equality for state/counters; actual preparation.manager==source manager; sole CurrentLevel global; actual debug identity/file provider; actual App/scalar/device/audio views; genuine source step order. Meaningful host checks should exercise missing body prefixes, parser-3 and expired leases, fresh MenuManager instance order, registry erase after callbacks, real stage5 bounds, PM flag-before-Update, GameEvent publication-before-Load, and later real event-owner retention. A callback returningtrue with an empty body does not close these requirements.

| Subgoal | Status |
|---|---|
| Current shared interface/source hash audit | Finished |
| Coherent migration prerequisites and actual CurrentLevel route | Finished |
| Per-call prefix exact typed binding plan | Finished |
| Early stages available bodies versus missing implementations | Finished |
| Additional original GetInstance/Unload/Character+Save fasttravel proof | Finished |
| Production App/Debug/Menu/FX/PM/Event bodies and bindings | Required main owners |
| Actual GS frame+V46 successor host test | Next bounded taskV49 |

No tests or builds of the shared main graph were run by this audit, and no device or process operations occurred. This is a binding proposal with source evidence, not a claim that these services have been adopted or run.
