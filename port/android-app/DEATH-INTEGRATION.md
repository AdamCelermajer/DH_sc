# Android 17 source death development build

This source reconstruction milestone is not a complete playable game. The APK
contains owned ARM64/x86_64 libraries, targets SDK 37 and aligns ELF/APK libraries
at 16 KiB. Original cache assets are supplied separately for diagnostic imports.

APK: `DH2-source-death-and-character-Android17-debug.apk`
Size: 869,283 bytes. SHA-256: `7ab0f1e1b8e61405c4221f9e33d4b46c2efc150538306c2e8b643abb3be32599`.
Package: `local.dh2.sourceviewer`.

## Tested

- Android 17 SDK 37 emulators with 4 KiB and 16 KiB pages, both x86_64.
- Original non-player/null-killer Kill versus host/source ARM64: 2,510 comparisons,
  zero mismatches; all four property sheets, dead flag, loot request/ID and ordered
  quest request payloads. Actual original HP, level, loot forwarding and async
  event forwarding execute; external owners are explicit fixtures.
- 12,000 strict death sanitizer iterations. Source Lua runtime selftests pass on
  strict host and both Android emulators.
- 14 imports per installed APK run: recovered melee/health sequence, explicit
  owned dead state, repeated-kill no-op, real v2QuestObjectiveType IDs 0/1/10/11,
  property/template match identifiers, two-event and suppressed/forced paths,
  invalid-input rollback and retained state after data replacement/rejection.
- Textured 335-vertex, 18-bone character and 25-track walk preview load afterward.
  Installed APK hash matches; the same process survives and has no fatal in-run.

## Development use

Install with the Android development toolkit. The existing data/script and asset
import controls expose diagnostic actors and animation preview. SetDeathContext
owns dead/network/suppression flags, unsigned target_id and signed-short
property_id/template_id. KillNonplayer returns ordered requests, with match_id
identifying a property record for kinds 0/1 or a template for kinds 2/3. See
DEATH-BINDING.md. Caller-supplied policies expose unresolved engine ownership.

Actual loot generation, quest consumers, resolved killer/threat credit and XP,
player death, full attack dispatch, animation/FSM/world/AI, progression and saves
remain pending. ARM64 is compared in Unicorn; device tests execute x86_64.
No Fold7, Android 9, ARM64 hardware or full gameplay equivalence is claimed.

The browser denied release-upload permission. No release was published from this
continuation; the exact upload authorization question remains pending.

Evidence: `death-4k-runtime-validation.json`, `death-16k-runtime-validation.json`,
`../character-death/differential-validation.json` and `../lua-runtime/death-*-validation.json`.
