$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
Set-Location $repo
$cxx = '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
if (-not (Test-Path $cxx)) { throw "Pinned native compiler is missing: $cxx" }
$sources = @(
    'port/windows-foundation/features/interactions/session_container_interactions_tests.cpp',
    'port/windows-foundation/features/interactions/world_object_container_state_v1.cpp',
    'port/windows-foundation/features/interactions/source_container_loot_v1.cpp',
    'port/windows-foundation/features/interactions/session_openable_interaction_v1.cpp',
    'port/windows-foundation/features/interactions/session_destructible_interaction_v1.cpp',
    'port/windows-foundation/features/loot/runtime_world_item_adapter_v1.cpp',
    'port/windows-foundation/features/loot/runtime_world_item_interaction_v1.cpp',
    'port/windows-foundation/features/inventory/inventory_feature.cpp',
    'port/level-world/openable_container_owner_v1.cpp',
    'port/level-world/openable_container_interaction_v2.cpp',
    'port/level-world/destructible_container_data_v16.cpp',
    'port/engine-animation/event_track.cpp',
    'port/engine-animation/animation.cpp',
    'port/engine-animation/angle_interpreter.cpp',
    'port/scene-materials/scene.cpp',
    'port/engine-resources/resources.cpp',
    'port/asset-payloads/payloads.cpp',
    'port/engine-math/math.cpp',
    'port/game-data/loot_table_selection_v8.cpp',
    'port/game-data/loot_item_selection_v8.cpp',
    'port/game-data/loot_entry_selection_v8.cpp',
    'port/game-data/loot_power_creation_v7.cpp',
    'port/game-data/loot_power_resources_v7.cpp',
    'port/game-data/item_power_tables_v5.cpp',
    'port/game-data/item_presentation_v5.cpp'
)
$includes = @('port/windows-foundation','port/game-data','port/level-world','port/engine-animation',
    'port/scene-materials','port/engine-resources','port/asset-payloads','port/engine-math') |
    ForEach-Object { "-I$_" }
$archives = @(
    '.local-inputs/windows-foundation-build/libfoundation_data.a',
    '.local-inputs/windows-foundation-build/librecovered_content.a',
    '.local-inputs/windows-foundation-build/libcontent_xml.a'
)
foreach ($archive in $archives) {
    if (-not (Test-Path $archive)) { throw "Required native foundation archive is missing: $archive" }
}
$output = 'port/windows-foundation/features/interactions/test-output/session_container_interactions_tests.exe'
New-Item -ItemType Directory -Force (Split-Path $output) | Out-Null
& $cxx -std=c++17 -O1 -static -ffunction-sections -fdata-sections `
    @includes @sources @archives '-Wl,--gc-sections' -o $output
if ($LASTEXITCODE -ne 0) { throw "Native source container loot link failed with exit code $LASTEXITCODE" }
& $output '.local-inputs/player-loot-v7/cache'
if ($LASTEXITCODE -ne 0) { throw "Native source container loot test failed with exit code $LASTEXITCODE" }
