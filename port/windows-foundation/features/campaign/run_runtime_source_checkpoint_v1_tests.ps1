param([string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path $RepoRoot).Path
$build = Join-Path $repo '.local-inputs/windows-foundation-build'
$tool = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin'
$cxx = Join-Path $tool 'clang++.exe'
if (-not (Test-Path -LiteralPath $cxx)) { throw "Pinned LLVM-MinGW compiler not found: $cxx" }
foreach ($library in @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','libdh2_freetype237.a')) {
    if (-not (Test-Path -LiteralPath (Join-Path $build $library))) { throw "Required source library missing: $library" }
}
$exe = Join-Path $build 'runtime_source_checkpoint_v1_tests.exe'
$save = Join-Path $build 'runtime_source_checkpoint_v1_roundtrip.save'
$args = @(
    '-std=c++17','-O0','-static','-Wall','-Wextra','-Wpedantic',
    '-Iport/windows-foundation','-Iport/game-data',
    'port/windows-foundation/features/campaign/runtime_source_checkpoint_v1_tests.cpp',
    'port/windows-foundation/features/campaign/runtime_source_checkpoint_v1.cpp',
    "-L$build",'-lfoundation_data','-lrecovered_content','-lcontent_xml','-ldh2_freetype237',
    '-o',$exe
)
& $cxx @args
if ($LASTEXITCODE -ne 0) { throw 'Source checkpoint feature test compile/link failed' }
& $exe $repo (Join-Path $repo '.local-inputs/windows-shared-assets') $save
if ($LASTEXITCODE -ne 0) { throw 'Source checkpoint feature test failed' }
