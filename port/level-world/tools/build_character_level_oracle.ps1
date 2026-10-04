$ErrorActionPreference='Stop'
$repo=Split-Path (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent) -Parent
$clang=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
$dest=Join-Path $repo '.local-inputs/character-level-discovery'
New-Item -ItemType Directory -Force -Path $dest | Out-Null
$sources=@('port/level-world/character_level.cpp','port/game-data/properties.cpp','port/game-data/class_tables.cpp') | ForEach-Object {Join-Path $repo $_}
$inputs=@($sources)+(@('port/level-world/character_level.hpp','port/game-data/properties.hpp','port/game-data/class_tables.hpp','port/game-data/data.hpp','port/script-runtime/script_design_bindings.h','port/script-runtime/script_runtime.h') | ForEach-Object {Join-Path $repo $_})
$before=@{};foreach($path in $inputs){$before[$path]=(Get-FileHash -LiteralPath $path).Hash.ToLowerInvariant()}
& $clang --target=aarch64-linux-android24 -shared -fPIC -O2 -ffp-contract=off -std=c++17 -Wall -Wextra -Werror @sources -o (Join-Path $dest 'libcharacter_level.so')
if($LASTEXITCODE -ne 0){throw 'level oracle compilation failed'}
foreach($path in $inputs){if((Get-FileHash -LiteralPath $path).Hash.ToLowerInvariant() -ne $before[$path]){throw 'compiler inputs changed during level build'}}
@{source_bindings=$before;library_sha256=(Get-FileHash (Join-Path $dest 'libcharacter_level.so')).Hash.ToLowerInvariant();compiler=$clang;target='aarch64-linux-android24';optimization='O2';fp_contract='off'} | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $dest 'arm64-build.json')
