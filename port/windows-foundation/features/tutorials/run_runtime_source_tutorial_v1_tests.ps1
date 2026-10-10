param([string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path $RepoRoot).Path
$build = Join-Path $repo '.local-inputs/windows-foundation-build'
$tool = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin'
$cxx = Join-Path $tool 'clang++.exe'
if (-not (Test-Path -LiteralPath $cxx)) { throw "Pinned LLVM-MinGW compiler not found: $cxx" }
if (-not (Test-Path -LiteralPath (Join-Path $build 'libfoundation_data.a'))) { throw "Foundation archive not found: $build" }
$includes = @(
    '-Iport/windows-foundation','-Iport/level-world','-Iport/engine-animation',
    '-Iport/engine-resources','-Iport/engine-skinning','-Iport/game-data',
    '-Iport/level-loader','-Iport/scene-materials','-Iport/engine-math',
    '-Iport/engine-ui','-Iport/engine-textures','-Iport/script-runtime',
    '-Iport/engine-audio','-Iport/level-loader/vendor/tinyxml'
)
$exe = Join-Path $build 'runtime_source_tutorial_v1_tests.exe'
$sources = @(
    'port/windows-foundation/features/tutorials/runtime_source_tutorial_v1_tests.cpp',
    'port/windows-foundation/features/tutorials/runtime_source_tutorial_v1.cpp'
)
$args = @('-std=c++17','-O0','-static','-Wall','-Wextra','-Wpedantic') + $includes + $sources + @(
    "-L$build",'-lfoundation_data','-lrecovered_content','-lcontent_xml','-ldh2_freetype237','-o',$exe
)
& $cxx @args
if ($LASTEXITCODE -ne 0) { throw 'Source tutorial feature test compile/link failed' }
& $exe $repo
if ($LASTEXITCODE -ne 0) { throw 'Source tutorial feature test failed' }
