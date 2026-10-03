param([string]$Output='.local-inputs/subobjects-update-oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O0 -fno-fast-math -ffp-contract=off -std=c++17 port/level-world/subobjects_update.cpp port/level-world/physical_controls.cpp -o $Output
 if($LASTEXITCODE -ne 0){throw 'ARM64 subobjects update oracle build failed'}
}finally{Pop-Location}
