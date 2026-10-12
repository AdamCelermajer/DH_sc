[CmdletBinding()]
param(
    [string]$PrimaryRoot = 'C:\Users\adamc\Desktop\workspace\DH_sc',
    [string]$SafetyInventory = 'C:\Users\adamc\Desktop\workspace\DH_sc\coordination\v21.3\CLEANUP-SAFETY.json',
    [string]$ManifestPath,
    [string]$ReceiptPath,
    [switch]$Apply
)

# Safe worktree cleanup runner. Default mode is a read-only dry run.
# Apply requires a fresh, complete, root-reviewed manifest with this shape:
# {
#   "schemaVersion": 1, "primaryRoot": "...", "scopeReviewed": true,
#   "capturedAtUtc": "...", "expectedWorktrees": [{"path":"...","head":"<40 hex>"}, ...],
#   "removals": [{"path":"...","head":"<40 hex>",
#     "preservationRef":"refs/heads/...", "localInputsPreserved": true,
#     "ownerSnapshotComplete": true}, ...],
#   "holds": [{"path":"...","head":"<40 hex>","reason":"..."}, ...]
# }
# expectedWorktrees must cover every live extra worktree exactly once. A removal
# must have a local ref resolving exactly to its captured HEAD, a clean Git status,
# and no worktree-owned ignored .local-inputs directory. Junctions to the primary
# .local-inputs are revalidated and unlinked nonrecursively before git worktree
# remove. Holds document exclusions such as the primary checkout, active integration,
# app-managed worktrees awaiting archive_worktree, and unsnapshotted dirty trees.

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

function Test-ApprovedWorktreeScope([string]$Path) {
    if (Test-SamePath $Path 'C:\DHqa') { return $true }
    return (@($approvedRoots | Where-Object { Test-PathUnder $Path $_ })).Count -gt 0
}

function Invoke-Git([string[]]$GitArgs, [switch]$AllowFailure) {
    $output = & git -C $script:root @GitArgs 2>&1
    $exit = $LASTEXITCODE
    if ($exit -ne 0 -and -not $AllowFailure) {
        throw "git $($GitArgs -join ' ') failed ($exit): $($output -join ' ')"
    }
    return [pscustomobject]@{ ExitCode = $exit; Output = @($output | ForEach-Object { [string]$_ }) }
}

