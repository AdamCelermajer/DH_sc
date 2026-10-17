# SWAMP module-zero actor-floor bridge

This isolated adapter turns the source-selected SWAMP module-zero floor data
into the `level-world` `CollisionWorld` and `Graph` borrowed by
`dh2::actor::RuntimeRequest`. It does not create a Character, physics body,
controller, or gameplay session itself. The separate
`SwampActorSession` composes those inputs for the SWAMP NativeActivity; it owns
the Character/actor frame and NativeWorld body lifecycle while borrowing this
bridge's stable collision/graph storage.

## Source mapping

`port/navigation/navigation.cpp` imports the nine selected SWAMP module
instances from the supplied MLX and `swamp.bdae` catalogue. For each selected
floor mesh it applies the full catalogue scene transform plus the checked
module-to-level placement correction from `dh2_world_place_matrix`, then
retains world-space triangles. `Surface` records keep module index, source
MLX record, catalogue node record, geometry index, source node/geometry IDs,
visibility, native `floortypes` text, parsed flags, and source triangle range.

`SwampActorFloorBridge` selects only surfaces with `module_index == 0`. The
source Navigation importer records placed BRES triangles in A/B/C order, but
the original `PFFloor` selector producer reorders those primitive indices to
C/B/A before building collision and graph data. The bridge applies this
recovered winding conversion without another transform, builds one
selector/octree per source surface, and feeds the same floor records into the existing
`floors::build_graph` / `floors::post_load` adapter. Beside every actor floor it
retains the source surface identity and a per-triangle mapping to original
surface, primitive, and source triangle IDs. The actor geometry therefore
keeps the exact source bounds/IDs/flags while owning all storage independently
of the source cache import.

Native floor-type flags are checked again from constructor defaults using
`dh2_floor_source_flags` over the original chosen type text: the authored `floortypes` value when present,
otherwise the source node-name fallback. The full result is preserved in
`FloorTraits.type`; source collision category bits are carried in
`FloorTraits.object` following the existing floor loader's layout. The actor
graph is built from those source flags, including the existing void/wall edge
rules. The diagnostic player path mask is fixed to `0x2` (water-capable); the
bridge rejects a caller requesting another mask instead of silently applying
the wrong player capability.

The source-navigation and actor-runtime data remain behind a fixed-width,
owned neutral view. Their importer namespaces and triangle getter symbols have
since been separated, and a single-image compile/link gate verifies that the
two implementations coexist. The host bridge gate still builds the source
navigation library separately, exports a neutral snapshot, then feeds that
snapshot to the actor-floor bridge test. The NativeActivity copies importer
records into that same view; it never aliases source structs as actor structs.

## Cache-backed verification

Run from the repository root:

```powershell
python port/irrlicht-android/game/tests/run_swamp_actor_floor_bridge_host.py `
  --cache ..\cache\files
```

The runner compiles the source SWAMP floor importer separately from the
actor-runtime adapter, imports the actual cache, and asserts the transformed
triangle coordinates, engine winding conversion, source IDs, source flags and floor tags across the bridge. It
compares actor-runtime collision and position validation with the source
`dh2_nav_query_actor_floor` results for the original module-zero SpawnPoint,
the one-unit rightward endpoint used by the movement diagnostic, and an
authored water-surface triangle. It also exercises an outside point, unknown
source flags, an incorrect player mask, and rejection of water when the
object's path mask lacks bit `0x2`. It rejects unknown flags, a forged claimed
water mask on the boardwalk, and a source-surface ID that differs from its
record position.

Current cache-backed result:

| Check | Source result | Actor bridge result |
| --- | --- | --- |
| Start `(1090.75, -212.202, 258)` | boardwalk node, mask `0`, height `255` | Same source surface and height |
| Move endpoint `(1091.75, -212.202, 255)` | boardwalk node, mask `0`, height `255.000015` | Same source surface and height `255` |
| Water sample `(636.4, 2140, 10)` | water node, mask `2`, height `10` for object mask `2` | Same source surface and height |
| Outside `(5000, 0, 255)` | no eligible source floor | No actor collision; position validation rejects/clamps |
| Water sample with object mask `0` | water-required source surface is not selected | Actor position validation rejects that floor |

The module-zero slice contains 2 selected source floor surfaces and 99
triangles. The tested actor graph contains 133 nodes and 430 edges. The
structured host report and its hashes are written under ignored
`port/irrlicht-android/build/swamp-actor-floor-bridge-host-checks/`.

## Limits

- Only module zero is represented. No seam links or collision surfaces from
  the other eight SWAMP modules are imported.
- The bridge adapts source floor triangles and floor-type filtering. It does
  not import MGP/MVP obstacle colliders, wall/decor bodies, contacts, triggers,
  or moving objects.
- The app's `SwampActorSession` owns and orders the recovered Character
  Coordinator, root-motion scene phase, one Box2D world step, timers, authored
  animator, actor path/rotation/subobjects, and Prince pose upload. The actor
  movement position remains source visual-root driven rather than being
  replaced by Box2D velocity.
- This module-zero slice has no imported environment bodies or wall/decor
  colliders in Box2D. Actor-runtime navigation and endpoint/path checks bound
  the player against the selected floor graph; this is not the original
  continuous collision/contact world.
- The 2D source actor-floor height query selects the nearest eligible floor by
  vertical distance. The existing actor-runtime world collision wrapper walks
  floor selectors in room order. The tested start, movement, and water samples
  agree; broader stacked-floor equivalence and query-order behavior remain
  unverified.
- This host check proves source geometry and actor-floor integration only. It
  does not build/install an APK or claim Android runtime, native physics,
  gameplay, or full-game parity.
