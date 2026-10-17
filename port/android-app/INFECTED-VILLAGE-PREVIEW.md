# INFECTED VILLAGE static source preview

The development encounter now has an opt-in button for a static `INFECTED_VILLAGE_01`
source preview. It is a separate Activity that loads the original level MLX, both
module MGP/MVP pairs, their shared module BDAE, and the three resolved material
sampler textures. It draws each imported module subtree at the placement authored
by the MLX. Drag orbit and pitch are runtime-verified. Pinch zoom is implemented,
but was not runtime-verified because the headless ADB input harness did not
provide verified two-pointer injection; the on-screen hint is not test evidence.

The module roots are `_module_infectedvillage_01-node` at `(-3448.5, 3000, 0)`
and `_module_infectedvillage_02-node` at `(2551.5, 3000, 0)`. The imported source
objects are `infected01.mgp` + `infected01.mvp` and `infected02.mgp` +
`infected02.mvp`; both resolve to
`data/3d/modules/infectedvillage/infectedvillage.bdae`.

The importer retains all 20 source draws. The diagnostic preview filters four
draws by exact `(node id, material id)` provenance:

- `_module_infectedvillage_01-node` / `ColorMaterial` and
  `_module_infectedvillage_02-node` / `ColorMaterial`: one 36-index enclosing
  guide draw on each root. Rendering those source boxes as opaque fallback draws
  dominated the framing and obscured descendant geometry, so the diagnostic
  preview omits them from drawing and view bounds.
- `_floor_infectedvillage_01-node_PIVOT` / `Standard_8` and
  `_floor_infectedvillage_02-node_PIVOT` / `Standard_8`: diagnostic-only
  untextured fallback draws without sampler references.

The other 16 draws remain visible; other `ColorMaterial` or `Standard_8` draws
and any draw with a different node/material pair are not filtered. All 20 source
records, including the filtered records and floor/navigation data, remain in
the imported scene. These preview filters do not establish how the original
renderer treated any omitted node; native pass visibility is unresolved. A host
test checks the exact-match policy. Loading another source scene into an already
populated native `Preview` replaces and frees the prior preview before the new
import. EGL context recreation discards stale GL handles without deleting names
owned by the lost context, then re-creates the program and re-uploads scene data
on the next load.

## Reproducible inputs

`infected-village-asset-lock.json` is the finalized 23-file source bundle manifest
(SHA-256 `88e0c556c9c763bccb5e548c63c2a648f1e7da3b5671b84d53ce59c227156fd0`).
The Android builder reads the original unpacked cache, verifies each selected
source against that manifest's size and SHA-256, then packages only nine files:
the MLX, two MGP/MVP pairs, BDAE, and the exact Diffuse, Specular, and AlphaMap
texture paths listed by the manifest. The app reads those APK assets and verifies
the same hashes at runtime. The original cache is used only during the build.

The static material preview samples the three resolved textures with a small
preview shader. The original effect selection, lighting/pass state, animated fog
and decor scenes, and gameplay are not activated. The overlay lists the three
unbound sampler slots (`Material__2341` parameters 3 and 4; `Material__2342`
parameter 4), unresolved external effect/pass selection, omitted animated scenes,
and unresolved numeric dialog/text and music/audio resource IDs.

## Verification

Host import and authored-placement checks:

```powershell
python port/android-app/tests/run_infected_village_preview_host.py
python port/android-app/tests/test_infected_village_bundle.py
```

The host importer reports two roots, 50 imported MGP/MVP records, 20 draw
commands, 15,434 vertices, 18,702 indices, and nonempty Diffuse, AlphaMap, and
Specular source references. The renderer shows 16 of 20 draws and reports the
two root guides and two `Standard_8` fallback omissions in its overlay.
The exact current candidate is 5,465,698 bytes, targets API 37 (minimum API 26),
and has SHA-256
`ea9d0aef09125f0dac6b4cdb3d37727aae277e6bdabd2faf92b56e219d09583b`. On
Android 17/API 37 x86_64 emulators with both 4 KiB and 16 KiB pages, the current
APK passed three complete open/render/return cycles and a drag-orbit check. The
overlay reported all source omissions and unresolved samplers, installed APK
bytes matched the candidate, and filtered app/EGL/GLES error logs were empty.
Pinch zoom and physical-device behavior remain unverified. No Fold7 was tested.
The [current exact-APK report](infected-village-current-apk-runtime-validation.json)
lists screenshots and hashes; the [earlier candidate report](infected-village-preview-runtime-validation.json)
is retained as historical evidence.

The screenshots show static terrain/road, structures, and fences from both
authored module roots. Three exit-marker draws (`_exit_east_01-node`,
`_exit_west_01-node`, `_exit_west_02-node`, all `Standard_7`) remain simple
gray-green fallback geometry with no sampler references in the checked
bindings. Three other sampler slots are unbound (`Material__2341` parameters 3
and 4; `Material__2342` parameter 4); external effect/pass selection, animated
fog/decor, dialogs/text, audio, and music resources remain unresolved. The
render test makes no claim about native renderer visibility, and material or
lighting parity is not established.

This screen is a source-geometry preview only: no character movement, AI,
collision, scripts, triggers, transitions, or level gameplay are started.
