# Original XML to actual SWAMP Module graph

The private integration now consumes the complete original data/scene/001_swamp.mlx through CanonicalCachedFileV1 and CanonicalReceiverTransportV1 into main's actual LevelConfig/Module constructors and graph services. The nine Module declarations, attributes and placements are read directly from the original cache; the copied per-module declaration maps in the incoming fixture are removed.

Actual canonical Add/property/default/override execution publishes one LevelConfig and nine Module objects first. Actual Module InitPost then creates nine canonical generated RoomZones on the SAME manager. That source order yields nineteen registered objects: config + nine modules + nine zones. Their ordinary registry keys are distinct from source Module IDs and diagnostic occurrence indices. Original room64 remains -1 and source PFRoom labels remain UINT32_MAX; portable collision-room indices are separate ordered indices.

All nine selected original roots assemble through the actual static visual/ConditionData/SetPosition/floor/zone graph. The result retains sixteen floor clones, nineteen exits, nine generated zones/pending entries and Module backlinks. Same-world post_load/collision bounds and all nine InitFinal calls use the same PF objects, collision world and obstacle registry. Geometry/floor kernels use the incoming authoritative source paths on existing target names; no second canonical manager or navigation world is created.

After the map graph is assembled, the first actual CanonicalModuleV1::load selects its authored gameplay URI and calls the loader's CanonicalModuleFilesV1 over the SAME Level160/18c storage. Its unfiltered original MGP reaches OpenableContainer and fails at the unavailable actual actor constructor. The original source and first factory failure prefix remain retained, and Module::Load does not fabricate a field reset. No authored chest is skipped to turn this into a whole-level success.

## Evidence

Host, ASAN/UBSAN/leak and native Android x86_64 execution on only emulator-5590/DH2_Loader_API37 pass. ARM64 compiles. reports/canonical-module-graph-source-checks.json records the exact binaries/cache/source/coherent-manifest hashes and result counts. The original incoming 135 files from canonical-module-graph-handoff-0598cd0ac3591e30.zip remain hash-identical in the coherent tree; archive SHA256 is 64cdf8c34aab885b3bc9b9e9a1f619056d1ad5d70e4057ac2545414f763c5ac2.

Run tools/build_module_graph_source.py for host, sanitizers, x86_64 and arm64-v8a, then tools/run_module_graph_source_checks.py. The native Android executable reads the already verified original cache and uses /data/local/tmp/dh2-loader-module-graph-source-v1; no APK install or other emulator mutation occurs. Source example: tests/canonical_module_graph_source_probe.cpp.

## Declared boundaries

Platform/high-performance/driver/Root-counter/modular-mesh transport, CheckSpawnProbability, profile/Handle/TestEnableCondition transport, network assignment, Debug/parser/release and sound-name transport retain the incoming fixture boundaries. Actual class constructors, properties, original XML, scene/floor/zone/PF graph and methods execute. This is not whole GSLevel/Level/EventManager/Lua/save/online construction or live gameplay readiness.

Graph/map release calls perform the actual visual/ConditionData/resource cleanup exercised by the incoming graph. They do not implement manager/network/pending unpublication or whole Level destruction. The failed authored MGP journals remain retained through the test scope; the probe does not acknowledge whole candidate release as if graph release alone satisfied it.

The unconstructed tiny AnimController+38 successor remains an explicit main-owned constructor gap. Actual mob/chest constructor services, full production providers, live scene draw publication, save/quest restoration and other fixed/procedural levels remain required. This graph check has not changed the visible map-inspection APK or met the full loader acceptance goal.

## Review and integration

This layer follows canonical-config-source-handoff-3e4e66b2f0edf454.zip (SHA256 fd2afedc49408ba642771b90d29a210616f6327652a9c92faf0bbead07533d3a) and its source/transport/Module/Level prerequisites. It adds the actual root Module Load implementation (SHA256 80c343e1cd47132306269657b592a9d7f35a5773c4768100224765e4beb42cd4) to the existing canonical owner target for standalone linking; production supplies its existing Module owner target.

Reuse current main owner targets and the real platform providers. The full standalone CMake/coherent overlay is reproduction material, not a wholesale source/vendor replacement for newer main code. The example's constructor context must preserve the same candidate/global counters/scene/floor/map/manager/property graph and weak per-record service captures described in main's canonical-module-graph-v3-handoff.md.
