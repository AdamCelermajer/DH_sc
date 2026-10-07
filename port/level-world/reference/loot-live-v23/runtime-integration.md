# Loot gameplay V23 integration candidate

This package is **source/compile accepted, positive runtime pending**. It does
not claim that a monster kill has dropped loot or that the player has picked it
up in the running app. The corrected positive Item145 executable is compiled
against the recorded APK dependencies; emulator5554 became offline before its
corrected execution. Preserve the earlier failure receipt rather than replacing
it with a PASS. The standalone StatusMsg queue test passed 31 checks; its movie
and PlayerInfo constructor endpoints are fixtures.

## One owned source graph

`WorldLootGameplayV23` composes the existing V22 DropLoot/AddLoot coordinator,
one V5 canonical Item145 factory/pool/visual/PF/body graph, actual Gear transfer,
source update prefix and genuine pickup interaction. It borrows the existing
World, PlayerManager, canonical ObjectManager/PropertyMap, immutable LootTables,
PowerResources/ItemPowers, Debug/file services, ItemText owner and Application RNG.
It does not insert awards directly into Gear or invent an NPC XP/loot row.

Call `initialize()` exactly once at the actual ItemManager initialization stage.
Call `initialize_final()` at the actual pending-object InitFinal stage; this is
separate from construction/PreCache and must not replay on retained reload.
`route(KillActor56&,KillRequest56&,KillResponse16&,handled,error)` handles only
the original early `kill_drop_loot` service. Do not hook Hit8 until whole Kill's
XP/quest/OnDied continuations are also ready.

`update(same_scene_ms,source_dt_ms,error)` visits the actual enabled Item receivers
and calls their original Item/GameObject update, including deferred collision
Interact. The caller supplies the existing scene clock; no new clock is owned.
This renderer transport does not claim to consume the source pending-start queue.
`draw_scenes()` returns object identity plus the SAME retained visual: BRES,
scene graph, material rows, authored animator, node visibility and current world
matrices. Root converts these directly to `LootVisualDrawSourceV27` and calls
`sync_loot_visual_draws_v27` AFTER source Item world update.

`renderer_character_loot_gameplay_v23.inc` provides the actual runtime binder.
Include its headers (`world_loot_gameplay_v23.hpp`, `renderer_loot_gpu_v27.hpp`,
`condition_data_init_v3.hpp`, `game_object_spawn_probability_v1.hpp`) at normal
namespace scope; include V4 graph adapter and this include after canonical world
and PlayerSkillsRuntime definitions. Do not also call the old adapter's `create`:
the extracted `make_services` supplies cache/device/condition/font facts without
allocating another scene/PF/body. Supply the SAME obstacle registry explicitly.

Before physical world/canonical map teardown, call the binder's explicit
`release(error)` (source ItemManager Flush then graph release), clear GPU draws,
and remove canonical receivers through the source manager lifecycle. Do not
silently retry a failed destructive prefix. On GL resource restoration detach
physics and rebind the new actual floor/obstacle borrow; retain Item inventory,
actor identities and original pool rather than reconstructing them.

## Required production services still supplied by root

1. Current Level borrow: identity + provider lease, actual source word118 and
   **uint32 word130**. C1 state130=0 is genuine. Never write38 or lootGate150=1
   to bypass required work. Player duplicates use actual PM character_count6c4,
   not player-map size.
2. Canonical Spawn/condition/network/assertion callbacks and real source cache,
   actual light-name owner, ItemPower sidecar lifetime observer, same RNG/text.
3. Mixed-world physical peer mappings: PhysicalObject pointer -> actual owner,
   raw visible80, Handle->AsCharacter, SAME Character OOI14a4. V23 recognizes its
   own retained POItems and resolves their actual shared handles itself; unknown
   actor/decor peers remain required. `borrow_contact` supports root's typed
   mixed physical contact dispatcher, without casting unknown contexts.
4. Frame camera/auxiliary/runtime facts, actual path/controller workspace and
   offline query. The original generic Item frame handles its own visual update,
   speed6, Stop, IsAtDestination, PF and body authority. Positive unsupported
   camera/auxiliary/tooltip branches fail by name. Tooltip OOI is borrowed only
   when the actual tooltip pointer is nonNULL.
5. Pickup source online/difficulty/tutorial/local-network/trophy/loot-FX leaves.
   Same Gear transfer and gathering-ID quest tail are composed, but positive
   registered quest delivery still needs actual GS/constant/immediate event base.
   GetInteractionType3ebeb4 ALWAYS returns -1 after its source IsInteractive and
   optional inventory GetItem(0); do not replace it with ItemTable.PickUpType.
6. `enqueue_status` MUST bind the ONE Application `MenuStatusMessagesV26` owner:
   `enqueue_status(0,text,metadata18,error)`. No V23 production queue is allocated.
   The retained V23 StatusMessageOwner is only an independent source algorithm
   regression. `_root.onStatusMessage(0)`, NativeGetNext (peek only) and NativeStop
   ('status', pop then invoke next) must reach the same actual HUD movie.
7. Sound: V23 composes real VoxPlay3D Debug/current-Level gates. Current phase0
   returns before audio playback, exactly as source. Positive phase38 still needs
   actual source sound row/loader/engine delivery. Exact original drop/pickup WAV
   filenames are absent from the supplied cache/APK census; no unrelated sample
   is substituted and no audio-play success is claimed. Source LoadSound can
   return NULL and native PlaySoundPackSound return0, but that missing-file path
   must be bound/proved rather than fabricated.

## Source and layout changes

The canonical manager gains one ctor-zero source update_count58 field/accessor;
this changes native class layout. Rebuild ALL manager consumers coherently.
GameObject.Update38cbe8 queries actual Debug TraceUpdateGameObjectOnce before a
fresh Application.manager38 borrow and increment. Profiling push/pop are original
literal bx-lr leaves. Shared V5/visual additions delegate actual InitFinal and
same PF update; no duplicate base or field authorities were added there.

Production new TUs: gameobject_update_prefix_v23.cpp, world_loot_pickup_v23.cpp,
world_loot_gameplay_v23.cpp. Recompile modified canonical_object_manager header,
world_item_visual_v2.cpp, world_item_live_owner_v5.cpp and existing consumers.
Dependencies remain the original V22/V5 factory, temporary-inventory transfer,
ItemText/Power, navigation/physics, light-name, font, Vox and source frame owners.
StatusMessageOwnerV23.cpp is TEST ONLY; production uses the shared V26 singleton.

BothABI strict compile: six TUs x two ABI PASS12; current complete renderer with
the new binder PASS2. Positive executable: COMPILED_NOT_EXECUTED after corrected
fixture. Root may execute `.local-inputs/run_world_loot_positive_v23.py` when its
isolated device lease is healthy; it never installs or controls the game app.
The test covers actual cached145 constructor/visual/PF and a selected actual-cache
potion's localized constructor, transfer, drop body/clip/visibility/reuse cleanup.
It does not prove NPC award selection or real Gear pickup and declares its device,
condition, network, empty LightSet table and currentLevel phase0 fixture inputs.
