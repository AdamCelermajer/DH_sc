# Renderer source Kill composition V43

`renderer_character_kill_bindings_v43.inc` is a complete renderer-facing composition over the existing CharacterKillProductionV23 kernel, not an alternative death implementation. It is currently **unpublished**. Root must retain it on SAME PlayerSkillsRuntime and call bind with actual providers before routing Hit8; no current app deployment or lethal acceptance is supplied.

Include after renderer_character_kill_live_v21.inc, renderer_npc_death_v2.inc and the player-manager locality helpers; the test injects it after renderer_player_target_frame_v2.inc. Header dependencies are character_kill_production_v23.hpp, retained_character_kill_borrow_v42.hpp, world_loot_gameplay_v23.hpp and character_progression_world_v23.hpp. Production TUs already present or required: character_kill_production_v23.cpp, character_kill_live_v21.cpp, character_kill_contributor_event_v23.cpp, character_ais_kill_vm_v23.cpp, character_kill_level_events_v23.cpp, retained_character_kill_borrow_v42.cpp, character_kill_fields_v21.cpp, whole Kill/combat owner, real loot/progression owners. No global CMake changes were made here.

## Typed root construction

RendererKillProvidersV43 takes provider-storage lease (not an App/Runtime ownership cycle), actual KillLevelProviderV23 callback/weak lease, WorldLootGameplayV23* (ready ONE145 pool), CharacterProgressionWorldV23*, current source callback scope supplier, player borrow function and whole player OnDied delivery. Player borrow must return SAME metadata.kill_fields, PropertyView/life/controllers, PM/canonical Handle and prince_combat.aggro.outgoing_table. Root has added those fresh C1 fields; this packet does not create another player base or replay them.

`bind(error)` preflights actual current GSLevel completed C1/gate150 projection, then constructs source contributors for every registered Character. NPC borrowed fields come from retained_character_kill_borrow_v42 using its exact existing object-group script lease and outgoing table. Player uses actual events and ScriptOwnerV2. Contributor dispatch refreshes selected active, forced, locked and actual global policy before source event4; source forced/blocked branch uses SAME FSM event delivery. Invalid callable adoption is an explicit false result with cleanup, not a thrown frame success.

`hit(actor,q,out)` returns0 another service,1 completed original Hit8,-1 required failure. Parent Hit facade maps1 to provider0. Requests pass controller/character/attacker/force unchanged. No HP/dead reset, direct dead stamp, independent target death broadcast, automatic reward or suppression is added.

## Providers genuinely composed here

* DropLoot: WorldLootGameplayV23::route, original early Kill position.
* XP: CharacterProgressionWorldV23::route, after contributor OnKill/counters.
* NPC OnDied: renderer_kill_npc_raise_v21, complete CharAI selected AIS and source FSM ordering.
* Contributor OnKill: CharacterKillContributorEventV23 over actual ScriptOwner/AI state, ordinary/scoped source VM handling.
* Locality and online: actual player network identity and EquipmentPlatform world query.
* Remote: source Player heading_remote and retained NPC ObjectBase118 byte (no inferred network ID).
* Quest constants: actual design lookup with requested group/key. Quest typed immediate events remain CharacterKillLevelEventsV23 over same Level embedded EventManager.
* Trophies: actual captured TrophyManager identity, catalog exact ID, source unlock0 success semantics. Positive missing queue/save/text services remain true errors.

## Required producers still not claimed available

The current production PM/XP owner remains unpublished per status_progression. Count0 still executes genuine debug and gives no XP; no count override is provided. Positive XP recipients/level-up require the retained AddCharacter, source Save, tutorial/FX87/statistics/text/save branches. Loot requires actual live initialization, InitFinal, creation/temporary awards/pool/scatter/visual/PF and phase-dependent sound, not only isolated1627 proof. GSLevel must genuinely publish completed canonical Level C1 with its real events, no fake Level150=1. Player death callback is a typed whole source endpoint, not an empty success. Consequently root must not publish this Hit8 just because bind compiles.

## Lifetime and cleanup

Call `release()` before actor maps, NPC sessions, PlayerSkills VM, PM or World teardown. Production destruction restores contributor callable metadata while receivers exist. Current-level scoped borrows pin actual Level through nested synchronous events; quest stack payload never becomes a queued pointer. Source no-retry failure policy is retained by CharacterKillLiveWorldV21. The actor remains registered through true death animation/timer/despawn and existing FX ownership, rather than being removed at HP0.

## Evidence

Private copy of current model_renderer with new include strictly compiled ARM64+x86_64 (no shared file mutation). O1/O2 ASAN+UBSAN rebuilt health→CtrlKill→leech source composition passes88 checks each on actual character-cache property rules, reentrant dead gate, required loot failure, preserved lifecycle/leech prefix and ordered events. Loot/XP/quest observer callbacks in that test are explicit fixtures. Historical dependency hashes are in kill-order-v43-host.json; they do not attribute old binaries to current source. This is source-bound ordering/adapter compile evidence, not complete live kill rewards or original all-family death acceptance.
