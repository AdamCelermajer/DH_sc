# Authored shader source ownership and GameSWF selection

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The 20 captured bodies are in `original-functions.json` and `reference/original-functions.asm`.
The new native files are `shader_sources.hpp/.cpp`. No shared renderer, CMake, staging, or packaged input was changed by this recovery.

## Exact cache inputs

Canonical ZIP: `C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip`, SHA256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
Its `com.gameloft.android.GAND.GloftD2SS/files/shaders.pak` is a 44,194-byte ordinary stored ZIP with 34 members, SHA256 `365a4d3c432454c44208ebb484c7a472a3a4534a0c5c9e77a7a90f3b87b1b5c0`.
`glsl.config` is zero bytes. Authored source bytes, including CR/CR/LF sequences, are preserved; the GLSL source body is not rewritten.
The sibling 660-byte `shaders.state` is SHA256 `5768ad4228363bf48cac0139db6b058538b11e0b9629f8586cbd6cf4fcb4325f`; its textual STATE values have not been interpreted as a substitute for the actual GL driver.

Scratch pack, state, material and extracted members are under `.local-inputs/fx-render-connection-discovery/`; the exact pack and material are also retained here as proof inputs.
GameSWF vertex source SHA256 `23d0c271fe19c3f305b44529879b18fab395be3416c2c8300605401f42c8bf24` (410 bytes), normal fragment `7465b4e89519dd371f779eef62048efd12ea9ced3d4e542606ffa0c867036889` (276 bytes), blend fragment `2038491cbcd7af3523e70c98d0ba72a7e904efa784acd7b878b835b60003b59c` (300 bytes).

## Source preamble, stage and cache name

`CGLSLShaderManager::createShaderCode` 0x6dfe68 calls `initAdditionalConfig` 0x6e0a3c only while manager+0x80 is -1. The latter really opens/reads a file via the filesystem service and replaces every caret byte with LF. A missing file leaves the sentinel and supports another attempt; the native pure config helper does not invent that owner/service or a once-only guard.

At 0x6dfff0–0x6e0098 it constructs these eight nonnull strings, followed by a null pointer:

1. `#define GLITCH_USE_HIGHP\n` iff driver+0x88 bit0x400, otherwise empty.
2. `#define GLITCH_USE_BIAS\n` iff bit0x800, otherwise empty.
3. `#define GLITCH_FORCE_USE_BIAS\n` iff bit0x1000, otherwise empty.
4. `#define GLITCH_OPENGLES_2\n`.
5. Current additional config or empty.
6. Caller preamble or empty.
7. One LF.
8. Exact file body, NUL terminated after an exact-length read.

The constructor 0x6df738 counts the eight pointers and individually deep-copies each C string, then `createShader` 0x6df560 calls `glShaderSource(handle,count,strings,NULL)`. Source type4 selects GL_VERTEX_SHADER0x8b31; every other value selects GL_FRAGMENT_SHADER0x8b30. `compileShader` 0x6df3c0 is actually executed in the oracle with explicit successful GL service results; no GPU compiler parity is claimed.

`makeShaderCodeName` 0x6e0c10/0x6e0b64 concatenates first, second, third, then current additional config with no separators. The createCode caller passes filename, empty, caller preamble. This key excludes stage and driver capability flags, preserving the original cache identity rather than improving it. The native helper owns strings but does not implement the complete original cache/refcount manager.

## Actual GameSWF selection and material state

The `render_handler_glitch` constructor 0x7d60f4 loads exact `gameswf_effects.bdae` and asks for material `_1_-_Default-fx` (0x7d6424–0x7d6460). Resource size3,752, SHA256 `8e9f5a3d59c2bac3d713f856a326e5556412f0bca1f2646feaa4be3a502895d9`.
Its original GLES2 `SProfileGLES2Traits::createShader` 0x634b30 was executed on all four actual serialized passes, observing the required shader-manager call. Results and raw authored render-state bytes are in `gameswf-material-probe.json`:

