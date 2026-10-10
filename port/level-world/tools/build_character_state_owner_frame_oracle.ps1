param([string]$Output='.local-inputs/character-state-owner-frame/libframe_arm64_final.so')
$ErrorActionPreference='Stop'
$repo=(Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$clang=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
$target=Join-Path $repo $Output
New-Item -ItemType Directory -Force -Path (Split-Path $target) | Out-Null
$units=@('character_state.cpp','character_native_fsm.cpp','character_state_owner_frame.cpp','tests/character_state_owner_frame_oracle.cpp') | ForEach-Object { Join-Path $repo ('port/level-world/'+$_) }
& $clang '--target=aarch64-linux-android26' '-std=c++17' '-O2' '-shared' '-fPIC' '-fno-fast-math' '-ffp-contract=off' @units '-o' $target
if($LASTEXITCODE -ne 0){throw 'ARM64 owner frame oracle compilation failed'}
Get-FileHash -Algorithm SHA256 -LiteralPath $target
