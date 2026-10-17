# Android 17 source damage development build

This APK is a source reconstruction milestone, not a complete playable game.
It contains owned ARM64/x86_64 source libraries, targets SDK 37 and has 16 KiB
ELF/APK library alignment. It does not contain the original engine or translator.
Original cache assets remain separately supplied for the diagnostic imports.

APK: `DH2-source-damage-and-character-Android17-debug.apk`

SHA-256: `1692549ee46c4ca241dd1c31ce1334a3d7a8bc20865816bbb22d146e66ed63d9`

Size: 865,187 bytes. Package: `local.dh2.sourceviewer`.

## Tested

- Android 17 SDK 37 emulator: 4 KiB pages and 16 KiB pages, both x86_64.
- Actual original non-player HitFor versus owned host/ARM64: 2,835 comparisons,
  zero mismatches; four sheets, death requests/reason and damage stack local.
- 12,000 damage sanitizer iterations; runtime selftests on strict host and both
  Android emulators, including health/mana and legacy actor foundations.
- 11 imports per installed APK run: exact recovered melee formula feeds two
  five-point hits into a ten-point actor, lethal request, explicit dead-target
  no-op, local/online/debug policy, retained death reason, invalid argument
  rollback and retained dataset after real property replacement/rejection.
- Original character model, texture and 25-track walk preview load afterward
  in the same process. Installed APK hash matches; no source-app fatal in-run.

These are diagnostic actor integration checks. The native ARM64 damage library
is compared in Unicorn; device execution is x86_64 emulator execution. No Fold7,
Android 9, ARM64 hardware, original full Lua VM or full gameplay equivalence is
claimed. Previous health/combat/gear corpus reports retain their old identities.

## Development use

Install the APK with the Android development toolkit. The application exposes
the existing asset/script/data import controls and animation preview. A script
actor's authored `ApplyNonplayerHit(rawDamage, policy, deathReason)` method
returns processed, death-requested, whole damage, death reason. Its required
policy booleans expose unimplemented engine ownership; see DAMAGE-BINDING.md.

The full original Character/death state, player warning/audio, attacker
achievements, attack result flags, buffs, leech/DoT/events, world/AI,
progression and saves must still be connected before the user's complete game
and complete reconstructed source goal is satisfied.

The release upload remains blocked by the browser's denied permission check.
No release has been published from this continuation.

Evidence: `damage-4k-runtime-validation.json`, `damage-16k-runtime-validation.json`,
`../character-damage/differential-validation.json` and
`../lua-runtime/damage-*-validation.json`.
