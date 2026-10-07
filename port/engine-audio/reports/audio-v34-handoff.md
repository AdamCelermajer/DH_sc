# Audio V34 independent handoff

This is an independent source and transport subsystem for the original cache. It is not linked into the shared APK. The root chat owns integration, the single app focus owner, source World/Vox authorities, and guarded device/audible acceptance. No emulator, ADB, GLES runtime, shared renderer, `native_app`, or CMake mutation was performed here. The frozen V32 FX handoff remains unchanged.

## Production units

- `port/engine-audio/audio_sample_v34.cpp`: PCM16 WAVE, WAVE/native Microsoft IMA, retained encoded bytes, per-voice fixed block cache, source segment extents and true final-frame trimming.
- `audio_catalog_v34.cpp`: exact selected soundpack XML, 578 sounds, 63 events, 12 groups, seven banks, original random/playlist history and caller-owned RNG.
- `audio_bank_v34.cpp`: exact `data/sounds/<XML filename>` loading, filename-shared cache, producer-owned sample pins until terminal receipts. No suffix guessing.
- `audio_spatial_v34.cpp`: source Q14 panning, six distance models and Doppler leaves; caller supplies actual listener and source fields.
- `audio_native_envelope_v34.cpp`: original normal segment Q30 fade arithmetic.
- `audio_mixer_v34.cpp`: fixed voice/command/receipt capacities, source bank admission/theft, per-voice pitch/rate conversion, exact source event frame scheduling, pause/resume, gain controls, explicit native playlist/state requests.
- `port/level-world/vox_audio_bridge_v34.cpp`: retained `VoxPlay3DOwnerV2` ordering with exact UID/event bindings and actual World/Vox/driver authorities.
- `port/android-native/app/src/main/cpp/audio_output_v34.cpp`: runtime-resolved AAudio float stereo driver, device rate and timestamp, reconnect on control thread.
- `port/android-native/app/src/main/java/com/example/dh2/AudioLifecycleV34.java`: one app audio-focus/lifecycle owner, API26+.

The callback performs no file reads, allocations, locks, or shared-pointer destruction. Limits are 64 voice slots, 511 usable command slots, 1023 usable callback receipt slots, and 1024 retained producer receipts. The cache target is 64 MiB of encoded bytes; live producer/voice pins deliberately prevent eviction. Drain receipts on the producer, including the public observation queue, to maintain backpressure. There was no unbounded runtime reproduction or emulator launch.

## Integration contract

1. Add the eight C++ units above to the root build, retaining the dependency snapshot identities in the manifest. `AAudio` is dynamically resolved with `dlopen`/`dlsym`; keep the existing API23 compile target, and require a real API26+ output device. Link the usual Android `dl` dependency.
2. Keep mixer/sample bank/catalog alive until the output stream is closed and callbacks have stopped. Configure banks and device rate while output is stopped. All command producers must serialize into the one producer side of the SPSC queue; only the audio callback consumes it.
3. Load the XML actually selected by original initialization. The catalog does not select `sounds.xml` versus `sounds_he.xml` for the game. Original `LoadSound3699fc` prepends `data/sounds/` to the exact XML filename; there is no proved `_22`/`_44` fallback.
4. Bind genuine `SoundAutoGen` UID/event rows and the same source Vox manager identity. Supply actual disabled/current-level/online/mute/platform/trace gates. The production World phase zero must remain an early return. The positive phase38 test is a borrowed test fixture only.
5. The caller resolves actual source listener, emitter position/velocity, relative type, gain/pitch/ref/max/rolloff overrides, and native music state through `source_command`. Use `audio_spatial_v34` for the recovered three leaves. Preserve the real source RNG stream and timestamp into `event_frame`; the AAudio helper maps actual monotonic event time to the current output timeline.
6. Choose one focus owner for all title/UI/gameplay audio. Do not acquire a second focus request alongside existing `FrontAudio`. On integration, connect or replace its focus ownership under root control. Close/recover streams only on the control thread. Silent callbacks advance the output clock while voice cursors remain paused.
7. UI can play the exact XML UID through the bank. Native music requires an explicit initial state index and subsequent `native_state` commands for the same token. Resolve the state name from retained `sample.states`; never invent a default game state. Stop/fade/pause retain the same voice and sample lifetime.

## Native music domain and remaining boundaries

All 17 actual VXN files parse with original `VoxN/Afmt/Segm/Rule/Plst/Stat/Trsn/Grps/Grpe/Data` metadata retained. `m_title.vxn` has one state, a once-played intro and indefinitely repeated second segment. The other 16 have two infinite playlists, phase-transposed symmetric two-second fades. The normal two-state transition path uses the recovered integer Q30 envelope and source frame clock.

Rapid reversal during an active fade reaches the original extra dying-segment branch. It remains an explicit nonterminal `control_required` receipt; it never releases the live sample pin or silently substitutes another state. Arbitrary native rule/cue/group domains, terminal segment type3 envelope behavior, and full original music-controller equivalence remain unproved. Fractional-rate transition output is a modern transport adaptation, not a claim of original driver bit identity.

The cached 183-row filename-bearing logical catalog has a different schema from this executable's `SoundAutoGen::read502af4` (serialized UID+event pairs; original runtime stride12 includes its vtable). It must not be silently reinterpreted as live source bindings. Matching labels in XML is diagnostic evidence, not authorization to invent gameplay UID/event mappings. `routing-census.json` retains both catalogs and the source character/listener/type references for the root's source restoration. Missing exact XML assets, including loot drop/pick samples, remain required rather than fabricated.

## Evidence

- Full real corpus: 228 PCM WAVE, 16 IMA WAVE, 17 VXN; first, middle and tail cursor checks.
- Original executable versus compiled ARM64: 136 native IMA blocks across both segments of every VXN, 420 panning/distance/Doppler cases, 240 Q30 fade cases, eight 24-selection event traces.
- O1/O2 ASan/UBSan host replay of original fixture bytes, XML topology, positive decoded output, event frames, pause/resume, bank theft, source early/positive/unavailable gates, all native playlist startups, normal transition output, and explicit rapid-reversal pin retention. Exact totals are in `audio-v34-host.json`.
- Strict compilation of eight C++ production units for each of ARM64 and x86_64; Java11 lifecycle compilation with warnings as errors. `audio-v34-strict-compile.json` is the receipt.

These checks establish source/CPU component behavior. They do not establish APK linkage, guarded live-device behavior, audible music/SFX, real gameplay trigger delivery, or device latency. The host has reported memory-pressure reboots; device validation must use the root's guarded runtime lease and owned-process commit/headroom/watchdog controls.
