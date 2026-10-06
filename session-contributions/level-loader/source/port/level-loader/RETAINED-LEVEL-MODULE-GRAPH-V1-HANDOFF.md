# Retained original Swamp graph and class load connection

`RetainedLevelModuleGraphV1` is an application-facing retained SOURCE
preparation component. It accepts the existing `CanonicalLevelContextV1`,
`CanonicalObjectManagerV1`, canonical class dispatch, `CanonicalModuleGraphV3`,
`ModulePFRoomsV3` and its same `floors::World`. It allocates no alternative
Level, manager, scene registry, floor world or current-Level global. The
application owns these providers and keeps the preparation as a sibling of
their retained lease, avoiding ownership cycles.

The root owns the full Level constructor and GSLevel lifetime. The frozen
GSLevel successor `d4b10856113d` was verified against all six manifest entries
and retained under `vendor/gslevel-lifecycle-v2-d4b10856113d`. Its original/native
receipt covers the OUTER GSLevel Ctor/Dtor with deeper fixtures. Use its same
existing `s_level` slot and allocation contract. This handoff leaves
`canonical_level_context_v1.hpp` unchanged: its five previously connected
constructor fields, Module offset/context and sole word150 remain one authority.
Full Level C1/EventManager/LuaScript/Arrays/table/save/online providers remain
the root's required continuation, not satisfied by calling context `create`.

## Preparation API

Create with `RetainedLevelModuleGraphInputsV1`. The class services and manager
must belong to the real retained provider lease. The supplied graph must have
been bound to the SAME roots/map/room/floor owners; creation verifies
`rooms->world() == floors`. The class does not supply platform, menu, condition,
debug, sound, networking, actual current-Level, or save restoration fixtures.

- `load_root_step()` walks the original complete Level XML, retaining source
  leases and canonical factory attempts. It collects actual completed
  Module/Block records from the same manager without filtering construction.
- `initialize_next_module()` invokes the existing class dispatch's actual
  Module InitPost once, preserving failed prefixes.
- `prepare_floors()` invokes the proven floor post_load and publishes the
  actual room collision bounds into that same floor world.
- `finalize_next_module()` invokes the same actual Module InitFinal.
- `load_next_module_sources(random)` calls the original Module Load relay
  through the same Level18c/160 fields and original MGP/MVP file occurrences.
  Supply actual application RNG selection for alternate layouts when reached.
- `status()` exposes attempted stage, exact failed stage/error and counts;
  `modules()`, `manager()`, `floor_world()`, `root_file()` and `module_files()`
  expose retained authoritative graph/source borrows.
- `capture_draw_frames()` reads each actual retained visual's cached matrices,
  material/resource data and current visibility/attachment membership. It
  returns no substitute scene or geometry. A completed map stays available
  after an object-source failure. Each frame pins resource bytes independently
  of scene-root lifetime; recapture after scene changes.
- `discard_after_owner_release(actual_release)` requires actual caller-owned
  Level/manager/visual release first, then releases source journals. It retries
  only unfinished journal cleanup and never replays a completed release.

These are explicit SOURCE preparation operations. Their grouped test sequence
is not claimed as the reconstructed complete Level::Init orchestration. The
application should use the root's actual lifecycle to order them. Map
preparation and completed object source walks do not imply gameplay readiness.
The API has no gameplay-ready flag and never publishes GSLevel's global.

## Canonical class connection and shared RNG

`CanonicalLevelClassDispatchV1` composes actual LevelConfig/Module/Block
bindings, the incoming `CanonicalCharacterFamilyFactoryV4` and actual
`CanonicalOpenableGraphV4` constructors. Pass its constructor through the
existing `CanonicalReceiverTransportV1` so PropertyMap, factory order, Add,
identity and retained manager remain unchanged. Every other catalog class
delegates to a required actual `remaining` constructor; unavailable classes
fail explicitly. No unsupported declaration is skipped.

The caller provides actual GameDesign, model dictionary, loot tables, Character
positioning/state/InitPost services and all Openable initialization/visual/
physics/table/condition/script/audio/loot services. The loader does not
implement those gameplay systems or replace the Character with a second
canonical base owner.

Supply ONE application-lifetime `ApplicationSpawnRandomOwnerV4` and its actual
owner lease. Constructor BSS zero is appropriate only before the first source
consumption; live/restored state must borrow the existing application fields.
Never reseed per object, Level, GL recreation, or from renderer demonstration
randomness. Container spawn binds the same C1 receiver using a weak publication
cell; both derived and base spawn checks use real canonical
`canonical_check_spawn_probability_v4` and its cached result. The per-container
spawn provider must return the same application ownership block and RNG
pointer. Actual online, Handle/player, full visibility and MarkForDeletion
providers remain required when reached. The loader supplies none of them.

## Current verified prefix and limitations

The source test reads original `data/scene/001_swamp.mlx`, the authored Module
MGP and actual `data/pydata/` Character/AI/class/model/loot tables from the cache.
It constructs nine Module roots with 386 attached visible mesh submissions,
16 floor clones, 19 exits and nine generated RoomZones. It retains two authored
chest factory results with original data descriptions and positions. The same
generic factory then constructs/registers the authored priest Character,
preserving `charpropsname=WanderingPriest`, empty `char_template` and original
position. It stops after overrides at required whole same Character
`SetPosition/PF/destination` (source factory SetPosition393db4). No partial pose
write is used to bypass that boundary. The map remains capturable afterwards.

Shared application RNG is connected to Module, RoomZone, Container and the
Character's inventory borrow in this private source composition. A Container
InitPost consumes and caches its real spawn result and next requires the
actual OpenableContainer GetDataId table provider. This is not a rendered or
fully initialized chest. Production also requires its Visuals/table services,
condition/script/audio/PODecor/PF providers and actual cleanup.

The incoming Character-family source check covers 433 actual nonplayer/nonfaery
rows through genuine constructor/sheets/inventory/FSM/base/class/AI/model
prefixes. That separate check intentionally stops at whole Visual InitPost
38be5c, with explicit spawn/debug fixtures. It is not evidence that the MGP
factory completed whole Character positioning or that an NPC is visible.

The Swamp source probe still declares its outer platform/debug/online/Handle/
network/sound fixtures. Named-template clone, Config87 and RoomZone87 fixes
remain documented MODERN corrections, not original constructor parity.
Whole Level creation/unload, live renderer integration, map+mobs+chests visual
acceptance, other fixed/procedural levels, persistence and gameplay remain
unfinished. The visible APK is unchanged by these native probe checks.

## Integration and verification

Select the additive loader API/class connection CPP/HPP and its existing
canonical file/transport/draw sources. Reuse the root's authoritative engine
owner targets; do not copy the vendor snapshot over newer shared headers or
create duplicate engine implementations. `tests/cmake-receiver-transport`
demonstrates one coherent standalone source graph, including genuine Character
dependency definitions needed outside the main APK's DSOs. The integration
manifest records incoming and frozen read-only dependencies and explicit
modern CPP overrides. No unresolved-symbol suppression or stubs are used.

Build using `tools/build_module_graph_source.py` with `host`, `sanitizers`,
`x86_64` and `arm64-v8a`. Run `tools/run_retained_graph_checks.py host sanitizers
x86_64` after all builds finish. The Android runner verifies AVD
`DH2_Loader_API37` before every operation and targets only emulator5590. It
does not install or replace any APK. The JSON receipt identifies source/binary
hashes, table provenance, actual reached boundaries and full_loader=false.
Root5554 and menu5580 must remain untouched.
