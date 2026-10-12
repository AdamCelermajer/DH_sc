param([switch]$Apply)
$ErrorActionPreference = 'Stop'
$primary = 'C:\Users\adamc\Desktop\workspace\DH_sc'
$protected = Join-Path $primary '.local-inputs'
$backupRoot = Join-Path $protected 'cleanup-preserved-20261012'
$plan = Get-Content -Raw -LiteralPath (Join-Path $primary 'coordination\v21.3\IGNORED-CLEANUP-PLAN.json') | ConvertFrom-Json
$preservation = Get-Content -Raw -LiteralPath (Join-Path $primary 'coordination\v21.3\WORKTREE-PRESERVATION.json') | ConvertFrom-Json
$receipt = Join-Path $protected 'v21.3\root\ignored-cleanup-receipt.jsonl'

function Under([string]$Path, [string]$Root) {
    return [IO.Path]::GetFullPath($Path).StartsWith([IO.Path]::GetFullPath($Root).TrimEnd('\')+'\', [StringComparison]::OrdinalIgnoreCase)
}
function SafeFiles([string]$Root) {
    $stack = [Collections.Generic.Stack[string]]::new()
    $stack.Push($Root)
    $files = [Collections.Generic.List[object]]::new()
    while ($stack.Count) {
        $current = $stack.Pop()
        $entry = Get-Item -LiteralPath $current -Force
        if ($entry.Attributes -band [IO.FileAttributes]::ReparsePoint) { throw "Refusing reparse target: $current" }
        foreach ($item in Get-ChildItem -LiteralPath $current -Force) {
            if (-not (Under $item.FullName $Root)) { throw 'Target escaped its explicit root' }
            if ($item.Attributes -band [IO.FileAttributes]::ReparsePoint) { throw "Refusing nested reparse point: $($item.FullName)" }
            if ($item.PSIsContainer) { $stack.Push($item.FullName) } else { $files.Add($item) }
        }
    }
    return @($files)
}
function Record($value) { $value | ConvertTo-Json -Compress -Depth 6 | Add-Content -LiteralPath $receipt -Encoding UTF8 }

$actions = @()
foreach ($worktree in $plan.worktrees) {
    $root = [IO.Path]::GetFullPath([string]$worktree.path)
    $saved = @($preservation.worktrees | Where-Object { [IO.Path]::GetFullPath($_.path) -eq $root })
    if ($saved.Count -ne 1) { throw "No unique preserved HEAD: $root" }
    $head = (& git -C $root rev-parse HEAD).Trim()
    if ($LASTEXITCODE -ne 0 -or $head -ne $saved[0].head) { throw "HEAD changed since preservation: $root" }
    if (@(& git -C $root status --porcelain=v1 --untracked-files=all --ignored=no).Count) { throw "Uncommitted source remains: $root" }
    foreach ($ignored in $worktree.ignoredPaths) {
        $relative = ([string]$ignored.path).TrimEnd('/', '\').Replace('/', '\')
        $source = [IO.Path]::GetFullPath((Join-Path $root $relative))
        if (-not (Under $source $root) -or (Under $source $protected)) { throw "Unsafe explicit cleanup target: $source" }
        $entry = Get-Item -LiteralPath $source -Force
        if ($entry.Attributes -band [IO.FileAttributes]::ReparsePoint) { throw "Refusing reparse point: $source" }
        $files = if ($entry.PSIsContainer) { @(SafeFiles $source) } else { @($entry) }
        if ($ignored.classification -eq 'reproducible-cache-candidate-after-source-snapshot') {
            $allowed = $relative -match '(^|\\)__pycache__$' -or $relative -in @('port\android-native\.gradle', 'port\android-native\app\.cxx', 'port\android-native\app\build', 'port\android-native\build')
            if (-not $allowed) { throw "Cache path outside exact allowlist: $relative" }
            $actions += [pscustomobject]@{ Kind='cache'; Source=$source; Root=$root; Head=$head; Bytes=($files | Measure-Object Length -Sum).Sum; Files=$files }
        } elseif ($ignored.classification -eq 'preserve-and-move-verified') {
            if ($relative -ne '.local-inputs' -or $files.Count -ne $ignored.fileCount) { throw 'Owned input tree changed since inventory' }
            foreach ($file in $ignored.files) {
                $path = [IO.Path]::GetFullPath((Join-Path $source $file.relativePath))
                if (-not (Under $path $source) -or (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash -ne $file.sha256) { throw 'Owned input hash changed' }
            }
            $destination = Join-Path $backupRoot ($saved[0].label+'\local-inputs')
            if (-not (Under $destination $backupRoot) -or (Test-Path -LiteralPath $destination)) { throw 'Preserving existing backup destination' }
            $actions += [pscustomobject]@{ Kind='move'; Source=$source; Root=$root; Head=$head; Destination=$destination; Expected=$ignored.files; Bytes=$ignored.bytes }
        } elseif ($ignored.classification -eq 'preserve-before-worktree-removal') {
            if ($entry.PSIsContainer) { throw 'Unreviewed ignored directory requires separate archive' }
            $destination = Join-Path $backupRoot ($saved[0].label+'\'+$relative)
            if (-not (Under $destination $backupRoot) -or (Test-Path -LiteralPath $destination)) { throw 'Preserving existing backup destination' }
            $digest = (Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash
            $actions += [pscustomobject]@{ Kind='file'; Source=$source; Root=$root; Head=$head; Destination=$destination; Hash=$digest; Bytes=$entry.Length }
        } else { throw "Unreviewed classification: $($ignored.classification)" }
    }
}
if (-not $Apply) { $actions | Select-Object Kind,Source,Bytes | Format-Table -AutoSize; Write-Output 'DRY RUN: no data changed'; exit 0 }
if ((Get-Content -Raw -LiteralPath (Join-Path $primary 'coordination\v21.3\FINAL-REVIEW.md')) -notmatch 'Root accepts v21.3') { throw 'Release not accepted' }
if (Test-Path -LiteralPath $receipt) { throw 'Refusing to overwrite an execution receipt' }
Record @{ action='begin'; freeBefore=(Get-PSDrive C).Free; timeUtc=[DateTime]::UtcNow.ToString('o') }
foreach ($action in $actions) {
    if ((& git -C $action.Root rev-parse HEAD).Trim() -ne $action.Head) { throw 'Source HEAD changed after preflight' }
    if (-not (Under $action.Source $action.Root)) { throw 'Source target escaped before mutation' }
    if ($action.Kind -eq 'cache') {
        $null = SafeFiles $action.Source
        Remove-Item -LiteralPath $action.Source -Recurse -Force
    } else {
        New-Item -ItemType Directory -Path (Split-Path $action.Destination -Parent) -Force | Out-Null
        if ($action.Kind -eq 'move') {
            Move-Item -LiteralPath $action.Source -Destination $action.Destination
            foreach ($file in $action.Expected) {
                $path = [IO.Path]::GetFullPath((Join-Path $action.Destination $file.relativePath))
                if (-not (Under $path $action.Destination) -or (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash -ne $file.sha256) { throw 'Moved data did not preserve hash' }
            }
        } else {
            Copy-Item -LiteralPath $action.Source -Destination $action.Destination
            if ((Get-FileHash -LiteralPath $action.Destination -Algorithm SHA256).Hash -ne $action.Hash -or (Get-FileHash -LiteralPath $action.Source -Algorithm SHA256).Hash -ne $action.Hash) { throw 'Copied ignored file changed' }
            Remove-Item -LiteralPath $action.Source -Force
        }
    }
    Record @{ action=$action.Kind; source=$action.Source; destination=$action.Destination; bytes=$action.Bytes; head=$action.Head; timeUtc=[DateTime]::UtcNow.ToString('o') }
}
Record @{ action='complete'; freeAfter=(Get-PSDrive C).Free; timeUtc=[DateTime]::UtcNow.ToString('o') }
Write-Output "Prepared ignored data for safe worktree removal: $($actions.Count) actions; receipt $receipt"
