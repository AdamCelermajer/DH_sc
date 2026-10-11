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
    '-o',$exe)
Push-Location $repo
try {
    & $tool @args
    if ($LASTEXITCODE -ne 0) { throw 'Death checkpoint snapshot feature test compile failed' }
    & $exe
    if ($LASTEXITCODE -ne 0) { throw 'Death checkpoint snapshot feature test failed' }
} finally {
    Pop-Location
}
