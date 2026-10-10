[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$assets = Join-Path $repoRoot 'port\android-native\app\src\main\assets\data'
$exe = Join-Path $PSScriptRoot 'generic_skills_page_v1_tests.exe'
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $Compiler -Parent) + ';' + $env:PATH
    & $Compiler -std=c++17 -Wall -Wextra -Werror `
        (Join-Path $PSScriptRoot 'generic_skills_page_v1.cpp') `
        (Join-Path $repoRoot 'port\game-data\data.cpp') `
        (Join-Path $repoRoot 'port\game-data\skill_tables.cpp') `
        (Join-Path $PSScriptRoot 'generic_skills_page_v1_tests.cpp') `
        -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'Generic skills page compilation failed' }
    $output = & $exe $assets
    if ($LASTEXITCODE -ne 0) { throw 'Generic skills page tests failed' }
    $output
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $exe) { Remove-Item -LiteralPath $exe }
}
