# Actual chest table and visual dictionary callbacks

`OpenableContainerDataConnectionV11` is one shared immutable cache owner for the World. Its `load` takes the genuine isolated `Arrays::OpenableContainers` record/name groups and complete `game_objects_dictionary_pyarray.bin`/`game_objects_dictionary_pyarraynames.bin` streams. The existing `OpenableContainerTableV1` parser lives in `openable_container_owner_v1.{hpp,cpp}`; there is no separate table TU.

Actual bundled input copies are preserved under `port/level-loader/vendor/connected-owners-1b96f4edb021619d/reference/openable-container-v1/cache`. OpenableContainers is source group5 with68 rows. For these exact hashed assets, records are the half-open slice `[3031,5007)` of `game_objects_pyarray.bin` (1976bytes), and names are `[1749,3614)` of `game_objects_pyarraynames.bin` (1865bytes). Both slices INCLUDE their uint32 count68. Supply those matching isolated groups, not entire multi-group streams. Frozen `reference/openable-container-data-v11/group-extraction-proof.json` records asset/slice hashes; the two extracted `.bin` files are also included. Extraction matches the exact bytes used by the105-check native fixture and does not assume these offsets for other assets. The separate complete dictionary streams have85 rows and also include their count words.

The source container callback called “Visuals” indexes **Arrays::GameObjectDict**, stride12 `Structs::ColladaFile` with its filename pointer+8, then copies `strlen` bytes to the SAME base CString290 at39f974..39f9a0. `GameObjectDictionaryV11` ports its count/length/string stream; no chest-specific path table exists in the new owner. Empty/missing GetDataId results are−1; invalid dictionary indices remain explicit errors.

For each retained `CanonicalOpenableGraphV4`, bind:

```cpp
services.container.resolve_row = [data](const auto& desc, auto& id,
                                        auto& row, auto& error) {
    return data->resolve_row(desc, id, row, error);
};
// Shared construction slot, populated after graph construction.
auto slot = std::make_shared<std::weak_ptr<CanonicalOpenableGraphV4>>();
services.container.visual_asset = [data, slot](int id, auto& error) {
    auto graph = slot->lock();
    if (!graph) { error = "Required same canonical chest"; return false; }
    return data->visual_asset(graph->receiver().base(), id, error);
};
auto graph = std::make_shared<CanonicalOpenableGraphV4>(std::move(services));
*slot = graph;
```

Actual `CanonicalOpenableGraphV4::factory_receiver()` returns the one canonical class/property/InitPost receiver for the loader transport. Its existing scene, named controller and PODecor composition require the same World roots/physics plus real condition/RNG/PF/light/script/audio/loot services. These new callbacks do not manufacture those services or claim a completed unfiltered MGP.

Root CMake additions: `port/game-data/game_object_dictionary_v11.cpp` in game-data and `port/level-world/openable_container_data_connection_v11.cpp` in level-world, reusing existing `openable_container_owner_v1.cpp` and canonical graph owners. Do not link vendor copies alongside root implementations.

Android native proof PASS105 checks against actual68 container and85 dictionary rows: three real Swamp paths47/48/49, first normal chest loot227/sound33, exact same CString290, miss/empty/invalid/reload behavior and all85 nonempty dictionary records. External InitPost/physics/conditions are outside this cache-only proof. Source captures: `reference/inventory-gathering-v11/chest-binding-original.asm`; exact dictionary record virtual is `Structs::ColladaFile::read4dc424`, and actual GOT target is `Arrays::GameObjectDict::members9a6744`.
