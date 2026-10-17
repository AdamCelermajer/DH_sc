# SceneMesh adapter runtime smoke

This standalone NativeActivity constructs a small synthetic four-face pyramid
as `dh2::viewer::SceneMesh`, passes it through
`dh2::irrlicht_adapter::build_mesh`, adds the resulting Irrlicht mesh to a
scene, and renders it with the pinned Irrlicht OGL-ES r6038 library. It uses a
unique package ID (`org.dh2.irrlicht.adapter.smoke`) and does not alter the
upstream build harness or the Android game application.

The APK packages the shader files from `upstream/media/Shaders/`, checked
against `upstream-source-manifest.json` (official Irrlicht OGL-ES SVN r6038).
The fixed-function material renderer needs these files at runtime. The GUI
label uses Irrlicht's embedded `BuiltInFontData`, so this smoke does not package
external font assets.

The geometry is test data. It is **not a DH2 asset, imported BRES model, or
evidence of integration into the DH2 game**. It verifies the actual adapter
allocation/conversion path, mesh-buffer construction, scene-node setup, and
Irrlicht draw loop on Android.

Build from the repository root:

```powershell
python port/irrlicht-android/game-smoke/build_smoke.py
```

This compiles x86_64 and ARM64 NativeActivity libraries against the already
pinned r6038 static libraries, packages API 37/min API 26, and checks ELF
`PT_LOAD` alignment, APK 16 KiB zip alignment, and APK signatures. Set
`ANDROID_NDK_HOME` or `ANDROID_SDK_ROOT` to override the repository's local
tool paths. Generated output and emulator captures are ignored under `build/`.

To run the x86_64 package on an online API 37 16 KiB emulator:

```powershell
python port/irrlicht-android/game-smoke/build_smoke.py --install --serial emulator-5558
```

The run record and screenshot are written to `build/runtime-smoke-validation.json`
and `build/runtime-smoke.png` respectively. The initial API 37/16 KiB run
created the adapter mesh and entered the Irrlicht activity, but its screenshot
showed only the clear-color background. That run is **not** a visible-render
pass. The cause was that the isolated APK omitted Irrlicht's OGLES2 material
shaders. The harness now packages and verifies the pinned upstream shader
assets, logs the Activity PID and scene bounds, and includes a built-in cube as
a camera/scene-render control. Runtime screenshot verification uses Pillow and
requires at least 500 changed pixels in the adapter-pyramid screen region; a
startup log alone cannot pass.

After adding the shaders, a 12-second-settled run passed on the Android 17/API
37 x86_64 emulator with 16 KiB pages. The screenshot shows the
adapter-built pyramid next to the built-in cube control. Its checked center
region around the pyramid (excluding the cube and label) contains **28,080
pixels** that differ from the clear color (the pass threshold is 500). The APK
pulled back from the emulator has the same SHA-256 as the built APK. The app
log has no missing-shader message or fatal error. See the ignored runtime
report, screen capture, and app-only log under `build/` for exact hashes and
details.
