[CmdletBinding()]
param(
    [string]$Compiler,
    [string]$CMake,
    [string]$Ninja,
    [ValidateRange(1, 32)][int]$Jobs = 3
)

$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..'))
$sourceRoot = Join-Path $repoRoot 'port\windows-foundation'
$buildRoot = Join-Path $repoRoot '.local-inputs\windows-foundation-build'
$portableRoot = Join-Path $repoRoot '.local-inputs\windows-toolchain'

function Resolve-Executable {
    param([string]$Requested, [string[]]$Candidates, [string]$CommandName)
    if ($Requested) {
        if (Test-Path -LiteralPath $Requested -PathType Leaf) {
            return (Resolve-Path -LiteralPath $Requested).Path
        }
        $command = Get-Command $Requested -CommandType Application -ErrorAction SilentlyContinue
        if ($command) { return $command.Source }
        throw "Requested executable does not exist: $Requested"
    }
    foreach ($candidate in $Candidates) {
        if ($candidate -and (Test-Path -LiteralPath $candidate -PathType Leaf)) {
            return (Resolve-Path -LiteralPath $candidate).Path
        }
    }
    $command = Get-Command $CommandName -CommandType Application -ErrorAction SilentlyContinue
    if ($command) { return $command.Source }
    throw "Cannot find $CommandName. Supply its path explicitly. This script never downloads tools."
}

function Invoke-Checked {
    param([string]$Executable, [string[]]$Arguments)
    & $Executable @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "Command failed with exit code ${LASTEXITCODE}: $Executable"
    }
}

$compilerCandidates = @(
    (Join-Path $portableRoot 'llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe')
)
if (Test-Path -LiteralPath $portableRoot -PathType Container) {
    $compilerCandidates += @(Get-ChildItem -LiteralPath $portableRoot -Directory |
        Where-Object { $_.Name -match '^llvm-mingw-.*-ucrt-x86_64$' } |
        Sort-Object Name -Descending |
        ForEach-Object { Join-Path $_.FullName 'bin\clang++.exe' })
}
$sdkTools = if ($env:LOCALAPPDATA) {
    Join-Path $env:LOCALAPPDATA 'Android\Sdk\cmake\3.22.1\bin'
} else { $null }
$cmakeCandidates = @()
$ninjaCandidates = @()
if ($sdkTools) {
    $cmakeCandidates += Join-Path $sdkTools 'cmake.exe'
    $ninjaCandidates += Join-Path $sdkTools 'ninja.exe'
}
$Compiler = Resolve-Executable $Compiler $compilerCandidates 'clang++.exe'
$CMake = Resolve-Executable $CMake $cmakeCandidates 'cmake.exe'
$Ninja = Resolve-Executable $Ninja $ninjaCandidates 'ninja.exe'
$ctest = Resolve-Executable '' @((Join-Path (Split-Path $CMake -Parent) 'ctest.exe')) 'ctest.exe'

Write-Host "Compiler: $Compiler"
Write-Host "Build directory: $buildRoot"
Invoke-Checked $CMake @(
    '-S', $sourceRoot, '-B', $buildRoot, '-G', 'Ninja',
    "-DCMAKE_MAKE_PROGRAM=$Ninja", "-DCMAKE_CXX_COMPILER=$Compiler",
    '-DCMAKE_BUILD_TYPE=Release'
)
Invoke-Checked $CMake @('--build', $buildRoot, '--parallel', "$Jobs")
Invoke-Checked $ctest @('--test-dir', $buildRoot, '--output-on-failure', '--timeout', '60')
Write-Host "Build and tests passed: $(Join-Path $buildRoot 'dh-foundation.exe')"
