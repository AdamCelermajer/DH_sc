[CmdletBinding()]
param(
    [string]$PrimaryRoot = 'C:\Users\adamc\Desktop\workspace\DH_sc',
    [string]$OutputPath = 'C:\Users\adamc\Desktop\workspace\DH_sc\coordination\v21.3\IGNORED-CLEANUP-PLAN.json'
)

# Read-only inventory. It never moves, deletes, snapshots, or changes a worktree.
# Git's ignored=matching mode reports collapsed ignored directories. Only the
# exact shared .local-inputs junction to PrimaryRoot is omitted from the report.
# Recursive size/hash walks are restricted to explicitly recognized reproducible
# caches and owned worktree .local-inputs trees; no whole worktree is traversed.

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

function Get-FullPath([string]$Path) {
    return [System.IO.Path]::GetFullPath($Path.TrimEnd('\', '/'))
}

function Test-SamePath([string]$Left, [string]$Right) {
    return [string]::Equals((Get-FullPath $Left), (Get-FullPath $Right), [System.StringComparison]::OrdinalIgnoreCase)
}

function Test-PathUnder([string]$Path, [string]$Root) {
    $fullPath = Get-FullPath $Path
    $fullRoot = (Get-FullPath $Root).TrimEnd('\') + '\'
    return $fullPath.StartsWith($fullRoot, [System.StringComparison]::OrdinalIgnoreCase)
}

function Invoke-Git([string]$At, [string[]]$GitArguments) {
    $output = & git -C $At @GitArguments 2>&1
    if ($LASTEXITCODE -ne 0) { throw "git -C $At $($GitArguments -join ' ') failed: $($output -join ' ')" }
    return @($output | ForEach-Object { [string]$_ })
}

function Get-LiveWorktrees([string]$At) {
    $lines = Invoke-Git $At @('worktree', 'list', '--porcelain')
    $items = [System.Collections.Generic.List[object]]::new()
    $entry = @{}
    foreach ($line in $lines) {
        if ([string]::IsNullOrWhiteSpace($line)) {
            if ($entry.ContainsKey('worktree')) { $items.Add([pscustomobject]$entry) }
            $entry = @{}
            continue
        }
        $pair = $line -split ' ', 2
        if ($pair.Count -eq 2) { $entry[$pair[0]] = $pair[1] }
        else { $entry[$pair[0]] = $true }
    }
    if ($entry.ContainsKey('worktree')) { $items.Add([pscustomobject]$entry) }
    return @($items)
}

function Get-GitStatusRows([string]$Path) {
    return @(Invoke-Git $Path @('status', '--porcelain=v1', '--untracked-files=all'))
}

function Get-IgnoredPaths([string]$Path) {
    $rows = Invoke-Git $Path @('status', '--porcelain=v1', '--ignored=matching')
    $paths = [System.Collections.Generic.List[string]]::new()
    foreach ($row in $rows) {
        if (-not $row.StartsWith('!! ', [System.StringComparison]::Ordinal)) { continue }
        $relative = $row.Substring(3).Trim()
        if ($relative.StartsWith('"', [System.StringComparison]::Ordinal)) {
            try { $relative = [string]($relative | ConvertFrom-Json) } catch { throw "Cannot decode ignored Git status path in ${Path}: $row" }
        }
        $relative = $relative.Replace('\', '/')
        if ($relative -ne '.local-inputs/' -and $relative -ne '.local-inputs') { $paths.Add($relative) }
    }
    return @($paths | Sort-Object -Unique)
}

function Get-LocalInputsState([string]$WorktreePath, [string]$PrimaryInputs) {
    $local = Join-Path $WorktreePath '.local-inputs'
    $item = Get-Item -LiteralPath $local -Force -ErrorAction SilentlyContinue
    if ($null -eq $item) { return [pscustomobject]@{ kind = 'absent'; path = $local; target = $null } }
    if (-not ($item.Attributes -band [System.IO.FileAttributes]::ReparsePoint)) {
        return [pscustomobject]@{ kind = 'owned-directory'; path = $local; target = $null }
    }
    if ($item.LinkType -ne 'Junction') {
        return [pscustomobject]@{ kind = 'unexpected-reparse'; path = $local; target = $null }
    }
    $targets = @($item.Target)
    if ($targets.Count -ne 1 -or [string]::IsNullOrWhiteSpace([string]$targets[0])) {
        return [pscustomobject]@{ kind = 'unexpected-reparse'; path = $local; target = $null }
    }
    $target = Get-FullPath ([string]$targets[0])
    if (Test-SamePath $target $PrimaryInputs) {
        return [pscustomobject]@{ kind = 'shared-primary-junction'; path = $local; target = $target }
    }
    return [pscustomobject]@{ kind = 'unexpected-reparse'; path = $local; target = $target }
}

function Get-CacheKind([string]$RelativePath) {
    $p = $RelativePath.Replace('\', '/').TrimStart('./')
    if ($p -match '(^|/)__pycache__/$') { return 'python-bytecode-cache' }
    if ($p -eq 'port/android-native/.gradle/') { return 'android-gradle-cache' }
    if ($p -eq 'port/android-native/app/.cxx/') { return 'android-cxx-cache' }
    if ($p -eq 'port/android-native/app/build/') { return 'android-app-build-output' }
    if ($p -eq 'port/android-native/build/') { return 'android-build-output' }
    return $null
}

function Get-ExplicitTreeSummary([string]$Path, [switch]$HashFiles) {
    $stack = [System.Collections.Generic.Stack[string]]::new()
    $stack.Push((Get-FullPath $Path))
    $files = [System.Collections.Generic.List[object]]::new()
    $reparse = [System.Collections.Generic.List[string]]::new()
    [long]$bytes = 0
    while ($stack.Count -gt 0) {
        $current = $stack.Pop()
        foreach ($child in @(Get-ChildItem -LiteralPath $current -Force -ErrorAction Stop)) {
            if ($child.Attributes -band [System.IO.FileAttributes]::ReparsePoint) {
                $reparse.Add($child.FullName)
                continue
            }
            if ($child.PSIsContainer) {
                $stack.Push($child.FullName)
                continue
            }
            $length = [long]$child.Length
            $bytes += $length
            $entry = [ordered]@{ relativePath = $child.FullName.Substring((Get-FullPath $Path).Length).TrimStart('\'); bytes = $length }
            if ($HashFiles) { $entry.sha256 = (Get-FileHash -LiteralPath $child.FullName -Algorithm SHA256).Hash.ToLowerInvariant() }
            $files.Add([pscustomobject]$entry)
        }
    }
    return [pscustomobject]@{ bytes = $bytes; fileCount = $files.Count; reparsePoints = @($reparse); files = @($files) }
}

function Get-IgnoredEntry([string]$WorktreePath, [string]$RelativePath) {
    $normalized = $RelativePath.Replace('\', '/')
    $full = Get-FullPath (Join-Path $WorktreePath ($normalized.Replace('/', '\').TrimEnd('\')))
    if (-not (Test-Path -LiteralPath $full)) {
        return [pscustomobject]@{ path = $RelativePath; scope = 'worktree-relative'; kind = 'missing-at-capture'; classification = 'preserve-unresolved'; reason = 'Git reported this ignored path but its filesystem entry was absent during inspection.'; bytes = $null; fileCount = $null; reparsePoints = @() }
    }
    $item = Get-Item -LiteralPath $full -Force
    if ($item.Attributes -band [System.IO.FileAttributes]::ReparsePoint) {
        return [pscustomobject]@{ path = $RelativePath; scope = 'worktree-relative'; kind = 'reparse-point'; classification = 'preserve-and-review'; reason = 'Ignored path is a reparse point; inventory did not follow it.'; bytes = $null; fileCount = $null; reparsePoints = @($full) }
    }
    if (-not $item.PSIsContainer) {
        return [pscustomobject]@{ path = $RelativePath; scope = 'worktree-relative'; kind = 'file'; classification = 'preserve-before-worktree-removal'; reason = 'Ignored file is not on the explicit reproducible-cache allowlist; preserve a hash-verified copy under primary .local-inputs/cleanup-preserved first.'; bytes = [long]$item.Length; fileCount = 1; sha256 = (Get-FileHash -LiteralPath $full -Algorithm SHA256).Hash.ToLowerInvariant(); reparsePoints = @() }
    }
    $cacheKind = Get-CacheKind $normalized
    if ($cacheKind) {
        $summary = Get-ExplicitTreeSummary $full
        $blocked = $summary.reparsePoints.Count -gt 0
        return [pscustomobject]@{ path = $RelativePath; scope = 'worktree-relative'; kind = 'directory'; classification = $(if ($blocked) { 'preserve-and-review-reparse' } else { 'reproducible-cache-candidate-after-source-snapshot' }); reason = $(if ($blocked) { 'An explicit cache walk encountered reparse points and did not follow them.' } else { "Exact allowlisted $cacheKind; deletion still waits for source snapshot and root approval." }); bytes = $summary.bytes; fileCount = $summary.fileCount; reparsePoints = $summary.reparsePoints }
    }
    return [pscustomobject]@{ path = $RelativePath; scope = 'worktree-relative'; kind = 'directory'; classification = 'preserve-before-worktree-removal'; reason = 'Ignored directory is not on the explicit reproducible-cache allowlist; archive/hash its contents under primary .local-inputs/cleanup-preserved before worktree removal.'; bytes = $null; fileCount = $null; reparsePoints = @() }
}

$root = Get-FullPath $PrimaryRoot
$primaryInputs = Join-Path $root '.local-inputs'
$output = Get-FullPath $OutputPath
if (-not (Test-Path -LiteralPath $root -PathType Container)) { throw "PrimaryRoot does not exist: $root" }
if (-not (Test-Path -LiteralPath (Join-Path $root '.git'))) { throw "PrimaryRoot is not a Git checkout: $root" }
if (-not (Test-Path -LiteralPath $primaryInputs -PathType Container)) { throw "Protected primary .local-inputs is missing: $primaryInputs" }

$live = @(Get-LiveWorktrees $root)
$primary = @($live | Where-Object { Test-SamePath ([string]$_.worktree) $root })
if ($primary.Count -ne 1) { throw "Expected exactly one primary checkout in git worktree list; found $($primary.Count)" }
$extras = @($live | Where-Object { -not (Test-SamePath ([string]$_.worktree) $root) })
$approvedRoots = @((Join-Path (Split-Path $root -Parent) 'DH_wt'), (Join-Path $env:USERPROFILE '.codex\worktrees'))
$records = [System.Collections.Generic.List[object]]::new()

foreach ($entry in $extras) {
    $path = Get-FullPath ([string]$entry.worktree)
    if ((-not (Test-SamePath $path 'C:\DHqa')) -and ((@($approvedRoots | Where-Object { Test-PathUnder $path $_ })).Count -eq 0)) { throw "Live extra worktree falls outside approved roots: $path" }
    $rootItem = Get-Item -LiteralPath $path -Force -ErrorAction Stop
    if (-not $rootItem.PSIsContainer) { throw "Live worktree path is not a directory: $path" }
    $rootIsReparsePoint = [bool]($rootItem.Attributes -band [System.IO.FileAttributes]::ReparsePoint)
    if ($rootIsReparsePoint) { throw "Live worktree root is a reparse point; refusing to inspect through it: $path" }
    $headText = Invoke-Git $path @('rev-parse', '--verify', 'HEAD')
    $head = ($headText -join '').Trim().ToLowerInvariant()
    $status = @(Get-GitStatusRows $path)
    $local = Get-LocalInputsState $path $primaryInputs
    $ignoredPaths = @(Get-IgnoredPaths $path)
    $ignored = [System.Collections.Generic.List[object]]::new()
    foreach ($relative in $ignoredPaths) { $ignored.Add((Get-IgnoredEntry $path $relative)) }

    if ($local.kind -eq 'owned-directory') {
        $summary = Get-ExplicitTreeSummary $local.path -HashFiles
        $ignored.Add([pscustomobject]@{
            path = '.local-inputs/'; scope = 'worktree-relative'; kind = 'owned-directory'
            classification = $(if ($summary.reparsePoints.Count -gt 0) { 'preserve-and-review-reparse' } else { 'preserve-and-move-verified' })
            reason = 'Worktree-owned ignored data is unique. Move to an explicit verified destination and compare per-file SHA-256 values before cleanup.'
            bytes = $summary.bytes; fileCount = $summary.fileCount; reparsePoints = $summary.reparsePoints; files = $summary.files
        })
    } elseif ($local.kind -eq 'unexpected-reparse') {
        $ignored.Add([pscustomobject]@{ path = '.local-inputs/'; scope = 'worktree-relative'; kind = 'unexpected-reparse'; classification = 'preserve-and-review-reparse'; reason = 'Unexpected local-inputs reparse target; inventory did not follow it.'; bytes = $null; fileCount = $null; target = $local.target; reparsePoints = @($local.path) })
    }

    $branchProperty = $entry.PSObject.Properties['branch']
    $detachedProperty = $entry.PSObject.Properties['detached']
    $branch = if ($null -ne $branchProperty) { [string]$branchProperty.Value } elseif ($null -ne $detachedProperty -and $detachedProperty.Value) { '(detached)' } else { '(unknown)' }
    $records.Add([pscustomobject]@{
        path = $path; worktreeRootIsReparsePoint = $rootIsReparsePoint; branch = $branch; head = $head; gitStatusRows = $status.Count
        localInputs = $local; ignoredPathCount = $ignored.Count; ignoredPaths = @($ignored)
    })
}

$allIgnored = @($records | ForEach-Object { $_.ignoredPaths })
$result = [ordered]@{
    schemaVersion = 1
    generatedAtUtc = [DateTime]::UtcNow.ToString('o')
    primaryRoot = $root
    source = 'fresh git worktree list --porcelain plus per-worktree git status --porcelain=v1 --ignored=matching'
    readOnly = $true
    cleanupPerformed = $false
    exactExcludedIgnoredPath = '.local-inputs/'
    exclusionRule = 'Only a verified Junction targeting the primary .local-inputs is skipped. Owned directories and unexpected reparse points are reported.'
    worktreeCountIncludingPrimary = $live.Count
    extraWorktreeCount = $extras.Count
    ignoredPathCount = $allIgnored.Count
    classifications = @{
        reproducibleCacheCandidate = 'Only __pycache__/ and the exact port/android-native .gradle, app/.cxx, app/build, and build paths; candidate after source snapshots and root approval.'
        preserveBeforeRemoval = 'Every ignored file or directory outside that exact cache allowlist; hash-copy or archive under primary .local-inputs/cleanup-preserved first.'
        worktreeOwnedLocalInputs = 'Inventory exact owned directories, hashes and sizes; move to a verified destination before removal.'
    }
    worktrees = @($records)
}

$parent = Split-Path $output -Parent
if (-not (Test-Path -LiteralPath $parent -PathType Container)) { throw "Output parent does not exist: $parent" }
$json = $result | ConvertTo-Json -Depth 12
Set-Content -LiteralPath $output -Value $json -Encoding UTF8
Write-Output "Read-only inventory written: $output"
Write-Output "Worktrees: $($live.Count) total / $($extras.Count) extra; ignored entries: $($allIgnored.Count). No cleanup performed."
