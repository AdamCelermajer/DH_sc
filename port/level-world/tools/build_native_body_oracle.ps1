param([string]$Output='.local-inputs/native-body-oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 $sources=Get-ChildItem -LiteralPath 'port/physics-backend/box2d-2.0.1/Source' -Recurse -File -Filter '*.cpp' | ForEach-Object FullName
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O0 -DNDEBUG -Wno-unused-value -fno-fast-math -ffp-contract=off -include cstring -std=c++17 '-Iport/physics-backend/box2d-2.0.1/Include' '-Iport/level-world' port/level-world/native_body.cpp port/level-world/tests/native_body_oracle.cpp @sources -o $Output
 if($LASTEXITCODE -ne 0){throw 'ARM64 genuine native body oracle build failed'}
}finally{Pop-Location}
