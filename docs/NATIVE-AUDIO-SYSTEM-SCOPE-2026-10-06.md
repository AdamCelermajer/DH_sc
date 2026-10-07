# Full native audio subsystem

Audio is an explicit completion requirement for the ongoing native-source,
single-APK reconstruction. User requested all skills, attacks and other sounds.

## Current verified source and assets

The supplied original cache contains 244 WAV files (228 PCM format1 and16 IMA
ADPCM format0x11),17 VXN streams and2 sound XML files. They remain embedded in
the APK's original cache. Fresh census and original sounds.xml are retained in
`port/android-native/reports/audio-system-v33`.

FrontAudio currently supplies Android title intro/loop, fades, UI effects,
volume and lifecycle/audio-focus handling. Source gameplay components recover
combat sound selection, animation-event requests, actor positions, Vox Play3D
branch order and some music state. Positive gameplay sample/bank/platform
playback remains unfinished. Current demo load phase0 causes the source Play3D
early return; this must not be changed to38 merely to produce sound.

The generated183-record logical sound catalog has only26 exact filename matches.
The real578-record soundpack XML contains122 matching logical labels with
different filenames in some cases. For example its `SlamSkill` label names
`sfx_skill_warrior_headsplitter.wav`, while the old table declares
`sfx_skill_knight_slam.wav`. XML matching alone is not proof of the original
code's routing. Source label/UID/bank/quality selection must be recovered before
deciding that assets are missing or choosing alternate files.

## Complete work item

- Original sample/bank/XML/UID routing and asset availability.
- PCM/IMA ADPCM decoding and VXN streaming/loop boundaries.
- Modern Android output transport behind the original native contracts.
- Concurrent voices, original priority, pitch, volume, loops and spatial rules.
- Attack/skill cast and animation markers; hit/block/critical/injury/death sounds.
- Floor-dependent footsteps, objects/chests/doors, potions, loot/drop/pickup.
- UI/menu effects, cutscene audio, level ambience and music/combat transitions.
- Settings, mute, audio focus, background pause/resume and teardown lifetimes.
- Timing checks against actual animation/gameplay events and audible runtime
  verification. A log that selected a sound is not playback acceptance.

The external session `01a11070-399a-7e62-b099-8e6df81dfc45` is assigned this full
subsystem after finishing its current authored FX resource handoff. This keeps
the user's five parallel worker budget: three main subagents, loader and this
session. It owns new versioned audio files, tests and immutable handoffs; root
owns integration, APK builds and audible visible-emulator verification.

Do not substitute generic effects, fabricate sound readiness, publish source
phase38 early, or infer whole-game sound parity from menu music alone. Reconcile
actual source soundpack loading and any genuinely absent samples explicitly.
