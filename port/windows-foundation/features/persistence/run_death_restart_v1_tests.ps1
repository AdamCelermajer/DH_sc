param([string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path $RepoRoot).Path
$tool = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$out = Join-Path $repo '.local-inputs/death-restart-v1'
New-Item -ItemType Directory -Force -Path $out | Out-Null
if (-not (Test-Path -LiteralPath $tool)) { throw "Pinned LLVM-MinGW compiler not found: $tool" }
$exe = Join-Path $out 'death_restart_v1_tests.exe'
$args = @('-std=c++17','-O0','-static','-Wall','-Wextra','-Werror','-Wpedantic',
    'port/windows-foundation/features/persistence/death_restart_v1.cpp',
    'port/windows-foundation/features/persistence/death_restart_v1_tests.cpp',
    '-o',$exe)
Push-Location $repo
try {
    & $tool @args
    if ($LASTEXITCODE -ne 0) { throw 'Death restart feature test compile failed' }
    & $exe
    if ($LASTEXITCODE -ne 0) { throw 'Death restart feature test failed' }
} finally {
    Pop-Location
}
