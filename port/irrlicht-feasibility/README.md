# Irrlicht integration feasibility

## Current status — 2026-10-04

The game engine is Irrlicht. The [DH2Work lineage mapping](https://github.com/Noamcelermajer/DH2Work/blob/main/docs/IRRLICHT-MAPPING.md)
maps `glitch::` to Irrlicht's `irr::` namespace and places the fork in the 1.8
family (most likely 1.8.0–1.8.3; the point release is uncertain). It finds the
video driver and scene/mesh interfaces substantially rewritten, so upstream
Irrlicht is not ABI-compatible with the shipped fork. The exact customized
source has not been recovered. The separate, complete official
Irrlicht OGL-ES branch snapshot at SVN r6038 now builds and renders cleanly in
its isolated Android NativeActivity sample. Its signed API 37 APK has
ARM64/x86_64 libraries with 16 KiB-aligned load segments; on Android 17/API 37
x86_64 it draws the textured, animated upstream dwarf on both 4 KiB and 16 KiB
page-size emulators, with no app-PID `GL_INVALID_OPERATION` or
`GL_INVALID_ENUM` reports.

An opt-in same-package API 37 build also opens the cache-backed SWAMP module-zero
view through Irrlicht NativeActivity from the Java app's Diagnostics screen. Its
latest AlphaMap build passed package-hash, navigation, assembly, and app-error
checks on the Android 17 x86_64 16 KiB emulator. Current validation is 16 KiB
only; see the [in-app SWAMP report](../android-app/IRRLICHT-SWAMP-IN-APP.md).

The SceneMesh-to-Irrlicht adapter has rendered a synthetic test pyramid in a
standalone smoke app and in the Android package. A cache-backed `void_maze`
scene has rendered visible geometry on API 37/16 KiB and maps
`env_voidmaze.tga` to the 30 draws that reference it. The in-app SWAMP
NativeActivity now assembles a full source module-zero subtree and passes
navigation/runtime smoke on API 37/16 KiB; alpha cutouts are mapped for 22
verified foliage draws, while floor materials, full scene fidelity, and
gameplay remain unfinished. The
ordinary launcher still uses the authored custom GLES renderer. See the
[in-app SWAMP report](../android-app/IRRLICHT-SWAMP-IN-APP.md),
[upstream runtime/build proof](../irrlicht-android/PROOF.md),
[synthetic adapter smoke](../irrlicht-android/game-smoke/README.md),
[cache-scene smoke](../irrlicht-android/cache-scene-smoke/README.md), and
[opt-in app diagnostic](../android-app/IRRLICHT-HOST.md).

## Finding

The local original game binary contains a substantial customized Irrlicht
engine, including an Android device implementation and an OpenGL ES 2 driver.
The [DH2Work mapping](https://github.com/Noamcelermajer/DH2Work/blob/main/docs/IRRLICHT-MAPPING.md)
reports a verified namespace rename from `irr::` to `glitch::` and an Irrlicht
1.8-family ancestry, with the exact point release unresolved. This confirms
Irrlicht as the engine and gives us source-level lineage clues.

The original is a **game-specific fork**, not a drop-in copy of the public
Irrlicht SDK. The engine has custom Collada/BDAE loading, modular skinning,
scene nodes, materials, shaders and platform code; DH2Work's vtable analysis
finds the video driver and scene/mesh APIs were substantially rewritten. No
source archive for this exact fork was found. The separate public upstream r6038
snapshot under `port/irrlicht-android/upstream/` is a rendering port and source
reference; it is not the recovered game's engine. The decompilation and
assembly exports are evidence, not compilable source.

The binary does not establish an exact upstream Irrlicht release. DH2Work's
comparison supports the 1.8 family (most likely 1.8.0–1.8.3) but does not pin the
point release. The recovered source-file list has 867 basenames, including `COpenGLES2Driver.cpp`,
`CAndroidOSDevice.cpp`, `CIrrFactory.cpp`, `CSceneManager.cpp`, and many
`CCollada*` files, but no version-named source file. Searches of its printable
strings and recovered names found no reliable version banner or version macro.
Treat any public SDK version as a **new baseline choice**, not as the original's
proven version. The selected separate baseline is the official OGL-ES r6038
branch (1.9.0 alpha); it is not evidence that Dungeon Hunter 2 used that exact
revision.

## Evidence from this checkout and owner inputs

| Evidence | What it establishes | Limit |
| --- | --- | --- |
| `work/native-bridge-experiment/decoded/assets/dh2/game.apk` (10,250,918 bytes, SHA-256 `84efe6018bcabb5c7e7bfb1953911dc67a88af19425816a8d55bc81dbd03a79b`) | This embedded guest APK contains `lib/armeabi-v7a/libDungeonHunter2.so`. | It is the extracted guest APK, not a new source build. |
| `libDungeonHunter2.so` (15,938,284 bytes, SHA-256 `45891aad9e7a5b1d84a5218f91926a04bd13a62ce104cb29beca7c70228c93c4`) | ELF32, ARM, SONAME `libDungeonHunter2.so`; depends on `libGLESv2.so`, `libGLESv1_CM.so`, `libstdc++.so`, and old Android system libraries. | This ARM32 binary is not an Android 17 ARM64 replacement. |
| `recovered/native/symbols/libDungeonHunter2.so/strings.csv` and `build-source-filenames.json` | Mangled names include `glitch::CIrrFactory`, `glitch::video::COpenGLES2Driver`, `glitch::CAndroidOSDevice`, and `glitch::scene::CSceneManager`. The source list includes Irrlicht, Android, GLES2, GLSL, Collada, and skinned-mesh files. | Filenames and symbols do not supply the source bodies or identify an upstream version. |
| Binary printable source paths | The ELF includes paths such as `project_vs2005/Game/../../sources/Core/Irrlicht/SceneManager.cpp` and `.../Nodes/RootSceneNode.cpp`; other source basenames include `COpenGLES2Driver.cpp` and `CAndroidOSDevice.cpp`. | Build-path strings are corroborating fingerprints, not source availability. |
| `recovered/assets/source-data/.../shaders.pak.contents/` and `../cache/files/shaders.pak` | The local `shaders.pak` is a ZIP with 34 entries: 32 GLSL files and `cg.config`/`glsl.config`. Original shader sources are already present in the recovered checkout, including `UnlitMaterialColorVP/FP.glsl`, shadow/reflection, multi-texture and skinned/lighting shader families. | Knowing the shader text does not reproduce original effect selection, uniforms, pass ordering or GPU state. |
| `port/engine-resources/`, `port/asset-payloads/`, `port/material-bindings/`, `port/scene-payloads/`, `port/scene-draw/`, `port/skin-payloads/`, and animation modules | There is already a substantial set of checked source readers and renderer-neutral mesh/scene/animation interfaces to feed an Irrlicht adapter. `port/renderer-preview/` already compiles selected original GLSL for a diagnostic WebGL draw. | These are not Irrlicht classes and do not implement the original renderer, full effect/material behavior, or gameplay. |
| `port/android-app/build.py` | The ordinary app targets SDK 37/minimum 26, compiles native ARM64 and x86_64 with NDK r29 Clang, GLES2/EGL, and `-Wl,-z,max-page-size=16384`. An opt-in build includes the separate r6038 engine, a cache-backed SWAMP module-zero scene, and an Irrlicht `NativeActivity`. | The ordinary launcher still uses Java `GLSurfaceView` plus the authored custom GLES encounter. SWAMP rendering is diagnostic and has no gameplay. |

The `IrrlichtConfig.lodBiasValue` entry in
`reports/pydata-constant-reader-trace.json` is another game configuration
fingerprint. It is not an engine-version marker.

## Android 17 build path

The pinned OGL-ES r6038 sample compiles for ARM64/x86_64 with NDK r29 (native
API 35), packages at target API 37/minimum API 26, and passes 16 KiB ELF plus
APK alignment/signing checks. The exact API 37 APK was visually checked on
x86_64 emulators with both 4 KiB and 16 KiB pages; the settled screenshots show
the textured, animated upstream dwarf, and app-PID logs report zero
`GL_INVALID_OPERATION` and `GL_INVALID_ENUM`. An early cold-start screenshot
was blank, but later captures after scene loading rendered correctly. The
evidence is in `port/irrlicht-android/runtime-smoke-validation.json` and
`port/irrlicht-android/PROOF.md`. This is a clean upstream-sample render check,
not proof of DH2 integration or process-death/EGL-context-loss recovery.

There is an Android lifecycle boundary to resolve before moving gameplay onto
Irrlicht. The default application hands an EGL context and render thread to
native code through Java `GLSurfaceView`; the upstream sample and opt-in app
diagnostic instead use Irrlicht's `android_native_app_glue`/`NativeActivity`
loop and let Irrlicht own the device and surface. Both approaches have now
rendered on API 37, but the diagnostic renders only synthetic geometry. A
gameplay integration must choose one EGL owner and connect it to the existing
app lifecycle rather than create a second EGL owner on the same surface.

The build flags also need deliberate review. The current renderer build uses
`-fno-exceptions -fno-rtti -nostdlib++`; an Irrlicht source build may require a
different C++ runtime and compiler configuration. Decide that from the exact
pinned source rather than assuming the current flags are suitable.

Original `.bdae` resources are BRES/Collada-derived and not ordinary stock
Irrlicht meshes. The existing checked readers should remain the source of
truth: adapt their `SceneMesh` / `SceneDrawDescriptor` output to an Irrlicht
`IMesh`/`IMeshBuffer` and scene node, then add source material, shader and
animation adapters incrementally. Stock Irrlicht can provide device, driver,
scene graph, visibility/camera traversal and render submission, but it does
not replace the game's Lua, AI, collision, quests, combat, save semantics,
custom resource loading, or effects pipeline.

## Small first integration slice

The separate SceneMesh-to-Irrlicht adapter compiles and links against the
pinned r6038 libraries for ARM64 and x86_64. It converts checked draws to
bounded `SMeshBuffer` objects, remaps local indices, and estimates normals.
The synthetic pyramid has rendered through this adapter in a standalone
NativeActivity smoke and in the opt-in app diagnostic; neither test uses a DH2
asset. A separate cache-backed `void_maze` scene has rendered its geometry on
API 37/16 KiB through the same adapter. The cache-backed smoke visibly maps
`env_voidmaze.tga` onto the 30 draws that reference it. This verifies that
texture subset only; it is not a full-scene material signoff. See the
[adapter implementation](../irrlicht-android/game/README.md),
[synthetic runtime proof](../irrlicht-android/game-smoke/README.md), and
[cache-scene report](../irrlicht-android/cache-scene-smoke/README.md).

Acceptance checks for that integration are:

1. Host tests assert that every converted index is in range and that position,
   UV, triangle and material counts agree with the source descriptors.
2. The upstream standalone APK and synthetic adapter smoke already render on
   API 37; the opt-in app diagnostic also renders its synthetic mesh on API 37
   / 4 KiB. The cache-backed scene shows geometry and maps the checked diffuse
   texture to its 30 referencing draws on API 37 / 16 KiB. Whole-scene
   composition and other source material, effect, and pass behavior remain
   unresolved.
3. The exact APK's ARM64 and x86_64 native libraries have 16 KiB-aligned load
   segments, and the APK targets API 37.
4. A screenshot/readback shows a non-background triangle/mesh, then the same
   run exercises pause/resume and surface recreation.

The existing checks prove that the selected upstream Irrlicht build and
adapter can render test geometry on Android, and that one cache-backed scene
maps its checked diffuse texture to the 30 referencing draws. They do not
prove complete scene composition, other source material/effect fidelity, full
renderer parity, gameplay integration, or a playable game. The next renderer
work is to integrate the module-zero SWAMP scene with its source-driven
movement, then resolve the remaining materials and connect character
animation. Keep the custom renderer as the current app path until an
integrated Irrlicht gameplay build passes equivalent level and lifecycle
tests.

## Version and licensing decision

The public Irrlicht site lists the source-containing 1.8.5 SDK separately from
the experimental OGL-ES r6038 branch. The game project now pins the latter as a
new Android baseline; the successful current-toolchain build is recorded in
`port/irrlicht-android/PROOF.md`. The engine license is zlib/libpng-derived and
the imported third-party notices must be preserved. This does not establish
rights to redistribute DH2 assets or reconstructed game-specific code.

The game's embedded custom fork and proprietary game assets remain a separate
rights question. Reusing the public Irrlicht SDK license does not license DH2
assets, original APK code, or reverse-engineered game-specific material.
`RIGHTS.md` remains authoritative for this repository's provenance boundary.

## Commands used for this audit

Run from the repository root. The original inputs are outside Git.

```powershell
# Inspect the embedded guest APK and hash its native engine library.
$apkPath = Resolve-Path ..\native-bridge-experiment\decoded\assets\dh2\game.apk
$engineElf = Join-Path $env:TEMP 'libDungeonHunter2-irrlicht-audit.so'
python -c 'import hashlib,pathlib,sys,zipfile; p=pathlib.Path(sys.argv[1]); z=zipfile.ZipFile(p); b=z.read("lib/armeabi-v7a/libDungeonHunter2.so"); pathlib.Path(sys.argv[2]).write_bytes(b); print("APK",len(p.read_bytes()),hashlib.sha256(p.read_bytes()).hexdigest()); print("ELF",len(b),hashlib.sha256(b).hexdigest())' $apkPath $engineElf

# Confirm symbols and retained source-file names from checked-in recovery data.
rg -n "COpenGLES2Driver.cpp|CAndroidOSDevice.cpp|CIrrFactory.cpp|CSceneManager.cpp|glitch.*IrrFactory" recovered/native/symbols/libDungeonHunter2.so

# Check whether engine source/header/archive names exist in this workspace.
rg --files .. | rg -i "(^|[\\/])(irrlicht|CIrrFactory|COpenGLES2Driver|CAndroidOSDevice|irrlicht\.h)([\\/._-]|$)"

# Inspect the extracted ARM32 ELF with the installed Android NDK reader.
..\emulator-test\sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-readelf.exe `
  --file-header --dynamic $engineElf

# Confirm the real cache shader archive is a ZIP and enumerate its programs.
python -c "import zipfile,pathlib; z=zipfile.ZipFile(pathlib.Path(r'..\cache\files\shaders.pak')); print(len(z.namelist()),sum(x.endswith('.glsl') for x in z.namelist()),z.namelist())"
```

The original APK and cache are owner-supplied local inputs and are deliberately
not copied into this report folder.

## References

- [Irrlicht project downloads: SDK 1.8.5 and source](https://irrlicht.sourceforge.io/?page_id=10)
- [Irrlicht project forum: Android/Gradle example using its OGL-ES source branch](https://irrlicht.sourceforge.io/forum/viewtopic.php?t=52182) (historical community instructions)
- [Android 17 SDK setup](https://developer.android.com/about/versions/17/setup-sdk)
- [Android 16 KiB page-size compatibility](https://developer.android.com/guide/practices/page-sizes)
