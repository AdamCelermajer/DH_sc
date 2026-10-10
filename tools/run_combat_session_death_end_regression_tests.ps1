$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskBuild = Join-Path $taskRoot '.local-inputs/session-death-end-regression-test'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskSources = @(
    'port/windows-foundation/features/combat/death_end_regression_tests.cpp',
    'port/windows-foundation/combat_session.cpp',
    'port/windows-foundation/retained_sequence_playback.cpp',
    'port/windows-foundation/actor_combat_runtime.cpp',
    'port/windows-foundation/world.cpp',
    'port/windows-foundation/playable_actor_world.cpp',
    'port/windows-foundation/original_combat_properties.cpp'
) | ForEach-Object { Join-Path $taskRoot $_ }
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $taskRoot ('.local-inputs/windows-foundation-build/' + $_) }
$taskExe = Join-Path $taskBuild 'combat_session_death_end_regression_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -Iport/windows-foundation -static `
    @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Session death End34 regression compile failed' }
& $taskExe (Join-Path $taskRoot '.local-inputs/windows-shared-assets') 2>&1 | Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw "Session death End34 regression failed; see $taskBuild/run.log" }
