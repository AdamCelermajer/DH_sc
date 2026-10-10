param([string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path $RepoRoot).Path
$build = Join-Path $repo '.local-inputs/windows-foundation-build'
$tool = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin'
$cxx = Join-Path $tool 'clang++.exe'
$cmake = Join-Path $env:LOCALAPPDATA 'Android/Sdk/cmake/3.22.1/bin/cmake.exe'
if (-not (Test-Path -LiteralPath $cxx)) { throw "Pinned LLVM-MinGW compiler not found: $cxx" }
if (-not (Test-Path -LiteralPath (Join-Path $build 'CMakeCache.txt'))) { throw "Configured Windows Foundation build is required: $build" }
if (-not (Test-Path -LiteralPath $cmake)) { throw "Configured CMake executable not found: $cmake" }
& $cmake --build $build --target foundation_data
if ($LASTEXITCODE -ne 0) { throw 'Foundation source library build failed' }
$includes = @(
    '-Iport/windows-foundation','-Iport/level-world','-Iport/engine-animation',
    '-Iport/engine-resources','-Iport/engine-skinning','-Iport/game-data',
    '-Iport/level-loader','-Iport/scene-materials','-Iport/engine-math',
    '-Iport/engine-ui','-Iport/engine-textures','-Iport/script-runtime',
    '-Iport/engine-audio','-Iport/level-loader/vendor/tinyxml'
)
$exe = Join-Path $build 'runtime_troll_return_v1_tests.exe'
$sources = @(
    'port/windows-foundation/features/cinematics/runtime_troll_return_v1_tests.cpp',
    'port/windows-foundation/features/cinematics/runtime_troll_return_v1.cpp'
)
$args = @('-std=c++17','-O0','-static','-Wall','-Wextra','-Wpedantic',
    '-Wno-missing-field-initializers') + $includes + $sources + @(
    "-L$build",'-lfoundation_data','-lrecovered_content','-lcontent_xml','-ldh2_freetype237','-o',$exe
)
& $cxx @args
if ($LASTEXITCODE -ne 0) { throw 'TrollReturn native feature test compile/link failed' }
& $exe $repo
if ($LASTEXITCODE -ne 0) { throw 'TrollReturn native feature test failed' }
