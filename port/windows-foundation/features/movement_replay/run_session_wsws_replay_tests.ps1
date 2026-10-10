$ErrorActionPreference = 'Stop'
$taskWorkspace = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$taskCompiler = Join-Path $taskWorkspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskOutput = Join-Path $taskWorkspace '.local-inputs/windows-foundation-build/session_wsws_replay_tests.exe'
$taskCsv = Join-Path $taskWorkspace '.local-inputs/windows-foundation-build/session_wsws_replay.csv'
$taskSources = @(
    'port/windows-foundation/features/movement_replay/session_wsws_replay_tests.cpp',
    'port/windows-foundation/features/platform_input/semantic_input.cpp'
) | ForEach-Object { Join-Path $taskWorkspace $_ }
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $taskWorkspace ('.local-inputs/windows-foundation-build/' + $_) }
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -Iport/windows-foundation -static @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskOutput
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $taskOutput $taskWorkspace $taskCsv
exit $LASTEXITCODE
