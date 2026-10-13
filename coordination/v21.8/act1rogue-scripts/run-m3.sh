S='C:\Users\adamc\Desktop\workspace\DH_sc\.claude\worktrees\act1rogue\coordination\v21.8\act1rogue-run-rogue2.ps1'
PKG='C:\dh218\act1rogue\pkg'
case_name=$1; seed=$2; hp="$3"
mkC() { awk 'BEGIN{for(u=20;u<=240;u+=20){printf ",%.1f:-32:0:80",u/1}}' ; }
taps=""; for t in 2 4 6 8 10 12 14 16 18 20 22 24; do taps="$taps,$t:-32:0:80"; done
fight="0.3:-87:0:900,1.1:-32:0:-1,10:-50:0:80,22:-50:0:80,34:-50:0:80,46:-50:0:80,58:-50:0:80,68:-32:0:-2"; for t in $(seq 70 1.5 100); do fight="$fight,$t:-32:0:80"; done
CL="W|{CASE}\S1.sync,W|{CASE}\S2.sync,0.5:-32:0:80,W|{CASE}\S3.sync$taps,W|{CASE}\S4.sync,$fight,W|{CASE}\S5.sync,2.0:-69:0:100,4.0:-69:0:100,6.0:-69:0:100"
X="--capture-at;170:{CASE}\S2x.sync;--player-to;-12136.77,11990,258@700;--capture-at;705:{CASE}\S2.sync;--player-to;-15437,12000,-112@1200;--capture-at;1210:{CASE}\S3.sync"
X="$X;--capture-at;2950:{CASE}\S4.sync;--follow-actor;SwampKing@2960-6400:300;--player-to;_prim_ExitLevelZone_001@9300;--capture-at;9400:{CASE}\S5.sync$hp"
powershell -NoProfile -ExecutionPolicy Bypass -File "$S" -Case $case_name -Pkg "$PKG" -Seed $seed -Frames 10700 -Timeout 1800 -Captures "2900,3300,4200,5000,5800,6400,6700,7200,7800,8400,8900,9300,9700,10200,10600" -Extra "$X" -Clicks "$CL" > logs/$case_name.out 2>&1
echo done > logs/$case_name.done
