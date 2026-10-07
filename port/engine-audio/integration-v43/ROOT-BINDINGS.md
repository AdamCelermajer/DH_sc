# Callable root audio bindings for Stage0 and Container

Install the coherent V42 packet first: `application/native-application.patch` supplies the native constructor, actual nullable Application borrow, owner-token JNI hooks and exact CMake source list; `application/application-java-v42.patch` plus its three new Java helpers supplies shared focus and bounded producer shutdown. Copy the independent sources named by those diffs. Verify captured source hashes before applying/rebase semantic drift. V42 is already frozen; do not install a second V40 session.

The constructor hook calls `ensure_application_audio_v42(env,actualAssetManager,error)` before menu/ACT1 loader use. This constructs the actual selected pack and manager through the original GSInit phase0 audio seam, then publishes the stable THIS identity and lifetime lease. It does not force Level phase38 or assert full phase9 settings readiness.

## Stage0 StopAllSounds(500)

Include `integration-v43/audio_stage0_sound_v43.hpp`. At actual Stage0 service construction, borrow the SAME global once through `model_renderer::borrow_actual_application_audio_v42(borrow,error)`. Require a nonnull actual manager at this source-owned caller. Retain that borrow in `std::shared_ptr<AudioStage0SoundV43> sound` and bind:

```cpp
services.sound_manager_owner = sound->captured.manager;
services.sound_manager_identity = sound->captured.identity();
services.stop_all_sounds = [sound](std::uintptr_t receiver,int fade,std::string& error) {
    return sound->stop_all(receiver,fade,error);
};
```

Set `sound->captured` to the actual borrow BEFORE the above assignments. This matches the loader's `stage_loader_v46_early.hpp` callable signature. It checks the SAME receiver and exact500 argument, posts one stop to the shared native mixer, and leaves loader state untouched. It does not call `stop_world`: that would quiesce the process manager and block later ACT1 plays.

The original StopAllSounds369990 uses the actual disabled global; enabled with a null engine returns, enabled with engine requests all-group(-1) stop after float(500)/1000 =0.5 seconds. Disabled calls the song control531940(-1). Supply that genuine receiver if disabled; no empty success branch is supplied. The new wrapper converts0.5 seconds using the real published device rate. A cold engine with neither sample pins nor active voices has nothing to fade and accepts a stop without demanding focus/device readiness. If voices or queued plays exist but the driver rate is unavailable, it reports required rather than guessing. Command acceptance is asynchronous; sample pins stay until terminal receipts. No caller drains the mixer while callbacks exist.

## Container raw precache

Apply `integration-v42/loader/container-precache-complete-v42.patch` with a bound complete callback. Include `integration-v42/audio_container_precache_v42.hpp`; provide actual Application borrow and actual Container GetSound at the reached InitPost prefix. The complete helper captures a shared manager before GetSound, skips the getter only on actual null, and calls `borrow.precache_raw_uid(rawTableField8,error)`. It owns each invocation separately, including nested calls.

If root retains the loader's separated has_sound_manager/GetSound/load_sound service shape temporarily, retain an equivalent per-invocation manager snapshot: the pointer checked before GetSound must be the exact receiver passed to `precache_raw_uid`. Do not look up the global again in load_sound. Do not return false to evade a constructed manager. InitPost raw33 loads XML33 unchanged; interaction source33 maps through the generated table to XML162. Never convert InitPost33 to162 or play to precache. The decoded raw33 path already passes both real-source sanitizer runs.

## Playback and lifecycle

V42 `events/active-application-producers-v42.patch` captures actual Application identity and sends all active ordinary/named/combat/generic deliveries through manager.submit_actual_play. Root publishes actual World/GS/gates/RNG/listener/settings/source command providers using `publish_actual_playback_v42` and binds the actual authored scheduler timestamp callback. Those are genuine gameplay services, not loader precache requirements. Missing providers remain required.

Keep the Application owner outside renderer resets. Use the captured serial for independent close and producer shutdown; old/zero serials cannot affect a replacement. Successful close/join/drain precedes unpublication or releasing assets/World providers. Failed/timed-out close retains the complete owner and blocks replacement. No emulator/device/FPS work was used for this addendum. Root owns final installation, link and mainmenu->ACT1 acceptance.
