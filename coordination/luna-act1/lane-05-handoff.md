# Lane 05 handoff — Level activation and transitions

## Scope and result

Inspected all twelve assigned lane05 files and the current Level/World loader, native area-transition sequence, object-loading bridge, and source campaign runtime. The assigned source already contains the coherent lifecycle implementation; I made no C++ changes because the only concrete activation gap I found requires an owner outside this lane and cannot be safely filled by inventing one.

The existing path uses the canonical `CanonicalLevelContextV1`, actual GS `s_level`, current selected Character/Save, native StageLoader, source retirement journals, and existing resource/object graph. A confirmed transition projects the destination against the actual LevelList, retains a serialized profile receipt, closes old-world admission, drains actual GS/Level retirement, attempts fresh bootstrap once, and waits for native loading plus restoration of the same logical profile and completed Character bootstrap. Stage9 LightSet loading, Stage18 Character state setup, Stage32 warm start, Stage33 restore, Stage34 callbacks, and Stage10/17 object condition/enabled loading are enrolled on their existing stage/object owners.

## Remaining activation blocker

`source_campaign_runtime_v61.cpp` still binds `input.online_state` and `input.constructor.online_state34` to explicit failures. The native GS/C1 contracts call these only in the nonzero `GetOnline().byte5` branch; offline Act1 should not invoke them. I found only the canonical `ApplicationServicesOwnerV5::get_online_loading_v55()` / `SourceOnlineLoadingOwnerV55`, which owns COnline byte4/5. There is no existing `OnlineGameState` owner or state34 accessor in the lane’s current source. The callback must remain an explicit failure for online sessions until a canonical provider is added and wired by its owning lane; returning a guessed offline value would fabricate readiness.

Other actual loading leaves can still stop activation when their current owner has not been published (for example the PlayerManager gameplay update and native Stage34 primitives). Those errors are deliberate dependency failures, not completed gameplay evidence.

## IDA evidence and caller checks

- `0x32bdc8 Application::LoadLevel` is the original request caller. IDA pseudocode writes the selected profile/difficulty/entry/seed values, handles save and quick-save for an existing current Level, then calls `GSLevel::LoadLevel`; it resets PlayerStatManager and updates PlayerManager after that call.
- `0x386818 GSLevel::LoadLevel` is the native area handoff target. Its caller contract is the nine arguments represented by the current `GSLevelArgumentsV2`/`ApplicationLoadLevelArgumentsV114` projection.
- `0x386190 GSLevel::Ctor` constructs and publishes one Level, looks up `menu_Loading`, then consults COnline byte5. Only the nonzero branch calls `OnlineSingleton<OnlineGameState>::GetInstance()` and reads `state34` before the online overlay check.
- `0x3f3128 Level::Level` scans the authored LevelList and selects the first filename substring match. The online `state34` call is also only in the nonzero COnline byte5 branch; offline initialization proceeds through the LevelSavegame path without it.
- `0x320e98 OnlineSingleton<OnlineGameState>::GetInstance` is the original singleton accessor. Its native owner has not been found in current source, so its ABI/state cannot be supplied by the COnline byte5 projection.

IDA export consulted: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/functions.jsonl` and pseudocode for the addresses above. The original assembly/caller evidence is consistent with the source call conditions; this handoff does not claim runtime execution.

## Integration handoff

No shared root wiring changes are required for the source lane that is already present. The root may continue using `start_source_campaign_runtime_v61`, `tick_source_campaign_runtime_v61`, `request_confirmed_source_area_load_v114`, and `enqueue_confirmed_source_area_transition_v114`. Integrating a genuine OnlineGameState provider requires its canonical owner and application-level publication/wiring; that ownership is not in lane05. No build, test, emulator, ADB, APK, proof packet, hash, commit, or push was performed. Source delivery is not gameplay acceptance.

Requested Luna/high worker settings were not exposed in the tools available to this worker.
