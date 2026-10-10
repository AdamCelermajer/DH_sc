$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$taskBuild = Join-Path $taskRoot '.local-inputs/source-calculated-hit-runtime-test'
$taskLibraries = Join-Path $taskRoot '.local-inputs/windows-foundation-build'
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskAssets = Join-Path $taskRoot '.local-inputs/windows-shared-assets'
$taskFaeryAssets = Join-Path $taskRoot '.local-inputs/character-skills'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskSources = @(
    (Join-Path $taskRoot 'port/windows-foundation/tests/source_calculated_hit_runtime_tests.cpp'),
    (Join-Path $taskRoot 'port/windows-foundation/playable_actor_world.cpp'),
    (Join-Path $taskRoot 'port/game-data/faery_tables.cpp')
)
$taskExe = Join-Path $taskBuild 'source_calculated_hit_runtime_tests.exe'
& $taskCompiler -std=c++17 -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    -ffunction-sections -fdata-sections '-Wl,--gc-sections' @taskSources `
    ('-L' + $taskLibraries) -lfoundation_data -lrecovered_content -lcontent_xml -ldh2_freetype237 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Source-calculated runtime test build failed' }
& $taskExe $taskAssets $taskAssets $taskFaeryAssets
if ($LASTEXITCODE -ne 0) { throw 'Source-calculated runtime test failed' }
