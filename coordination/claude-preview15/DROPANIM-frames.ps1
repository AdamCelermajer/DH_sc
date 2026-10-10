# Usage: mkframes.ps1 -Label before|after -Exe <abs exe> -Frames "149,151,..."
# One quiet job per captured frame: the kill (seed 1234, frame 148) replays deterministically to --frames N.
param([string]$Label,[string]$Exe,[string]$Frames)
$root="C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview15/dropanim"
$tpl=Get-Content "C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview14/drops/runs/r160/run.args"
$assets="C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview14/drops/assets-overlay"
$jobs=@()
foreach($f in $Frames.Split(',')){
  $dir="$root/$Label/f$f"; New-Item -ItemType Directory -Force $dir | Out-Null
  $out=@(); for($i=0;$i -lt $tpl.Count;$i++){ $l=$tpl[$i]
    if($l -eq '--assets'){ $out+=$l; $out+=$assets; $i++; continue }
    if($l -eq '--frames' -or $l -eq '--capture'){ $i++; continue }
    $out+=$l }
  $out+='--frames'; $out+=$f; $out+='--capture'; $out+="$dir/cap.ppm"
  Set-Content -Path "$dir/run.args" -Value $out -Encoding ascii
  $jobs+=[ordered]@{name="dropanim-$Label-f$f";exe=$Exe;args=@("--startup-config","run.args");cwd=$dir;log="$dir/run.log";timeoutSec=300}
}
New-Item -ItemType Directory -Force "$root/$Label" | Out-Null
ConvertTo-Json -InputObject @($jobs) -Depth 4 | Set-Content "$root/$Label/jobs.json" -Encoding ascii
"jobs: $($jobs.Count)"
