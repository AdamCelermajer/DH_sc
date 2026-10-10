Interaction source bridge
=========================

Compile original_interactions.cpp with the existing level-world
openable_container_owner_v1.cpp and openable_container_interaction_v2.cpp.
interactions_tests.cpp is a standalone test executable using these same sources.

CharacterUseBorrow references actual Character OOI5284, CharAI current target1032
and use byte1042. Ctrl_UseOOI checks the live remote virtual first; a non-null
explicit target follows ForceUseOOI, while null uses OOI. Idle/moving and active
skill gates precede AI_SetTarget(target,false); only completion stores use=1.
This admits source intent, and does not immediately perform Interact.

ControllerUseBorrow reads the existing force/lock/global block fields. An admitted
online command requires a complete original network prefix; a host must not
install a no-op backend. Its controllable callback invokes the same Character
receiver. The router accepts canonical-manager borrows and dispatches by their
live source type. It never owns another world, inventory, campaign or object
state. Register actual item-owner Interact, openable, NPC/native UI, and authored
script receivers there; unknown types return unsupported with a diagnostic.

RangeBorrow consumes the source owner's target anchor, target's interaction spot,
node2e8, melee radius, target virtual148 and AI padding24. The boundary is 3D
distance minus both radii. A node substitutes zero radii and padding80. Interaction
type8 requires separation<=0. ARM assembly calls __aeabi_fcmple, so NaN is rejected;
the decompiler's apparent greater-than rejection is not copied.

Openable delegates all lock, item consumption, event, animation, loot and script
effects to the existing OpenableContainerOwnerV1. Its unusual key_consume=false
branch deliberately remains locked, as the existing verified source owner does.
The tests read all68 original cache declaration rows, and exercise key quantity,
consumption, original state gates, forced command admission, range boundaries,
node bypass and live canonical type dispatch.

Door IsInteractive is false at original3e74a8. Authored scripts must use the
existing CanonicalDoorV27 source_opened_v91/source_closed_v91 transitions, not
invent a player Use toggle. Lever/portal/NPC dialogue effects need their complete
existing Lua/native UI owner handlers; this module does not substitute effects
when those services are unavailable. Main-loop bindings and CMake registration
belong to the integrating root agent.

LiveReceiverBindings in live_receiver_bindings.hpp now provides reusable wiring
over a shared existing CanonicalObjectManagerV1. Root supplies LiveReceiverServices
with its existing world lease, retained openable identity lookup plus source
interaction services, and the existing shared WorldItemLiveOwnerV5. The helper
pins published target and actor receiver borrows before effects and verifies the
openable receiver's original identity. Types7/3 call the actual source owners;
type2 doors are blocked; type0 needs a complete Character.Interact backend.
Additional authored receiver callbacks are keyed by source type only.

For input, call command(existing ControllerUseOoiV47,requested,error), preserving
the complete existing controller network implementation. For range use range()
with fresh source anchor/radius/type services. Deliver(target_manager_key,
actor_manager_key,error) belongs to the original interaction FSM/animation event,
not the input click. Character native UI remains an explicit required receiver
dependency: TalkToNPC RaiseAsync must precede SM_SetInteractState(3,true,actor,
false), followed by source RaiseEvent5 and the merchant/cleaner/dialogue branches.

Live completion requires an admitted real input to change the same AI target/use
cell, original source state/animation to deliver Interact, and effects to appear
in the existing inventory/loot pool, quest event queue and canonical container
state. Callback-only tests do not establish this integrated acceptance.

npc_interact.hpp/.cpp now implements the full Character.Interact3a4d78 source
coordinator (verified against assembly), with native Windows ordering tests.
LiveReceiverServices.npc provides the canonical NPC borrow plus reached services;
borrow_npc_interact() directly borrows retained Character metadata, talk flag and
display-name storage. It owns no FSM, queue or inventory. Root must bind actual
SM_IsInteracting/SM_SetInteractState, Character.RaiseEvent5, current Level
EventManager RaiseAsync using real QE_TalkToNPC event constructor/type, original
merchant declaration/NPC inventory AddLoot and MenuManager/PlayerInfo/RenderFX
AS bridge. Standard nonmerchant/noncleaner dialogue is reached via Character
RaiseEvent5/scripts; this source body adds no invented dialogue menu.

