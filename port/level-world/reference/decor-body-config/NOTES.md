# Original decor body configuration

Original engine SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

`Level::_LoadProcess` state 5 constructs the gameplay physics world at
`0x3f7384–0x3f73ac`. The arguments are literal float words
`c4fa0000,c4fa0000,44fa0000,44fa0000`: **(-2000,-2000,+2000,+2000)**.
These bounds are already physics world units. They are independent of the
authored Crypt scene extent and receive no additional 0.01 scaling.
The audit executes this original caller slice and observes the load arguments.

`VisualObject` constructor `0x472c5c` starts `visual+0x28` at zero. Its call
at `0x472de0` searches for the literal `_colbox_` at ELF `0x8cd6b0`.
`SceneManager::SearchByName` (`0x35a0e4`, `0x352b74`) performs first-match,
depth-first source child traversal. The true argument selects an eight-character
prefix comparison. Finding that node sets `visual+0x28=1`; the returned marker
node is stored at `visual+0x0c`. No authored marker means no PODecor body.

`Decor::InitPost` (`0x388a98`) applies the visual box and checks this byte before
allocating a 40-byte PODecor. AnimatedDecor repeats this policy in its post-init
path after updating animation state. Container also uses the identical clone at
`0x39fc2c`, with its own caller gates; those gates are captured but not part of
this decor kernel. The body constructors do not enumerate subshapes.

The recovered marker branch of `CalcMeshBox` (`0x47211c`) reads the marker
node's bounding box, multiplies each endpoint by the parent node's scale, copies
the root node matrix and zeros translation. It transforms **two endpoints**,
orders their components, computes half their difference, and writes a box
centered at zero. It does not transform all eight corners, preserve the marker
offset, or create triangle colliders. `dh2_decor_marker_mesh_box` implements
this branch, including separate float multiplication/addition order.

`ApplyMeshBox` calls `GameObject::SetRelativeAABB` (`0x38b110`), which pads
each XY extent strictly below 10 game units by subtracting 5 from the minimum
and adding 5 to the maximum. Exactly 10 receives no padding. Initially zero XY
extents set the flat byte at owner+0x2f9 to 1; other extents preserve its previous
value. `UpdateAbsoluteAABB` (`0x38aac8`) adds the owner XYZ position, followed
by a PF-object update. This mesh update is exposed separately as
`DecorBodyConfig.mesh_pf_updates`.

The PODecor clones force the polygon branch of `PhysicalObject` (`0x46f2f0`):

* Width and height are the owner's absolute XY AABB differences multiplied by
  float `0.01`; body XY is owner game XY multiplied by the same float.
* Polygon vertices are `(-hx,-hy),(hx,-hy),(hx,hy),(-hx,hy)`, where half extents
  are scaled width/height multiplied by 0.5. Radius is max(width,height)*0.5,
  with unordered comparisons selecting width, matching the original.
* Body angle, mass, inertia, local center, and damping start at zero.
  allowSleep, isSleeping, fixedRotation are true; bullet is false.
* Shape friction is 1, restitution and density are 0; sensor is false.
  Group is 0, category is 2, mask is 0xffff. The original debug group override
  changes group to -666. Body and shape user data refer to the physical object.
* Ordered services are allocate, CreateBody, CreateShape, SetMassFromShapes,
  optional destroy previous, assign, PF update. Attachment passes pin=false,
  so the physical object's pinned byte stays zero. It is static because its
  shape density/mass is zero. The disable-physical setting destroys the newly
  allocated object and preserves the previous attachment without PF update.

`crypt-colbox-inventory.json` reads complete original BRES node and geometry
records without modifying assets. It records model hashes, original marker
names/node transforms/serialized geometry bounds and authored actor placements.
All eight Crypt candle/candlestand models have one marker, covering 82 authored
placements. The two swamp_caveentrance_effect placements have no marker.

Verification: 2,082 original marker calculations and 2,222 original decor
bounds/body/filter/attachment comparisons, including 84 authored placements,
all branch gates, prior attachments, debug flags, exact zero/10 padding
thresholds, signed zero, finite overflow and NaNs. Native pointer identities
exceed 4 GiB. Host O1 ASan/UBSan replays the same gold fixture: 11,794 physical
service requests and 12 malformed argument checks, zero mismatches.
Finite float words and integer fields compare exactly; arithmetic NaN payloads
compare by class. The separate source-built physics backend audit proves real
shape/mass/broadphase/contact/solver/world stepping rather than these service
observers.

Boundaries: node bounding-box access, parent scale/root matrix generation,
visual/database factories and the non-marker union branch of CalcMeshBox remain
scene services. The authored fixture uses actual Crypt boxes/scales/positions
with an explicit identity root-matrix service fixture; it does not establish
the authored Euler conversion or the final rotated live Crypt body sizes.
The game object's base post-init and floor-map loading are service observers.
No visual object is turned into a collider without the recovered marker gate.

Reproduction uses the installed NDK clang++ with
`--target=aarch64-linux-android24 -shared -fPIC -O0 -fno-fast-math
-ffp-contract=off -std=c++17 port/level-world/decor_body_config.cpp`, then runs
`tests/decor_body_config_differential.py --engine ORIGINAL --library NATIVE
--reference-output FIXTURE --report REPORT`. Inventory generation is retained
at `.local-inputs/decor-body-discovery/crypt_inventory.py`.
The host test links `decor_body_config.cpp` and `tests/decor_body_config.cpp`
with `-O1 -fno-fast-math -ffp-contract=off -fsanitize=address,undefined` and
takes that fixture as its sole argument.
