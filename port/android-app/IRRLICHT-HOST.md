# Opt-in Irrlicht host diagnostic

The ordinary `dh2-source-renderer-debug.apk` and its custom GLES renderer keep
their existing package entry points and build output. To produce a separate
same-package build variant with a native Irrlicht activity, run:

```powershell
python port/android-app/build.py --sdk ..\emulator-test\sdk `
  --ndk ..\emulator-test\sdk\ndk\29.0.14206865 `
  --cache ..\cache\files --irrlicht-host
```

The variant is written to
`port/android-app/build/dh2-source-renderer-irrlicht-host-debug.apk`; its
separate report is `port/android-app/irrlicht-host-build-validation.json`.
The alternate manifest registers an internal, opt-in `NativeActivity`. Open
the app's **Diagnostics** screen and scroll to **Irrlicht adapter diagnostic**
to launch it. The normal manifest does not declare that activity, so the button
is omitted from ordinary builds.

This APK packages `assets/third-party-notices/NOTICE.txt` and the exact
upstream Irrlicht, IJG, libpng, AES Gladman, bzip2, zlib, and Lua 5.1.4
license/notice texts. The IJG package includes its upstream README, which states
the required Independent JPEG Group acknowledgment. The per-file source and
package hashes are recorded in `irrlicht-host-build-validation.json`.

This activity runs the pinned official Irrlicht OGL-ES r6038 engine and passes
a synthetic four-face pyramid through `SceneMesh` → `SMesh` → `SMeshBuffer`.
It verifies that the engine can live beside the existing Android app and that
the adapter creates a renderable node. The geometry is test data: this is not a
DH2 level, imported BRES scene, or proof that the game now runs on Irrlicht.
Scene materials, source shaders, actors, input, collision, scripts, and gameplay
are still not hosted by this diagnostic. The ordinary APK remains the existing
custom GLES source preview.

The installed variant rendered on an Android 17/API 37 x86_64 emulator with
4 KiB pages. The APK pulled back from the device matched the built SHA-256;
after backgrounding and resuming, the pyramid region still had 49,024
non-clear pixels, with no `GL_INVALID_OPERATION`, `GL_INVALID_ENUM`, shader
load, or fatal-process errors in the app-PID log. Exact evidence is in
`irrlicht-host-current-apk-runtime-validation.json` (the screenshot and raw log
are local ignored build outputs).

For a separate local-only variant that replaces the synthetic scene with the
pinned `void_maze` textured source-draw subset, see
[Local-only Irrlicht cache-scene APK](IRRLICHT-CACHE-SCENE-LOCAL.md). That
cache-bearing APK is release-ineligible and is not a playable level.
