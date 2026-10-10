This transport leaf reuses the recovered engine-audio catalog, sample bank,
RIFF PCM/IMA and original VXN segmented decoder/mixer. It introduces no sound
assets or gameplay cue guesses. Feed it the detached source event occurrence,
the source-selected catalog UID or event UID, actual emitter gains/pitch and
the authored output frame. SourceAudioRouter deduplicates by generation,
producer and occurrence; retire_generation queues owned Stop commands.

SoundAutoGen row index is NOT a catalog UID. Preserve AudioSourceBindingsV38
and original Play/Play3D gates/event selection before this leaf. Animation
audio_named_animation_sound_v38 and audio_animation_swoosh_v38 are existing
source producers; do not derive footstep sounds from generic step markers.
CharacterCombatSound and canonical audio_campaign_bridge_v46 already contain
their original hit/voice semantics. This adapter does not replace those owners.

PC output uses WinmmAudioOutput with real waveOutOpen/Prepare/Write/Reset/Close
and reports failures. Open before submitting commands, then update every frame
(four 512-frame buffers at 48kHz, about 43ms coverage); pump producer receipts.
Mixer has large inline storage: allocate it on the heap. Output owns no thread;
stalls longer than buffer coverage can underrun. Close output before destroying
the router and mixer. Command acceptance and started receipt are not audible
parity evidence. Scheduling against hardware clock remains caller-owned.

Android already has AndroidAudioOutputV34 under android-native/app/src/main/cpp:
AAudio API26+ runtime resolution, float stereo callback and hardware timestamp
mapping. Adapt its open/lifecycle/recover/close to AudioOutputServices using the
SAME mixer; do not run WinMM or a second consumer concurrently. Android API23
without AAudio remains a required backend. Original JNI nativePlaySoundBig and
online networking branches remain required services in the source owner.

Build feature_audio.cpp, winmm_output.cpp and engine-audio audio_sample_v34.cpp,
audio_catalog_v34.cpp, audio_bank_v34.cpp, audio_native_envelope_v34.cpp,
audio_mixer_v34.cpp; link winmm on Windows. Existing engine-audio sources retain
their independently audited original arithmetic; compile without fast-math.
audio_tests.cpp consumes the actual .local-inputs/audio-v34/cache reference,
checking exact mappings, real original swing decode, authored delayed start,
duplicate suppression, explicit stop, original portal loop and absent backend.

SourceAudioRuntime adds the actual retained named-sound leaf and campaign
command13/14 adapter. named_sound must be invoked from the sound leaf of the
whole melee event owner, after its step-index/count gates. It calls the existing
named owner, preserves captured manager then source target-position order and
false/1/-1/-1 positional arguments. SourceAudioRuntimeServices.prepare_3d must
execute the real Play3D admission and emitter/listener fields. Event-marked
generated rows remain an explicit required source event-selection branch.
There is no equipment/Swoosh redispatch; share the whole audiovisual prefix
result with the effects owner. Call source_frame with event.wall_timestamp_ms
minus event.lag_ms; caller supplies a genuine output-frame mapping.

Campaign command13 uses offsets8/12/13/16 exactly. Received commands skip
nonmusic playback before trace lookup. Regular Play uses row.uid directly,
including generated event rows, preserving bool13/int8/0/false arguments.
Music commands retain PlayMusic(id,bool13,int8!=0,2000) through a required
original owner callback; no native initial state is guessed. Command14 keeps
StopMusic or generated-row Stop with at most ten tracked matching UID voices.
Trace queries, regular-Play authority and actual output rate remain required.

Root composes SourceAudioRuntimeServices with actual manager/actor/listener and
single mixer owner, adds source_audio_runtime.cpp + audio_source_bindings_v38.cpp
to CMake, and mounts features/audio/assets as the feature URI source. Keep that
path alive for AudioFilesystem. Retained generation and unique detached event
occurrence keys must follow the same animation owner; use distinct producer
identities for campaign invocations. Call retire on actual generation teardown
and observe_receipt for every drained router receipt to prune terminal handles.
The optional callbacks expose requirements; successful tests validate argument
routing and actual decoding, not completion of unbound production authorities.

source_audio_tests.cpp uses actual staged Sounds streams/catalog/swing/Moth
assets. It checks generated ordinal363 -> catalog UID459, named args/lag,
duplicate transport, received command admission, regular Play, Stop and cleanup.
source-subset-manifest.json records exact asset hashes and unavailable Troll
source ordinal328 -> UID252 without replacement. MothIntro contains one source
command13, all flags zero, offset16=363; no stop commands are invented.

## Current Windows production seam

`PlatformSourceAudio` is the single owner of the heap `AudioGameplayRuntimeV42`,
catalog, decoded sample bank, channel registry, mixer, and `WinmmAudioOutput`.
`CanonicalSourceAudioAdapter` and any root marker/campaign callbacks must borrow
that same runtime. Do not create a second `SourceAudioRouter` or
`SourceAudioRuntime` in the production path.

