param([string]$Output='.local-inputs/component-applicator-discovery/oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O2 -fno-fast-math -ffp-contract=off -std=c++17 -Wall -Wextra -Werror -DDH2_COMPONENT_ORACLE -static-libstdc++ port/engine-animation/component_applicator.cpp port/engine-animation/animation_blend.cpp port/engine-math/math.cpp '-Wl,-z,max-page-size=16384' -o $Output
 if($LASTEXITCODE -ne 0){throw 'Optimized ARM64 component applicator oracle build failed'}
}finally{Pop-Location}
