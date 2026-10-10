[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$outputRoot = Join-Path $repoRoot '.local-inputs\pc-gameplay-hud-v1-tests'
New-Item -ItemType Directory -Force -Path $outputRoot | Out-Null
$exe = Join-Path $outputRoot 'pc_gameplay_hud_v1_tests.exe'
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $Compiler -Parent) + ';' + $env:PATH
    & $Compiler -std=c++17 -Wall -Wextra -Werror -Wno-missing-field-initializers -O2 `
        (Join-Path $PSScriptRoot 'pc_gameplay_hud_v1_tests.cpp') `
        (Join-Path $PSScriptRoot 'pc_gameplay_hud_v1.cpp') `
        (Join-Path $PSScriptRoot 'pc_gameplay_hud_source_art_v1.cpp') `
        (Join-Path $PSScriptRoot 'pc_gameplay_hud_button_art_v1.cpp') `
        (Join-Path $PSScriptRoot 'pc_cooldown_frame_v1.cpp') `
        (Join-Path $PSScriptRoot '..\skill_ui\original_skill_art.cpp') `
        (Join-Path $PSScriptRoot '..\platform_input\semantic_input.cpp') `
        -static -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'PC gameplay HUD strict C++17 compile failed' }
    & $exe
    if ($LASTEXITCODE -ne 0) { throw 'PC gameplay HUD tests failed' }
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $exe) { Remove-Item -LiteralPath $exe }
}
