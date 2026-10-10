param([string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path $RepoRoot).Path
$build = Join-Path $repo '.local-inputs/windows-foundation-build'
$tool = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
if (-not (Test-Path -LiteralPath $tool)) { throw "Pinned LLVM-MinGW compiler not found: $tool" }
foreach ($archive in @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','libdh2_freetype237.a')) {
    if (-not (Test-Path -LiteralPath (Join-Path $build $archive))) { throw "Terminal foundation archive missing: $archive" }
}
$includes = @(
    '-Iport/windows-foundation','-Iport/level-world','-Iport/engine-animation',
    '-Iport/physics-backend/box2d-2.0.1/Include',
    '-Iport/engine-resources','-Iport/engine-skinning','-Iport/game-data',
    '-Iport/level-loader','-Iport/scene-materials','-Iport/engine-math',
    '-Iport/engine-ui','-Iport/engine-textures','-Iport/script-runtime',
    '-Iport/engine-audio','-Iport/level-loader/vendor/tinyxml'
)
$exe = Join-Path $repo '.local-inputs/runtime-companion-session-v1-tests.exe'
$sources = @(
    'port/windows-foundation/features/companions/runtime_companion_session_v1_tests.cpp',
    'port/windows-foundation/features/companions/runtime_companion_session_v1.cpp',
    'port/windows-foundation/features/companions/runtime_companion_movement_v1.cpp',
    'port/windows-foundation/features/companions/runtime_companion_follow_v1.cpp',
    'port/windows-foundation/features/navigation/source_commands.cpp'
)
$args = @('-std=c++17','-O0','-static','-Dfinite=_finite','-Wall','-Wextra','-Werror','-Wpedantic','-Wno-unused-value',
    '-Wno-missing-field-initializers') + $includes + $sources + @(
    (Join-Path $build 'libfoundation_data.a'),
    (Join-Path $build 'librecovered_content.a'),
    (Join-Path $build 'libcontent_xml.a'),
    (Join-Path $build 'libdh2_freetype237.a'),
    (Join-Path $build 'physics-backend/libdh2_box2d_201.a'), '-o', $exe
)
Push-Location $repo
try {
    & $tool @args
    if ($LASTEXITCODE -ne 0) { throw 'Companion Session native test compile/link failed' }
    & $exe (Join-Path $repo '.local-inputs/windows-shared-assets')
    if ($LASTEXITCODE -ne 0) { throw 'Companion Session native test failed' }
} finally {
    Pop-Location
}
