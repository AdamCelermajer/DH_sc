param([string]$Output=".local-inputs/fx-material-animation-discovery/material_color64.so")
$ErrorActionPreference='Stop'
$clang=Join-Path $env:LOCALAPPDATA 'Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe'
New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
& $clang --target=aarch64-linux-android24 '-std=c++17' -O2 '-fno-fast-math' '-ffp-contract=off' -shared -fPIC '-DDH2_MATERIAL_COLOR_ORACLE' port/engine-animation/material_color.cpp port/engine-animation/tests/material_color.cpp -o $Output
if($LASTEXITCODE){throw 'material color oracle build failed'}