npc_source_services.hpp adds bind_npc_event5() over the existing attached
CharacterIdleEvents owner and bind_npc_quest_raise() over the captured SAME
Level EventManager. Event5 enters the existing AI/FSM dispatcher and keeps its
selected CharacterScriptSession. QE_TalkToNPC fields are produced in sole native
backing using the original caller stores, then lent through the existing scoped
GameEventQuestBorrowV75 projection. Original RaiseAsync is synchronous Raise;
no alternate queue/clone is created. npc_source_services_tests.cpp passed natively
against the existing event_manager_owner_v12, game_event_runtime_v75 and
game_event_manager_v50 code and checks same mutable payload and zero queued events.

npc_menu_services.hpp/.cpp provides genuine RenderFX/AS service composition over
the existing SwfMovie facade and SwfAsGraph. Root supplies its actual MenuManager
lease, fresh HUDRoot and f4->138 movie getters, captured RenderFX identity resolver
and PlayerManager.GetPlayerByCharacter(false) PlayerInfo678 field borrow. The
helper invokes the authored methods with source typed string/number arguments
inside the owning movie's menu_action_script Scope. It loads no movie and creates
no player/menu. npc_menu_services_tests.cpp links the existing native engine UI
and verifies live PlayerInfo reads, captured identity mismatch rejection and
explicit failure on an unconstructed movie. Full merchant/cleaner UI acceptance
still requires the root's genuine current movie/global/native providers.

`container_receiver_bindings.hpp` adapts the source factory GO_ID1
`CanonicalDestructibleContainerV16` into the existing live ObjectManager type
router. The resolver must borrow the already-published receiver and the helper
checks its identity, source type, and `DestructibleContainer` declaration before
calling the original staged `interact` method. Prepare the V21 animation
bindings before constructing either receiver, then attach the same receiver
before InitPost installs callbacks. These adapters use the current retained
visual animator; its source event stream supplies the `opened` marker that
reaches the existing breakable quest and loot path. They do not schedule events
or create a second timeline.

The original Act 1 cache declares `Swamp_NoMoth_DestructibleBarrel` and
`Swamp_Normal_DestructibleBarrel`. Their actual DestructibleContainers rows both
select visual dictionary id70 (`go_swamp_urn_breakable`), loot table9, and sound
325; the NoMoth row has no object script, while the Normal row names
`moth_spawn_container`. The exact recovered breakable BDAE is consumed from the isolated source fixture; its authored activate/idle/idleactive clips and opened marker at 266 ms are parsed and tested.
The exact `moth_spawn_container.luac` is staged at
`.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/objects/moth_spawn_container.luac`
(1538 bytes, SHA256
`68c87e4eb16ec991e0929be9e1bb71e93e9606c966b46848ee88edba32508d93`, manifest
row 79). Its `OnOpen` sets probability 25, samples `GetRand(0,100)`, and only
when the result is strictly less than 25 summons `Swamp_Moth_Minions` with the
authored `Summon(..., true, 0, 0, 0, true)` arguments. If the actor exists, it
then deals `Level01Damage` through `GameObjectDamager` with Fire element. The
NoMoth row has an empty object-script field. These authored rules are now known;
runtime summon integration is still missing because the actual
`CharacterScriptSession` has no `_Summon` binding. Do not replace it with an
unconditional spawn or a separate RNG.

