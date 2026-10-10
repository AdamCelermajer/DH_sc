$ErrorActionPreference = 'Stop'
$root = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..\..')).Path
$compiler = Join-Path $root '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
$python = Join-Path $root '.local-inputs\ida-ghidra-review\venv\Scripts\python.exe'
$outDir = Join-Path $root '.local-inputs\inventory-character-pane-tests'
$exe = Join-Path $outDir 'inventory_character_pane_tests.exe'
New-Item -ItemType Directory -Force -Path $outDir | Out-Null

& $python (Join-Path $PSScriptRoot 'inventory_character_pane_source_tests.py')
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $compiler -std=c++17 -O0 -Wall -Wextra -Werror -Wno-missing-field-initializers `
    -I (Join-Path $root 'port\windows-foundation') `
    -I (Join-Path $root 'port\windows-foundation\features\inventory') `
    -I (Join-Path $root 'port\windows-foundation\features\character_menu') `
    (Join-Path $PSScriptRoot 'inventory_character_pane_tests.cpp') `
    (Join-Path $PSScriptRoot 'original_inventory_art.cpp') -o $exe
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$oldPath = $env:PATH
$env:PATH = (Split-Path $compiler) + ';' + $oldPath
& $exe
$result = $LASTEXITCODE
$env:PATH = $oldPath
exit $result
