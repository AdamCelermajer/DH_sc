# Same Item factory before source Stage29

SOURCE ONLY; shared Root read-only. No tests, compiler, runtime or hash jobs.

Original source: static Level.cpp initialization3f6848 invokes ItemManagerC1
3eac00, whose vector4/8/c starts empty. Actual Level.Stage29 calls PreCache3eb6e4
at3f7c78; it spawns5 Item per audiovisual row and calls InitOnce3ece80. These are
distinct source phases. Item340d38 C1 can therefore exist before PreCache;
its InitPost3ebca0 is literal BX LR and does not initialize the145 pool.

Apply item-factory-ordering-v88.patch narrowly to CURRENT three files:
world_loot_gameplay_v23.hpp/.cpp and renderer_item_candidate_v57.inc.
prepare_source_factory_v88 binds existing genuine sources and constructs the SAME
empty owner/factory/pool; it does not spawn/InitOnce/publish145.
precache_source_pool_v88 runs only that owner's once-only actual PreCache.
Legacy initialize still performs preparation+PreCache, and original renderer
stage29 admission remains unchanged.

Main native death/loot composition must prepare once BEFORE XML from genuine SAME
World/manager/property map/PM/target/Gear/cache/roots/floors/nav/PhysicalWorld and
real graph/resource providers; keep that actual owner lease alive. Its existing
classfactory chain calls renderer_source_item_receiver_v88 for authored Item.
At actual published Level.Stage29 call precache_source_pool_v88 on THAT owner.
Keep the existing actual phase29 guard in the stage hook; do not initialize
another WorldLootGameplay or replay InitOnce. Kill/drop readiness remains false
until the real pool completes. A released/failed preparation cannot admit Item.

Current Root has no Application/candidate-published Item owner. Legacy V44/V23
construct it only at stage29 and require PlayerSkillsRuntime.targets/
player_manager, global level.native_floor/player_equipment and actor_world.
Those legacy unique owners cannot be inferred as ready source campaign services
before XML. Main's actual native composition must provide those genuine owner
bindings; this patch supplies the ordering split, not fake early readiness.
No full renderer provider replacement or second classfactory bind is supplied.
