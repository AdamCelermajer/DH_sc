[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$assets = Join-Path $repoRoot '.local-inputs\windows-source-clock-v19-preview-9\assets'
$build = Join-Path $repoRoot '.local-inputs\windows-foundation-build'
$exe = Join-Path $PSScriptRoot 'runtime_skills_text_v1_tests.exe'
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $Compiler -Parent) + ';' + $env:PATH
    & $Compiler -std=c++17 -Wall -Wextra -Werror `
        (Join-Path $PSScriptRoot 'runtime_skills_text_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skills_text_v1_tests.cpp') `
        (Join-Path $PSScriptRoot 'generic_skills_page_v1.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\features\character_menu\menu_text_layout_v1.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\features\character_menu\skill_page_text_projection_v1.cpp') `
        (Join-Path $repoRoot 'port\game-data\data.cpp') `
        (Join-Path $repoRoot 'port\game-data\skill_tables.cpp') `
        (Join-Path $repoRoot 'port\game-data\class_tables.cpp') `
        (Join-Path $repoRoot 'port\game-data\properties.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\features\generic_skills\runtime_skill_mana_v1.cpp') `
        (Join-Path $build 'libfoundation_data.a') `
        (Join-Path $build 'libcontent_xml.a') `
        (Join-Path $build 'libdh2_freetype237.a') `
        (Join-Path $build 'librecovered_trigger_contacts.a') `
        (Join-Path $build 'librecovered_content.a') `
        (Join-Path $build 'physics-backend\libdh2_box2d_201.a') `
        -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'Runtime Skills text compilation failed' }
    $gold = Join-Path $repoRoot 'port\engine-ui\reference\hud-formatting-v1\skill-class-gold.bin'
    $output = & $exe $assets $gold
    if ($LASTEXITCODE -ne 0) { throw 'Runtime Skills text tests failed' }
    $output
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $exe) { Remove-Item -LiteralPath $exe }
}
