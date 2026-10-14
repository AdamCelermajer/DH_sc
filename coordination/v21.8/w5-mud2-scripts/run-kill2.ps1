param([string]$Seed='bossroom4',[string]$Case,[string]$Exe='C:\dh218\w5\mud2-dump.exe',[int]$KillFrame=200,[int]$Frames=2600,[string]$CapList='',[string[]]$MoreExtra=@())
$clicks='3:1135:226:80,6:1130:259:80,W|C:\dh217\cases\'+$Case+'\S1.sync,0.3:-87:0:900'
$extra=@('--capture-at','150:{CASE}\S1.sync','--kill-test',("_prim_Boss_SwampKing_01@{0}" -f $KillFrame),'--actor-state-dump','--target-hud-trace','--test-player-heal','1-4000:15')+$MoreExtra
if($CapList){$extra+=@('--capture-frames',$CapList)}
& C:\dh217\tools\run-case.ps1 -Case $Case -Seed $Seed -Exe $Exe -Assets C:\dh217\assets-overlay -Frames $Frames -Timeout 900 -Clicks $clicks -Extra $extra
"$Case KILL2DONE" | Out-File C:\dh218\w5\status.txt -Append
