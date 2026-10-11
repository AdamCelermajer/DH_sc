param(
    [string]$SourceRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path,
    [string]$SnapshotRoot = (Join-Path $env:TEMP 'dh2-desktop-source-snapshot'),
    [string]$BuildDirectory = (Join-Path $env:TEMP 'dh2-desktop-cmake-snapshot')
)

$ErrorActionPreference = 'Stop'
$SourceRoot = (Resolve-Path -LiteralPath $SourceRoot).Path.TrimEnd('\')
$SnapshotRoot = [System.IO.Path]::GetFullPath($SnapshotRoot).TrimEnd('\')
$BuildDirectory = [System.IO.Path]::GetFullPath($BuildDirectory).TrimEnd('\')

if ($SnapshotRoot.StartsWith($SourceRoot + '\', [System.StringComparison]::OrdinalIgnoreCase) -or
    $SourceRoot.StartsWith($SnapshotRoot + '\', [System.StringComparison]::OrdinalIgnoreCase)) {
    throw 'SourceRoot and SnapshotRoot must be separate trees.'
}

$excludedDirectoryNames = @('.git', '.checks', '.cxx', '.gradle', 'build', 'outputs', 'reports')
$sourceRoots = @(
    'port/desktop-native',
    'port/android-native/app/src/main/cpp',
    'port/android-native/app/src/main/assets',
    'port/adam-script-runtime',
    'port/asset-payloads',
    'port/engine-animation',
    'port/engine-audio',
    'port/engine-camera',
    'port/engine-math',
    'port/engine-resources',
    'port/engine-skinning',
    'port/engine-textures',
    'port/engine-ui',
    'port/floor-types',
    'port/game-data',
    'port/irrlicht-android/upstream/source/Irrlicht/zlib',
    'port/level-world',
    'port/lua-numeric',
    'port/material-bindings',
    'port/physics-backend',
    'port/persistence',
    'port/player-info-level',
    'port/pydata-constants',
    'port/pydata-names',
    'port/pydata-scripts',
    'port/quest-data',
    'port/quest-kill',
    'port/random',
    'port/random-level',
    'port/scene-materials',
    'port/scene-payloads',
    'port/script-runtime',
    'port/trigger-contact',
    'port/world-data',
    'port/zone-contact-runtime'
)

function Get-SelectedFiles {
    param([string]$Root)

    $selected = [System.Collections.Generic.List[System.IO.FileInfo]]::new()
    foreach ($relativeRoot in $sourceRoots) {
        $directory = Join-Path $Root ($relativeRoot -replace '/', '\')
        if (!(Test-Path -LiteralPath $directory -PathType Container)) {
            throw "Required CMake source/resource root is missing: $relativeRoot"
        }
        $pending = [System.Collections.Generic.Stack[string]]::new()
        $pending.Push($directory)
        while ($pending.Count) {
            $current = $pending.Pop()
            foreach ($child in Get-ChildItem -LiteralPath $current -Force) {
                if ($child.PSIsContainer) {
                    if ($excludedDirectoryNames -contains $child.Name -or
                        ($child.Attributes -band [System.IO.FileAttributes]::ReparsePoint)) { continue }
                    $pending.Push($child.FullName)
                } else {
                    $selected.Add($child)
                }
            }
        }
    }
    return $selected | Sort-Object { $_.FullName.Substring($Root.Length + 1) }
}

function Get-SourceSnapshotManifest {
    param([string]$Root)

    $files = foreach ($file in Get-SelectedFiles -Root $Root) {
        $relative = ($file.FullName.Substring($Root.Length + 1) -replace '\\', '/')
        [pscustomobject]@{
            path = $relative
            length = $file.Length
            sha256 = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash.ToLowerInvariant()
            lastWriteTimeUtc = $file.LastWriteTimeUtc.ToString('o')
        }
    }
    $git = Get-Command git -ErrorAction Stop
    $head = (& $git.Source -C $Root rev-parse HEAD 2>$null | Select-Object -First 1)
    if ($LASTEXITCODE -ne 0) { $head = 'unknown' }
    $statusRaw = @(& $git.Source -C $Root status --short --untracked-files=all -- $sourceRoots 2>$null)
    if ($LASTEXITCODE -ne 0) { throw 'Could not collect Git dirty/untracked provenance.' }
    $status = @($statusRaw | Where-Object {
        $statusPath = $_.Substring([Math]::Min(3, $_.Length)).Trim('"')
        $statusSegments = $statusPath.Split([char[]]@('/', '\'), [System.StringSplitOptions]::RemoveEmptyEntries)
        -not ($statusSegments | Where-Object { $excludedDirectoryNames -contains $_ })
    } | Sort-Object)
    [pscustomobject]@{
        head = [string]$head
        gitStatus = @($status | Sort-Object)
        files = @($files)
    }
}

foreach ($process in Get-CimInstance Win32_Process -Filter "Name='ninja.exe' OR Name='cmake.exe' OR Name='dh2_desktop_runtime.exe'" -ErrorAction SilentlyContinue) {
    if ($process.CommandLine -and
        ($process.CommandLine.IndexOf($SnapshotRoot, [System.StringComparison]::OrdinalIgnoreCase) -ge 0 -or
         $process.CommandLine.IndexOf($BuildDirectory, [System.StringComparison]::OrdinalIgnoreCase) -ge 0)) {
        throw "The desktop build/runtime is active (PID $($process.ProcessId)); stop it before refreshing the source snapshot."
    }
}

$before = Get-SourceSnapshotManifest -Root $SourceRoot
$manifestPath = Join-Path $SnapshotRoot '.dh2-source-manifest.json'
New-Item -ItemType Directory -Path $SnapshotRoot -Force | Out-Null
$previousManifestPath = Join-Path $SnapshotRoot '.dh2-source-manifest.previous.json'
if (Test-Path -LiteralPath $manifestPath -PathType Leaf) {
    Copy-Item -LiteralPath $manifestPath -Destination $previousManifestPath -Force
    Remove-Item -LiteralPath $manifestPath -Force
}

$expectedByPath = @{}
foreach ($entry in $before.files) { $expectedByPath[$entry.path] = $entry }

foreach ($entry in $before.files) {
    $source = Join-Path $SourceRoot ($entry.path -replace '/', '\')
    $destination = Join-Path $SnapshotRoot ($entry.path -replace '/', '\')
    $same = $false
    if (Test-Path -LiteralPath $destination -PathType Leaf) {
        $existing = Get-Item -LiteralPath $destination
        if ($existing.Length -eq $entry.length) {
            $same = (Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.Equals(
                $entry.sha256, [System.StringComparison]::OrdinalIgnoreCase)
        }
    }
    if ($same) { continue }

    $parent = Split-Path -Parent $destination
    New-Item -ItemType Directory -Path $parent -Force | Out-Null
    Copy-Item -LiteralPath $source -Destination $destination -Force
    $copied = Get-Item -LiteralPath $destination
    $copiedHash = (Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash
    if ($copied.Length -ne $entry.length -or
        !$copiedHash.Equals($entry.sha256, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Snapshot copy verification failed: $($entry.path); expected $($entry.sha256)/$($entry.length), got $copiedHash/$($copied.Length)."
    }
    # A changed file must be newer than existing Ninja objects even if a
    # checkout restored it with an old timestamp. Equal-content files above
    # are skipped, keeping their mirror timestamps stable across snapshots.
    $copied.LastWriteTimeUtc = [DateTime]::UtcNow
}

$after = Get-SourceSnapshotManifest -Root $SourceRoot
$beforeJson = $before.files | ConvertTo-Json -Depth 6 -Compress
$afterJson = $after.files | ConvertTo-Json -Depth 6 -Compress
if ($beforeJson -cne $afterJson) {
    $beforeByPath = @{}
    $afterByPath = @{}
    foreach ($entry in $before.files) { $beforeByPath[$entry.path] = $entry }
    foreach ($entry in $after.files) { $afterByPath[$entry.path] = $entry }
    $changedPaths = @(
        @($beforeByPath.Keys) + @($afterByPath.Keys) |
            Sort-Object -Unique |
            Where-Object {
                !$beforeByPath.ContainsKey($_) -or !$afterByPath.ContainsKey($_) -or
                $beforeByPath[$_].sha256 -cne $afterByPath[$_].sha256 -or
                $beforeByPath[$_].length -ne $afterByPath[$_].length -or
                $beforeByPath[$_].lastWriteTimeUtc -cne $afterByPath[$_].lastWriteTimeUtc
            }
    )
    $detail = if ($changedPaths.Count) { $changedPaths -join ', ' } else { 'selected file metadata changed' }
    throw "Selected source/resources changed during snapshot copy: $detail; no valid manifest was written."
}
$before.head = $after.head
$before.gitStatus = $after.gitStatus

# Remove only stale files listed by the last successful managed manifest.
# The exact snapshot root was validated above, and no recursive deletion is used.
if (Test-Path -LiteralPath $previousManifestPath -PathType Leaf) {
    $previous = Get-Content -LiteralPath $previousManifestPath -Raw | ConvertFrom-Json
    foreach ($entry in $previous.files) {
        if ($expectedByPath.ContainsKey($entry.path)) { continue }
        $stale = [System.IO.Path]::GetFullPath((Join-Path $SnapshotRoot ($entry.path -replace '/', '\')))
        if ($stale.StartsWith($SnapshotRoot + '\', [System.StringComparison]::OrdinalIgnoreCase) -and
            (Test-Path -LiteralPath $stale -PathType Leaf)) {
            Remove-Item -LiteralPath $stale -Force
        }
    }
    Remove-Item -LiteralPath $previousManifestPath -Force
}

$manifestTemp = Join-Path $SnapshotRoot '.dh2-source-manifest.tmp.json'
$before | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $manifestTemp -Encoding UTF8
Move-Item -LiteralPath $manifestTemp -Destination $manifestPath -Force
Write-Output "Snapshot verified: $SnapshotRoot"
Write-Output "Source files: $($before.files.Count); manifest: $manifestPath"
