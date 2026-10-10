[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
}
if (-not (Test-Path -LiteralPath $Compiler)) { throw "Required LLVM-MinGW compiler not found: $Compiler" }
$build = Join-Path $repo '.local-inputs/windows-foundation-build/source-equipment-renderer-smoke'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$includes = @(
    'port/windows-foundation','port','port/game-data','port/scene-materials',
    'port/engine-skinning','port/engine-ui','port/engine-physics','port/level-world',
    'port/level-loader','port/physics-backend/box2d-2.0.1/Include'
) | ForEach-Object { '-I' + (Join-Path $repo $_) }
$sources = @(
    'port/windows-foundation/features/equipment/source_equipment_renderer_smoke.cpp',
    'port/windows-foundation/features/equipment/source_equipment_render_bridge.cpp',
    'port/windows-foundation/features/equipment/source_equipment_material_binding.cpp',
    'port/windows-foundation/features/effects/effects_material_binding.cpp',
    'port/windows-foundation/asset_catalog.cpp',
    'port/windows-foundation/content_paths.cpp',
    'port/windows-foundation/texture_loader.cpp',
    'port/windows-foundation/platform_win32.cpp',
    'port/windows-foundation/renderer.cpp',
    'port/windows-foundation/original_character.cpp',
    'port/engine-skinning/visual_skin_owner_v6.cpp',
    'port/engine-skinning/visual_skin_selection_v6.cpp',
    'port/engine-skinning/skin_pose_cache_v32.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$archives = @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','libdh2_freetype237.a') |
    ForEach-Object { Join-Path $repo ('.local-inputs/windows-foundation-build/' + $_) }
foreach ($archive in $archives) {
    if (-not (Test-Path -LiteralPath $archive)) { throw "Required native archive not found: $archive" }
}
$exe = Join-Path $build 'source_equipment_renderer_smoke.exe'
& $Compiler '-std=c++17' '-Wall' '-Wextra' '-Werror' '-O1' '-static' '-ffunction-sections' '-fdata-sections' `
    '-Wl,--gc-sections' @includes @sources @archives '-lopengl32' '-lgdi32' '-luser32' '-o' $exe
if ($LASTEXITCODE -ne 0) { throw 'Strict LLVM-MinGW source equipment WGL smoke compile/link failed' }
& $exe $repo
if ($LASTEXITCODE -ne 0) { throw 'Actual-cache source equipment WGL smoke failed' }
