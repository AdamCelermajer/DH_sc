[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$buildRoot = Join-Path $repoRoot '.local-inputs\windows-foundation-build'
$privateTestOutput = Join-Path $repoRoot '.local-inputs\windows-skill-animation-bank-rank0-test'
New-Item -ItemType Directory -Force -Path $privateTestOutput | Out-Null
$exe = Join-Path $privateTestOutput 'runtime_skill_animation_bank_rank0_v1_tests.exe'
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $Compiler -Parent) + ';' + $env:PATH
    & $Compiler -std=c++17 -Wall -Wextra -Werror -Wno-missing-field-initializers -O2 `
        (Join-Path $PSScriptRoot 'runtime_skill_animation_bank_rank0_v1_tests.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_animation_bank_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_activation_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_mana_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_progression_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_cast_prepare_v1.cpp') `
        (Join-Path $PSScriptRoot '..\skills_animation\skill_animation_program.cpp') `
        (Join-Path $repoRoot 'port\game-data\data.cpp') `
        (Join-Path $repoRoot 'port\game-data\skill_tables.cpp') `
        (Join-Path $repoRoot 'port\game-data\animation_tables.cpp') `
        -L $buildRoot -lfoundation_data -lcontent_xml -ldh2_freetype237 `
        -lrecovered_trigger_contacts -lrecovered_content `
        (Join-Path $buildRoot 'physics-backend\libdh2_box2d_201.a') `
        -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 `
        -static -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'Rank-zero skill animation bank test compilation failed' }
    $output = & $exe $repoRoot
    if ($LASTEXITCODE -ne 0) { $output; throw 'Rank-zero skill animation bank tests failed' }
    $output
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $exe) { Remove-Item -LiteralPath $exe }
}
