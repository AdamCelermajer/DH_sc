# Floor type properties

This component parses the bounded `floortypes` UserProperties value and
reproduces the original floor masks used by navigation. It is linked into the
selected `dh2_level_world` library. `find_property`
requires a checked byte extent containing a NUL terminator, splits on LF and
the first `=`, trims ASCII whitespace around a key, decodes the first `%22...%22`
value pair, and uses the last occurrence of a duplicate key. A key-only line or
`key=` is present with an empty value; this is distinct from an absent key.
Parsing is capped at 64 KiB. Spans borrow the source bytes.

`floor_type_mask` uses only the explicit `floortypes` property value. An absent
property leaves the type string empty; the native loader does not inspect the
node name. Matching is case-sensitive substring matching and multiple tokens
combine. Values such as `sand`, `wood`, and `door` add no mask. The native masks are:

| Token | Mask | Meaning in the native floor record |
| --- | ---: | --- |
| `hole` | `0x00000001` | Low path-eligibility requirement |
| `water` | `0x00000002` | Low path-eligibility requirement |
| `void` | `0x01000000` | Separate floor category flag |
| `wall` | `0x02000000` | Separate floor category flag |

`can_path_on` mirrors `PFObject::CanPathOn`: zero floor requirements are
always allowed; otherwise `(floor_mask & object_path_mask) == floor_mask`.
Void and wall remain distinct category flags and do not imply hole or water
path permissions.

## Reverse-engineering evidence

- `recovered/native/assembly/libDungeonHunter2.so/UserProperties-75e915465cf3-001.asm`:
  `_ParseKeyValue` at `0x318e5c`, `_ParseLine` at `0x318fe4`,
  `_ParseProperties` at `0x31903c`, and constructor at `0x319158`. The original
  splits LF lines, splits on the first `=`, and extracts the text between the
  first two `%22` markers when both exist.
- `recovered/native/assembly/libDungeonHunter2.so/PFFloor-ed6502919ecd-001.asm`:
  `_LoadNavMesh` at `0x520b40`; it reads only the parent `floortypes` property.
  An absent property leaves the tag string empty; case-sensitive substring
  checks OR the four masks above.
- `recovered/native/assembly/libDungeonHunter2.so/PFObject-d3788b6ddde1-001.asm`:
  `CanPathOn` at `0x524230`; the floor mask must be a subset of the object's
  path mask, with a zero floor mask accepted.
- `recovered/native/assembly/libDungeonHunter2.so/PFWorld-c21c45a6e09d-001.asm`:
  `ValidatePosition` at `0x525d84` performs the path/floor query and invokes
  `CanPathOn`; subsequent world-position and collision handling remains outside
  this mask-only helper.
- The checked cache sample `cache/files/data/3d/modules/swamp/swamp.bdae` has
  `floortypes = %22hole%22` at BRES offset `0x94b18`, water at `0x94c6c`, wood
  at `0x95298`, and door at `0x9c6a8`. Cache files are supplied separately and
  are not copied into this source component.

Build and run focused host tests (the default cache path is the supplied local
cache next to the repository):

```sh
python port/floor-types/build.py
```

The strict host test binary is built under the system temporary directory and
removed after the run.
