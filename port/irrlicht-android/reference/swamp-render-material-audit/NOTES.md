# SWAMP AlphaMap source-to-Irrlicht audit

## Finding

The dark foliage cards are explained by two renderer mismatches on the 22
visible module-zero `Material__11611` draws. The source fragment shader takes
the mask from the **blue** channel and the current adapter copied the decoded
texture **alpha** channel instead. More importantly, the adapter assigned
Irrlicht's `EMT_TRANSPARENT_ALPHA_CHANNEL_REF` while the upstream GLES2
renderer registers that shader with the `EMT_SOLID` base material: it performs
no blending. The source material's serialized alpha reference is zero, so the
strict `Color.a < reference` condition rejects no mask texels. Transparent
pixels therefore write their diffuse RGB, and fractional mask values render
opaque.

The original shader sources were read from the supplied recovery archive but
are not copied here. This audit keeps their archive-member paths, sizes,
checksums and narrow contract observations only. It makes no new license claim
for recovered source material.

## Evidence

The original members are in
`dh2-reconstruction/recovered/assets/source-data/com.gameloft.android.GAND.GloftD2SS/files/shaders.pak.contents/`:

| Stage | Member | Bytes | SHA-256 |
| --- | --- | ---: | --- |
| Vertex | `GL_Diffuse_L1_iPhone_VS.glsl` | 6,633 | `1e04da1687b9171cac7aa694966f045bd16b04148a9a3a39b95d9ad4ad25b84c` |
| Fragment | `GL_Diffuse_L1_iPhone_FS.glsl` | 3,483 | `a39f0025f0605e3bc4e81c984e8cc9b110639bb5901b2cf9b0798d65871e2686` |

The inspected recovery ZIP SHA-256 is
`b3ff974e2b74f50387465d5665f60d56ac79c29a449c6299745461998045c4d8`.
The scene cache BRES is `89da80c60a7ebecd0e8a27a9d46f2625e3a5ec112933e5aa7cab251412b8364d`
and the external effect BRES is
`10c64054906caf1683f3669becf415fe20196491ac5f182881cb69705d080bdc`.

The relevant source contract is:

- Diffuse and AlphaMap sample the same `texcoord0` UV. The checked BRES mesh
  reader resolves both materials' UV slot 4 to attribute ID 3, two float
  components. A host probe examines indexed vertices from every draw: 25
  `Material__11610` and 22 `Material__11611` draws.
- The fragment source replaces diffuse alpha with AlphaSampler's blue channel.
- The `AL` variant retains that fractional alpha. The separate `AT` variant
  discards values strictly below `0.8`.
- `Material__11611` binds `Diffuse=env_swamp.tga` and
  `AlphaMap=pvr2_env_swamp_alpha.tga`. `Material__11610` has the same diffuse
  but an unbound AlphaMap and remains on the opaque diffuse path.
- Both material records serialize `alpha-ref=0` and
  `Multilight-fx-profile_GLES2/CurrentTechnique` as type code 20. The checked
  local reader does not decode that opaque current-technique selection, so the
  exact selected `AL` versus `AT` variant is not proven. The external effect
  contains both named variants; that is not sufficient to claim which was
  selected for these draws.
- The shader body does not establish the GL blend equation/factors or depth
  write setting. Do not report those as recovered source state.

The cache AlphaMap is a 1024x1024 PVRTC 2bpp texture
(`928622d0ef735d0cda4120b5bbe66e2d58e56d81cd7646579a23f683c20e76a0`). The
checked local decoder produces RGBA8. Blue differs from decoded alpha at
124,368 of 1,048,576 texels; the maximum absolute difference is 35 and the
95th-percentile difference is 4. In particular, 382 texels have blue zero but
nonzero alpha, and 261 have nonzero blue but alpha zero. Copying alpha instead
of blue is not byte-equivalent to the original shader input.

Irrlicht r6038's `COGLES2TransparentAlphaChannelRef.fsh` uses a strict alpha
comparison and receives `SMaterial::MaterialTypeParam`; its default is zero.
`COGLES2Driver.cpp` registers this reference shader with base `EMT_SOLID`, so
the material does not enable GL blending. In contrast,
`COGLES2MaterialRenderer.cpp` enables `GL_SRC_ALPHA,
GL_ONE_MINUS_SRC_ALPHA` and marks `EMT_TRANSPARENT_ALPHA_CHANNEL` as
transparent. With `ZWriteEnable=EZW_AUTO`, `CNullDriver::getWriteZBuffer`
suppresses depth writes for that transparent pass by default. This documents
Irrlicht's chosen preview behavior, not a recovered DH2 GL state.

## Bounded correction

Use the recovered `AL` shader output semantics for a clearly labeled preview
of the 22 resolved AlphaMap draws: copy decoded blue into the diffuse alpha
channel, then let Irrlicht's `EMT_TRANSPARENT_ALPHA_CHANNEL` blend the
fractional result. Keep `EZW_AUTO` so the upstream transparent pass does not
write alpha-blended foliage depth by default. Leave `Material__11610` and
unresolved AlphaMap refs unchanged.

This does **not** prove the game selected `AL` over `AT`, reconstruct the
shader's light/specular/fog/material inputs, or recover source blend/depth
state. The app must call this an `AL` preview and keep the `AT <0.8` branch
explicitly unresolved until the type-20 effect selection is decoded.

## Reproduction

The host probe and JSON report can be regenerated from the supplied local
cache and recovery archive (no shader bodies are exported):

```powershell
python port/irrlicht-android/reference/swamp-render-material-audit/run_audit.py `
  --cache ..\cache\files `
  --recovery-archive ..\Dungeon-Hunter-2-Source-Recovery.zip
```

By default the ignored report is written to
`port/irrlicht-android/build/swamp-render-material-audit/swamp-render-material-audit.json`.
The repository evidence contains only this note, the source probe, and the
reproducible audit runner; generated binaries, decoded texture previews, and
machine-local cache copies stay under ignored build/work directories.
