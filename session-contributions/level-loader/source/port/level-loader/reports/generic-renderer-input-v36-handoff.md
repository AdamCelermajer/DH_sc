# Generic renderer input V36

Status: host source transport proof complete; runtime actor visuals and whole Level activation remain pending.

The adapter borrows the actual RetainedLevelModuleGraphV1 and GameObjectSceneRootRegistryV1. It captures nine canonical Module root/resource submissions, 386 visible mesh submissions, the same navigation floor world, all 214 originally registered SWAMP receivers, and all 205 reached original MLX/MGP/MVP source occurrences. Original XML includes the 11 SpawnPoint declarations plus nested properties, conditions and scripts, with actual source module IDs and offsets retained. It contains no Crypt-specific geometry or authored actor list.

Already captured mesh resources remain readable after visual-root removal. Live submission, navigation and object borrowing reject a released graph, a foreign root registry, changed canonical shared-handle keys, or registry membership changes. Captures must be refreshed every frame after pose/visibility changes: validation checks identity and membership, not pose freshness. Calls stay on the runtime owning thread. A live validation currently re-captures module meshes to verify the existing retained preparation API; its performance budget has not been established.

The meaningful host proof runs the real original-cache 195-object source walk. It checks 50 Character constructors, five chest constructors, module/floor identities, resource survival after graph teardown, atomic failures, and stale-handle rejection. One additional Dummy is an explicit host-only dynamic-registration fixture, produced by the actual Dummy constructor and ObjectManager::Add. This verifies that new keys force recapture; it is not SWAMP content.

Source preparation status is exported intact, including failed or incomplete prefixes. Actor visuals, skinning and animation producers are explicitly unbound. The adapter never claims gameplay readiness or current GSLevel publication. Main must lend the actual retained Character/chest/Decor visual producers and use its genuine activation/loading fields before consuming this as gameplay input. No emulator was started.

Files: generic_renderer_input_v36.hpp/.cpp, tests/generic_renderer_input_v36_probe.cpp (generated from current source probe without modifying it), tools/run_generic_renderer_input_v36_checks.py and reports/generic-renderer-input-v36-verified.json. The script reuses the existing private host build's exact canonical compile/link graph and regenerates its owned probe source. It does not edit common CMake or common probes.
