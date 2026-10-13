param([Parameter(Mandatory=$true)][string]$Case,[Parameter(Mandatory=$true)][string]$Seed,[string[]]$Extra=@(),[string]$Clicks='',[int]$Frames=1500,[int]$Timeout=400,[string]$Exe='C:\dh218\act1rogue-build\dh-foundation.exe',[int[]]$Captures=@())
# act1rogue: copy of C:\dh217\tools\run-case.ps1 that uses the v21.8 staged package and C:\dh218\seeds.
$ErrorActionPreference='Stop'
$ws='C:\Users\adamc\Desktop\workspace\DH_sc'
$v="$ws\.local-inputs\v21.4\verify"
$pkg="$ws\.local-inputs\windows-source-clock-v21.8-staged"
$caseRoot="C:\dh218\act1rogue\cases\$Case"
if(Test-Path -LiteralPath $caseRoot){throw 'case exists'}
New-Item -ItemType Directory $caseRoot | Out-Null
if(Test-Path "C:\dh218\seeds\$Seed"){Copy-Item "C:\dh218\seeds\$Seed\*" $caseRoot}elseif(Test-Path "C:\dh217\seeds\$Seed"){Copy-Item "C:\dh217\seeds\$Seed\*" $caseRoot}else{throw "no seed $Seed"}
Copy-Item $Exe "$caseRoot\run.exe"
$assets="$pkg\assets";$audio="$pkg\audio-assets";$ui="$pkg\ui-assets"
$tpl=Get-Content "$pkg\startup.args"
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
$out.Add('--skip-boot');$out.Add('--menu-ui-assets');$out.Add($ui)
$out.Add('--frames');$out.Add([string]$Frames);$out.Add('--capture');$out.Add("$caseRoot\final.ppm")
foreach($c in $Captures){$out.Add('--capture-at');$out.Add("${c}:$caseRoot\f$c.ppm")}
foreach($e in $Extra){$out.Add($e.Replace('{CASE}',$caseRoot))}
$out | Set-Content -LiteralPath "$caseRoot\run.args"
(Get-FileHash $Exe).Hash | Set-Content "$caseRoot\exe-sha256.txt"
$Clicks=$Clicks.Replace('{CASE}',$caseRoot)
$Clicks | Set-Content "$caseRoot\physical-inputs.txt"
$env:DH_STATE_TRACE='1'
& "$v\tools\quiet_click_sync.ps1" -Exe "$caseRoot\run.exe" -ExeArgs @('--startup-config',"$caseRoot\run.args") -Cwd $caseRoot -Log "$caseRoot\run.log" -Clicks $Clicks -TimeoutSec $Timeout | Tee-Object "$caseRoot\injector.txt"
$p=Get-ChildItem $caseRoot -Filter *.ppm | % FullName
if($p){ & "$v\tools\ppm2png.ps1" -Files @($p); $p | % { Remove-Item $_ } }

