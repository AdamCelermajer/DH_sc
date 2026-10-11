param([string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path $RepoRoot).Path
$tool = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$out = Join-Path $repo '.local-inputs/death-checkpoint-snapshot-v1'
New-Item -ItemType Directory -Force -Path $out | Out-Null
if (-not (Test-Path -LiteralPath $tool)) { throw "Pinned LLVM-MinGW compiler not found: $tool" }
$exe = Join-Path $out 'death_checkpoint_snapshot_v1_tests.exe'
$args = @('-std=c++17','-O0','-static','-Wall','-Wextra','-Werror','-Wpedantic',
    'port/windows-foundation/features/persistence/death_checkpoint_snapshot_v1.cpp',
    'port/windows-foundation/features/persistence/source_character_restore_plan_v1.cpp',
    'port/windows-foundation/features/persistence/death_checkpoint_snapshot_codec_tests.cpp',
    'port/windows-foundation/playable_actor_world.cpp',
    'port/windows-foundation/original_combat_properties.cpp',
    'port/windows-foundation/combat_system.cpp',
    'port/windows-foundation/world.cpp')
$libs = @(
    (Join-Path $repo '.local-inputs/windows-foundation-build/libfoundation_data.a'),
    (Join-Path $repo '.local-inputs/windows-foundation-build/libcontent_xml.a'),
    (Join-Path $repo '.local-inputs/windows-foundation-build/libdh2_freetype237.a'),
    (Join-Path $repo '.local-inputs/windows-foundation-build/librecovered_trigger_contacts.a'),
    (Join-Path $repo '.local-inputs/windows-foundation-build/librecovered_content.a'),
    (Join-Path $repo '.local-inputs/windows-foundation-build/physics-backend/libdh2_box2d_201.a'))
foreach ($lib in $libs) { if (-not (Test-Path -LiteralPath $lib)) { throw "Required foundation library missing: $lib" } }
$args += @('-I',(Join-Path $repo 'port/windows-foundation'),'-I',(Join-Path $repo 'port/game-data'))
$args += $libs
$args += @('-lkernel32','-luser32','-lgdi32','-lwinspool','-lshell32','-lole32','-loleaut32','-luuid',
    '-lcomdlg32','-ladvapi32','-o',$exe)
Push-Location $repo
try {
    & $tool @args
    if ($LASTEXITCODE -ne 0) { throw 'Death checkpoint snapshot feature test compile failed' }
    & $exe
    if ($LASTEXITCODE -ne 0) { throw 'Death checkpoint snapshot feature test failed' }
} finally {
    Pop-Location
}
