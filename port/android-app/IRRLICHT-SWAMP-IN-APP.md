# Opt-in Irrlicht SWAMP view inside the Android app

The `--irrlicht-swamp-in-app` build combines the existing Java app and its
GameplayActivity launcher with a non-exported Irrlicht NativeActivity. It is a
separate build output and does not change the default APK or the standalone
`--irrlicht-swamp` package.

Build it from the repository root with Android SDK 37, NDK r29, and the
owner-supplied extracted cache:

```powershell
python port/android-app/build.py --sdk ..\emulator-test\sdk `
  --ndk ..\emulator-test\sdk\ndk\29.0.14206865 `
  --cache ..\cache\files --irrlicht-swamp-in-app
```

The local APK is written to
`port/android-app/build/irrlicht-swamp-in-app/dh2-source-renderer-irrlicht-swamp-in-app-local-debug.apk`.
It uses `port/android-app/build/debug.jks`, the same key as the standard
same-package APK, so installing it with `adb install -r` can upgrade that app.
The build report is
`port/android-app/build/irrlicht-swamp-in-app/irrlicht-swamp-in-app-build-validation.json`.

Open the standard authored encounter, tap **Diagnostics**, scroll to
**SWAMP module 0 · Irrlicht source view**, then press Android Back twice to
return to the Java encounter. The first Back returns to the Diagnostics
`MainActivity`; the second returns to `GameplayActivity`. Android pauses the
Java GLSurfaceView activity while the NativeActivity owns its own Irrlicht EGL
surface. The Irrlicht scene and module-zero movement use source coordinates
and navigation independently of the Java encounter session.

After the build, the UI runtime harness installs the APK as an in-place update,
checks its installed SHA-256, taps both controls through the accessibility tree,
asserts all three foreground Activities and both Back transitions, and checks
the SWAMP startup marker and app-process error logs. Run it only on the
Android 17/API 37 x86_64 16 KiB emulator for this project. It does not clear
app data or logcat.

```powershell
python port/android-app/tests/irrlicht_swamp_in_app_runtime.py `
  --adb ..\emulator-test\sdk\platform-tools\adb.exe `
  --apk port/android-app/build/irrlicht-swamp-in-app/dh2-source-renderer-irrlicht-swamp-in-app-local-debug.apk `
  --serial emulator-5558 --expected-page-size 16384 `
  --output port/android-app/build/irrlicht-swamp-in-app/runtime-16k
```

Each run writes screenshots, the Diagnostics and NativeActivity UI hierarchies,
app-PID filtered logcat, and
`irrlicht-swamp-in-app-runtime-validation.json` beneath its output directory.

This variant bundles original cache inputs and is for local testing only. Keep
the APK and its build output private; it is not a public release artifact.
The independent SWAMP module-zero renderer remains a partial diagnostic rather
than a complete game port.

## Verified build and emulator checkpoint — 2026-10-04

The exact local APK is 74,319,320 bytes with SHA-256
`1d0d6580560a2168eb62fbd8c8b541802b51028fb695090e539c65f8d3fefc46`. The
standard package installed as an in-place update and its installed hash matched
the APK. The current build passed on Android 17/API 37 x86_64 with 16 KiB pages:

| Emulator | Page size | Result |
| --- | ---: | --- |
| `emulator-5558` | 16 KiB | Pass |

The run followed Gameplay → Diagnostics → Irrlicht NativeActivity → Diagnostics
→ Gameplay, observed SWAMP module-zero assembly, found no fatal/GL/texture
ownership errors, and returned to the Java encounter HUD. App data was
preserved and logcat was not cleared. The existing saved encounter was already
defeated; this navigation test does not claim to reset or validate that
encounter. This build was tested only on the 16 KiB emulator; no 4 KiB or Fold7
test was run. Reports, screenshots, UI dumps, and filtered logs are under the
ignored `port/android-app/build/irrlicht-swamp-in-app/runtime-final-alpha-16k/`
directory.

This verifies a development route into the Irrlicht renderer, not that the
SWAMP scene is visually faithful or playable. The current capture shows
textured trees and bridges after mapping the cache AlphaMap onto its 22
`Material__11611` alpha-reference cutout draws. A large black foreground slab
and white fallback floor bands remain. The alpha-cutout host check also
asserts that visible foliage pixels write depth; the two source additive
overlays retain depth-write-off behavior.
