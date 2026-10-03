param([string]$Output='.local-inputs/character-target-search-oracle/libcharacter_target_search.so')
$ErrorActionPreference='Stop'
$repo=Resolve-Path "$PSScriptRoot/../../.."
Push-Location $repo
try {
 New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
 $compiler="$env:LOCALAPPDATA/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe"
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O2 -ffp-contract=off -std=c++17 -Wall -Wextra -Werror port/level-world/character_target_search.cpp -o $Output
 if($LASTEXITCODE){throw 'Target search oracle build failed'}
 Get-FileHash $Output -Algorithm SHA256
} finally {Pop-Location}
