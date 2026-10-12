param([string]$Output='.local-inputs/design-settings/libdesign_settings.so')
$ErrorActionPreference='Stop';$repo=Resolve-Path "$PSScriptRoot/../../..";Push-Location $repo
try {New-Item -ItemType Directory -Force (Split-Path $Output)|Out-Null;& "$env:LOCALAPPDATA/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe" --target=aarch64-linux-android24 -shared -fPIC -O2 -std=c++17 -Wall -Wextra -Werror port/game-data/design_settings.cpp -o $Output;if($LASTEXITCODE){throw 'DesignSettings build failed'};Get-FileHash $Output -Algorithm SHA256}finally{Pop-Location}
