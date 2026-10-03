# Character mesh and collision-bound production

The original ELF hash and captured routine hashes are in original-functions.json.
ARM32 original instructions and compiled ARM64 helpers match in 206 skin-bound
cases across all four skin techniques (824 observations), 202 non-marker mesh
cases, five visual scale cases and 101 owner-bound cases. Host O1 ASan/UBSan
replays every fixture, opens the actual Prince BRES, maps four selected warrior
controllers and verifies group-versus-grandparent scale mapping; 18 malformed
checks pass. No parent CMake or renderer files were edited by this worker.

## Source skin provider

CSkinnedMesh::getBoundingBox 0x6636a4 computes through its active technique when
dirty and returns the result at +0x24. Hardware quaternion 0x66e384, matrix
0x66c5f4, texture 0x66ef44 and software 0x66fb84 use the same bounding algorithm.
The techniques cache pointers to each joint's getAbsoluteTransformation
(virtual +0x38), rather than skinning rendered mesh vertices for bounds.

The source SSkin fields +0x74/+0x78 contain joint count/name links; +0x8c/+0x90
contain authored per-joint bounding boxes. The matrix-pointer vector count is
masked to eight bits (a length of 256 wraps to zero). Without boxes, the bounds
union each joint matrix's translation point. With boxes, CMatrix4::transformBox
0x587398 transforms two endpoints with each matrix including translation,
orders endpoints, and unions the transformed boxes. An asserted identity hint
bypasses the transform, even for inconsistent matrix words; the differential
preserves that behavior. Box count is used as a nonzero gate, so checked native
inputs require enough boxes for every effective joint. Empty joint bounds retain
the original FLT_MAX/-FLT_MAX sentinels.

CModularSkinnedMesh::computeBoundingBox 0x646c1c unions the currently selected
component IMesh providers. The actual four Prince default warrior component
providers and their original modular union are exercised. Equipment selection
is an explicit caller input; this bridge does not invent armor-slot policy.

## Non-marker VisualObject bounds

VisualObject::CalcMeshBox 0x472408 collects skinned node type 0x73656164 first;
only an empty result falls back to mesh type 0x6d656164. For each provider, it
multiplies lower/upper coordinates by its immediate parent's scale and unions
lower coordinates as minima and upper coordinates as maxima. Negative parent
scales are not independently reordered at that union stage. The aggregate then
uses the outer-root relative matrix through the original two-endpoint/recenter
branch. Existing decor_scene supplies the exact owner scale/Euler/root matrix.

Native Scene represents geometry instances on their authored SNode **group**.
The original factory creates a separate identity geometry child under that
group. Therefore graph[instance.node_index].scale is the original provider's
immediate parent scale; graph[node.parent].scale is its grandparent's scale.
The host fixture changes these scales to different values to verify the mapping.
Selected controllers attached to the same group are unioned before that scale.

## Owner size properties and bounds

Character::InitPost 0x3b4d60 loads/recalculates CharProperties, then reads **base**
Scale_X/Y/Z from Character+0x59c/0x5a0/0x5a4. The base sheet's values start at
0x56c, so these are indices 12,13,14. X/Y convert integer to float and multiply
by float bits 0x3c1374bc (0.009), Z by 0x3c23d70a (0.01). This overwrites visual
owner scale before calling GameObject::InitPost, whose near-zero clamp and
degree conversion remain separately verified by decor_scene.

Character::SetRelativeAABB 0x3a4398 overrides GameObject's method. For
already_scaled=false it reads **resolved** Collision_Scale at Character+0x1038.
Resolved values start at 0xff8, so this is index 16. It converts that integer,
multiplies by float(0x3c23d70a), scales all six mesh bounds, then tailcalls shared
GameObject::SetRelativeAABB 0x38b110. That adds five units to each side of widths
or heights below ten, preserves/sets the flat byte, produces absolute bounds
with owner XYZ and performs one PF update. ApplyMeshBox supplies false. The
native helper reuses that already-verified shared decor bounds production.

