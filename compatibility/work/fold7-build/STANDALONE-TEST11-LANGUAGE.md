# Test 11 saved-language preference

The Test 10 setup screen checked **Prefer English**, but the tested game menu
still rendered Russian. Its Java `Get_PhoneLanguage()` hook returned English
(`0`), while the original native `SavegameManager::getLanguage()` preferred
the `Language` value in `dh2_settings.savegame`. The owner-provided cache has
`Language=2`. Its `data/text/menu.english` contains English strings, and its
`data/text/menu.german` contains Russian strings. These are facts about this
owner archive; the private cache and original game APK are not committed.

Test 11 applies the checkbox to the installed settings copy immediately before
launching the guest. With the checkbox off it reads or changes nothing. With
it on, it accepts only the observed 16-key settings layout, a `Language` int32
in the expected position, and the 14 trailing tutorial bytes. If the current
value is nonzero, it writes the exact original file once to
`dh2_settings.savegame.before-prefer-english.bak`, verifies the backup, changes
only that four-byte value to zero, and atomically replaces the installed
settings file. The owner ZIP and character/level saves are untouched. An
unknown format or storage failure leaves the settings as they were and does
not prevent game launch; the diagnostic session records the outcome.

The native behavior is visible in
`recovered/native/decompiled/libDungeonHunter2.so/functions-006.pseudo.c`:
`__loadOptions`, `getLanguage`, and `loadSettings` use the saved option, while
`__saveOptions` and `__saveTutorials` write the option records followed by 14
bytes. The earlier phone-language hook is in `GameTrace.java`. The specific
owner settings fixture is 294 bytes, SHA-256
`3cb97b47cc9853d65dc596ccc0a8b715b82fe4794812435673a921b215f07c4a`;
only bytes 207–210 differ after applying English (`02 00 00 00` to
`00 00 00 00`). The focused host test checks that exact fixture when the
owner ZIP is locally available, plus disabled, absent, malformed, backup,
idempotency, and byte-preservation cases:

```text
python compatibility/work/fold7-build/tests/test_language_preference.py
```

The complete host Java source set (28 files) compiled cleanly with pinned
JDK 17 against Android 17/API 37.0 `android.jar` and
`hiddenapibypass.jar`, producing 42 classes. D8 converted that host JAR plus
the hidden-API dependency to one `classes.dex` with minimum API 29. This is
source/build validation; the Test 11 APK and its gameplay still require
separate verification.

## Build identity

Test 11 uses the same verified unsigned Test 10 guest APK and owner cache ZIP
as Test 10. Their SHA-256 values are respectively
`57cefd15cba47116a98fa96e406ba8d8a4ef90fb0e82185802a8f09210ba2b7e`
and `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
In the restored private compatibility build tree, provide these as
`DH2_TEST10_GUEST_APK` and `DH2_CACHE_ZIP`, along with the JDK 17 and Android
SDK inputs described in `STANDALONE-TEST7.md`; run the reviewed `build_apk.py`.
This revision builds version code **14**, version name
`1.0-test11-language`, and defaults to the private output filename
`Dungeon-Hunter-2-Android17-test11-language.apk`. Set `DH2_OUTPUT_APK` for a
different private destination. The output APK hash and Android 17 test results
must be recorded after packaging and testing.

The already published signed Test 10 APK has SHA-256
`02ba96298aa2639e1bd3c3a34f0b54447756d85aed9e8ed85b3d53bf726964aa`
and version code 13. That hash identifies the prior source revision. Its
source is pinned at Git commit `9723cf0` and its private-input build recipe is
`STANDALONE-TEST10-VIEWPORT.md`; check out that commit to rebuild Test 10.
The local development signing key is private, so byte-identical signed output
also depends on retaining that key and the same packaging inputs. The Test 11
source change does not retroactively change the published Test 10 artifact.

This is a compatibility wrapper around the original ARM32 engine, not a
complete source-built game. The ownership and license limits in `RIGHTS.md`
still apply. No Fold7 or other physical device was used for this finding.
