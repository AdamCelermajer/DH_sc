param([string]$Output='.local-inputs/gameobject-lua-representation/libgameobject_lua.so')
$ErrorActionPreference='Stop';$repo=Resolve-Path "$PSScriptRoot/../../..";Push-Location $repo
try {
 New-Item -ItemType Directory -Force (Split-Path $Output)|Out-Null
 $bin="$env:LOCALAPPDATA/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin"
 $object=Join-Path (Split-Path $Output) 'script_object_bridge.o'
 $cargs=@('--target=aarch64-linux-android24','-c','-fPIC','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-Iport/script-runtime/lua','port/script-runtime/script_object_bridge.c','-o',$object)
 & "$bin/clang.exe" @cargs;if($LASTEXITCODE){throw 'Object C build failed'}
 $cppargs=@('--target=aarch64-linux-android24','-shared','-fPIC','-O2','-ffp-contract=off','-std=c++17','-Wall','-Wextra','-Werror','port/level-world/gameobject_lua_representation.cpp','port/level-world/character_target_bindings.cpp',$object,'-o',$Output)
 & "$bin/clang++.exe" @cppargs;if($LASTEXITCODE){throw 'Object C++ build failed'}
 $sources=@('port/script-runtime/script_runtime.h','port/script-runtime/script_object_bridge.h','port/script-runtime/script_object_bridge_internal.h','port/script-runtime/script_object_bridge.c','port/level-world/gameobject_lua_representation.hpp','port/level-world/gameobject_lua_representation.cpp','port/level-world/gameobject_lua_catalog.inc','port/level-world/character_target_bindings.hpp','port/level-world/character_target_bindings.cpp')
 $bindings=@{};foreach($p in $sources){$bindings[$p]=(Get-FileHash $p -Algorithm SHA256).Hash.ToLower()}
 @{c_compiler="$bin/clang.exe";c_arguments=$cargs;cpp_compiler="$bin/clang++.exe";cpp_arguments=$cppargs;source_bindings=$bindings;library_sha256=(Get-FileHash $Output -Algorithm SHA256).Hash.ToLower()} | ConvertTo-Json -Depth 8 | Set-Content (Join-Path (Split-Path $Output) 'arm64-build.json')
} finally{Pop-Location}
