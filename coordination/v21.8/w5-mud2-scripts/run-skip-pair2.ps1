param([string]$Exe='C:\dh218\w5\mud2-trace.exe',[string]$Prefix='w5m2',[string]$SkipClick='2:125:60:80',[int]$Frames=2800,[string[]]$MoreExtra=@(),[int[]]$Caps=@(1300,2100,2470,2600,2790),[switch]$OnlySkip,[switch]$OnlyPlayed)
$ErrorActionPreference='Continue'
$ext=@('--player-to','_prim_ExitLevelZone_42@150','--capture-at','170:{CASE}\S1.sync','--player-to','-12136.77,11990,258@700','--capture-at','705:{CASE}\S2.sync','--player-to','-15437,12000,-112@1200','--capture-at','1210:{CASE}\S3.sync','--actor-state-dump','--target-hud-trace')+$MoreExtra
$head='3:1135:226:80,6:1130:259:80,W|{CASE}\S1.sync,1.0:-69:0:100,W|{CASE}\S2.sync,0.5:-32:0:100,W|{CASE}\S3.sync,'
$tail='2:-32:0:80,4:-32:0:80,6:-32:0:80,8:-32:0:80,10:-32:0:80,12:-32:0:80,14:-32:0:80,16:-32:0:80,18:-32:0:80,20:-32:0:80,22:-32:0:80,24:-32:0:80,26:-32:0:80,28:-32:0:80,30:-32:0:80,32:-32:0:80,34:-32:0:80,36:-32:0:80,38:-32:0:80,40:-32:0:80,42:-32:0:80,44:-32:0:80'
$played=$head+$tail
$skipTail=$SkipClick+',4:-32:0:80,6:-32:0:80,8:-32:0:80,10:-32:0:80,12:-32:0:80,14:-32:0:80,16:-32:0:80,18:-32:0:80,20:-32:0:80,22:-32:0:80,24:-32:0:80,26:-32:0:80,28:-32:0:80,30:-32:0:80,32:-32:0:80,34:-32:0:80,36:-32:0:80,38:-32:0:80,40:-32:0:80,42:-32:0:80,44:-32:0:80'
$skip=$head+$skipTail
if(-not $OnlySkip){ & C:\dh217\tools\run-case.ps1 -Case "$Prefix-played" -Seed afterwitch -Exe $Exe -Assets C:\dh217\assets-overlay -Frames $Frames -Timeout 400 -Clicks $played -Extra $ext -Captures $Caps }
if(-not $OnlyPlayed){ & C:\dh217\tools\run-case.ps1 -Case "$Prefix-skip" -Seed afterwitch -Exe $Exe -Assets C:\dh217\assets-overlay -Frames $Frames -Timeout 400 -Clicks $skip -Extra $ext -Captures $Caps }
"$Prefix DONE" | Out-File C:\dh218\w5\status.txt -Append
