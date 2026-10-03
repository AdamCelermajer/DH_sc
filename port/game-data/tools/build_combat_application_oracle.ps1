param([string]$Output='.local-inputs/combat-application-oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O0 -std=c++17 port/game-data/properties.cpp port/game-data/health.cpp port/game-data/vitals.cpp port/game-data/combat_application.cpp -o $Output
 if($LASTEXITCODE -ne 0){throw 'ARM64 combat application oracle build failed'}
}finally{Pop-Location}
