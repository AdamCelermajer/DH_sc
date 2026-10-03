param([string]$Output='.local-inputs/floor-source-oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O0 -fno-fast-math -ffp-contract=off -std=c++17 port/level-world/floor_source.cpp port/level-world/selector.cpp port/level-world/octree.cpp port/level-world/collision.cpp port/level-world/navigation.cpp port/asset-payloads/payloads.cpp port/engine-resources/resources.cpp port/scene-materials/scene.cpp port/engine-math/math.cpp -lm -o $Output
 if($LASTEXITCODE -ne 0){throw 'ARM64 floor source oracle build failed'}
}finally{Pop-Location}
