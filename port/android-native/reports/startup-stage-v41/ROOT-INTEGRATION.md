# V41 measurement integration and loading extraction contract

This is a prepared integration contract, not a deployed staged loader. Parent
owns MainActivity/native_app/model_renderer/OriginalUiSession/CMake changes and
the physical phone. New telemetry files compile independently. No current
producer is bypassed and no Level phase, current-Level pointer or percentage is
inferred from these measurement stages.

## Wire the measurement before changing scheduling

Add `startup_stage_telemetry_v41.cpp` and `startup_stage_telemetry_jni_v41.cpp` to
the existing `dh2_native` CMake target. The new Java companion already lies in
the app source directory; no NativeBridge method changes are required. Ensure
the existing native library is loaded before calling its native entry points.
The companion performs no implicit library load, logging, request or scheduling.

Parent should gate capture with an explicit debug intent/configuration, for
example `startup_trace`. At Activity entry retain a Java `System.nanoTime()`
origin locally. Once NativeBridge has loaded its existing library, call
`StartupStagesV41.begin(COLD_START, origin)` and retain that generation only for
measurement. On actual subsequent GL-context recreation start CONTEXT_RESTORE;
on the actual front-menu demo launch start DEMO_LAUNCH. Finish/cancel and save the
prior report before replacing its generation. SOURCE_CAMPAIGN is reserved for a
real future source launch and must not label the current Crypt demo.

Java scope example, using the actual output length and explicit success:

```java
try (StartupStagesV41.Scope trace = StartupStagesV41.scope(
        startupGeneration, StartupStagesV41.JAVA_ASSET_READ, 0)) {
    // Existing actual asset read stays here, unchanged.
    trace.succeeded(out.size());
}
```

Native source scopes use the same recorder/TIDs/generation:

```cpp
#include "startup_stage_telemetry_v41.hpp"
// Inside the existing try block, so real exceptions unwind the scope:
dh2::startup_v41::Scope trace(dh2::startup_v41::Stage::world_load);
```

For functions that return false/error strings without throwing, call
`trace.fail()` on the actual failed branch. Do not derive success from "scope
returned". `output_bytes(n)` records the actual source result size when known;
unknown bytes remain zero and must not be converted into guessed totals.

| Boundary | Stage | Actual source anchor / thread |
| --- | --- | --- |
| Activity layout and asset listing | activity_create / asset_listing | MainActivity.onCreate; main thread |
| Surface creation and native initialization | surface_create / native_initialize | MainActivity.onSurfaceCreated and NativeBridge.initialize; GL thread |
| Actual authored GPU probe | shader_validation | native_app.validate_pixels; GL thread |
| Two UI GPU owners | ui_gpu_initialize / front_initialize | original_ui.initialize and front_ui.initialize; GL thread |
| Valid dimensions | surface_resize | NativeBridge.resize before loadSelected; GL thread |
| Selected developer/default load | load_selected | MainActivity.loadSelected; GL thread |
| Descriptor/provenance reads | java_asset_read / java_provenance_read | actual InputStream/ByteArrayOutputStream and toByteArray branches; GL thread |
| JNI descriptor copy | jni_input_copy | native_app.loadWorld allocation/GetByteArrayRegion; GL thread |
| Full world work | world_load | model_renderer.load_world existing try block; GL thread |
| Immutable design source reads | design_assets | existing actual data-table/constants reads; current GL thread |
| Descriptor/floor parse | world_parse | existing actual descriptor/layout/floor services; current GL thread |
| Object model/animation banks | object_resources | group/resource reconstruction loops; current GL thread |
| Model texture CPU/GPU split | texture_decode / texture_upload | actual dh2_texture_decode vs glTexImage2D; GL thread |
| Native player/C1 | player_construct / level_c1 | existing same-world actor/equipment/actual C1 calls; GL thread |
| Actual publication | world_publish | existing successful candidate-to-live assignments; GL thread |
| HUD first preparation | hud_load | prepare_player_frame→Impl.load; GL thread |
| HUD data and movies | ui_constants / ui_localization / swf_parse | actual constants/localization/movie.load calls; GL thread |
| SWF image/font producer | ui_bitmap_decode / ui_bitmap_upload / ui_font_upload | actual bitmap export and GPU.image/font callbacks; GL thread |
| First live HUD binding | hud_bind | gameplay/status bind+activate; GL thread |
| First submissions | first_world_submit / first_hud_submit | only after actual successful model/HUD draw; GL thread |
| Native frame callback return | native_draw | actual NativeBridge.draw scope; GL thread |
| Front-menu transition | menu_launch | native_app.consume_launch_request branch; GL thread |

