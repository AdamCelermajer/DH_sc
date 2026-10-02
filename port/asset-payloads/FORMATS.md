# Verified nested BRES layouts

Offsets below are decimal byte offsets within each ARM32 serialized record. Words are little-endian. Top-level references use the file-relative offsets tracked by the BRES fixup table; they are not native C++ pointers. The port never casts the bytes to widened structs.

## Geometry

The root's geometry count/pointer are at `0x68` / `0x6c`; records occupy 16 bytes. `SGeometry` stores ID/name string offsets at 0/4, a type at 8, and a payload offset at 12. The original `constructGeometry` normal-mesh branch accepts type 0. This cache contains 10,924 type-0 and nine type-1 records.

All nine type-1 records are named `Circle01-spline` or `Line01-spline`. Their payload begins with five 32-bit words, observed as `[0, 15, 3, 0, 0]` in every file. Their meanings are unproven, so `Type1Geometry` exposes them as opaque words. A mesh-shaped record begins at payload+20 and uses the same 44-byte `SMesh`, stream, attribute, primitive, vertex and index layouts below. The separate `dh2_type1_geometry_open` reads that embedded record; `dh2_mesh_open` continues to reject type 1 because the original `constructGeometry` returns null for it. This is a checked asset view, not evidence that the original game draws these records as meshes or interprets the prefix as spline control data.

| `SMesh` offset | Field |
| ---: | --- |
| 0 | Stream mode; 1 is interleaved in all recovered type-0 meshes |
| 4 | Vertex count |
| 8 | Stream descriptor offset |
| 12 | Primitive buffer count |
| 16 | Primitive buffer array offset, stride 56 |
| 20 / 32 | Minimum / maximum float XYZ bounds |

| Interleaved descriptor offset | Field |
| ---: | --- |
| 0 | Vertex stride, read as unsigned 16-bit by the original |
| 4 / 8 | Count / offset of attribute byte-offset array |
| 12 / 16 | Count / offset of attribute type array |
| 20 / 24 | Count / offset of component-count array |
| 28 / 32 | Count / offset of attribute bounding-box reference array |
| 36 | Vertex bytes or on-demand record offset |
| 40 | Runtime GPU buffer pointer, initially zero |

The original static width table at `0x008e4ca4` is `[1,1,2,2,4,4,4]`. Actual mesh data uses type 6 (float) with two/three components and type 1 (unsigned byte) with four components. The reader also supplies conventional signed/unsigned integer conversions for types 0/2/3/4/5, though those mesh formats do not occur in this corpus.

| Primitive buffer offset | Field |
| ---: | --- |
| 0 | Collada primitive type |
| 4 | Material name string offset |
| 8 | Declared primitive count |
| 12–29 | Eighteen signed-byte attribute references; `-1` means absent |
| 30–31 | Padding |
| 32 / 36 | Minimum / maximum vertex index |
| 40 | Index count |
| 44 | Index bytes or on-demand record offset |
| 48 / 52 | Runtime GPU / cached mesh-buffer pointers, initially zero |

References at 12/13/14 identify position/normal/color in observed meshes; 16–19 identify UV streams. Other reference roles remain exposed without invented semantic names. The index width is two bytes when the maximum is below 65,536, otherwise four. All 11,962 recovered primitive buffers have type 0 and exactly three indices per declared triangle.

`ColladaPrimitiveMap` at `0x008eb338` contains `[6,4,3,1,2]`. These are **engine primitive enums**, not GL constants. Type 0 is confirmed to contain triangle-list indices.

`CMesh::CMesh` tests the root word at `0x64`: if its signed value is positive, buffer offsets point to 16-byte on-demand records: reference count at 0, reader file offset at 4, byte-size/flags at 8 and cached buffer pointer at 12. The recovered Prince file uses this form; all 346 vertex/index records have zero cached pointers and byte sizes with their low two bits clear. The decoder borrows the bytes at the complete-image file offset. It does not emulate allocation, loading callbacks or reference counts.

## Animation and segments

The animation library count/pointer are at root `0x24` / `0x28`, with 32-byte records.

| `SAnimation` offset | Field |
| ---: | --- |
| 0 | Animation ID/name string offset |
| 4 / 8 | Sampler count / sampler array offset; stride 28 |
| 12 / 16 | Channel count / channel array offset; stride 16 |
| 20 | Animator runtime word; zero in recovered files |
| 24 / 28 | Optional default / offset-scale variant record offsets |

| Sampler offset | Field |
| ---: | --- |
| 0 | Interpolation enum; 0 disables interpolation |
| 4 / 8 / 12 | Time scalar type / component count / data-entry index |
| 16 / 20 / 24 | Output scalar type / component count / data-entry index |

All recovered time component counts are one. Time storage types are 1 (unsigned byte frames), 3 (unsigned short frames), or 4 (signed int milliseconds). Output data uses float or unsigned byte, with one/three/four components. Channel fields are a signed unresolved/runtime word at 0 (all `-1` here), target string offset at 4, track type at 8 and target hash at 12. Track type numbers are retained; they are not mapped to guessed transform/property names.

The root `0x30` points to an eight-byte segment library: count and array offset. Each segment occupies 24 bytes:

| Segment offset | Field |
| ---: | --- |
| 0 / 4 | Signed segment start / end time |
| 8 | On-demand storage state |
| 12 / 16 / 20, state 0 | File offset / byte-size-flags / cached pointer |
| 12 / 16 / 20, state 1 | Ownership word / inner-relocated flag / embedded data offset |

There are 2,278 state-0 and 1,454 state-1 segments. The new decoder resolves both from an intact file; runtime states above 1 and already inner-relocated state-1 data are rejected. This does not implement the original deferred file-reader/ownership machinery.

Each `SAnimationData` block begins with a word count followed by eight-byte entries. Each entry contains an element count and a **signed offset relative to its pointer word**. For entry `i`, the pointer word is `data + 8 + 8*i`; its target is that address plus the encoded offset. This second relocation is performed by the original `SAnimationSegment::getData`, independently of BRES base-relative fixups. Entry counts refer to keys, not the scalar-component count. The selected sampler defines the type and components needed to bound the data.

Optional default records expose the value address at record+8. Optional offset/scale records expose type at 0, scales at 4 and offsets at 8. Their variant payload layouts and track-specific use remain unresolved.

## Evidence limits

The captured original routines preserve full bytes and literal pools with ELF addresses and SHA-256 identities. `SAnimationAccessor` getter and typed search semantics are differential-tested against original instructions. Mesh constructors, `addStream` and `SAnimationSegment::getData` provide field/relocation evidence only; their full GPU and ownership semantics are not ported or claimed as executed-original equivalence. The separate ARM64 mesh test compares host and target implementations, and the full-cache audit verifies data integrity.
