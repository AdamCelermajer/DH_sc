# SWAMP module-zero Irrlicht source-actor diagnostic

This separate Android `NativeActivity` assembles the supplied SWAMP module-zero
source geometry through the checked BRES/MLX readers and the SceneMesh adapter,
then renders it with the official Irrlicht OGL-ES r6038 port. The screen has a
lower-left touch pad. Its stick drives source-world X/Y and a following camera;
the four selected source Prince default-warrior skins replace the earlier
colored-sphere marker.

The startup position comes from module zero's original `SpawnPoint` with
`entrypointID="0"`. The app resolves it against source Navigation and the
constructor-derived player path mask `0x2`, then creates a bounded
`SwampActorSession`. Each accepted 20 ms development tick is routed through
the source Character Coordinator, authored animation bank and BlendedPlayback,
`actor_runtime`, the module-zero source floor graph, and one Box2D
`NativeWorld::Step`. Source visual-root motion owns the displayed position;
the native player body is pinned in Idle and unpinned while moving. The old
point-mover implementation no longer owns app movement.

The touch receiver clears any held stick on Irrlicht's forwarded Android
`APP_CMD_LOST_FOCUS`, `APP_CMD_PAUSE` and `APP_CMD_TERM_WINDOW` commands, then
allows Irrlicht to process each lifecycle command. The frame loop also requires
both an active Irrlicht window and a non-null Android window, resetting touch
and accumulated time while the surface is absent. When its dimensions change,
the app resizes Irrlicht, resets input bounds, updates camera aspect and moves
the status hint. A failed source frame permanently stops further source ticks;
the status retains completed-frame diagnostics and does not promise rollback of
partially sampled source state.

The source module has 103 selected scene records, 54 draw commands, 10,816
vertices and 13,284 indices. The diagnostic omits exactly the verified
`_module_obj_4of4_brdwalk_sw_00-node` / `ColorMaterial` bridge-root draw because
its original effect state is unresolved. It maps the source `env_swamp.tga`
diffuse sampler on matching draws. The 22 visible `Material__11611` draws each
pair that diffuse with source `AlphaMap` path
`textures/pvr2_env_swamp_alpha.tga`. The diagnostic decodes that cache PVRTC
texture and copies its decoded **blue** channel into the diffuse texture's
alpha for those draws only. It selects Irrlicht's
`EMT_TRANSPARENT_ALPHA_CHANNEL` to preview the recovered shader's `AL`
fractional-alpha output. The source material's type-20 `CurrentTechnique`
selection remains opaque to the checked reader, so the exact `AL` versus `AT`
variant is unknown; the separate source `AT` branch discards values strictly
below `0.8`. Original blend and depth-write state are not established by the
shader source. Irrlicht's standard SRC_ALPHA blending and automatic transparent
pass depth behavior are preview choices, not recovered source state. A host
regression checks the exact material/path pairing and blue-to-alpha
composition. This narrow mapping does not reconstruct the rest of the
referenced Collada effect.

The two visible `Material__11598` overlay draws now use Irrlicht's explicit
`EMT_ONETEXTURE_BLEND` mapping for source `GL_ONE, GL_ONE` factors and
`GL_FUNC_ADD`, with source `LEQUAL` depth testing and depth writes disabled.
The adapter asserts that exactly two source passes are mapped. It does not use
Irrlicht's `EMT_TRANSPARENT_ADD_COLOR`, whose GLES2 implementation uses a
different destination factor. Five source draws have no texture reference; one
is the omitted bridge root and the other four retain Irrlicht's default
material. Draw and sampler counts are reported on screen and in Android logcat.

## Prince source mesh and animation slice

The scene adapter loads four `_default_warrior-mesh-skin` controllers from
`prince_modular.bdae`, binds `atlas_modular_warrior.tga` on all four selected
source primitives, then deforms fresh vertices from immutable bind-pose
positions through the engine-skinning palette into mutable Irrlicht
`SMeshBuffer`s. The Irrlicht renderer node remains at identity. The source
`SceneBinding` computes owner, helper, authored graph, and animated-root
composition once; SWAMP spawn position is the development owner producer.

