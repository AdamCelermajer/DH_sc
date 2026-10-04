$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$repo = Split-Path -Parent (Split-Path -Parent $root)
$out = Join-Path $repo '.local-inputs/fx-particle-animation-discovery'
New-Item -ItemType Directory -Force $out | Out-Null
$clang = Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
& $clang --target=aarch64-linux-android26 -std=c++17 -O2 -shared -fPIC -fno-fast-math -ffp-contract=off -DDH2_PARTICLE_PARAMETER_ORACLE (Join-Path $root 'particle_parameter.cpp') (Join-Path $root 'tests/particle_parameter.cpp') -o (Join-Path $out 'particle_parameter64.so')
if ($LASTEXITCODE -ne 0) { throw 'Particle parameter oracle build failed' }
