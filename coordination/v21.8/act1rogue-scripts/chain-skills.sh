S='C:\Users\adamc\Desktop\workspace\DH_sc\.claude\worktrees\act1rogue\coordination\v21.8\act1rogue-run-rogue2.ps1'
PKG='C:\dh218\act1rogue\pkg'
mk() { awk 'BEGIN{for(t=1.0;t<=56;t+=1.5){ if(t==13.0)printf ",%.1f:-9:0:80",t; else if(t==14.5||t==20.5||t==26.5||t==32.5||t==38.5)printf ",%.1f:-50:0:80",t; else if(t==19)printf ",%.1f:-9:0:80",t; else printf ",%.1f:-32:0:80",t}}'; }
taps=$(mk)
for sk in "$@"; do
CL="W|{CASE}\S1.sync,1.0:-13:0:80,3.8:-13:0:80,W|{CASE}\S2.sync$taps"
powershell -NoProfile -ExecutionPolicy Bypass -File "$S" -Case t-$sk -Pkg "$PKG" -Seed rogue-$sk -Frames 3900 -Timeout 700 -Captures "3345,3355,3366,3380,3420" -Extra "--player-to;-2900,-100,255@700;--capture-at;2100:{CASE}\S2.sync" -Clicks "$CL" > logs/t-$sk.out 2>&1
done
echo done > logs/chain-skills.done
