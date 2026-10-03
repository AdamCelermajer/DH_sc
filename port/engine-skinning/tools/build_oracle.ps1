param([string]$Output='.local-inputs/character-oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O0 -fno-fast-math -ffp-contract=off -std=c++17 port/engine-skinning/skinning.cpp port/engine-animation/animation.cpp port/scene-materials/scene.cpp port/engine-math/math.cpp port/asset-payloads/payloads.cpp port/engine-resources/resources.cpp -o $Output
 if($LASTEXITCODE -ne 0){throw 'ARM64 character oracle build failed'}
}finally{Pop-Location}
