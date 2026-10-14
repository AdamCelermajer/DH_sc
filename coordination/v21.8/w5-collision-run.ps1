# w5-collision runner: real Swamp start (staged3 swamp.args, shipped path incl. --original-floor-motion),
# hidden private desktop, REAL Win32 key messages (negative X = virtual key), frame-synced by --capture-at anchor.
# Derived from act1rogue-run-rogue2.ps1. DH_STATE_TRACE=1 gives "Timed buff probe frame=N position=x,y" every 4 frames.
param(
  [Parameter(Mandatory=$true)][string]$Case,
  [string]$Exe='C:\dh219\w5-build\dh-foundation.exe',
  [string]$Clicks="",
  [int]$Frames=900,
  [string]$Captures="",
  [int]$Timeout=300,
  [string]$Extra="",
  [string]$Pkg="C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\windows-source-clock-v21.8-staged3",
  [int]$Anchor=40
)
$ErrorActionPreference='Stop'
$v='C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\v21.4\verify'
$caseRoot="C:\dh219\cases\$Case"
if(Test-Path -LiteralPath $caseRoot){throw 'case exists'}
New-Item -ItemType Directory $caseRoot | Out-Null
Copy-Item -LiteralPath $Exe "$caseRoot\run.exe"
$assets="$Pkg\assets"; $audio="$Pkg\audio-assets"; $ui="$Pkg\ui-assets"
$tpl=Get-Content "$Pkg\swamp.args" | Where-Object { $_ -and -not $_.StartsWith('#') }
$out=New-Object System.Collections.Generic.List[string]
for($i=0;$i -lt $tpl.Count;$i++){
  $l=$tpl[$i].TrimEnd("`r")
  if($i -gt 0){$prev=$tpl[$i-1].TrimEnd("`r")} else {$prev=''}
  if($prev -in '--assets','--menu-assets'){$l=$assets}
  elseif($prev -eq '--audio-assets'){$l=$audio}
  elseif($prev -eq '--audio-table'){$l="$audio\sounds_pyarray.bin"}
  elseif($prev -eq '--save'){$l="$caseRoot\character.save"}
  elseif($prev -eq '--game-save'){$l="$caseRoot\gameplay.save"}
  $out.Add($l)
}
$out.Add('--menu-ui-assets');$out.Add($ui)
$out.Add('--fixed-step');$out.Add('.016')
$out.Add('--frames');$out.Add([string]$Frames);$out.Add('--capture');$out.Add("$caseRoot\final.ppm")
$out.Add('--capture-at');$out.Add("${Anchor}:$caseRoot\S1.sync")
foreach($c in ($Captures.Split(",") | Where-Object {$_})){$out.Add('--capture-at');$out.Add("${c}:$caseRoot\f$c.ppm")}
foreach($x in $Extra.Split(";")){if($x){$out.Add($x.Replace("{CASE}",$caseRoot))}}
$out | Set-Content -LiteralPath "$caseRoot\run.args"
(Get-FileHash $Exe).Hash | Set-Content "$caseRoot\exe-sha256.txt"
$Clicks=$Clicks.Replace("{CASE}",$caseRoot)
$Clicks | Set-Content "$caseRoot\physical-inputs.txt"
$env:DH_STATE_TRACE='1'
& "$v\tools\quiet_click_sync.ps1" -Exe "$caseRoot\run.exe" -ExeArgs @('--startup-config',"$caseRoot\run.args") -Cwd $caseRoot -Log "$caseRoot\run.log" -Clicks $Clicks -TimeoutSec $Timeout | Tee-Object "$caseRoot\injector.txt"
$p=Get-ChildItem $caseRoot -Filter *.ppm | % FullName
if($p){ & "$v\tools\ppm2png.ps1" -Files @($p); $p | % { Remove-Item $_ } }
