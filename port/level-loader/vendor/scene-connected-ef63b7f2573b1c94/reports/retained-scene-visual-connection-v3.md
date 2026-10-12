# Concrete retained scene root connection V3

This replaces the register/ForceRegister/release fixtures used by earlier chest
resource tests. `retained_scene_visual_connection_v3.hpp/.cpp` composes the SAME
V2 visual registry with one shared `GameObjectSceneRootRegistryV1`. The manager
uses its own native root identity, ordered child leases, same borrowed visual
root parent/flags, and actual source constructor/dirty/force stores. It does not
create a duplicate render graph or clone the canonical GameObject base.

Narrow additions to `retained_gameobject_visual_v1.hpp/.cpp` retain original
ISceneNode ctor visibility120=1, parentVisibility121=1 and parentec=0. Every node
uses its existing same `node_flags_` and stable scene index. `notify_root_visibility`
implements 596e34: always store parentVisibility, compute bit1 from local/parent,
and recursively visit children in authored order only when effective visibility
changes. `set_node_local_visibility` implements 596ec4 over the actual same node
fields. Hidden descendants survive a parent hide/show; local restoration then
propagates only through that descendant tree.

`remove_root_animators` drops the actual retained Player tracks/event bytes/
database resources by move-destroying that owner, clears its clip/timeline/
completion bindings, and disables subsequent animator sampling. The already
sampled scene and skin pose remain intact. This is a real lifetime mutation,
not an empty callback. Source ISceneNode::removeAnimators598658 invokes detach
and drops its list entries; CSceneNodeAnimatorD1 65dab0 calls removeAnimationTracks,
releases AnimationBlock and destroys its Collada database. Original base callbacks
remain DoNothing; no fake gameplay animation event is emitted.

V3 root release invokes this real removal before manager unlink/drop. A root
already detached follows original remove59706c's parent==NULL leaf after genuine
animator removal, then the existing VisualD1 tail clears its root and invokes
ForceRegister. Failed PF construction leaves its reached root registered;
V2 `discard_failed` now tears it down through these same concrete services.

Integration:

1. Retain one shared `GameObjectSceneRootRegistryV1` for the actual scene owner.
2. For each canonical base, construct shared `RetainedSceneVisualConnectionV3`
   with that base, actual asset-read/PF/world lease and the SAME manager.
3. Supply `bridge->services(bridge)` to GameObjectVisualAssetOwnerV1.
4. Borrow `bridge->attached()` for real mesh/timeline/PODecor.
5. On failed candidate call `discard_failed`, preserving diagnostic. Detach a
   successful base2d8 through source SetVisualObject before `discard_unattached`.
6. Release bodies, visuals and child membership before world/manager teardown.

Files to compile: the existing V2 connection and scene-root registry TUs, new
`retained_scene_visual_connection_v3.cpp`, and rebuilt `retained_gameobject_visual_v1.cpp`.
Visual class layout changed; matching headers and all APK consumers MUST rebuild.
Frozen V1 connection remains available but does not receive these concrete
manager callbacks unless explicitly supplied.

`tests/retained_scene_visual_connection_v3.cpp` passes strict ARM64 syntax. It
loads ALL3 actual chest resources, checks actual manager membership and flags,
parent/local visibility propagation, real authored animation, track removal with
stable sampled pose, repeated destructor after prior detach, and actual PF-fail
prefix cleanup. Only asset-read and PF are boundary providers; no scene,
visibility, animator or destructor fixture is supplied. Android run is pending.

Source captures: `scene-parent-visibility-source.asm`,
`scene-local-visibility-source.asm`, `scene-root-membership-source.asm`,
`scene-animator-detach-source.asm`, `scene-animator-tracks-release-source.asm`,
`scene-render-list-dirty-source.asm` plus the prior V2 captures. This is the
construction/release subsystem, not the separately reached draw-list registration
method357ea4 with application/Debug/Level/camera/culling/mesh render callbacks.
Actual asset-manager resource ABI allocation, optimized static and subscene
branches remain the previously documented explicit resource-domain limits.
