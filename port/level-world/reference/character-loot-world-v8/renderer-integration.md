# Retained source loot V8 handoff

Generation is available against all actual cache rows. World presentation and collection require the real receivers listed below; this handoff does not install fixture callbacks or claim complete live Kill.

## Same Gear authority

`PlayerEquipmentRenderOwnerV1::loot_sources_v8` borrows its retained LootTables, ItemPowerTables, ItemTextServices, and **same** LootRandom8V2. Keep Gear and all borrowed providers alive through each operation. `loot_add_item_v8(unique_ptr<ItemInstanceV1>&,force,convert_gold,index,error)` calls its same FreshInventoryOwnedV4 AddItemInstance. `loot_add_gold_v8(amount,error)` calls its same gold authority. `check_item_requirements_v1(error)` exposes only the existing guarded original CheckItems/prune chain.

Create one `LootPowerResourcesV7` over the actual authored power/quantity blobs and the borrowed power tables. A `LootCreationV8` uses those borrows and the Gear RNG. Its destination during DropLoot is `LootTemporaryInventoryV8`, the original NULL-character inventory, not a copy of the player's inventory or properties. Supply actual Debug, class-count, CurrentLevel, ItemText and Power callbacks. Creation failures retain `pending_item()` and the source prefix. Retain its presentation owner until its registered item is forgotten before destruction.

## Canonical death call

Use the existing whole `dh2_character_kill`/`ctrl_kill` service boundary. Its **early** DropLoot call when level.loot_gate150 is zero precedes contributor/credited-killer XP. Do not hook death animation or install an unconditional XP→loot callback. One genuine Kill attempt owns its source prefix and delivery order, including failures; repeated IsDead must not award again.

`CharacterLootDropV8` resolves killer/victim through the SAME CharacterWorldRuntimeV1, borrows actual AI/property state, and constructs the source temporary inventory only for the recovered eligibility branch. Its AddLoot receiver composes `LootCreationV8`; its world-drop receiver composes `CharacterLootDropAwardV8`.

## World pool and item graph

Load `LootAudiovisualV8` from all four actual AV cache inputs. There are 29 categories. `CharacterLootItemManagerV8::precache` requires genuine ObjectManager Spawn("Item", "ItemObject_%02u_%02u", false, true), canonical World identity/type lookup, ItemObject InitOnce and DeSpawn receivers. It retains five borrowed ItemObjects per category: 145 total. Keep that exact World graph alive until pool flush, then destroy the World objects through their real owner. Flush clears borrows; it does not destroy scene/body objects.

Each ItemObject owns ONE `LootItemFieldsV8` constructor projection and ONE source temporary inventory. InitOnce applies source speed6 and the real AV Visual mesh from `data/3D/GameObjects/itemdrops.bdae`. InitAgain must implement real inventory TransferItemTo, material/color, AV audio and physical-object creation. A NULL new character does not clear the old source owner3bc field. Enable/SetPhysical/RemoveAllItems/Play3D are required receivers; no empty successful implementations are supplied.

Spawn preserves source cursor advance before slot handling and source SetPosition→SetDestination→InitAgain→Enable→enabled85 ordering. DeSpawn preserves active=false→Enable(false)→RemoveAllItems(true)→SetPhysical(NULL,false)→enabled85=0. Retain live ItemObject fields and inventory over graphics reload if World remains alive; rebind graphics through actual scene lifetime.

## Scatter and player producers still required

`CharacterLootDropAwardV8` needs actual source position160, original `_GetRandomDropPos` over the SAME Gear Random, `PlayerManager.GetLocalPlayer(0,true)` and its actual Character660, pool Spawn and internal GetItem(0). It performs the original PickUpType/Automatic comparison and calls whole Interact only when that type and killer match. No pickup radius or fallback asset is introduced.

Direct original GOT resolution proves scatter3ec5d4 and loot401bxx share seed99f89c and call-counter99f8a4. `character_loot_scatter_v8` borrows the same Gear Random; no second stream. NULL-killer draws500 twice; killer-present requires original Normalize34d0b0 and actual Vec3f_K global99f878 before draws200 then300. No floor/path query occurs in this leaf. All256 source scatter comparisons pass at O1/O2 ASAN+UBSAN,257 checks each; unit-delta normalization and basis are explicit fixtures, not proof of those two production producers. Compile scalar kernel with floating contraction disabled to retain original operation ordering.

`player_manager_loot_queries_v8` reconstructs class counts from fresh manager count6c4 and GetPlayer(index,true), then same Character cached signed16 base-id13c8. Warrior263/Mage290/Rogue325 are original literals. It cannot use Save.class_id or invented zero counters. Friendly/internal/local-ID maps and online branches remain real required PlayerManager receivers. The current development single-player query is not the full original manager.

## Whole collection boundary

`CharacterLootInteractV8` preserves owner/lock/player gates, Save AutoTransmute, inventory/potion capacity, text/tutorial, transfer/transmute, property223/trophy and empty-inventory network/audio/tooltip/FX/despawn/stat ordering. Its typed service operations are mandatory only when reached. Source pickup_override58 is initialized -1 by ItemInstance C1; expose its actual retained field rather than choosing an inferred pickup type.

`LootTemporaryInventoryV8::transfer` preserves destination AddItemInstance before source slot removal. Failure after destination consumption keeps a borrowed source-slot alias to the same item; destination must outlive that failed prefix. Read `peek()`, never assume `items()[i]->item` still owns the object after failure. Retrying that prefix is rejected. Capture itemID before transfer because real stack merge/gold conversion can destroy it. Whole TransferAll also requires original async quest/gold tails; a loop of Gear mutations alone is not full collection.

After successful same-Gear mutation, root refreshes its borrowed Character/UI snapshot. Collection does not create another Save, inventory, property or RNG authority.

## Verification and limitations

All339 original AddLoot tables compare IDs, quantities, values, powers and RNG after each table: 1351 localized items,1078 powered,6701 draws. O1/O2 ASAN+UBSAN:20626 checks each including transfer failure prefixes. Actual pool metadata all29 rows and145 slots compare with original AV reader; native source coordinator covers175 spawns using declared scene/audio/body fixtures. Player class-count host tests cover fresh dynamic bounds, NULL Character and missing required fields; they do not claim a live PlayerManager. Interact, DropAward and WorldDrop currently have strict native syntax checks, not a whole production World oracle. See reports for exact source hashes and dependency attribution.
