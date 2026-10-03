param([string]$Output='.local-inputs/component-transforms-discovery/oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O2 -fno-fast-math -ffp-contract=off -std=c++17 -Wall -Wextra -Werror -DDH2_TRANSFORM_ORACLE -static-libstdc++ port/engine-animation/tests/component_transforms.cpp port/engine-animation/animation.cpp port/engine-animation/angle_interpreter.cpp port/engine-animation/events.cpp port/engine-animation/event_track.cpp port/scene-materials/scene.cpp port/engine-resources/resources.cpp port/asset-payloads/payloads.cpp port/engine-math/math.cpp '-Wl,-z,max-page-size=16384' '-Wl,--no-undefined' -o $Output
 if($LASTEXITCODE -ne 0){throw 'Optimized ARM64 component transforms oracle build failed'}
}finally{Pop-Location}