The current candidate now loads the complete authored Prince AnimationBank
(116 unique resources, 158 ordered registration occurrences), animation and
class/property tables, then drives source `Character::State` through the
shared `character::Coordinator`. The source Idle (state 3, sequence 262) and
Move/Walk (state 4, sequence 280; clip 1126) transitions run through the
shared two-slot `BlendedPlayback`; Idle selects clips 1040/1041. Registered
resources with no serialized animation payload are retained without made-up
timing. The host check verifies Walk pose deformation, release to Idle, and
single application of source owner translation.

The next bounded integration composes those source pieces into
`SwampActorSession`: source Navigation is copied across an explicit neutral
boundary into the module-zero floor graph; source `actor_runtime` owns the
checked visual-root path; one NativeWorld step is ordered before timer/state,
animation, and actor phases per accepted logical frame. It copies the actual
224 resolved Character properties and derives the body/world bounds from the
source Character/decor producers. Its focused host gate checks real cache
floors, state 3 Idle/state 4 Move, body pinning, route activity/clear, one
physics step per actor frame, and explicit teardown. It does not invent a
physics-velocity movement path.

This remains a bounded source integration, not full-game parity. Its player
has a source-sized NativeWorld body and source actor path/floor checks, but the
module-zero scene does not create environment bodies, walls, swept-volume
collision, or contacts against level geometry. The recovered coordinator does
not yet receive the original Character body-present service, so this adapter
mirrors source Idle/Move pinning after those state changes. External
Character/AI services, full combat, triggers, scripts, other-module seams, and
game-level orchestration are not implemented here. Selected source materials
use the atlas and preserve UV/material-color input, but external Collada
effects and the original shader are not reconstructed. The four selected
primitives have no AlphaMap references; other model/material variants remain
outside this slice.

## Rendering and gameplay limits

Irrlicht is confirmed as the game's engine family. This diagnostic embeds the
official upstream Irrlicht OGL-ES r6038 port; the exact customized DH2 Irrlicht
fork/revision and its game-specific layer remain under investigation. The
supplied APK/BRES identifies `glitch::` classes, but this source slice does not
reconstruct that customized engine or the complete game. It is a source-driven
renderer and movement diagnostic.

The two source `Material__11598` passes are mapped to `GL_ONE, GL_ONE`,
`GL_FUNC_ADD`, `LEQUAL`, and depth-write-off. The blue-channel AlphaMap and
fractional `AL` output preview are applied on the 22 verified
`Material__11611` draws. The actual `AL` versus `AT` technique selection is
still unresolved; if source `AT` is selected, its recovered shader threshold
is `0.8`. Other LightMap or Specular samplers,
external Collada effects, source lighting, and native material sorting remain
unmapped. Four visible draws have no source texture references and use
Irrlicht's default material, which accounts for the broad white floor bands in
the current screenshot; smaller dark patches also remain. These gaps can
change the appearance of bridges, water and other transparent or lit surfaces.

The source actor path checks use module-zero floor triangles and source
movement flags, but do not reproduce full controller acceleration, continuous
or swept collision, environmental wall response, AI, combat, triggers, scripts,
camera behavior or save state. A sufficiently thin obstacle or a gap between
checks can still be crossed. This app exercises source Idle and Move only and
does not run the full Character physics/service stack. The full nine-module
SWAMP floor collection supports checked navigation queries; this rendered
scene and session remain restricted to module zero.

## Local build and status

Build a separate, local-only APK from the extracted supplied cache. Keep the
candidate output separate from the previous two-clip artifact. The latest
source session wiring has not yet received its own APK/device validation:

```powershell
python port/android-app/build.py `
  --sdk ..\emulator-test\sdk `
  --ndk ..\emulator-test\sdk\ndk\29.0.14206865 `
  --cache ..\cache\files `
  --irrlicht-swamp `
  --irrlicht-static-dir port\irrlicht-android\build\variant-curated-api37\static `
  --build-dir port\android-app\build\irrlicht-swamp-source-session-candidate
```

