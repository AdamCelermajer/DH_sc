param([string]$AssetRoot, [string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if (!$AssetRoot) { $AssetRoot = Join-Path $repo '.local-inputs/windows-source-clock-v19-preview-9/assets' }
$compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
if (!$BuildRoot) { $BuildRoot = Join-Path $repo '.local-inputs/windows-foundation-build' }
$build = [IO.Path]::GetFullPath($BuildRoot)
$null = New-Item -ItemType Directory -Path $build -Force
$output = Join-Path $build 'runtime_player_incoming_animation_bank_v1_tests.exe'
$includeDirs = @('port/windows-foundation', 'port/game-data', 'port/level-world', 'port/level-loader',
                 'port/engine-ui/vendor/freetype-2.3.7-hud/include', 'port/physics-backend/box2d-2.0.1/Include') |
    ForEach-Object { '-I' + (Join-Path $repo $_) }
$sources = @(
    'port/windows-foundation/features/combat/runtime_player_incoming_animation_bank_v1_tests.cpp',
    'port/windows-foundation/features/combat/runtime_player_incoming_animation_bank_v1.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$libraries = @('libfoundation_data.a', 'libcontent_xml.a', 'libdh2_freetype237.a',
               'librecovered_trigger_contacts.a', 'librecovered_content.a',
               'physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $build $_ }
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-static' @includeDirs @sources @libraries `
    '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' '-lole32' `
    '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output $AssetRoot
exit $LASTEXITCODE
