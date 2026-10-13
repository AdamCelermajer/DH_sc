S='C:\Users\adamc\Desktop\workspace\DH_sc\.claude\worktrees\act1rogue\coordination\v21.8\act1rogue-run-rogue2.ps1'
PKG='C:\dh218\act1rogue\pkg'
case_name=$1; seed=$2; hp="$3"
mkB() { awk 'BEGIN{for(u=30;u<=600;u+=6){t=u/10; k=0; if(u%120==0)k=53; else if((u-84)%120==0)k=50; else if(u==30||u==180||u==330)k=9; else k=32; printf ",%.1f:-%d:0:80",t,k}}'; }
mkA() { awk 'BEGIN{for(u=10;u<=280;u+=15){t=u/10; if(u==130||u==190||u==250)printf ",%.1f:-9:0:80",t; else printf ",%.1f:-32:0:80",t}}'; }
mkC() { awk 'BEGIN{for(u=10;u<=240;u+=12){t=u/10; printf ",%.1f:-32:0:80",t}}'; }
A=$(mkA); B=$(mkB); C=$(mkC)
CL="W|{CASE}\S1.sync,1.0:-13:0:80,3.8:-13:0:80,W|{CASE}\S2.sync$A,W|{CASE}\S3.sync$C,W|{CASE}\S3b.sync,3.0:-69:0:100,W|{CASE}\S4.sync$B,W|{CASE}\S5.sync,2.0:-69:0:100,4.0:-69:0:100"
X="--player-to;-2900,-100,255@700;--capture-at;2100:{CASE}\S2.sync"
X="$X;--player-to;-11800,12200,255@4410;--capture-at;4400:{CASE}\S3.sync;--capture-at;6300:{CASE}\S3b.sync;--player-to;_prim_ExitLevelZone@6320"
X="$X;--player-to;6346.94,-10602.9,60.779@6850;--capture-at;6950:{CASE}\S4.sync;--follow-actor;Witch@7300-10700:150!"
X="$X;--player-to;_prim_ExitLevelZone_42@10800;--capture-at;10900:{CASE}\S5.sync$hp"
powershell -NoProfile -ExecutionPolicy Bypass -File "$S" -Case $case_name -Pkg "$PKG" -Seed $seed -Frames 12300 -Timeout 1800 -Captures "4600,5300,6000,6300,6700,7000,7600,8000,8400,8800,9200,9600,10000,10400,10800,11200,11800" -Extra "$X" -Clicks "$CL" > logs/$case_name.out 2>&1
echo done > logs/$case_name.done
