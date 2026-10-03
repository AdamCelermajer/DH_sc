param([string]$Output='.local-inputs/navigation-controller-oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O0 -fno-fast-math -ffp-contract=off -std=c++17 port/level-world/navigation_controller.cpp port/level-world/navigation_heading.cpp port/level-world/navigation_path.cpp port/level-world/navigation_avoidance.cpp port/level-world/navigation_objects.cpp port/level-world/navigation_motion.cpp port/level-world/navigation_world.cpp port/level-world/navigation_search.cpp port/level-world/collision.cpp port/level-world/octree.cpp port/level-world/selector.cpp -o $Output
 if($LASTEXITCODE -ne 0){throw 'ARM64 path controller oracle build failed'}
}finally{Pop-Location}
