param(
    [string]$BuildDirectory = (Join-Path $env:TEMP 'dh2-desktop-cmake'),
    [string]$SdlRoot = $env:SDL2_ROOT,
    [string]$AngleRoot = $env:ANGLE_ROOT,
    [ValidateRange(1, 64)][int]$Jobs = 4
)

$ErrorActionPreference = 'Stop'
$sourceDirectory = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
foreach ($dependency in @(@{Name='SDL2_ROOT'; Path=$SdlRoot}, @{Name='ANGLE_ROOT'; Path=$AngleRoot})) {
    if (!$dependency.Path -or !(Test-Path -LiteralPath $dependency.Path -PathType Container)) {
        throw "Supply $($dependency.Name) or the corresponding script parameter with a development package directory."
    }
}
$cmake = (Get-Command cmake -ErrorAction Stop).Source
$gcc = (Get-Command gcc -ErrorAction Stop).Source
$cxx = (Get-Command 'c++' -ErrorAction Stop).Source
$ninja = (Get-Command ninja -ErrorAction Stop).Source

& $cmake -S $sourceDirectory -B $BuildDirectory -G Ninja `
    "-DCMAKE_C_COMPILER=$gcc" "-DCMAKE_CXX_COMPILER=$cxx" `
    "-DCMAKE_MAKE_PROGRAM=$ninja" "-DDH2_SDL2_ROOT=$SdlRoot" "-DDH2_ANGLE_ROOT=$AngleRoot"
if ($LASTEXITCODE -ne 0) { throw "Windows CMake configuration failed ($LASTEXITCODE)." }
& $cmake --build $BuildDirectory --target dh2_desktop_runtime --parallel $Jobs
if ($LASTEXITCODE -ne 0) { throw "Windows native build failed ($LASTEXITCODE)." }
Write-Output (Join-Path $BuildDirectory 'bin\dh2_desktop_runtime.exe')