`container_receiver_bindings_tests.cpp` is linked and run as a Windows native
executable using the pinned LLVM-MinGW compiler and existing Windows foundation
archives. Run `run_container_receiver_bindings_tests.ps1` to reproduce. It uses
the actual `CanonicalObjectManagerV1`, source GO_ID1 `DestructibleContainer`,
canonical receiver, and both V21 animation connection functions. The test
proves publication/resolver identity and preconstruction V21 service setup,
then deliberately stops at the absent real retained visual/root required by
source `SetState`; it does not claim breakable authored clip playback.
`container_live_loot_binding.hpp` adapts Openable and Destructible
DropLootTable callbacks through the same initialized `WorldLootGameplayV23`,
which owns `CharacterLootLiveV22` and its matching `WorldItemLiveOwnerV5`
Item145 pool. Root supplies the typed adapter that calls that owner's
`drop_table_v104` (the canonical `container_live_drop_table_v104` adapter is
provided), a strong shared owner and world lease, and a resolver for
the already-published container identity. The helper checks readiness and pool
identity before binding and again at delivery; absent owners or adapters fail
closed. Root calls it before receiver graph construction. The native test reads the
actual `go_chest_swamp.bdae` clip/event tables: `activate` spans 166–1266 ms,
and the authored `opened` marker reports 233 ms and dispatches once at 234 ms.
The existing `canonical_openable_graph_v21` native receipt drives the actual
retained chest visual/controller through the source event/completion path; its
DropLoot callback remains an explicit owner-boundary fixture. The shared
Quest/NPC integration fixture in
`../quests/npc_talk_objective_integration_test.cpp` calls
`bind_npc_quest_raise` on the same Level EventManager already projected by
`bind_source_quest_event_projection_v108`. Its real ObjectiveRuntime listener
updates the Save-owned `Abbey_Rescue` TalkToNPC objective synchronously and
preserves the source network/deferred-detach ordering. This fixture tests that
specific native service route, not full profile/session acceptance. Remaining
host requirements and status are recorded in
`reports/act1-interactions-native-integration.json`.

The native loot-table loader confirms that table9 `Barrel_Level_01` repeats
table124 `Gold_01` three times. Table124 has `GoldLevel_01` and `PotionLoot`
child lists, which resolve to `GoldStack01` (item418, type13) and `Potion0`
(item925, type14), respectively; each child list's selected item has quantity
1 and probability1. No relative child-entry weights are claimed. This
establishes the authored child contents, not guaranteed money or a fixed
amount; the same live loot RNG and GoldStack valuation/conversion still
determine the awarded result.

`container_source_pipeline_v21.hpp` adds an atomic preconstruction composition
for Openable and Destructible services. The caller supplies the retained
current-visual animator provider, same ready `WorldLootGameplayV23`, typed
drop adapter, world lease and published source resolver. The helper prepares
the V21 source event/completion callbacks and original DropLootTable callback
on a temporary service bundle, then publishes them together; it rejects
already-bound endpoints to prevent duplicate effects. The returned animation
binding must attach to that same canonical receiver before InitPost. If any
required provider is missing, the input services stay unchanged.

Same-session interaction route (current generic contract)
--------------------------------------------------------

`session_openable_interaction_v1.{hpp,cpp}` and
`session_destructible_interaction_v1.{hpp,cpp}` resolve an `ActorId` back to
the host's existing `ActorDefinition`, `ActorState`, interaction fields and
same-session lease. They own no second manager or animation clock. The host
feeds the actual retained-visual event/completion callbacks and source clip
playback; these adapters do not synthesize animation times or markers.

`source_container_loot_v1.{hpp,cpp}` composes the original
`LootTableSelectionV8` and `LootItemSelectionV8` over the loaded source tables,
the caller's existing `LootRandom8V2`, and a synchronous host drop sink. It
passes the original selected item/entry row identities and source/opener IDs;
it does not construct an inventory, item pool, manager, or RNG. A per-ActorId
lifecycle receipt prevents duplicate delivery from replaying loot effects.

Run `run_session_container_interactions_tests.ps1` for the actual Windows native link and execution. It proves the actual chest BRES `opened` marker (233 ms) reaches the same-session `OpenableContainerOwnerV1` once and the completion callback advances source state. The object-only chest fixture uses that same marker and source table with a neutral `WorldObject` and no `ActorState`; it publishes exact selected records into the existing `RuntimeWorldItemAdapterV1` at the object transform, checks store render packets, persists state2→state3→state4, and suppresses duplicate delivery. `run_session_world_item_consumer_v1_tests.ps1` now creates its table9 Potion0/GoldStack01 records from a neutral object and resolves their original itemdrop material passes/textures and draw packets from that same store. The breakable row30 fixture parses the recovered BDAE and dispatches its authored marker. These are linked feature fixtures, not full production Level enrollment or Save/Load session acceptance; production source-value providers and retained visual/session ownership remain host responsibilities. Exact limits are in `reports/act1-interactions-native-integration.json`.

