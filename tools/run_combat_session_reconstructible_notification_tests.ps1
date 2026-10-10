param([string]$AssetRoot, [string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
if (!$AssetRoot) { $AssetRoot = Join-Path $repo '.local-inputs/windows-shared-assets' }
if (!$BuildRoot) { $BuildRoot = Join-Path $repo '.local-inputs/session-reconstructible-notification-test' }
$build = [IO.Path]::GetFullPath($BuildRoot)
$compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$shared = Join-Path $repo '.local-inputs/windows-foundation-build'
$null = New-Item -ItemType Directory -Path $build -Force

# Snapshot coherent static inputs privately. Session, ActorState, world and
# GameSave sources compile directly to avoid old header/archive ABI consumers.
$libraries = @('libfoundation_data.a', 'libcontent_xml.a', 'libdh2_freetype237.a',
               'librecovered_trigger_contacts.a', 'librecovered_content.a',
               'physics-backend/libdh2_box2d_201.a') | ForEach-Object {
    $source = Join-Path $shared $_
    if (!(Test-Path -LiteralPath $source)) { throw "Required coherent archive is missing: $source" }
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
    'port/windows-foundation/tests/combat_session_reconstructible_notification_tests.cpp',
    'port/windows-foundation/combat_session.cpp',
    'port/windows-foundation/retained_sequence_playback.cpp',
    'port/windows-foundation/actor_combat_runtime.cpp',
    'port/windows-foundation/actor_state.cpp',
    'port/windows-foundation/game_save.cpp',
    'port/windows-foundation/playable_actor_world.cpp',
    'port/windows-foundation/world.cpp',
    'port/windows-foundation/original_combat_properties.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$output = Join-Path $build 'combat_session_reconstructible_notification_tests.exe'
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-Dfinite=_finite' '-static' @includes @sources @libraries `
    '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' '-lole32' `
    '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $output
if ($LASTEXITCODE -ne 0) { throw 'Reconstructible animation notification test compile failed' }
$savePath = Join-Path $build 'end34-checkpoint.save'
& $output $AssetRoot $savePath 2>&1 | Tee-Object -FilePath (Join-Path $build 'run.log')
if ($LASTEXITCODE -ne 0) { throw "Reconstructible animation notification test failed; see $build/run.log" }
Get-FileHash -Algorithm SHA256 -LiteralPath $output | Format-List
Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $build 'run.log') | Format-List
