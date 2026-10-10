$ErrorActionPreference='Stop'
$repo=Split-Path (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent) -Parent
$clang=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
$dest=Join-Path $repo '.local-inputs/game-design-tables-discovery'
New-Item -ItemType Directory -Force -Path $dest | Out-Null
$inputs=@('port/game-data/game_design_tables.cpp','port/game-data/game_design_tables.hpp') | ForEach-Object {Join-Path $repo $_}
$before=@{};foreach($path in $inputs){$before[$path]=(Get-FileHash -LiteralPath $path).Hash.ToLowerInvariant()}
& $clang --target=aarch64-linux-android24 -shared -fPIC -O2 -ffp-contract=off -std=c++17 -Wall -Wextra -Werror $inputs[0] -o (Join-Path $dest 'libgame_design_tables.so')
if($LASTEXITCODE -ne 0){throw 'design table oracle compilation failed'}
foreach($path in $inputs){if((Get-FileHash -LiteralPath $path).Hash.ToLowerInvariant() -ne $before[$path]){throw 'compiler inputs changed'}}
@{source_bindings=$before;library_sha256=(Get-FileHash (Join-Path $dest 'libgame_design_tables.so')).Hash.ToLowerInvariant();compiler=$clang;target='aarch64-linux-android24';optimization='O2'} | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $dest 'arm64-build.json')
