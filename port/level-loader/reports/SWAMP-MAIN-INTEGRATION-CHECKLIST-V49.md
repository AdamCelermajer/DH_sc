# Main menu to full SWAMP: immediate V49 integration

Use original selected LevelList row.file as C1 name: SWAMP row41 is `001_swamp.mlx`; SWAMP_02 row42 is `022_swamp2.rule.xml`. Keep logicalidentity SWAMP and cacheURI data/scene/001_swamp.mlx separate. Historical V49 host argument worlds/001_swamp.mlx is a transport fixture, not an original filename or supported mount alias. Original LoadFile tries empty, data/, data/scene/, data/3d/modules/ prefixes; concrete resolver is being delivered. Seed/profile/player services come from main's actual selected save/application; do not copy fixture seed7/slot0 into production.

First surgical hunks (frozen V49/V47 dependencies; rebuild C1 consumers coherently):

1. `port/level-loader/level_constructor_v3.hpp`: replace `uintptr_t field134,field138,field13c,field140` with `uint32_t field134,field138,field13c; uintptr_t field140`. Keep actual130/30 borrows and single manager registry.
2. `native_level_application_v25.cpp::construct_allocated`: remove only logicalidentity==arguments.name equality; require candidate/app/name and retain seed equality. Logical identity is not an original pathname.
3. `native_gslevel_runtime_v27.hpp/.cpp`: add constructed-only `frame_fields_v45(GSLevelFieldsV2<CanonicalLevelContextV1>*&,string&)`: fail unless `gs_ && gs_->lifecycle().stage()==GSLevelLifecycleStageV2::constructed`, otherwise lend `&fields_`. No second fields/currentLevel or phase scalar.
4. Add checked V49 sibling connection to actual GS candidate route; do not force demo C1/currentLevel when native_gs_launch=false. Leave GS services.borrow_level/level_load/level_update empty before connection.create; bind real source preparation+same manager, prefix/early bodies and explicit main gameplay continuation. Keep application/current/manager/file/cache owners pinned and weak parent callbacks.

Connection call: `NativeGSLevelLoadingConnectionV49::create(actual_gs, actual_application, actual_gs_services, source_inputs, actual_per_call_prefix, main_gameplay_update, sibling_out, error)`. Actual GS construction publishes source current before the loading menu. Frame1→2→3 then executes original Level.Update loading branch. State38/force gameplay is main. Bare SourceLoading.tick is not whole LoadProcess.

| Original stage | Required real provider / immediate owner |
|---|---|
| Every call;3/16/19 | Actual cheat/debug/log facets and Debug load/GetSwitch; optional real fasttravel/save/MenuWorldMap branch. Reuse actual debug singleton; existing-file parser-3 must fail, not become false. |
|0,25| Main App CleanGlitch/device chain and same Vox StopAll500;0 also actual g_bigI/g_bigV/byteb4/counter reset. V46 body/aliases ready. Audio chat owns manager. |
|2| Real fresh MenuManager.UnloadMenu2,1,3; registry erase/delete/SWF reset, not Hide/Pop. |
|4,30| Actual FX BuildLibraries and PreCache; absent-file AnimatedFX GetModule genuinely enables build. |
|5| Sole Application+44 retained NativeWorld.load, original ±2000 bounds; no parallel world or GL-reset replay. V46 body ready. |
|6| PM byte6c9→Update, distinct Level194 GameEvent manager C1→publish→Load66 actual events. Loader agent implementing storage/reset; Compile/effects main. |
|7| Original seed/rule + LoadFile/generated-stream route; same root preparation. Raw selected row filename, not worlds alias. Loader resolver agent delivering. |
|8,9| Actual LevelConfig/fog/_LoadScripts and whole lightset load; C1 script44/names-only LightSet owner do not replace these. |
|10| Same ObjectManager source walk/InitPost. Loader connector ready; audio owns chest manager/load33 raw value; character qualified parent/conditions/visual services still required. Actual8 fields patch ready, shared control extraction in progress. |
|11| Actual PFWorld.PostLoad after successful10. |
|12,13| Real menu1 load and ScriptManager.InitCommands. |
|14,15,18| Main real saved player/online binds, sound routing, character states. |
|17| Real InitFinal/geometry; no early floor/geometry activation to bypass10. |
|20| Existing real GameplayCameraRuntimeV11 with source config, actual scene/App/camera slots. |
|21–24| Actual batching Init/List/Map/Load managers. |
|26| Real HUD/post-init characters/cache refresh. |
|29| Root loot agent: original Item145 + Projectile precache and real DropLoot/pickup/UseOOI; no loader duplicate. |
|31,32| Main actual GameEvent Compile, ambient/ObjectManager/AI/physics/camera update. |
|33,34| Main LevelSavegame restore/spawn/level-id/camera/environment/checkpoint finalization. |
|36,37| Actual online/quest sync + menu NativeEndLoading36→37; then genuine input/menu completion→38. Progress100 alone is insufficient. |
|1,27,28,35| Original scalar default increments; no effect body needed. |

First integration gates: actual GS/C1 borrow succeeds; authenticate original row filename; run real prefix→0..6; source7 loads full root/modules;10 initializes all195 SWAMP declarations/50 Characters/5 chests; then finish11..37 and real current renderer. Visible full map/mobs/chests and gameplay are acceptance. Never mark wholeInit from host prefix proof. Failure/abort must release real owners safely; normal Unload's player save is a separate main path.

Frozen V49 packet `native-gslevel-loading-v49-d2c6e02d9d55ee73.zip`, SHA25646a24ce1eda17c491487e90b7db3cb540d507bb79b928848912182fb67f32df2. Exact V47 binding detail is `loader-provider-binding-v47-plan.md`. Parent selected proof `native-gslevel-loading-v49-selected-verified.json` passes2 actualGS cases/20 guards with explicit flag/log/UI/deep-cleanup transports. This checklist records required services, not unverified production completion. No shared source or device operations by loader.
