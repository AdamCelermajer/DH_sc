# AnimatedDecor canonical receiver

`CanonicalAnimatedDecorV1` is factory342600/GO_ID20, with one borrowed retained
runtime and one owned `CanonicalGameObjectBaseOwnerV1`. Construct runtime before
the receiver. Constructor owns actual derived byte375=0, solid376=1,
startanim37c empty and static84=1. Shared PropertyMap routes inherited writes to
the same base and derived `is_solid`/`startanim` writes to these fields.

Use `canonical_class_receiver_v1(shared_receiver)` in the dispatcher's
`animated_decor` construction callback. Attach real SetPosition and position
closures over `receiver->base().runtime()`. `init_post` invokes the receiver's
method; IsGameObject uses the original GameObject virtual result. Deferred loader
InitFinal invokes `receiver->init_final(eligible,error)` using the same generic
initialization providers. No new registry identity, HP, AI, inventory or pose
is created by the class dispatcher.

InitPost ordering is source389128: optimization10c=1; full Decor InitPost
(generic GameObject InitPost, then visual Sync and optional PODecor ctor→whole
SetPhysicalObject); LoadFloorMap reads byte375 and skips because actual Animated
factory wrote zero; source MeetCondition38ab60 returns1; null visual ends; empty
startanim becomes literal `idle`; case-insensitive `randomall` chooses source Random(count−1),
Play(index,false), installs completion; other names use lookup→Play(name,true),
falling back Play(0,true) if absent/rejected; visual Sync→optional second PODecor
replacement; actual virtual GameObject Update. Do not deduplicate the two physical
constructor/replacement calls. Low-performance facultative visual omission and
spawn gating still run inherited base control exactly.

Timeline services must operate on actual retained VisualObject+38. Source
callback replacement must use the real successor controller, not a base method
whose original body is `bx lr`. Completion runs count→shared Random(count−1)→
Play(index,false); failed Play reaches original assertion provider. It does not
resynchronize/rebuild bodies or reinstall callbacks. Other named starts install
no new completion callback. Full inherited InitFinal remains the generic owner's
spawn/disabled/PF/light/target_node/updatePF ordering.

Services retain the actual scene/physical graph. SetPhysicalObject must release
the old owner and publish the new body in SAME base2dc. `destroy(error)` frees the
derived string before the actual GameObject destructor continuation. Base
teardown must release timeline callbacks before receiver/context destruction;
the caller must explicitly invoke it and handle any failure before dropping the
lease. C++ object destruction does not pretend an unavailable native destructor
completed.

Validation: original factory+InitPost control oracle six cases executed from
ELF, with external base/Decor/timeline/RNG/body methods explicitly recorded as
fixtures in original-control-oracle.json. Native test uses actual base/property
storage and declared visual/timeline/PF fixtures. Production resource providers
and all-module scenery acceptance still require renderer integration.

Native fixture sources: tests/canonical_animated_decor_v1.cpp,
canonical_animated_decor_v1.cpp, canonical_gameobject_base_owner_v1.cpp,
canonical_property_map_v1.cpp, game_object_initialization_owner_v1.cpp,
plus the existing source `dh2_nav_object_defaults` navigation implementation.
