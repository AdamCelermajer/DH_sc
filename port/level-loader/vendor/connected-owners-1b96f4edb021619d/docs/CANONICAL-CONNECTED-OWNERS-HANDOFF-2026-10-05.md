# Canonical connected owner delta

This is a callable native subsystem handoff over the earlier
`canonical-loader-owners-handoff-e8cc6acab5183a2b.zip`. It is not an accepted
gameplay APK or a claim that SWAMP can already commit a complete runtime.
The loader must use its retained XML adapter and one canonical candidate.

## Shared ownership and construction

`CanonicalPropertyMapV1` retains the shared per-class/template schema and
applies the actual defaults and XML conversion rules. Missing attributes and
present-empty strings remain distinct. `CanonicalGameObjectBaseOwnerV1`
borrows the sole actor RuntimeState for position, rotation, scale and bounds.
Its canonical Handle, room and registered-property fields remain the same
fields throughout factory, rendering, physics and lifecycle calls.

`CanonicalClassReceiverBindingsV1` dispatches typed retained Character and
OpenableContainer receivers through that schema. It retains the source/context
lease and preserves failed construction/property mutation prefixes.
`RetainedCharacterActorV1` supplies staged fields, properties, script and
animation ownership; it does not make missing NPC InitPost providers succeed.
The existing Crypt development renderer now adopts one retained graph for each
of its eleven NPCs. This adoption is separate from full generic XML construction.

`CharacterNpcInitPostOwnerV1` executes the full recovered NPC InitPost ordering
over real borrowed fields, requiring each reached helper. It includes delayed
field adoption after the real AI/script producers. Player initialization remains
a separate required path. `CanonicalOpenableContainerV1` owns its actual base,
container property connection, network fields, interaction and source loot
connection. World drops use the shared LootCreationV8/DropAwardV8 owners and RNG;
the loader must not convert a drop into direct inventory insertion.

## Initialization, visual and physics components

`GameObjectInitializationOwnerV1` executes ObjectBase InitPost, GameObject
InitPost and InitFinal. Difficulty/sound name lookups, spawn gates, field stores,
scale/bounds operations, visual/light/target-node and PF ordering are preserved.
Condition compilation, SceneManager, actual PF calls and source global catalogs
remain concrete services supplied by the candidate. False eligibility is a
source result; a missing service is a failure.

`GameObjectVisualAssetOwnerV1` owns the original replacement/load ordering.
`RetainedGameObjectVisualAssetConnectionV1` maps its visual attachment to one
typed retained visual, including root+204 association and old-visual lifetime.
`RetainedGameObjectVisualV1` retains real BRES bytes, the complete Scene, skin
controllers, original clip library, timeline and graph. Its companion
`GameObjectSceneBindingV1` composes generic authored transforms without requiring
Character-only root-motion names. Renderers use the exposed real skin positions
and instance matrices; they must not apply the owner matrix twice.

`RetainedGameObjectDecorV1` builds a real oldBox2D body and polygon shape from
the same visual bounds and base. It preserves shared Debug/collision gates,
filter arithmetic and physical attach/detach/destruction. Captured contact
Begin/Persist/End/Result bodies are genuinely empty original methods.
`CanonicalAnimatedDecorV1` provides the recovered scenery constructor,
InitPost/InitFinal and named/random animation continuation. The special token is
`randomall`; empty start animation resolves to `idle`.

The base animation controller's virtual+44 reads the real timeline `getLoop`.
Its callback is named `timeline_get_loop`. Base SetCallbacks is a captured empty
body and does not synthesize a container-opened event.

## Verification and boundaries

Both ARM64 and x86_64 main APK builds include these owners. Android receipts
under `port/level-world/reports/android-native-owner-tests` distinguish actual
APK-linked code from explicitly declared service fixtures:

- Generic initialization: 136 checks over shared base/property storage.
- NPC InitPost: ordered helper delivery and every mandatory failure prefix.
- Canonical receiver dispatch: shared schema, leases and failure retention.
- AnimatedDecor: shared fields with declared visual/timeline/physical fixtures.
- Three original chest resources: real complete graph, skinning, named clips,
  animation pose change, timeline and actual Box2D polygon/filter/lifetime.
- Revive: 288 original ARM cases and 6,528 ordered store/helper/failure checks.

The chest test declares SceneManager/PF/Debug outer fixtures. This does not
prove production level registration. Actual asset/SceneManager registration and
release, canonical PF/condition services, shared Debug and physical peer lookup
must be supplied before the loader can accept a live candidate. Nonempty
subscene, OptimizeStatic and no-colbox traversal remain explicit unsupported
branches. Modular equipment reconstruction is outside the chest visual domain.

All-nine-module SWAMP construction, eligibility, rendered mob/chest acceptance,
failure cleanup, restoration and commit remain required. The loader should
retain the previous level on a failed candidate and report the reached provider.
Root separately has a reproduced targeted-skill Hit-provider failure; no
gameplay checkpoint is attached to this subsystem archive.

## Build use

Use the existing `dh2_level_world` target as
`DH2_LOADER_CANONICAL_OWNER_TARGET` before adding the loader adapter directory.
Do not link the loader's four-file standalone canonical fallback beside it.
The archive records its main-target CMake snapshot and exact source/receipt
hashes for review. It is a delta with existing source dependencies, not a
replacement standalone project. Retain candidate, source XML and receiver leases
through callbacks and release bodies before the shared physical world.
