# Building and running the Track B Android build

Two APKs come out of one command. Both contain the same native library (our reconstructed `libdh2.so`, arm64-v8a, Release, stripped,
installed as `libDungeonHunter2.so`) and the same launcher.

| APK | Java side | Status (emulator, API 37 x86_64 + ndk_translation) |
|---|---|---|
| `dh2-trackb-origdex.apk` | the original v1.0.2 `classes.dex` + resources (from your APK) + `DataSetup` as `classes2.dex` | boots: intro, title, menu, new game, level 1, pause/quit/continue |
| `dh2-trackb-source.apk` | `port/android-trackb` (reconstructed Java source), resources imported from your APK | same path, verified end to end (see below) |

Neither APK, `.so` nor game data is tracked: everything lands in `build/own-ab/` (git-ignored).

## Build

Prerequisites: Windows with Python 3, JDK 17, Android SDK (build-tools 37.0.0, platform 37.0, NDK 29.0.14206865; `ANDROID_HOME`
or the default in the script), CMake + Ninja, and your own `Dungeon-Hunter-2-HD-v1-0-2.apk` (used for resources and for the origdex build).

```
py -3 tools/trackb/android/build_release.py [--apk PATH\Dungeon-Hunter-2-HD-v1-0-2.apk] [--skip-native] [--only origdex|source] [--install [--serial emulator-5554]]
```

Steps: `ninja dh2` in `build/own-ab-arm64` (Release, `DH2_BOOT_DEBUG=OFF`, `DH2_ANDROID_ABORT_STUBS=OFF`, FORTIFY at its default = off) -> strip ->
`libStormGLOFT` stub (`tools/trackb/boot/storm_stub.c`) -> source APK via `tools/trackb/java/build_apk.py` (runs
`import_resources.py` once if `port/android-trackb/app/src/main/res` is missing) -> origdex APK -> zipalign 16 KiB -> apksigner with the debug key
(`build/own-ab/debug.keystore`, else `~/.android/debug.keystore`, else generated). Outputs: `build/own-ab/out/dh2-trackb-origdex.apk`,
`dh2-trackb-source.apk`, `libdh2-unstripped.so` (symbolise crashes with `llvm-addr2line -e libdh2-unstripped.so 0xPC`).
`--install` runs `adb install -r -g` and sets the `USE_ICC_AUTH_WITH_DEVICE_IDENTIFIER` app-op (needed by the origdex build only, see below).

## Install and game data

The game reads its data (about 650 MB) from `/sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files`. The original fetched it with Gameloft's
downloader (`GameInstaller`), whose servers are gone. The launcher activity of both APKs is `DataSetup` (a Track B addition): when the data is missing
it unpacks **your** `Dungeon-Hunter-2-HD-v1-0-2-cache.zip` and then starts the game. It looks, in this order, in `/sdcard/Android/obb/<pkg>/`,
`/sdcard/Android/data/<pkg>/`, `/sdcard/Download/`, `/sdcard/` for a `*.zip` or `*.obb` larger than 50 MB that contains `.../files/data/...`.
Savegames and settings inside the zip are skipped (clean profile, English by default). First start takes about a minute (progress bar).

Emulator or phone with USB debugging (arm64 device, Android 8+; the 16 KiB-aligned libs also run on 16 KiB-page devices):

```
adb install -r -g build/own-ab/out/dh2-trackb-source.apk
adb push Dungeon-Hunter-2-HD-v1-0-2-cache.zip /sdcard/Download/        # or /sdcard/Android/obb/com.gameloft.android.GAND.GloftD2SS/
```

Then tap the Dungeon Hunter 2 icon. (Copying the zip to the phone with a file manager works the same.) `tools/trackb/boot/push_data.sh` is the old
root-only way to push extracted files directly; it is not needed any more. Uninstalling the app deletes the unpacked data (the zip stays).

Start-up differences from the original, on purpose: the launcher is `DataSetup`, not the Samsung DRM activity `Zirconia_DRM` (its licence server is gone;
offline it ends in an error dialog). `targetSdk` is 35 for the source/bundled APKs (28 for the origdex one); no "built for an older version of Android" dialog on Android 14 to 17.

