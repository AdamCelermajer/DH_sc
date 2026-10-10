$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskBuild = Join-Path $taskRoot '.local-inputs/session-source-attack-bank-test'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskSources = @('tests/combat_session_source_attack_bank_tests.cpp','combat_session.cpp','actor_combat_runtime.cpp','world.cpp','playable_actor_world.cpp','original_combat_properties.cpp','player_profile_properties.cpp','game_save.cpp','features/combat/runtime_player_combo_chain_v1.cpp','features/combat/runtime_player_profile_attack_bank_v1.cpp','features/equipment/runtime_player_locomotion_v1.cpp','../level-world/player_equipment_queries_v1.cpp','../level-world/character_stance.cpp') |
    ForEach-Object { Join-Path $taskRoot ('port/windows-foundation/' + $_) }
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $taskRoot ('.local-inputs/windows-foundation-build/' + $_) }
$taskExe = Join-Path $taskBuild 'combat_session_source_attack_bank_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Session source attack bank compile failed' }
& $taskExe (Join-Path $taskRoot '.local-inputs/windows-source-clock-v19-preview-9/assets') | Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw 'Session source attack bank test failed' }
