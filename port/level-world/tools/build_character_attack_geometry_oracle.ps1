param([string]$Output=".local-inputs/prince-live-attack-services/character_attack_geometry_oracle.so")
$ErrorActionPreference="Stop"
$repo=Resolve-Path (Join-Path $PSScriptRoot "../../..")
$compiler=Join-Path $env:LOCALAPPDATA "Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe"
Push-Location $repo
try {
 New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O2 -ffp-contract=off -std=c++17 -Wall -Wextra -Werror port/level-world/character_attack_geometry.cpp -o $Output
 if($LASTEXITCODE -ne 0){throw "Attack geometry oracle build failed"}
 Get-FileHash -Algorithm SHA256 $Output
} finally {Pop-Location}
