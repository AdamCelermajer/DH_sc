# SWAMP effect pass state conversion

## Result

The original `video::SRenderState` to `renderpass::SRenderState` conversion is
now reconstructed from the pinned ARM instructions. The source object is 76
bytes; the produced render-pass value is 32 bytes. The maintained reader
[`source_state_conversion.cpp`](../../source_state_conversion.cpp) reads
little-endian bytes explicitly and never overlays either proprietary C++ ABI.

The checked scene BRES resolves `Material__11611` to external effect file
`GL_Diffuse_L1_VC_iPhone.bdae` and effect URL `#Multilight-fx`. Its GLES2
`CurrentTechnique` value selects `L1_Vc_Al_Sp_----_----_----`, which occurs at
**serialized group 1 named-table ordinal 4**. Its named record is at BRES
offset `0x30ac`, its one-pass payload
starts at `0x3498`, and the pass's 76-byte state source starts at `0x34b4`.
The BRES is 18,792 bytes, SHA-256
`10c64054906caf1683f3669becf415fe20196491ac5f182881cb69705d080bdc`.
The original ELF is `libDungeonHunter2.so`, SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

The original copy constructor at ELF `0x005d7a10` (564 bytes) was executed
against the exact ordinal-4 source object. The 76 input bytes are also
byte-identical to the ordinal-2 `L1_Vc_Al_----_----_----_----` source, and the
original instruction result is:

```text
5400ff1807000b00ffffffff0000803f0000803f00000000000000000000803f
```

This corresponds to packed words `0x18ff0054` and `0x000b0007`, followed by
the copied source fields. The constructor input/output fixture is also stored
in [`manifest.json`](manifest.json); the original execution reports are in the
local ignored `port/scene-materials/build/render-state-snapshot-root/`
directory.
The raw 564-byte function body is pinned by SHA-256
`e06e8f5ee8f6df566c1576414d31e3a4389e32d442da94250e6f49de3ba1c08f`.

## Reconstructed field mapping

The mapping below follows the instruction listing for the copy constructor in
[`glitch_video_detail_renderpass_SRenderState-2777ca965cb6-001.asm`](../../../../recovered/native/assembly/libDungeonHunter2.so/glitch_video_detail_renderpass_SRenderState-2777ca965cb6-001.asm).
Its ELF address and size are in the manifest. In particular, source word
`+0x0c` bit 24 is skipped, and source word `+0x10` bit 0 becomes output flag
bit 27; this is why the implementation uses an explicit mapping instead of a
bulk copy or a guessed struct conversion.

| Render-pass output | Original video-state input |
| --- | --- |
| Packed word `+0` bits 0–7, 8–15, 16–23 | bytes `+0`, `+2`, `+3` |
| Packed word `+0` bits 24–26 | word `+0x08` bits 12–14 |
| Packed word `+0` bits 27–29 | word `+0x0c` bits 12–14 |
| Packed word `+0` bits 30–31 | word `+0x08` bits 30–31 |
| Flags word `+4` bits 0–11 | word `+0x08` bits 18–29 |
| Flags word `+4` bits 12–15 | word `+0x0c` bits 15–18 |
| Flags word `+4` bits 16–20 | word `+0x0c` bits 19–23 |
| Flags word `+4` bits 21–26 | word `+0x0c` bits 25–30 |
| Flags word `+4` bit 27 | word `+0x10` bit 0 |
| Bytes `+8..+11` | bytes `+0x14..+0x17` |
| Words `+0x0c..+0x1c` | words `+0x28,+0x2c,+0x30,+0x34,+0x38` |

The copy constructor's name and register-level source layout are verified.
The GLES2 renderer factory consumes a pass record with stride `0x74` and
copies the state at pass offset `+0x1c`. This matches the BRES source span at
`0x34b4`. The association is reported as a GLES2-shaped payload and matching
selector name; this does not prove which profile or compiled technique the
original device selected at runtime.

## Source GL state

The original pass-state lookup table at ELF `0x008e003c` was read directly and
verified against the Android GLES header. Therefore factor ordinals 4 and 5
mean **`GL_SRC_ALPHA` and `GL_ONE_MINUS_SRC_ALPHA`**. The ordinal-4 AL+specular
state converts as follows:

The first ten numeric lookup entries are `0x0000, 0x0001, 0x0300, 0x0301,
0x0302, 0x0303, 0x0306, 0x0307, 0x0304, 0x0305`. The GLES header assigns
`0x0302/0x0303` to source alpha/inverse source alpha, `0x0304/0x0305` to
destination alpha/inverse destination alpha, and `0x0306/0x0307` to
destination color/inverse destination color.

| State | Decoded value |
| --- | --- |
| Blend enabled | yes |
| Blend source factor | ordinal 4, `GL_SRC_ALPHA` (`0x0302`) |
| Blend destination factor | ordinal 5, `GL_ONE_MINUS_SRC_ALPHA` (`0x0303`) |
| Blend equation | ordinal 0, `GL_FUNC_ADD` (`0x8006`) |
| Depth test | enabled |
| Depth function | ordinal 3, `GL_LEQUAL` (`0x0203`) |
| Depth write | disabled |

The 32-byte snapshot decoder gets the GL enum values from the original ELF's
lookup tables. The alpha mask's AL shader behavior is separate evidence; it
does not substitute for these render-state values. Also, the selector name
and serialized BRES named-table ordinal do not establish runtime profile
selection or prove that the shader compiled on a device.

## Host validation and original-instruction differential

Run the strict host check with the private cache's effect BRES:

```powershell
python port/scene-materials/tests/run_source_state_conversion_host.py `
  --effect-bres C:\path\to\cache\files\data\gfx\effects\gl_diffuse_l1_vc_iphone.bdae `
  --scene-bres C:\path\to\cache\files\data\3d\modules\swamp\swamp.bdae `
  --output port/scene-materials/build/source-state-conversion-host
```

The host check bounds both BRES inputs, resolves the serialized material
selectors for GLES and GLES2, checks group 1 ordinals 2 and 4 and their payloads,
verifies their 76 state bytes match, runs the converter, checks its result
against the actual original ordinal-4 ctor fixture, and decodes the resulting
32-byte snapshot. It also emits 36 original
instruction comparison inputs in ignored build output:
`original-arm-differential-inputs.json`. The original instructions were run on
all 36; every result matched. The tracked
[`original-instruction-differential.json`](original-instruction-differential.json)
records the original ELF/function byte hashes, input hash, each input/output
pair, and the zero-mismatch result. Cases 00–03 are designed inputs
(zeroes, all-`ff`, the selected AL+Sp state, and a byte ramp); cases 04–35 use
the fixed xorshift seed `0x5d7a10c1`. The JSON's `expected_pass_hex` values are
the candidate outputs which the original instruction executor independently
matched byte-for-byte. The maintained host integration runner checks that
tracked proof's input hash, function identity/body hash, all 36 cases, and
zero mismatch count.

This work establishes source-object conversion and values. It does not claim
original GPU rendering parity or change the frozen Android APK.
