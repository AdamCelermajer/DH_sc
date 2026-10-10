$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskBuild = Join-Path $taskRoot '.local-inputs/player-profile-properties-test'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskSources = @('tests/player_profile_properties_tests.cpp','player_profile_properties.cpp','original_combat_properties.cpp','features/equipment/equipment_adapter.cpp','world.cpp','playable_actor_world.cpp','game_save.cpp') |
    ForEach-Object { Join-Path $taskRoot ('port/windows-foundation/' + $_) }
$taskSources += Join-Path $taskRoot 'port/game-data/player_equipment_v3.cpp'
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $taskRoot ('.local-inputs/windows-foundation-build/' + $_) }
$taskExe = Join-Path $taskBuild 'player_profile_properties_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Player profile projection compile failed' }
& $taskExe (Join-Path $taskRoot '.local-inputs/windows-shared-assets') $taskBuild
if ($LASTEXITCODE -ne 0) { throw 'Player profile projection test failed' }
