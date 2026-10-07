# Root Act 1 source loading connection V50

Historical V50 staging report. The reviewed core migration was subsequently
applied and V55 now has direct root Play/GS frame source hooks. See
`ROOT-CAMPAIGN-ADOPTION-V55.md` for the current source state and evidence
boundaries. The details below describe the earlier staged packet and must not
be used as the latest adoption or validation claim. Pending performance/pacing
V47 remains unapplied.

## Adopt the reviewed core first

`loader-migration.patch` changes only the manifest's 25 dependency files. It
adds V36/V38/V39/V43/V44/V45/V46/V49 source drivers, migrates original scalar
134/138/13c to uint32, separates logical identity from C1 filename while
retaining seed equality, and exposes mutable GS fields only after its actual
constructor completes. It adds original manager phase7c/ordered-map views and
retained preparation source-stage methods. No old loader/world snapshot is
overwritten wholesale. All original 51 row filenames remain authored values;
SWAMP row41 is **001_swamp.mlx**, not worlds/001_swamp.mlx. Cache identity is a
separate data/scene URI, resolved by the original LoadFile search order.

New owned helpers:

- `source_load_process_prefix_v50.hpp`: complete original call envelope,
  strict missing-service errors, exact null branches, fresh source phase reads
  after callbacks, nonreentrant/failed-prefix latch. Its provider bodies remain
  required; this does not implement Save fast-travel unlock or `_DEBUG_OUT`.
- `native_root_loading_connection_v50.hpp`: retains the constructed actual GS,
  binds SAME published C1/preparation/registry and actual current global, installs
  Stage0 and Stage5 genuine source bodies, then delegates one source GS tick.
- `native_driver_unused_v50.hpp`: functional **native backend adaptation** of
  the driver cleanup dependency. See its separate hook contract below.

Root retention, approved by parent:

```cpp
std::unique_ptr<dh2::loader::NativeRootLoadingConnectionV50> native_loading_v50;
```

Place it beside `WorldScriptContext::native_gslevel_v27`. Construct once after
actual GS C1 completes and after the existing canonical source candidate graph
has been bound. Its callbacks must borrow World weakly. An aliasing manager
lease pins the existing canonical facade, which has a raw World observer; do
not pin the containing World from a connection it owns.

`NativeRootLoadingInputsV50` takes the existing actual GS/global/Application,
frame menu/Debug providers, `SourceLoadingInputsV43`, source prefix borrows,
Stage0 Application/audio and SAME PhysicalWorld. Leave unavailable providers
unbound; a reached dependency reports failure. It rejects caller-provided
current-Level, Stage0/5 and GS Load/read/update duplicates. It never constructs
another C1/GS/PM/Script/Scene/PF owner or stores phase38/readiness.

Root CMake additions to existing **dh2_level_world** target:

```cmake
target_sources(dh2_level_world PRIVATE
 ../level-loader/lifecycle_v36.cpp
 ../level-loader/level_source_loading_v43.cpp
 ../level-loader/native_gslevel_frame_v45.cpp
 ../level-loader/retained_level_module_graph_v1.cpp)
```

The three new V50 helpers are header-only. Existing native V25/V27 sources
already belong to that target. Preserve concurrent Save/Item/audio changes.

Root per-frame call belongs in the actual GS/GameSM update path before rendering
the loading roster. Do not also run the old direct Crypt world bootstrap, do
not advance SWF root again from this helper, and do not replace the actual menu
Update(true) callback with a timer or redraw. `tick()` returns source
`application_gate`, `advanced` or latched `failed`; the caller reports the
specific `error()/required_service()`. Stage38/gameplay continuation is an
explicit original provider, not a fabricated success callback.

## Functional native CleanGlitch hooks

Captured original ELF receipts prove all seven driver vtable AC slots resolve
to `IVideoDriver::removeUnused5aa290`. Its source sequence is:

1. C2DDriver.freeTextures59f300 (first actual driver virtual1fc, then two
   conditional texture-manager remove(id,false) calls, temporary intrusive drop).
