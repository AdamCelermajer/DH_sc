[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$assets = Join-Path $repoRoot 'port\android-native\app\src\main\assets\data'
$exe = Join-Path $PSScriptRoot 'runtime_skill_progression_v1_tests.exe'
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $Compiler -Parent) + ';' + $env:PATH
    & $Compiler -std=c++17 -Wall -Wextra -Werror -O2 `
        (Join-Path $PSScriptRoot 'runtime_skill_progression_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_progression_v1_tests.cpp') `
        (Join-Path $repoRoot 'port\game-data\data.cpp') `
        (Join-Path $repoRoot 'port\game-data\skill_tables.cpp') `
        -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'Runtime skill progression compilation failed' }
    $output = & $exe $assets
    if ($LASTEXITCODE -ne 0) { throw 'Runtime skill progression tests failed' }
    $output
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $exe) { Remove-Item -LiteralPath $exe }
}
