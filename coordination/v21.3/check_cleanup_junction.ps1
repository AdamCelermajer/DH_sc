# Prove nonrecursive unlinking on a disposable fixture, never on a game package.
$ErrorActionPreference = 'Stop'
$proofRoot = 'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\v21.3\root'
$fixture = Join-Path $proofRoot 'junction-method-proof-20261012'
if (Test-Path -LiteralPath $fixture) { throw 'Refusing to reuse an existing fixture' }
$target = Join-Path $fixture 'target'
$link = Join-Path $fixture 'link'
$marker = Join-Path $target 'preserve.txt'
foreach ($path in @($fixture, $target, $link, $marker)) {
    $absolute = [System.IO.Path]::GetFullPath($path)
    if (-not $absolute.StartsWith($proofRoot + '\', [System.StringComparison]::OrdinalIgnoreCase)) { throw 'Fixture path escaped proof root' }
}
New-Item -ItemType Directory -Path $target | Out-Null
[System.IO.File]::WriteAllText($marker, 'junction target must remain intact')
$before = (Get-FileHash -LiteralPath $marker -Algorithm SHA256).Hash
New-Item -ItemType Junction -Path $link -Target $target | Out-Null
$entry = Get-Item -LiteralPath $link -Force
if (-not ($entry.Attributes -band [System.IO.FileAttributes]::ReparsePoint)) { throw 'Fixture is not a reparse point' }
if ([System.IO.Path]::GetFullPath($entry.Target) -ne $target) { throw 'Unexpected junction target' }
[System.IO.Directory]::Delete($link, $false)
if (Test-Path -LiteralPath $link) { throw 'Junction was not removed' }
$after = (Get-FileHash -LiteralPath $marker -Algorithm SHA256).Hash
if ($before -ne $after) { throw 'Target changed while unlinking junction' }
@{ method = 'System.IO.Directory.Delete(link,false)'; junction = $link; target = $target; targetSha256Before = $before; targetSha256After = $after; preserved = $true } |
    ConvertTo-Json | Set-Content -LiteralPath (Join-Path $proofRoot 'junction-unlink-proof.json') -Encoding UTF8
Remove-Item -LiteralPath $marker -Force
[System.IO.Directory]::Delete($target, $false)
[System.IO.Directory]::Delete($fixture, $false)
Write-Output 'PASS: nonrecursive junction unlink preserves its target'
