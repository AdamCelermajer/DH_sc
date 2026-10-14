#!/bin/bash
# w3-pose real run with the --test-apply-status seam: same Rogue lizard-zone setup and key taps as run-vicious.sh.
# SK=32 replaces the hotbar-2 skill taps (-50) with plain attack taps, so a skill cannot be the stun source.
S='V:\coordination\v21.8\act1rogue-run-rogue2.ps1'
PKG='V:\.local-inputs\windows-source-clock-v21.8-staged3'
EXE="$1"; CASE="$2"; CAPS="$3"; SEAM="$4"; FRAMES="${5:-3700}"
mk() { awk -v sk="${SK:-50}" -v tm="${TMAX:-56}" 'BEGIN{for(t=1.0;t<=tm;t+=1.5){ if(t==13.0)printf ",%.1f:-9:0:80",t; else if(t==14.5||t==20.5||t==26.5||t==32.5||t==38.5)printf ",%.1f:-%s:0:80",t,sk; else if(t==19)printf ",%.1f:-9:0:80",t; else printf ",%.1f:-32:0:80",t}}'; }
taps=$(mk); if [ -n "$NOTAPS" ]; then taps=""; fi; taps="$taps$XCL"
CL="W|{CASE}\S1.sync,1.0:-13:0:80,3.8:-13:0:80,W|{CASE}\S2.sync$taps"
powershell -NoProfile -ExecutionPolicy Bypass -File "$S" -Case "$CASE" -Exe "$EXE" -Pkg "$PKG" -Seed rogue-vicious -Frames $FRAMES -Timeout 700 -Captures "$CAPS" -Extra "--player-to;-2900,-100,255@700;--capture-at;2100:{CASE}\S2.sync;--test-apply-status;$SEAM" -Clicks "$CL"