2. MaterialRendererManager.removeAllBatchBaker5dad20.
3. MaterialRendererManager.clearUnusedInstances5d9e94.
4. MaterialRendererManager.removeAll(false)5da080.
5. TextureManager.clearPlaceHolders5e828c (eight raw slots; null a slot only
   when pointed receiver's source refcount4 equals1).
6. TextureManager.removeAll(false)5e9f78.

The original intrusive RB trees, shader-baker internals and source C2D/texture
manager object ABI are **not** already present in root. V50 therefore exposes a
native backend owner with explicit consumer leases and actual release hooks,
preserving source order and protecting used allocations. It does not claim
bit-identical original collection internals or force-remove an actively used
native batch. Native adaptation is explicitly allowed by parent for this
functional loader dependency; it is not an FPS optimization.

Create ONE `NativeDriverUnusedOwnerV50` retained by the actual Application's
native device/backend facet. Bind actual current context generation and the
actual 2D temporary/default binding and texture-placeholder cleanup hooks.
Each of four domains must bind its existing real allocator owner, including a
genuine zero-size allocator. Do not fabricate an empty list or bind success
closures. Retain allocator owners outside this registry: it intentionally
borrows them weakly to avoid Application/driver/cache cycles.

Allocation/consumer hook API:

```cpp
UnusedKeyV50 key{domain, actual_context_generation, actual_GL_name_or_instance_id};
driver.register_allocation(key, actual_allocator_lease,
  [/* weak actual backend */](bool lost, std::string& error) {
    // Existing actual release function: GL + its resource-ledger release.
    // lost is false during CleanGlitch; no renderer/world reset.
  }, error);
NativeDriverUnusedOwnerV50::UseLease lease;
driver.acquire(key, lease, error); // before publishing any raw consumer
```

Copying a UseLease adds a real consumer; moving it transfers one; destroying it
releases one. The count is independent of shared_ptr.use_count and raw GL names.
Store leases with Draw's VBO/EBO/diffuse/alpha bindings, active/retained effect
geometry and textures, live Program owners, SWF image/font logical caches and
retained stream/cache buffers. Reset the relevant consumer lease before the
actual allocator release and then call `unregister_allocation(key,error)`.
During cleanup's exact current release callback this call acknowledges the
same record; the outer collector erases after backend success. Unrelated or
reentrant registry mutation fails closed and latches the source failure.

Concrete shared hook sites (parent owns these edits):

- Model texture allocation/publication: `renderer_model_texture_budget_v40.inc`
  upload registry insertion; rollback registration along with GPU rollback if
  consumer publication fails. Use existing `release_model_texture_v40` for the
  actual release, with unregister preflight before GL deletion. A cleanup
  deletion must also remove that unused name from existing ownership vectors
  and memo keys so later cleanup cannot delete a reused GLuint.
- Model Draw raw uses: `model_renderer.cpp` Draw; every diffuse/alpha and buffer
  assignment and teardown. World/equipment/actor/loot copy/move must carry the
  lease. The allocation-only images vector is not proof of a Draw reference.
- Generic VBO/EBO allocator: `renderer_buffer_budget_v41.inc` create/release;
  include actual buffers used by retained actor/FX geometry.
- Authored FX texture callback/owner and FX GPU resource entry: preserve active
  source and pending owner references through source draw/submission lifetime.
- SWF: `swf_gpu.hpp/cpp` Texture, normal/premultiplied Program, stream and
  retained cache owners. Existing logical image/font cache ownership must hold
  a consumer until all dependent movies/fonts die (`reset_images` boundary).
  Draw-only last-frame activity is insufficient to certify a font unused.

All existing GL/accounting release functions remain authoritative. Register
while existing rollback scope still owns allocation, before it is exposed.
If bookkeeping allocation throws, rollback the actual new GPU object and its
ledger ticket; never publish a raw name without a lease. Context loss:
`abandon_context(old_generation,error)` detaches bookkeeping WITHOUT GL deletes;
existing backend generation-safe lost-release remains responsible for ledger
cleanup. New generation registration may reuse numeric GL names. Old leases
can never delete/release a new generation's allocation. Source cleanup failure
never retries a partially run release callback.

## Evidence and boundaries

- Prefix: strict C++17 ASan/UBSan **114 checks PASS**.
- Native cleanup owner: strict C++17 ASan/UBSan **84 checks PASS** with tiny fake
  handles and real registry/consumer/backend release hooks.
- Coherent root headers and six migrated CPPs plus facade: **14 NDK compiles
  PASS**, ARM64 and x86_64, uint32 type assertions. Known legacy compressed
  loader headers require Wno-misleading-indentation; no other warning disabled.
- Actual-cache/C1/GS root facade host probe: **2 cases, 16 atomic rejection
  attempts PASS**, logical SWAMP / authored001_swamp.mlx / actual row41;
  original Lua44/Save_ec/GS/current/manager aliases maintained. Genuine missing
  Debug file attempted once. It stops honestly before source Stage0 without
  cheat storage, and after real bigI/bigV stores at missing CleanGlitch.
  This probe uses independently frozen loader-host implementation libraries,
  explicit platform/menu/log/audio/deep-cleanup transports and root coherent
  headers; it is not root APK or full Init proof.
- No whole native cleanup hook integration, Stage0 audio integration, full
  source prefix providers, stages2/4/6+, GameSM/Mainmenu→Act1, gameplay38,
  original unload/abort/death or device testing is claimed by this packet.

Remaining dependencies are implementable source work, not reasons to fake
source success: actual cheat/debug globals; source fast-travel/Map/debug text;
real device cleanup registration above; actual SoundManager.StopAll(500);
whole MenuManager.UnloadMenu and source cache-library/event/PM stages;
same canonical class/Scene/PF source publication and final gameplay bridge.
