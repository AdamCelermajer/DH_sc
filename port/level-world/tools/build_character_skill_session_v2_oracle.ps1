param([string]$Output = '.local-inputs/character-skill-session-v2/libskill_session_v2_oracle.so')
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$compiler = 'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe'
if (!(Test-Path -LiteralPath $compiler)) { throw 'NDK compiler unavailable' }
$target = Join-Path $repo $Output
New-Item -ItemType Directory -Force -Path (Split-Path $target) | Out-Null
& $compiler --target=aarch64-linux-android26 -std=c++17 -O2 -fPIC -shared -static-libstdc++ -fno-fast-math -ffp-contract=off -fno-stack-protector '-Wl,--unresolved-symbols=ignore-all' (Join-Path $repo 'port/level-world/character_script_player_vcb_v2.cpp') (Join-Path $repo 'port/level-world/character_script_virtual.cpp') (Join-Path $repo 'port/level-world/character_current_skill_v2.cpp') -o $target
if ($LASTEXITCODE) { throw "Compiler failed: $LASTEXITCODE" }
Get-FileHash -Algorithm SHA256 -LiteralPath $target
