# Original SWAMP material pass-state audit

## Result

The source chain from a material's `CurrentTechnique` selector to the original
GL pass-state calls is now mapped. The two inputs remain distinct:

1. `Material__11611` requests an `Al`-named technique in both material
   profiles. The checked effect reader resolves the selector names in the
   serialized `Multilight-fx` named tables.
2. The original Collada renderer factory constructs profile-specific pass
   objects, copies each pass's 32-byte `renderpass::SRenderState` into the
   material renderer, and applies that stored state when drawing the selected
   profile/technique/pass.

The external effect BRES reader currently preserves the AL record's payload as
opaque data. It has not established the exact in-memory pass snapshot that the
profile factory reads. Therefore the AL pass's blend enable, source and
destination factors, equation, depth test/function, and depth write remain
unresolved. The name `Al`, the shader's fractional-alpha behavior, and the
default constructor are not evidence for those values.

Function symbols, addresses, declared ARM sizes, and hashes for the recovered
ELF and assembly dependencies are recorded in
[`original-functions.json`](original-functions.json). The maintained C++
decoder is intentionally only the 32-byte pass-snapshot boundary; it does not
consume BRES technique payloads or attempt to recreate original renderer
construction.

## Pass-state representation recovered from the ELF

The pass type is
`glitch::video::detail::renderpass::SRenderState`, a 32-byte value containing
eight little-endian 32-bit words. The constructor at ELF address `0x00588ddc`
stores:

| Offset | Default | Meaning established from the serializer/apply path |
| ---: | ---: | --- |
| `+0` | `0x18ff0001` | Packed blend factors/equation, depth compare, and cull face |
| `+4` | `0x00180007` | Feature flags and other packed state bits |
| `+8` | `0` | Four byte components of `BlendColor` |
| `+12` | `1.0f` | Line width |
| `+16` | `1.0f` | Point size |
| `+20` | `0.0f` | Polygon offset factor |
| `+24` | `1.0f` | Polygon offset units |
| `+28` | `1.0f` | Sample coverage value |

The low fields of word 0 are:

| Bits | Source field |
| ---: | --- |
| `0..3` | `BlendFactorSrc` enum ordinal |
| `4..7` | `BlendFactorDest` enum ordinal |
| `24..26` | `BlendEquation` enum ordinal |
| `27..29` | `DepthFunc` enum ordinal |
| `30..31` | `CullFace` enum ordinal |

In word 1, bit 16 is `BlendEnable`, bit 17 is `CullFaceEnable`, bit 18 is
`FrontFace`, bit 19 is `DepthTestEnable`, and bit 20 is `DepthMask` (depth
write enable). The names and bit positions are present in the original
`serializeAttributes`/`deserializeAttributes` implementations. The pass
`apply` path dispatches blend and depth-test state only when their enable bits
are set; it separately sends the depth-mask bit to `glDepthMask`.

The constructor default decodes as blend disabled; depth testing and depth
writes enabled; source factor `GL_ONE`; destination factor `GL_ZERO`; blend
equation `GL_FUNC_ADD`; and compare function `GL_LEQUAL`. This is only the
constructor value. A serialized pass can override it before insertion, so the
constructor must not be substituted for the missing AL record.

The pinned ELF's static maps are directly inspectable. `BlendFactorMap` at
`0x008e003c` has 15 entries, `BlendEquationMap` at `0x008e0078` has 5, and
`CompareFuncMap` at `0x008e0098` has 8. Their numeric GLES values are preserved
in [`render_state_snapshot.cpp`](../../render_state_snapshot.cpp). The decoder
accepts exactly 32 input bytes, reads words explicitly as little-endian, keeps
all raw words, and rejects out-of-range blend-factor/equation ordinals. It
does not cast serialized data to a native struct or treat source offsets as
pointers.

## State selection and storage chain

- Original `createMaterial` recognizes serialized material parameter type 20
  (`CurrentTechnique`), resolves its name with
  `CMaterialRenderer::getTechniqueID`, then writes the resulting byte ordinal
  to the material's active-technique byte at `+8`.
- `CMaterial::getTechnique` reads that active byte and the material's profile
  byte at `+0x14`. The full profile-selection conditions are not inferred from
  the shader variant name.
- `createMaterialRendererForProfile<SProfileNullTraits>` and
  `createMaterialRendererForProfile<SProfileGLES2Traits>` walk the selected
  effect/profile's technique list and its passes. The GLES path constructs
  pass state from the `SPass<SRenderStatesGLES>` record at source-object offset
  `+0x8c`; the GLES2 path uses the
  `SPass<SRenderStatesProgrammable>` record at `+0x1c`. Both pass the copied
  state to `CMaterialRendererManager::addRenderPass`.