KnightPlayerBase is authored row 263, with base scale properties100/100/100 and
Collision_Scale85. The existing original-resolver raw-row fixture
.local-inputs/properties-original-rows.bin confirms resolved index16=85; actual
runtime gear/buffs or save producers remain caller inputs. Source visual scale
is (0.899999976,0.899999976,1). The selected warrior rest pose yields mesh bounds
±(133.764282,36.183228,177.258026), then owner relative bounds
±(113.699638,30.755743,150.669312) for those source properties. These are pose
and selection fixtures, not a universal radius or a replacement for live state.

## Initialization order and remaining boundary

VisualObject constructor 0x472a0c calls source AssetManager LoadNode0x50a504,
SetParent/Sync0x47295c, RefreshBoundingBox0x35c854, finds the modular node and
marker, then CalcMeshBox/ApplyMeshBox at0x472c0c/0x472c14. Only afterward does it
construct AnimController0x474d30 and assign it via0x470a84. Assignment stores
the controller pointer; it does not sample an Idle animation. Constructor
source captures for these statements are in
initialization/reference/original-functions.asm, with routine hashes in its
manifest. Additional local visual-construction captures retain the attachment
caller 0x394338 as well.

Character InitPost's late path calls SetInitialPosition0x3a58f4,
GameObject::SetPosition0x393db4, ApplyMeshBox0x470a54 at0x3b541c, then
Revive(null,true)0x3a59ac at0x3b542c. Revive's true branch calls
InitPhysicalObject0x3b4088 at0x3a5ad4. The late ApplyMeshBox reuses the stored
visual mesh box. No Idle sampling or recursive joint refresh appears in that
late chain. Earlier SG_Load(2)/(4) branches can restore saved equipment/state;
SG_Load0x3bc4d0 only tailcalls0x465430 when Character+0x14e8 is nonnull. Those
save/equipment producers and full source AssetManager/root clone lifecycle are
not reconstructed here. A complete source-selected initial pose for every
save path cannot be inferred solely from the packaged factory/rest fixture.

The native bridge takes the caller's cached complete Scene world matrices as
the joint-pose service. Callers must supply factory/rest or previously updated
joint matrices in the same model frame; outer owner TRS is applied separately.
The audit does not execute the entire Collada allocation/cache-preparation
lifecycle. Common libm dependencies do not prove historical Android libm bits.

GetCharAIId0x3a2fec reads resolved index1 at0xffc, selecting row8 on an invalid
index. GetCharAI0x3a3024 resolves the 0x44-byte runtime AiProps row, and
GetCharType0x3a3054 reads its +0x38 Type field. Knight row263's original resolved
AIId is44; the packed AI table row44 is Player, script `__player__`, Type1.
The variable-length packed reader consumes all5071 bytes; this is not a cast
of the packed table to runtime AiProps. Thus the selected Knight source fixture
uses CharacterConfig character_type=1 and is_player=1.

## Owner Z

GameObject::SetPosition0x393db4 stores XYZ unchanged. VisualObject::SyncPosition
0x470cb8 and SetPosition0x470c24 pass that XYZ unchanged to the root and update
absolute transforms with recursive=false. No subtraction of mesh foot/minZ
exists in these adapters. SetInitialPosition0x3a58f4 obtains a floor/height result
through0x525508; its full floor producer is a separate parent-owned boundary.

## APIs

- character_scene_entries(BresView, completeScene, entries, error): selected
  geometry providers, authored joint boxes, cached joints and group scales.
- dh2_character_skin_bounds: exact skin-technique box/point bound kernel.
- dh2_character_mesh_box: provider-kind preference/union and outer transform.
- dh2_character_visual_scale: base[12..14] to source visual owner scale.
- dh2_character_owner_bounds: resolved[16], mesh box and owner XYZ to padded
  relative/absolute AABB, flat byte and PF update count. Feed absolute XY to
  the existing character_body_config constructor kernel.
