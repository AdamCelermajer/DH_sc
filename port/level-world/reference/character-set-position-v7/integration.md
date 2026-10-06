# Whole retained Character SetPosition V7

Production TUs: `character_set_position_v7.cpp`, `retained_character_position_owner_v7.cpp`, both level-world. Existing actor/native-body/navigation/Box2D dependencies suffice. Existing GameObjectSetPositionV2 remains unchanged: the V7 typed borrow successor ports its whole source algorithm without constructing another CanonicalGameObjectBase.

The immediate loader393db4 block is removed by supplying actual source field storage. Constructor-null physical2dc, attached2e0 and visual2d8 select the genuine branches that do not invoke external providers. The method still writes actual Character position160, source absolute bounds and destination1a8.

## Proposed shared hooks — not applied during root freeze

RetainedCharacterActorV1:

```cpp
#include "character_set_position_v7.hpp"
// One member, alongside source_visual2d8; never a second base or runtime.
CharacterPositionFieldsV7 source_position_fields_v7_;
// C1 body, before PropertyMap/defaults/template/XML mutation:
if (!source_position_fields_v7_.construct(runtime, error_))
    throw std::logic_error(error_);
// Public typed borrowed accessor:
CharacterPositionFieldsV7& position_fields_v7() noexcept {
    return source_position_fields_v7_;
}
```

This writes source GameObjectC2 physical/attached NULL, destination0, bounds−100/+100 and nested PF constructor defaults exactly once. It must not be called lazily by SetPosition after an initialized actor. Source pointers2dc/2e0 are subsequently changed only by their actual attachment/SetPhysicalObject producers. Visual2d8 remains the already-owned source_visual field. A fresh Character record can retain one `RetainedCharacterPositionOwnerV7(actor, actor.position_fields_v7(), services)`; its `set_position(p.data(), destination, error)` directly supplies CanonicalCharacterFamilyServicesV4.set_position.

RetainedGameObjectVisualV1 should expose a narrow position-only method for source **SyncPosition470cb8**, rather than substituting full Sync with rotation/scale side effects:

```cpp
bool sync_position_v7(std::string& error) {
    const float* p = base_ ? base_->vector3(0x160) : fields_.position160;
    if (!root_present_) return true; // source root-null branch
    if (!p) return missing("SAME parent GetPosition", error);
    std::copy_n(p, 3, binding_.root.position);
    if (!binding_.update_world(scene_, error)) return false;
    return update_skinned_meshes(error);
}
```

Original470cb8 checks parent+4, then passes parent+160 to470c24. The latter checks root+8, snapshots XYZ, calls root virtual+a4 (position setter) and then virtual+b8 with argument0 (absolute transform update). Thus position-only and root-null behavior are source-backed; the typed native hook recomputes the existing scene/skin from the same Root without touching rotation/scale. The current adapter tests declare that endpoint as a fixture, so the hook still needs a same-scene native test after root releases the shared freeze. At a reached visual pointer, resolve that SAME visual through the retained family connection; no source visual means no callback is required.

## Physical binding and source order

The actual SetPhysicalObject assignment publishes the same source pointer2dc. `physical_body(id, out, error)` must resolve that exact receiver to its owned NativeBody. The adapter applies `dh2_native_body_set_position` with actual gameXY, accepts the original ignored frozen/false return, and refreshes only its same native observation. Unknown/nonexistent positive physical receivers fail; no actor-id or nearest-body fallback is used. The constructor-null path does not fabricate a physical identity.

Source order is attached-object XYZ delta (Y/Z/X subtraction capture; X/Y/Z stores), owner position sequential stores, all relative-AABB capture/copy then six owner-position additions, physical.setPosition46ea80, reread visual2d8/SyncPosition470cb8, optional sequential SetDestination393600. Aliased input is not snapshotted. Physical and visual callback failures retain the reached position/bounds prefix and do not force destination completion. Ordinary diagnostics survive.

`393db4`, `38aac8` and `393600` contain **no** UpdatePFObject, floor selection, obstacle publication, path removal, heading reset, velocity reset or AI event. Those states must remain unchanged. Runtime position arrays are semantic observations of the sole ScriptCharacterObject position, while actual PF floor/path/control state stays intact. The later original InitFinal/update/body lifecycle performs the actual PF production; a World with no PF producer is not declared initialized by this method.

## Proof

Whole original ARM393db4 +38aac8 +393600 execute for128 finite cases, four pointer aliases, all attached/body/visual/destination combinations and physical callback reentry clearing visual/changing aliased input. Physical/visual endpoints are declared fixtures. Native replay PASS1080 checks. Retained actor adapter plus actual pinned NativeWorld body position PASS16 checks, including constructor-null branch, scaling250/375 gameXY→2.5/3.75 physicalXY, source destinationfalse and missing-backend prefix. Visual endpoint remains a fixture there.

Strict ARM64 and x86_64 compile PASS. These are isolated tests; no app install, lifecycle, touch, RNG or HP mutation. Live loader acceptance awaits shared-hook integration and the next actual source provider reached.

Source producer evidence: CharacterC1 calls GameObjectC2 at3aa1c8; GameObjectC2 uses r5=0 established38c3bc and stores visual2d8/body2dc/attached2e0 at38c530/534/538; destination at38c464..46c; final bounds±100 at38c5bc..5d0 before UpdateAbsoluteAABB38c5dc; nested PF C1 at38c484. Original SetPosition has physical46ea80 and visual470cb8; it does not call full visual Sync472? or nav placement.

Runners: `.local-inputs/run_character_set_position_v7.py`, `.local-inputs/run_retained_character_position_owner_v7.py`, `.local-inputs/compile_character_set_position_v7.py`; original producer: `tools/produce_character_set_position_v7.py`.