- `SCreationState::addRenderPass` copies eight words (32 bytes) into the
  stored `SRenderPass` entry. `CMaterialRenderer::setRenderState(profile,
  pass, state)` locates the selected profile's pass table, compares exactly 32
  bytes, copies changed state, and marks the pass dirty.
- `detail::applyRenderStates` chooses the renderer profile, active technique,
  and pass. `detail::apply<true, renderpass::SRenderState>` reads the packed
  fields and calls `glBlendEquation`, `glBlendFunc`, `glDepthFunc`, and
  `glDepthMask` as enabled by that stored state.

This proves where an AL state value would come from and how it reaches GLES.
It does not decode the raw effect-BRES payload into either profile's in-memory
pass record. The checked BRES APIs currently expose the 12-byte named record
table, 24-byte parameter table, and image references. The named-record third
word remains an opaque payload reference. There is no validated adapter yet
from that serialized payload to the `+0x8c` or `+0x1c` pass-state source.

## SWAMP AL selector context

The scene BRES is `data/3d/modules/swamp/swamp.bdae` (5,585,024 bytes,
SHA-256 `89da80c60a7ebecd0e8a27a9d46f2625e3a5ec112933e5aa7cab251412b8364d`).
Its external effect is
`data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae` (18,792 bytes, SHA-256
`10c64054906caf1683f3669becf415fe20196491ac5f182881cb69705d080bdc`).
`Material__11611` maps to effect named-record ordinals 2 and 4; both names
contain `Al` and do not contain `At`. The group/profile runtime mapping and
compiled renderer ordinal remain separate from this serialized named-table
ordinal. See the earlier
[`CurrentTechnique audit`](../swamp-technique-selection-audit/NOTES.md) for
the bounded selector evidence.

The recovered source fragment shader's AL branch retains fractional alpha;
the AT branch applies a strict discard threshold at `0.8`. This describes
shader behavior, not blend/depth state. The shader body is not copied here.

## Evidence inputs

The original ARM ELF used for direct static-map inspection is
`libDungeonHunter2.so`, 15,938,284 bytes, SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

Relevant recovered assembly listings and hashes:

| Listing | Covered function(s) | SHA-256 |
| --- | --- | --- |
| `glitch_video_detail_renderpass_SRenderState-2777ca965cb6-001.asm` | Pass-state default/copy constructors and attribute state fields | `632fa9d145d4f5b0481b5eb1df1c7a997675ec04bde8aaaa44a00d3a8bbf2f07` |
| `glitch_video_CMaterialRenderer-a2aa4bc66635-001.asm` | Name lookup and `CMaterialRenderer::setRenderState` | `96a9fdb420fa978b925a970090d265849068d2d075a91f1782f53ece21093f35` |
| `glitch_video_CMaterialRendererManager_SCreationState-ec75b4328515-001.asm` | Per-profile/pass `SCreationState::addRenderPass` storage | `ea3063b7111f48580388aa004852803dc957863ad23dda66a6ab76cc44251859` |
| `boost_intrusive_ptr_glitch_video_CMaterialRenderer_glitch_collada-103a5e550bf3-001.asm` | GLES and GLES2 profile pass-state source offsets | `8dadd55679a3ca84c39281473c91f6cdc81c36647ae282cceb412c5d9963d129` |
| `void_glitch_video_CCommonGLDriver_glitch_video_CProgrammableGLDriver_glitch_video_CGLSLSha-8379cfb2ad39-001.asm` | Packed-state to GL application | `ed7ad4f18deb60eb84e9ced0d4887f7979b2de37010f6d745e30e426747d18e4` |
| `glitch_video_CMaterial-938c40b685b3-001.asm` | Active technique/profile access | `04bc6a0ab651bd2449cbbfe0b9a16bfdc64ff0b280e971052779b2c287db1a12` |
| `glitch_collada-f9638587a0e2-001.asm` | Type-20 selector to active technique byte | `29a3972f7e8ff323647f53cb67a255ffc072d6e8c3dd2fb987af6c64eeff89d9` |

## Host verification

Run:

```powershell
python port/scene-materials/tests/run_render_state_snapshot_host.py `
  --output port/scene-materials/build/render-state-snapshot-host
```

The test checks the exact constructor byte fixture, depth/blend flags, a
synthetic ONE/ONE + ADD + LEQUAL input vector, all 15 blend-factor entries, all
5 blend-equation entries, all 8 depth-compare entries, length/null rejection,
and invalid blend/equation ordinals. The synthetic vector is a decoder test
only; it is not a recovered `Material__11611` pass. The ignored output's
`validation.json` records source hashes and the executable hash.
This helper is not wired into the Android APK. It is a source-snapshot decoder,
not an end-to-end BRES-to-pass-state reconstruction or device rendering test.
