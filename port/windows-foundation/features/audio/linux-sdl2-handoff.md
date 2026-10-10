# Linux SDL2 output handoff

## Evidence before implementation

This is output infrastructure, with no direct original on-screen scene or
animation counterpart. The original visible/gameplay invariant is that a
source-admitted sound reaches the existing audio mix and is heard with its
selected sample, gains, authored timing, and lifetime. This backend adds no
cue selection or gameplay rule. No video frame/timestamp is claimed as direct
evidence for a platform queue implementation.

The recovered-output handoff at
`handoff/audio-v34/owned/port/engine-audio/reports/audio-v34-handoff.md`
records the same V34 mixer, sample bank, RIFF/IMA and VXN decoder as the
transport authority, plus the requirement to attach only one platform output
consumer. The current feature notes in `README.md` record the corresponding
original source boundary: `Script_PlaySound::Execute` calls
`VoxSoundManager::Play`; `Character::F_ApplyCombatSound` and authored
`CharAI::_OnAnimEvent` ignore the reached `Play3D` result and continue their
event path. Those source paths decide whether/how a cue is admitted; this
backend only carries the mix to a device.

The concrete consumer invariant is in
`port/engine-audio/audio_mixer_v34.hpp`: `AudioMixerV34::render` is the single
consumer and must run without locks, I/O, allocation, or `shared_ptr`
destruction. `AudioMixerV34::set_rate` is documented as stopped-output only.
`port/engine-audio/audio_mixer_v34.cpp`, `AudioMixerV34::render`, consumes
posted commands at output-frame boundaries, applies start frames and fades,
mixes stereo floats, and advances the observable output frame. Existing
`features/audio/winmm_output.cpp`, `WinmmAudioOutput::update`, pumps that same
mixer on the control thread into four 512-frame S16 buffers; the existing
Android `audio_output_v40.cpp`, `AndroidAudioOutputV40::data`, calls the same
render leaf from its output callback and advances silence while inaudible.
`AudioClockV40::frame_at` maps a paired device position and monotonic timestamp
to the mixer timeline, so each backend must return position relative to its
captured mixer base frame and a timestamp in a monotonic clock domain.

## Expected behavior and implementation boundary

Open one 48 kHz, stereo, signed-16 SDL2 queued-audio device and set the shared
mixer rate while output is stopped. `update` is the sole mixer consumer: it
converts stereo float mix blocks to S16 and maintains roughly four 512-frame
blocks in SDL's copied queue. `focus` pauses/resumes the device from the actual
focus value supplied by its caller. `device_clock` reports an estimated sample
position from submitted frames minus SDL's queued frames, paired with
`std::chrono::steady_clock`; the caller supplies the nonzero output generation.
`close` clears queued audio, closes the device, and releases SDL audio
subsystem ownership only if this object initialized it. `AudioOutputServices`
adapts open/update/close to the existing stable leaf contract.

The implementation is restricted to new files in this directory. No main,
renderer, CMake, existing platform header, WinMM code, source event admission,
or content selection changes are included.

## Uncertainties and limitations

SDL2 queued mode exposes application-queue depth, not a reliable hardware
sample timestamp. The reported playback position therefore omits downstream
host audio-server/device buffering and is lower-confidence than WinMM's sample
position paired with QPC or AAudio's timestamp API. SDL focus is caller-driven;
this backend does not install a window focus listener or claim operating-system
focus policy. The Linux caller and build integration have not been selected in
this bounded patch. Audio-driver availability and queue latency vary by host.

## Focused verification defined before coding

Use a fresh process with `SDL_AUDIODRIVER=dummy`, an isolated heap mixer, and no
game save. Compile the new backend against SDL2 and the existing V34 mixer
sources; open, pump, confirm mixer frames advance and the estimated clock is
valid, lose focus and confirm clock invalidation/no mixer consumption, regain
focus and confirm pumping resumes, close and reopen, and exercise failure to
open an unavailable driver. Also exercise `AudioOutputServices` callbacks.
This checks the transport boundary only; it cannot establish audible hardware
parity or complete Linux game integration.

## Verification results

Compiled the new backend and smoke with WSL Ubuntu, GCC, SDL2 2.32.10, and the
existing V34 mixer/sample/envelope sources. Both smoke modes passed:

- `SDL_AUDIODRIVER=dummy`: services open/update, mixer frame advance, valid
  estimated clock, focus loss with stable mixer frame and invalid clock, focus
  resume/pump, close, and reopen.
- Unavailable SDL driver: open returned an error and left the output closed.

This is isolated transport verification. It does not test Linux game
integration, a physical audio device, audible parity, or downstream latency.

## Session lifecycle control

The new `LinuxSdl2SourceSessionControlV1` is a control-thread owner over the
session's existing mixer, `AudioClockV40`, `AudioLifecycleGateV40`, and SDL2
output. Its lifecycle gate behavior follows the existing source contract:
`AudioLifecycleGateV40::permitted_for` requires the actual resumed/window/focus
snapshot and the current initialized source epoch; V42's control loop calls
`tick` on its dedicated owner thread and requires `shutdown` plus
`close_succeeded` before releasing the runtime/provider lease
(`audio_native_session_v42.cpp`, `control_thread` and `shutdown`). The Windows
reference implementation is `WindowsSourceSessionControlV1::tick/shutdown`.
The SDL version pauses output on denied focus, invalidates the clock while
denied and during close, and republishes the SDL queued-position estimate after
the same epoch becomes permitted. A fresh factory context is pinned by
`AudioSessionControlFactoryV42`, matching the production factory lifetime
contract. This adds no source event, cue, or gameplay behavior.

Verification was defined as a focused dummy-driver control smoke: start with an
unfocused lifecycle gate, verify no ready clock or mixer consumption, grant
focus and verify a clock is published, revoke focus and verify clock
invalidation plus a stable mixer frame, resume and verify the estimated clock
and mixer advance, then perform and repeat checked shutdown. Also reject a null
factory context. The smoke compiled under WSL Ubuntu with SDL2 2.32.10 and the
existing mixer, clock, and lifecycle gate sources, then passed all branches.
The checked close proves SDL's device handle is closed; SDL exposes no close
error return. This remains isolated Linux transport/lifecycle verification,
not Linux runtime integration or physical-device parity.
