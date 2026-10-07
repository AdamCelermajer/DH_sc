# V59 Application PlayerManager pre-Gear bootstrap

Source frozen 2026-10-06. Compilation, host execution, Android execution and whole campaign acceptance are **pending**. The RAM hold was obeyed: no WSL, compiler, test, emulator or ADB launch was performed by this agent for V59.

## Implemented component

`ApplicationPlayerManagerBootstrapV59` retains the existing typed `PlayerManagerCombatRuntimeV2`, `PlayerManagerOwnerV1`, `PlayerNetworkLocalOwnerV4` and the actual PlayerInfo record addresses. It depends on the SAME Application and source COnline facet; it no longer requires equipment, a live Character or a current Level to construct the manager and first local record.

Fresh construction invokes the existing manager constructor body, its dummy PlayerInfo constructor, the existing query-relevant CNet C1/Reset owner, scalar PlayerInfo Reset and the actual three-slot/thirty-level signed-byte Reset owner. It leaves Character660 null, save_slot664 -1 and character_count6c4 zero. This is not a claim that CNet packet/NetStruct serialization or the complete Application constructor is reconstructed.

The original first-local prefix reads actual InputManager.GetNumGamepads34d754, applies the original clamp to one, calls actual GetGamepad(0) through virtual+8 and reads that SAME receiver's connected758 before the COnline/transport chain. Root's `SourceInputManagerV60::first_local_services()` supplies the production constructor/query owner: original fixed capacity4 and embedded slot0 with constructor-connected758=0. Source PM forces the first local connection; V59 does not mutate the gamepad field or derive hardware counts.

Original source `_CheckLocalControllers378c80` then calls IsPlayerInLocalMap36d280. Missing entry follows `_AddPlayer378e50` with exact `{internal0, controller_owner-1, local_index0, local=true}`. Existing entry borrows GetPlayerByInternalID(false), reads slot664 and never calls AddPlayer again. Native phase `first_local_retained` distinguishes that branch; `profile_input_pending()` identifies the existing slot-1 branch into the original controller/menu input tail at378e58. A successful prefix does **not** claim that tail, all other controllers, controller removal, network users or whole PlayerManager.Update succeeded.

## Adoption and native ownership transport

If an actual prior manager/network exists, `adopt_existing` requires its initialized, idle, non-development-scalar runtime, the expected exact callback/context and a lifetime pin for that context. It observes the SAME constructed dummy CNet receiver, rebinds only native service transport and moves unique ownership. It does not call initialize, Reset, AddPlayer or AddCharacter and does not normalize the old development tuple into source provenance. Existing Character660/save_slot664/map/count/network identity remain unchanged.

`lend_to_runtime` supports existing unique_ptr gameplay consumers without creating another manager. It allocates a guarded return callback before moving the SAME owners. On return, exact manager/network pointer identity and inactive source guards are checked. A changed/active owner invalidates observed App publication before any possible dangling-pointer query; it does not reset source fields. The callback must execute before the consumer's manager/network fields are destroyed. Application is weakly borrowed inside the helper and source CNet leases, preventing an App-to-PM-to-App cycle.

`renderer_application_player_manager_v59.inc` adopts the observed legacy boot or its already-transferred PlayerSkillsRuntime owners, loans them back when appropriate, publishes the typed App slot and verifies same App/control-block ownership on subsequent borrows. Legacy post-adoption allocation/transfer APIs must not be replayed; fresh source campaign callers use the App helper.

## Exact integration seam

Root includes `application_player_manager_bootstrap_v59.hpp` and the new renderer include after `renderer_player_manager_boot_v25.inc` and complete runtime definitions. Root links `application_player_manager_bootstrap_v59.cpp` in world CMake; this agent did not edit global CMake/model/menu files.

Before source Character/Gear/GS construction, the campaign calls `prepare_source_application_player_manager_v59(actualApplication, out, error)` and retains `out` even when a reached constructor prefix fails. It borrows root's `model_renderer::borrow_actual_input_manager_v60` and passes `input->first_local_services()` to `out->source_first_local_add_prefix`. App publication is separate from whole source game readiness.

V60's player/Character and V59's selected-profile Save providers borrow `get_local_player(0,false,record,error)` and the SAME manager lifetime. `record->save_slot664` must come from the actual front selected-profile producer or an explicitly named modern request transport. This helper never selects a slot, allocates Save, supplies Character660 or forces count6c4. Original AddCharacter remains delegated through `bind_remaining` to actual source callbacks, with SAME record/count field pointers. The callback/provider closure must not strongly capture App or a containing World.

## Remaining genuine lifecycle boundaries

* Original profile-input tail, whole PM.Update/AddCharacter/RemoveCharacter and online PlayerInfo178/1a0/670 producers are not implemented by this prefix. Unsupported positive operations fail with the actual missing operation, rather than returning success.
* Before an adopted legacy Character/World/runtime is destroyed or replaced, actual RemoveCharacter must run. The legacy boot pin holds a weak World and alone cannot keep the Character660 receiver alive after teardown. Retaining the field without an actual acyclic Character lifetime lease is not sufficient; V59 does not clear or dereference it as a substitute.
* The existing source manager RemovePlayer body erases scalar map records without a native release notification for additive CNet/skill-buffer facets. Full remove/re-add needs a native lifetime release seam at the real record-deletion point. This packet does not claim that destructor domain and does not alter shared removal source policy.
* Shared source GS/current Level, stage29 Item/Projectile PreCache, whole player initialization and authentic Save/Quest/GEAR completion remain separate subsystem providers.

## Validation prepared, not executed

`tests/application_player_manager_bootstrap_v59.cpp` composes the actual root InputManagerV60 positive path and existing PM/CNet/buffer bodies. Injected fixtures only cover count/GetGamepad failure order, online required continuations and old-provider adoption. Checks cover original missing/existing-map branches, pending profile tail, untouched660/664/count and gamepad cell, constructor buffer contents, exact CNet pointer identity, foreign/reentrant callback refusal, no constructor replay on adoption, original removal failure retaining Character, loan/return identity, changed-loan invalidation and weak App lifetime.

The root-admitted leaf needs these eight TUs: application_player_manager_bootstrap_v59.cpp, player_manager_combat_runtime_v2.cpp, player_manager_owner_v1.cpp, player_network_local_owner_v4.cpp, player_info_skill_buffers_v26.cpp, player_manager_loot_queries_v8.cpp, source_input_manager_v60.cpp and the test TU. Strict both-ABI syntax/link evidence and host execution receipts must be added only after actual root admission; source checks alone are not a PASS.

Original provenance: `reference/player-manager-owner-v1/original-source.asm` captures IsPlayerInLocalMap36d280, PlayerInfo.Reset373bdc, PlayerInfo.C137418c, _AddPlayer378a40 and _CheckLocalControllers378c80. `reference/world-touch-target-v1/input-symbols.json` identifies GetNumGamepads34d754 and GetInstance34dda4. InputManager constructor/body proof is owned by root's V60 packet.
