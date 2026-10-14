#!/bin/bash
# usage: w4drive.sh CASE SCRIPTFILE [game extra args...]
# SCRIPTFILE lines: "sleep N" | "click X Y" | "shot NAME" | "key K" | "keydown K" | "keyup K" | "hold K SEC" | "end"
export DISPLAY=:0
CASE=$1; SCR=$2; shift 2
cp /mnt/c/dh-fedora/dhrun.sh ~/dhrun.sh; chmod +x ~/dhrun.sh
mkdir -p ~/run; pkill dh-foundation; sleep 0.5
(STRACE=$STRACE SEED=$SEED ~/dhrun.sh $CASE "$@" > ~/run/$CASE.out 2>&1 &)
for i in $(seq 1 60); do W=$(xdotool search --onlyvisible --name . 2>/dev/null | tail -1); [ -n "$W" ] && break; sleep 0.5; done
echo "win=$W"; xdotool windowfocus $W; xdotool windowraise $W; sleep 0.5
mkdir -p ~/run/$CASE-shots
while read -r cmd a b; do
  case "$cmd" in
    sleep) sleep $a;;
    click) xdotool mousemove --window $W $a $b; sleep 0.15; xdotool mousedown 1; sleep 0.15; xdotool mouseup 1;;
    shot) timeout 8 import -window $W ~/run/$CASE-shots/$a.png; echo "shot $a";;
    clickv) for try in 1 2 3 4 5 6 7 8 9 10; do timeout 8 import -window $W /tmp/cv-before.png; xdotool mousemove --window $W $a $b; sleep 0.15; xdotool mousedown 1; sleep 0.15; xdotool mouseup 1; sleep 2.5; timeout 8 import -window $W /tmp/cv-after.png; D=$(compare -metric AE -fuzz 12% /tmp/cv-before.png /tmp/cv-after.png null: 2>&1 | cut -d" " -f1); echo "clickv $a $b try=$try diffpx=$D"; [ "${D:-0}" -gt 20000 ] && break; done;;
    key) xdotool key --window $W $a;;
    keydown) xdotool keydown $a;;
    keyup) xdotool keyup $a;;
    hold) xdotool keydown $a; sleep $b; xdotool keyup $a;;
    end) break;;
  esac
done < $SCR
pkill dh-foundation
echo done
