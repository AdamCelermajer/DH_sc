# Checked BRES scene hierarchy

This component exposes immutable, bounds-checked views of the recovered
Collada scene records. It lets callers follow a scene's visual-scene URL,
traverse root and child nodes, read local position/quaternion/scale values,
compose local-to-world affine matrices during a bounded preorder walk,
classify instances, and resolve type-3 geometry URLs to local geometry records.
The input BRES bytes must remain alive and unchanged while views are used.
The interfaces in `scene.hpp` are new port interfaces, not the original
ARM32 class ABI.

## Evidence and scope

The serialized offsets were traced from the verified external recovery ZIP's
`libDungeonHunter2.so` decompilation. The relevant function-index ELF
addresses are `CColladaDatabase::constructScene` at `0x61b9e8`,
`constructVisualScene` at `0x61b8bc`, `constructNode` at `0x61b2f4`, and
`constructGeometry(SInstanceGeometry*)` at `0x61aeb8`. The ZIP's SHA-256 is
`b3ff974e2b74f50387465d5665f60d56ac79c29a449c6299745461998045c4d8`.
This is a reconstruction of serialized data access, not an instruction-level
translation of those functions or proof of original rendering behavior.

The matrix convention has separate instruction evidence from the exact
original ARM32 ELF (SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`):

- `ISceneNode::getRelativeTransformation` at ELF `0x598908` calls
  `quaternion::getMatrix_transposed`, then `CMatrix4::postScale`, then writes
  the node position to matrix elements 12–14.
- `CMatrix4::postScale` at `0x597788` multiplies the first three elements of
  each of the three affine columns by the corresponding X/Y/Z scale.
- `ISceneNode::updateAbsolutePosition` at `0x597c60` passes the parent matrix
  first and child-local matrix second to `CMatrix4Base::mult34` at `0x597884`.
  Its instructions compute `out[0] = A[0]*B[0] + A[4]*B[1] + A[8]*B[2]` and
  `out[12] = A[0]*B[12] + A[4]*B[13] + A[8]*B[14] + A[12]`. This establishes
  column-major parent × child affine composition.

`dh2_scene_local_matrix`, `dh2_scene_world_matrix` and
`dh2_scene_walk_visual` implement that data path. The quaternion matrix comes
from the already reconstructed and instruction-tested `port/engine-math`
component. The walk has a caller-supplied node limit, a depth limit of 64,
ancestor-cycle detection and a callback that receives each node's world
matrix. The callback runs for hidden nodes too; visibility filtering is the
caller's decision. Runtime dirty flags, scene animation, unusual original
identity fast paths and rendering coordinate conversion remain outside this
source reader. The new composition has cache and fixture checks, not an
original-instruction differential test of `mult34` itself.

| Record | Serialized fields read |
| --- | --- |
| Collada root | Visual-scene count/pointer at `0x98/0x9c`; scene-reference count/pointer at `0xb8/0xbc` |
| Scene reference | Eight-byte type/payload pair; type 6's payload has a URL offset at `+4` |
| Visual scene | Sixteen bytes: ID/name offsets, root-node count/pointer |
| Node | Eighty bytes: ID/name, position at `+0x0c`, quaternion at `+0x18`, scale at `+0x28`, visibility at `+0x34`, child count/pointer at `+0x38/+0x3c`, instance count/pointer at `+0x40/+0x44`; `+0x4c` is retained as an opaque extension offset |
| Instance | Eight-byte type/payload pair; type 3's payload has a geometry URL offset at `+4` |

The parser checks record spans, strings and finite transform values. It keeps
types 1, 2, 4, 9, 12 and 13 as opaque instances. Only local `#ID` URLs are
resolved. A caller can use `dh2_scene_geometry_index` with the existing
`dh2_mesh_open` reader to obtain the corresponding mesh payload. Static
world matrices are available through the bounded walk. Material bindings,
skinned nodes, animation application, ownership, scene rendering and gameplay
are not implemented here.

## Validation

`validation.json` records a full scan of the owner's separately supplied
cache: 2,904 BRES files, 1,563 scene references, 1,563 visual scenes, 3,337
root nodes, 21,472 total nodes, and 11,648 instances. All 1,563 visual URLs
resolve locally. Of 10,444 type-3 geometry URLs, 10,403 resolve within their
file; 41 remain unresolved locally (mostly camera and animation template
files). All 21,472 world matrices compose and the walk covers 6,400 nodes
with non-identity quaternions and 1,508 with non-unit scales. Sixteen focused
malformed-input, cycle, walk-limit and noncommutative ordering checks pass
using one real BRES sample and a synthetic transform fixture.
No original assets, cache files or APKs are included in this module.

The host DLL and Android ARM64 library both build with strict warnings. The
ARM64 ELF has 16 KiB-aligned load segments. The full-cache audit executes the
host build; the ARM64 artifact was compiled and inspected, not run on a device.

From the repository root, with Python 3, a C++17 compiler, an optional
Android NDK r29, and the owner's local cache:

```sh
python port/scene-payloads/build.py --ndk /path/to/android-ndk-r29 \
  --report port/scene-payloads/build-validation.json
python port/scene-payloads/tests/audit_cache.py \
  --cache /path/to/cache/files \
  --library port/scene-payloads/build/libdh2_scene_host.so \
  --report port/scene-payloads/validation.json
python port/scene-payloads/tests/safety.py \
  --sample /path/to/cache/files/data/3d/animateddecors/candle_flame.bdae \
  --library port/scene-payloads/build/libdh2_scene_host.so \
  --report port/scene-payloads/safety-validation.json
```

On Windows, the host output suffix is `.dll`. Build outputs are local and
ignored by Git. The cache audit's manifest digest identifies the tested corpus
without publishing its contents.
