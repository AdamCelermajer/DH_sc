# Items batch fidelity audit (2026-10-11)

## Progress
- last_address_done: core gameplay scope complete (loot roll, item gen, equip gate, inventory capacity, pickup, merchant, drop scatter, gold)
- groups_done: loot selection/quantity/pct/weights (0x401b90-0x403310, 0x4039a0, 0x40407c), Random::GetRandom, ItemInstance::IsEquippableBy/AddPower/_UpdateStats, ItemInventory capacity/_AddItemInstance/Gold, ItemObject drop/pickup (0x3ec668-0x3ed144), NativeMerchantBuy/Sell
- next (not done, lower priority): rnd::RandomGenerator level generator, Reward*/RewardList::Give details, equip/unequip slot helpers line-by-line, NativeInvGetItemDetails text, UpdateReqs colouring, stack-merge in _AddItemInstance

## Summary
Scope note: the port keeps a very literal data-layer copy of the loot algorithm (port/game-data/loot_*_v7/v8.cpp) that matches the IDA code line for line, but the live Windows game does NOT run most of that pipeline.

Counts (gameplay-relevant groups): faithful 17, divergent 2, partial 3, missing 3, unclear 5, n/a ~4 groups.

Top risks:
1. Live death drops never generate bonus attributes (powers), difficulty variants, value, or qty -2 -> 99 (high).
2. Merchant buy/sell (NativeMerchantBuyItem/SellItem, price formula, buyback rate) not implemented (high).
3. Death-drop admission gate (killer is a player, or victim boss/miniboss, or victim == killer) not replicated (medium).
4. Killer power bonus (property 196) never passed; RNG draw sequence after a drop differs from original (medium).
5. AutoTransmute pickup branch absent (medium; masked by #1 since powers are always 0).

## Findings

### F1 (high) Live death-drop path skips the item-generation stage of AddLoot
- IDA: ItemInventory::AddLoot 0x40407c. After table/item selection, per item: (a) if ItemTable type word 4 is 2 or 3, repeat count = player count (0 skips); (b) Hard (level 1) / VeryHard (level 2) swaps item id to id+1 / id+2 when name+"_Hard"/"_VeryHard" equals that row name (and id+2 < table size, unless bypass flag); (c) quantity -2 becomes 99; (d) _AddLootItemPowers 0x403310 with power bonus = killer property 196, then CalcLootItemValue 0x4020b4 with value bonus = property 195; (e) _AddItemInstance.
- Port: faithful implementation exists in game-data/loot_creation_v8.cpp:4-36 (LootCreationV8::add), game-data/loot_power_creation_v7.cpp (add_powers, dh2_loot_item_value_v7) and features/loot/original_loot.cpp:75 (OriginalLootFlow::generate), but OriginalLootFlow::generate is referenced only from tests. The live path is features/loot/runtime_death_rewards_v1.cpp:43-55 (select_death_items: table select + expand only) and :395-440, which spawns a world-item record {item id, quantity} and only for gold calls dh2_loot_item_value_v7 with property 195. Inventory entries are InventoryItem{instance_id, definition_id, quantity} (character_state.hpp:35), no powers/value. features/loot/world_drop_rules_v1.cpp:137 says "Dropped items carry no powers in this port"; BUGS-AND-IMPLEMENTATION.md B016 scopes to "unpowered ItemTable rows".
- Consequence: all dropped gear is unpowered (no random bonus attributes, no power-count colouring), Hard/VeryHard drops use Normal item rows, qty -2 (99 stack) not applied, sell value is base only. Large loot-progression difference.
- Confidence: high (no add_powers / LootPowerCreationV7 use outside game-data and tests).
- Fix: route death/chest drops through LootCreationV8::add into a retained ItemInstance carried by world item and inventory; extend InventoryItem with powers/value.

### F2 (high) Merchant buy/sell logic missing
- IDA NativeMerchantSellItem 0x44058c: price = max(1, (int)((float)item.value * MerchantTable[GetLoot].word@+4 / 100.0f)); item flagged sold-by-player (byte +105); gold added; stat 220 +1, trophy at >99 sold. NativeMerchantBuyItem 0x4408d4: rate = flagged ? MerchantTable+4 (buyback) : +8; price = max(1, (int)(rate/100.0f * value)); returns 1 if price > gold, 2 if inventory full and item not potion (type 14), 3 if potion count == potion capacity; else AddGold(-price), clear flag, transfer, stat 221 += price, trophies at >=1000 / >9999.
- Port: no NativeMerchant* or price code found in windows-foundation; features/interactions/npc_interact.cpp:32-59 only opens the merchant menu and seeds stock with AddLoot(id,0,0,-1,false).
- Consequence: no buying/selling with original prices/buyback rates. Confidence: high for absence in windows-foundation (grep); another location not searched could exist.
- Fix: implement both with the exact float math (float multiply then truncate, min 1) and return codes.

### F3 (medium) Death-drop admission gate not replicated
- IDA ItemObject::DropLootTable 0x3ecba0: drop only if killer object exists and (killer is player-type [vfunc +40] or victim == killer), OR victim is Boss/MiniBoss; no killer means no drop unless boss. Killer bonuses (195/196) read only when killer handle valid, else AddLoot(table,0,0,fixed,0).
- Port: runtime_death_rewards_v1.cpp:331-440 drops for every non-player death event regardless of killer.
- Consequence: kills by traps/other NPCs drop loot in the port but not in the original (unless boss). Confidence: medium (Character::DropLoot caller is outside this batch and may gate earlier).

### F4 (medium) Power bonus (property 196) never passed; RNG stream differs
- IDA ItemObject::GetInventoryToDrop 0x3ecae4 passes killer props 195 and 196 to AddLoot; the power bonus (>>8) shifts the power-count draw. Powers consume RNG before gold value draws.
- Port reads only 195 (runtime_death_rewards_v1.cpp:381); no power draws, so later scatter and subsequent rolls are shifted versus the original stream. Mostly consequence of F1.

### F5 (medium) AutoTransmute pickup branch absent
- IDA ItemObject::Interact 0x3ed144: equippable item with NumPowers < saved option "AutoTransmute" is converted to gold (Character::INV_TransmuteItem) instead of picked up.
- Port: world_drop_rules_v1.cpp:138 returns auto_transmute_unavailable if option>0; option never bound (stays 0). Masked because powers are always 0.

### F6 (low) Class gate disabled for legacy profiles
- IDA ItemInstance::IsEquippableBy 0x3fa330: online bypass; level (prop 19) >= req<<8 and four stats (base + prereq) >= req<<8; class lock Item[34] 1..9 mapped to CharacterTable names, compared with player class.
- Port features/equipment/equipment_adapter.cpp:20-50 matches (props 19,149-152 plus 153-156; same nine class names) but equipment_class_allows returns true when player_class_id is empty (legacy ClassTables profiles). Commented in code; faithful for source-class characters.

### F7 (low) Single RNG seed
- IDA Random::GetRandom 0x33ff90 uses synced seed when flag set, else s_seed. Port (game-data/loot_tables_v2.cpp dh2_loot_v2_random) uses one seed; LCG identical ((59051*s+177149) % 0xDAF26B, abs(s % n), n==0 does not advance). Only matters for multiplayer.

### Verified faithful details (no action)
- _DoPctRoll: GetRandom(100) <= Pct passes (inclusive); port percent_roll fails on draw > pct. _IsLootEntryUsingPct = Pct <= 100 or InfiniteLootDrops. _GetEffectiveProb = Prob + MProb*mages + RProb*rogues + WProb*warriors (IDA offsets are 1 higher than port word indices; checked Prob=5, MProb=2, RProb=6, WProb=7).
- _GetRandomLootEntry returns index 0 when total weight is 0 (even if entry 0 is Pct-based); port keeps the quirk.
- _AddLootTable order (fixed, NumProb quantity with bonus 0, weighted picks, Pct rolls over own list, sub-loot recursion with collector, nested roll afterwards): port loot_table_selection_v8.cpp:11-30 matches.
- _GetProbQuantity (sum, draw+bonus clamp sum-1), _GetRandomItem (int16 weights), _GetRandomItemPowerEntry (signed byte weights): match.
- _AddLootItemPowers: take-all if quantity >= list size; else weighted picks with AttrMonopoly rejection, abort after 10 consecutive conflicts, then deterministic fill pass using the same used-type set: loot_power_creation_v7.cpp:56-106 matches (IDA v54/v55 are return-pair buffers, not separate sets).
- CalcLootItemValue: gold (rand(max+1-min)+min)*(100+(bonus>>8))/100 truncated; others base*qty + sum(power mult*bonus): match.
- IsInventoryFull (>=100 slots unless InfiniteInventory), Interact gates (owner window 5000 ms, potion capacity, full check only for equippable items): world_drop_rules_v1.cpp:98-160 match. AddGold/SetGold clamp: runtime_world_item_adapter_v1.cpp:47-69 match.
- _GetRandomDropPos: world_drop_rules_v1.hpp:56 documents 150+rand(200) along killer-victim direction, lateral rand(300)-150, no-killer +-(rand(500)-250); dword_C8=200 confirmed, 300/500 inferred from address-valued args and not independently confirmed.
- SortByValueAndClass class multipliers: equipment_adapter.cpp:63-80.

## Mapping table
| IDA address | IDA function (or group) | port location(s) | status | note |
|---|---|---|---|---|
| 0x33ff90 + clones | Random::GetRandom | game-data/loot_tables_v2.cpp dh2_loot_v2_random | faithful | single seed (F7) |
| 0x401b90 | ItemInventory::_GetProbQuantity | game-data/loot_power_creation_v7.cpp:28 | faithful | |
| 0x401dbc | _GetRandomItem | game-data/loot_item_selection_v8.cpp:6 | faithful | |
| 0x401f38 | _GetRandomItemPowerEntry | loot_power_creation_v7.cpp:20 | faithful | |
| 0x4027a4/0x402854/0x402bdc | _IsLootEntryUsingPct/_GetEffectiveProb/_DoPctRoll | loot_entry_selection_v8.cpp | faithful | |
| 0x4028d0/0x402a60 | _GetRandomLootEntry (2 overloads) | loot_entry_selection_v8.cpp weighted_index | faithful | |
| 0x402dfc | _AddLootItems | loot_item_selection_v8.cpp expand | faithful | |
| 0x4039a0 | _AddLootTable | loot_table_selection_v8.cpp | faithful | |
| 0x403310 | _AddLootItemPowers | loot_power_creation_v7.cpp add_powers | faithful (data) / missing (live) | F1 |
| 0x4020b4 | CalcLootItemValue | loot_power_creation_v7.cpp loot_item_value_v7 | faithful (data) / partial (live: gold only) | F1 |
| 0x40407c | ItemInventory::AddLoot | game-data/loot_creation_v8.cpp; features/loot/original_loot.cpp | divergent (live path uses select_death_items) | F1 |
| 0x3ecae4 | ItemObject::GetInventoryToDrop | runtime_death_rewards_v1.cpp:375-385 | partial | only prop 195; F4 |
| 0x3ecba0/0x3eccc8 | ItemObject::DropLootTable | runtime_death_rewards_v1.cpp:331-440 | divergent | no admission gate (F3) |
| 0x3ec8a0 | DropAndAwardLoot | runtime_death_rewards_v1.cpp spawn path | faithful | |
| 0x3ec974 | DropInventory (player drops, 5000 ms protect) | world_drop_rules_v1.cpp drop_item_to_world | faithful | |
| 0x3ec668 | _GetRandomDropPos | world_drop_rules_v1.cpp scatter_destination_v1 | faithful | constants partly unverified |
| 0x3ed144 | ItemObject::Interact | world_drop_rules_v1.cpp:98-160 | partial | AutoTransmute (F5) |
| 0x3ec474 | _DoAutoPickupHack | main.cpp:3340-3400 automatic loop | faithful | |
| 0x3fa330 | ItemInstance::IsEquippableBy | equipment_adapter.cpp:20-50 | faithful | F6 |
| 0x3fbc60 | ItemInstance::AddPower (_Hard/_VeryHard power variants) | service hook in loot_power_creation_v7 only | missing (live) | F1 |
| 0x3fb290 | ItemInstance::_UpdateStats (text only) | inventory_details.cpp | unclear | text rows not compared |
| 0x3fe330 | IsInventoryFull | world_drop_rules_v1.cpp:140 | faithful | |
| 0x3ff5d4 | _AddItemInstance | runtime_world_item_adapter_v1.cpp | unclear | stack merge / potion cap not compared line by line |
| 0x3fdfd8/0x3fe164 | SetGold/AddGold | runtime_world_item_adapter_v1.cpp:47-69 | faithful | |
| 0x3fd1e8 | SortByValueAndClass | equipment_adapter.cpp:63 | faithful | |
| 0x44058c | NativeMerchantSellItem | none | missing | F2 |
| 0x4408d4 | NativeMerchantBuyItem | none | missing | F2 |
| 0x43f23c / 0x43cfb4 | NativeInvTransmuteItem / NativeInvDropItem | equipment_inventory_actions_v1.hpp | unclear | callee Character::INV_TransmuteItem not in batch |
| 0x3fc*-0x401* | ItemInventory equip/unequip/slot helpers | features/equipment/* | unclear | not compared line by line |
| 0x482870-0x483950 | Reward*/RewardList::Give | not located | unclear | likely quests batch |
| 0x483a94-0x491d90 | rnd::RandomGenerator (modular level gen) | not examined | unclear | out of focus |
| 0x4a58e4-0x4bacc8 | Arrays::*::finalize/read (loot/item tables) | game-data/loot_tables_v2.cpp, item_power_tables_v5.cpp | n/a | data loaders |
| 0x39f324-0x3a2ce4 | Container/SlotContainer | features/interactions/* | n/a | see world batch |
| 0x31f484-0x328c84; Net*/Serialize/ctors/dtors/thunks/DragAndDrop/Menu* chrome | multiplayer, boilerplate, UI chrome | - | n/a | |
