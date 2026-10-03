param([string]$Output='.local-inputs/character-timer-discovery/character_timing_oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O2 -fno-fast-math -ffp-contract=off -std=c++17 -Wall -Wextra -Werror port/level-world/character_timers.cpp port/level-world/character_stance.cpp port/level-world/character_state.cpp -o $Output
 if($LASTEXITCODE -ne 0){throw 'Optimized ARM64 character timing oracle build failed'}
}finally{Pop-Location}
