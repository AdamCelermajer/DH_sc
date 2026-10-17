# Build and verification

## Build the Android app

Install JDK 17+, Android SDK platform 37, NDK `29.0.14206865`, and CMake
`3.22.1`. Open [`port/android-native`](../port/android-native/) in Android
Studio, or use its Gradle wrapper:

```sh
cd port/android-native
./gradlew :app:assembleDebug
```

On Windows, run `gradlew.bat :app:assembleDebug`. Configure the SDK through
Android Studio or an untracked `local.properties`. The APK is created at
`app/build/outputs/apk/debug/app-debug.apk`; the build targets API 37 and
packages ARM64 and x86_64. See the [Android project guide](../port/android-native/README.md)
for install and current scope.

## Test reconstructed modules

Run the focused test runner for the module being changed; its README and test
directory give the command and evidence scope. There is no single full-game
test suite. Host comparisons, Android compilation, package checks, and live
gameplay are separate evidence. The [project checklist](PROJECT-CHECKLIST.md)
states what each current result does and does not verify.

## Local original inputs

The original APK and complete cache ZIP are owner-supplied local inputs and are
not stored in Git. Most checked-in Android assets are sufficient for the
current build. Cache-dependent asset regeneration or comparisons require the
original cache; use the preparation command documented by the affected module,
such as [level-world](../port/level-world/README.md). Keep original inputs and
generated build/test products outside commits; local input directories are
ignored by Git.

Recovered assembly, pseudocode, raw Java/smali, and symbol exports are evidence
from the original binaries, not buildable original game source. To verify the
checked-in imports without restoring external archives, run from the repository
root:

```sh
python tools/verify_recovery_import.py
python tools/verify_native_decomp_import.py
python tools/verify_android_raw_import.py
python tools/verify_native_evidence_import.py
```

The original APK/cache and full historical compressed bundles are separate
inputs. See [source availability](SOURCE-AVAILABILITY.md) and
[rights and provenance](../RIGHTS.md) before redistributing game materials.