The earlier `container_receiver_bindings.hpp`,
`container_live_loot_binding.hpp` and `container_source_pipeline_v21.hpp`
are retained as historical reference for the rejected canonical receiver /
`WorldLootGameplayV23` route. They are not requirements of this generic
same-session API.

World-item source rendering
----------------------------

`source_world_item_drop_render_v1.{hpp,cpp}` projects the existing
`RuntimeWorldItemAdapterV1` entries into source-backed draw packets. It loads
the staged original `data/3D/GameObjects/itemdrops.bdae`, resolves each stored
ItemTable AudioVisualID to its exact ItemAudioVisual Visual, and currently
decodes only the verified static `dummy_itemdrop_Potion` and the
`root_itemdrop_Gold_01` CharacterVisual controller at authored rest pose.
Packets retain the exact store ID/record/position and source geometry; other
visuals remain explicit unresolved rows. The root material callback must bind
the original source material/pass/texture to its current renderer and submit
the frame to the existing RenderQueue, retaining it until that queue drains.
This helper owns no camera/frame, item world, animation clock, bob or spin.
The focused consumer runner exercises the same store and source geometries;
its material handle is synthetic and therefore does not claim GPU rendering.
Exact source/test limits are in `source-world-item-drop-render-v1.json`.


Neutral container save state and source identity
------------------------------------------------

`world_object_container_state_v1.{hpp,cpp}` stores the original seven-byte non-character container payload in the existing `WorldObject::state_components` map under the versioned key `dh2.source-container.objs.v1`. Bytes 0 and 1 are original GameObject base `visible80` and `enabled8a`; bytes 2–5 are little-endian `archetype270` u32; byte 6 is derived container `state394`. The native fixture checks exact encoding, malformed-length rejection, field preservation on state changes, and restore visual selection (state2 idle, state4 idleactive, other states no animation).

`run_session_container_game_save_v1_tests.ps1` links the actual `game_save.cpp`/`PlayableActorWorld` sources and performs a v2 disk save/load/restore for the authored Swamp chest and destructible barrel with their exact seven-byte components. It roundtrips state2→3→4, restores state4 as `idleactive`, preserves source transforms, and consumes no RNG. Their stable ObjectIds pair to the exact current ActorDefinitions; those definitions do not contain the native `room64` written by Level OBJS, so the source tuple resolver fails closed instead of inventing it. Existing interaction tests reject ambiguous duplicate `(gametype,map_name,room)` definitions. The original Level OBJS writer/reader and production room resolver are still not connected to the modern GameSave caller. The same-session interaction owner also suppresses state4 input and stale `opened` markers, matching source `IsInteractive(state394==2)` and the non-replaying source state4 restore route.

Source evidence: `level-world/object_save_restore_v3.cpp` writes the two base bytes at payload offsets 0–1 and `archetype270` at 2–5. `level-loader/noncharacter_save_connection_v89.hpp` appends one state byte at offset 6 for Openable/Destructible containers. Restore reads current Level flags and `death_reset390` before the base or payload; if reset is false and Level `f3` or `f4` is set, it returns without reading the record. Otherwise it restores the GameObject base, checks the saved archetype, synchronizes visibility, reads state394, and calls source SetState. State2 restores authored `idle`; state4 restores `idleactive` (and detaches physical object first when derived KeepPhysics is false). Restore uses source animation play with false/zero flags and does not emit `opened`, replay loot, or call `OnOpen`. `level-world/tests/campaign_opened_container_save_v89.cpp` confirms a 7-byte payload ending in state4 and the state/physics/idleactive restore route.

`level-world/level_savegame_objects_v2.cpp` wraps each virtual Save payload with `gametype`, `map_name`, `room`, and a u64 payload length; load looks objects up by `ObjectManager::GetObjectByName(name,room,false,NULL)` (except the special player record). It does not persist modern stable `ObjectId`. `source_object_id_for_save_key_v1` resolves the current authored definitions by exact `(gametype,map_name,room)` and requires one unique match; the host supplies the original room resolver. Missing or ambiguous bindings fail closed. The source state byte must be seeded when a neutral object is enrolled and then persisted with the existing WorldObject; animation callbacks and lifecycle receipts remain transient.

Doors share the 6-byte GameObject prefix and append 4-byte `state3a8` u32 (10-byte derived payload). Restore invokes original `Opened(false)` for state1 or state3, otherwise `Closed(false)`. The neutral container component does not encode Door state. The modern bridge does not establish global uniqueness of every `(gametype,map_name,room)` tuple or wire neutral objects into production Level enrollment.

