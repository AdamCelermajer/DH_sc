param([string]$AssetRoot,[string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
$taskBuild = if ($BuildRoot) { [IO.Path]::GetFullPath($BuildRoot) } else { Join-Path $taskRoot '.local-inputs/b035-session-push-effect-test' }
$taskAssets = if ($AssetRoot) { $AssetRoot } else { Join-Path $taskRoot '.local-inputs/windows-source-clock-v19-preview-9/assets' }
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$null = New-Item -ItemType Directory -Force -Path $taskBuild
$taskSources = @(
 'port/windows-foundation/features/combat/b035_session_push_effect_v1_tests.cpp',
 'port/windows-foundation/features/combat/b035_session_push_effect_v1.cpp',
 'port/windows-foundation/combat_session.cpp','port/windows-foundation/actor_combat_runtime.cpp',
 'port/windows-foundation/combat_system.cpp','port/windows-foundation/retained_sequence_playback.cpp',
 'port/windows-foundation/world.cpp','port/windows-foundation/playable_actor_world.cpp',
 'port/windows-foundation/original_combat_properties.cpp','port/windows-foundation/player_profile_properties.cpp',
 'port/windows-foundation/actor_state.cpp','port/windows-foundation/actor_profiles.cpp',
 'port/windows-foundation/features/combat/runtime_player_combo_chain_v1.cpp',
 'port/windows-foundation/features/combat/runtime_player_profile_attack_bank_v1.cpp',
 'port/windows-foundation/features/skills_animation/skill_animation_program.cpp',
 'port/windows-foundation/features/equipment/runtime_player_locomotion_v1.cpp',
 'port/level-world/player_equipment_queries_v1.cpp','port/level-world/character_stance.cpp') |
 ForEach-Object { Join-Path $taskRoot $_ }
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a',
 'librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') |
 ForEach-Object { Join-Path $taskRoot ('.local-inputs/windows-foundation-build/' + $_) }
$taskExe = Join-Path $taskBuild 'b035_session_push_effect_v1_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
 @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'B035 focused test compile failed' }
& $taskExe $taskAssets 2>&1 | Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw 'B035 focused test failed' }
Get-FileHash -Algorithm SHA256 -LiteralPath $taskExe | Format-List
Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $taskBuild 'run.log') | Format-List