First build the pinned engine libraries with
`python port/irrlicht-android/build.py --build-dir port/irrlicht-android/build/variant-curated-api37`.
Install the runtime-smoke helper dependency with
`python -m pip install -r port/android-app/tests/requirements-irrlicht-swamp.txt`.

This is a development diagnostic, not full-game parity. It packages the
owner-supplied cache inputs listed below and has no Drive/GitHub upload step.

The previous two-clip runtime-tested artifact lives in
`port/android-app/build/irrlicht-swamp-prince-preview/`. The full-bank,
source-session candidate is written to
`port/android-app/build/irrlicht-swamp-source-session-candidate/`; its APK is
`dh2-swamp-irrlicht-source-local-debug.apk`. It uses the separate package
`local.dh2.sourceviewer.irrlichtswamp` and launches directly into the native
SWAMP activity, so it can be installed alongside the default app. The focused
host source assertions pass; root owns the APK/device gate. Device smoke must wait for
`FIRST_SOURCE_CHARACTER_FRAME`, emitted after the first successful renderer
buffer swap; `SOURCE_ACTOR_SESSION_READY` proves the session initialized, while
the first-frame marker avoids the earlier assembly/startup timing race. Inspect
`SOURCE_ACTOR_SESSION_READY`, `MOVE`, `SURFACE_RESIZED`, and
`FIRST_SOURCE_CHARACTER_FRAME` logs for source state/flags, sequence/clip,
actor/world counters, `body_present`, body-service calls, path result,
desired/validated headings, source/physics positions, and body radius. The
source Character owns body pin transitions: Move focus unpins; Move blur runs
Stop (clearing the source path/target/heading) and then pins on return to Idle.
Expected state trace is Idle `3/0x2380`, then held-input Move `4/0x23c1`
(Walk clip 1126), then release to Idle. Host callback/lifetime evidence is in
`port/irrlicht-android/build/swamp-actor-body-services-host/validation.json`;
it is separate from the Android device gate. Root owns device runtime validation.

```powershell
python port/android-app/tests/irrlicht_swamp_native_runtime.py `
  --apk port\android-app\build\irrlicht-swamp-source-session-candidate\dh2-swamp-irrlicht-source-local-debug.apk `
  --adb ..\emulator-test\sdk\platform-tools\adb.exe `
  --serial emulator-5558 `
  --require-prince-character `
  --require-source-actor `
  --output port\android-app\build\irrlicht-swamp-source-session-candidate\runtime-source-session-16k
```

The app bundle contains the SWAMP BRES/MLX/entrypoint/diffuse/AlphaMap, Prince
modular model and atlas, plus the 12 checked animation/character table inputs
and 116 source animation-bank assets with per-file size/hash verification.
It also includes the pinned upstream Irrlicht shaders/native library, a hash
manifest, and local/third-party notices. It does not bundle unrelated
encounter or village cache sets.
The bundled `LOCAL-ASSET-NOTICE.txt` records the original cache provenance;
the `third-party-notices/` directory carries Irrlicht and bundled library
notices. See the repository [provenance and rights](../../../RIGHTS.md) record.

The previous APK was built with Android build tools 37.0.0 and NDK r29 for
`arm64-v8a` and `x86_64`, signed and checked with `zipalign`, and packaged as an
API 37 NativeActivity. Its visual/movement/lifecycle checks apply to that
two-clip checkpoint only; the new session candidate needs its own 4 KiB and
16 KiB tests before inheriting any pass claim. The separate integrated app has
its own record in
[IRRLICHT-SWAMP-IN-APP.md](../../android-app/IRRLICHT-SWAMP-IN-APP.md).

