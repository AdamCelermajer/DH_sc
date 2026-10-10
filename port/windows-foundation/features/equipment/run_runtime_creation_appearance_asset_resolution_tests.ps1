[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$AssetRoot,
    [string]$Compiler
)

$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$AssetRoot = [IO.Path]::GetFullPath($AssetRoot)
if (-not (Test-Path -LiteralPath $AssetRoot -PathType Container)) {
    throw "Caller-supplied AssetCatalog root not found: $AssetRoot"
}
if (-not $Compiler) {
    $Compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
}
if (-not (Test-Path -LiteralPath $Compiler)) { throw "Required LLVM-MinGW compiler not found: $Compiler" }

# Keep this focused validation's executable and intermediate output separate
# from the active shared Windows Foundation build tree.
$build = Join-Path $repo '.local-inputs/equipment-appearance-asset-resolution-tests'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$includes = @(
    'port/windows-foundation','port','port/game-data','port/scene-materials',
    'port/engine-resources','port/engine-skinning','port/engine-ui','port/engine-physics',
    'port/level-world','port/level-loader','port/physics-backend/box2d-2.0.1/Include'
) | ForEach-Object { '-I' + (Join-Path $repo $_) }
$sources = @(
    'port/windows-foundation/features/equipment/runtime_creation_appearance_v1_tests.cpp',
    'port/windows-foundation/features/equipment/runtime_creation_appearance_v1.cpp',
    'port/game-data/class_preview_setup.cpp',
    'port/engine-skinning/visual_skin_owner_v6.cpp',
    'port/engine-skinning/visual_skin_selection_v6.cpp',
    'port/engine-skinning/skin_pose_cache_v32.cpp',
    'port/asset-payloads/sha256.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$archives = @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','libdh2_freetype237.a') |
    ForEach-Object { Join-Path $repo ('.local-inputs/windows-foundation-build/' + $_) }
foreach ($archive in $archives) {
    if (-not (Test-Path -LiteralPath $archive)) { throw "Required native archive not found: $archive" }
}
$exe = Join-Path $build 'runtime_creation_appearance_asset_resolution_tests.exe'
& $Compiler '-std=c++17' '-Wall' '-Wextra' '-Werror' '-Wno-missing-field-initializers' '-O1' '-static' `
    '-ffunction-sections' '-fdata-sections' '-Wl,--gc-sections' @includes @sources @archives '-o' $exe
if ($LASTEXITCODE -ne 0) { throw 'Strict LLVM-MinGW appearance asset-resolution build failed' }
& $exe $AssetRoot '--require-all-appearance-assets'
if ($LASTEXITCODE -ne 0) { throw 'Caller-root original appearance asset-resolution test failed' }
