param([string]$OutputDirectory = ".local-inputs/windows-foundation-build")
$ErrorActionPreference = "Stop"
$root = (Resolve-Path (Join-Path $PSScriptRoot "..\..\..\..\..")).Path
$clang = Join-Path $root ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
$output = Join-Path $root $OutputDirectory
$exe = Join-Path $output "source_indexed_slot_service_v1_tests.exe"
if (-not (Test-Path -LiteralPath $clang)) { throw "Pinned LLVM-MinGW compiler missing: $clang" }
New-Item -ItemType Directory -Force -Path $output | Out-Null
$includes = @(
    "-Iport/windows-foundation", "-Iport/level-world", "-Iport/game-data",
    "-Iport/engine-skinning", "-Iport/engine-animation", "-Iport/engine-ui",
    "-Iport/scene-materials", "-Iport/physics-backend/box2d-2.0.1/Include"
)
Push-Location $root
try {
    & $clang -std=c++17 -Wall -Wextra -Werror -Wno-unused-value -Dfinite=_finite -fsyntax-only @includes `
        "port/windows-foundation/features/frontend/creation/source_indexed_slot_service_v1.cpp"
    if ($LASTEXITCODE -ne 0) { throw "Indexed slot service syntax check failed: $LASTEXITCODE" }
    & $clang -std=c++17 -O0 -static -Wall -Wextra -Werror -Wno-unused-value -Wno-misleading-indentation -Dfinite=_finite -ffunction-sections -fdata-sections -fuse-ld=lld '-Wl,--gc-sections' @includes `
        "port/windows-foundation/features/frontend/creation/source_indexed_slot_service_v1_tests.cpp" `
        "port/windows-foundation/features/frontend/frontend_runtime_contract_v1.cpp" `
        "port/level-world/application_player_manager_bootstrap_v59.cpp" `
        "port/level-world/application_services_owner_v5.cpp" `
        "port/level-world/player_network_local_owner_v4.cpp" `
        "port/level-world/player_manager_combat_runtime_v2.cpp" `
        "port/level-world/player_manager_owner_v1.cpp" `
        "port/level-world/player_manager_loot_queries_v8.cpp" `
        "port/level-world/player_info_skill_buffers_v26.cpp" `
        "port/level-world/event_manager_owner_v12.cpp" `
        "port/level-world/visual_fx_manager_libraries_v63.cpp" `
        "port/level-world/visual_fx_preload.cpp" `
        "port/game-data/effects_tables.cpp" `
        "port/game-data/data.cpp" `
        -o $exe
    if ($LASTEXITCODE -ne 0) { throw "Indexed slot contract test compile failed: $LASTEXITCODE" }
    & $exe
    if ($LASTEXITCODE -ne 0) { throw "Indexed slot contract test failed: $LASTEXITCODE" }
} finally { Pop-Location }
