# Android 17 source quest data development build

This source reconstruction milestone is not a complete playable game. It has
owned ARM64/x86_64 libraries, targets SDK 37 and aligns ELF/APK libraries at
16 KiB. Original cache assets are supplied separately for diagnostic imports.

APK: `DH2-source-quest-data-Android17-debug.apk`
Size: 902,051 bytes. SHA-256: `3de5a72da41717d3be63f9c1a2901007a8cee8858550d5e688655b4d04fcf7c0`.
Package: `local.dh2.sourceviewer`.

## Tested

- Android 17 SDK 37 emulators with 4 KiB/16 KiB pages, both x86_64.
- Actual original quest array/nested readers execute against all 21,817 cache
  bytes. Host/source ARM64 accessors match all 64 rows, 80 conditions, 66 list
  objectives, 128 accept/end stubs, 222 rewards and 896 script slots. All 21,817
  host and 128 ARM64 truncations reject, along with a trailing byte.
- 12,000 strict reader sanitizer mutations and full runtime selftests on strict
  host and both Android emulators. Native host corpus checks all 64 snapshots
  and 34 counted-kill objectives.
- 19 imports per installed APK run. Exact recovered melee formula generates two
  five-point hits per ten-point diagnostic actor. Actual property/template IDs
  and required counts feed all 34 supported counted-kill objectives. Completion
  occurs at the recorded threshold; the next kill requests completion again.
- Replacement quest data and rejected malformed data preserve existing objective
  generations/progress; a new objective uses the new dataset. Existing policy,
  synchronization, invalid-argument, death, retained-property and recovery checks
  pass. Original textured character and 25-track walk preview load afterward in
  the same process. Installed APK hashes match; no source-app fatal in either run.

## Development use

Import `data/pydata/v2quests_pyarray.bin` with Import quest data. Diagnostic Lua
can call DH2GetQuestCount, DH2GetQuestRecord and DH2CreateQuestKillObjective.
The constructor accepts row/objective zero-based indexes, signed current count
and boolean completed. It supports types 0/10 with positive required counts;
clear objectives require world compilation. ConsumeKillEvent accepts the
KillNonplayer event table. Record getters/constructors and explicit caller-driven
consumption are authored development controls. See the source quest-data README
for the checked API and numeric limits.

Original quest compile/level/world counts, conditions, automatic event dispatch,
markers/observers, persistence/rewards, loot generation, killer credit/XP, player
death, full attack/world/AI, progression/saves and complete source gameplay remain
pending. ARM64 is compared in Unicorn; devices execute x86_64 emulator builds.
No Fold7, Android 9, ARM64 hardware or full gameplay equivalence is claimed.

Browser permission denied the release upload. No release was published from this
continuation; the exact upload authorization question remains pending.

Evidence: `questdata-4k-runtime-validation.json`, `questdata-16k-runtime-validation.json`,
`../quest-data/differential-validation.json` and `../lua-runtime/questdata-*-validation.json`.
