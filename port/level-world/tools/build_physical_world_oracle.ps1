param([string]$Output='.local-inputs/physical-world-oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 $physicsSources=Get-ChildItem -LiteralPath port/physics-backend/box2d-2.0.1/Source -Recurse -Filter '*.cpp' | ForEach-Object {$_.FullName}
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O0 -fno-fast-math -ffp-contract=off -std=c++17 -include cstring '-Iport/physics-backend/box2d-2.0.1/Include' port/level-world/physical_world.cpp @physicsSources -o $Output
 if($LASTEXITCODE -ne 0){throw 'ARM64 physical world oracle build failed'}
}finally{Pop-Location}
