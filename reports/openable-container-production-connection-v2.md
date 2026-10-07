# Chest production connections V2

Existing receiver and whole derived-interaction fixtures passed on Android:
`port/level-world/reports/android-native-owner-tests/openable-container-v2/receipt.json`,
binary SHA256 eee7d53b5cd4b74fb4ed2698f0104ad7ff8023e4628e7ebf9bbd9a382705f81a.
The subsequent schema correction removes duplicate `data_name`; actual row
resolution uses ONLY registered `data_desc` string378, with cached data374.
That correction and new modules need a new combined receipt.

New concrete connections (strict Android syntax PASS):

- `OpenableContainerPropertyConnectionV1`: class-specific offsets map into the
  SAME receiver. Inherited writes delegate canonical GameObject base services.
  Factory class_name20 and PropertyMap template name must come from actual
  catalog/registration; no detached class identity is generated here.
- `OpenableContainerLootConnectionV1`: borrows SAME LootCreationV8/RNG and
  CharacterLootDropAwardV8/ItemManager pool. Allocates only original temporary
  NULL-character inventory, executes original eligibility/bonus reads, real
  generation/store and world-pool spawn. No direct Player inventory insertion.
  Canonical ObjectManager character resolver must support the type7 chest
  source and source type0 filter; Character-only registry is insufficient.
- `GameObjectSpawnProbabilityV1`: actual cached roll270/probability274,
  network108/ownerfc/byte82 and shared Random channel0/1. Original RNG helper
  has bound99. Accepted roll stores−2; failed roll reaches actual visibility,
  Delete and MarkForDeletion. Actual MeetCondition38ab60 returns literal1.
- `BaseNamedAnimationControllerV1`: actual library/animator/timeline borrows;
  original missing-name false, same inactive-clip restart, loop/scale1,
  RootNewAnim(false), scene bit200. Source callback registration is preserved.

The actual selected five SWAMP declarations, retaining document/module IDs and
XML tree, are in `reference/openable-container-v1/selected-swamp-chests.json`.
Four select Swamp_Normal_Chest→loot227; the cave declaration selects
SwampCave_Normal_Chest→loot223. All use visual47→go_chest_swamp.bdae.
Original registered spawn_prob default100 has no override in these five
declarations. Source roll0..98 and literal MeetCondition1 imply five eligible
declarations after genuine defaults and RNG side effects; this is not evidence
of constructed/visible objects or successful whole-level loading.

## Important controller distinction

VisualObject472a0c constructs BASE AnimController(root,false). Its named
PlayClip4749f4 is functional. Actual vtable969058+8+2c targets474d10:
`SetCallbacks` consists of `bx lr`. Constructor installs original DoNothing
callbacks through SetCallbacksOnAll; Container's virtual registration does not
replace them for this base controller. AnimSetController namedPlay474f68 is
literalfalse, so replacing it with a Character scheduler would alter behavior.

Direct ELF BL cross-reference audit finds controller replacement only in
CameraLevel and CharAnimator, aside from Visual construction/destruction.
No direct chest replacement was found. This is not proof that no indirect
script/external replacement exists. Do not synthesize `opened`/completion to
make a source-inert callback route appear functional. New receiver event
methods are callable only when the actual source producer delivers the event.

Relevant exact captures: `base-visual-init-source.asm`,
`load-visual-body-source.asm`, `set-visual-source.asm`,
`visual-controller-source.asm`, `controller-direct-xrefs.json`,
`eligibility-source.asm`, `spawn-random-source.asm`, `drop-and-outer-source.asm`.

## Android fixture dependencies

Property bridge: openable_container_property_connection_v1.cpp and
tests/openable_container_property_connection_v1.cpp.

Named controller: base_named_animation_controller_v1.cpp, visual_timeline.cpp,
tests/base_named_animation_controller_v1.cpp.

These are declared receiver/animator fixtures. Actual-cache object visual,
physical/PODecor, local PlayerManager, pool scene/audio, canonical registry
and source asset manager integration remain required. Root owns their generic
GameObject composition; no production graphics/body/audio success is claimed
by this handoff.
