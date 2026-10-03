param([string]$Output=".local-inputs/character-ai-frame-discovery/oracle.so")
$ErrorActionPreference="Stop"
$repo=Resolve-Path (Join-Path $PSScriptRoot "../../..")
$compiler=Join-Path $env:LOCALAPPDATA "Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe"
Push-Location $repo
try {
 New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
 & $compiler --target=aarch64-linux-android24 -shared -fPIC -O2 -fno-fast-math -ffp-contract=off -std=c++17 -Wall -Wextra -Werror port/level-world/character_ai_frame.cpp port/level-world/character_ai_update.cpp port/level-world/character_script_update.cpp port/level-world/tests/character_ai_frame_bridge.cpp -o $Output
 if($LASTEXITCODE -ne 0){throw "AI frame oracle build failed"}
 Get-FileHash -Algorithm SHA256 $Output
} finally {Pop-Location}
