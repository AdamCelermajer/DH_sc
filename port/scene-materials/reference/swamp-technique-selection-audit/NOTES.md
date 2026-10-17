# SWAMP `CurrentTechnique` source selection audit

## Result

The material's serialized type-20 field resolves the shader-variant name. The
original material constructor passes that name to
`CMaterialRenderer::getTechniqueID` and stores the returned byte in the
material's active-technique field. For `Material__11611`, both GLES profiles
name an `Al` variant and neither names an `At` variant. The exact selector name
is present in both named technique tables in the external `Multilight-fx`
effect. This changes the earlier “AL versus AT unresolved” result: the source
material selector requests AL for both profiles.

This establishes the source selector and its matching name records. It does
not prove which effect group or profile the original device selected at
runtime, that the corresponding shader compiled successfully, or what GL
blend/depth state was applied. No blend or depth behavior is inferred from
`Al` or `At`.

## Serialized BRES fields

The scene input is `data/3d/modules/swamp/swamp.bdae` (5,585,024 bytes,
SHA-256 `89da80c60a7ebecd0e8a27a9d46f2625e3a5ec112933e5aa7cab251412b8364d`).
The external effect input is
`data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae` (18,792 bytes, SHA-256
`10c64054906caf1683f3669becf415fe20196491ac5f182881cb69705d080bdc`). The
MLX is `data/scene/001_swamp.mlx` (6,788 bytes, SHA-256
`e3694600f07184567efcdecb79826da8bfe7aa09b7f1505b1bc13eb1eb68a041`).

A material `CurrentTechnique` parameter uses the observed 24-byte material
parameter record: `+8` is type code 20, `+12` is scalar count 1, `+16/+20`
point to the checked count header and type-specific value bytes. The first two
value words are a raw tag and a selector-string BRES offset. The reader keeps
the tag uninterpreted and bounds-checks the string; it does not treat file
offsets as native pointers. The source selectors are:

| Material | Profile | Raw tag | Selector string offset | Selector | Effect-group named-record ordinal |
| --- | --- | ---: | ---: | --- | ---: |
| `Material__11610` | GLES | 1 | `0x88fa8` | `L1_Vc_----_----_----_----_----` | 0 |
| `Material__11610` | GLES2 | 2 | `0x89000` | `L1_Vc_----_Sp_----_----_----` | 1 |
| `Material__11611` | GLES | 1 | `0x89038` | `L1_Vc_Al_----_----_----_----` | 2 |
| `Material__11611` | GLES2 | 2 | `0x8905c` | `L1_Vc_Al_Sp_----_----_----` | 4 |

The BRES external-effect filename on both material records is
`GL_Diffuse_L1_VC_iPhone.bdae`, with effect URL `#Multilight-fx`. The cache
asset used by this audit is the same effect BRES under the cache's lowercase
path above; filename search and case-normalization policy are outside this
decoder.

The checked effect view exposes two groups, each with 24 12-byte named records.
Their first words resolve to technique names; the remaining words stay opaque.
Both groups have the same ordered name list. In each list, the no-specular AL
name is at record 2 and the AL+specular name is at record 4; the paired AT
names are at records 3 and 5. `Al` and `At` are therefore distinguished by
actual effect names rather than by a guessed variant rule. The group-to-profile
mapping is not established, but both groups resolve the exact material
selector names.

## Original ARM selection and active-byte behavior

The supplied `libDungeonHunter2.so` has SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Instruction evidence is preserved in the recovered assembly views:

- `glitch_collada-f9638587a0e2-001.asm`, `createMaterial` at `0x00631ce8`:
  at `0x00631e64` it checks parameter type `0x14` (20); at `0x00631e70` it
  reads the runtime value-data pointer; at `0x00631e84` it passes value-data
  `+4` to `getTechniqueID`; at `0x00631e88` it calls the method; and at
  `0x00631e98` it stores the non-`0xff` result at material byte `+8`.
