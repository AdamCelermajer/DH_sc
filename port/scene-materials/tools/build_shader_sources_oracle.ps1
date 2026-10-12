$ErrorActionPreference='Stop'
$root=Split-Path -Parent $PSScriptRoot
$repo=Split-Path -Parent (Split-Path -Parent $root)
$out=Join-Path $repo '.local-inputs/fx-render-connection-discovery'
$clang=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
& $clang --target=aarch64-linux-android26 -std=c++17 -O2 -shared -fPIC -static-libstdc++ -DDH2_SHADER_SOURCES_ORACLE (Join-Path $root 'shader_sources.cpp') (Join-Path $root 'tests/shader_sources.cpp') -o (Join-Path $out 'shader_sources64.so')
if($LASTEXITCODE -ne 0){throw 'Shader source oracle build failed'}
