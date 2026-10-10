[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$buildRoot = Join-Path $repoRoot '.local-inputs\windows-foundation-build'
$privateTestOutput = Join-Path $repoRoot '.local-inputs\windows-skill-cast-session-test'
New-Item -ItemType Directory -Force -Path $privateTestOutput | Out-Null
$exe = Join-Path $privateTestOutput 'runtime_skill_activation_session_v1_tests.exe'
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $Compiler -Parent) + ';' + $env:PATH
    # This linked test exercises real Warrior and Rogue coordinator branches.
    # The Hotty coordinator branch is syntax-checked in the normal build, while its
    # separately-owned effect adapter has its own connected Session runner;
    # its large FX/resource linker closure is not part of this test binary.
    & $Compiler -std=c++17 -Wall -Wextra -Werror -Wno-missing-field-initializers -O2 `
        -DDH_RUNTIME_SKILL_CAST_WARRIOR_ONLY_TEST `
        (Join-Path $PSScriptRoot 'runtime_skill_activation_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_mana_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_cast_prepare_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_cast_coordinator_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_target_query_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_activation_session_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_animation_bank_v1.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_activation_session_v1_tests.cpp') `
        (Join-Path $PSScriptRoot 'runtime_skill_progression_v1.cpp') `
        (Join-Path $PSScriptRoot 'generic_skills_page_v1.cpp') `
        (Join-Path $PSScriptRoot '..\skills_animation\skill_animation_program.cpp') `
        (Join-Path $repoRoot 'port\level-world\character_path_commands.cpp') `
        (Join-Path $repoRoot 'port\level-world\navigation_heading.cpp') `
        (Join-Path $repoRoot 'port\level-world\character_animation_ai.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\combat_session.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\playable_actor_world.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\save_store.cpp') `
        (Join-Path $repoRoot 'port\windows-foundation\game_save.cpp') `
        (Join-Path $repoRoot 'port\game-data\data.cpp') `
        (Join-Path $repoRoot 'port\game-data\skill_tables.cpp') `
        (Join-Path $repoRoot 'port\game-data\animation_tables.cpp') `
        -L $buildRoot -lfoundation_data -lcontent_xml -ldh2_freetype237 `
        -lrecovered_trigger_contacts -lrecovered_content `
        (Join-Path $buildRoot 'physics-backend\libdh2_box2d_201.a') `
        -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 `
        -static -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'Runtime skill activation session compilation failed' }
    $output = & $exe $repoRoot
    if ($LASTEXITCODE -ne 0) { $output; throw 'Runtime skill activation session tests failed' }
    $output
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $exe) { Remove-Item -LiteralPath $exe }
}
