param([string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
if (!$BuildRoot) { $BuildRoot = Join-Path $repo '.local-inputs/native-world-character-create-failure-v1' }
$build = [IO.Path]::GetFullPath($BuildRoot)
$compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$sharedArchive = Join-Path $repo '.local-inputs/windows-foundation-build/physics-backend/libdh2_box2d_201.a'
$privateArchive = Join-Path $build 'physics-backend/libdh2_box2d_201.a'
$null = New-Item -ItemType Directory -Path $build -Force
if (!(Test-Path -LiteralPath $sharedArchive)) { throw "Missing immutable Box2D input archive: $sharedArchive" }
$archiveHash = (Get-FileHash -Algorithm SHA256 -LiteralPath $sharedArchive).Hash
$null = New-Item -ItemType Directory -Path (Split-Path -Parent $privateArchive) -Force
Copy-Item -LiteralPath $sharedArchive -Destination $privateArchive -Force
if ($archiveHash -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $sharedArchive).Hash) {
    throw 'Shared Box2D archive changed during private snapshot.'
}
if ($archiveHash -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $privateArchive).Hash) {
    throw 'Private Box2D archive snapshot does not match its source.'
}
$includes = @('port/level-world', 'port/physics-backend/box2d-2.0.1/Include') |
    ForEach-Object { '-I' + (Join-Path $repo $_) }
$source = Join-Path $repo 'port/level-world/tests/native_world_character_creation_failure_v1.cpp'
$world = Join-Path $repo 'port/level-world/physical_world.cpp'
$output = Join-Path $build 'native_world_character_creation_failure_v1.exe'
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-Wno-unused-value' '-Dfinite=_finite' '-static' @includes `
    $source $world $privateArchive '-lkernel32' '-luser32' '-lgdi32' '-o' $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output
exit $LASTEXITCODE
