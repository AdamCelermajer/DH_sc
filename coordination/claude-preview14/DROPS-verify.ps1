# DROPS-verify.ps1 (P14 DROPS verifier). Runs seeded quiet batches of the swamp lizard kill
# and checks the world-item log lines. Usage:
#   powershell -NoProfile -File DROPS-verify.ps1 -Exe <abs dh-foundation.exe> -Assets <abs assets dir> -Out <abs empty scratch dir> -Runner <abs quiet_run.ps1>
# Template args: .local-inputs/claude-preview13/rng-check/R/run.args (kill at frame 148 with seed 1234).
# Prints one PASS/FAIL line per expectation. Captures land in <Out>/<job>/cap.ppm (convert with ppm2png).
param([Parameter(Mandatory=$true)][string]$Exe,[Parameter(Mandatory=$true)][string]$Assets,
      [Parameter(Mandatory=$true)][string]$Out,[Parameter(Mandatory=$true)][string]$Runner,
      [string]$Template="C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview13/rng-check/R/run.args")
$ErrorActionPreference='Stop'
New-Item -ItemType Directory -Force $Out | Out-Null
# name, seed, frames, extra args (pairs)
$jobSpecs=@(
  @{n='drop';      seed='1234'; frames=160; extra=@()},
  @{n='gold-potion';seed='16';  frames=220; extra=@('--pickup-frame','190','--pickup-frame','200')},
  @{n='staff';     seed='14';   frames=200; extra=@('--pickup-frame','190')},
  @{n='reload';    seed='16';   frames=330; extra=@('--reload-frame','300')},
  @{n='inventory'; seed='16';   frames=260; extra=@('--pickup-frame','190','--pickup-frame','200','--equipment-page-frame','230')}
)
$tpl=Get-Content $Template
$jobs=@()
foreach($s in $jobSpecs){
  $dir="$Out/$($s.n)"; New-Item -ItemType Directory -Force $dir | Out-Null
  $args=@()
  for($i=0;$i -lt $tpl.Count;$i++){
    $l=$tpl[$i]
    if($l -eq '--assets'){$args+=$l;$args+=$Assets;$i++;continue}
    if($l -eq '--combat-seed'){$args+=$l;$args+=$s.seed;$i++;continue}
    if($l -eq '--frames' -or $l -eq '--capture'){$i++;continue}
    $args+=$l
  }
  $args+=@('--frames',"$($s.frames)",'--capture',"$dir/cap.ppm")+$s.extra
  Set-Content -Path "$dir/run.args" -Value $args -Encoding ascii
  $jobs+=@{name="drops-verify-$($s.n)";exe=$Exe;args=@('--startup-config','run.args');cwd=$dir;log="$dir/run.log";timeoutSec=300}
}
$jobsFile="$Out/jobs.json"
$jobs | ConvertTo-Json -Depth 4 | Set-Content $jobsFile -Encoding ascii
& powershell -NoProfile -File $Runner -JobsFile $jobsFile -Parallel 3 -Summary "$Out/summary.json" | Out-Null

function Check([string]$name,[string]$job,[string]$pattern,[switch]$Absent){
  $log="$Out/$job/run.log"
  $hit=(Select-String -Path $log -Pattern $pattern -SimpleMatch:$false -ErrorAction SilentlyContinue | Measure-Object).Count
  $ok= if($Absent){$hit -eq 0}else{$hit -gt 0}
  "{0} {1}: {2} (pattern: {3})" -f $(if($ok){'PASS'}else{'FAIL'}),$name,$job,$pattern
}
Check 'drop target at kill' 'drop' 'World item target frame=148 item=1 id=ClothGloves01'
Check 'drop drawn' 'drop' 'World item draws frame=148 count=1 store=1'
Check 'auto item not drawn for skipped visuals' 'drop' 'World item not drawn' -Absent
Check 'gold pickup adds 9 gold' 'gold-potion' 'World item pickup frame=190 item=1 id=GoldStack01 reason=scripted outcome=0 picked=1 gold=0->9'
Check 'potion pickup adds stack' 'gold-potion' 'World item pickup frame=200 item=2 id=Potion0 reason=scripted outcome=0 picked=1 gold=9->9 stacks=4->5 store=0'
Check 'equippable pickup inserts' 'staff' 'World item pickup frame=190 item=1 id=Staff01 reason=scripted outcome=0 picked=1 gold=0->0 stacks=4->5 store=0'
Check 'reload clears ground items (no draws after reload)' 'reload' 'Content unloaded and reloaded at frame=300'
Check 'no draws after reload' 'reload' 'World item draws frame=3[0-9][0-9]' -Absent
Check 'inventory page shows 9 gold and Potions: 1' 'inventory' 'Character menu Equipment selected frame=230'
Check 'no diagnostics' 'drop' 'World item presentation diagnostic' -Absent
"Captures: $Out/<job>/cap.ppm (drop, gold-potion, reload, inventory). Look at them with tools/ppm2png.ps1."
