# Health and mana in the source Android app

The current source APK adds original-matched health/mana methods on owned
diagnostic actors. The app remains a character/scene preview and script runner;
full source gameplay is unfinished. It packages no original game engine or ARM
translator.

- APK: `build/dh2-source-renderer-debug.apk`, 848,803 bytes.
- SHA-256: `991355b1c6dcd9a42e595ec5e0e56c12b2aedcde055efb1821b770d9eb3b89f8`.
- ARM64 and x86_64 source libraries, target SDK 37, minimum SDK 26, checked
  16 KiB ELF/ZIP alignment, verified APK v2/v3 signatures.
- Tested on Android 17 / SDK 37 x86_64 emulators with 4 KiB and 16 KiB pages.
  No Fold7 testing; physical ARM64 runtime is unverified.

## Use your own cache

Import `character_properties_pyarray.bin` with **Import character properties**,
then a Lua source file with **Import script source**. The actor factory and
health/mana methods use that owned dataset. See
[method units, return values and limits](../lua-character/HEALTH-BINDING.md).

Native health setters use whole values. Script regen/mana costs use raw fixed
amounts: 256 represents one unit. `GetHP()` returns three values. `RegenHP(-1)`
requests filling to maximum using original wrap/cap behavior. Validation retains
negative health. Mana uses offline normal policy without exemptions.

The existing exact shared combat formula, offline random, class/item/power
datasets and gear calculations remain available. Health methods do not wire the
calculated attack result to an original Character or combat event loop.

For the character preview import `prince_low_poly_warrior.bdae`,
`prince-warrior.tga` and `prince_walk_dual.bdae` through their respective import
buttons. The preview uses stored absolute keys and an authored camera/shader.

## Evidence and limits

`tests/health_runtime.py` installs this exact APK and test-only hierarchy helper,
uses the Android document picker, and checks:

- Three selected controlled chunks: 40 original-derived actor cases.
- Three selected real chunks: 46 character record cases.
- All 224 final fields per case: 19,264 queries on each page size, plus health
  returns, mana decisions and original-matched mutations.
- Actor retention after property dataset replacement; malformed and oversized
  input rejection; unsafe numeric/percentage error rollback and recovery.
- Character model (335 vertices / 1,092 indices / 18 bones), texture (256×256)
  and dual-walk (25 tracks / 799 ms) imports after script tests in the same
  process. Both ready screenshots were visually inspected.
- Pulled installed APK hashes match the built APK; no in-run fatal error.

Reports: `health-4k-runtime-validation.json` and
`health-16k-runtime-validation.json`. The standalone x86_64 source runner also
checks all 520 controlled and 446 real cases (216,384 final fields) on both
Android page sizes, host and strict host sanitizers. The C module has 4,156
original ARM32 / source ARM64 / host comparisons and 12,000 sanitizer iterations.

The original interpreter/Character constructor is not tested. Full combat result
application, death/events, buffs, AI/navigation, level progression, audio and
saves remain unfinished. Earlier combat, gear and animation records retain
their exact earlier APK identities. The playable Test11 APK still uses the
original ARM32 engine through translation. The previously prepared Test11
release upload remains unpublished after the browser permission denial.