Screen layout on a non-16:9 screen (Fold, 21:9 phones): the "!" bubble dialog has the port settings: *Unstretched layout* (HUD at the real edges, menus in a centred 16:9 box; off = the original's stretch) and *HUD size* (Auto = 125% on a square screen). Stored in `files/dh2_port.cfg` (`screenfit=0|1`, `hudscale=0|100..200`); `adb shell "echo screenfit=0 > /sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files/dh2_port.cfg"` works too (read at start). Per-button size: Options > Custom (the game's HUD layout page), tap a button, then the + / - medallions above it; Reset puts all back to 100%.

## Bundled APK (one file, no data copy)

```
py -3 tools/trackb/android/build_release.py --bundle-data C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip [--out-root build/own-x --build-dir build/own-x-arm64] [--skip-native]
```

Builds the source-Java APK and embeds your cache zip as the STORED asset `assets/dh2data.zip` (plus `assets/dh2data.txt`: prefix, file count, byte count after skipping the repacker's savegames/settings) -> `out/dh2-trackb-bundled.apk` (about 452 MB, debug-signed, zipaligned). `DataSetup` (launcher) streams the asset through `ZipInputStream` (never in memory) into `/sdcard/Android/data/<pkg>/files`, shows a progress bar, refuses to start with a message if free space is below the unpacked size + 64 MB, verifies file/byte counts and the `d_o_w_n_l_o_a_d_e_d.txt` marker, then starts the game; later launches skip straight to the game (an interrupted unpack leaves `.dh2_extracting` and is redone). Without the asset DataSetup falls back to the zip/obb search described above.
Install: `adb install -r -g dh2-trackb-bundled.apk` or sideload the file; first start takes about a minute. Never commit the APK, the .so or the data.

## Known issues

- origdex APK: `TelephonyManager.getDeviceId()` in the original dex is not caught and Android 10+ refuses it, so run
  `adb shell appops set com.gameloft.android.GAND.GloftD2SS USE_ICC_AUTH_WITH_DEVICE_IDENTIFIER allow` once (`--install` does). The source APK catches it
  (`DungeonHunter2`, `GLUtils/Device`, `GetNpost`) and needs no adb step.
- Rendering and timing were only tested in the emulator (software GL, ndk_translation); no real arm64 phone yet. Expect lower frame rates in the emulator.
- libStormGLOFT is a stub (no glsl-optimizer, no Mali identity, no CPU texture decode); Billing, Samsung/KDDI DRM and Gameloft Live are not functional.
- Open-world bugs of the reconstruction still apply (see `reports/decomp-reconstruction/FIRST-BOOT.md` and `CHAOS-BUGS.md`).
- The source APK has been booted only on API 37; the closed-SDK classes (billing, Plasma, ALML) are reproduced but unexercised.

## Capturing a crash on a real phone (always-on diagnostics)

The app records crashes itself: a Java uncaught-exception handler (`dh2_java_crash.txt`), the native signal handler in libStormGLOFT (`dh2_native_crash.txt`), and on the next
launch `DataSetup` merges them with the exit reasons / tombstone (`dh2_tombstone.bin`) and the app's logcat into `dh2_crash.txt` and shows a dialog with the first line
(buttons: Play anyway, Copy report). All files are in `/sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files/`.

```
adb devices                                   # USB debugging, or: adb pair <ip:port> then adb connect <ip:port> (wireless debugging)
adb logcat -c                                 # clear, then start the game and let it crash
adb logcat -b all -d -v threadtime > dh2_logcat.txt
adb logcat -d -s DH2CRASH DH2GL DH2GLINFO DH2STDOUT AndroidRuntime DEBUG libc   # the interesting tags only
adb pull /sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files/dh2_crash.txt
adb pull /sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files/dh2_native_crash.old
adb bugreport dh2_bugreport.zip               # contains the system tombstones (needs no root)
adb shell dumpsys activity exit-info com.gameloft.android.GAND.GloftD2SS     # why the last processes died
```
Native offsets (`pc (off 0x...)`, `stack ...: off 0x...`) map to source with `llvm-addr2line -e libdh2-unstripped.so 0x<off>` (the .so next to the APK).
`DH2GLINFO` lines give the GPU, limits and extension list; `DH2GL` lines are shader-compile and texture-upload errors. Phone identity (IMEI, number, SIM operator)
is never requested: all TelephonyManager reads go through `Phone.java`, which returns the no-SIM values when Android refuses.
