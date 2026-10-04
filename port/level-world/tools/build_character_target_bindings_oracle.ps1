param([string]$Output='.local-inputs/character-target-bindings-discovery/libcharacter_target_bindings.so')
$ErrorActionPreference='Stop';$repo=Resolve-Path "$PSScriptRoot/../../..";Push-Location $repo
try {
 New-Item -ItemType Directory -Force (Split-Path $Output)|Out-Null
 $compiler="$env:LOCALAPPDATA/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe"
 $arguments=@('--target=aarch64-linux-android24','-shared','-fPIC','-O2','-ffp-contract=off','-std=c++17','-Wall','-Wextra','-Werror','port/level-world/character_target_bindings.cpp','-o',$Output)
 & $compiler @arguments;if($LASTEXITCODE){throw 'Target bindings build failed'}
 $sources=@('port/level-world/character_target_bindings.hpp','port/level-world/character_target_bindings.cpp')
 $bindings=@{};foreach($p in $sources){$bindings[$p]=(Get-FileHash $p -Algorithm SHA256).Hash.ToLower()}
 $report=@{compiler=$compiler;compiler_arguments=$arguments;source_bindings=$bindings;library_sha256=(Get-FileHash $Output -Algorithm SHA256).Hash.ToLower()}
 $report|ConvertTo-Json -Depth 8|Set-Content (Join-Path (Split-Path $Output) 'arm64-build.json')
}finally{Pop-Location}
