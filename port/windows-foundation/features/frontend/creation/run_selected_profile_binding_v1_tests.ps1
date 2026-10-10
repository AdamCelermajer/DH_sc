param(
    [string]$OutputDirectory = ".local-inputs/windows-foundation-build"
)

$ErrorActionPreference = "Stop"
$root = (Resolve-Path (Join-Path $PSScriptRoot "..\..\..\..\..")).Path
$clang = Join-Path $root ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
$output = Join-Path $root $OutputDirectory
$exe = Join-Path $output "selected_profile_binding_v1_tests.exe"
if (-not (Test-Path -LiteralPath $clang)) { throw "Pinned LLVM-MinGW compiler missing: $clang" }
New-Item -ItemType Directory -Force -Path $output | Out-Null

$sources = @(
    "port/windows-foundation/features/frontend/creation/selected_profile_binding_v1_tests.cpp",
    "port/windows-foundation/features/frontend/creation/selected_profile_binding_v1.cpp",
    "port/game-data/data.cpp",
    "port/game-data/fresh_player_profile_v1.cpp",
    "port/game-data/player_profile_index_v1.cpp",
    "port/game-data/player_save_load_owner_v1.cpp",
    "port/game-data/player_save_named_writer_v1.cpp",
    "port/game-data/player_savegame_v1.cpp",
    "port/game-data/skill_tables.cpp",
    "port/level-world/application_save_files_owner_v61.cpp",
    "port/level-world/private_save_file_transport_v45.cpp",
    "port/level-world/savegame_jobs_owner_v2.cpp",
    "port/level-world/campaign_save_profile_v45.cpp",
    "port/level-world/campaign_save_filename_v45.cpp",
    "port/level-world/savegame_stream_v2.cpp",
    "port/level-world/level_savegame_writer_v2.cpp",
    "port/level-world/level_savegame_cache_v1.cpp",
    "port/level-world/level_savegame_objects_v2.cpp"
) | ForEach-Object { Join-Path $root $_ }

$arguments = @(
    "-std=c++17", "-O0", "-static", "-Wall", "-Wextra", "-Werror",
    "-ffunction-sections", "-fdata-sections", "-fuse-ld=lld", "-Wl,--gc-sections",
    "-Iport/windows-foundation", "-Iport/level-world", "-Iport/game-data",
    "-Iport/engine-skinning", "-Iport/engine-animation", "-Iport/engine-ui",
    "-Iport/scene-materials"
) + $sources + @("-o", $exe)

Push-Location $root
try {
    & $clang @arguments
    if ($LASTEXITCODE -ne 0) { throw "Selected profile binding compile failed: $LASTEXITCODE" }
    & $exe "port/android-native/app/src/main/assets"
    if ($LASTEXITCODE -ne 0) { throw "Selected profile binding source test failed: $LASTEXITCODE" }
} finally {
    Pop-Location
}
