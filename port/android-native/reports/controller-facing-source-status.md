# Controller-facing candidate and native game source work

This document records an earlier stage. The latest verified checkpoint is
`dh2-native-ai-timers-alias-b4493ed5.apk`; current engine/game scope and all four
passing emulator suites are recorded in `ai-timers-alias-source-status.md`.

The native reconstruction goal remains active. The last fully verified saved
checkpoint is `../build/checkpoints/dh2-native-prince-bank-4f5b7d11.apk`.
Its four emulator suites passed, as recorded by
`prince-bank-checkpoint-validation.json`. No physical ARM64 phone has been tested.

The newer saved candidate is
`../build/checkpoints/dh2-native-controller-facing-d17eec5e.apk`, 20,793,313 bytes,
SHA256 `d17eec5ecd1cf9fc2d334767b91254b2a29b8cbcceba0e69a99087b68259b656`.
Both repository and Android Studio builds succeeded for ARM64 and x86_64.
`.local-inputs/controller-facing-build-capture.zip` preserves the 253 actual
compiler inputs and both built APKs. The candidate is not yet a verified
replacement for the Prince-bank checkpoint.

## Engine and live prototype

The verified Prince-bank build added all 116 Prince animation resources and 158
original registration requests, native blending through two playback slots,
authored event callbacks, and preserved body/pose/clocks during inspection.
The bundled prototype assets reside inside one APK; full-game asset coverage
is still unfinished. There is no original ARM32 engine in these native APKs.

The newer candidate connects the recovered controller LookAt gates and complete
GameObject LookAt(Point)/LookTowards path to the Prince's live facing. It also
compiles native PathTo and outer-AI update modules; those complete game service
paths are not yet connected to live gameplay. Target positioning currently
uses the prototype object's world position, with original target-node/cache
ownership explicitly unfinished.

The candidate's real-touch movement test passed. Its combat test stopped on a
no-displacement waypoint before reaching the attack assertions. The bank test
rejected a locomotion selection observation: the immediate selection log named
1040 at speed 1, while a later moving frame used 1126. That timing discrepancy
requires source and runtime diagnosis; the assertion has not been removed.
The rotation test initially rejected radius 113.699577 after translation,
versus spawn 113.699707. The lifecycle harness now uses the bank harness's
source-derived float32 bounds/rounding validation and retains the exact spawn
check. That harness correction still requires a new runtime replay.
Failed evidence is preserved in the repository's
`.local-inputs/controller-facing-live-{combat,bank,lifecycle}` folders.

## Game source reconstruction

The current game remains a limited Crypt prototype with movement, collisions,
authored-event damage, health/death, and development controls. Native controller,
AI update, CanAttack, PathTo, and point-facing kernels have bounded original
instruction comparisons and sanitized host audits. They do not establish
complete game ownership or original full-game/GPU parity.

The new melee/controller attack kernel passed 2,458 original instruction cases
and 15,800 ordered services, including speculative restoration and callback
reentry. It is newer than the candidate APK and is being integrated into the
host world library. Target search, inventory, range, and other borrowed
services remain explicit dependencies.

Parallel workers continue complete AI event dispatch and attack service
recovery. Lua 5.1/common-script/timer binding source has been handed back for
integration and is not yet packaged in the candidate. The full NPC, quest,
equipment, progression, original UI, audio, save system, all levels/assets,
and modern physical-device testing remain.
No defensible whole-project completion percentage is available.