Same-session retained visuals, source drops, and state4 restore
-----------------------------------------------------------

`session_container_retained_visual_v1.{hpp,cpp}` bridges the existing chest and destructible interaction owners to `CombatSession`'s retained neutral-object visuals. Each resolver reacquires the current mutable `WorldObject` from the same `PlayableActorWorld`; visual callbacks check the current `actor_binding_lease()` owner before dispatch. It does not own another clock, object registry, store, or RNG. Destructible opener resolution accepts the current same-session character/object identity without requiring the opener to carry destructible state.

`run_session_container_live_visual_integration_tests.ps1` links a strict Windows C++17 test over the exact 001_swamp chest (ObjectId `4308955945491066525`) and urn/barrel (ObjectId `17396591008448001070`). The chest's authored BRES `opened` event drives its original Openable row/table selector; the recovered urn BDAE's `opened` event drives Destructible row30/table9 and the declared `moth_spawn_container` OnOpen dispatch request. Both use one caller-owned Loot RNG and publish their exact source records/positions into the same existing `RuntimeWorldItemAdapterV1`. The test resolves source itemdrop geometry, material passes, external effect/texture leases from those same store entries, then roundtrips each state4 component through GameSave v2 and silently restores `idleactive` without another drop/event.

The material test uses decoded source pixels and a CPU lease plus a caller submission callback; it does not claim a GPU upload or actual RenderQueue execution. The urn callback records the exact `OnOpen` request but does not execute the missing `_Summon` runtime. Production Level enrollment and original OBJS tuple resolution remain separate host integration gaps.

Root-callable neutral visual handoff
------------------------------------

`SessionContainerRetainedVisualV1::bind_authored_object(const ActorDefinition&, const AssetCatalog&, std::string&)` is the one-call visual enrollment seam. It validates the current same-session `WorldObject` ID/name/model and finite position, then delegates to `CombatSession::bind_object_visual`; it does not construct another object/world. After `detach_for_restore` / `restore_game_save` / `rebind_after_restore`, call `refresh_after_restore(error)` followed by `restore_authored_object(definition, assets, error)`. The restore call revalidates the same object, reloads its source visual, reads its seven-byte component, and silently selects `idle` for state2 or `idleactive` for state4. It intentionally binds no source callbacks during restore.

`session_container_modern_openable_v1.{hpp,cpp}` adds a production-consumable one-object Openable composition over those real owners. Its row-backed mode requires the original Openable table and a typed prior-admission receipt bound to the exact Session lease, ObjectId, definition name, and `data_desc`; it derives row index 58 and original constructor defaults (empty key name, key id -1, quantity 1, consume true, death-reset false, opener 0) for the authored Swamp chest, then restores `state394` from the saved component. It composes the root Session visual callbacks and same-store/scoped-RNG loot callbacks. `initialize_admitted` binds the admitted object silently and does not rerun `CheckSpawnProbability` or `GameObject::InitPost`; the loader/enrollment owner must issue the receipt only after its source admission succeeds. Other opener IDs go through the caller's same-session typed resolver. After a GameSave restore the host recreates the binding against the new lease and calls `restore_silently`: state2 binds fresh callbacks without replaying InitPost, while state4 accepts neither another input nor an opened marker. The original BRES opened marker, loot route and save-state path are covered by the linked fixture. Main still has no production object-enrollment/activation callsite; source audio, physical, key/inventory, quest/event, script, and full admission providers remain host gaps. `init_post` remains the full source-prefix path only for a caller that truly owns those original gates.

