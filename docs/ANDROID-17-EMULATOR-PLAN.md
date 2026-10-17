# Android 17 emulator compatibility test

The target for current Android compatibility testing is **Android 17 (API 37)** on an official Android Emulator x86_64 system image. Android 9 and 11 results in the [emulator report](EMULATOR-TEST-2026-10-02.md) are diagnostic baselines, not a substitute for this test. No phone test is required for this plan.

## Why the two APK routes differ

| APK | Manifest / libraries inspected locally | What the Android 17 AVD can establish |
| --- | --- | --- |
| Supplied original game | `minSdkVersion=8`, no explicit target SDK, only ARM32 `armeabi` / `armeabi-v7a` libraries | The install restriction can be probed with `adb install --bypass-low-target-sdk-block`; the standard API 37 AVD cannot execute these 32-bit libraries. |
| Direct Test 7 guest | `minSdkVersion=21`, `targetSdkVersion=24`, only ARM32 `armeabi-v7a` libraries | Target SDK meets Android 15's minimum install level, but the standard API 37 AVD still lacks a 32-bit app ABI. Run the install attempt and preserve its exact result. |
| Published Test 5 wrapper | `minSdkVersion=29`, `targetSdkVersion=35`, `arm64-v8a` host libraries | Test launcher, cache import, plugin activation and engine startup separately. An Android 11 x86_64 AVD displayed the launcher but failed to load an AArch64 proxy in its x86_64 plugin process. Repeat this discriminator on API 37. |

Google states that **Android Emulator images from Android 12 (API 31) onward are 64-bit only**. The game's ARM32 guest cannot be used as a direct gameplay test in a standard Android 17 AVD. The host wrapper is the appropriate existing APK to try on API 37, but its translator/proxy ABI must be checked at runtime; launcher success does not imply translated game success. The signed direct guest remains useful on the older Android 9 diagnostic AVD, where the supplied ARM32 code actually executes.

## Run and record

1. Install the current stable Android 17/API 37 x86_64 Google APIs phone image with the SDK Manager. Use an image revision at least 5 if Google services are needed; Google's emulator troubleshooting page identifies problems in older revisions. The official emulator requires at least **4 GB AVD RAM** for API 37 phones. Keep this AVD separate from the older diagnostic AVDs.
2. Boot the AVD and save `ro.build.version.release`, `ro.build.version.sdk`, `ro.build.fingerprint`, `ro.product.cpu.abilist`, `ro.product.cpu.abilist32`, `ro.product.cpu.abilist64`, `ro.dalvik.vm.native.bridge`, `getconf PAGESIZE`, emulator version, and system-image revision. Verify `sys.boot_completed=1`.
3. Attempt to install the signed direct Test 7 guest and preserve the package-manager result. Do not infer app compatibility from the older AVD. The original APK needs the explicit low-target-SDK bypass for this developer test; record its ABI result separately.
4. Install the unmodified published Test 5 wrapper. Confirm the launcher appears, then exercise cache import with the verified complete cache ZIP. Record the cache file count and a representative SHA-256 inside the AVD before launching the game. If the import UI is unreliable, a direct cache copy may isolate native startup, but label it as a separate test that does not validate the importer.
5. Launch the game and capture logcat, the wrapper's diagnostic archive and a screenshot. Report the number of proxy loads, guest `JNI_OnLoad` calls, GL calls, the process ABI and the first fatal error. If the earlier `EM_AARCH64` versus `EM_X86_64` error repeats, the Android 17 result is an **engine-start blocker**, not a gameplay result.
6. Only if native startup succeeds, work through title, animated menu, new-game loading, actual movement and combat, save/reload, and a second cold launch. Preserve screenshots and logs at each passed boundary.

If the wrapper cannot start the engine on this x86_64 AVD, moving forward requires a host-native x86_64 bridge/proxy implementation for emulator testing, an ARM64-hosted Android emulator or device, or an integrated rebuilt engine with a supported 64-bit ABI. Changing the APK's ABI label does not convert its ARM32 machine code.

## Source references

- [Android 17 SDK and emulator setup](https://developer.android.com/about/versions/17/setup-sdk)
- [Android Emulator release notes](https://developer.android.com/studio/releases/emulator) (API 37 minimum AVD RAM; ARM binary support and ARMv7 caveat for older images)
- [Android 64-bit game testing](https://developer.android.com/games/optimize/64-bit) (API 31+ emulator images are 64-bit only)
- [Android 15 all-app behavior changes](https://developer.android.com/about/versions/15/behavior-changes-all) (target-SDK install block and `adb` bypass)
- [Emulator troubleshooting](https://developer.android.com/studio/run/emulator-troubleshooting) (API 37 image revision 5 or newer for Google services)

The APK manifest/ABI values above were read from the actual local APKs with Android SDK Build Tools `aapt dump badging`. The earlier wrapper failure is recorded in [the Test 5 local check](LOCAL-TEST-2026-10-02.md).
