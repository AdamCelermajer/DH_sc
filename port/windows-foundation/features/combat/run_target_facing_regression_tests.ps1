param([string]$AssetRoot, [string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if (!$AssetRoot) { $AssetRoot = Join-Path $repo '.local-inputs/windows-source-clock-v19-preview-9/assets' }
$sharedBuild = [IO.Path]::GetFullPath((Join-Path $repo '.local-inputs/windows-foundation-build'))
if (!$BuildRoot) { $BuildRoot = Join-Path $repo '.local-inputs/target-facing-20261010-private-build' }
$build = [IO.Path]::GetFullPath($BuildRoot)
if ($build -eq $sharedBuild) { throw 'Target-facing regression runner must use a private BuildRoot' }
$null = New-Item -ItemType Directory -Path $build -Force

# Consume a stable copy of the existing dependency archives. The runner compiles
# the feature test and CombatSession into its private directory only.
$archives = @(
    'libfoundation_data.a', 'libcontent_xml.a', 'libdh2_freetype237.a',
    'librecovered_trigger_contacts.a', 'librecovered_content.a',
    'physics-backend/libdh2_box2d_201.a'
)
foreach ($archive in $archives) {
    $source = Join-Path $sharedBuild $archive
    if (!(Test-Path -LiteralPath $source -PathType Leaf)) { throw "Required dependency archive is absent: $source" }
    $destination = Join-Path $build $archive
    $null = New-Item -ItemType Directory -Path (Split-Path -Parent $destination) -Force
    Copy-Item -LiteralPath $source -Destination $destination -Force
}

$compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$includeDirs = @('port/windows-foundation', 'port/game-data', 'port/level-world', 'port/level-loader',
                 'port/engine-ui/vendor/freetype-2.3.7-hud/include', 'port/physics-backend/box2d-2.0.1/Include') |
    ForEach-Object { '-I' + (Join-Path $repo $_) }
$sources = @(
    'port/windows-foundation/features/combat/target_facing_regression_tests.cpp',
    'port/windows-foundation/features/combat/runtime_player_combo_chain_v1.cpp',
    'port/windows-foundation/features/equipment/runtime_player_locomotion_v1.cpp',
    'port/windows-foundation/combat_session.cpp',
    'port/level-world/player_equipment_queries_v1.cpp',
    'port/level-world/character_stance.cpp',
    'port/script-runtime/script_constants.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$libraries = @('libfoundation_data.a', 'libcontent_xml.a', 'libdh2_freetype237.a',
               'librecovered_trigger_contacts.a', 'librecovered_content.a',
               'physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $build $_ }
$output = Join-Path $build 'target_facing_regression_tests.exe'
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-static' @includeDirs @sources @libraries `
    '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' '-lole32' `
    '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output $AssetRoot
exit $LASTEXITCODE
