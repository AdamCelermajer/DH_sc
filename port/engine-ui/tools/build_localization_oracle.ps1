param([string]$Output=".local-inputs/localization-discovery/localization64.so")
$ErrorActionPreference='Stop'
$Root=(Resolve-Path "$PSScriptRoot/../../..").Path
$Compiler="$env:LOCALAPPDATA/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe"
$Target=Join-Path $Root $Output
New-Item -ItemType Directory -Force (Split-Path $Target) | Out-Null
& $Compiler --target=aarch64-linux-android26 -std=c++17 -O2 -shared -fPIC -static-libstdc++ -fno-fast-math -ffp-contract=off -DDH2_LOCALIZATION_ORACLE "$Root/port/engine-ui/localization.cpp" "$Root/port/engine-ui/tests/localization.cpp" -o $Target
if($LASTEXITCODE -ne 0){throw 'ARM64 source build failed'}
Write-Output (Get-FileHash -Algorithm SHA256 -LiteralPath $Target).Hash.ToLowerInvariant()
