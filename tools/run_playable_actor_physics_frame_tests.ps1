param([string]$AssetRoot, [string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
if (!$AssetRoot) { $AssetRoot = Join-Path $repo '.local-inputs/windows-shared-assets' }
if (!$BuildRoot) { $BuildRoot = Join-Path $repo '.local-inputs/playable-actor-physics-frame-test' }
$build = [IO.Path]::GetFullPath($BuildRoot)
$compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$shared = Join-Path $repo '.local-inputs/windows-foundation-build'
$null = New-Item -ItemType Directory -Path $build -Force

# Snapshot immutable build inputs to this runner's private directory. Never
# write/relink the shared archives while the integration lead is building.
$libraries = @('libfoundation_data.a', 'libcontent_xml.a', 'libdh2_freetype237.a',
               'librecovered_trigger_contacts.a', 'librecovered_content.a',
               'physics-backend/libdh2_box2d_201.a') | ForEach-Object {
    $source = Join-Path $shared $_
    if (!(Test-Path -LiteralPath $source)) { throw "Required coherent input archive is missing: $source" }
    $before = (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash
    $destination = Join-Path $build $_
    $null = New-Item -ItemType Directory -Path (Split-Path -Parent $destination) -Force
    Copy-Item -LiteralPath $source -Destination $destination -Force
    if ($before -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash) {
        throw "Shared input archive changed during snapshot: $source"
    }
    $destination
}
$includes = @('port/windows-foundation', 'port/game-data', 'port/level-world',
              'port/level-loader', 'port/scene-materials', 'port/engine-animation',
              'port/engine-skinning', 'port/engine-resources',
              'port/engine-ui/vendor/freetype-2.3.7-hud/include',
              'port/physics-backend/box2d-2.0.1/Include') |
    ForEach-Object { '-I' + (Join-Path $repo $_) }
$sources = @(
    'port/windows-foundation/tests/playable_actor_physics_frame_tests.cpp',
    'port/windows-foundation/combat_session.cpp',
    'port/windows-foundation/retained_sequence_playback.cpp',
    'port/windows-foundation/actor_combat_runtime.cpp',
    'port/windows-foundation/original_combat_properties.cpp',
    'port/windows-foundation/original_combat_visual_plan.cpp',
    'port/windows-foundation/original_melee_bindings.cpp',
    'port/windows-foundation/original_actor_properties.cpp',
    'port/windows-foundation/original_actor_body_plan.cpp',
    'port/windows-foundation/original_actor_bounds.cpp',
    'port/windows-foundation/original_actor_physical.cpp',
    'port/windows-foundation/original_actor_navigation.cpp',
    'port/windows-foundation/playable_actor_bodies.cpp',
    'port/windows-foundation/world.cpp',
    'port/level-world/physical_world.cpp',
    'port/level-world/character_script_collision.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$output = Join-Path $build 'playable_actor_physics_frame_tests.exe'
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-Wno-unused-value' '-Dfinite=_finite' `
    '-isystem' (Join-Path $repo 'port/physics-backend/box2d-2.0.1/Include') `
    '-static' @includes @sources @libraries `
    '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' '-lole32' `
    '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output $AssetRoot
exit $LASTEXITCODE
