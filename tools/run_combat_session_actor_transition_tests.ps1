param([string]$AssetRoot,[string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
if (!$AssetRoot) { $AssetRoot = Join-Path $taskRoot '.local-inputs/windows-source-clock-v19-preview-9/assets' }
if (!$BuildRoot) { $BuildRoot = Join-Path $taskRoot '.local-inputs/session-actor-transition-test' }
$taskBuild = [IO.Path]::GetFullPath($BuildRoot)
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$sharedBuild = Join-Path $taskRoot '.local-inputs/windows-foundation-build'
$null = New-Item -ItemType Directory -Force -Path $taskBuild
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a',
    'librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') | ForEach-Object {
    $source = Join-Path $sharedBuild $_
    if (!(Test-Path -LiteralPath $source)) { throw "Required shared archive is missing: $source" }
    $before = (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash
    $destination = Join-Path $taskBuild $_
    $null = New-Item -ItemType Directory -Force -Path (Split-Path -Parent $destination)
    Copy-Item -LiteralPath $source -Destination $destination -Force
    if ($before -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash) { throw "Shared archive changed during snapshot: $source" }
    $destination
}
$taskIncludes = @('port/windows-foundation','port/game-data','port/level-world','port/level-loader',
    'port/scene-materials','port/engine-animation','port/engine-skinning','port/engine-resources',
    'port/engine-ui/vendor/freetype-2.3.7-hud/include','port/physics-backend/box2d-2.0.1/Include') |
    ForEach-Object { '-I' + (Join-Path $taskRoot $_) }
$taskSources = @('port/windows-foundation/tests/combat_session_actor_transition_tests.cpp',
    'port/windows-foundation/combat_session.cpp','port/windows-foundation/actor_combat_runtime.cpp',
    'port/windows-foundation/combat_system.cpp',
    'port/windows-foundation/retained_sequence_playback.cpp','port/windows-foundation/world.cpp',
    'port/windows-foundation/playable_actor_world.cpp','port/windows-foundation/original_combat_properties.cpp',
    'port/windows-foundation/player_profile_properties.cpp','port/windows-foundation/actor_state.cpp',
    'port/windows-foundation/actor_profiles.cpp',
    'port/windows-foundation/game_save.cpp','port/windows-foundation/features/combat/runtime_player_combo_chain_v1.cpp',
    'port/windows-foundation/features/combat/runtime_player_profile_attack_bank_v1.cpp',
    'port/windows-foundation/features/equipment/runtime_player_locomotion_v1.cpp',
    'port/windows-foundation/features/skills_animation/skill_animation_program.cpp',
    'port/level-world/player_equipment_queries_v1.cpp','port/level-world/character_stance.cpp') |
    ForEach-Object { Join-Path $taskRoot $_ }
$taskExe = Join-Path $taskBuild 'combat_session_actor_transition_tests.exe'
& $taskCompiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' '-Wno-missing-field-initializers' `
    '-Dfinite=_finite' '-static' @taskIncludes @taskSources @taskLibraries `
    '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' '-lole32' `
    '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Session actor-transition test compile failed' }
& $taskExe $AssetRoot 2>&1 | Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw "Session actor-transition test failed; see $taskBuild/run.log" }
Get-FileHash -Algorithm SHA256 -LiteralPath $taskExe | Format-List
Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $taskBuild 'run.log') | Format-List
