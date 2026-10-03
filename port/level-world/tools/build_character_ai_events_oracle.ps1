param([string]$Output=".local-inputs/character-ai-events-discovery/oracle.so")
$ErrorActionPreference="Stop"
$taskRepo=Resolve-Path (Join-Path $PSScriptRoot "../../..")
$taskCompiler=Join-Path $env:LOCALAPPDATA "Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe"
Push-Location $taskRepo
try {
 New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
 & $taskCompiler --target=aarch64-linux-android24 -shared -fPIC -O2 -fno-fast-math -ffp-contract=off -std=c++17 -Wall -Wextra -Werror port/level-world/character_ai_events.cpp -o $Output
 if($LASTEXITCODE -ne 0){throw "AI events oracle build failed"}
 Get-FileHash -Algorithm SHA256 $Output
} finally {Pop-Location}
