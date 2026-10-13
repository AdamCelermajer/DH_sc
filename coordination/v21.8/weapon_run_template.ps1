# TEMPLATE - NOT EXECUTED (v21.8 weapon matrix, Step 3). Verify before trusting any output.
#
# Batch runner: for each main-hand ItemTable identifier, start the game hidden with the
# existing --combat-main-item flag, press the attack key a fixed number of times, keep the
# run log and frame captures, and summarise the dispatch lines.
#
# Inputs you must supply (none are hard-coded here):
#   -Exe          built dh-foundation.exe (copied into each case folder, hash recorded)
#   -Package      staged package folder (contains assets/, audio-assets/, ui-assets/)
#   -TemplateArgs run.args template (a --startup-config file) whose paths are rewritten
#   -Seed         profile .save to copy into each case (must be a class the attack bank supports)
#   -Injector     key/click injector script (e.g. quiet_click_sync.ps1) that accepts -Exe -ExeArgs -Cwd -Log -Clicks -TimeoutSec
#   -Items        ItemTable identifiers, e.g. Longsword01,Staff01,Bow01,TwoHandsSword01
#   -CaseRoot     folder for case output (must NOT exist for a case; the script refuses to reuse one)
#
# Expected limits (from coordination/v21.8/WEAPON-VERIFICATION-MATRIX.md section 8):
#   - the attack bank is only built for RoguePlayerBase (main.cpp:1750); other classes will show animation-only
#   - there is no --combat-off-item flag, so dual wield cannot be set from the command line
#   - no player projectile launcher exists on this branch; a projectile line only appears on the mage branch
param(
  [Parameter(Mandatory=$true)][string]$Exe,
  [Parameter(Mandatory=$true)][string]$Package,
  [Parameter(Mandatory=$true)][string]$TemplateArgs,
  [Parameter(Mandatory=$true)][string]$Seed,
  [Parameter(Mandatory=$true)][string]$Injector,
  [Parameter(Mandatory=$true)][string]$CaseRoot,
  [string[]]$Items = @('Longsword01'),
  [string]$Clicks = '',
  [int]$Frames = 600,
  [int]$Timeout = 180,
  [int[]]$CaptureFrames = @(120, 240, 360)
)
$ErrorActionPreference = 'Stop'

# Dispatch lines to summarise. These are the log strings the port prints today.
$patterns = @(
  'Selected equipment initialization main=',       # main hand actually resolved from the flag
  'Selected class ',                               # animation-only vs full profile
  'Source melee stance',                           # stance resolution errors
  'Source attack',                                 # attack bank planning (errors or success)
  'Source skill bank preloaded',                   # skill bank built for the class
  'Source target command',                         # Tab target selection
  'projectile',                                    # projectile launch/refusal (mage branch only)
  'launch accepted',                               # projectile launcher (mage branch only)
  'attack',                                        # generic attack lines
  'swing',                                         # swing FX observer lines
  'sound'                                          # sound dispatch lines (if logged)
)

if (-not (Test-Path -LiteralPath $Exe)) { throw "Exe not found: $Exe" }
if (-not (Test-Path -LiteralPath $Seed)) { throw "Seed not found: $Seed" }
New-Item -ItemType Directory -Force -Path $CaseRoot | Out-Null
$summary = New-Object System.Collections.Generic.List[object]

foreach ($item in $Items) {
  $case = Join-Path $CaseRoot $item
  if (Test-Path -LiteralPath $case) { throw "case exists, refusing to reuse: $case" }
  New-Item -ItemType Directory -Path $case | Out-Null
  Copy-Item -LiteralPath $Exe -Destination (Join-Path $case 'run.exe')
  (Get-FileHash -LiteralPath $Exe).Hash | Set-Content -LiteralPath (Join-Path $case 'exe-sha256.txt')
  Copy-Item -LiteralPath $Seed -Destination (Join-Path $case 'profile.save')

  # Rewrite the run.args template: asset roots -> $Package, save -> case copy, add the weapon flag and captures.
  $lines = Get-Content -LiteralPath $TemplateArgs | ForEach-Object { $_.TrimEnd("`r") }
  $out = New-Object System.Collections.Generic.List[string]
  for ($i = 0; $i -lt $lines.Count; $i++) {
    $prev = if ($i -gt 0) { $lines[$i - 1] } else { '' }
    $l = $lines[$i]
    if ($prev -in '--assets', '--menu-assets') { $l = Join-Path $Package 'assets' }
    elseif ($prev -eq '--audio-assets') { $l = Join-Path $Package 'audio-assets' }
    elseif ($prev -eq '--save') { $l = Join-Path $case 'profile.save' }
    elseif ($prev -eq '--frames') { $l = [string]$Frames }
    $out.Add($l)
  }
  $out.Add('--combat-main-item'); $out.Add($item)
  $out.Add('--test-player-hp'); $out.Add('1000')
  foreach ($c in $CaptureFrames) { $out.Add('--capture-at'); $out.Add("${c}:" + (Join-Path $case "f$c.ppm")) }
  $out | Set-Content -LiteralPath (Join-Path $case 'run.args')
  Set-Content -LiteralPath (Join-Path $case 'physical-inputs.txt') -Value $Clicks

  # Run hidden. The injector sends the real keys in $Clicks; it must accept these parameters.
  & $Injector -Exe (Join-Path $case 'run.exe') -ExeArgs @('--startup-config', (Join-Path $case 'run.args')) `
      -Cwd $case -Log (Join-Path $case 'run.log') -Clicks $Clicks -TimeoutSec $Timeout | Out-File -FilePath (Join-Path $case 'injector.txt')

  $log = Join-Path $case 'run.log'
  $hits = @()
  if (Test-Path -LiteralPath $log) {
    foreach ($p in $patterns) {
      $n = (Select-String -LiteralPath $log -SimpleMatch -Pattern $p | Measure-Object).Count
      $hits += "$p=$n"
    }
  }
  $frames = @(Get-ChildItem -LiteralPath $case -Filter '*.ppm' -ErrorAction SilentlyContinue).Count
  $summary.Add([pscustomobject]@{ Item = $item; Frames = $frames; Counts = ($hits -join ' ') })
}

$summary | Format-Table -AutoSize | Out-String | Set-Content -LiteralPath (Join-Path $CaseRoot 'summary.txt')
$summary | Format-Table -AutoSize
