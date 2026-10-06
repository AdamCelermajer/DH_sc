# Retained visual registry successor

The frozen bb4bcf60 APK passed BOTH executable-only tests: the actual three chest
BRES/complete graphs/skins/timelines/native Box2D pipeline, and concrete asset
connection composed with generic InitPost/InitFinal over all three resources.
This supersedes the earlier pending APK-only status in the V1 report.

New files: `port/level-world/retained_gameobject_visual_asset_connection_v2.hpp`,
the matching cpp, and `port/level-world/tests/retained_gameobject_visual_asset_connection_v2.cpp`.
The successor replaces V1 when selected; it does not create a second registry.
It retains the same canonical base and unchanged V1 visual owner. Its service
contract is compatible with GameObjectVisualAssetOwnerV1. The host explicitly
calls `discard_failed(error)` at candidate rejection, then detaches any attached
visual through the original SetVisualObject owner before `discard_unattached`.
Both APIs preserve diagnostics on successful cleanup. Failure stops scanning,
retains unfinished receivers, and permits retry of the uncompleted source tail.
Reverse candidate order is host lifetime policy, not an original engine claim.
Each receiver delegates the actual original destructor order to VisualV1.

The actual-cache test covers read/register/ForceRegister/PF construction failure
prefixes, failed root release, failed destructor ForceRegister after root drop,
retry without a second root drop, and refusal to discard an attached successful
visual. Both new files pass strict ARM64 syntax. Runtime receipt is pending.

## Source SceneManager endpoint correction

VisualC1 472b68 calls manager-root virtual5c, which is ISceneNode::addChild598864;
it does not register rendering lists at construction. The RootSceneNode vtable
starts at table+1c. Destructor virtual74 is removeAnimators598658, and virtual68
is remove59706c. The earlier generic names register_root/release_root describe
these boundaries, not draw-list registration. ForceRegister350ee0 writes byte
448=1 and byte289=1. Constructor352c3c initializes counter440=0, cadence444=4,
force448=0. Parent root addChild grabs the same child, removes its previous
parent, appends it, sets parent, notifies hierarchy, and propagates visibility.
RemoveChild597004 unlinks, clears the same child's parent, then drops it.

Evidence: `reference/openable-container-v1/scene-registration-source.asm`,
`scene-manager-registration-source.asm`, `scene-root-membership-source.asm`,
and `root-scene-vtable.txt`. Actual draw-registration357ea4 additionally reaches
application Debug/current Level/batch visibility and native virtual render-node
registration; this is a distinct whole method and must not be faked by adding
the roots to a vector. Production manager-root/animator ownership remains next.

`gameobject_scene_root_registry_v1.hpp/.cpp` now owns this actual ordered root
membership and constructor-derived manager flags. `add_child` borrows the SAME
root flag/parent fields plus a real root lease, reappends an existing child in
source order, sets parent and bit40, notifies source hierarchy/render dirtiness,
and calls the required complete visibility receiver. `release_visual_root`
requires actual removeAnimators delivery before unlink/drop. Its focused test
audits flag/membership/release-failure ordering with declared endpoint fixtures.
The owner refuses foreign-parent moves without the original previous parent's
RemoveChild receiver. `V2.lookup_root` resolves even constructing/failed candidates
to the same retained visual; it creates no additional scene or render graph.

This manager member graph does not yet complete production callbacks: same root
visibility bytes120/121 and recursive descendant flag propagation need their
native scene lifetime owner, and the actual animator list detach/drop must reach
the retained animation owner before root removal. The frozen VisualV1 does not
currently expose that removal endpoint. No successful empty callback is supplied
to conceal these gaps. The original Play/Update render pipeline remains intact.
