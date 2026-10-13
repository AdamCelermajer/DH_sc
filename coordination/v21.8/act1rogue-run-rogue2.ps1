# act1rogue run-rogue2.ps1: deterministic Rogue runner (menu started by --menu-actions test aid, --fixed-step .016, sync-file waits)
# derived from the v21.8 act1mage runner. Package: v21.8 staged (production startup.args incl. --campaign-triggers).
# Clicks: "T:X:Y:HOLD" (T seconds after the window exists, negative X = virtual key, HOLD ms; HOLD -1/-2 = down/up).
# "W|<file>" waits for a sync file written by --capture-at N:<file> (frame N).
param(
  [Parameter(Mandatory=$true)][string]$Case,
  [Parameter(Mandatory=$true)][string]$Seed,
  [string]$Exe='C:\dh218\act1rogue-build\dh-foundation.exe',
  [string]$Clicks="",
  [int]$Frames=600,
  [string]$Captures="",
  [int]$Timeout=300,
  [string]$Extra="",
  [string]$GameSave="",
  [string]$Pkg="C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\windows-source-clock-v21.8-staged",
  [int]$Anchor=40,
  [string]$Menu="menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER"
)
$ErrorActionPreference='Stop'
$v='C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\v21.4\verify'
$caseRoot="C:\dh218\act1rogue\cases\$Case"
if(Test-Path -LiteralPath $caseRoot){throw 'case exists'}
New-Item -ItemType Directory $caseRoot | Out-Null
$src=$Seed; if(-not (Test-Path -LiteralPath $src)){$src="C:\dh218\seeds\$Seed"}
# a seed is a directory (every file is copied: profile, per-level world saves, tutorial flags) or a single character.save file
if(Test-Path -LiteralPath $src -PathType Container){Copy-Item -Path "$src\*" -Destination $caseRoot -Recurse}else{Copy-Item -LiteralPath $src "$caseRoot\character.save"}
if($GameSave){Copy-Item -LiteralPath $GameSave "$caseRoot\gameplay.save"}
Copy-Item -LiteralPath $Exe "$caseRoot\run.exe"
$assets="$Pkg\assets"; $audio="$Pkg\audio-assets"; $ui="$Pkg\ui-assets"
$tpl=Get-Content "$Pkg\startup.args"
$out=New-Object System.Collections.Generic.List[string]
for($i=0;$i -lt $tpl.Count;$i++){
  $l=$tpl[$i].TrimEnd("`r")
  if($i -gt 0){$prev=$tpl[$i-1].TrimEnd("`r")} else {$prev=''}
  if($prev -in '--assets','--menu-assets'){$l=$assets}
  elseif($prev -eq '--audio-assets'){$l=$audio}
  elseif($prev -eq '--audio-table'){$l="$audio\sounds_pyarray.bin"}
  elseif($prev -eq '--save'){$l="$caseRoot\character.save"}
  elseif($prev -eq '--game-save'){$l="$caseRoot\gameplay.save"}
  if($prev -eq '--attach' -or $l -eq '--attach'){ continue }
  $out.Add($l)
}
$out.Add('--skip-boot');$out.Add('--menu-ui-assets');$out.Add($ui)
$out.Add('--menu-actions');$out.Add($Menu)
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
