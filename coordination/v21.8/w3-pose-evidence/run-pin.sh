#!/bin/bash
# w3-pose real run: Archer PinShot (stun on the lizard) with the w2-archer-PinShot seed; dense frames before/during/after the stun.
S='V:\coordination\v21.8\act1rogue-run-rogue2.ps1'
PKG='V:\.local-inputs\windows-source-clock-v21.8-staged3'
EXE="$1"; CASE="$2"; CAPS="$3"; FRAMES="${4:-3800}"
A=$(awk 'BEGIN{for(t=1.0;t<=11.6;t+=1.5)printf ",%.1f:-32:0:80",t; printf ",13.0:-9:0:80"; printf ",14.5:-50:0:80,17.5:-32:0:80,20.0:-32:0:80,22.0:-50:0:80,25.0:-32:0:80"}')
CL="W|{CASE}\S1.sync,1.0:-13:0:80,3.8:-13:0:80,W|{CASE}\S2.sync$A"
powershell -NoProfile -ExecutionPolicy Bypass -File "$S" -Case "$CASE" -Exe "$EXE" -Pkg "$PKG" -Seed 'C:\dh218\seeds\w2-archer-PinShot' -Frames $FRAMES -Timeout 900 -Captures "$CAPS" -Extra "--player-to;-2900,-100,255@700;--capture-at;2100:{CASE}\S2.sync" -Clicks "$CL"
