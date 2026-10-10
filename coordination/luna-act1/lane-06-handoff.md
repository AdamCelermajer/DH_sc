# Lane 06 handoff: Gameplay HUD and character entry

## Changes

- `original_ui_gameplay_bridge_v1.inc` now returns a protected ActionScript failure when a HUD action reports that its required gameplay owner is unavailable. This closes the `NativeHUDSpell` path that previously logged `Original cast owner unavailable for current player` as if the callback had been delivered. Existing cooldown/locked/no-learned-skill results remain ordinary gameplay rejections.
- The existing HUD snapshot in `gameplay_hud.cpp` runs the retained `dh2_ui_hud_player_infos_v1` producer against the current `PlayerGameplayBinding`: saved skill slots and levels, skill/fairy usability, fixed-point properties, potion count, DPad setting, and current HUD fields all come from that same player/save/gear owner. Icon names use the same saved skill rows and current skill table.
- Existing player actions route skill use through the current saved slot and `CharacterSkill` AI, faery use through saved difficulty/current faery and the retained cast controller, and potion use through the live Gear inventory, player properties, and original regeneration owner. Missing required providers are reported; no action is fabricated as accepted.
- `renderer_hud_attack_actor_v46.inc` borrows the actual local PlayerInfo Character and requires the same canonical player's object-of-interest and pending click target. Attack commands require the same live attack controller.

## Existing interfaces preserved for root wiring

- Portrait press/release and character entry are owned by `OriginalUiSession::hud_pointer`: it asks `AuthoredGameplayHudV1::geometry` for current authored hit geometry and screen-to-movie conversion, then returns the `character` command on a genuine portrait release. The session's existing portrait alignment runs against the retained HUD movie after update. Both paths are outside lane 06's writable list and were left intact.
- Joystick placement and touch geometry use the current authored `Joystick`/`stick` transforms and retained controller services. The authored clip remains the layout authority across viewport changes.
- Root must keep the `source_hud_attack_actor_v46` and `source_hud_attack_command_v46` declarations and their include/wiring in the shared renderer integration. Those shared files were not edited here.

## Reverse-engineering and validation boundary

- The HUD status mapping follows the IDA-derived `NativeGetPlayerHUDInfos` contract at `0x44e5cc` and its captured ordered service kernel. The original `NativeUsePotion` assembly at `0x43d2c8` was checked in the existing IDA handoff material. No Ghidra output was used.
- Source inspection only. No build, tests, emulator, ADB, screenshots, or gameplay verification were run. Portrait display/click, HUD values, skills, faery, potions, and input placement still need confirmation in the integrated APK.

## Remaining limitations

- This lane does not provide the shared renderer declarations/CMake integration or a runtime milestone. Original UI/Application owners still supply menu transitions, audio, and broader source input lifecycle. HUD presentation is not evidence that live HP/MP or action paths have passed integrated gameplay checks.
## Broad PRE-WORLD InfoHUD/HUDControls operation audit (2026-10-07)

This is a source/IDA routing audit, not gameplay verification. No C++ was changed for this audit. IDA (not Ghidra) evidence is from `MenuManager::Init` 0x42f304 (`pseudocode/0042/0042f304.c`), `PostLoad` 0x42efb8, `Update` 0x42ea04, `Application::IsCurrentlyInGameView` 0x31f684, `InfoHUDManager::initCachedChars` 0x41d880, and `HUDControls::initCachedChars` 0x419b4c.

### Cold Init/PostLoad provider closure

`MenuManager::Init` stage 4 invokes `InfoHUDManager::initCachedChars` and `HUDControls::initCachedChars` directly while installing the actual primary3 HUD RenderFX root. This precedes a World. Both initializers read `Application::GetSavedOption("HUDStyle")`; InfoHUD then resolves its actual authored HUD root/cache paths, while HUDControls resolves its authored controls and captures transforms. HUDControls' later SetHUDPos branch is conditional on two fields initialized to zero by its original constructor, so it is not a cold-Init dependency. `GetSavedOption` is an Application/SavegameManager service; a missing option returns 0.

