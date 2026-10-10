param([string]$AssetRoot, [string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
if (!$AssetRoot) { $AssetRoot = Join-Path $repo '.local-inputs/windows-shared-assets' }
if (!$BuildRoot) { $BuildRoot = Join-Path $repo '.local-inputs/game-save-physical-presence-test' }
$build = [IO.Path]::GetFullPath($BuildRoot)
$compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$shared = Join-Path $repo '.local-inputs/windows-foundation-build'
$null = New-Item -ItemType Directory -Path $build -Force

# Snapshot static inputs privately; compile all changed ActorState/GameSave
# consumers directly so this test cannot accidentally use an old ABI archive.
$libraries = @('libfoundation_data.a', 'libcontent_xml.a', 'libdh2_freetype237.a',
               'librecovered_trigger_contacts.a', 'librecovered_content.a',
               'physics-backend/libdh2_box2d_201.a') | ForEach-Object {
    $source = Join-Path $shared $_
    if (!(Test-Path -LiteralPath $source)) { throw "Required archive is missing: $source" }
    $before = (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash
    $destination = Join-Path $build $_
    $null = New-Item -ItemType Directory -Path (Split-Path -Parent $destination) -Force
    Copy-Item -LiteralPath $source -Destination $destination -Force
    if ($before -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash) {
        throw "Shared archive changed during snapshot: $source"
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
    'port/windows-foundation/tests/game_save_physical_presence_tests.cpp',
    'port/windows-foundation/actor_state.cpp',
    'port/windows-foundation/game_save.cpp',
    'port/windows-foundation/playable_actor_world.cpp',
    'port/windows-foundation/world.cpp',
    'port/windows-foundation/original_combat_properties.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$output = Join-Path $build 'game_save_physical_presence_tests.exe'
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-Dfinite=_finite' '-static' @includes @sources @libraries `
    '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' '-lole32' `
    '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $output
if ($LASTEXITCODE -ne 0) { throw 'Physical presence GameSave regression compile failed' }
$savePath = Join-Path $build 'physical-presence.save'
& $output $AssetRoot $savePath | Tee-Object -FilePath (Join-Path $build 'run.log')
if ($LASTEXITCODE -ne 0) { throw 'Physical presence GameSave regression failed' }
Get-FileHash -Algorithm SHA256 -LiteralPath $output | Format-List
Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $build 'run.log') | Format-List