`session_source_object_admission_v1.{hpp,cpp}` adds the bounded candidate path for authored Openable and Destructible definitions. It evaluates the existing `game_object_meet_condition_v1` and `game_object_check_spawn_probability_v1` before publishing the staged WorldObject, then mints the typed `SessionContainerPriorAdmissionV1` only after success. Fresh GameObject source defaults are `data_id=−1`, `network_id=−1`, `cached_roll=−1`, and `probability=100`; the selected Swamp chest and urn have no authored probability override. The exact CheckSpawnProbability assembly at 0x38bd64 calls GetHandle→Character conversion; `ObjectHandle::operator Character*` at 0x33ff54 calls `GetObject(false)` and then virtual +0x24 `IsCharacter`. For a correctly registered container GameObject's self-handle, the GameObject/ObjectBase implementation at 0x33dcd0 returns false, so the conversion is null and Character::IsPlayer (+0x28) is not queried. A null lookup likewise yields null. Thus a fresh container candidate reaches online byte5 and `Random(99)`; even probability 100 draws. The API takes a synchronous typed two-channel RNG loan plus its strong owner lease, so the root may bind the canonical source-global owner without creating a feature stream. Source-global proof confirms offline/default spawn and modern combat/loot share the same source RNG algorithm/state ordering; for the clean offline Windows Session, the existing `PlayableActorWorld::with_loot_random` is the modern canonical channel0 owner used by combat/loot/audio. The test uses that exact current-Session loan. This does not claim memory aliasing to native ApplicationSpawnRandomOwnerV4, online synced channel1 behavior, original startup/level reseeding, or full campaign RNG sequence. Before any source draw, the adapter validates the full WorldObject and rejects invalid, duplicate, or current Character-colliding ObjectIds. It snapshots authored identity fields and retains the initial Session lifetime, World pointer and binding lease; callbacks that replace the binding after the RNG loan are detected, consume only the already-reached source prefix, and cannot publish a candidate or mint a receipt. Replacing/destroying the Session or World while the scoped RNG loan is active is forbidden. On the rejection branch, the existing source kernel's `SetVisible(false)`, Delete byte82=2, continuation byte82=0 and MarkForDeletion order is projected onto the unpublished staging candidate; rejection discards it before `WorldObject` publication or retained visual callback binding. The focused linked fixture covers the actual authored chest p=100 path, controlled p=0 rejection, malformed ID/transform and actor collision preflight, and a typed post-loan lease renewal. See [source-object-admission-v1.json](../../reports/source-object-admission-v1.json) for exact source hashes, evidence limits, and test result.

`session_container_admitted_openable_v1.{hpp,cpp}` is the one-call composition for a staged Openable candidate. `admit_and_bind_session_openable_v1` preflights the current retained-visual lease, original Openable table presence, typed same-session opener resolver, and exact existing drop/store/RNG service before admission. It then runs source admission once; a source probability rejection returns its hide/Delete/Mark receipt with no WorldObject, retained visual, or Openable callbacks. On admission success, it forwards that exact receipt to the existing row-backed `SessionContainerModernOpenableV1::create` and `initialize_admitted` path. If binding/initialization fails, it removes only the same candidate and any retained visual while the original Session/World lease is still current. The returned interaction owns the existing open-event-to-drop/WorldItemAdapter path; caller-owned level definitions must remain immutable and alive as in the level definition lease. The linked fixture now uses this composition for the actual 001_swamp chest, drives the authored BRES opened marker into the same store, and verifies silent GameSave state4 restore/no replay. A missing opener resolver fails before RNG; an authored candidate rejected at probability zero has no object or visual. This remains a feature API: main does not yet provide scene enrollment or live online/current-level providers.

`run_session_container_live_visual_integration_tests.ps1` now exercises this binder with the exact authored chest and its actual BRES `opened` marker, then verifies selected rows enter the same existing store at the WorldObject transform. The fixture still supplies source policy callbacks and the fallback actor resolver; it does not establish those services as production implementations. The urn now uses `SessionAdmittedDestructibleV1` after explicit source admission. Its typed callback endpoints remain fixture providers until the integration lead binds their real current-Level, audio, physical, script, Character/stat/trophy and object-context owners; `_Summon` remains unavailable in the current script session.

`session_container_modern_drop_v1.{hpp,cpp}` now composes the genuine modern drop leaves without waiting for Level/OBJS object graph callbacks. `SessionContainerModernDropV1::create` validates a live `CombatSession`, same retained LootTables snapshot in the existing `RuntimeWorldItemAdapterV1`, original loot powers/entry service, and the caller's optional exact opener gold-value provider. `services()` returns a `SourceContainerLootServicesV1` that loans the same `PlayableActorWorld::with_loot_random` stream for the full selection-and-publication callback, validates the current source `WorldObject`/definition/transform and actual opener ID, values type13 GoldStack only through the typed opener provider, and publishes to that same store through `publish_source_object_drop`. The binding is lifecycle-bound and must be recreated after GameSave restore. The focused actual chest/urn runner uses a test-only zero gold bonus; production type13 drops fail closed until the source opener property provider is wired.