The current root-owned `native_menu_postmovie_v62.inc::NativeMenuPostMovieV62::info` now handles `saved_option` against the SAME process `ApplicationServicesOwnerV5::source_settings4c_v67()->saved_option(q.text)` before any World fallback. It rejects a missing settings owner or key; it preserves the original missing-option value of 0. This is the needed process-owned fix for both InfoHUD initialization and the direct HUDControls `HUDStyle` query; HUDControls' DPad input callback uses the same process query.

InfoHUD's operations 4/5/6 (`root_lookup`, `cache_initialize`, `cache_get`) are retained movie/core work, not World services. `original_ui_postmovie_owner_v62.inc::source_hud_dispatch_v62` first dispatches them through `HudManagerCore` while `source_info_run_v62` holds the same HUD graph/movie scope. That core finds the requested child in the retained root and stores/reads weak character caches; a missing authored child is a successful null lookup, while malformed indices/expired movie ownership fail. Keep this route intact and do not send operations 4/5/6 through `world_operations`.

Thus the single cold provider closure is: process App current-level/dt and saved-option providers; actual retained primary3 movie/root/cache/UI-core operations; and the already initialized process App PlayerManager for calls that genuinely need it. `MenuManager::PostLoad` only searches loaded movie slots for `menu_` characters, registers new MenuBase receivers, and attaches `flush_text`; IDA shows no InfoHUD/HUDControls calls there.

### Pre-World frame guards and actual World path

IDA `MenuManager::Update` always calls `PlayerManager::GetLocalPlayer(0,true)` and dereferences the returned PlayerInfo record to read Character660. It then branches on Character660: if null, skip `GetInteractionType`/`FillActionIcon`; if non-null, use that actual Character. Preserve the distinction: a missing PlayerInfo is an unmet source requirement, while its null Character is valid pre-World state. Current `MenuManagerPrefixV62::update` follows that distinction: its PM provider requires the actual record, returns `character660` (possibly zero), and the action-icon work is guarded by `if(player)`.

The source next reads App dt, updates only existing movie slots, and calls `Application::IsCurrentlyInGameView`. IDA 0x31f684 returns false when `GetCurrentLevel()` is null; otherwise it returns current Level byte+408. Only a positive result calls InfoHUD::Update and HUDControls::Update. `MenuManagerUpdateV58::update` keeps those calls behind the same boolean. For the primary3 per-movie path, it rereads the actual Level and honors byte198; no Level is allowed on this path. Do not require World-only HUD providers in the pre-World/null-Level branch.

Once the real in-game gate is positive, InfoHUD's one-time/slow/fast work uses the actual local Player/Character, class and properties, saved skills and spells, cooldown timers, Gear potion inventory, targets and monster/character checks, names/HP, ally roster, camera projection and UI transforms. HUDControls input/update uses the real Character/controller and (for touch) PFWorld/screen conversion. These remain positive World paths: each must resolve from the same active canonical Level/World and same actor/controller lifetimes; an absent required provider should fail at that reached operation. Do not zero-default missing values, synthesize a Level/World, or turn provider failure into success.

### Root wiring decision

Keep the process-owned `saved_option` branch in `NativeMenuPostMovieV62::info`, before the World fallback, with SAME process Application/settings ownership checks and `OwnedHudSettingsV1::saved_option` miss semantics. Keep `root_lookup/cache_initialize/cache_get` routed through `source_hud_dispatch_v62` to the retained `HudManagerCore`. Keep other process-only sources (App dt/current Level and App PM) available before World, preserving optional Level/null Character behavior above. Leave the remaining character/actor/combat/input/provider operations on the canonical World closure and only require that closure after the original in-game gate reaches them. Current integrated-runtime status is unknown: this audit ran no build, tests, emulator, ADB, or gameplay checks.
