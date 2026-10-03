# Crypt collision marker scene boundary

The supplied ELF SHA256 is recorded in original-functions.json. The bounded
ARM32/ARM64 differential passes 503 comparisons, including all 82 authored
Crypt colliders from eight marked models. The native host audit opens the
actual nine Crypt decor BRES files (eight marked, one unmarked), checks their
marker definitions against those fixtures, and passes 58 rejection checks
under ASan/UBSan at O1. No parent build or renderer files were changed.

## Original factory and matrix facts

- CColladaDatabase::constructNode 0x61b2f4 creates an authored CSceneNode group
  with the SNode TRS, and a separate default-transform MeshSceneNode child for
  each static geometry. ColladaFactory::createMeshNode 0x3508a4 takes the mesh,
  not the SNode TRS. CRootSceneNode::onPostLoad 0x65b3dc recursively updates
  transforms then invokes virtual +0xf4 to compute group bounds.
- CSceneNode::computeBoundingBox 0x65cda8 recurses into child groups, reads
  geometry bounds and applies each child's relative transform. All eight
  actual Crypt marker groups contain one static mesh and no child SNodes.
  Executing the original group-bounds routine for their identity mesh child
  produces the stored raw BRES bounds exactly. The marker SNode's own
  translation/rotation do not enter that group's local bounding box.
- SearchByName 0x352b74 with true uses an eight-byte `_colbox_` prefix and
  depth-first child order. Native Scene::graph preserves that order. The
  first matching node wins. A complete scene must retain helper instances;
  the BresView overload loads one when render instances have been filtered.
- GameObject::InitPost 0x38be84..0x38bf30 changes any owner scale component
  with abs(scale)<float(0x38d1b717) to 1, then multiplies each authored degree
  component by float(0x3c8efa35). VisualObject::SetRotation 0x472874 calls the
  Euler quaternion routine with register arguments **ownerY,-ownerX,-ownerZ**.
  It compares the computed quaternion numerically with the initial root
  quaternion before writing, preserving initial positive-zero components
  when the whole quaternion is equal.
- ISceneNode::getRelativeTransformation 0x598908 calls quaternion matrix
  transposition 0x5602d0, then scales its three basis columns (0x597788),
  then writes position. The bridge reuses existing `dh2_quat_from_euler` and
  `dh2_node_matrix`, rather than introducing an Euler convention.
- VisualObject::CalcMeshBox 0x47211c reads marker group bounds and **the
  marker's immediate authored parent scale**, separately from owner scale.
  Three models have an internal parent scale of 0.648593. It then uses the
  outer visual root relative matrix, clears its translation, transforms two
  bounding endpoints, orders them, and recenters around zero. The bridge
  reuses the existing decor marker kernel. It does not transform eight corners.

## Stable integration

`decor_scene_marker(view, completeScene, marker, error)` obtains the verified
marker bounds and parent scale. `dh2_decor_scene` takes owner position,
authored degree rotation, owner scale, marker bounds and marker parent scale;
it returns effective scale, radians, stored root quaternion, root matrix and
mesh box. Feed that mesh box and the original owner position to the existing
`dh2_decor_body_config`. Models without a marker report found=0.

Other marker topologies fail explicitly, including missing/filtered geometry,
multiple geometry children, skinned markers and child groups. Generic Collada
materials, allocation, animation and arbitrary group factories were traced
but not executed/reconstructed by this bounded bridge. The oracle executes
the original arithmetic and group-bounds routine with typed mesh services;
common IEEE/libm imports do not prove historical Android libm bit behavior.

## Character and owner Z boundary (read-only follow-up)

VisualObject::SyncPosition 0x470cb8 passes owner+0x160 XYZ unchanged to
SetPosition 0x470c24, which sets the root position and updates absolute
position with recursive=false. GameObject::SetPosition 0x393db4 stores XYZ
unchanged, updates absolute owner AABB, calls physical XY and visual sync.
There is no subtraction of rendered mesh minZ/footZ in these routines.
Upstream owner floor projection is a separate producer.

Without a marker, CalcMeshBox searches descendant skinned nodes (0x73656164)
first; only an empty result falls back to static mesh nodes (0x6d656164).
It unions each provider's local bounding box multiplied by that node's
immediate parent scale, then uses the same outer-root matrix/two-endpoint
recenter branch. It does not read root RefreshBoundingBox or rendered feet.
CSkinnedMesh::getBoundingBox 0x6636a4 computes via a skin-technique provider;
those providers use per-joint authored boxes or joint positions and cached
joint absolute matrices. A raw geometry box cannot replace that provider.
Modular skin 0x646c1c unions the selected component mesh boxes. Descendant
joint matrices are not recursively refreshed by the SetPosition(false),
SetRotation or SetScaling calls themselves; their last loaded/sampled pose
is an explicit input to any character bbox reconstruction.
