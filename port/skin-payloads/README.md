# Skin controllers and software bone palettes

Checked immutable views of the original Collada skin records and a software
position-skinning path. The owner must keep the complete, unrelocated BRES
bytes alive. This is a source component, not a complete character system.

## Recovered layout

Controller library records are 12 bytes: type, ID string, payload. Type 0 is
a skin; its 156-byte serialized payload contains:

| Offset | Exposed meaning |
| --- | --- |
| `0x00`, `0x04` | Inverse bind float count and matrix pointer; 16 floats per joint |
| `0x10` | Inline 16-float bind shape matrix |
| `0x70` | Source geometry URL |
| `0x74`, `0x78` | Joint count and joint scope-name pointer array |
| `0x7c`, `0x80` | Packed skin-data **word count** and buffer pointer |
| `0x98` | Weight component count, 1–4 in supported cache records |

Each vertex record contains four packed joint-index bytes followed by the
declared float weights. Stride is `(components + 1) * 4`. Zero-weight slots
may have unused indices; zero-weight vertices remain zero. The port does not
renormalize weights. Deferred data borrows bytes at the on-demand descriptor's
file offset. Per-joint weight lists, quaternion palettes, bounds, GPU buffers
and modular armour selection remain unimplemented.

`dh2_skin_open` checks strings, dimensions, widened ranges, finite affine
matrices, used joint indices, weights and a matching local type-0 mesh. It
returns an error for external or absent geometry. These are explicit port
types, not the original ARM object ABI.

The software palette is `joint_world * inverse_bind * bind_shape`, column
major. `dh2_skin_scene_palette` resolves the joint names against SNode's
scope-ID string at offset 8 within one visual, rejecting unresolved or
duplicate joints. Position skinning sums the stored weighted transforms.
Normals and animated node transforms are not implemented.

## Original instruction evidence

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The [software technique ARM assembly](../../recovered/native/assembly/libDungeonHunter2.so/glitch_collada_detail_CColladaSoftwareSkinTechnique-f7c7c7ba27ef-001.asm)
contains `preparePtrCache` at ELF `0x0066fab4`, `prepareCache` at
`0x0066fe34`, and `skin` at `0x0066ff48`. The common `initProxyBuffer` is
ELF `0x00670ec8`. These expose joint count/name offsets, 64-byte inverse
matrices, the inline bind matrix, packed index bytes and weight stride.
`CSceneNode::getScopeID`, ELF `0x0065cd4c`, exposes the serialized scope field.
Ghidra pseudocode addresses add `0x10000` to the ELF addresses. See
[functions-019](../../recovered/native/decompiled/libDungeonHunter2.so/functions-019.pseudo.c)
for constructor/accessor evidence and
[functions-020](../../recovered/native/decompiled/libDungeonHunter2.so/functions-020.pseudo.c)
for software preparation and skinning.

## Validation

[The cache audit](validation.json) covers 2,904 BRES files, 727 controllers in
386 files, 531 locally resolved skins, 4,319 joints, 104,486 vertices and
1,593 transformed position checks. There are 22 all-zero-weight vertices.
All 196 unsupported controllers occur in animation files needing geometry
outside the current file. Influence counts are 260 one-weight, 181 two-weight,
81 three-weight and 9 four-weight skins. Raw names, inverse floats and weights
are checked against cache bytes; transforms against an independent fixture
evaluator with binary32 rounding.

[The ARM comparison](arm-differential-validation.json) executes the original
ARM32 `prepareCache` routine in Unicorn with preallocated vectors and
already-resolved world matrices. It compares 304 palette matrices across
16 synthetic poses for each warrior and shadow controller. Output float bits
match compiled host source except signed zero. Stack restoration, cache flag
and output guards are checked. Imported float arithmetic and libc use the
host C oracle. This does not validate original bone lookup, allocation,
vertex skinning, GPU rendering, animation or gameplay.

[Safety checks](safety-validation.json) passed 5,000 corruption and actual
buffer truncation probes under AddressSanitizer and UndefinedBehaviorSanitizer,
plus capacity, alias and immutable-input checks.
[The build record](build-validation.json) covers the Linux host component.
The [Android app build](../android-app/build-validation.json) separately
compiles it for ARM64 and x86_64, with 16 KiB alignment.

The [Android runtime record](../android-app/character-runtime-validation.json)
shows the source app's first-controller fallback rendering a textured warrior
pose: 18 bones, 335 vertices, 1,092 indices, on Android 17 with both page sizes.
It is a stored pose with one texture and no animation or gameplay.

## Reproduce

On Linux with C++17, Python, Unicorn and pyelftools:

```sh
python3 port/skin-payloads/build.py
python3 port/skin-payloads/tests/audit.py --library port/skin-payloads/build/skin-host.so --cache PRIVATE_CACHE --report cache-report.json
c++ -std=c++17 -O1 -g -fsanitize=address,undefined -fno-fast-math -ffp-contract=off port/skin-payloads/skin.cpp port/asset-payloads/payloads.cpp port/engine-resources/resources.cpp port/scene-payloads/scene.cpp port/engine-math/math.cpp port/skin-payloads/tests/safety.cpp -o port/skin-payloads/build/safety
port/skin-payloads/build/safety PRIVATE_CACHE/data/3d/characters/prince/prince_low_poly_warrior.bdae
cc -shared -fPIC -O2 -fno-fast-math -ffp-contract=off port/engine-math/tests/fp_oracle.c -lm -o port/skin-payloads/build/oracle.so
python3 port/skin-payloads/tests/differential.py --original PRIVATE_ORIGINAL_SO --library port/skin-payloads/build/skin-host.so --oracle port/skin-payloads/build/oracle.so --sample PRIVATE_CACHE/data/3d/characters/prince/prince_low_poly_warrior.bdae --report arm-report.json
```

Original binaries and cache are private inputs. See [RIGHTS.md](../../RIGHTS.md);
this component does not establish a game-wide open-source licence.
