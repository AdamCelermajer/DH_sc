# Authored character mutations V4 and source inventory drop V9

Stable production supplement: `character_menu_mutations_v4.hpp/.cpp`;
`renderer_character_mutations_v4.inc`; additive
`PlayerEquipmentRenderOwnerV1::bind_menu_item_actions_v4`; additive original
NULL-character `FreshInventoryOwnedV4::create_drop_temporary_v4`. Historical
constructors, structures and request layouts remain unchanged.

The supplemental query frame owns the exact ItemActions/SaveActions used by
the current synchronous Queries dispatch and pins the original graph lease.
It does not retain a stack Actions pointer beyond that dispatch and has no
strong ownership cycle. Gear supplies its actual inventory, PropertyView,
effects, ItemText and source-only Skin. No detached inventory is substituted.

## Save

Character.SG_Save3bc4a8 reads SAME Character14e8. Save.SG_Save464b2c returns
before all online/file operations when SAME profile8 is NULL or disabled_c is
set. Slot−1 alone is not the guard. Existing PlayerSaveWriteOwnerV1 preserves
this branch. NativeSaveGame's subsequent skill/achievement checks still run.
The development no-profile branch writes no disk file; this is not a fabricated
successful profile save. A non-NULL campaign profile requires the actual
PlayerSaveWriteServicesV1 named-section/file/job/network producers. Portable
binding accepts those real services; current renderer reports required writer
operations because no complete campaign writer is retained there.

## Transmute and gold

The actual design_pycst cache db014f9832c1d9e60c62272a2ee67d15568d003c191ae32bbce070cb49056931
stores CharacterDesign -> TransmuteMultiplier25. Current lookup uses that
real group/key order. Existing ItemActions executes original fixed-point
value arithmetic, quantity/remove, gold store, property213 and source Skin.
The supplement supplies whole reached SetGold3fdfd8 notifications: IsPlayer,
IsLocalPlayer, then strict thresholds9999/99999/999999 and actual trophy keys
gear_10kgold/gear_100kgold/gear_1mgold. Gold is reread after every trophy call;
required trophy errors preserve the actual storage/mutation prefix.

## Shared original local-player identity

`player_network_local_owner_v4.hpp/.cpp` supplies a narrow constructor/query
owner, shared by menu and the renderer Hit adapter. Actual PlayerInfo vtable50
and CNetPlayerInfo vtable50 both resolve80f1ec; local66c is NOT this predicate.
Source CNet Reset80f27c sets owner1a0−1. Matching singleton a2f0e4 is .bss0;
mode99e234 is .data initial1. Get800f8c selects Local for1 and normalizes an
explicit0 to1. CMatching base C2 initializes joined_c0; Local C1/Reset stores
member3638−1/server363c−2. IsLocal uses original IsServer and compares the
actual owner1a0 to that same retained member, preserving server/negative-owner
branch semantics. Other matching modes require their actual constructors.

This is the source identity/storage domain, not full network/room/NetStruct
serialization completion. No empty packet or room implementation is installed.
The PlayerInfo successor is published by construct_player_info BEFORE manager
map publication. Its storage is associated with the SAME record; frozen
PlayerInfoFieldsV1 layout is untouched. It is destroyed after that manager.

Root insertion: include player_network_local_owner_v4.hpp, add
unique_ptr<PlayerNetworkLocalOwnerV4> player_network_local BEFORE player_manager
in PlayerSkillsRuntime. Owned renderer_player_manager_v1.inc already calls
actual constructor service + initialize(), and intercepts Hit.local with the
same player_network_is_local_v4 helper. Its Application Matching identity owner
is retained across World/GL replacement. Keep a single such owner if another
future front/network source runtime is integrated.

## Source menu DropInventory

DropInventory3ec974 is DIFFERENT from DropAndAwardLoot3ec8a0. It performs
scatter and SAME ItemManager Spawn, then source IsCharacter virtual24 and
PlayerManager.GetByCharacter(false).friendly678 -> dropped Item.lock3b8=5000,
player_id3c0=signed16(index). It does NOT execute automatic pickup.

New files loot_inventory_source_v9.hpp, fresh_inventory_loot_source_v9.cpp,
character_loot_item_source_v9.hpp/.cpp, world_item_init_again_source_v9.cpp and
world_item_drop_inventory_v9.hpp/.cpp provide the typed route from actual
Fresh NULL source to the SAME existing145 pool and Item InitAgain. This moves
the actual unique item, with no clone, second temporary vector or pool. Old V8
methods/signatures/layout remain unchanged. bind_menu_drop_world_v9 installs
the endpoint over a real retained WorldLootItemRuntimeV1 and actual source
IsCharacter/friendly-index providers. Current production renderer still lacks
the complete pooled Item factory/visual/audio/PF/interaction binding; no live
drop success is claimed. Positive native pool test declares those boundaries.

## HUD and renderer binding

Include renderer_character_mutations_v4.inc immediately after
renderer_player_character_panel_v3.inc. Final connect_character_panel_runtime_impl
step is supplement_character_menu_mutations_v4(out,error), preserving original
player queries. NativeApp binds actions.swap_hud to actual HUD.activate for
DisplayRightHud, then HUD.refresh_action_icon for FillActionIcon. The latter
uses SAME source MenuManager action-icon cache (including−1), not weapon
inference. NativeApp/OriginalUiSession own these borrowed AS endpoints.
Existing renderer frame2811-14 rereads real Gear draw parts after mutations.

## Build and proof

New required mutation TUs: character_menu_mutations_v4.cpp,
player_network_local_owner_v4.cpp, player_save_write_owner_v1.cpp if absent.
Drop successor TUs: fresh_inventory_loot_source_v9.cpp,
character_loot_item_source_v9.cpp, world_item_init_again_source_v9.cpp,
world_item_drop_inventory_v9.cpp plus existing world_loot_item_runtime_v1.cpp
and character_loot_drop_award_v8.cpp if absent.

Both ABI strict source/renderer candidate checks PASS. Android receipts:
character-menu-mutations-v4 PASS91 (real cached tables, gold/Save/NULL-temp
guards); world-item-drop-inventory-v9 PASS176 (SAME145 identity/order/failure
prefix with declared world providers); player-network-local-v4 PASS163
(same-record ctor/publication,72 predicate cases and required other modes).
Original ARM IsLocal+IsServer+actual source Local vtable getters PASS72 with
zero mismatches; only CMatching.Get is a declared same-receiver fixture.
Receipts under reports/android-native-owner-tests and original gold/source
captures in this directory. No live transmute/drop/campaign persistence result
is represented by these isolated owner proofs.
