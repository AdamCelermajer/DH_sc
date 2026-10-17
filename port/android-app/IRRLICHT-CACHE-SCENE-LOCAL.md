# Local-only Irrlicht cache-scene APK

`--irrlicht-cache-scene` builds an opt-in local APK variant of the Android
source app. Its internal Irrlicht NativeActivity renders the checked
`void_maze.bdae` BRES and the source `env_voidmaze.tga` PVRTC texture through
the pinned Irrlicht adapter. The diagnostic currently isolates the 30 source
draws that reference that texture and aims the camera at their centroid. A
small texture swatch is drawn as an explicit upload diagnostic. This verifies
texture mapping on that source-draw subset; it does not validate all materials
in a complete level render and is not a playable level or a release build.

Build with a caller-supplied cache root outside the repository:

```powershell
python port/android-app/build.py `
  --sdk ..\emulator-test\sdk `
  --ndk ..\emulator-test\sdk\ndk\29.0.14206865 `
  --cache ..\cache\files `
  --irrlicht-cache-scene
```

The builder verifies both selected cache files against fixed byte counts and
SHA-256 hashes before staging them under ignored `port/android-app/build/`.
It packages just those scene-test inputs in the Irrlicht asset bundle, builds
`arm64-v8a` and `x86_64`, uses API 37 / min API 26, checks 16 KiB ELF and APK
alignment, and includes the pinned shader and third-party notice bundle. The
source assets are not added to Git. The APK path is
`port/android-app/build/dh2-source-renderer-irrlicht-cache-scene-local-debug.apk`.

The ignored report `build/irrlicht-cache-scene-build-validation.json` records
the local-only/release-ineligible scope, source asset provenance and hashes,
native build hashes, Irrlicht shader/notice hashes, and the APK hash. It is not
a release manifest.

Run the exact APK on the API 37 / 4 KiB x86_64 test emulator and verify the
installed APK hash, source-scene log, visible texture color variation, and
pause/resume lifecycle:

```powershell
python port/android-app/tests/irrlicht_cache_scene_runtime.py `
  --sdk ..\emulator-test\sdk `
  --apk port/android-app/build/dh2-source-renderer-irrlicht-cache-scene-local-debug.apk
```

The runtime script accepts `--serial emulator-5558` for the API 37 / 16 KiB
emulator. Its independent ignored evidence is written to
`build/irrlicht-cache-scene-16k/`; it does not replace the 4 KiB runtime report.

Runtime JSON, screenshots, installed APK, and app log are all under ignored
`build/irrlicht-cache-scene/`. The variant can also be opened from the app's
Diagnostics screen using the existing **Irrlicht adapter diagnostic** entry.
The ordinary APK and synthetic `--irrlicht-host` build retain their existing
manifest, NativeActivity library and package behavior. This mode alone stages
the direct `assets/dh2/...` inputs and substitutes the cache-scene renderer in
its separate local APK.

## Latest local verification

The built and installed APK SHA-256 is
`17f4ea0bee95b16a03d0cc6f4ea7d876a4acb9c7deda8b1db5a9b91fc9d85981`; the
pulled base APK matched. On emulator-5556 (Android 17/API 37, x86_64, 4 KiB
pages) and emulator-5558 (same Android/API/ABI, 16 KiB pages), the installed
hash matched and all 30 referenced source draws were assigned the texture. On
both emulators the screenshot showed 2,835 distinct RGB colors across 13,022
changed mesh pixels; the diagnostic swatch showed 18,978 colors on 4 KiB and
18,991 on 16 KiB. HOME and relaunch returned to the scene with the process
alive, with zero `GL_INVALID_OPERATION`, `GL_INVALID_ENUM`, or fatal-signal
reports.

The ignored build report records both app and Irrlicht ARM64/x86_64 library
hashes and their 16 KiB PT_LOAD alignments. The resumed screenshot is
`build/irrlicht-cache-scene/runtime-emulator-5556-resumed.png`; runtime JSON
and the app-PID log are beside it in that ignored directory.

Pinned local cache inputs:

| Input | Bytes | SHA-256 |
|---|---:|---|
| `data/3d/modules/void_maze/void_maze.bdae` | 199,300 | `7b67b90b65b41de96a5bd5a9a9d9d8f40cdd4425e806057a28ee5299ed2840ba` |
| `data/3d/textures/env_voidmaze.tga` | 32,828 | `aac2c1923d2b9add49d6d8c32211d0af9d1f0a9e022530d551d1dc65d2703c20` |

The texture extension is misleading: this file is a BTEX wrapper containing
PVRTC1 data. The local NativeActivity uses the checked repository decoder and
converts RGBA8 output into the format expected by Irrlicht's Android driver.
