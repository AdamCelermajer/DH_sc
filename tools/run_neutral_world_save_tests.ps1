$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskBuild = Join-Path $taskRoot '.local-inputs/neutral-world-save-test'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskSources = @('tests/neutral_world_save_tests.cpp','world.cpp','game_save.cpp','playable_actor_world.cpp','original_combat_properties.cpp') |
    ForEach-Object { Join-Path $taskRoot ('port/windows-foundation/' + $_) }
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $taskRoot ('.local-inputs/windows-foundation-build/' + $_) }
$taskExe = Join-Path $taskBuild 'neutral_world_save_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Neutral world/save test compile failed' }
& $taskExe (Join-Path $taskRoot '.local-inputs/windows-shared-assets') (Join-Path $taskBuild 'world.save') | Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw 'Neutral world/save test failed' }
