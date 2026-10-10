[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../../..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
}
if (-not (Test-Path -LiteralPath $Compiler)) { throw "Required LLVM-MinGW compiler not found: $Compiler" }
$build = Join-Path $repo '.local-inputs/windows-foundation-build/runtime-equipment-preview-v1'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$exe = Join-Path $build 'runtime_equipment_preview_v1_tests.exe'
$source = Join-Path $PSScriptRoot 'runtime_equipment_preview_v1.cpp'
$frameSource = Join-Path $PSScriptRoot 'runtime_equipment_preview_frame_v1.cpp'
$test = Join-Path $PSScriptRoot 'runtime_equipment_preview_v1_tests.cpp'
$frameObject = Join-Path $build 'runtime_equipment_preview_frame_v1.o'
& $Compiler '-std=c++17' '-Wall' '-Wextra' '-Werror' '-O1' '-c' `
    ('-I' + (Join-Path $repo 'port/windows-foundation')) ('-I' + (Join-Path $repo 'port')) `
    $frameSource '-o' $frameObject
if ($LASTEXITCODE -ne 0) { throw 'Strict LLVM-MinGW Equipment preview frame source compile failed' }
& $Compiler '-std=c++17' '-Wall' '-Wextra' '-Werror' '-O1' '-static' '-ffunction-sections' '-fdata-sections' `
    '-Wl,--gc-sections' ('-I' + (Join-Path $repo 'port/windows-foundation')) ('-I' + (Join-Path $repo 'port')) `
    $test $source '-o' $exe
if ($LASTEXITCODE -ne 0) { throw 'Strict LLVM-MinGW Equipment preview source test build failed' }
$output = & $exe
if ($LASTEXITCODE -ne 0) { throw 'Equipment preview source projection test failed' }
$output | ForEach-Object { Write-Output $_ }
$reportPath = Join-Path $PSScriptRoot 'runtime_equipment_preview_camera_v1_report.json'
$report = [ordered]@{
    status = 'PASS_SOURCE_METADATA_ONLY'
    test = 'runtime_equipment_preview_v1_tests.cpp'
    runner = 'run_runtime_equipment_preview_v1_tests.ps1'
    output = ($output -join "`n")
    source = @{
        authored_viewport_480x320 = @(155.05, 87.70, 321.10, 282.75)
        camera_eye = @(0, -800, 200)
        camera_target = @(0, 0, 200)
        camera_up = @(0, 0, 1)
        fov_raw_bits = '0x3f0efb1a'
        fov_degrees = 32.00078
        near = 10
        far = 1000
        aspect = 0.85132017
        root_selection = 'CharacterVisual::source_motion_root_name(false), then the identically named node in the same retained Scene::graph'
    }
    limitation = 'This separate leaf test verifies camera/layout metadata only; the linked same-session binding and modular refresh test is recorded in runtime_equipment_preview_v1_report.json.'
}
$report | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $reportPath -Encoding utf8
Write-Output "report=$reportPath"