function Get-LiveWorktrees {
    $result = Invoke-Git @('worktree', 'list', '--porcelain')
    $items = [System.Collections.Generic.List[object]]::new()
    $entry = @{}
    foreach ($line in $result.Output) {
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

function Get-Head([string]$Path) {
    $out = & git -C $Path rev-parse --verify HEAD 2>&1
    if ($LASTEXITCODE -ne 0) { throw "Cannot resolve HEAD in ${Path}: $($out -join ' ')" }
    return ([string]$out).Trim().ToLowerInvariant()
}

function Get-Status([string]$Path) {
    $out = & git -C $Path status --porcelain=v1 --untracked-files=all --ignored=no 2>&1
    if ($LASTEXITCODE -ne 0) { throw "Cannot read Git status in ${Path}: $($out -join ' ')" }
    return @($out | ForEach-Object { [string]$_ })
}

function Get-IgnoredStatusEntries([string]$Path) {
    # `matching` collapses ignored directories, so this stays bounded and does not
    # recursively enumerate ignored trees. The shared root junction is the sole
    # permitted ignored status path; Get-LocalInputsState validates its target.
    $out = & git -C $Path status --porcelain=v1 --ignored=matching 2>&1
    if ($LASTEXITCODE -ne 0) { throw "Cannot read ignored Git status in ${Path}: $($out -join ' ')" }
    $ignored = [System.Collections.Generic.List[string]]::new()
    foreach ($line in $out) {
        $text = [string]$line
        if (-not $text.StartsWith('!! ', [System.StringComparison]::Ordinal)) { continue }
        $relative = $text.Substring(3).Trim()
        if ($relative -eq '.local-inputs/' -or $relative -eq '.local-inputs') { continue }
        $ignored.Add($relative)
    }
    return @($ignored)
}

function Get-LocalInputsState([string]$WorktreePath) {
    $local = Join-Path $WorktreePath '.local-inputs'
    if (-not (Test-Path -LiteralPath $local)) { return [pscustomobject]@{ Kind = 'absent'; Target = $null } }
    $item = Get-Item -LiteralPath $local -Force
    if (-not ($item.Attributes -band [System.IO.FileAttributes]::ReparsePoint)) {
        return [pscustomobject]@{ Kind = 'owned'; Target = $null }
    }
    if ($item.LinkType -ne 'Junction') {
        return [pscustomobject]@{ Kind = 'unexpected-reparse'; Target = $null }
    }
    $targets = @($item.Target)
    if ($targets.Count -ne 1 -or [string]::IsNullOrWhiteSpace([string]$targets[0])) {
        return [pscustomobject]@{ Kind = 'unexpected-reparse'; Target = $null }
    }
    return [pscustomobject]@{ Kind = 'junction'; Target = (Get-FullPath ([string]$targets[0])) }
}

$script:root = Get-FullPath $PrimaryRoot
$primaryInputs = Join-Path $script:root '.local-inputs'
$safetyPath = Get-FullPath $SafetyInventory
if (-not (Test-Path -LiteralPath $script:root -PathType Container)) { throw "PrimaryRoot does not exist: $script:root" }
if (-not (Test-Path -LiteralPath (Join-Path $script:root '.git'))) { throw "PrimaryRoot is not a Git checkout: $script:root" }
if (-not (Test-Path -LiteralPath $primaryInputs -PathType Container)) { throw "Protected primary .local-inputs is missing: $primaryInputs" }
if (-not (Test-Path -LiteralPath $safetyPath -PathType Leaf)) { throw "Safety inventory is missing: $safetyPath" }
$safety = Get-Content -LiteralPath $safetyPath -Raw | ConvertFrom-Json
if (-not (Test-SamePath ([string]$safety.primaryLocalInputs.path) $primaryInputs)) { throw 'Safety inventory primary .local-inputs path does not match PrimaryRoot' }

$live = @(Get-LiveWorktrees)
$primaryEntries = @($live | Where-Object { Test-SamePath ([string]$_.worktree) $script:root })
if ($primaryEntries.Count -ne 1) { throw "Expected exactly one primary worktree entry; found $($primaryEntries.Count)" }
$extras = @($live | Where-Object { -not (Test-SamePath ([string]$_.worktree) $script:root) })
$approvedRoots = @((Join-Path (Split-Path $script:root -Parent) 'DH_wt'), (Join-Path $env:USERPROFILE '.codex\worktrees'))

if (-not $Apply) {
    Write-Output "DRY RUN — no paths changed. Live worktrees: $($live.Count); extra worktrees: $($extras.Count)."
    Write-Output "Primary checkout preserved: $script:root"
    Write-Output "Protected accepted packages, saves and evidence root preserved: $primaryInputs"
    foreach ($entry in $extras) {
        $path = Get-FullPath ([string]$entry.worktree)
        $head = Get-Head $path
        $status = @(Get-Status $path)
        $local = Get-LocalInputsState $path
        $ignored = @(Get-IgnoredStatusEntries $path)
        $branchProperty = $entry.PSObject.Properties['branch']
        $detachedProperty = $entry.PSObject.Properties['detached']
        $branch = if ($null -ne $branchProperty) { [string]$branchProperty.Value } elseif ($null -ne $detachedProperty -and $detachedProperty.Value) { '(detached)' } else { '(unknown)' }
        $b117Path = Join-Path $env:USERPROFILE '.codex\worktrees\b117-chest-crash\DH_sc'
        $b118Path = Join-Path $env:USERPROFILE '.codex\worktrees\b118-continue-crash\DH_sc'
        $gate = if ((Test-SamePath $path $b117Path) -or (Test-SamePath $path $b118Path)) { 'HOLD: app-managed worktree; archive through Codex after release' } elseif ($status.Count -gt 0) { 'HOLD: dirty tracked/untracked files' } elseif ($local.Kind -eq 'owned') { 'HOLD: worktree-owned ignored .local-inputs requires separate preservation' } elseif ($local.Kind -eq 'unexpected-reparse') { 'HOLD: unexpected .local-inputs reparse point' } elseif ($local.Kind -eq 'junction' -and -not (Test-SamePath $local.Target $primaryInputs)) { 'HOLD: junction target differs from protected primary .local-inputs' } elseif ($ignored.Count -gt 0) { 'HOLD: ignored nested data requires separate preservation' } elseif (-not (Test-ApprovedWorktreeScope $path)) { 'HOLD: outside approved worktree roots' } else { 'READY FOR ROOT MANIFEST REVIEW ONLY' }
        [pscustomobject]@{ Path = $path; Branch = $branch; Head = $head; GitStatusRows = $status.Count; IgnoredPaths = $ignored; LocalInputs = $local.Kind; JunctionTarget = $local.Target; Decision = $gate } | ConvertTo-Json -Compress
    }
    if (-not $ManifestPath) { Write-Output 'Execution requires -Apply and a fresh complete preservation manifest.' }
    exit 0
}

if (-not $ManifestPath) { throw '-Apply requires -ManifestPath; no cleanup is performed without a preservation manifest.' }
$manifestFile = Get-FullPath $ManifestPath
if (-not (Test-Path -LiteralPath $manifestFile -PathType Leaf)) { throw "Manifest does not exist: $manifestFile" }
$manifest = Get-Content -LiteralPath $manifestFile -Raw | ConvertFrom-Json
if ([int]$manifest.schemaVersion -ne 1) { throw 'Unsupported preservation manifest schemaVersion' }
if (-not (Test-SamePath ([string]$manifest.primaryRoot) $script:root)) { throw 'Manifest primaryRoot does not match this checkout' }
if ($manifest.scopeReviewed -ne $true) { throw 'Manifest must explicitly set scopeReviewed=true' }
if ([string]::IsNullOrWhiteSpace([string]$manifest.capturedAtUtc)) { throw 'Manifest must include capturedAtUtc from the fresh pre-cleanup inventory' }

$expected = @($manifest.expectedWorktrees)
$removals = @($manifest.removals)
$holds = @($manifest.holds)
$listed = @($removals) + @($holds)
if ($expected.Count -ne $extras.Count -or $listed.Count -ne $extras.Count) { throw "Manifest scope mismatch: expectedWorktrees=$($expected.Count), removals+holds=$($listed.Count), live extras=$($extras.Count)" }

$liveMap = @{}
foreach ($entry in $extras) {
    $path = Get-FullPath ([string]$entry.worktree)
    if (-not (Test-ApprovedWorktreeScope $path)) { throw "Live extra worktree falls outside approved roots: $path" }
    if ($liveMap.ContainsKey($path)) { throw "Duplicate live worktree path: $path" }
    $liveMap[$path] = [pscustomobject]@{ Entry = $entry; Head = Get-Head $path }
}

$expectedMap = @{}
foreach ($item in $expected) {
    $path = Get-FullPath ([string]$item.path)
    if (-not $liveMap.ContainsKey($path)) { throw "Manifest contains unknown or stale worktree path: $path" }
    if ($expectedMap.ContainsKey($path)) { throw "Manifest repeats expectedWorktrees path: $path" }
    if (([string]$item.head).ToLowerInvariant() -ne $liveMap[$path].Head) { throw "Live HEAD differs from fresh inventory for $path" }
    $expectedMap[$path] = $true
}
if ($expectedMap.Count -ne $liveMap.Count) { throw 'Fresh manifest does not enumerate every live extra worktree' }

$actionMap = @{}
foreach ($item in $listed) {
    $path = Get-FullPath ([string]$item.path)
    if (-not $liveMap.ContainsKey($path)) { throw "Manifest contains unknown/stale action path: $path" }
    if ($actionMap.ContainsKey($path)) { throw "Manifest repeats action path: $path" }
    if (([string]$item.head).ToLowerInvariant() -ne $liveMap[$path].Head) { throw "Action HEAD differs from live HEAD for $path" }
    if (@($holds | Where-Object { Test-SamePath ([string]$_.path) $path }).Count -gt 0 -and [string]::IsNullOrWhiteSpace([string]$item.reason)) { throw "Every held worktree needs a concrete reason: $path" }
    $actionMap[$path] = $item
}
if ($actionMap.Count -ne $liveMap.Count) { throw 'Every live extra worktree must appear exactly once in removals or holds' }

# Preflight every approved removal before touching any link or checkout.
$plans = [System.Collections.Generic.List[object]]::new()
foreach ($item in $removals) {
    $path = Get-FullPath ([string]$item.path)
    if ($item.ownerSnapshotComplete -ne $true) { throw "ownerSnapshotComplete=true is required for $path" }
    if ($item.localInputsPreserved -ne $true) { throw "localInputsPreserved=true is required for $path" }
    $b117 = Join-Path $env:USERPROFILE '.codex\worktrees\b117-chest-crash\DH_sc'
    $b118 = Join-Path $env:USERPROFILE '.codex\worktrees\b118-continue-crash\DH_sc'
    $activeIntegration = Join-Path (Split-Path $script:root -Parent) 'DH_wt\v201int'
    if ((Test-SamePath $path $b117) -or (Test-SamePath $path $b118)) { throw "App-managed crash worktrees must first be archived with Codex archive_worktree: $path" }
    $releaseAcceptedProperty = $manifest.PSObject.Properties['releaseAccepted']
    if ((Test-SamePath $path $activeIntegration) -and ($null -eq $releaseAcceptedProperty -or $releaseAcceptedProperty.Value -ne $true)) { throw "Active integration worktree remains a release gate until manifest explicitly records releaseAccepted=true: $path" }
    $ref = [string]$item.preservationRef
    if ($ref -notmatch '^refs/(heads|remotes|tags|codex)/[A-Za-z0-9._/-]+$') { throw "Invalid explicit preservationRef for ${path}: $ref" }
    $refOut = & git -C $script:root rev-parse --verify "$ref^{commit}" 2>&1
    if ($LASTEXITCODE -ne 0 -or ([string]$refOut).Trim().ToLowerInvariant() -ne $liveMap[$path].Head) { throw "Preservation ref does not resolve exactly to live HEAD for ${path}: $ref" }
    $status = @(Get-Status $path)
    if ($status.Count -gt 0) { throw "Refusing dirty worktree $path ($($status.Count) status rows); snapshot and commit all tracked/untracked source first" }
    $ignored = @(Get-IgnoredStatusEntries $path)
    if ($ignored.Count -gt 0) { throw "Refusing worktree with ignored nested data in ${path}: $($ignored -join ', ')" }
    $local = Get-LocalInputsState $path
    if ($local.Kind -eq 'owned') { throw "Refusing to delete worktree-owned .local-inputs in $path; preserve/verify it, remove it from the checkout, then refresh the manifest" }
    if ($local.Kind -eq 'unexpected-reparse') { throw "Refusing unexpected .local-inputs reparse point in $path" }
    if ($local.Kind -eq 'junction' -and -not (Test-SamePath $local.Target $primaryInputs)) { throw "Refusing .local-inputs junction with unexpected target in $path" }
    $lockedProperty = $liveMap[$path].Entry.PSObject.Properties['locked']
    if ($null -ne $lockedProperty -and $lockedProperty.Value) { throw "Refusing locked worktree $path" }
    $plans.Add([pscustomobject]@{ Path = $path; Head = $liveMap[$path].Head; Ref = $ref; LocalInputs = $local })
}

if (-not $ReceiptPath) {
    $stamp = [DateTime]::UtcNow.ToString('yyyyMMddTHHmmssZ')
    $ReceiptPath = Join-Path $primaryInputs "v21.3\root\worktree-cleanup-$stamp.jsonl"
}
$receipt = Get-FullPath $ReceiptPath
$receiptRoot = Join-Path $primaryInputs 'v21.3\root'
if (-not (Test-PathUnder $receipt $receiptRoot)) { throw "ReceiptPath must remain under the protected root evidence directory: $receiptRoot" }
if (Test-Path -LiteralPath $receipt) { throw "Refusing to overwrite cleanup receipt: $receipt" }
$receiptDir = Split-Path $receipt -Parent
if (-not (Test-Path -LiteralPath $receiptDir -PathType Container)) { throw "Receipt directory must already exist: $receiptDir" }

Write-Output "APPLY — fully preflighted $($plans.Count) worktree removals; $($holds.Count) explicit holds remain."
foreach ($plan in $plans) {
    # Recheck immediately before each destructive step.
    if (-not (Test-Path -LiteralPath $plan.Path -PathType Container)) { throw "Worktree disappeared after preflight: $($plan.Path)" }
    if ((Get-Head $plan.Path) -ne $plan.Head) { throw "HEAD changed after preflight: $($plan.Path)" }
    if ((@(Get-Status $plan.Path)).Count -gt 0) { throw "Worktree became dirty after preflight: $($plan.Path)" }
    $local = Get-LocalInputsState $plan.Path
    if ($local.Kind -ne $plan.LocalInputs.Kind -or ($local.Kind -eq 'junction' -and -not (Test-SamePath $local.Target $primaryInputs))) { throw "Local-inputs entry changed after preflight: $($plan.Path)" }

    $record = [ordered]@{ timeUtc = [DateTime]::UtcNow.ToString('o'); action = 'begin'; path = $plan.Path; head = $plan.Head; preservationRef = $plan.Ref; localInputs = $local.Kind; junctionTarget = $local.Target }
    $record | ConvertTo-Json -Compress | Add-Content -LiteralPath $receipt -Encoding UTF8
    if ($local.Kind -eq 'junction') {
        # .NET Directory.Delete(path, false) unlinks the junction itself and does not traverse its target.
        [System.IO.Directory]::Delete((Join-Path $plan.Path '.local-inputs'), $false)
        if (Test-Path -LiteralPath (Join-Path $plan.Path '.local-inputs')) { throw "Failed to unlink .local-inputs junction in $($plan.Path)" }
        ([ordered]@{ timeUtc = [DateTime]::UtcNow.ToString('o'); action = 'junction-unlinked'; path = $plan.Path; junctionTargetPreserved = $local.Target }) |
            ConvertTo-Json -Compress | Add-Content -LiteralPath $receipt -Encoding UTF8
    }
    $remove = Invoke-Git @('worktree', 'remove', $plan.Path) -AllowFailure
    if ($remove.ExitCode -ne 0) { throw "git worktree remove refused $($plan.Path): $($remove.Output -join ' ')" }
    if (Test-Path -LiteralPath $plan.Path) { throw "Git reported removal but path remains: $($plan.Path)" }
    ([ordered]@{ timeUtc = [DateTime]::UtcNow.ToString('o'); action = 'removed'; path = $plan.Path; head = $plan.Head; preservationRef = $plan.Ref; junctionTargetPreserved = ($local.Kind -eq 'junction') }) |
        ConvertTo-Json -Compress | Add-Content -LiteralPath $receipt -Encoding UTF8
    Write-Output "Removed $($plan.Path) at $($plan.Head); preserved by $($plan.Ref)."
}
Write-Output "Cleanup receipt: $receipt"
