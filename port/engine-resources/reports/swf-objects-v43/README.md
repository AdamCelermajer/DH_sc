# SWF vertex, CPU geometry and authored shader ownership V43

This is a staged component handoff. Apply `integration.patch` surgically; never
overwrite complete shared files from this directory. The parent owns APK build,
integration, actual GL execution and performance acceptance. No emulator, device
or graphics driver was used for this component's host fixtures.

## Coverage

- The actual `SwfGpu` retained VBO upload callback now reserves requested bytes
  and an object count before creating or uploading a name. Failed GL upload,
  zero-name allocation and failed registry publication clean up both ownerships.
- Actual stream fallback admits its first full upload before GL creation. Later
  `glBufferData` replacements charge the full old plus new storage peak with one
  name/count. Quota rejection preserves the accepted old buffer; a failed
  mutation deletes/discards that buffer and leaves the owner empty for retry.
- The real `SwfVertexCacheV36` reserves CPU vector capacity before copying packed
  vertices. Its existing 8 MiB / 1024-entry policy, exact-byte/mode keys, LRU and
  oversized-primitive streaming policy remain unchanged. Both CPU capacity and
  retained GPU buffers participate in the same front/gameplay-scoped ledger.
- Actual per-primitive stream packing reserves its retained numeric capacity
  before growth. Existing XYUV data, matrices, UVs, alpha/blend rules, line/strip
  modes, stencil masks and draw ordering are unchanged.
- Hash-verified `GameSWF` authored programs and temporary shaders use move-only
  owners with strong ledger leases. Counts are admitted before `glCreate*`.
  Linked temporary shaders are detached before their names/counts are released;
  an active program is unbound before deleting it. Failed compile/link/interface
  validation cleans up all reached owners. Program-pair publication is atomic.
- Cleanup uses the actual ledger generation/readiness: old-context integer names
  are discarded without deleting names reused in a new context. CPU cache data
  survives genuine context loss and is admitted only once before reupload.
- `ResourceKindV37::shader` is appended after `cpu_request`, preserving existing
  kind indexes. Its default engineering count is 256. Program/shader GPU storage
  bytes are **unknown**, explicitly counted in `unknown_gpu_storage_objects`;
  zero recorded bytes must never be interpreted as zero driver memory.

## Diagnostics

The existing explicit `budget_lease_v39()->snapshot()` readiness API now includes
`peak_by_scope` for requested live + pending peaks and unknown-byte object counts
for each kind. Existing `context_state_v41()` remains the cheap generation/ready
query. No warm primitive/cache hit copies a full snapshot. Capture diagnostics at
loading/readiness/error boundaries, not once per primitive.

Portable byte limits remain unchanged. This patch has no Android-native profile
hunk; it preserves the parent's separate 512 MiB total GPU / 256 MiB texture
profile adjustment. Limits and the new shader count remain configurable
engineering policies, not original-game constants or validated residency limits.

## Evidence

`host-receipt.json` records strict GCC O1/O2 ASan/UBSan source-bound fixtures. They
execute the **exact staged production** SWF upload callback, stream/release
methods, cache and authored compile/create/release bodies with tiny fake GL
resources. Shader asset reads and source-plan creation are stubbed in that
lifecycle fixture; the production hash/parser/plan code is unchanged and compiled
in full by NDK. This is lifecycle evidence, not shader-pixel proof.

Cases cover exact cache data and warm reuse, LRU/byte/count bounds, front/gameplay
scope isolation, pre-allocation CPU/GPU quota rejection, genuine oversized stream
fallback, old + new stream peaks, OOM/zero-name failures, allocation failure after
successful upload, cache publication failure, context loss during commit, integer
name reuse, compile/link/interface failures, temporary shader detachment, active
program release, move ownership and separate shader/program count rejection.
Every case ends with zero fake GL objects and zero ledger records/bytes. Separate
existing 89-check ledger and 1049-check exact-cache/strip topology fixtures run
against the staged core/header. `android-compile.json` covers complete SWF,
authored-shader, cache and resource-core translation units plus the existing
native caller, both arm64-v8a and x86_64, strict warnings with no suppression.

## Explicit boundaries

This is requested resource accounting, not whole-process, whole-heap or GPU
driver residency accounting. Shader pack strings/parsed plans, retained SWF
display-list graphs, fonts/text registries, list/map/control-block metadata,
driver compile/link scratch and deferred driver work remain outside byte bounds.
The unused standalone triangle-expansion helper retains its original portable
API; it is not called by this actual SWF renderer. Portable unbound reference
vertex caches remain possible; production `SwfGpu` always binds before allocation.
Other recovered shader/program collections and the existing 1-pixel startup
validation probe's transient texture/FBO owners are separate migration domains.
No resolution reduction, source-clock skip, offscreen semantic skip or active
resource exemption was introduced. Actual APK behavior and FPS remain parent
integration checks; this component does not claim to solve the lethal-skill bug.
