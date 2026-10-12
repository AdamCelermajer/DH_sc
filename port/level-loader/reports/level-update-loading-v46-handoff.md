# V46 original loading branch and early stage providers

The actual original `Level.Update(false)` loading branch is now implemented as a checked same-C1 adapter. It calls one explicit `_LoadProcess` provider and returns, including the37->38 transition. At initial state38 or `force=true`, it reports that main must execute gameplay at3f8350. It never calls the loader as gameplay, creates another current Level/global, or writes readiness.

The original envelope passed172 ARM cases. Original0..6 plus debug-only16/19 stage prefixes passed18 observer cases; all reached per-call fast-travel/debug prefix branches passed48 cases. The engine-body callbacks in these ARM cases are explicit observation fixtures. Actual profiling/memory-stat functions are BX LR no-ops; actual field/global stores execute original ARM.

The standalone native test passed172 original gold selector/envelope cases, four stage0 order/failure-prefix cases, four real Box2D boundary cases and ten safety guards. Normal and ASAN/UBSAN builds passed. The Box2D static dependency is unsanitized; only the test/adapters and directly compiled physical-world body were sanitized. The large coherent-C1 probe was a normal host build.

The real SWAMP C1 probe passed five pointer aliases, row41/seed7/logicalSWAMP/rawworlds/001_swamp.mlx, unchanged Lua/Save owners, and force/main-gameplay separation. With explicit dispatcher-prefix fixture it reaches stage0's actual g_bigI/g_bigV zero stores and stops on absent CleanGlitch while preserving the five unreached suffix values. A separate selected5 fixture executes the genuine retained NativeWorld load; it does not claim stages0..4 completed. The SourceLoadingV43 composition refuses absent per-call prefix and nested prefix reentry before any dispatcher service. Preparation class/map bodies are unopened and explicitly required.

| Subgoal | Task | Status | Evidence or remaining owner |
|---|---|---|---|
| Original Update | Entire loading selector/call/return envelope | Finished |172 ARM cases; initial38/force ->main |
| Original prefix | All fast-travel/debug branch paths | Finished evidence |48 ARM cases; real body providers required |
| Native Update | Checked completed-C1/same-owner/foreign/reentry/error adapter | Finished |10 standalone guards +realC1 |
| Stage0 | Genuine borrowed134/138/13c/144 and original scalar reset order | Finished source/test |Four prefix cases;5 realC1 aliases |
| Stage0 | Actual Application/global/device/audio production bindings | Required main services |CleanGlitch virtualac;Vox StopAll500 |
| Stage1 | Source default increment | Existing verified source |LifecycleV36 |
| Stage2 | UnloadMenu2,1,3 with fresh instance each | Finished evidence; body required |Actual MenuManager/menu owner |
| Stage3/16/19 | Actual debug-only calls | Finished evidence; body required |Explicit GetInstance/GetSwitch hook |
| Stage4 | BuildLibraries and reached inner-body source | Finished evidence; body required |Actual FX manager/debug/library owners |
| Stage5 | SAME retained PhysicalWorld.load with exact2000 bounds | Finished source/test |Real backend+four boundary cases |
| Stage5 | Actual Application+44 and debug service production borrow | Required main binding |No replacement world |
| Stage6 | PM flag/Update ->GameEvent C1/store/Load order | Finished evidence; bodies required |Actual PM6c9/Update;event table owner/lease |
| Source composition | V43 preparation and typed Update wrapper | Finished bounded tests |Absent-prefix/reentry ->no dispatch |
| Later player/light/camera | Concrete existing provider inventory | Finished audit |Main CameraRuntimeV11 exists; actual facets/publication required |
| Whole chapter Init/gameplay | Continuous original progression/rendering | Open |No whole Init/gameplay proof |

New source headers are `level_update_loading_v46.hpp`, `level_source_loading_update_v46.hpp`, `stage_loader_v46_early.hpp`, and `stage_loader_v46_physical.hpp`. The new concrete wrapper requires authentic per-call prefix services and debug-only stage hooks; bare SourceLoadingV43.tick is not certified as complete original `_LoadProcess`.

Proposed integration: main supplies actual source-prefix/App facets; parent registers the stage0/stage5 body callbacks on the existing source dispatcher and binds this wrapper as the loading part of the actual GS frame's Level.Update(false). Main's whole phase38/forced gameplay body remains an explicit continuation. Other stage bodies remain required. Cancel/unload is a separate real abort-teardown contract, not triggered by this original Update branch.

Proof commands are recorded in `reports/level-update-loading-v46-build.json`; runner is `tools/level_update_loading_v46_build.py`. Original proof is `reference/level-init-v36/level-update-loading-v46-original.py`. No emulator/app process, selected graph/CMake/manifest, common source or shared checkout was changed. Exact libraries are hashed rather than claiming an unrelated full graph checkpoint.
