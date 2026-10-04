$ErrorActionPreference='Stop'
$root=(Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$dest=Join-Path $root '.local-inputs/player-loot-v7'
New-Item -ItemType Directory -Force $dest|Out-Null
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
$inputs=@('loot_power_resources_v7.cpp','loot_power_creation_v7.cpp','loot_tables_v2.cpp','item_power_tables_v5.cpp','item_presentation_v5.cpp','items.cpp','tests/loot_power_creation_v7_fixture.cpp')|ForEach-Object{Join-Path $root ('port/game-data/'+$_)}
$before=@{};foreach($p in $inputs){$before[$p]=(Get-FileHash $p -Algorithm SHA256).Hash.ToLowerInvariant()}
$output=Join-Path $dest 'libloot_power_creation_v7_arm64.so'
& $compiler --target=aarch64-linux-android26 -shared -fPIC -O2 -fno-fast-math -ffp-contract=off -std=c++17 -Wall -Wextra -Werror -Wno-misleading-indentation -static-libstdc++ '-Wl,--no-undefined' '-Wl,-z,max-page-size=16384' @inputs -o $output
if($LASTEXITCODE){throw 'Loot power creation ARM64 build failed'}
foreach($p in $inputs){if($before[$p] -ne (Get-FileHash $p -Algorithm SHA256).Hash.ToLowerInvariant()){throw 'Source changed during build'}}
@{source_sha256=$before;library_sha256=(Get-FileHash $output -Algorithm SHA256).Hash.ToLowerInvariant();compiler=$compiler;target='aarch64-linux-android26';optimization='O2';floating='no-fast-math;ffp-contract=off';build_scope='isolated production modules; no central CMake or APK'}|ConvertTo-Json -Depth 5|Set-Content -LiteralPath (Join-Path $dest 'arm64-build.json')
Get-FileHash $output -Algorithm SHA256
