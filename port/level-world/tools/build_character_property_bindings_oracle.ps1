$ErrorActionPreference='Stop'
$repo=Split-Path (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent) -Parent
$clang=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
$dest=Join-Path $repo '.local-inputs/character-property-bindings-discovery'
New-Item -ItemType Directory -Force -Path $dest | Out-Null
& $clang --target=aarch64-linux-android24 -shared -fPIC -O2 -ffp-contract=off -std=c++17 -Wall -Wextra -Werror (Join-Path $repo 'port/level-world/character_property_bindings.cpp') -o (Join-Path $dest 'libcharacter_property_bindings.so')
if($LASTEXITCODE -ne 0){throw 'property binding oracle compilation failed'}