```text
APK: port/android-app/build/irrlicht-swamp-prince-character-candidate/dh2-swamp-irrlicht-source-local-debug.apk
Size: 71,416,410 bytes
SHA-256: b367d0fa2e21ebae18c2552e81c16a398f0d4adb3d60b9c07eaf18f9f74dd8f1
Build report: port/android-app/build/irrlicht-swamp-prince-character-candidate/irrlicht-swamp-build-validation.json
Host report: port/irrlicht-android/build/prince-character-host-checks/validation.json
Runtime report: port/android-app/build/irrlicht-swamp-prince-character-candidate/runtime-character-final-16k/irrlicht-swamp-runtime-validation.json
```

The helper run recorded below covers the prior eeb3 source-Prince preview artifact.
It is separate from the upstream dwarf HelloWorld smoke; that sample proves
engine startup only and is not a game run. The new full-bank candidate passed
its own host and device visual/movement/resume checks. Consolidated reports
and exact scope are in [the Character checkpoint](../../../docs/CHARACTER-RUNTIME-CHECKPOINT-2026-10-04.md).

## Previous artifact runtime record

| Device | Result | Source scene and Prince | Movement / rendering | Runtime evidence |
| --- | --- | --- | --- | --- |
| Android 17 / API 37, x86_64, 16 KiB pages (`emulator-5558`) | Passed; installed APK hash matched build | 53 of 54 scene draws rendered; 1 unresolved bridge-root draw omitted; 10,816 vertices / 13,284 indices; 49 diffuse draws and 2 additive passes mapped. Prince: 4 controllers, 27 joints, 487 vertices, 586 triangles; all 4 atlas parts mapped. | +X and +Y moved; release settled to stable idle positions. Prince remains visible head-to-feet in initial Idle, held Walk, and resumed screenshots. No fatal renderer errors, GL errors, or native crash markers. | `port/android-app/build/irrlicht-swamp-prince-preview/runtime-final-16k/` |
| Android 17 / API 37, x86_64, 4 KiB pages (`emulator-5556`) | Historical geometry-only pass; not current Prince build | Earlier artifact `2b4a0232...` predated Prince integration and additive mapping. | Earlier +X/+Y movement passed; it does not validate the current renderer. | `port/android-app/build/irrlicht-swamp-curated/runtime-final-4k/` |

An earlier Prince build (`543019887d1dbbfb47e46bf525a1f86c6314ca726811f4831d6fe63862eaf9d5`)
was a visual regression: Idle framing cropped the head and Walk disappeared.
Its screenshots and report remain under
`port/android-app/build/irrlicht-swamp-curated/runtime-16k/` as failure evidence.
The current build corrects owner/helper/source-graph compensation, camera
framing, and the two additive overlays. Its screenshots show the full Prince
in both sampled motions and no broad foreground black overlay. White fallback
floor bands and smaller dark patches remain visible, so the diagnostic is not
visually complete. A HOME/background then same-process NativeActivity resume
also passed: source position stayed at `(1099.104, -203.522, 255.0)`, Idle was
restored, and the renderer loop resumed. This does not test process-death save
restoration. The detailed report, screenshots, and app-scoped logs are in the
runtime evidence directory; the consolidated report is
`reports/reconstruction-2026-10-04/irrlicht-prince-runtime.json`.

Run the combined host assertions (source module placement/material policy,
movement boundaries and rollback, UTF-8 status decoding, joystick axis
mapping, diagonal clamp, 20 ms pacing, catch-up cap and pause reset) with:

```powershell
python port/irrlicht-android/swamp-smoke/tests/run_host.py --cache ..\cache\files
```

The report is written under the ignored local `port/irrlicht-android/build/`
directory. Run the focused host bridge and session gates when changing these
interfaces:

```powershell
python port/irrlicht-android/game/tests/run_swamp_actor_floor_bridge_host.py `
  --cache ..\cache\files
python port/irrlicht-android/game/tests/run_swamp_actor_session_host.py `
  --cache ..\cache\files `
  --output port\irrlicht-android\build\swamp-actor-session-final
```

The prior source-Prince artifact has a 16 KiB device result; the new
Character/AnimationBank/session candidate still requires its own APK/device
validation after this integration. These host checks are not an Android runtime
claim.
