#!/bin/bash
# w3-pose real run: Rogue ViciousStrike on the Act 1 lizard zone (same clicks as es-status es-vicious-1), dense frames around the stun.
S='V:\coordination\v21.8\act1rogue-run-rogue2.ps1'
PKG='V:\.local-inputs\windows-source-clock-v21.8-staged3'
EXE="$1"
CASE="$2"
CAPS="$3"
mk() { awk 'BEGIN{for(t=1.0;t<=56;t+=1.5){ if(t==13.0)printf ",%.1f:-9:0:80",t; else if(t==14.5||t==20.5||t==26.5||t==32.5||t==38.5)printf ",%.1f:-50:0:80",t; else if(t==19)printf ",%.1f:-9:0:80",t; else printf ",%.1f:-32:0:80",t}}'; }
taps=$(mk)
CL="W|{CASE}\S1.sync,1.0:-13:0:80,3.8:-13:0:80,W|{CASE}\S2.sync$taps"
powershell -NoProfile -ExecutionPolicy Bypass -File "$S" -Case "$CASE" -Exe "$EXE" -Pkg "$PKG" -Seed rogue-vicious -Frames 3900 -Timeout 700 -Captures "$CAPS" -Extra "--player-to;-2900,-100,255@700;--capture-at;2100:{CASE}\S2.sync" -Clicks "$CL"