This composition covers table selection, same-session RNG, and same-store publication only. It deliberately leaves condition/admission, current-level quest events, audio, physical detach, script execution, and source stats/trophies as typed required leaves in `SessionOpenableInteractionServicesV1` / `SessionDestructibleInteractionServicesV1`; the current integration fixture uses callbacks for those. Modern GameSave v2 container state and silent restore are covered separately; original Level OBJS room import remains optional and unintegrated.

Authored-scene Openable collection
----------------------------------

`session_authored_openable_scene_v1.{hpp,cpp}` exposes one scene-level handoff:
`SessionAuthoredOpenableSceneV1::bind(session, source_definitions, prior_admission_receipts, assets, retained_visual, same_session_drop, exact_openable_table, providers, output, error)`. It copies and retains the source `ActorDefinition` snapshot and owner leases, resolves each `OpenableContainer` `data_desc` against the exact authored row-name table, requires a receipt bound to the current Session lease/ObjectId/definition/data_desc, verifies the actual current same-World `WorldObject` and row-to-model mapping, and builds every source policy before binding any visuals or callbacks. `interact`, `animation_event`, and `animation_finished` route only to the retained owner for that exact ObjectId; `release` / `release_all` remove its visual callbacks while the original Session binding remains current. The collection owns no registry, world, object, store, or RNG.

The linked fixture uses the actual `001_swamp.mlx` normal chest ObjectId `4308955945491066525` and table row58 / visual47 / loot227 / sound33. It verifies admission receipt required, duplicate receipts and duplicate source row-name mappings rejected, missing same-Level Quest RaiseAsync provider rejected without publishing visual callbacks, then drives explicit input through the collection to the source chest BRES opened event and same-session drop/store path. The same test runner still validates silent state4 GameSavev2 restore separately. Exact authored 001_swamp inventory is four `Swamp_Normal_Chest` rows and one `SwampCave_Normal_Chest`; there is no Openable pot in that scene.

This is a feature-owned callable binder, not a production main callsite. The caller must perform actual scene candidate enrollment and provide source-backed online/current-Level/key/quest/audio/physical/script services (including their retained lifetimes). The test's typed providers are fixtures. Modern GameSavev2 support is separate from native Level OBJS/room import, which remains unintegrated. See `port/windows-foundation/reports/act1-interactions-native-integration.json` for placements and boundaries.

## Authored Destructible one-object binding

`session_admitted_destructible_v1.{hpp,cpp}` adds the bounded per-object counterpart for an already-admitted source `DestructibleContainer`. `SessionAdmittedDestructibleV1::bind` requires the explicit successful `SessionSourceObjectAdmissionReceiptV1`; it checks exact Session lease, ObjectId, immutable definition name/data_desc, current WorldObject, seven-byte GameSave component, unique source row name, row-to-visual mapping, and all required source policy endpoints before retaining a visual or installing callbacks. It composes over the same caller-owned definition/provider lifetimes, `SessionContainerRetainedVisualV1`, `SessionDestructibleInteractionV1`, current Session RNG and existing world-item store. It creates no Level, quest, audio, physical, script, actor, store or RNG owner. Missing production providers fail before visual enrollment.

The strict current-archive `run_session_container_live_visual_integration_tests.ps1` now admits and binds the exact 001_swamp urn ObjectId `17396591008448001070`, checks missing and mismatched receipt plus missing quest/script policy rejection, routes the actual BDAE `opened` marker through both source DestroyGameObject sites and row30/table9 publication into the current item store, duplicate-marker suppression, silent state4 GameSavev2 restore and stale-lease rejection. Seed 6 is fixed for this fixture so table9's randomized source selection publishes an item. The callbacks remain explicit fixture providers. The original asset sequence and its limits are documented in `session-authored-destructible-bind-v1.md`; no original urn gameplay capture exists, and the callback request does not execute the source script's missing `_Summon` runtime. Production scene enrollment and real caller providers remain integration-lead work.

