param([string]$AssetRoot, [string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if (!$AssetRoot) { $AssetRoot = Join-Path $repo '.local-inputs/windows-source-clock-v19-preview-9/assets' }
$shared = Join-Path $repo '.local-inputs/windows-foundation-build'
$compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
if (!$BuildRoot) { $BuildRoot = Join-Path $repo '.local-inputs/runtime-player-profile-attack-bank-v1-build' }
$build = [IO.Path]::GetFullPath($BuildRoot)
$null = New-Item -ItemType Directory -Path $build -Force
$output = Join-Path $build 'runtime_player_profile_attack_bank_v1_tests.exe'

# Snapshot only the consumed archives into this runner's private output tree.
# The shared coherent build is read-only input; no objects or outputs are written there.
$archiveNames = @('libfoundation_data.a', 'libcontent_xml.a', 'libdh2_freetype237.a',
                  'librecovered_trigger_contacts.a', 'librecovered_content.a',
                  'physics-backend/libdh2_box2d_201.a')
$libraries = foreach ($name in $archiveNames) {
    $source = Join-Path $shared $name
    if (!(Test-Path -LiteralPath $source)) { throw "Required coherent input archive is missing: $source" }
    $destination = Join-Path $build $name
    $parent = Split-Path -Parent $destination
    $null = New-Item -ItemType Directory -Path $parent -Force
    $before = (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash
    Copy-Item -LiteralPath $source -Destination $destination -Force
    $after = (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash
    if ($before -ne $after) { throw "Shared input archive changed during snapshot: $source" }
    $destination
}

$includeDirs = @('port/windows-foundation', 'port/game-data', 'port/level-world', 'port/level-loader',
                 'port/engine-ui/vendor/freetype-2.3.7-hud/include', 'port/physics-backend/box2d-2.0.1/Include') |
    ForEach-Object { '-I' + (Join-Path $repo $_) }
$sources = @(
    'port/windows-foundation/features/combat/runtime_player_profile_attack_bank_v1_tests.cpp',
    'port/windows-foundation/features/combat/runtime_player_profile_attack_bank_v1.cpp',
    'port/windows-foundation/features/combat/runtime_player_combo_chain_v1.cpp',
    'port/windows-foundation/features/equipment/runtime_player_locomotion_v1.cpp',
    'port/windows-foundation/actor_profiles.cpp',
    'port/windows-foundation/original_combat_visual_plan.cpp',
    'port/windows-foundation/content_paths.cpp',
    'port/script-runtime/script_constants.cpp',
    'port/level-world/player_equipment_queries_v1.cpp',
    'port/level-world/character_stance.cpp'
) | ForEach-Object { Join-Path $repo $_ }
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-static' @includeDirs @sources @libraries `
    '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' '-lole32' `
    '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output $AssetRoot
exit $LASTEXITCODE
