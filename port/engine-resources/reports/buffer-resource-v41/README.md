# Generic GPU buffers and precise CPU geometry capacity V41

Frozen staged handoff; no live shared renderer, resource core, FX cache or CMake edits. New helper files already exist. Apply `buffer-resource-v41.patch` surgically. Full snapshots are compile/inspection inputs, not replacements for root's concurrent source changes.

## Completed subsystem

The generic buffer registry covers active world/model, actor-preview, equipment and loot VBO/EBO allocation, cleanup, resize and full skin/geometry updates. All allocation and replacement admission precedes GL mutation. Replacement of storage on the same GLuint reserves full old/new overlap while counting one name. Quota refusal preserves the accepted old buffer; mutation/commit failure deletes or discards invalid storage, unlocks and releases its token, and leaves the owning field zero for a source-data retry. Warm SubData checks live ownership, target, scope, context and capacity before writing the actual current skinned bytes. A zero field recreates admitted storage from that full source update.

Cleanup uses actual context generation, preventing deletion of an unrelated replacement-context buffer with a recycled number. Lost-context loot cleanup releases its own entries before remaining registry discard. `context_state_v41` returns only ready/generation under the existing lock; hot buffer validation does not copy the complete statistics snapshot.

`CpuVectorCapacityV41` admits exact numeric vector capacity before reserve/growth, including old/new CPU overlap. It rejects adoption of already-allocated unowned storage and keeps the ledger lease. Renderer vertex/index staging buffers, retained Draw CPU vertices and loot CPU vertices use these owners. `CpuGeometryStorageV41` transfers actual storage before releasing old accounting during move assignment. Deallocation precedes ticket destruction by member/local declaration order.

Production FX rendering explicitly binds the shared CPU ledger before every cache update. All five actual cache capacities are covered: packet/scratch vertices, packet/scratch u16 indices and retained source u32 indices. Tickets follow packet/scratch swaps. The source index snapshot allocation now precedes accepted packet publication; quota/allocator failure cannot hide a partially published topology. Safe cache reset deallocates vectors before releasing charges and preserves the ledger binding for retry. Portable standalone legacy/reference cache calls retain their previous unbound behavior; this does not mean every external cache user has admission.

Existing source animation clocks, skinning math, particle streams, material data, sampler state, culling and source events are preserved. UI stream/vertex-cache buffers and legacy renderer fragments not included by the current native renderer are outside this migration; existing authored FX GPU admission remains in its prior V38 owner route.

## Verification

`host-receipt.json`: strict GCC O1/O2 with ASan/UBSan each pass **76 checks**, with no warning suppression and zero final records/fake buffer names. Fake GL stores capacity metadata and at most a 256-byte sample. The fixture compiles the actual new buffer helper, staged resource core, staged FX cache and original skinning implementation.

Tests verify same-name byte peaks/object counts, shrinking, pre-GL quota refusal preserving old storage, mutation OOM and zero-name rollback, CPU capacity quota refusal preserving the accepted vector, CPU overlap/move/scope ownership, actual skin-point deformation reaching a warm GPU SubData without reallocation, bounds rejection before driver calls, update failure and source-data retry, context replacement with numeric-name reuse, exact legacy FX packet parity, unchanged/changed FX data, all cache capacities, safe reset and topology-snapshot failure preserving accepted bytes followed by a successful retry.

`android-compile.json`: **six full strict NDK compilations pass** (complete staged model_renderer.cpp, FX cache implementation and resource core for ARM64/x86_64), using root's native compile flags plus source/header VFS overlays. This validates the actual renderer call sites and changed move-only Draw storage, including front, loot and production FX hooks. No APK build, GLES driver, device or emulator operation was performed by this agent.

The staged source sweep has no remaining raw generic buffer allocation/update calls in model_renderer.cpp or the active front/loot fragments. Source-bound receipt hashes and baseline hashes are in the frozen manifest. Whole APK/device behavior and performance remain root acceptance.

## Coverage boundaries

Capacity admission is supported for the actual C++17 reserve policy validated by the host/Android builds; a different capacity policy fails closed. Standard allocator rounding and container/control-block metadata are not physical heap accounting. GPU bytes remain requested storage, not driver residency.

Immutable parsed resource/mesh graphs, source vertices already owned by Resources, rest-position arrays, palette/deformation/pose workspaces, scratch in other engine producers, UI vertex caches/streams, program/shader memory and index/map/registry overhead remain separate resource domains. They are not claimed bounded by this handoff. FX cache CPU storage is covered after explicit production binding; the underlying source geometry itself is outside that cache's ownership. The Windows OS guard remains necessary.

These checks establish resource and data-flow invariants, not a live FPS improvement or an emulator leak diagnosis. Ownership-transition GL barriers and skin update error checks must be measured by root's warm-frame acceptance before a performance claim.
