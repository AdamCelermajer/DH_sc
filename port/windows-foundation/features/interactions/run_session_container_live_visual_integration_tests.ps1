$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskBuild = Join-Path $PSScriptRoot 'test-output/container-live-visual-v1'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
if (-not (Test-Path $taskCompiler)) { throw "Pinned native compiler is missing: $taskCompiler" }
$taskObjectCache = Join-Path $taskBuild 'objects'
New-Item -ItemType Directory -Force -Path $taskObjectCache | Out-Null
Copy-Item -LiteralPath (Join-Path $taskRoot '.local-inputs/publication/checkpoint/reference/openable-container-v1/cache/go_chest_swamp.bdae') -Destination $taskObjectCache -Force
Copy-Item -LiteralPath (Join-Path $taskRoot 'port/windows-foundation/features/interactions/test-assets/go_swamp_urn_breakable.bdae') -Destination $taskObjectCache -Force
$taskSources = @(
    'features/interactions/session_container_live_visual_integration_tests.cpp',
    'features/interactions/session_world_item_consumer_v1.cpp',
    'features/interactions/source_world_item_drop_render_v1.cpp',
    'features/interactions/source_world_item_drop_material_v1.cpp',
    'features/interactions/session_container_retained_visual_v1.cpp',
    'features/interactions/session_container_modern_drop_v1.cpp',
    'features/interactions/session_container_modern_openable_v1.cpp',
    'features/interactions/session_container_admitted_openable_v1.cpp',
    'features/interactions/session_authored_openable_scene_v1.cpp',
    'features/interactions/session_source_object_admission_v1.cpp',
    'features/interactions/session_destructible_interaction_v1.cpp',
    'features/interactions/world_object_container_state_v1.cpp',
    'features/interactions/source_container_loot_v1.cpp',
    'features/interactions/session_openable_interaction_v1.cpp',
    'features/loot/runtime_world_item_adapter_v1.cpp',
    'features/loot/runtime_world_item_interaction_v1.cpp',
    'features/inventory/inventory_feature.cpp',
    'combat_session.cpp', 'actor_combat_runtime.cpp', 'combat_system.cpp', 'actor_state.cpp', 'game_save.cpp', 'world.cpp', 'playable_actor_world.cpp',
    'original_combat_properties.cpp', 'renderer.cpp',
    '../level-world/openable_container_owner_v1.cpp',
    '../level-world/game_object_spawn_probability_v1.cpp',
    '../level-world/openable_container_interaction_v2.cpp',
    '../level-world/destructible_container_data_v16.cpp',
    '../engine-animation/event_track.cpp', '../engine-animation/animation.cpp',
    '../engine-animation/angle_interpreter.cpp', '../scene-materials/scene.cpp',
    '../engine-resources/resources.cpp', '../asset-payloads/payloads.cpp',
    '../engine-math/math.cpp', '../game-data/loot_table_selection_v8.cpp',
    '../game-data/loot_item_selection_v8.cpp', '../game-data/loot_entry_selection_v8.cpp',
    '../game-data/loot_power_creation_v7.cpp', '../game-data/loot_power_resources_v7.cpp',
    '../game-data/item_power_tables_v5.cpp', '../game-data/item_presentation_v5.cpp',
    '../game-data/loot_audiovisual_v8.cpp', 'texture_loader.cpp'
) | ForEach-Object { Join-Path $taskRoot ('port/windows-foundation/' + $_) }
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a',
    'librecovered_trigger_contacts.a','librecovered_content.a',
    'physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $taskRoot ('.local-inputs/windows-foundation-build/' + $_) }
foreach ($library in $taskLibraries) {
    if (-not (Test-Path $library)) { throw "Required foundation archive is missing: $library" }
}
$taskIncludes = @('-Iport/windows-foundation','-Iport/game-data','-Iport/level-world',
    '-Iport/engine-animation','-Iport/engine-skinning','-Iport/scene-materials','-Iport/engine-resources',
    '-Iport/asset-payloads','-Iport/engine-math')
$taskExe = Join-Path $taskBuild 'session_container_live_visual_integration_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @taskIncludes @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lopengl32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Live container visual integration compile failed' }
& $taskExe (Join-Path $taskRoot '.local-inputs/windows-shared-assets') `
    (Join-Path $taskRoot '.local-inputs/player-loot-v7/cache') $taskObjectCache `
    (Join-Path $taskRoot '.local-inputs/loot-world-v8/cache') `
    (Join-Path $taskRoot '.local-inputs/windows-main-frontend-v1/assets') `
    (Join-Path $taskRoot '.local-inputs/interactions-source-material-cache-v1/assets') |
    Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw 'Live container visual integration test failed' }
