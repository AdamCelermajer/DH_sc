# Retained chest visual and physical pipeline

New modules:

- `gameobject_scene_binding_v1`: actual full authored graph composition while
  source displacement byte1ec is zero. Existing Character `SceneBinding` remains
  strict and unchanged. No character root-motion node is created or renamed.
- `retained_gameobject_visual_v1`: retains source BRES bytes, complete Scene,
  root pose/flags, authored clip names/ranges, Player, timeline and skin data.
- `retained_gameobject_visual_asset_connection_v1`: supplies concrete callbacks
  for the existing GameObjectVisualAssetOwner; one typed receiver per visual
  identity and the same canonical base+2d8 attachment.
- `retained_gameobject_decor_v1`: builds actual oldBox2D polygon body and shape,
  same parent+2dc assignment, original Debug gates, filter and detach lifetime.

Original source evidence is under `reference/openable-container-v1`:
`retained-visual-source.asm`, `retained-parent-source.asm`,
`retained-animator-constructors.asm`, `scene-node-ctor-source.asm`,
`set-on-animate-source.asm`, `retained-visual-replay-source.asm`,
`retained-displacement-source.asm`, `retained-animation-ending-source.asm`,
`retained-podecor-source.asm`, `retained-podecor-collision-source.asm`,
`retained-visual-destructor-source.asm`, `timeline-vtable-source.txt`.

The actual scene constructor initializes source flags to 0x60f. The dynamic
parent branch recursively ORs bit0x200. Source CTimeline constructor has loop1
and scale1; its nonempty library constructor selects clip0. Original vtable+44
is getLoop666c30. The BaseNamed helper callback is now named `timeline_get_loop`
without changing its algorithm or field layout. `NewAnim(false)` writes the
source displacement flag zero and returns; it does not reset or resample the
graph. Internal Animator completion uses original extra-time/pending stores
before the constructor-installed DoNothing callback clears pending.

Actual three cache chest models contain skin controllers. Each is validated
through the existing source-backed skinning loader, actual bone references,
inverse-bind matrices, influences and primitive position semantic. Palette and
world-space deformed positions are updated from the same animated Scene. They
are exposed through `skinned_meshes()`; a renderer must not apply the owner
matrix a second time to those world-space positions. Static instances expose
their original model vertices and submitted Scene instance matrices instead.

The source PODecor Begin/Persist/End/Result methods are independently captured
literal `bx lr` leaves. These empty methods are genuine source behavior. The
filter uses actual canonical byte80 and peer-owner byte80 before original
group/mask/category arithmetic. Missing peer or visible-field producers fail
when reached. Actual body velocity is projected in original game units (*100).

Integration order: retain one canonical base/runtime receiver; construct the
visual connection with that same base and live source services; give its
`services(real_connection_lease)` to GameObjectVisualAssetOwnerV1. That existing
owner produces base+2d8 and root+204. Borrow the connection's `attached()` visual
for chest named play, meshbox, timeline and PODecor. Retain the physical owner
until source destruction; release bodies before clearing NativeWorld. Source
SetPhysicalObject(NULL,false) preserves the original MP_NoPhysics early return.
Do not replace it with unconditional body destruction.

Required production outer endpoints remain real asset reading, SceneManager
register/ForceRegister/root release, canonical UpdatePFObject, shared Debug
load/query and canonical physical-peer lookup. Nonempty authored subscene,
OptimizeStatic and no-colbox node-type traversal are explicit unrecovered
branches. Modular equipment reconstruction is not supplied by this chest
resource successor. Base callbacks do not deliver container `opened` events;
the source base SetCallbacks virtual is still `bx lr`.

Validation: all new modules and test pass strict Android ARM64 syntax with
`-Wall -Wextra -Werror` (vendor Box2D headers are system headers). Root's exact
three-resource marker Android audit passed. The first composed runtime run
failed at Character root-motion binding; the second correctly reached skin
construction. Both producer mismatches were fixed as separate real domains.
The third runtime run passed all three real-resource visuals and three native
bodies against APK d5bd80db with the new implementation TUs linked into the test.
This is composition proof, not yet APK-only integration proof. A later APK-only
run against d99af96a failed the compound constructor postcondition; the test now
reports ready/root/marker/ForceRegister/PF separately. All headers and library
TUs must be rebuilt together: stable node handles and root204/light40 fields
were added after the earlier test. No loader/world acceptance is claimed.
`tests/retained_gameobject_chest_pipeline_v1.cpp` requires the
three exact extracted resources directory and exercises real skin/animation
pose change, native polygon allocation/filter/detach/release. SceneManager,
PF and Debug at its outer boundary are expressly declared fixtures.

The additional `tests/retained_gameobject_asset_initialization_v1.cpp` passes
strict ARM64 syntax and awaits root's runtime run. It loads recovered GameObject
defaults over the same GO_ID7 base, runs generic InitPost and InitFinal through
the concrete asset connection for all three chest resources, verifies source
2d8/root204/target_node transport, real activation timeline, resource miss
preserving the previous visual, and deleting-destructor delivery. Its condition,
device, PF, light catalog and SceneManager endpoints are declared fixtures. It
does not claim the derived container network constructor or source LoadFloor.

## Loader handoff files and ownership

Add these four implementation TUs together with their matching headers:
`port/level-world/gameobject_scene_binding_v1.cpp`,
`port/level-world/retained_gameobject_visual_v1.cpp`,
`port/level-world/retained_gameobject_visual_asset_connection_v1.cpp`,
`port/level-world/retained_gameobject_decor_v1.cpp`.
They reuse already retained base/default/property, asset lifecycle, BaseNamed,
BRES, complete Scene, animation/clip/event/timeline, skinning/mesh, decor marker,
meshbox/body configuration, physical world/native polygon and engine math TUs.
No new renderer or replacement canonical field owner is required.

One visual connection belongs to one canonical base. Its callbacks must remain
leased while the generic asset owner exists. A root handle is the retained root;
a node handle is stable owner storage indexing the current graph, not a raw
vector element that animation sampling may replace. Source root204 is set only
by SetVisualObject after attachment. Visual light40 storage is exposed; full
ApplyLightSet/SceneManager lighting propagation is not implemented here.

Production acceptance still requires real registered scene release callbacks,
same PF ownership, physical peers and shared Debug; fixtures are not admissible
loader substitutes. Failure during construction preserves the reached candidate
in the connection; a production candidate-discard boundary must explicitly
release all registered failed candidates before destroying that connection.
Current connection lookup/attachment APIs do not yet provide a bulk failed-
candidate discard endpoint. This limitation must be closed before allowing a
loader failure path to drop a connection with registered roots.
