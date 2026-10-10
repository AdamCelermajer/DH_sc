# Audio writer checkpoint

Writer stopped for exclusive Luna successor as instructed. Owned feature files
are uncommitted under port/windows-foundation/features/audio/. Root main/CMake
have not been edited. No asset subset expansion was made in the latest batch.

## Current production path

`platform_source_audio.hpp/.cpp` now owns ONE heap recovered
AudioGameplayRuntimeV42, SAME catalog/sample bank/channel registry/mixer and
WinmmAudioOutput. It wraps AudioGameplaySourcesV40 with concrete output-ready
and source-command forwarding leaves. Caller original gates and source-command
context remain preserved; absent original gates fail. Do not instantiate the
older SourceAudioRouter/SourceAudioRuntime as a second production runtime.

Start: constructor(actual original manager identity, actual
AudioGameplaySourcesV40), initialize_and_open(error), set_actual_focus(actual
foreground focus,error). Initial focus is false. initialize_exact_source loads
original Sounds records/names/XML and original priority banks before output.
This is source pack initialization, NOT certification of the whole original
SoundManager Initialize/global settings/source listener operations.

Frame: publish_device_clock(error) before actual source event dispatch;
runtime().submit_actual_play for canonical Play3D, or submit_plain_source after
whole original regular Play prefix; pump(error) after events. Pump submits
real native buffers and drains original channel receipts. Root drains
runtime().take_receipt and forwards to the adapter. Source timestamps must be
the same genuine QPC monotonic domain; do not derive from dt or private now().

Focus: set_actual_focus calls real waveOutPause/waveOutRestart and publishes
clock ready=false while unfocused. Hardware clock uses actual
waveOutGetPosition(TIME_SAMPLES), wrap extension, base mixer frame and QPC.
winmm_monotonic_ns exposes that same QPC domain. Root pairs its source frame
sample with actual QPC and applies source marker lag once.

Shutdown: close_and_drain queues source StopAll, invalidates clock, checks actual
waveOutReset/Unprepare/Close success, then calls the SAME V42
finalize_after_output_closed with true closed/no-consumer-thread proof. The
output has no render thread: main/control-thread update is sole mixer consumer.
Close failure preserves receiver state and returns failure; root must retain
session. Finalized sessions cannot reopen: a new actual process receiver is
required. World cleanup should stop owned channels rather than finalize the
persistent process receiver. Destructor is best effort; call checked shutdown.

## Canonical source adapter added last

`canonical_source_audio_adapter.hpp/.cpp` consumes the SAME V42 runtime, no
second router/mixer. Strict standalone compile passed, but this last adapter
has NOT received its own runtime/integration test yet.

named_sound(event, actual_actor, batch occurrence ordinal, sampled_frame_QPC_ns,
error) invokes existing audio_named_animation_sound_v38. It retains exact
generated suffix lookup, manager-before-target position, source args
false/1/-1/-1, and delegates whole Play3D to recovered V42 original gates/event
selection/bank load/channel owner. Timestamp is paired frame QPC minus source
lag_ms. Dedup includes generation/actor/clip/slot/wall timestamp/lag/ordinal,
so loop/new-batch ordinal reuse is distinct. Call only from existing whole
melee-event sound leaf; do not rerun Swoosh/equipment prefix.

campaign(phase,command,received,authored_event_ns,handled,blocking,error) forwards
exact command13/14 fields8/12/13/16. Received nonmusic command13 skips before
trace query. Regular Play uses generated row.uid directly and passes
bool13/int8/0/false to required original plain_command authority. Source14
delegates actual V42 stop_source_sound_v106 (same ready datasource, at most10
handles, original float fade). Music uses required original same-owner
PlayMusic/StopMusic callbacks; no guessed native initial state. Cleanup uses
retire(generation,actor,error), checked SAME runtime voice handles. Root forwards
terminal receipts to observe_receipt; delivered event identity remains until
retire so completed sounds do not replay on duplicate dispatch.

Required root leaves: actual target position; whole regular Play authority;
isTracingScriptCmd source query; original Music owner. Required Play3D
AudioGameplaySourcesV40 gates: disabled/current Level identity+phase38/online/
network muted/platform route(+platform endpoint if reached)/trace. Runtime owns
sound_row, bank_info, emit, native/event selection. Actual source_command must
borrow SAME listener/emitter/DSP/general initialized authorities (existing
audio_source_command_v40 + vox_source_fields_v38 helpers, not camera guesses).
Original Listener rows/live vectors require canonical Level UpdateListener.

## Link requirements

