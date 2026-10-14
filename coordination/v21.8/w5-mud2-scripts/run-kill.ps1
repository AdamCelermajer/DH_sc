param([string]$Case,[string]$Exe='C:\dh218\w5\mud2-dump.exe',[double]$Shift=0,[int]$ExitFrame=9000,[int]$Frames=5600,[string]$CapList='',[string[]]$MoreExtra=@())
$p=@();for($t=1.2+$Shift;$t -lt 50;$t+=2.5){$p+=("{0}:-53:0:80" -f $t)}
$taps=@();for($t=26+$Shift;$t -le 84;$t+=2){$taps+=("{0}:-32:0:80" -f $t)}
$clicks='3:1135:226:80,6:1130:259:80,W|C:\dh217\cases\'+$Case+'\S1.sync,0.3:-87:0:900,1.1:-32:0:-1,'+($p -join ',')+',22:-32:0:-2,'+($taps -join ',')
$extra=@('--capture-at','150:{CASE}\S1.sync','--player-to',("_prim_ExitLevelZone_001@{0}" -f $ExitFrame),'--actor-state-dump','--target-hud-trace')+$MoreExtra
if($CapList){$extra+=@('--capture-frames',$CapList)}
& C:\dh217\tools\run-case.ps1 -Case $Case -Seed bossroom4 -Exe $Exe -Assets C:\dh217\assets-overlay -Frames $Frames -Timeout 900 -Clicks $clicks -Extra $extra -Captures @(1200)
"$Case KILLDONE" | Out-File C:\dh218\w5\status.txt -Append
