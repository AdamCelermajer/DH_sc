[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
}
$build = Join-Path $repo '.local-inputs/campaign-source-owner-component'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$includes = @(
    '-Iport/game-data', '-Iport/level-world', '-Iport/scene-materials',
    '-Iport/physics-backend', '-isystem', 'port/physics-backend/box2d-2.0.1/Include',
    '-Iport/engine-ui', '-Iport/engine', '-Iport/character', '-Iport/world',
    '-Iport/assets', '-Iport'
)
$common = @('-std=c++17', '-Wall', '-Wextra', '-Werror', '-Dfinite=_finite') + $includes
& $Compiler @common '-c' (Join-Path $PSScriptRoot 'save_writer_binding_v1.cpp') `
    '-o' (Join-Path $build 'save_writer_binding_v1.o')
if ($LASTEXITCODE -ne 0) { throw 'Whole-profile Save writer binding strict compile failed' }
& $Compiler @common '-c' (Join-Path $PSScriptRoot 'save_writer_binding_tests.cpp') `
    '-o' (Join-Path $build 'save_writer_binding_tests.o')
if ($LASTEXITCODE -ne 0) { throw 'Whole-profile Save writer binding API compile test failed' }
Write-Output 'PASS whole-profile Save writer binding strict C++17 compile and API contract compile'
