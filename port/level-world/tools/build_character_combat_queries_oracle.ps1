param([string]$Output='.local-inputs/character-combat-queries-discovery/character_combat_queries_oracle.so')
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O2 -fno-fast-math -ffp-contract=off -std=c++17 -Wall -Wextra -Werror port/level-world/character_combat_queries.cpp -o $Output
 if($LASTEXITCODE -ne 0){throw 'Optimized ARM64 character combat query oracle build failed'}
}finally{Pop-Location}