Feature platform_source_audio.cpp, winmm_output.cpp,
canonical_source_audio_adapter.cpp; recovered engine-audio
audio_gameplay_runtime_v42.cpp, audio_clock_v40.cpp,
audio_source_bindings_v38.cpp, audio_sample_v34.cpp, audio_catalog_v34.cpp,
audio_bank_v34.cpp, audio_native_envelope_v34.cpp, audio_mixer_v34.cpp;
level-world/vox_play3d_owner_v2.cpp. Windows link `winmm`. If root uses source
spatial helpers add audio_spatial_v34.cpp, vox_source_fields_v38.cpp,
audio_listener_rows_v38.cpp, audio_source_command_v40.cpp and their actual
authority callers. Keep original arithmetic no fast-math.

## Validation completed

Earlier actual asset routing tests passed: actual Sounds streams, UID/ordinal
distinction, original swing/Moth decoder, retained args/lag, delayed start,
duplicate transport/new-loop ordinal reuse, command13 received gate and plain
call, command14 stop, generation retirement, original portal looping.

Latest `native_output_smoke.cpp` was rewritten to use ONE recovered V42 runtime
and real WinMM (no SourceAudioRouter). Strict full link/run passed with llvm-mingw
clang++ -std=c++17 -O2 -Wall -Wextra -Werror -static, winmm. It loaded actual
staged source records/names/XML, submitted source ordinal232→UID22 original
sword swing through V42 submit_plain_source, wrote actual decoded frames,
observed started channel, actual hardware samples 0→36987, actual focus pause
and checked close/drain. Missing World/Play3D gates explicitly failed. Exit0.
Direct transport test used verified original fresh-emitter gain/pitch constants;
it does NOT certify whole source regular Play gameplay prefix or audible parity.
Canonical adapter strict compile only; platform wrapper full native link/run.
Temporary exe/.check.o files removed at checkpoint.

## Original cues and provenance

Existing exact subset manifest is features/audio/source-subset-manifest.json
with SHA256s. Contains original Sounds records/names, sounds.xml, sword1/2 and
Moth assets. Root explicitly denied broader staging; do not expand subset.
MothIntro genuine command13 flags8/12/13=0,16=363 → generated ordinal363,
catalog UID459, sfx_cutscene_moth.wav available/staged. TrollReturn genuine
command13 source ordinal328 → UID252 sfx_cave_troll_smash_ground.wav. Exact file
is absent from `.local-inputs/audio-v34/cache` and canonical source ZIP
C:/Users/adamc/Downloads/Dungeon-Hunter-2-HD-v1-0-2-cache.zip. Searched ZIP names
`cave_troll.*smash|cutscene_moth`; only exact Moth appeared. No filename/codec
substitution. Neither MothIntro nor TrollReturn source script has StopSound.

Original combat owner port/level-world/character_combat_sound_v1.cpp source
3afee0/3afd38 uses SAME CharSounds death/hit/attacker impact lists; actual
MP_MinimalRandoms; dead→death else amount>0→hit; impact list by target flags;
manager→Random(count,false)→target position→Play3D false/1/-1/-1. Catalog
CharacterHurt1/2/3 UIDs38/39/40, CharacterDie50 have original local cache assets;
skill WaterMasterySkill UID113 sfx_skill_mage_cold_ray.wav. Per-actor source
CharSounds is authoritative; do NOT presume Knight selection from label.
DOT must use canonical combat sound selection, no generic hit/skill cue guess.

Video_fidelity initially found local MP4 had no audio, then acquired original
Opus48kstereo audio under .local-inputs/fidelity-video-v18/immersion/audio/ and
extracted moth-267-277.wav and trungus-349-359.wav. The API runtime cannot
audition audio content. New optional bounded task: compare actual original
Moth sample to reference with signature analysis; no claim audition/parity.

## Next bounded successor task

1. Re-read source platform/helper APIs and root current integration state.
2. Add real actual-owner test for canonical_source_audio_adapter (existing
native_output_smoke proves platform V42 only), preserve one runtime.
3. Coordinate root CMake/main minimal leaves/clock/scene listener with current
integration lead. Root still owns all main/CMake/source camera/service changes.
4. Fix any actual integration result; do not fabricate missing source gates,
Listener vectors/settings/native state/source328 file or cue schedule.
5. Update feature-only receipts/docs to reflect canonical production path.

Subtle correction already made: older SourceAudioRequest gained
assign_catalog_group; regular Play uses false, preserving caller fresh/group
behavior rather than unconditionally assigning catalog group. Preferred
canonical V42 submit_plain_source already uses its original false flag.
