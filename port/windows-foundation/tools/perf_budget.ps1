<#
.SYNOPSIS
B066 performance regression guard: runs Swamp scenarios (idle, melee, skills 1/2/3, Faery) with DH_PERF=1 through
quiet_run.ps1 (hidden desktop, silent) and fails when a per-frame budget is exceeded.
.DESCRIPTION
Budgets are per-frame costs that scale with CPU/driver speed or reveal structural regressions (per-frame path resolution,
unbatched/unculled level draws, per-frame glFinish, sleep rounding). Defaults leave ~2.5x headroom over the 4070/Ryzen
measurements in B066-report.md so a slow CI box does not flake, while the regressions fixed in B062/B066 (4.5 ms per FX
packet, 392 level draw calls, 15.6 ms sleep ticks) trip them. Override with -Budget @{ workMs = 12 } etc.
-Package: a Windows package root containing swamp.args, assets/ (e.g. .local-inputs/windows-source-clock-v19-preview-15).
-SaveDir: folder with character.save + gameplay.save of a knight with the skills used by the scenarios.
Exit code 0 = all budgets met, 1 = a budget was exceeded, 2 = the scenario did not produce a Perf summary.
#>
param(
  [Parameter(Mandatory=$true)][string]$Exe,
  [Parameter(Mandatory=$true)][string]$Package,
  [Parameter(Mandatory=$true)][string]$SaveDir,
  [string]$WorkDir = "$env:TEMP/dh-perf-budget",
  [hashtable]$Budget = @{}
)
$ErrorActionPreference = 'Stop'
$limits = @{
  workMs = 9.0            # avg ms/frame excluding pacing sleep and swap (measured 2.6-4.5)
  p50Ms = 18.5            # median frame: 60 fps cap + vsync, never the 31 ms double wait
  p99Ms = 45.0
  over33 = 6              # frames > 33 ms in a 420-520 frame run (first-frame upload and first-use FX textures)
  worldCalls = 160        # level + actor draw calls per frame (392 level ranges are culled/batched to ~80)
  worldClientKB = 4000    # client-array bytes the driver may copy per frame (scene was ~10x larger before VBOs)
  fxRenderPrepMs = 1.5    # B062: FX render prep per frame (was 5-8 ms)
  hudMs = 2.0
  fxDrainMs = 3.0         # B066: finish_and_drain must not glFinish every frame
}
foreach($k in $Budget.Keys){ $limits[$k] = $Budget[$k] }
$scen = @(
  @{n='idle'; keys=@(); space=''; frames=420}, @{n='melee'; keys=@(); space='60:400'; frames=520},
  @{n='k1'; keys=@('150:1','300:1'); space='60:400'; frames=520}, @{n='k2'; keys=@('150:2','300:2'); space='60:400'; frames=520},
  @{n='k3'; keys=@('150:3','300:3'); space='60:400'; frames=520}, @{n='faery'; keys=@('150:4','300:5'); space='60:400'; frames=520})
if(Test-Path $WorkDir){ Remove-Item -Recurse -Force $WorkDir }
New-Item -ItemType Directory -Force -Path $WorkDir | Out-Null
$lines = @(Get-Content -LiteralPath "$Package/swamp.args" | Where-Object { $_ -ne '' -and -not $_.StartsWith('#') })
$jobs = @()
foreach($s in $scen){
  $d = "$WorkDir/$($s.n)"; New-Item -ItemType Directory -Force -Path $d | Out-Null
  Copy-Item "$SaveDir/character.save","$SaveDir/gameplay.save" $d -Force
  $a = New-Object System.Collections.ArrayList
  for($i=0; $i -lt $lines.Count; $i++){ if($lines[$i] -eq '--game-save'){ $i++; continue }; [void]$a.Add($lines[$i]) }
  foreach($x in @('--save',"$d/character.save",'--game-save',"$d/gameplay.save",'--frames',"$($s.frames)")){ [void]$a.Add($x) }
  foreach($k in $s.keys){ [void]$a.Add('--skill-key-frame'); [void]$a.Add($k) }
  if($s.space){ [void]$a.Add('--space-key-interval'); [void]$a.Add($s.space) }
  $jobs += [ordered]@{ name=$s.n; exe=$Exe; args=$a.ToArray(); cwd=$Package; log="$d/run.log"; timeoutSec=300 }
}
ConvertTo-Json $jobs -Depth 5 | Set-Content -LiteralPath "$WorkDir/jobs.json" -Encoding utf8
$env:DH_PERF = '1'; $env:DH_PERF_LOG = "$WorkDir/perf.log"; $env:DH_FPS_CAP = ''; $env:DH_VSYNC = ''
& "$PSScriptRoot/quiet_run.ps1" -JobsFile "$WorkDir/jobs.json" -Parallel 1 -Summary "$WorkDir/summary.json" | Out-Null
function Get-Kv($line){ $h=@{}; foreach($m in [regex]::Matches($line,'(\w+)=([-\d\.e]+)')){ $h[$m.Groups[1].Value]=[double]$m.Groups[2].Value }; $h }
$failed = $false; $missing = $false
foreach($s in $scen){
  $log = "$WorkDir/$($s.n)/run.log"
  $sum = if(Test-Path $log){ Select-String -Path $log -Pattern '^Perf summary' | Select-Object -First 1 }
  if(-not $sum){ Write-Host "[$($s.n)] no Perf summary (scenario failed to run)"; $missing = $true; continue }
  $m = Get-Kv $sum.Line
  $gl = Select-String -Path $log -Pattern '^Perf gl' | Select-Object -Last 1
  $world = @{ calls=0; clientKB=0 }
  if($gl){ $w = ($gl.Line -split '\|')[1]; $kv = Get-Kv $w; $world.calls=$kv['calls']; $world.clientKB=$kv['clientKB'] }
  $got = @{ workMs=$m['workMs']; p50Ms=$m['p50']; p99Ms=$m['p99']; over33=$m['over33ms']; worldCalls=$world.calls; worldClientKB=$world.clientKB
            fxRenderPrepMs=$m['fxRenderPrep']; hudMs=$m['hudUi']; fxDrainMs=$m['fxDrain'] }
  foreach($k in ($limits.Keys | Sort-Object)){
    $v = $got[$k]; $ok = ($null -ne $v) -and ($v -le $limits[$k])
    Write-Host ("[{0}] {1} = {2} (budget {3}) {4}" -f $s.n,$k,$v,$limits[$k],$(if($ok){'ok'}else{'OVER BUDGET'}))
    if(-not $ok){ $failed = $true }
  }
}
if($missing){ exit 2 }
if($failed){ Write-Host 'PERF BUDGET FAILED'; exit 1 }
Write-Host 'PERF BUDGET OK'; exit 0
