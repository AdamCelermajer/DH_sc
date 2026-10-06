# Module controller/root ownership continuation

This additive continuation replaces the earlier Module visual's missing
controller38 object. RetainedModuleVisualV3 now owns one heap ModuleStaticSceneV2
root through its initial factory lease. SAME SceneManager membership owns a
lease of that root, and actual VisualAnimControllerOwnerV4 owns a lease of that
same allocation. Neither metadata refcounts nor a second root/graph are created.
The root owns its parentec/gameObject204 fields, immutable resource pin and
genuine constructor-empty animator/bound-animator containers.

Source AnimController474d30 stores root then grabs before either continuation:
false calls whole SetCallbacksOnAll474cac with literal DoNothing callbacks;
true calls root removeAnimators. The native owner retains failure prefixes,
requires real nonempty animator SetCallbacks and ignores null animator entries
as whole source474c78 does. All nine SWAMP static resources have empty actual
animation libraries, so their own root list is genuinely empty.

Exact VisualD1 order473884: SetAnimController(NULL) first (deletes controller and
drops its root lease); root removeAnimators74; root remove68 (drops parent lease);
own root drop and null8. RetainedModuleVisualV3::release follows that order.
Root destruction clears its ctor-empty bound/ordinary animator lists, then native
RAII destroys its graph/resource fields. SceneManager must outlive this release;
stop callbacks, release visuals, then destroy shared registry/candidate owners.
The helper does not implement foreign-parent migration or dynamic animator drop.
Those reached branches require their actual owners. Raw foreign ARM vptr layouts
and numeric IReferenceCounted getters are not projected by native shared leases.

Existing GameObjectSceneRootRegistryV1 interface was preserved. Its membership
now pins the SAME root allocation, instead of indirectly pinning the whole visual.
Visibility/removal callbacks borrow that root; no self-visual ownership cycle.

Evidence: reference/module-root-lifecycle-v4/original.asm captures complete
RootC1, getAnimators/remove/drop and VisualD1. controller-original.json executes
whole original C1/D1 plus get/remove/drop in two modes against explicit source
Root fixtures; it does not claim complete original Root factory/destruction.
Native owner lifecycle fixture PASS binary
d47fdb896e724dc7995b165a78b02e75e1c240689022bcd4ac1aa5e98f8f09fc.
Nine actual SWAMP Module graph, canonical generated zones, InitFinal/PF/light,
real controller/root identity checks and explicit cleanup PASS binary
a3d37a29f7f878525105b0c49d0069b637366c9ca7efd635f34827bdebe5e1ef.
Fixture now finds zones through actual pending identities/handles, preserving
the corrected source manager C1 reserved-null key0 and first real key1.

Add visual_anim_controller_owner_v4.cpp to the existing shared canonical-owner
target; do not duplicate vendors or replace root CMake. This is layered over the
previous Module graph handoff. Platform/profile/network fixture services remain
explicit in the native graph test, not production Level/GSLevel acceptance.
GSLevel EventManager/Lua/save/full publication work remains queued.
