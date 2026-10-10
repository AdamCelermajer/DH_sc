# Lane 23 handoff: Combat and world audio

## Change

`renderer_animation_sound_v4.inc` now snapshots the actual borrowed Vox manager identity before querying the player's target position, then places that captured identity into the unchanged source `false / 1 / -1 / -1` Play3D request. This matches the original call order at IDA ARM callsites `CharAnimator::_PlayItemSwooshFX` (`0x3c94f8`) and ordinary animation step (`0x3ca898`): load `VoxSoundManager::s_instance`, call `GameObject::GetTargetPosition`, then call `VoxSoundManager::Play3D`. The borrowed Application manager remains retained for delivery. The named-animation leaf already snapshots its manager before position lookup.

## Existing active owners and wiring

The lane's source files already participate in the active renderer: `renderer_combat_sound_v2.inc` routes the skill sound phase through `character_combat_sound_v1` and `submit_captured_audio_v46`; `renderer_animation_sound_v4.inc` publishes ordinary step/weapon and authored `sfx_` cues; `renderer_campaign_audio_services_v68.inc` supplies the current Campaign/World listener, source settings, random stream and source emitter modifiers; `source_campaign_script_audio_v117.cpp` handles authored Play/Stop Sound and Level Music commands. `renderer_campaign_producers_v46.inc` also forwards target-event and container cues.

The full positive playback path is the current `Application`'s `AudioCampaignBridgeV46::submit` into its retained audio runtime, catalog, actual source listener/emitter fields, mixer and sample bank. `vox_play3d_owner_v2` retains original Play3D gates and per-operation failures; `vox_audio_bridge_v34` maps source SoundAutoGen ordinals to the actual XML catalog and reports missing source rows, selected UIDs, output readiness or source emitter services at the reached operation. Container precache adapters in `renderer_container_audio_bindings_v45.hpp` read sound IDs from the same canonical receiver/table.

## Remaining integration and limits

Root owns the shared `model_renderer.cpp`/application publication and build. Check that the completed campaign's audio services are published before gameplay cues and that all active player/NPC/container/item callsites reach the retained publisher; the active file fragments already present their narrow interfaces, so no parallel audio owner is required. Do not use player-only identity for monster cues.

Audio is not fully accepted as audible playback. Positive playback still depends on the integrated current-World service and actual output lifecycle. The source platform/JNI `nativePlaySoundBig` and `nativeStopSoundBig` routes remain explicit required-provider failures when the original platform flag reaches them; network mute needs the actual online state. The tracked exact-asset audit reports 354 missing and 2 partial of 638 source bindings, and separately records absent chest-opening and loot drop/pickup clips. Preserve each selected sound UID's failure; no substitute clip or success/readiness signal was added. Script `Play` intentionally follows its original ignored invalid-file return, while the transport retains the per-UID unavailable diagnostic.

This is a source call-order correction only. No build, tests, APK, emulator, ADB or audible gameplay validation was run in this lane.

## Read-only lane 01 schema support: legacy sound cache

IDA confirms the current `Listeners` record descriptor needs one type correction: `Structs::Listener::read` (`0x4ed978`) reads fields at `this+4/+8/+12/+16` as four `int32`, `this+20` as `float32`, and `this+24` as `int32`, so the six-field descriptor is `iiiifi` (the existing `iiiiii` has the wrong fifth field type). `Arrays::Listeners::read` (`0x4b9578`) reads an unsigned row count, allocates stride `0x1c` including its native vtable, then invokes each row reader; `readNames` (`0x4b0c68`) loads an equal-count member-name array. The `PyDataArrays` caller at `0x4c062c` passes the actual `Arrays::Listeners::read` and `readNames` functions and registers class `Listener` against `Listeners`.

Direct read-only parsing of `port/android-native/app/src/main/assets/data/sounds_pyarray.bin` and its names stream confirms the serialized order and bytes: CharSounds has 3 rows and ends at `0x62`; Listeners has count 5 at `0x62`, 24-byte rows starting at `0x66`, and ends at `0xde`. The names are `AAA_DONT_DELETE_Listener`, `curListener`, `PlayerListener`, `BAPListener`, and `CameraListener`. Interpreting field five as float gives rows `(3,2500,0,3150,1.0,0)`, `(2,1800,2,1000,1.0,0)` twice, `(1,3500,2,1500,1.0,0)`, and `(0,2500,0,3150,1.0,0)`.

Neighbor audit found no nearby wire-shape omission: CharSounds `[i][i][i][i]bb` matches its four count-prefixed int vectors and two bools (`Structs::CharSounds::read`, `0x4eade4`); SoundBankPlayback `ii` matches `0x4b9434`; SoundGroupsRouting `Si` matches its string and int (`0x4eb784`); and Sounds_bak `iiSiiiiii` matches `0x4eb89c`. The actual stream counts are 27, 8, and 183 respectively; parsing these arrays in order ends exactly at EOF `0x2c1f`. No code or registration was changed by lane 23.
