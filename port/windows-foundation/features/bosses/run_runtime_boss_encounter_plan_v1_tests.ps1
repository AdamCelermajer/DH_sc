$ErrorActionPreference = 'Stop'
$root = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$out = Join-Path $root '.local-inputs/boss-plan-v1/native-test'
New-Item -ItemType Directory -Force -Path $out | Out-Null
$clang = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$python = 'C:/Users/adamc/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe'
if (!(Test-Path -LiteralPath $clang)) { throw "Required compiler not found: $clang" }
if (!(Test-Path -LiteralPath $python)) { throw "Required Python not found: $python" }
$env:PATH = "$(Split-Path -Parent $clang);$env:PATH"
$source = Join-Path $PSScriptRoot 'runtime_boss_encounter_plan_v1.cpp'
$test = Join-Path $PSScriptRoot 'runtime_boss_encounter_plan_v1_tests.cpp'
$questProgress = Join-Path $root 'port/windows-foundation/features/quests/character_quest_progress_v1.cpp'
$questTables = Join-Path $root 'port/game-data/quest_persistence_v51.cpp'
$questSave = Join-Path $root 'port/game-data/quest_savegame_v1.cpp'
$exe = Join-Path $out 'runtime_boss_encounter_plan_v1_tests.exe'
& $clang -std=c++17 -Wall -Wextra -Werror -Wno-error=missing-field-initializers -pedantic $source $test $questProgress $questTables $questSave -o $exe
if ($LASTEXITCODE -ne 0) { throw "C++17 test compile failed ($LASTEXITCODE)" }
$cache = Join-Path $root '.local-inputs/runtime-source-projectile-v1/assets/pydata'
& $exe $cache
if ($LASTEXITCODE -ne 0) { throw "C++ source-plan tests failed ($LASTEXITCODE)" }
& $python (Join-Path $PSScriptRoot 'test_runtime_boss_encounter_plan_v1_sources.py') $root
if ($LASTEXITCODE -ne 0) { throw "Authored source-table tests failed ($LASTEXITCODE)" }
Write-Output "Native helper tests complete. No integrated game runtime was launched."