The root process owner creates the platform session with the actual
`VoxSoundManager` identity and a persistent `AudioFilesystem` rooted at the
feature asset URI. It initializes exact source records/names/XML and priority
banks before output opens. This only initializes the recovered sound pack; the
root still supplies the source owner's full initialization/settings state.
At source startup, supply actual `AudioGameplaySourcesV40` gate leaves for the
current World/GS/Level, phase, online/network mute, platform route, and the
actual same-Level listener/emitter command. The latter must borrow the existing
`audio_source_command_v40`/`vox_source_fields_v38` authorities, completed Vox
general/settings state, and canonical Level `UpdateListener` rows/live vectors.
Bind `CanonicalAudioLeaves::play3d_authorities_ready` to a presence check for
those actual World/GS/Level and same-Level Vox command owners. Absent
authorities must keep the corresponding sound path failed.

Per frame, capture genuine QPC monotonic time in the same domain as
`winmm_monotonic_ns`, publish the hardware device clock before event dispatch,
and give its paired frame timestamp to the adapter. After dispatch, pump the
same platform output and drain its receipts through `observe_receipt` before
root consumers take them. Call `named_sound` only from the existing whole
melee-event sound leaf after its step-index/count gates; share the original
equipment/Swoosh prefix with the effects marker owner. Retire each actual
generation/actor before teardown, while keeping the process receiver alive.

The original campaign command callback passes the unmodified command, phase,
received bit, and authored monotonic time to `campaign`. Bind the trace query,
whole regular-Play body, and same-owner PlayMusic/StopMusic callbacks from the
real campaign/Vox owners. Command 13 regular Play must retain the source's
`bool13, int8, 0, false` prefix and generated row UID; command 14 Stop uses the
same runtime's loaded source sample/owned handles. World shutdown only retires
that world's voices. Process shutdown checks focus loss and
`close_and_drain`; a failed close keeps the process receiver alive.
Music callbacks must report missing same-owner providers as integration
failures; after invoking the original `PlayMusic` or `StopMusic`, they must
consume the source manager's operation result because the script dispatcher
ignores `Execute`'s return.

An authored `sfx_` marker also follows the original ignored Play3D result: once
the retained owner, actual target position, and required Play3D authorities are
present, the exact missing-datasource result is recorded and the marker is
consumed so animation continues. The WinMM smoke verifies that an unbound
World/GS gate still fails; it cannot reach the missing-marker-file branch
without real source gates.

The root link set is `feature_audio.cpp`, `platform_source_audio.cpp`,
`winmm_output.cpp`, `canonical_source_audio_adapter.cpp`, engine-audio
`audio_gameplay_runtime_v42.cpp`, `audio_clock_v40.cpp`,
`audio_source_bindings_v38.cpp`, the v34 sample/catalog/bank/native-envelope/
mixer sources, and `level-world/vox_play3d_owner_v2.cpp`; Windows also links
`winmm`. If root wires listener/spatial helpers directly, include their source
binding helpers and the actual Level/listener authority callers as well.

The adapter now links into the actual WinMM smoke against exactly one recovered
V42 runtime. That smoke exercises real decoded output and hardware clock, then
checks that a retained marker fails at the unbound real Play3D gate, retries
without false deduplication, received nonmusic command 13 skips before trace,
regular command 13 requires its missing whole-Play authority, and command 14
routes through the same V42 Stop owner. It also runs command 13 for the exact
absent source ordinal328: with the same initialized runtime and required trace
and Play leaves present, the missing Troll datasource is reported through the
diagnostic callback while campaign execution succeeds. The adapter softens
that exact reached audio-datasource failure only after the required command
providers are present; missing runtime, trace, or whole-Play providers remain
fatal.

This behavior follows the original return boundary. `Script_PlaySound::Execute`
is void and calls `VoxSoundManager::Play`, whose original body returns zero
after a missing/not-ready datasource skips emitter creation. The original
`ScriptManager::ExecuteScript` invokes command `Execute` as void, ignores its
return (including `Script_StopSound::Execute`'s int), then checks only
`IsBlocking`; both sound commands return nonblocking. For hits,
`Character::F_ApplyResult` applies HitFor before calling scrolling text and
combat sound, ignores those return values, and continues its callbacks.
`Character::F_ApplyCombatSound` itself is void and ignores the reached Play3D
result. The authored `sfx_` branch in `CharAI::_OnAnimEvent` likewise ignores
Play3D and returns handled. Required source services must still be bound before
these operations; a failed provider lookup is not an asset-miss result. The
existing combat wrapper currently collapses provider and Play failures into one
status, so its caller policy still needs that distinction before audio failures
can be made diagnostic-only there. `do_skill`'s Lua skill execution is a
separate branch; this evidence covers authored `sfx_` markers, not Lua skill
failure policy.

The smoke's target-position, trace, regular-Play, and diagnostic callbacks are
explicit test fixtures; they do not claim production root enrollment.
Current root `main.cpp` has no audio session/adapter references, so actual
campaign, listener, Play3D, and retained-marker source-authority integration
remains pending. No source328 asset substitution or audible parity claim is
made.
