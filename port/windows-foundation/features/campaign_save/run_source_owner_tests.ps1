[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference='Stop'
$campaignRepo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if(-not $Compiler){$Compiler=Join-Path $campaignRepo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'}
$campaignBuild=Join-Path $campaignRepo '.local-inputs/campaign-source-owner-component'
New-Item -ItemType Directory -Force -Path $campaignBuild | Out-Null
$campaignIncludes=@(('-I'+(Join-Path $campaignRepo 'port/game-data')),('-I'+(Join-Path $campaignRepo 'port/level-world')))
& $Compiler '-std=c++17' '-Wall' '-Wextra' '-Werror' @campaignIncludes '-c' (Join-Path $PSScriptRoot 'source_owner_capture.cpp') '-o' (Join-Path $campaignBuild 'capture.o')
if($LASTEXITCODE -ne 0){throw 'Source capture helper strict compile failed'}
$campaignSources=@(
 'port/windows-foundation/features/campaign_save/source_owner_capture_tests.cpp',
 'port/windows-foundation/features/campaign_save/source_owner_capture.cpp',
 'port/windows-foundation/features/campaign_save/campaign_snapshot.cpp',
 'port/level-world/character_menu_quests_v51.cpp',
 'port/game-data/quest_persistence_v51.cpp','port/game-data/quest_savegame_v1.cpp',
 'port/level-world/player_save_collections_writer_v45.cpp','port/level-world/object_save_restore_v3.cpp',
 'port/level-world/savegame_stream_v2.cpp','port/level-world/native_quest_runtime_v76.cpp',
 'port/level-world/quest_condition_compile_v70.cpp','port/level-world/native_conditions_runtime_v69.cpp',
 'port/level-world/native_conditions_table_v69.cpp','port/level-world/canonical_gameobject_base_owner_v1.cpp',
 'port/level-loader/game_event_runtime_v75.cpp','port/level-loader/game_event_manager_v50.cpp',
 'port/level-world/event_manager_owner_v12.cpp'
) | ForEach-Object {Join-Path $campaignRepo $_}
$campaignArchives=@('libfoundation_data.a','librecovered_content.a','libcontent_xml.a') | ForEach-Object {Join-Path $campaignRepo ('.local-inputs/windows-foundation-build/'+$_)}
$campaignExe=Join-Path $campaignBuild 'source_owner_tests.exe'
& $Compiler '-std=c++17' '-O1' '-static' '-ffunction-sections' '-fdata-sections' '-Wl,--gc-sections' @campaignIncludes @campaignSources @campaignArchives '-o' $campaignExe
if($LASTEXITCODE -ne 0){throw 'Source capture owner closure link failed'}
& $campaignExe (Join-Path $campaignRepo 'port/level-world/reference/character-menu-profile-v51/cache')
if($LASTEXITCODE -ne 0){throw 'Source capture actual-data component check failed'}
