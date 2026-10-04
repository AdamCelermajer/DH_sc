# Native AI/timers/alias checkpoint status

Historical checkpoint report. The newer tested script-owner checkpoint is
documented in `script-owner-integration-source-status.md`.

The reconstruction goal remains active. This is a native source reconstruction
of a limited Crypt prototype, not the complete original game. No physical
ARM64 phone has been tested.

## Latest verified checkpoint

`../build/checkpoints/dh2-native-ai-timers-alias-b4493ed5.apk`

- APK: 22,721,062 bytes; SHA256
  `b4493ed558fb0956205d000acfee57aef4ba89d1325af288a85579a9dd0a8127`.
- Validation: `ai-timers-alias-checkpoint-validation.json` reports build PASS
  and live PASS, bound to frozen compiler inputs and the installed APK hash.
- Both repository and Android Studio projects build ARM64 and x86_64. The APK
  contains eight ELF64 libraries per ABI, with at least 16 KiB load alignment.
  No original ARM32 engine is bundled.
- The 233 bundled prototype assets include the complete recovered Prince bank:
  116 resources, 158 registration requests and two playback slots. Full-game
  asset coverage remains unfinished.
- Corrected source snapshot: `../build/checkpoints/dh2-native-ai-timers-alias-b4493ed5-source-build-inputs.zip`,
  562 ZIP entries, 7,463,903 bytes; SHA256
  `a916fcf9c2f6987234e1ba71573e3eb5014cabbea6da72ab0e10d29ea1772ce3`.
  It preserves frozen build inputs and exact APK assets. External SDK/NDK/JDK
  and dependency caches are not included.
- The earlier source archives omitted Lua CMake/app Gradle setup. The corrected
  archive restores exact bytes from the saved proof/compiler capture, preserving
  the earlier archives and APK. A cold isolated `assembleDebug --offline` build
  passed for both ABIs; all 233 asset bytes match the checkpoint. See
  `ai-timers-alias-source-rebuild-validation.json`. Rebuilt native library bytes
  differ, so the rebuilt APK is not asserted byte-identical or independently
  gameplay-verified. The original b449 APK remains the runtime checkpoint.

## Engine

The live prototype renders the Crypt world, textures and skinned characters,
plays authored animation with native blending/root motion, and has native
collision bodies. Walk/Run frames actually apply timeline scale 1.3. The new
debug inspection broadcast freezes/resumes clocks, body and pose in the same
GL context; Activity recreation is checked separately. Native controller
LookAt/LookTowards now drives the player's attack facing.

Complete original rendering, lighting, camera, blended-pose/GPU parity,
resolution behavior across devices and physical-device performance remain
unfinished. The current UI and camera are development controls.

## Game itself

The live prototype has movement, collisions, bounded attacks, authored-event
damage, health/death and development controls. This APK additionally compiles
the native Lua 5.1 backend, timer bridge, function aliases, complete AI event
switches, AI/controller update and attack kernels, path commands and attack
geometry. Compiled backend availability is distinct from live game ownership.

The actual packaged ARM64 world library agrees with 11,344 original AI event
cases and 13,381 ordered service requests, including timer/reentry cases.
Deep virtual/FSM services remain explicit fixtures. The sanitized host timer
composition loads the actual common script and routes seven native timer
expiries through event35, including six actual VM calls and an inactive gate.
That host fixture supplies caller-owned AIS identities; it does not establish
live LuaManager/selected AIS construction.

Full AIS/LuaManager construction, registration and publication are being
recovered. Native target search has a newer worker handoff and is not in this
checkpoint. Connecting these owners/services to live AI and combat remains
the next integration step. Quests, progression, complete equipment/skills,
original UI, audio, saves and all levels/assets remain unfinished.

## Runtime checks

All checks below ran on emulator-5554 with the exact installed b449 APK.

- Movement PASS: actual touch Walk/Run, native body displacement and Idle release.
- Lifecycle PASS: portrait/landscape/portrait and pause with held input canceled.
- Prince bank PASS: both slots, applied speeds, same-context freeze/resume,
  finite attack events and separate Activity recreation.
- Combat PASS: stationary and moving source Attack, authored native damage,
  repeated-input rejection and finite event0x22 closure into Idle. Moving
  attack displacement was 40.2018308 game units.

The first combat attempt failed before attack because separate ADB DOWN,
log polling and UP added transport delay, overshot waypoints and eventually
produced zero movement. Logs establish blocking, not a specific collider.
Its evidence remains in `.local-inputs/ai-timers-alias-live-combat`.
The corrected harness uses a real stationary Android touchscreen swipe for
each bounded hold, then checks fresh native Move/Idle and settled displacement.
Walk is calibrated separately for the final approach. No position is injected
and displacement/attack/damage assertions remain intact. Passing evidence is
in `.local-inputs/ai-timers-alias-live-combat-stationary-touch`.

These are bounded prototype checks, not full-game, original GPU or physical
ARM64 device verification. Earlier checkpoint/status documents are stage history.
