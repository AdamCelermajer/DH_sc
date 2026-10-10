#requires -Version 5.1
<#
.SYNOPSIS
Creates a portable Windows foundation package without copying the asset cache.
.DESCRIPTION
AssetList is optional. Without it, only the executable and adjacent runtime DLLs
are packaged. With it, JSON must be an array of asset-root-relative file paths,
or an object with a files array. Paths are preserved beneath assets/. Directories,
wildcards, parent traversal and reparse points are rejected. The destination must
be empty; this script never deletes an existing package.
.EXAMPLE
./package.ps1 -Executable ../build/Release/dh_windows_foundation.exe `
    -AssetRoot ../../../asset-cache -AssetList ./act1-files.json `
    -OutputDirectory ../../../dist/act1-foundation
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$Executable,
    [Parameter(Mandatory = $true)][string]$AssetRoot,
    [Parameter(Mandatory = $true)][string]$OutputDirectory,
    [string]$AssetList
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Get-AbsolutePath([string]$Path) {
    return $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($Path)
}

function Assert-NoReparsePoint([string]$Path) {
    $item = Get-Item -LiteralPath $Path -Force
    while ($null -ne $item) {
        if (($item.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) {
            throw "Reparse points are not supported in package inputs: $($item.FullName)"
        }
        if ($item -is [IO.FileInfo]) { $item = $item.Directory }
        else { $item = $item.Parent }
    }
}

$exePath = Get-AbsolutePath $Executable
$assetRootPath = (Get-AbsolutePath $AssetRoot).TrimEnd('\', '/')
$outputPath = (Get-AbsolutePath $OutputDirectory).TrimEnd('\', '/')
if (-not (Test-Path -LiteralPath $exePath -PathType Leaf)) {
    throw "Executable was not found: $exePath"
}
if ([IO.Path]::GetExtension($exePath) -ine '.exe') { throw 'Executable must be a .exe file.' }
if (-not (Test-Path -LiteralPath $assetRootPath -PathType Container)) {
    throw "Asset root was not found: $assetRootPath"
}
Assert-NoReparsePoint $exePath
Assert-NoReparsePoint $assetRootPath
if (Test-Path -LiteralPath $outputPath) {
    if (-not (Test-Path -LiteralPath $outputPath -PathType Container)) {
        throw "Output path is not a directory: $outputPath"
    }
    Assert-NoReparsePoint $outputPath
    if (@(Get-ChildItem -LiteralPath $outputPath -Force).Count -gt 0) {
        throw "Output directory must be empty. Choose a new path: $outputPath"
    }
} else {
    $existingAncestor = [IO.Directory]::GetParent($outputPath)
    while ($null -ne $existingAncestor -and -not $existingAncestor.Exists) {
        $existingAncestor = $existingAncestor.Parent
    }
    if ($null -ne $existingAncestor) { Assert-NoReparsePoint $existingAncestor.FullName }
}

# Resolve and validate every selected file before writing anything.
$selectedFiles = @()
$selectionPath = $null
if (-not [string]::IsNullOrWhiteSpace($AssetList)) {
    $selectionPath = Get-AbsolutePath $AssetList
    if (-not (Test-Path -LiteralPath $selectionPath -PathType Leaf)) {
        throw "Asset selection JSON was not found: $selectionPath"
    }
    Assert-NoReparsePoint $selectionPath
    $jsonText = Get-Content -LiteralPath $selectionPath -Raw
    $selection = ConvertFrom-Json -InputObject $jsonText
    if ($jsonText.TrimStart().StartsWith('[')) { $paths = @($selection) }
    elseif ($null -ne $selection -and $null -ne $selection.PSObject.Properties['files']) {
        $paths = @($selection.files)
    } else { throw 'Asset selection JSON must be an array or an object with a files array.' }
    $seen = @{}
    foreach ($relativePath in $paths) {
        if ($relativePath -isnot [string] -or [string]::IsNullOrWhiteSpace($relativePath)) {
            throw 'Every selected asset must be a nonempty relative file path.'
        }
        if ([IO.Path]::IsPathRooted($relativePath) -or $relativePath.Contains(':') -or
            $relativePath.IndexOfAny([char[]]'*?') -ge 0 -or
            @($relativePath -split '[/\\]' | Where-Object { $_ -eq '..' }).Count -gt 0) {
            throw "Unsafe selected asset path: $relativePath"
        }
        $sourcePath = [IO.Path]::GetFullPath((Join-Path $assetRootPath $relativePath))
        if (-not $sourcePath.StartsWith($assetRootPath + [IO.Path]::DirectorySeparatorChar,
                [StringComparison]::OrdinalIgnoreCase)) {
            throw "Selected asset is outside asset root: $relativePath"
        }
        if (-not (Test-Path -LiteralPath $sourcePath -PathType Leaf)) {
            throw "Selected asset is missing or is not a file: $relativePath"
        }
        Assert-NoReparsePoint $sourcePath
        $normalized = $sourcePath.Substring($assetRootPath.Length + 1)
        if (-not $seen.ContainsKey($normalized)) {
            $seen[$normalized] = $true
            $selectedFiles += [pscustomobject]@{ Source = $sourcePath; Relative = $normalized }
        }
    }
}
$runtimeFiles = @((Get-Item -LiteralPath $exePath)) +
    @(Get-ChildItem -LiteralPath ([IO.Path]::GetDirectoryName($exePath)) -Filter '*.dll' -File)
foreach ($runtimeFile in $runtimeFiles) { Assert-NoReparsePoint $runtimeFile.FullName }

New-Item -ItemType Directory -Path $outputPath -Force | Out-Null
$manifestFiles = @()
foreach ($runtimeFile in $runtimeFiles) {
    $destination = Join-Path $outputPath $runtimeFile.Name
    Copy-Item -LiteralPath $runtimeFile.FullName -Destination $destination
    $manifestFiles += [ordered]@{
        path = $runtimeFile.Name
        bytes = $runtimeFile.Length
        sha256 = (Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant()
    }
}
foreach ($asset in $selectedFiles) {
    $packageRelative = Join-Path 'assets' $asset.Relative
    $destination = Join-Path $outputPath $packageRelative
    New-Item -ItemType Directory -Path ([IO.Path]::GetDirectoryName($destination)) -Force | Out-Null
    Copy-Item -LiteralPath $asset.Source -Destination $destination
    $manifestFiles += [ordered]@{
        path = $packageRelative.Replace('\', '/')
        bytes = (Get-Item -LiteralPath $destination).Length
        sha256 = (Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant()
    }
}
if ($null -ne $selectionPath) {
    Copy-Item -LiteralPath $selectionPath -Destination (Join-Path $outputPath 'asset-selection.json')
}
$manifest = [ordered]@{
    schemaVersion = 1
    executable = [IO.Path]::GetFileName($exePath)
    assetDirectory = 'assets'
    assetCount = $selectedFiles.Count
    files = @($manifestFiles)
}
$manifestJson = ConvertTo-Json -InputObject $manifest -Depth 6
[IO.File]::WriteAllText((Join-Path $outputPath 'package-manifest.json'), $manifestJson,
    (New-Object Text.UTF8Encoding($false)))
Write-Host "Portable package: $outputPath"
Write-Host "Runtime files: $($runtimeFiles.Count); selected assets: $($selectedFiles.Count)"
if ($selectedFiles.Count -eq 0) {
    Write-Warning 'No assets were selected. This is an executable-only package; add an explicit AssetList for content.'
}
