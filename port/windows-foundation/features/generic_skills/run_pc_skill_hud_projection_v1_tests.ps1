[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$buildRoot = Join-Path $repoRoot '.local-inputs\windows-foundation-build'
$assetRoot = Join-Path $repoRoot '.local-inputs\windows-source-clock-v19-preview-9\assets\original-cache\data\pydata'
$privateOutput = Join-Path $repoRoot '.local-inputs\pc-skill-hud-projection-v1'
New-Item -ItemType Directory -Force -Path $privateOutput | Out-Null
$exe = Join-Path $privateOutput 'pc_skill_hud_projection_v1_tests.exe'
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $Compiler -Parent) + ';' + $env:PATH
    & $Compiler -std=c++17 -Wall -Wextra -Werror -Wno-missing-field-initializers -O2 `
        (Join-Path $PSScriptRoot 'pc_skill_hud_projection_v1_tests.cpp') `
        (Join-Path $PSScriptRoot 'pc_skill_hud_projection_v1.cpp') `
        (Join-Path $PSScriptRoot 'generic_skills_page_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_activation_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_mana_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_cast_prepare_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_cast_coordinator_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_target_query_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_activation_session_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_animation_bank_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_progression_v1.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\features\faery_menu\hotty_cast_v1.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\features\faery_menu\hotty_cast_session_v1.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\features\faery_menu\hotty_character_cast_v1.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\features\faery_menu\hotty_source_use_v1.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\features\faery_menu\celest_cast_v1.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\features\faery_menu\celest_source_use_v1.cpp') `
        (Join-Path $repoRoot 'port\level-world\character_path_commands.cpp') `
        (Join-Path $repoRoot 'port\level-world\navigation_heading.cpp') `
        (Join-Path $repoRoot 'port\level-world\character_animation_ai.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\combat_session.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\playable_actor_world.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\save_store.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\game_save.cpp') `
        (Join-Path $repoRoot 'port\game-data\data.cpp') `
        (Join-Path $repoRoot 'port\game-data\skill_tables.cpp') `
        (Join-Path $repoRoot 'port\game-data\faery_tables.cpp') `
        (Join-Path $repoRoot 'port\game-data\animation_tables.cpp') `
        -L $buildRoot -lfoundation_data -lcontent_xml -ldh2_freetype237 `
        -lrecovered_trigger_contacts -lrecovered_content `
        (Join-Path $buildRoot 'physics-backend\libdh2_box2d_201.a') `
        -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 `
        -static -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'PC skill HUD projection compilation failed' }
    & $exe $assetRoot
    if ($LASTEXITCODE -ne 0) { throw 'PC skill HUD projection tests failed' }
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $exe) { Remove-Item -LiteralPath $exe }
}
