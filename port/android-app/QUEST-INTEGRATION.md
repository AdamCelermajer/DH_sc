# Android 17 source quest progress development build

This source reconstruction milestone is not a complete playable game. It has
owned ARM64/x86_64 libraries, targets SDK 37 and aligns ELF/APK libraries at
16 KiB. Original cache assets are supplied separately for diagnostic imports.

APK: `DH2-source-quest-and-character-Android17-debug.apk`
Size: 873,379 bytes. SHA-256: `3005249a0ef0ece743c66ed6d67a01687e0b2590ea27c1ddd4997c6366c782cb`.
Package: `local.dh2.sourceviewer`.

## Tested

- Android 17 SDK 37 emulators with 4 KiB/16 KiB pages, both x86_64.
- Four original kill/clear event handlers versus host/source ARM64: 14,396
  comparisons with zero mismatches; signed count wrapping, match IDs, local
  outbound quantity/flag, synchronized maximum updates, completed byte and
  4,876 completion callbacks. Original SetIsCompleted executes with persistence
  ID -1; virtual completion callback is an observed external fixture.
- 12,000 strict quest sanitizer iterations and full runtime selftests on strict
  host and both Android emulators.
- 14 imports per installed APK run. Exact recovered melee formula generates two
  five-point hits per ten-point diagnostic actor. Three deaths feed four owned
  kill/clear counters, complete at two kills and request completion again on the
  third. Repeated dead kills emit no events. Stale synchronized counts do not
  change progress; larger ones update it. Wrong kind/match and invalid arguments
  preserve progress. All four objectives survive property replacement/rejection
  and garbage collection; further events update the retained objective.
- Original textured character and 25-track walk preview load afterward in the
  same process. Installed APK hashes match; no source-app fatal in either run.

## Development use

Install with the Android development toolkit and use the existing diagnostic
script/data import controls. DH2CreateKillObjective creates an owned objective
with kind, match_id, current, required and completed. ConsumeKillEvent accepts
the KillNonplayer event table. GetProgress returns its current snapshot. Source
kind dispatch and explicit caller-driven consumption are authored controls.
See ../quest-kill/README.md for the checked API and numeric limits.

Original quest data loading/compile, automatic event dispatch, live world counts,
markers/observers, persistence/rewards, loot generation, killer credit/XP, player
death, full attack/world/AI, progression/saves and complete source gameplay remain
pending. ARM64 is compared in Unicorn; devices execute x86_64 emulator builds.
No Fold7, Android 9, ARM64 hardware or full gameplay equivalence is claimed.

Browser permission denied the release upload. No release was published from this
continuation; the exact upload authorization question remains pending.

Evidence: `quest-4k-runtime-validation.json`, `quest-16k-runtime-validation.json`,
`../quest-kill/differential-validation.json` and `../lua-runtime/quest-*-validation.json`.
