[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$questRepo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if (-not $Compiler) { $Compiler = Join-Path $questRepo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe' }
$questBuild = Join-Path $PSScriptRoot 'test-output'
New-Item -ItemType Directory -Force -Path $questBuild | Out-Null
& py '-3' (Join-Path $PSScriptRoot 'export_runtime_quest_menu_art_v1.py')
if ($LASTEXITCODE -ne 0) { throw 'Original Quest SWF art export failed' }
$questSources = @(
    'port/windows-foundation/features/quests/original_quest_adapter_tests.cpp',
    'port/windows-foundation/features/quests/original_quest_adapter.cpp',
    'port/windows-foundation/features/quests/original_quest_runtime_bridge.cpp',
    'port/windows-foundation/features/quests/source_quest_service_binding.cpp',
    'port/windows-foundation/original_campaign_runtime.cpp',
    'port/windows-foundation/content_paths.cpp', 'port/windows-foundation/asset_catalog.cpp',
    'port/level-loader/vendor/tinyxml/tinyxml.cpp',
    'port/level-loader/vendor/tinyxml/tinyxmlparser.cpp',
    'port/level-loader/vendor/tinyxml/tinyxmlerror.cpp',
    'port/level-loader/vendor/tinyxml/tinystr.cpp',
    'port/level-world/character_menu_quests_v51.cpp',
    'port/level-world/quest_condition_compile_v70.cpp',
    'port/game-data/quest_persistence_v51.cpp', 'port/game-data/quest_savegame_v1.cpp',
    'port/level-world/native_quest_runtime_v76.cpp',
    'port/level-world/native_conditions_runtime_v69.cpp',
    'port/level-world/native_conditions_table_v69.cpp',
    'port/level-world/canonical_gameobject_base_owner_v1.cpp',
    'port/level-world/savegame_stream_v2.cpp',
    'port/level-loader/game_event_runtime_v75.cpp',
    'port/level-loader/game_event_manager_v50.cpp',
    'port/level-world/event_manager_owner_v12.cpp',
    'port/level-world/navigation_objects.cpp', 'port/level-world/navigation_world.cpp',
    'port/level-world/navigation_motion.cpp', 'port/level-world/navigation_search.cpp',
    'port/level-world/collision.cpp', 'port/level-world/selector.cpp', 'port/level-world/octree.cpp'
) | ForEach-Object { Join-Path $questRepo $_ }
$questIncludes = @(('-I'+(Join-Path $questRepo 'port/game-data')), ('-I'+(Join-Path $questRepo 'port/level-world')))
$questExecutable = Join-Path $questBuild 'quest_adapter_tests.exe'
& $Compiler '-std=c++17' '-O1' '-static' @questIncludes @questSources '-o' $questExecutable
if ($LASTEXITCODE -ne 0) { throw 'Quest component compile failed' }
& $questExecutable (Join-Path $questRepo 'port/level-world/reference/character-menu-profile-v51/cache')
if ($LASTEXITCODE -ne 0) { throw 'Quest original-data component test failed' }
$integrationSources = @($questSources | Select-Object -Skip 1) + @(Join-Path $PSScriptRoot 'npc_talk_objective_integration_test.cpp')
$integrationExecutable = Join-Path $questBuild 'npc_talk_objective_integration_test.exe'
& $Compiler '-std=c++17' '-O1' '-static' @questIncludes @integrationSources '-o' $integrationExecutable
if ($LASTEXITCODE -ne 0) { throw 'Quest/NPC shared Objective integration fixture compile failed' }
& $integrationExecutable (Join-Path $questRepo 'port/level-world/reference/character-menu-profile-v51/cache')
if ($LASTEXITCODE -ne 0) { throw 'Quest/NPC shared Objective integration fixture failed' }
$questLogExecutable = Join-Path $questBuild 'source_quest_log_services_v1_tests.exe'
$questLogSources = @($questSources | Select-Object -Skip 1)
$questLogSources += @(
    'port/windows-foundation/features/quests/source_quest_log_services_v1.cpp',
    'port/windows-foundation/features/quests/source_quest_page_v1.cpp',
    'port/windows-foundation/features/quests/character_quest_page_v1.cpp',
    'port/windows-foundation/features/quests/character_quest_progress_v1.cpp',
    'port/windows-foundation/features/quests/quest_text_resolver_v1.cpp',
    'port/windows-foundation/features/quests/runtime_quest_menu_v1.cpp',
    'port/windows-foundation/features/quests/runtime_quest_menu_art_v1.cpp',
    'port/windows-foundation/features/quests/source_quest_menu_page_provider_v1.cpp',
    'port/windows-foundation/features/quests/source_quest_log_services_v1_tests.cpp',
    'port/level-world/character_design_services.cpp',
    'port/script-runtime/script_constants.cpp',
    'port/engine-ui/hud_text_v1.cpp',
    'port/engine-ui/hud_text_format_v1.cpp',
    'port/engine-ui/localization_parse_ex_v1.cpp',
    'port/engine-ui/localization.cpp'
) | ForEach-Object { Join-Path $questRepo $_ }
$questLogIncludes = @(
    ('-I'+(Join-Path $questRepo 'port/game-data')),
    ('-I'+(Join-Path $questRepo 'port/level-world')),
    ('-I'+(Join-Path $questRepo 'port/engine-ui')),
    ('-I'+(Join-Path $questRepo 'port/script-runtime')),
    ('-I'+(Join-Path $questRepo 'port/windows-foundation'))
)
& $Compiler '-std=c++17' '-O1' '-static' '-ffunction-sections' '-fdata-sections' @questLogIncludes @questLogSources '-Wl,--gc-sections' '-o' $questLogExecutable
if ($LASTEXITCODE -ne 0) { throw 'Quest Log native service fixture compile failed' }
$textData = Join-Path $questRepo '.local-inputs/windows-shared-assets/original-cache/data'
$assetsRoot = Join-Path $questRepo 'port/android-native/app/src/main/assets'
$questMovie = Join-Path $assetsRoot 'original-cache/data/menus/dqcharmenu_droid.swf'
$questActions = Join-Path $questRepo 'port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt'
$questMovieHash = (Get-FileHash -LiteralPath $questMovie -Algorithm SHA256).Hash.ToLowerInvariant()
if ($questMovieHash -ne '43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0') { throw 'Original Quest Log movie SHA-256 differs from audited source' }
& $questLogExecutable (Join-Path $questRepo 'port/level-world/reference/character-menu-profile-v51/cache') $textData $assetsRoot $questMovie $questActions
if ($LASTEXITCODE -ne 0) { throw 'Quest Log native service fixture failed' }
foreach ($genericQuestUnit in @('character_quest_progress_v1.cpp','character_quest_page_v1.cpp','quest_text_resolver_v1.cpp','runtime_quest_menu_v1.cpp','runtime_quest_menu_art_v1.cpp')) {
    & $Compiler '-std=c++17' '-Wall' '-Wextra' @questLogIncludes '-c' (Join-Path $PSScriptRoot $genericQuestUnit) '-o' (Join-Path $questBuild ($genericQuestUnit.Replace('.cpp','.o')))
    if ($LASTEXITCODE -ne 0) { throw "Generic CharacterState Quest unit compile failed: $genericQuestUnit" }
}
& $Compiler '-std=c++17' '-Wall' '-Wextra' @questIncludes '-c' (Join-Path $PSScriptRoot 'original_quest_runtime_bridge.cpp') '-o' (Join-Path $questBuild 'quest_runtime_bridge.o')
if ($LASTEXITCODE -ne 0) { throw 'Quest native bridge compile failed' }
& py '-3' (Join-Path $PSScriptRoot 'runtime_quest_menu_v1_art_fixture.py')
if ($LASTEXITCODE -ne 0) { throw 'Original Quest SWF art/ActionScript fixture failed' }
$sourceMenuPageExecutable = Join-Path $questBuild 'source_menu_page_composition_tests.exe'
& $Compiler '-std=c++17' '-static' '-Wall' '-Wextra' '-Werror' '-Wno-missing-field-initializers' `
    '-ffunction-sections' '-fdata-sections' @questLogIncludes `
    (Join-Path $PSScriptRoot 'source_menu_page_composition_tests.cpp') `
    '-Wl,--gc-sections' '-o' $sourceMenuPageExecutable
if ($LASTEXITCODE -ne 0) { throw 'Source menu page composition fixture compile failed' }
& $sourceMenuPageExecutable
if ($LASTEXITCODE -ne 0) { throw 'Source menu page composition fixture failed' }
