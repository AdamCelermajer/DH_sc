param([string]$Output='.local-inputs/character-native-fsm/oracle.so')
$ErrorActionPreference='Stop'
$repo=(Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$clang=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
$target=Join-Path $repo $Output
New-Item -ItemType Directory -Force -Path (Split-Path $target) | Out-Null
& $clang '--target=aarch64-linux-android26' '-std=c++17' '-O2' '-shared' '-fPIC' '-fno-fast-math' '-ffp-contract=off' (Join-Path $repo 'port/level-world/character_native_fsm.cpp') '-o' $target
if($LASTEXITCODE -ne 0){throw 'ARM64 native FSM oracle compilation failed'}
Get-FileHash -Algorithm SHA256 -LiteralPath $target
