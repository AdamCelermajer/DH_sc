$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$taskBuild = Join-Path $taskRoot '.local-inputs/shared-source-result-test'
$taskLibraries = Join-Path $taskRoot '.local-inputs/windows-foundation-build'
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskCases = @('original_combat_properties_tests', 'source_result_world_tests')
foreach ($taskCase in $taskCases) {
    $taskSources = @(
        (Join-Path $taskRoot ('port/windows-foundation/tests/' + $taskCase + '.cpp')),
        (Join-Path $taskRoot 'port/windows-foundation/original_combat_properties.cpp')
    )
    if ($taskCase -eq 'source_result_world_tests') {
        $taskSources += (Join-Path $taskRoot 'port/windows-foundation/playable_actor_world.cpp')
    }
    $taskExe = Join-Path $taskBuild ($taskCase + '.exe')
    & $taskCompiler -std=c++17 -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
        -ffunction-sections -fdata-sections '-Wl,--gc-sections' @taskSources `
        ('-L' + $taskLibraries) -lfoundation_data -lrecovered_content -lcontent_xml -ldh2_freetype237 -o $taskExe
    if ($LASTEXITCODE -ne 0) { throw ('Shared result build failed: ' + $taskCase) }
    $taskAssets = Join-Path $taskRoot '.local-inputs/windows-shared-assets'
    if ($taskCase -eq 'source_result_world_tests') { & $taskExe $taskAssets $taskAssets }
    else { & $taskExe $taskAssets }
    if ($LASTEXITCODE -ne 0) { throw ('Shared result test failed: ' + $taskCase) }
}
