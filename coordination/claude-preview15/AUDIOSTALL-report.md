# AUDIOSTALL report (Preview 15 blocker B039: rc4 audio cadence)

Status: IN PROGRESS (skeleton). Branch p15/audiostall, worktree DH_wt/audiostall, build DH_wt/build-audiostall.

## Question
rc4 assets (rc2 + 256 iPad audio/3d files, level_up.bdae, minimap cameras, intro_v1.mpg) with the same EXE stall the
WinMM pump 1.8-3.2 s and underrun 6-115 times in the 35 s real-time swamp run (`-Parallel 1`). Which call stalls, and when?

## Evidence so far (from reading code and logs, not yet measured)
- B039 logs: `.local-inputs/claude-preview15/verify-rc4/B039/rt35*/run.log`. rc4 world-item drop cue resolves to
  `uid=151 status=submitted` (rc2: `uid=148 status=asset_missing`), so a real WAV is now loaded on the frame thread.
- Frame log `worstFrameMs` is clamped to 100 ms in main.cpp, so a multi-second main-thread stall is invisible there.
- Producer load path: `RuntimeAudioHostV1::submit_world_item_sound` / `submit_source_sound` ->
  `AudioGameplayRuntimeV42::load_sample_actual_xml_uid` -> `AudioSampleBankV34::load` (synchronous, producer thread;
  cache by filename, LRU 64 MB) -> `AudioFilesystem::read` (`std::istreambuf_iterator` byte loop into a vector, no reserve)
  -> `audio_sample_open_v34` (header parse only; VXN 12 MB files are not decoded at open).
- Pump: `WinmmAudioOutput::update` is driven by the control thread (`audio_native_session_v42.cpp` loop, tick every 20 ms,
  wait with lock released). No mutex in the mixer path.

## Measurements
(to fill)

## Changes
(to fill)

## Tests
(to fill)

## Verifier script
(to fill)

## Open risks
(to fill)
