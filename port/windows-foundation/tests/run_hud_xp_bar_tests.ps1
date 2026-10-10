$ErrorActionPreference = 'Stop'
$workspace = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$compiler = Resolve-Path (Join-Path $workspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe')
$buildDir = Join-Path $workspace '.local-inputs/b038-build'
New-Item -ItemType Directory -Force -Path $buildDir | Out-Null
$output = Join-Path $buildDir 'hud_xp_bar_tests.exe'
$source = @(
    'port/windows-foundation/tests/hud_xp_bar_tests.cpp',
    'port/windows-foundation/hud_geometry.cpp'
)
$absoluteSource = $source | ForEach-Object { Join-Path $workspace $_ }
& $compiler -std=c++17 -O1 -Wall -Wextra -Werror -static -I (Join-Path $workspace 'port/windows-foundation') -I (Join-Path $workspace 'port/game-data') @absoluteSource -o $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output
exit $LASTEXITCODE
