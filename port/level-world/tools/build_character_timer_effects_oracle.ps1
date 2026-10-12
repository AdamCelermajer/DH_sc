param([string]$Output='.local-inputs/character-timer-effects-discovery/oracle.so')
$ErrorActionPreference='Stop'
$compiler=Join-Path $env:LOCALAPPDATA 'Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe'
New-Item -ItemType Directory -Force -Path (Split-Path $Output) | Out-Null
& $compiler --target=aarch64-linux-android24 -std=c++17 -O2 -fno-fast-math -ffp-contract=off -fPIC -shared port/level-world/character_timer_effects.cpp port/game-data/properties.cpp -o $Output
if ($LASTEXITCODE -ne 0) {throw 'Timer effects oracle compilation failed'}
Get-FileHash $Output
