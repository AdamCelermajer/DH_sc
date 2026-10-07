# Resource admission and accounting V37; FX consumer proof V38

Shared configurable accounting is complete as a component. It admits requested bytes and object counts before allocation, rolls back failed reservations, preserves old ownership on quota rejection, and exposes live/pending/peak/rejection counters by resource kind and scope. This measures requested resources, not physical driver residency. The Windows job guard remains the host protection.

`resource_budget_v37.hpp/.cpp` own no GL calls. One ledger must outlive all reservations and tokens. Consumers reserve, allocate, commit, then explicitly release the token when the owned resource is deleted. A token destructor does not silently remove accounting. Context loss removes GPU charges while retaining CPU ownership. Old-context GPU reservations cannot commit in a new context.

Replacement policies differ deliberately: `same_object_storage` reserves positive growth; `separate_candidate` reserves the entire candidate including another object; `same_object_coexisting_storage` reserves the entire new storage beside the old bytes while counting one GL name. The last policy is used for FX `glBufferData`, allowing transient storage overlap without inventing a second GLuint. Active effects receive no quota exemption.

Defaults are engineering ceilings: total GPU 256 MiB, textures 128 MiB, FX buffers 64 MiB, CPU requests 512 MiB, individual GPU 128 MiB, individual CPU 64 MiB, with separately explicit bulk archive policy. They are configurable. Descriptor census supports these selected assets fitting; it does not validate every runtime concurrency pattern.

## Verification

`host-receipt.json` records strict GCC builds with `-Wall -Wextra -Werror`, AddressSanitizer and UndefinedBehaviorSanitizer at O1 and O2. Both runs pass 89 core checks and 61 FX consumer checks, ending with zero live charges. Core coverage includes replacement peaks, shrinking, rollback, limits/counts, record exhaustion, stale/foreign/double tokens, context loss, and overflow-safe texture/SWF/FX CPU capacity estimators.

The FX fixture compiles four extracted production helpers and the actual eviction helper from `renderer_authored_effect_scene_v5.inc`. The receipt binds the complete production source and extracted helper hashes. The fake driver records only names and byte metadata. It injects OOM after storage mutation, quota refusal, zero GL name, and a context loss during commit with numeric-name reuse by another owner. It verifies admission before GL calls, cleanup of both names/tokens after mutation failure, exact idle eviction, preservation of an active resource, and survival of the unrelated new-context owner. This proves the upload/cleanup/retry helpers. The unchanged-input geometry-cache synchronization gate is not modeled by this fixture; its reset/retry path was reviewed in the production source by root.

`asset-census.json` inspects 38 actual original-cache assets, reading 20,214,744 encoded bytes with 64 MiB per-file and 128 MiB total guards. It performs no pixel decode, GL call or emulator launch. Shipping image references follow the existing `scene-materials/scene.cpp` basename extraction and renderer `data/3d/textures/` lookup. Models cover the loader packet's 49 Swamp characters and 5 chests (10 distinct models), one Swamp module, Crypt module/player/skeleton libraries, three target assets, and two declared shipping fire casting/hurt assets. Full-RGBA-mip descriptor envelopes are:

| Selected family | Shared descriptor GPU bytes |
| --- | ---: |
| Swamp entity assets plus module | 45,747,106 |
| Crypt three libraries | 52,120,510 |
| Target assets | 5,593,496 |
| Representative fire assets | 5,631,252 |

These are whole-library primitive and emitter-MaxParticles envelopes plus distinct family textures, not actual visible-instance counts. Swamp activation remains unverified and its blocked faery template is recorded. Fire selection is representative shipping content, not a claim that it is the current Devastate demo resource. Shader/reflection allocations, bone state, occurrence-specific ownership, and simultaneous effect multiplicity remain outside this census.

## Integration boundary

Root owns the native ledger wrapper, JNI context lifecycle, CMake, actual renderer FX hooks, and texture integration. The fixture is source-bound to those hooks; it does not prove the APK's visual behavior, FPS, loading latency or the original emulator leak's cause. No emulator was started by this work.

This hook currently accounts FX VBO/EBO GPU ownership; it does not cap retained CPU geometry caches. `AuthoredFxGeometryCacheV34::update` can allocate packet/scratch vertices, packet/scratch indices and source-index caches before GPU admission. The reusable `fx_geometry_cache_bytes_v37` estimator calculates actual capacity bytes, but CPU reservations still need to be integrated at those producers. Other world, actor, UI target, program and asset-reader consumers must reserve through the shared ledger before their allocations before claiming whole-runtime coverage.

The 433 MiB archive is mounted through APK fd/pread backing by `OriginalCacheAssetsV1`, not retained as one 433 MiB vector. Archive size alone is not a reason to raise the CPU budget. Indexing, inflated reads and actual retained vectors still require separate accounting.
