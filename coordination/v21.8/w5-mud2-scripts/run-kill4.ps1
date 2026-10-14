param([string]$Case,[string]$Exe='C:\dh218\w5\mud2-final.exe',[int]$Frames=6200,[string]$CapList='',[string[]]$MoreExtra=@())
$taps=@();for($t=6;$t -le 100;$t+=2){$taps+=("{0}:-32:0:80" -f $t)}
$clicks='3:1135:226:80,6:1130:259:80,W|C:\dh217\cases\'+$Case+'\S1.sync,0.3:-87:0:900,'+($taps -join ',')
$extra=@('--capture-at','150:{CASE}\S1.sync','--kill-test','_prim_Boss_SwampKing_01@200','--quest-debug-kill-property','201:358:1','--actor-state-dump','--target-hud-trace','--test-player-heal','1-4000:15')+$MoreExtra
if($CapList){$extra+=@('--capture-frames',$CapList)}
& C:\dh217\tools\run-case.ps1 -Case $Case -Seed bossroom4 -Exe $Exe -Assets C:\dh217\assets-overlay -Frames $Frames -Timeout 900 -Clicks $clicks -Extra $extra
"$Case KILL4DONE" | Out-File C:\dh218\w5\status.txt -Append
