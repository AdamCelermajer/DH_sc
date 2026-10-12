param([string]$Output='.local-inputs/character-buffs-discovery/oracle.so')
$ErrorActionPreference='Stop'
$compiler=Join-Path $env:LOCALAPPDATA 'Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe'
New-Item -ItemType Directory -Force -Path (Split-Path $Output) | Out-Null
$arguments=@('--target=aarch64-linux-android24','-std=c++17','-O2','-Wall','-Wextra','-Werror','-fno-fast-math','-ffp-contract=off','-fPIC','-shared','port/level-world/character_buffs.cpp','port/level-world/character_timers.cpp','port/game-data/properties.cpp','port/game-data/class_tables.cpp','port/level-world/tests/character_buffs_oracle.cpp','-o',$Output)
& $compiler @arguments
if($LASTEXITCODE -ne 0){throw 'Buff owner oracle compilation failed'}
$sources=@{}
foreach($source in @('port/level-world/character_buffs.hpp','port/level-world/character_buffs.cpp','port/level-world/character_timers.hpp','port/level-world/character_timers.cpp','port/game-data/properties.hpp','port/game-data/properties.cpp','port/game-data/class_tables.hpp','port/game-data/class_tables.cpp','port/level-world/tests/character_buffs_oracle.cpp')){$sources[$source]=(Get-FileHash $source).Hash.ToLowerInvariant()}
@{compiler=$compiler;compiler_sha256=(Get-FileHash $compiler).Hash.ToLowerInvariant();arguments=$arguments;source_sha256=$sources;library_sha256=(Get-FileHash $Output).Hash.ToLowerInvariant()}|ConvertTo-Json -Depth 5|Set-Content ($Output+'.build.json')
Get-FileHash $Output