| Technique | Pass offset | VS | FS |
|---|---:|---|---|
| default | 0x974 | GameSWFVS.glsl | GameSWFFS.glsl |
| multiply | 0x9f4 | GameSWFVS.glsl | GameSWFFS_blend.glsl |
| screen | 0xa74 | GameSWFVS.glsl | GameSWFFS_blend.glsl |
| overlay | 0xaf4 | GameSWFVS.glsl | GameSWFFS_blend.glsl |

All vertex/fragment caller preambles are empty; entrypoint `main` fields exist but are not forwarded as preambles. Original program name is VS filename + VS preamble + FS filename + FS preamble. The serialized sampler binding is TextureSampler index0. Constructor registrations use modes0/1/15/16 default,3 multiply,4 screen,13 overlay. The constructor registration map is disassembly evidence, while four pass selections execute actual original instructions.

The authored normal shaders declare Position vec4, Color0 vec4, TexCoord0 vec2, WorldViewProjectionMatrix mat4, TextureSampler sampler2D, DiffuseColor vec4. Their output is `(texture2D + DiffuseColor) * Color0`; the blend fragment additionally multiplies RGB by final alpha. Thus a white Color0/zero DiffuseColor textured quad is an honest shader primitive acceptance, not complete authored menu/text acceptance.
The constructor's original vertex declarations use stride24, texcoord offset0 (two float components), color offset8 (four byte components), position offset12 (three float components). Source fill-style apply 0x7d6b54 produces coordinates and color before buffering; full SWF draw/cache/mask/transform/bitmap-color producers remain required for display-list parity.

Captured `CColladaFactory::createMaterial` 0x6323d0 requires genuine material renderer/root identities. `createMaterial` 0x631ce8 looks up each serialized parameter by name; typed/runtime capacity and conversion determine whether it is copied, rather than treating all material entries as diffuse. No missing factory/global material/light operation is accepted by this module. Actual render-state enum translation, full GL state ownership, shader uniform binding/caching and texture search aliases remain separate work. The current 3D preview shader must not be labeled this original material pipeline.

UI texture resolution is also a boundary: source `GameSWFUtils::SwfTextureLoader` 0x416c14 generically requests `data/<exportname>`; authored `menus/MenusGraphics_droid.tga` therefore needs actual texture-manager search/alias resolution, not an assumed basename rule. Font glyph providers eventually call source draw_bitmap0x7d8df4; the GameSWF shaders alone do not implement font selection/glyph geometry.

## Native interface, proof and integration

`ShaderSourcePlan` owns eight strings, cache name and projected GL type. `shader_source_plan`, `shader_code_name`, and `shader_config_text` validate bounded NUL-free port inputs and publish atomically. Source filesystem/config retry and cache owners are explicit callers. `ShaderSourcePack::load/find/members` owns exact members and validates central/local metadata, stored CRC, duplicate names and bounds. Stored ZIP decode is a bounded port container implementation, not original ZIP parser instruction parity; encryption/deflate/ZIP64/path names fail explicitly.

`shader_sources_differential.py`: 400 actual source-vector/stage cases,300 actual cache-name cases,160 original config rewrite cases; exact 860 original vs optimized ARM64 outputs. Original allocator runs against an explicitly initialized1MiB process arena; cache misses, file reads, base shader name/refcount owner, borrowed releases and GL results are services. The original source copies/array/GL stage selection execute, with glShaderSource observed. `shader_source-fixtures.bin` holds original-derived outputs.
`shader_sources_host.py`: actual isolated sanitized DSO replay of860, actual34-member pack, owned input/body retention,13 atomic malformed text/container guards, zero ASan/UBSan findings. No shared DSO or renderer proof is inferred.

Parent CMake integration: add `shader_sources.cpp` to scene-materials library; host target `tests/shader_sources.cpp` links that actual library and takes `reference/shader-sources/shader-source-fixtures.bin reference/shader-sources/shaders.pak`. No math wrappers required. Isolated reproductions: NDK `tools/build_shader_sources_oracle.ps1`, direct Python `tests/shader_sources_differential.py`, `tests/shader_material_probe.py`, `tests/shader_sources_host.py`.
Parent owns exact asset staging, real GLES compile/draw and visible acceptance report; this task performed no APK/ADB operation.
