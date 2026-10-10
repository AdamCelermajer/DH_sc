$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskBuild = Join-Path $PSScriptRoot 'test-output/container-game-save-v1'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
if (-not (Test-Path $taskCompiler)) { throw "Pinned native compiler is missing: $taskCompiler" }
$taskSources = @(
    'features/interactions/session_container_game_save_v1_tests.cpp',
    'features/interactions/world_object_container_state_v1.cpp',
    'game_save.cpp', 'world.cpp', 'playable_actor_world.cpp', 'original_combat_properties.cpp'
) | ForEach-Object { Join-Path $taskRoot ('port/windows-foundation/' + $_) }
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a',
    'librecovered_trigger_contacts.a','librecovered_content.a',
    'physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $taskRoot ('.local-inputs/windows-foundation-build/' + $_) }
foreach ($library in $taskLibraries) {
    if (-not (Test-Path $library)) { throw "Required foundation archive is missing: $library" }
}
$taskExe = Join-Path $taskBuild 'session_container_game_save_v1_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Container GameSave feature test compile failed' }
$taskSave = Join-Path $taskBuild 'container.save'
& $taskExe (Join-Path $taskRoot '.local-inputs/windows-shared-assets') $taskSave |
    Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw 'Container GameSave feature test failed' }
