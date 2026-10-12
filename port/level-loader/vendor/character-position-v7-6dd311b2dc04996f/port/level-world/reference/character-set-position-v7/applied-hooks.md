# Applied Position V7 hooks and source freeze

`RetainedCharacterActorV1` now owns exactly one `CharacterPositionFieldsV7`. `position_fields_v7().attached2e0` is also the sole NPC camera-anchor slot. `source_position160_v7()` borrows the existing primary XYZ, not a cached copy. Camera consumers must require `.constructed`; no second2e0 storage is permitted.

The constructor has explicit `RetainedCharacterConstructionV7` policy. Historical three-argument callers default to `existing_actor_adoption`, preserving their runtime/PF/path/bounds/destination and leaving new source pointer projections unavailable. The canonical Character family factory explicitly selects `fresh_canonical`, invokes genuine inherited pointer/bounds/destination/PF constructor fields once before defaults and XML. `adopt(actualPhysical2dc, actualAnchor2e0,error)` publishes supplied source pointer values only and cannot reset runtime. A fresh constructor cannot replay over adopted fields. Unavailable adoption fails with a named missing physical/anchor projection.

`RetainedGameObjectVisualV1::sync_position_v7` implements the recovered parent/root-null and position-only domain over its actual Root/Scene/skin. It snapshots Position160, updates only root position, recomputes the existing graph and reskins it. Scale and quaternion remain unchanged. `RetainedCharacterFamilyVisualV6::sync_visual_position_v7(id,error)` verifies the actual attached identity before dispatch.

`RetainedCharacterPositionOwnerV7(actor,backends)` always borrows the actor's sole fields. The explicit-fields overload rejects a competing field owner. Native body resolution remains an actual receiver lookup. `publish_physical(actorId, physicalReceiverId, sameNativeBody, assigned,error)` is a checked leaf for the true source assignment/detachment points; it never infers physical state from alive/enabled.

PhysicalV1 now exposes appended `WorldNpcPhysicalServicesV1::publish_physical_v7`. The new live-projection initializer requires it for a nonnull body, delivers it at real phase3 assignment **before UpdatePFObject**, and retains that prefix on a later PF error. Destruction delivers detachment after actual body destruction while the receiver/World remain alive. The legacy initializer preserves its optional behavior; adoption still requires actual pointer projection. This expands the services/physical-holder layouts and requires a coherent rebuild of all consumers.

## Loader binding

```cpp
familyServices.set_position = [context](CanonicalCharacterRecordV4& record,
    const std::array<float,3>& position, bool destination, std::string& error) {
    RetainedCharacterPositionBackendsV7 backends;
    backends.world = context->worldPin;
    // Bind actual attached-object/physical/visual resolvers when present.
    // Before InitPost all three source pointers are genuine constructor NULL,
    // so absent callbacks are not reached and do not block393db4.
    RetainedCharacterPositionOwnerV7 source(*record.actor, std::move(backends));
    return source.set_position(position.data(), destination, error);
};
```

Bind positive visual IDs to `record.visualFamily->sync_visual_position_v7`; bind positive body IDs to the actual retained physical owner's `native()`. On real physical creation, connect `publish_physical_v7` to the retained position owner's checked publication method. Do not construct another generic base, zero pointers after init, reset a path/floor, or substitute full visual Sync.

## Current acceptance

* Whole original393db4/38aac8/393600128 cases: native PASS1080 checks, aliases/reentry/all null combinations/failure prefix.
* Actual retained actor/body plus fresh-vs-adopt guard: PASS23 checks. Visual callback is a declared fixture in that small test.
* Actual Priest/Skeleton/Slime/Ghost plus second Priest shared-cache test: PASS772 checks/185 frames. Position-only same-root translation preserves scale/quaternion, and source marker bounds remain correct. PF/Debug/FX/AI endpoints retain explicit fixture limits.
* Actual canonical Character/PropertyMap/393db4 before-InitPost prefix: PASS, same identity/XYZ/destination/bounds, constructor-null dependencies, untouched PF floor and RNG. Placement123/456/789 and source-name transport are declared test inputs, not a claim of actual Swamp coordinates.
* Actual canonical family constructor/stat prefix: PASS433 NPC rows after the explicit fresh-C1 policy.
* Strict production compile: visual/actor/physical9TUs×2ABI PASS18; position2TUs×2ABI PASS4; actual renderer/panel9TUs×2ABI PASS18.

These are isolated current-APK-linked tests, with the updated source TUs compiled explicitly; no app install/input/lifecycle was changed. Full V7 physical callback graph and live loader acceptance remain to be tested by root. Source manifest in this directory describes the applied V7 closure; V6's earlier manifest remains its historical snapshot.

Root CMake: add `character_set_position_v7.cpp` and `retained_character_position_owner_v7.cpp` to level-world. The former is now referenced by fresh RetainedActor construction. `canonical_character_family_v4.cpp` is required by the loader's actual family factory. No additional library dependency or RNG owner is introduced.
