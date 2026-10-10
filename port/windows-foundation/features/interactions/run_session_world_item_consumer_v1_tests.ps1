param([string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath($RepositoryRoot)
$compiler = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $root 'port/windows-foundation/features/interactions/test-output/session_world_item_consumer_v1_tests.exe'
$sources = @(
    'port/windows-foundation/features/interactions/session_world_item_consumer_v1_tests.cpp',
    'port/windows-foundation/features/interactions/session_world_item_consumer_v1.cpp',
    'port/windows-foundation/features/interactions/source_world_item_drop_render_v1.cpp',
    'port/windows-foundation/features/interactions/source_world_item_drop_material_v1.cpp',
    'port/windows-foundation/features/loot/runtime_death_rewards_v1.cpp',
    'port/level-world/player_progression_v1.cpp',
    'port/windows-foundation/features/loot/runtime_world_item_adapter_v1.cpp',
    'port/windows-foundation/features/loot/runtime_world_item_interaction_v1.cpp',
    'port/windows-foundation/features/inventory/inventory_feature.cpp',
    'port/game-data/loot_table_selection_v8.cpp',
    'port/game-data/loot_item_selection_v8.cpp',
    'port/game-data/loot_entry_selection_v8.cpp',
    'port/game-data/loot_power_creation_v7.cpp',
    'port/game-data/loot_power_resources_v7.cpp',
    'port/game-data/loot_audiovisual_v8.cpp',
    'port/game-data/item_power_tables_v5.cpp',
    'port/game-data/item_presentation_v5.cpp'
)
$includes = @(
    'port/windows-foundation', 'port/game-data', 'port/level-world',
    'port/engine-animation', 'port/engine-skinning', 'port/scene-materials',
    'port/script-runtime', 'port/physics-backend/box2d-2.0.1/Include'
)
$arguments = @('-std=c++17','-O1','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-Wno-missing-field-initializers','-ffunction-sections','-fdata-sections','-Wl,--gc-sections')
$arguments += $includes | ForEach-Object { '-I' + (Join-Path $root $_) }
$arguments += $sources | ForEach-Object { Join-Path $root $_ }
$native = Join-Path $root 'port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a'
$arguments += $native
$arguments += ('-L' + (Join-Path $root '.local-inputs/windows-foundation-build'))
$arguments += ('-L' + (Join-Path $root '.local-inputs/windows-foundation-build/physics-backend'))
$arguments += @('-static','-lfoundation_data','-lcontent_xml','-ldh2_freetype237',
    '-lrecovered_trigger_contacts','-lrecovered_content','-ldh2_box2d_201',
    '-lkernel32','-luser32','-lgdi32','-lopengl32','-lwinspool','-lshell32','-lole32',
    '-loleaut32','-luuid','-lcomdlg32','-ladvapi32')
$arguments += $native
$arguments += @('-o',$output)
New-Item -ItemType Directory -Force (Split-Path $output) | Out-Null
& $compiler @arguments
if ($LASTEXITCODE -ne 0) { throw "Session world-item consumer compile failed ($LASTEXITCODE)" }
$resultText = & $output `
    (Join-Path $root '.local-inputs/windows-shared-assets') `
    (Join-Path $root '.local-inputs/player-loot-v7/cache') `
    (Join-Path $root '.local-inputs/loot-world-v8/cache') `
    (Join-Path $root '.local-inputs/windows-main-frontend-v1/assets') `
    (Join-Path $root '.local-inputs/interactions-source-material-cache-v1/assets')
if ($LASTEXITCODE -ne 0) { throw "Session world-item consumer test failed ($LASTEXITCODE): $resultText" }
$result = $resultText | ConvertFrom-Json
if ($result.validation -ne 'PASS') { throw 'Session world-item consumer test did not pass' }
$resultText
