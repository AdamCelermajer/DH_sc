> Imported static research from engine branch `e6da25b`. Current implementation and native wiring are tracked in the [branch audit](../../../docs/BRANCH-AUDIT-2026-10-05.md).

# Render-pass state to GLES calls

## Finding

The static engine path carries render-state bytes from a serialized Collada pass into the GLES2 material renderer and dispatches changed state groups to GLES calls. The trace now maps the blend, cull, depth, polygon-offset, sample-coverage, stencil, and selected non-grouped fields to call arguments and lookup values. It does not establish that a pass ran in gameplay.

The supplied APK has SHA-256 `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`; its `lib/armeabi-v7a/libDungeonHunter2.so` member has SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. [`ranges.json`](ranges.json) records 20 exact ELF function ranges. Selected ARM byte rows are in [`reference/pass-state-functions.asm`](reference/pass-state-functions.asm) and [`reference/pass-state-lifecycle-functions.asm`](reference/pass-state-lifecycle-functions.asm); the rows were checked against the corresponding APK ELF bytes.

## Static call path

The existing [BRES pass trace](https://github.com/Noamcelermajer/DH_sc/blob/e6da25b83086ea01fb1d4cd46ae12da3bbac645f/port/engine-materials/pass-record-route/ANALYSIS.md) establishes the file-backed material `+0x18` effect index, post-load conversion to `SEffect*`, and traversal to a 0x74-byte pass. In `createMaterialRendererForProfile<SProfileGLES2Traits>` at `0x006361e8` (2,436 bytes; SHA-256 `3669e766eac04456a732fe9a392b2df6d711dce39992a2e692ae7743fc339e3c`), the pass state block beginning at `pass+0x1c` is converted by `detail::renderpass::SRenderState::SRenderState(video::SRenderState const&)` at `0x005d7a10` (564 bytes; SHA-256 `e06e8f5ee8f6df566c1576414d31e3a4389e32d442da94250e6f49de3ba1c08f`). The renderer then passes the shader and state records to `CMaterialRendererManager::addRenderPass` at `0x005dcf2c` (100 bytes; SHA-256 `08b0332c8cb0e89140e6eaac4583286d8ac9ef8a2ec65d02bbc8f096b59b84f6`).

On material commit, `commitCurrentMaterialImpl` at `0x005b74e8` (172 bytes; SHA-256 `48645ce24c2397c05e3c19b90479ac583f1446cd90b971d91beb58180f3e03f9`) reaches `detail::applyRenderStates` at `0x005b73e0` (264 bytes; SHA-256 `4269a6555db594ac283ba023440c3614605684c4ee1413c0b8f6934734e731f4`). That code compares cached/current state and routes changed groups through `detail::apply<true,...>` at `0x005b71d8` (452 bytes; SHA-256 `207b3dd4641db243c26d51f67c94cfd0273aadcd15131f363f1f1b9bf4ef7847`) to the GLES2 helpers below. The selected state methods are cache-gated: this is a reachable static route, not evidence of runtime execution.

| State helper | ELF VA / bytes | Exact range SHA-256 | GLES operations reached |
| --- | ---: | --- | --- |
| Blend | `0x005af3f8` / 380 | `7fcb7764acdfffca0928b4e3634d8c49b954d5e8516e435c9dea1d8d90b8a5bf` | `glBlendEquation`, `glBlendFunc`, `glBlendColor` |
| Cull face | `0x005af654` / 100 | `159da0f483b2914007da91dd1af70617c9bc6deee97f618c7717ecac9b7629f1` | `glCullFace`; dispatcher handles enable/disable |
| Depth test | `0x005af6fc` / 100 | `218022e465873a05e084007822e453919d7daf45a148ec3084249f1a3daf383e` | `glDepthFunc`; dispatcher handles enable/disable |
| Polygon offset | `0x005af7a4` / 132 | `4221d371253df658855bc6630b0d6f748834db2b254c7a43822686c47621f5cc` | `glPolygonOffset`; dispatcher toggles polygon-offset fill |
| Sample coverage | `0x005af88c` / 108 | `4e121dcffca710c3622b4821eafd12695b55e77789f223eeb800e0c52c158bce` | `glSampleCoverage`; dispatcher handles enable/disable and invert cache |
| Stencil | `0x005afa78` / 212 | `c1f78a68ca53936f676445f815efd279a99b45eec6fea0c9b332d84a4f06f38b` | `glStencilFunc`, `glStencilOp`; dispatcher handles enable/disable |
| Non-grouped state | `0x005b70cc` / 268 | `493b9b879652b16e19864c66e6ab356902d5676f6d202b0868b306879f6d2d6d` | Includes `glFrontFace`, `glDepthMask`, and `glLineWidth` |

The GLES import names are also listed in [`../reference/gl-imports.json`](https://github.com/Noamcelermajer/DH_sc/blob/e6da25b83086ea01fb1d4cd46ae12da3bbac645f/port/engine-rendering/reference/gl-imports.json). The state helper ranges and directly read lookup-table bytes are recorded in this note's manifest and ARM excerpt. Khronos' [GLES 2.0 header](https://github.com/KhronosGroup/OpenGL-Registry/blob/main/api/GLES2/gl2.h) supplies the enum names; [EXT_blend_minmax](https://registry.khronos.org/OpenGL/extensions/EXT/EXT_blend_minmax.txt) defines the ES extension values used by two blend-equation entries.

## Field transport and GLES calls

The constructor receives `video::SRenderState` at the beginning of `pass+0x1c`; below, offsets in that source state are converted to BDAE pass offsets by adding `0x1c`. The dispatcher tests copied group flags before calling helpers, so these are static routes and field correspondences rather than proof of a draw.

| Serialized BDAE field | Static mapping | Call / gate |
| --- | --- | --- |
| `pass+0x1c`, low/high nibbles | Indexes source/destination blend-factor entries; low nibble is `sfactor`, high is `dfactor` | `glBlendFunc(sfactor, dfactor)` |
| `pass+0x24`, bits 12–14 | 3-bit equation-table index | `glBlendEquation(mode)` |
| `pass+0x30..+0x33` | Four bytes copied to packed color state and converted to float arguments | `glBlendColor(r,g,b,a)` |
| `pass+0x24`, bits 30–31 | 2-bit cull lookup index | `pass+0x28` bit 20 selects culling; helper calls `glCullFace(mode)` and enables `GL_CULL_FACE` on first use, while a clear selector makes the dispatcher disable it |
| `pass+0x28`, bits 12–14 | 3-bit depth comparison index | Bit 22 selects depth testing; helper calls `glDepthFunc(func)` and enables `GL_DEPTH_TEST` on first use, while a clear selector makes the dispatcher disable it |
| `pass+0x4c`, `pass+0x50` | Two float words copied from source state `+0x30`, `+0x34` | `glPolygonOffset(factor, units)` when either cached float changes; source bits 25–27 select the helper group, and packed bit 21 enables `GL_POLYGON_OFFSET_FILL`; when all three packed flags are clear the dispatcher disables polygon-offset fill |
| `pass+0x54` | Float copied from source state `+0x38` | `glSampleCoverage(value, invert)`; source bit 29 selects the group and bit 30 supplies `invert`; dispatcher disables `GL_SAMPLE_COVERAGE` when the group selector is clear |
| `pass+0x28`, low 3 bits | Stencil comparison-table index | With `pass+0x2c` bit 0 selecting stencil, byte `pass+0x1e` is `ref` and byte `pass+0x1f` is `mask`; helper calls `glStencilFunc(func,ref,mask)` |
| `pass+0x24`, bits 21–23, 24–26, 27–29 | Three stencil-op lookup indexes, passed in order | Stencil group calls `glStencilOp(fail,zfail,zpass)`; `pass+0x2c` bit 0 gates the group, and the dispatcher disables `GL_STENCIL_TEST` when clear |
| `pass+0x28` bit 21 | Front-face mode selector, with a conditional inversion from driver byte `+0x4a0` | `glFrontFace(mode)` |
| `pass+0x28` bit 23 | Boolean depth-write state | `glDepthMask(flag)` |
| `pass+0x44` | Float copied from source state `+0x28` | `glLineWidth(width)` when changed |
| `pass+0x48` | Float copied from source state `+0x2c` | Compared and cached in the helper, with no GLES call found there; point-size interpretation is not established |
| `pass+0x28` bits 12–13 and 14–15 | Cached in driver fields `+0x1e4` and `+0x1e8` | No GLES call found in this helper |
| `pass+0x28` bit 28 | Boolean alpha-to-coverage state | Directly toggles and caches `GL_SAMPLE_ALPHA_TO_COVERAGE` (`0x809e`) |

### Lookup values and bounds

The blend-factor table at ELF VA `0x008e003c` contains 15 entries: `0 GL_ZERO`, `1 GL_ONE`, `2 GL_SRC_COLOR`, `3 GL_ONE_MINUS_SRC_COLOR`, `4 GL_SRC_ALPHA`, `5 GL_ONE_MINUS_SRC_ALPHA`, `6 GL_DST_COLOR`, `7 GL_ONE_MINUS_DST_COLOR`, `8 GL_DST_ALPHA`, `9 GL_ONE_MINUS_DST_ALPHA`, `10 GL_CONSTANT_COLOR`, `11 GL_ONE_MINUS_CONSTANT_COLOR`, `12 GL_CONSTANT_ALPHA`, `13 GL_ONE_MINUS_CONSTANT_ALPHA`, and `14 GL_SRC_ALPHA_SATURATE`. These enum values match GLES2. `GL_SRC_ALPHA_SATURATE` is only valid in the source-factor position. The nibble allows index 15, for which no factor entry exists; because the adjacent word is the first equation value, an unchecked index 15 would read `0x8006`, which is not a blend factor. This is a lookup-boundary observation, not evidence that any asset contains that selector.

The five intended blend-equation entries at `0x008e0078` are `0 GL_FUNC_ADD`, `1 GL_FUNC_SUBTRACT`, `2 GL_FUNC_REVERSE_SUBTRACT`, `3 0x8007 (GL_MIN_EXT)`, and `4 0x8008 (GL_MAX_EXT)`. In GLES2, MIN/MAX require `EXT_blend_minmax` support. The helper extracts a 3-bit index without a bounds check: indices 5–7 read the adjacent cull entries `GL_BACK`, `GL_FRONT`, and `GL_FRONT_AND_BACK`, which are not valid blend equations. No asset-wide reachability conclusion is made.

The cull table at `0x008e008c` is `[GL_BACK, GL_FRONT, GL_FRONT_AND_BACK, GL_NEVER]`; the last value is not a valid `glCullFace` mode, though the 2-bit selector can address it. The depth and stencil comparison table at `0x008e0098` is the eight GLES comparison values from `GL_NEVER` through `GL_ALWAYS`. The stencil operation table at `0x008e00b8` is `[GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT]`. The front-face table at `0x008e00d8` is `[GL_CCW, GL_CW]` before the driver-state-dependent inversion. The byte-level values and ranges are in [`ranges.json`](ranges.json).

For the selected `chandelier_castle.bdae` record (asset SHA-256 `84098bbb6d5daac56fcd84c4a0e1248924a068da29bc436ac1f859d73997c9e8`, pass offset `0x3670`), `pass+0x1c = 0xff001111` yields factor indexes `1/1` (`GL_ONE/GL_ONE`), and `pass+0x24 = 0x001c0f00` yields equation index 0 (`GL_FUNC_ADD`) and cull index 0 (`GL_BACK`). `pass+0x28 = 0x01583007` yields depth comparison index 3 (`GL_LEQUAL`) and has both cull/depth group selector bits set. This is a static asset-to-table calculation only; it does not show that the pass was created, committed, or rendered.

## State cache and restore path

The state helpers cache capability booleans in the driver: blend `+0x1c4`, cull `+0x1c5`, depth `+0x1c6`, polygon-offset fill `+0x1cc`, alpha-to-coverage `+0x1d0`, sample coverage `+0x1d1` (with invert at `+0x1d2`), and stencil `+0x1d4`. The dispatcher turns the cull, depth, sample-coverage, and stencil groups off when their packed flags are clear; polygon-offset fill is turned off when packed bits 21–23 are all clear. The standalone `set*Enable(bool)` methods compare the requested state against these cache bytes, issue `glEnable`/`glDisable` on a change, then store the new boolean.

`restoreShadowState` at `0x005b6ccc` (340 bytes; SHA-256 `6aec5209b723ab86c7838b2644600ad77f1e36d80ea56468f2f6717b8b1bd730`) calls `restoreRenderState` at callsite `0x005b6cd8`. The restore helper at `0x005b69c0` (780 bytes; SHA-256 `28cebdcc6c60f27e79c4ee058bdc7064b99b7e13738f2789a6893812b2b0ba0a`) replays enable/disable and cached selector/value state, including blend, cull/depth, polygon offset, sample coverage, alpha coverage, stencil, and front-face inversion. The replay path reads but does not update those cache bytes.

In the inspected programmable-driver C1/C2 constructors, no writes to the cited cache bytes were found. The allocation and base-constructor path has not been traced far enough to establish their initial values or reset behavior after context recreation. These functions and the seven direct capability setters have exact hashes and byte-matched ARM rows in [`ranges.json`](ranges.json) and [`reference/pass-state-lifecycle-functions.asm`](reference/pass-state-lifecycle-functions.asm).

## Boundary

The sample's shader names and source prefixes are present in the recovered nested `shaders.pak`, but successful archive opening, shader compilation/linking, `beginTechnique`, and live drawing are not established. The pass-state path proves statically connected state-to-call routes and static table interpretations, not active gameplay use. Remaining open points include whether each selector value is reachable from real assets, polygon-offset line/point flag semantics, initial cache-byte values and context-recreation reset behavior, unexplained cached bits and point-size field, full renderer state integration, and runtime verification.

No builds, tests, or APK execution were run for this trace.
