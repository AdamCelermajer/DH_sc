$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
Set-Location $repo
Add-Type -AssemblyName System.IO.Compression.FileSystem

$canonical = 'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip'
$originalApk = '.local-inputs/ida-apk-export-2026-10-07/inputs/Dungeon-Hunter-2-HD-v1-0-2.apk'
$fullCacheApk = '.local-inputs/fsm-v166-installed-base.apk'
$staged = 'port/windows-foundation/features/interactions/test-assets/go_swamp_urn_breakable.bdae'
$member = 'com.gameloft.android.GAND.GloftD2SS/files/data/3d/gameobjects/go_swamp_urn_breakable.bdae'
$expectedCache = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
$expectedAsset = '534b1f5c99e1d005ffa9c7970412e496a062783b1b847abeeefcd1420f18706a'

function Get-StreamSha256($stream) {
    $sha = [System.Security.Cryptography.SHA256]::Create()
    try { return ([Convert]::ToHexString($sha.ComputeHash($stream))).ToLowerInvariant() }
    finally { $sha.Dispose() }
}

if ((Get-FileHash -LiteralPath $canonical -Algorithm SHA256).Hash.ToLowerInvariant() -ne $expectedCache) {
    throw 'Canonical source cache SHA256 changed'
}
$zip = [System.IO.Compression.ZipFile]::OpenRead((Resolve-Path $canonical))
try {
    $asset = $zip.GetEntry($member)
    if (-not $asset -or $asset.Length -ne 43828) { throw 'Exact authored breakable BDAE cache member missing or size changed' }
    $stream = $asset.Open()
    try { $memberHash = Get-StreamSha256 $stream } finally { $stream.Dispose() }
    if ($memberHash -ne $expectedAsset) { throw 'Canonical BDAE member SHA256 changed' }
} finally { $zip.Dispose() }
if ((Get-Item -LiteralPath $staged).Length -ne 43828 -or
    (Get-FileHash -LiteralPath $staged -Algorithm SHA256).Hash.ToLowerInvariant() -ne $expectedAsset) {
    throw 'Isolated feature test copy differs from the exact authored BDAE member'
}

$original = [System.IO.Compression.ZipFile]::OpenRead((Resolve-Path $originalApk))
try {
    if (@($original.Entries | Where-Object { $_.FullName -match '(?i)swamp_urn_breakable' }).Count) {
        throw 'Original APK unexpectedly has a direct breakable BDAE member; update the evidence receipt'
    }
} finally { $original.Dispose() }

$apk = [System.IO.Compression.ZipFile]::OpenRead((Resolve-Path $fullCacheApk))
try {
    $nestedEntry = $apk.GetEntry('assets/dh2-original-cache.zip')
    if (-not $nestedEntry -or $nestedEntry.Length -ne (Get-Item $canonical).Length) {
        throw 'Expected full-cache APK embedded source cache is absent or changed'
    }
    $memory = [System.IO.MemoryStream]::new()
    $nestedStream = $nestedEntry.Open()
    try { $nestedStream.CopyTo($memory) } finally { $nestedStream.Dispose() }
    $memory.Position = 0
    if ((Get-StreamSha256 $memory) -ne $expectedCache) {
        throw 'Embedded full-cache APK source cache differs from canonical original ZIP'
    }
    $memory.Position = 0
    $nested = [System.IO.Compression.ZipArchive]::new($memory,
        [System.IO.Compression.ZipArchiveMode]::Read, $true)
    try {
        $nestedAsset = $nested.GetEntry($member)
        if (-not $nestedAsset -or $nestedAsset.Length -ne 43828) { throw 'Embedded source cache does not contain the exact BDAE member' }
        $nestedAssetStream = $nestedAsset.Open()
        try { $nestedAssetHash = Get-StreamSha256 $nestedAssetStream }
        finally { $nestedAssetStream.Dispose() }
        if ($nestedAssetHash -ne $expectedAsset) { throw 'Embedded source BDAE hash differs from canonical member' }
    } finally { $nested.Dispose() }
    $memory.Dispose()
} finally { $apk.Dispose() }

$stagedFull = (Resolve-Path $staged).Path
foreach ($root in @((Resolve-Path '.local-inputs').Path, (Resolve-Path 'port').Path,
                    'C:\Users\adamc\Downloads\dungeonhunter2')) {
    if (-not (Test-Path $root)) { continue }
    $copies = @(Get-ChildItem -LiteralPath $root -Recurse -File -Filter 'go_swamp_urn_breakable.bdae' `
        -ErrorAction SilentlyContinue | Where-Object { $_.FullName -ne $stagedFull })
    if ($copies.Count) { throw "Unexpected decoded/staged copy outside isolated feature cache: $($copies[0].FullName)" }
}

& 'port/windows-foundation/features/interactions/run_session_container_interactions_tests.ps1'
if ($LASTEXITCODE -ne 0) { throw 'Native breakable interaction/source-bank fixture failed' }
Write-Output "PASS exact source cache/member/APK provenance and isolated BDAE SHA256 $expectedAsset"