WAIT_VALID_SURFACE is optional measurement of the actual gap between context
initialization and first valid resize. Avoid opening a nested span that outlives
its parent method; source time anchors can establish that gap without pretending
to time an SDK-main-thread wait from app code. No instrumentation is inserted
into framework files.

End capture only after the actual selected front menu or world+HUD submission
succeeded and its first frame callback returned, using MEASURED_RETURN. That
outcome means **measurement ended**, not Level.Init completed or pixels presented.
Use FAILED/CANCELLED on genuine failure/lifecycle cancellation. An EGL swap or
SurfaceFlinger presentation occurs outside these source callbacks and needs
parent physical evidence. Never finish merely when ready=true or loadedAsset is
assigned: ready is the surface/input gate; loadedAsset is assigned after the
reported load succeeds and still does not prove first HUD submission or GS Init.

`finish()` returns bounded JSON without logging. Persist it after the first frame
outside the critical GL callback, or chunk it for diagnostics; one Android log
line can truncate a complete report. Default includes34 aggregate stage slots
only; includeEvents adds up to128 bounded samples. Inclusive/exclusive spans are
**wall times**, including blocking, not CPU-only or GPU timer queries. Do not
sum inclusive nested stages or overlapping worker times into startup latency.
Java-origin time is stored separately; cross-language clock-domain alignment is
not presumed. TIDs are native Linux TIDs for both Java JNI spans and C++ spans.
Invalid nesting/open spans/dropped events are explicit diagnostics.

## Actionable extraction contract after measurement

1. **Do not place full world work inside the resize handshake.** Keep the valid
   resize/camera ordering fixed by parent. Return from onSurfaceChanged after
   resize/bootstrap. Simply queueing the same monolithic load to the GL thread
   does not establish responsive loading: event processing and draw may still
   block before meaningful presentation. Require a demonstrated first visible
   menu/loading frame before expensive continuation, then measure it.
2. **CPU-prefetch boundary:** a worker may own immutable bundled descriptor,
   provenance, model/animation/raw design/texture bytes and independent local
   decoded pixels. It must retain a genuine AssetManager global Java reference
   or an actual owned APK descriptor for the full job, use separate asset handles
   and request/source-generation identity, and register bytes with the existing
   resource ledger. OriginalCacheAssets positional descriptor reads are a useful
   source boundary, not permission to share its mutable caller maps unsafely.
3. **GL-commit boundary:** shader creation, textures/FBO/buffers, SwfGpu.image,
   font uploads, node scene binding and final renderer publication stay on the
   owning context thread. Bound queued bytes/items and per-frame commit work;
   retain current full-size assets/source material state and reject unsupported
   resources with their actual required error. No hidden resolution reduction.
4. **Original runtime affinity:** private Lua/native script callbacks, C1,
   Save/Profile/Inventory/VisualSkin ownership, PlayerManager/ObjectManager,
   MenuManager and Application/SceneManager changes stay with their actual
   services until those services explicitly support a worker. Moving the whole
   load_world function to a worker is unsafe: it currently mixes GL calls and
   live native source publication. Encoded/raw prefetch is the first extraction;
   genuine initialization requires the loader's exact provider contract.
5. **Request identity/lifetime:** a prepared packet belongs to the exact selected
   descriptor/layout, source/cache generation, actual Save/profile selection and
   context generation. Cancellation/Activity recreation must stop publication of
   stale data, release its own bytes and preserve actual source failure/lifetime
   rules. Never create a second Save authority or synthesize class defaults.
6. **Lazy HUD continuation:** the first prepare_player_frame currently reads UI
   constants/localization and constructs SWF/font/bitmap resources synchronously.
   A separate immutable UI-data preparation packet can read source inputs before
   the first gameplay frame; original timeline construction/native callbacks and
   GL uploads retain their proper owner. Avoid advancing a hidden timeline or
   callbacks merely to prewarm pixels. Front UI has its own loader/GL owner and
   must be timed separately, not assumed to load HUD.

## Missing genuine staged-loading producer

The current demo calls construct_native_level_c1_v25 with native_gs_launch=false.
Its real C1 is retained; the current physical log explicitly reports phase130=0
and GS/Level.Init campaign contract pending. The V27 true branch has genuine GS
publication/loading services, but whole Init/LoadProcess/Unload/destruction and
required reached online branches remain required dependencies. Root's external
loader session owns those source producers. No live phase38/current GS is
manufactured here.

To activate the authored menu_Loading path, the loader must supply the actual
selected source Level lifecycle and typed progress fields/services and bind it
to the shared native menu/Application graph. The renderer must supply the exact
original scene/module/GPU commit services required at each reached phase, with
valid camera and context ownership. Until that contract is reached, demo CPU/GL
continuations may have development telemetry but cannot claim original loading
phase completion or saved first-chapter restoration. Stage counts are neither
source progress percentages nor proof that a complete level is ready.