- `glitch_video_CMaterialRenderer-a2aa4bc66635-001.asm`,
  `CMaterialRenderer::getTechniqueID` at `0x005d4714`: it interns the input
  string, scans the renderer's 12-byte name entries from `+0x18` using the
  byte count at `+0x10`, returns the first matching zero-based byte ordinal,
  and returns `0xff` when no name matches. A `0xff` result is not stored by
  `createMaterial`.
- `glitch_video_CMaterial-938c40b685b3-001.asm`, `CMaterial::getTechnique`
  at `0x005c5d34`: it reads active byte `+8` and profile byte `+0x14` while
  accessing the selected profile/renderer data. This confirms later use of
  the active byte; the full profile/pass dispatch remains outside this audit.

The local recovered-assembly file hashes are `29a3972f7e8ff323647f53cb67a255ffc072d6e8c3dd2fb987af6c64eeff89d9`
for the `createMaterial` listing, `96a9fdb420fa978b925a970090d265849068d2d075a91f1782f53ece21093f35`
for the `CMaterialRenderer` listing, and `04bc6a0ab651bd2449cbbfe0b9a16bfdc64ff0b280e971052779b2c287db1a12`
for the `CMaterial` listing.

The host decoder reproduces exact ordered-name lookup (first match, one-byte
ordinal, absent `0xff`) as a standalone test. An ordinal from the serialized
effect group is labeled as a **named-record ordinal**, not asserted to be the
runtime renderer ordinal: original renderer construction may omit techniques
that fail to compile, and that compiled table was not recreated here.

## Shader and renderer limits

The recovered fragment shader member
`GL_Diffuse_L1_iPhone_FS.glsl` is 3,483 bytes with SHA-256
`a39f0025f0605e3bc4e81c984e8cc9b110639bb5901b2cf9b0798d65871e2686`. It uses
the alpha-mask blue channel; its AL branch keeps fractional alpha, while its
separate AT branch discards values strictly below `0.8`. The source hash and
archive path are recorded here without copying the recovered shader body.

The source selector resolves `Material__11611` to AL-named techniques in both
profiles. This does not recover the original `SRenderState`/GL blend factors,
blend equation, depth test or depth-write settings. The current Irrlicht
AL-preview remains a bounded rendering approximation until those independent
states are traced. The current APK and its earlier conservative report remain
unchanged by this host-only selector work.

## Nine-module check and reproduction

The checked MLX selects nine SWAMP module roots, all referencing the same
`swamp.bdae` catalogue. The host test assembles each module's visible draws
from its own root and checks their material indices against that catalogue.
Across the nine roots it sees 189 `Material__11610` draws and 188
`Material__11611` draws. Module zero separately reproduces 25 and 22 draws,
respectively. All 188 visible AlphaMap draws point to the source material
whose GLES/GLES2 selectors resolve above.

The maintained checked reader is [`technique_selector.hpp`](../../technique_selector.hpp)
and [`technique_selector.cpp`](../../technique_selector.cpp). It is not wired
into the frozen APK or the AL-preview policy; a future renderer build can use
its typed profile/name output instead of hardcoding a material variant.

Run the strict host gate with the supplied cache:

```powershell
python port/scene-materials/tests/run_technique_selector_host.py `
  --cache C:\path\to\private-cache\files `
  --output port/irrlicht-android/build/swamp-technique-selector-host
```

The gate compiles with C++17, `-Wall -Wextra -Werror`, `-fno-fast-math` and
`-ffp-contract=off`; it validates malformed type/count/range/profile inputs,
missing-selector `0xff`, first-match ordering, unrepresentable table size,
both material profiles, both effect groups, and the all-nine-module draw map.
Its JSON, executable and source/cache hashes remain in the ignored build
directory. This is host/source-record verification only: no APK, emulator,
shader execution, GPU output or gameplay run was part of this audit.
