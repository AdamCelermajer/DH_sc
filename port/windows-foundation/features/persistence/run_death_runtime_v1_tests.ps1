param([string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path $RepoRoot).Path
$tool = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$out = Join-Path $repo '.local-inputs/death-runtime-v1'
New-Item -ItemType Directory -Force -Path $out | Out-Null
if (-not (Test-Path -LiteralPath $tool)) { throw "Pinned LLVM-MinGW compiler not found: $tool" }
$exe = Join-Path $out 'death_runtime_v1_tests.exe'
$args = @('-std=c++17','-O0','-static','-Wall','-Wextra','-Werror','-Wpedantic',
    'port/windows-foundation/features/persistence/death_restart_v1.cpp',
    'port/windows-foundation/features/persistence/death_flow_v1.cpp',
    'port/windows-foundation/features/persistence/death_screen_v1.cpp',
    'port/windows-foundation/features/persistence/death_screen_runtime_v1.cpp',
    'port/windows-foundation/features/persistence/death_fade_from_black_v1.cpp',
    'port/windows-foundation/features/persistence/death_checkpoint_load_v1.cpp',
    'port/windows-foundation/features/persistence/death_runtime_v1_tests.cpp',
    '-o',$exe)
Push-Location $repo
try {
    & $tool @args
    if ($LASTEXITCODE -ne 0) { throw 'Death runtime feature test compile failed' }
    & $exe
    if ($LASTEXITCODE -ne 0) { throw 'Death runtime feature test failed' }
} finally {
    Pop-Location
}
