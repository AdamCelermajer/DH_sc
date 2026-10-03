# Native Prince bank checkpoint

The native reconstruction goal remains active. The current saved development
APK is `../build/checkpoints/dh2-native-prince-bank-4f5b7d11.apk`,20,726,897 bytes,
SHA256 `4f5b7d11891e575793f0b5e99056fe5d3cf3cf7a1a705f7ee0f5b632ba19bd82`.
`prince-bank-checkpoint-validation.json` records separate build, asset and live
PASS results. The local compiler capture is
`.local-inputs/prince-bank-build-capture.zip` at the repository root,
SHA256 `67b82b49a5be9ddb45c6b1fedcb5c93ec2eca9b3ee0f5d8576674c212149dd0f`.
Its243 source inputs and actual per-ABI Ninja dependencies bind both built APKs.

## Engine

The Prince renderer now uses native BlendedPlayback rather than its earlier
single-slot Playback. It loads116 exact resources, appends158 original requests,
designates template1111 and retains zero-track1138/engine155. The new owned PAB1
metadata format preserves ordered requests and declares port cache identity
tokens; those tokens are not original CCDB pointer addresses. Live dictionary
selection resolves the registered first engine index. Both slots compose the
scene used by GPU skinning, with synchronous events and scene-before-Step order.

Native metadata decode matches all source fields/116 records/158 requests, with
277 lookups/1,918 atomic failures and no sanitizer findings. Genuine host DSOs
pass72 focused original PlayClip cases,17 mapped selections,158 bounds,
constructor1111, aliases,131 frames/102 root-history checks, both-slot authored
callbacks and11 atomic guards. Legacy original-bound captures and scheduler,
selection/control regressions pass. The separate full-bank host test passes
26,228 samples after borrowed resources are destroyed. These are bounded source
and host-library proofs, not full-bank original sampled-pose or packaged-instruction
parity. Four runtime suites pass on the bound API37 x86_64 emulator APK.

All233 bundled assets match source bytes. Both repository and Studio APKs contain
14 ELF64 libraries (seven per ABI),16KiB LOAD/ZIP alignment and no original ARM32
engine. ARM64 compilation passes; physical ARM64 execution remains unverified.

World inspection pauses the entire actor pipeline and preserves pose/body/clock
snapshots. Recreation detaches playback before replacing its bank and starts a
new complete session, reselecting the saved sequence. This development policy
restarts animation; it does not restore interrupted fade/combo timing.

## Game

The authored Crypt prototype supports source character state/timer handling,
Walk/Run with genuine body movement, stationary/moving authored-event combat,
health/death and development lifecycle controls. These are a limited playable
scene, not the complete game. Item loading, script selection/lifecycle/update,
collision producers and animation-event/AI consumers have source modules and
bounded proofs, but their full live ownership/services remain unfinished.

The live playback observer currently connects authored28 to prototype combat
and finite22 to the existing FSM. It does not yet bind the complete six-event
CharAI/AIS router, controller/target/inventory services or Lua common runtime.
The full NPC/quest/progression/equipment systems, original UI, audio, saves and
all required game assets must still be integrated. Original GPU/whole-gameplay
parity and physical modern-phone testing are still required.

## Recorded harness corrections

The first movement run captured a selected Walk marker before its first body
displacement. The corrected check waits for actual measured displacement and
retains the earlier failure evidence. The first bank run passed freeze/combat,
then rejected a recreated radius that differs from spawn by0.00013 because
source float32 translated-bounds subtraction changes rounding. The corrected
check keeps the exact initial radius check and validates later radii against
source bounds/printed precision and a derived float32 rounding envelope.
Both corrected runs pass; deliberately wrong radii are still rejected. Failed
and verified reports remain in distinct `.local-inputs/prince-blended-*` folders.
