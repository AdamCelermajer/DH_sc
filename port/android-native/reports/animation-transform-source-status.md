# Source milestone: Prince bank, scripts and item data

The native reconstruction goal remains active. No new APK was packaged or
installed for this source milestone. The saved character-timing checkpoint
retains SHA256 `6d9782be431a8a70dabacf5932da4f40ccff2f59ec18a00d6f1f9194b6839cef`.

The dynamic sampler now handles the Prince bank's10 position-axis and7 angle
tracks. Source type9 is a quaternion angle interpreter with an authored XYZ
axis default, not a scale axis. Raw resource handlers remain separate from the
first-seen union handler. Original/optimized ARM64 and sanitized host replay
pass9,376 samples; prior static11,656 and dynamic117,030 sample regressions pass.
See `../../engine-animation/reports/component-transforms-arm64.json`,
`../../engine-animation/reports/component-transforms-host.json`, and
`../../engine-animation/reference/component-transforms/NOTES.md`.

The production-library full Prince bank test passes116 exact resources,
158 original registration occurrences,17 original game-ID mappings and26,228
samples after borrowed Players are destroyed. 83 union targets include2 null
scene bindings that are retained. Source clip1138 has zero tracks and original
INT_MAX/INT_MIN bounds; it remains registered. This proves integration/lifetime
safety, not full-bank original pose or GPU parity. See
`../../engine-animation/reports/prince-bank-integration-host.json`.

The main native libraries now include registration and full-vector component
setters, the selected AIS update/pause-expiry path, its collision-counter
producer and the1,322-row ItemTable reader. Original-gold shared-library host
audits pass: component1,872 cases; script update600+3 expiry; collision4,096/
6,839 callbacks; item1,578 records plus1,326 lookups and1,322 ranged-query bridges.
Lua/common binding, inventory ownership and gameplay service connections remain
unfinished. Corresponding source-bound reports are in engine-animation,
level-world and game-data reports directories.

Actual Android CMake `dh2_level_world` builds pass for ARM64 and x86_64. Ten
target/dependency ELFs and both compiler command files are preserved under
`.local-inputs/animation-transform-library-capture` at the repository root.
`libraries.json` records ELF64 architecture,16KiB LOAD alignment, exact hashes
and a current source-tree snapshot. It is BUILD_OUTPUTS_CAPTURED, not APK or
device validation. The renderer remains on its earlier single-slot Playback.

Next integration must preserve158 library positions, map source game IDs through
registration lookup, initialize library0 from template1111, keep1138 registered,
and wire the source two-slot actor/scene producers. Actual script/common services,
equipment, NPC gameplay, UI/audio/saves and physical ARM64 testing remain required
for the final playable single-APK goal.
