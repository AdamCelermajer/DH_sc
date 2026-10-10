param([string]$Workspace = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$compiler = Join-Path $Workspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$binary = Join-Path $Workspace '.local-inputs/status-effects-live-tests.exe'
$constructorBinary = Join-Path $Workspace '.local-inputs/status-source-owners-tests.exe'
$sources = @(
 'port/windows-foundation/features/status_effects/status_effects_tests.cpp',
 'port/windows-foundation/features/status_effects/status_effects.cpp',
 'port/windows-foundation/features/status_effects/status_live_binding.cpp',
 'port/level-world/character_buffs.cpp', 'port/level-world/character_timers.cpp',
 'port/level-world/character_native_effects.cpp', 'port/level-world/character_slow_reaction_v1.cpp',
 'port/level-world/character_timer_effects.cpp', 'port/level-world/character_dot_attack.cpp',
 'port/level-world/character_skill_buff_bindings_v3.cpp', 'port/game-data/properties.cpp',
 'port/game-data/class_tables.cpp', 'port/game-data/combat_result.cpp', 'port/game-data/combat.cpp',
 'port/game-data/ai.cpp', 'port/game-data/effects_tables.cpp', 'port/game-data/data.cpp'
) | ForEach-Object {Join-Path $Workspace $_}
& $compiler '-std=c++17' '-O1' '-g' '-ffp-contract=off' '-Wall' '-Wextra' @sources '-o' $binary
if ($LASTEXITCODE -ne 0) {throw "Status live binding compilation failed: $LASTEXITCODE"}
$constructorSources = @(
 'port/windows-foundation/features/status_effects/source_status_owners_tests.cpp',
 'port/windows-foundation/features/status_effects/source_status_owners.cpp',
 'port/level-world/character_buffs.cpp', 'port/level-world/character_timers.cpp',
 'port/level-world/character_timer_effects.cpp', 'port/level-world/character_script_lifecycle.cpp',
 'port/level-world/character_design_services.cpp', 'port/game-data/properties.cpp',
 'port/game-data/class_tables.cpp'
) | ForEach-Object {Join-Path $Workspace $_}
& $compiler '-std=c++17' '-O1' '-g' '-ffp-contract=off' '-Wall' '-Wextra' @constructorSources '-o' $constructorBinary
if ($LASTEXITCODE -ne 0) {throw "Source status construction compilation failed: $LASTEXITCODE"}
$oldPath = $env:PATH
try {
 $env:PATH = (Split-Path -Parent $compiler) + ';' + $oldPath
 & $binary (Join-Path $Workspace '.local-inputs/windows-shared-assets/original-cache/data/pydata')
 if ($LASTEXITCODE -ne 0) {throw "Status live binding test failed: $LASTEXITCODE"}
 & $constructorBinary
 if ($LASTEXITCODE -ne 0) {throw "Source status construction test failed: $LASTEXITCODE"}
} finally {$env:PATH = $oldPath}
