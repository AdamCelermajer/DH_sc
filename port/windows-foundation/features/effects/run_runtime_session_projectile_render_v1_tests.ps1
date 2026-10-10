$ErrorActionPreference = 'Stop'
$repository = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$build = Join-Path $repository '.local-inputs/runtime-session-projectile-render-v1'
$assets = Join-Path $repository '.local-inputs/runtime-session-projectile-v1/assets'
$compiler = Join-Path $repository '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$sourceModel = Join-Path $repository '.local-inputs/runtime-source-projectile-v1/assets/data/3D/projectiles/elemental_bolt_fire.bdae'
$modelDestination = Join-Path $assets 'data/3D/projectiles/elemental_bolt_fire.bdae'
New-Item -ItemType Directory -Force -Path (Split-Path $modelDestination), $build | Out-Null
Copy-Item -LiteralPath $sourceModel -Destination $modelDestination -Force

$libraryNames = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a',
    'librecovered_trigger_contacts.a','librecovered_content.a',
    'physics-backend/libdh2_box2d_201.a')
$sharedLibraries = Join-Path $repository '.local-inputs/windows-foundation-build'
$privateLibraries = Join-Path $build 'private-libs'
New-Item -ItemType Directory -Force -Path $privateLibraries | Out-Null
foreach ($libraryName in $libraryNames) {
    $source = Join-Path $sharedLibraries $libraryName
    $destination = Join-Path $privateLibraries (Split-Path $libraryName -Leaf)
    Copy-Item -LiteralPath $source -Destination $destination -Force
}

$sources = @(
    'port/windows-foundation/features/effects/runtime_session_projectile_render_v1_tests.cpp',
    'port/windows-foundation/features/effects/runtime_session_projectile_render_v1.cpp',
    'port/windows-foundation/original_scene.cpp',
    'port/windows-foundation/source_material_pass.cpp',
    'port/windows-foundation/content_paths.cpp',
    'port/windows-foundation/texture_loader.cpp'
) | ForEach-Object { Join-Path $repository $_ }
$libraries = $libraryNames | ForEach-Object {
    Join-Path $privateLibraries (Split-Path $_ -Leaf)
}
$exe = Join-Path $build 'runtime_session_projectile_render_v1_tests.exe'
& $compiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers `
    -Dfinite=isfinite -pedantic '-Iport/windows-foundation' `
    '-Iport/physics-backend/box2d-2.0.1/Include' -static @sources @libraries `
    -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 `
    -luuid -lcomdlg32 -ladvapi32 -o $exe
if ($LASTEXITCODE -ne 0) { throw 'Runtime projectile renderer test compile failed' }
$env:PATH = (Join-Path $repository '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin') +
    [IO.Path]::PathSeparator + $env:PATH
& $exe $assets
if ($LASTEXITCODE -ne 0) { throw 'Runtime projectile renderer test failed' }
