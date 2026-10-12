Projectile9/Laser10 canonical family — source integration

Adopt exactly these six new source files from this directory into Main's current port/level-loader:
canonical_projectile_v99.hpp/.cpp
catalog_projectiles_v99.hpp/.cpp
projectile_resource_services_v99.hpp/.cpp

Apply property-map-projectile-v99.patch (two surgical lines) to current canonical_property_map_v1.cpp. Preserve Main's current SoundEmitter/constants and other property changes; the included whole PropertyMap successor is context only. Parent supplies existing native_noncharacter_sources_v67.cmake source linkage and additional_parts_v101 composition; no second target or CMake packet is introduced here.

Before composing V69, call:
make_projectile_catalog_part_v99(scope,services,part,error,&actual_projectile_catalog)
Append part to ProductionNonCharacterServicesV67.additional_parts_v101 and retain the returned typed catalog as an existing additional catalog owner. Main's Spawn/projector provider keeps only a weak typed catalog. CatalogProjectilesV99::borrow_projectile_v99(identity,actual,error) returns an alias-owned SAME completed record/class; actual.base()/runtime() and source_manager378_v99() lend real fields to the V108 pool row. This is not a replacement manager or class constructor.

CatalogProjectileServicesV99 requires independent owner, weak SAME actual_graph, actual-scope validator, and a typed lend_platform callback. lend_platform fills the SAME class's GameObjectInitializationServicesV1, GameObjectSetPositionServicesV2 and independently owned ProjectileResourceServicesV99. Resource callbacks weakly capture current World/Level/receiver; no containing owner or whole constructor/InitPost/D0 callback is retained. The factory observes/upgrades the SAME common graph, commits a lent resource bag even before a failed graph suffix, and performs no InitPost. Existing canonical SourceFactory/Spawn owns property/template/default/InitPost order.

Bind retain_release_record/release_completed to existing V69 AuxiliaryReleaseRecord/SourceReleaseJournal callbacks. Admission occurs before real C1, including zero-identity failed-started metadata. destroy_source invokes established qualified GameObject D2 through the resource worker's exact V107 leaf. retire_storage_after_source_release_v91 explicitly routes back to THIS catalog, so journal retirement never falls through to CatalogAuxiliary. Real class completion, manager/transport unpublication and native/draw alias checks precede common graph/storage retirement. Passive C++ destruction is not native D0; failed prefixes remain pinned. Auxiliary release records expose inherited OBJS header/payload through existing make_noncharacter_record_save_v89 and preserve checkpoint28 eligibility.

Source constructor scope:
Projectile factory340d14 allocates source3d4 then C13e5e40 with GO_ID9. Laser factory340cf0 allocates3e4 then LaserC13e4b6c with GO_ID10, which continues SAME ProjectileC23e5edc/GameObjectC238c398. Constructor extra writes are exact; unwritten words remain opaque and uninitialized. Proven pointer378 is native-width uintptr_t, never truncated/copied ARM executable storage. Laser adds only C1 words3d4/3d8/3dc=0 and3e0=-1.
Both DeclareProperties/InitPost/InitFinal/Serialize/Deserialize inherit GameObject; there is no extra authored ProjectileTable default or startup/table body. Projectile.IsUpdatable3e3e0c is literal true for both, independent of actual byte85. IsZonable3e3e04 is literal false. Both D1 tails simply continue qualified base destruction; no guessed tail-resource delete is supplied.

Remaining Main native lenders:
- Actual CanonicalSpawnAttempt/receiver transport/name/pending publication using this Part exactly once.
- Main V108 CreateNativeV96 object_manager +genuine Spawn +read-only class-row projector. Row projection must NOT append again; existing source manager _Create owns its pool algorithm/publication.
- Real visible40/Stop/actual byte85 and object Delete row leaves over this actual class; SetManager uses same pointer378.
- Real common initialization/position resource leaves (conditions, native cache/visual/light/PF etc.) over SAME V68 graph.
- ResourceServices.owner/current +established qualified game_object_d2 V107. Positive Lua/network/native destruction gaps remain reached requirements.
Movement, impact, skill/physics execution and Projectile.SetInfo stay Main-owned. This family enables construction/resource transport, not a claim of those engine bodies.

No shared writes, tests, compilation, builds, hashes, proof freezes, device or emulator jobs were performed. Integrated milestone validation remains Main-scheduled.
