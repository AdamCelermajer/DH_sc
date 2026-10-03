param([string]$Output='.local-inputs/animation-registration-discovery/oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O2 -std=c++17 -Wall -Wextra -Werror -DDH2_REGISTRATION_ORACLE -static-libstdc++ port/engine-animation/tests/animation_registration.cpp port/engine-animation/animation_registration.cpp '-Wl,-z,max-page-size=16384' -o $Output
 if($LASTEXITCODE -ne 0){throw 'Optimized ARM64 animation registration oracle build failed'}
}finally{Pop-Location}
