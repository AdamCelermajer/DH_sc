[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$AssetRoot,
    [string]$Compiler
)

$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$AssetRoot = [IO.Path]::GetFullPath($AssetRoot)
if (-not (Test-Path -LiteralPath $AssetRoot -PathType Container)) {
    throw "Caller-supplied original AssetCatalog root not found: $AssetRoot"
}
if (-not $Compiler) {
    $Compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
}
if (-not (Test-Path -LiteralPath $Compiler)) { throw "Required LLVM-MinGW compiler not found: $Compiler" }

$build = Join-Path $repo '.local-inputs/runtime-player-locomotion-library-v1-tests'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$includes = @(
    'port/windows-foundation','port','port/game-data','port/scene-materials',
    'port/engine-resources','port/engine-skinning','port/engine-ui','port/engine-physics',
    'port/level-world','port/level-loader','port/script-runtime',
    'port/physics-backend/box2d-2.0.1/Include'
) | ForEach-Object { '-I' + (Join-Path $repo $_) }
$sources = @(
    'port/windows-foundation/features/equipment/runtime_player_locomotion_library_v1_tests.cpp',
    'port/windows-foundation/features/equipment/runtime_player_locomotion_library_v1.cpp',
    'port/windows-foundation/features/equipment/runtime_player_locomotion_v1.cpp',
    'port/windows-foundation/features/equipment/runtime_player_locomotion_program_v1.cpp',
    'port/windows-foundation/content_paths.cpp',
    'port/windows-foundation/asset_catalog.cpp',
    'port/windows-foundation/actor_profiles.cpp',
    'port/windows-foundation/original_combat_visual_plan.cpp',
    'port/game-data/class_preview_setup.cpp',
    'port/game-data/items.cpp',
    'port/game-data/animation_tables.cpp',
    'port/game-data/animation_selection.cpp',
    'port/level-world/player_equipment_queries_v1.cpp',
    'port/level-world/character_stance.cpp',
    'port/script-runtime/script_constants.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$archives = @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','libdh2_freetype237.a') |
    ForEach-Object { Join-Path $repo ('.local-inputs/windows-foundation-build/' + $_) }
foreach ($archive in $archives) {
    if (-not (Test-Path -LiteralPath $archive)) { throw "Required native archive not found: $archive" }
}
$exe = Join-Path $build 'runtime_player_locomotion_library_v1_tests.exe'
& $Compiler '-std=c++17' '-Wall' '-Wextra' '-Werror' '-Wno-missing-field-initializers' '-O1' '-static' `
    '-ffunction-sections' '-fdata-sections' '-Wl,--gc-sections' @includes @sources @archives '-o' $exe
if ($LASTEXITCODE -ne 0) { throw 'Strict LLVM-MinGW player locomotion library build failed' }
& $exe $AssetRoot
if ($LASTEXITCODE -ne 0) { throw 'Actual source player locomotion library test failed' }
