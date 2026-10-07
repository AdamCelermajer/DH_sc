# V45 installed shared-root audio integration

The V42 Application runtime/native provider/JNI/Java/focus and CMake closure is now installed in shared root through targeted edits. Full frozen staged files were not copied over current root files. Current menu, metadata/device, source-campaign and frame-pacing changes remain in place. No compiler, WSL, test, full build or emulator was launched; current integration is source-reviewed and `git diff --check` passed. Earlier frozen V42/V43 receipts do not verify this newer root build.

## Main/status Stage0 call

`model_renderer.hpp` exports this implemented callable:

```cpp
bool model_renderer::bind_root_stage0_sound_v44(
    dh2::loader::Stage0ServicesV46& services, std::string& error);
```

Status owns `renderer_source_campaign_v55.inc`; that file was not changed by audio. Immediately after its actual `out.stage0.clean_glitch` assignment, status adds:

```cpp
if (!model_renderer::bind_root_stage0_sound_v44(out.stage0, error)) return false;
```

The live `renderer_application_audio_v45.inc` delegates to the V44 actual global capture and V43 StopAll500 endpoint. It supplies actual manager owner/THIS/callback without changing CleanGlitch or Level fields, and does not quiesce the process manager. Actual boot constructor is invoked in `native_app.cpp` before menu/source loader use. Missing/pending/failed constructor stays required.

## Loader canonical precache binding

Audio does not allocate or clone an actor, replace loader ownership, or edit canonical owner implementations. The live renderer exports generic and typed complete precache binders. They capture the actual Application manager BEFORE the actual table getter and then use raw UID loading through its SAME runtime/bank. Successful actual null skips the getter; nonnull invokes the real getter/load. No Play-to-precache, generated ordinal conversion or guessed filename occurs.

For construction-time service wiring, allocate a weak-receiver slot and populate it with the actual completed C1 result before InitPost. The slot versions avoid receiver/service cycles and allow services to be created before C1. Example for the loader's actual Openable recipe:

```cpp
auto slot = std::make_shared<std::weak_ptr<dh2::world::CanonicalOpenableContainerV1>>();
services.precache_complete_source_v42 =
    model_renderer::bind_openable_precache_slot_v45(slot, actual_openable_table);
// Loader performs its own existing actual C1 construction, then:
*slot = actual_openable_receiver;
// Continue original property/InitPost lifecycle on that same receiver.
```

For the actual Destructible recipe use `bind_destructible_precache_slot_v45(slot, actual_destructible_table)` and assign its complete callback to `services.common.precache_complete_source_v42`. The non-slot versions accept already completed actual shared receivers. Openable resolves the current canonical `fields().data_desc` against the SAME supplied table and returns raw `row.sound`; Destructible uses the actual receiver's GetDataId and that SAME table's raw `row.sound()` (-1 on missing row). Destructible has its own recovered schema and is not reinterpreted as Openable field8.

The complete-prefix service field/invocation must be installed by loader in its canonical owners: V42 loader diff for Openable, V44 canonical Container diff for Destructible. This audio change exports their real getter bindings but does not overwrite loader-owned actor definitions. Never bind an empty callback to bypass those sites. If retaining the split old has_sound_manager/GetSound/load_sound shape, preserve the same per-invocation captured manager; a fresh lookup inside load_sound is insufficient.

## Application/output lifetime

`native_application_audio_v45.inc` implements nullable global borrow and reservation/owner/close/shutdown JNI leaves. Main Activity reserves a serial before constructor IO, uses one shared front/native focus owner, and passes its serial during explicit close/shutdown. The actual Application registry retains its JNI AssetManager and exact original archive source through source owner lifetime. Context reset retains CPU audio; verified close/join/drain precedes release. Stale/zero owner tokens cannot close a new owner. Muted callbacks do not drain commands. Failed/timed-out close retains the owner and blocks replacement.

CMake installs only one V42 session and its V40 clock/control/output adapters; it does not construct a parallel V40 session. Existing V34 codec/mixer/bank lives in its linked engine library. Complete original cache ZIP packaging was already present and was not changed. Source URI/numeric binding rules are retained.

## Admission and remaining acceptance

Central root scheduler must explicitly admit any compiler leaf. No admission was requested or consumed here. Current native_app/model_renderer/Java source integration has not been compiled, linked or launched. Root/status still inserts the Stage0 call; loader wires its actual canonical services above. Positive playback still needs the genuine World/GS/RNG/listener/settings/source-command publication and actual scheduler time scope. No fabricated phase38 or initialized general settings are asserted. All-category audible acceptance remains separate and open.
