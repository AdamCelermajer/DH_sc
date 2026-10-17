# Cache-backed Irrlicht scene smoke

This isolated Android NativeActivity bundles two user-supplied local-cache
inputs for build-time packaging only: `void_maze.bdae` and
`env_voidmaze.tga`. At runtime it opens the BRES with the repository's checked
`dh2_bres_open`, assembles the actual static `SceneMesh`, adapts every visible
draw into Irrlicht mesh buffers, assigns the referenced floor texture when the
source sampler names it, and draws the result through the pinned Irrlicht OGL-ES
SVN r6038 renderer. It is a render experiment; it is not integrated into the
main app and contains no gameplay.

Despite the `.tga` extension, the verified diffuse file starts with the
`BTEXpvr\0` wrapper and contains PVR v2 PVRTC1 data. Irrlicht's TGA loader
rejects it. This smoke uses the repository's checked PVRTC decoder, converts
RGBA8 to Irrlicht's BGRA `ECF_A8R8G8B8`, creates an Irrlicht texture, and binds
it to source draws whose sampler names `env_voidmaze.tga`.

The script verifies the exact cache-input hashes before copying either file.
Copies are written only under this directory's ignored `build/` tree and are
never source-controlled. The report records the provenance, API target, ELF
and APK alignment, build output hashes, and any optional emulator observation.

From the repository root:

```powershell
python port/irrlicht-android/cache-scene-smoke/build_cache_scene.py
```

The package targets API 37 with min API 26 and includes x86_64 and ARM64.
Native code is compiled against Android API 26 and linked with 16 KiB ELF page
alignment; the APK is zip-aligned for 16 KiB pages and signed with a local
debug key. Emulator execution is opt-in, requires an explicit `--serial`, and
checks that the selected device is Android 17/API 37. Outputs are confined to
the ignored `build/` directory here. The pinned upstream Irrlicht tree and its
shared build script are not modified by this experiment.

The harness records the installed base APK hash, an on-screen capture,
center-region pixel counts/color variation, Activity logs, and a Home/pause/
resume cycle. Inspect the screenshot before calling the imported scene a
visible render pass; a build or PASS log alone is not sufficient.

## Latest API 37 run

Tested on `emulator-5558` (Android 17 / API 37, x86_64, 16 KiB page size).
The diagnostic was adjusted after the first full-scene view hid all but
untextured upper surfaces: the 30 source draws referencing `env_voidmaze.tga`
are bounded near Z=-0.07, below the untextured surfaces in the initial camera
view. The final screenshot isolates those actual source draws and aims the
camera at their weighted centroid. They visibly render with source texture
variation (2,835 distinct RGB colors across 13,022 changed mesh pixels), while
the decoder swatch shows 18,981 distinct RGB colors. This confirms texture
mapping on the filtered textured subset; it does not yet validate every
material in a fully visible render of the whole scene.

The final installed base APK SHA-256 exactly matched the build APK:
`7a2b0454a5668f51fb3ea7bdc80638ce45c4a42a9f741d3de05ad52d541440b3`. The report records
30/30 filtered draws textured, no `GL_INVALID_OPERATION` messages, and a
NativeActivity process that survived a Home/pause/resume cycle.

Ignored build evidence is under `build/`:

- `cache-scene-build-report.json` records source hashes, exact APK hash,
  installed hash match, runtime logs, lifecycle result, and pixel counts.
- `runtime-emulator-5558-resumed.png` is the resumed API 37 screenshot. It
  shows both the source mesh and the decoded texture swatch used to check the
  texture upload path.
- `logs/runtime-emulator-5558-logcat.txt` contains Irrlicht and lifecycle
  logs.

The local cache inputs were verified before packaging. Their SHA-256 values
are `7b67b90b65b41de96a5bd5a9a9d9d8f40cdd4425e806057a28ee5299ed2840ba`
for `void_maze.bdae` and
`aac2c1923d2b9add49d6d8c32211d0af9d1f0a9e022530d551d1dc65d2703c20` for
`env_voidmaze.tga` (a BTEX/PVRTC v2 file despite its extension). No original
APK, cache assets, generated APK, or screenshots are tracked.
