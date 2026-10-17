# SceneMesh to Irrlicht adapter slice

This isolated module adapts the repository's checked, flattened
`dh2::viewer::SceneMesh` and `SceneDrawDescriptor` records into the public
Irrlicht `SMesh` / `SMeshBuffer` APIs. It adds one mesh buffer per visible draw,
copies each draw's vertex positions and UVs, remaps its bounded 16-bit indices
to buffer-local indices, and computes approximate vertex normals from triangle
winding. Source vertex positions are already world transformed by the
`SceneMesh` assembly path; this adapter applies no further transform.

It does not open BRES files or source APK assets, extract source materials, map
materials automatically, create a texture manager, or modify the app renderer.
Callers can provide a `TextureResolver` to map the source descriptor and its
sampler records to an `ITexture`; that callback's texture must remain valid for
the lifetime of the driver's mesh use. The adapter puts the returned texture
into Irrlicht layer 0. Multiple DH2 samplers/effects, opacity, shader
parameters, UV transforms, original vertex normals, lighting, and transparency
semantics are not reconstructed here. Normals are estimated and lighting is
disabled by default. This output is a static mesh snapshot even when its input
was assembled from a selected source animation pose.

## Prince source actor adapter

`PrinceActor` is the renderer-neutral rig/skin adapter used by the local SWAMP
diagnostic. It loads four source `_default_warrior-mesh-skin` controllers,
resolves each primitive's semantic position/color/UV slots through that
primitive's own `attributes[0]`, `attributes[2]`, and `attributes[4]` mapping,
and checks the corresponding weight/vertex counts. It retains immutable source
positions and deforms each pose through the checked engine-skinning palette.
`prince_mesh_adapter.cpp` copies the mutable positions into Irrlicht
`SMeshBuffer`s and marks vertex streams dirty after updates.

`PrinceCharacterRuntime` composes the shared source `character::Coordinator`,
the authored AnimationBank/table, and two-slot `actor::BlendedPlayback`. It
verifies and loads 116 registered resources and 158 ordered registration
occurrences, resolves KnightPlayerBase properties, and routes touch input
through recovered Idle/Move state updates. Registered assets with no serialized
animation payload remain registered without fabricated timing. SceneBinding
applies owner, helper, authored graph and root-displacement composition once;
the Irrlicht node stays at identity. `SwampActorSession` attaches its real
NativeWorld body to the source Character only after body creation. It sets
`Coordinator.state.body_present` while the backend owns that body, so source
Move focus calls unpin and Move blur calls Stop then pin. The Stop callback
clears the PF path, target, and heading before pinning. The session detaches
those callbacks and clears `body_present` before destroying the body/world.
Keep the `PrinceCharacterRuntime` wrapper at a stable address while the session
is attached; reloading it or binding a second body owner is rejected.

The SWAMP caller supplies a 20 ms development tick and a source owner position;
these are not claimed to reproduce the original game loop clock. Only Idle and
Move body transitions are supported. SWAMP props, other-module seams, walls,
AI, combat, script-blocked timers, and full game orchestration remain outside
this slice.

Read the [SWAMP source diagnostic](../swamp-smoke/README.md) for the package
scope, current APK verification state, build command, and remaining renderer
limits.

Run the host assertions (including per-primitive source stream equivalence):

```powershell
python port/irrlicht-android/game/tests/run_prince_host.py `
  --assets port/android-native/app/src/main/assets
```

The test stages the checked APK-format asset tree and writes JSON evidence
below ignored `port/irrlicht-android/build/prince-character-host-checks/`; no
Android device is used. For source actor body-service order, the actual-property
radius, +Y floor-boundary redirection, and callback teardown assertions, run:

```powershell
python port/irrlicht-android/game/tests/run_swamp_actor_session_host.py `
  --cache ..\cache\files `
  --output port\irrlicht-android\build\swamp-actor-body-services-host
```

The report records those host assertions separately from Android runtime
evidence.

## Provenance

The input ABI comes from `port/android-app/scene_buffers.hpp`. Its source data
is a bounded projection of checked BRES geometry and retains per-draw node,
geometry, material, and sampler identifiers. This module is new reconstruction
code; it does not claim to be part of DH2's recovered `glitch::` engine. It
uses the separate official Irrlicht OGL-ES branch import pinned at SVN r6038
(1.9.0 alpha) in `../upstream/`. The adapter and tests are outside that tree;
the upstream files remain covered by `../upstream-source-manifest.json`.

## Reproducible Android compile proof

From the repository root, with Android NDK r29 installed at the repository's
usual `work/emulator-test/sdk/ndk/29.0.14206865` path:

```powershell
python port/irrlicht-android/game/tests/compile_android.py
```

Set `ANDROID_NDK_HOME` or `ANDROID_NDK_ROOT` to override that location. The
script verifies the pinned header manifest, compiles this translation unit
for ARM64 and x86_64 Android API 26 against the r6038 headers, then links a
standalone shared library against each r6038 static archive. The generated
adapter libraries also receive 16 KiB `PT_LOAD` alignment checks. If Irrlicht
archives have not yet been built, run `python port/irrlicht-android/build.py
--no-apk` first. Objects, linked libraries, and the compile report are written
under ignored `game/build-compile/`. This command proves compile/link
compatibility; it does not run a renderer. The separate
[runtime smoke](../game-smoke/README.md) now exercises this adapter with a
synthetic pyramid on API 37/16 KiB, and the opt-in
[app-host diagnostic](../../android-app/IRRLICHT-HOST.md) does so on API
37/4 KiB. Those runtime fixtures are synthetic geometry, not imported DH2
assets. A separate cache-backed `void_maze` render is documented in the
[cache-scene smoke](../cache-scene-smoke/README.md); its texture/material
result verifies the `env_voidmaze.tga` mapping on its 30 referencing draws.
Full-scene composition and the other source materials and effect passes remain
unverified.

## Ownership and validation

`build_mesh` returns an `IMesh*` with its initial Irrlicht reference; the
caller releases it with `drop()`. The `SceneMesh` and its arrays remain
caller-owned and can be freed after the call. The adapter rejects absent
buffers, invalid capacities, non-finite vertex data, non-triangle index
counts, out-of-range draw spans, invalid texture spans, and indices that refer
outside their draw's own vertex span before allocating the Irrlicht mesh.
