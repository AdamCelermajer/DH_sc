$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskBuild = Join-Path $taskRoot '.local-inputs/p16-context-tests-build'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$foundation = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$tests = @(
    @{ name='object_of_interest_owner_v1_tests'; sources=@('features/combat/object_of_interest_owner_v1.cpp','features/combat/auto_target_marker_v1.cpp','features/combat/object_of_interest_owner_v1_tests.cpp') },
    @{ name='context_button_v1_tests'; sources=@('features/combat/context_button_v1.cpp','features/combat/context_button_v1_tests.cpp') },
    @{ name='world_item_contact_v1_tests'; sources=@('features/loot/world_item_contact_v1.cpp','features/loot/world_item_contact_v1_tests.cpp') }
)
Push-Location $foundation
try {
    foreach ($t in $tests) {
        $exe = Join-Path $taskBuild ($t.name + '.exe')
        $compileArgs = @('-std=c++17','-O2','-DNDEBUG','-Wall','-Wextra','-Werror','-static') + $t.sources + @('-o', $exe)
        & $taskCompiler @compileArgs
        if ($LASTEXITCODE -ne 0) { throw "compile failed: $($t.name)" }
        & $exe | Tee-Object -FilePath (Join-Path $taskBuild ($t.name + '.log'))
        if ($LASTEXITCODE -ne 0) { throw "test failed: $($t.name)" }
    }
} finally { Pop-Location }
'context P16 tests passed'
