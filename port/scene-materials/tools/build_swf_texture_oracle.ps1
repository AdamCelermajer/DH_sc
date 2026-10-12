param([string]$Output=".local-inputs/swf-render-connection-discovery/swf_texture64.so")
$ErrorActionPreference='Stop'
$Root=(Resolve-Path "$PSScriptRoot/../../..").Path
$Compiler="$env:LOCALAPPDATA/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe"
$Target=Join-Path $Root $Output
New-Item -ItemType Directory -Force (Split-Path $Target) | Out-Null
& $Compiler --target=aarch64-linux-android26 -std=c++17 -O2 -shared -fPIC -static-libstdc++ -fno-fast-math -ffp-contract=off -DDH2_SWF_TEXTURE_ORACLE "$Root/port/scene-materials/swf_texture.cpp" "$Root/port/scene-materials/tests/swf_texture.cpp" -o $Target
if($LASTEXITCODE -ne 0){throw 'ARM64 source build failed'}
Write-Output (Get-FileHash -Algorithm SHA256 -LiteralPath $Target).Hash.ToLowerInvariant()
