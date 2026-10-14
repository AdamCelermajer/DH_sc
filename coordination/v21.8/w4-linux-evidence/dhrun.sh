#!/bin/bash
# usage: dhrun.sh CASE [--bin PATH] [--args-file F] -- extra args...   (runs in ~/run/CASE, DISPLAY=:0)
CASE=$1; shift
BIN=${BIN:-$HOME/w4-build/dh-foundation}
PKG=$HOME/pkg/dungeon-hunter-2-linux
ARGS=${ARGS:-$PKG/startup.args}
D=$HOME/run/$CASE
rm -rf $D; mkdir -p $D; cd $D
ln -s $PKG/assets assets; ln -s $PKG/audio-assets audio-assets; ln -s $PKG/ui-assets ui-assets
[ -n "$SEED" ] && cp -r $SEED/. .
cp $ARGS run.args
for a in "$@"; do echo "$a" >> run.args; done
cp $BIN ./dh-foundation
export DISPLAY=${DISPLAY:-:0}
export SDL_AUDIODRIVER=${SDL_AUDIODRIVER:-pulseaudio}
export PULSE_SERVER=${PULSE_SERVER:-unix:/mnt/wslg/PulseServer}

if [ -n "$STRACE" ]; then exec strace -f -qq -e trace=file -o trace.txt ./dh-foundation --startup-config run.args; fi
exec ./dh-foundation --startup-config run.args
