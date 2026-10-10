# Lane 11 handoff: authored loot and pickup

## Source refresh

No functional edits were needed in the lane 11 source paths. The retained campaign owner already composes the requested system; its implementation is in `port/android-native/app/src/main/cpp/source_campaign_items_v88.cpp` and declaration in `source_campaign_items_v88.hpp`. The exact existing preparation entry point is called by `RendererCampaignProducerV89::prepare` in root-owned `renderer_campaign_producer_v89.inc`.

`SourceCampaignItemsV88::prepare` reuses the candidate's existing campaign item cache, CharacterDesign, files/localization owner, Application RNG channel 0, PlayerManager/Gear, canonical manager/property map, PhysicsWorld, floors, navigation registry, roots, and actual source actor/Character records. It creates one `WorldLootGameplayV23`; failed source-construction prefixes remain published and cannot be reconstructed. The current source Level callback is retained, but Item145 creation is performed only from the existing actual Level stage29 hook (`precache_source_campaign_items_v88`). There is no fallback pool or reward table.

Drop routes already point at the same `WorldLootGameplayV23`: character rewards use `drop_explicit_v108` from the reached source character loot table; container/destructible authored `DropLootTable` calls use `drop_table_v104` with their original table, opener, fixed powers, and flag. The same Item factory owns canonical Items, source visuals/physical state, frame updates, and despawn. Loot resources/table/item text come from `SourceItemResourcesV88` built from the real cache; drop RNG is the retained Application RNG.

Pickup routes through `LootPickupSourceLeavesV47` and the same Gear/item pool. The source owner binds actual actor/Character and Save borrows, gathering quest registration and current Level event dispatch, original Settings, status queue, item actions, tutorial ScriptManager, pickup FX owner, and same-world despawn. `OriginalUiSession::bind_item_presentation_v88` supplies tooltip lifetime and defers distinct native UI/network operations to a required continuation. Missing positive continuations return a named failure rather than success. No independent inventory/item pool is created.

## Root integration contract

Root owns the shared renderer and should connect the existing producer outputs to the active source World lifecycle: retain `SourceWorldBorrowV61::prepared_items_v88` with that World; execute the already-present actual stage29 precache once; call `update_source_campaign_items_v88(actual_world, actual_ms, actual_dt, error)` on the actual source frame; capture item visuals after source Item/Level changes; route actual character `kill_drop_loot` into the retained loot owner; preserve the authored container/destructible DropLootTable callback; and call `release_source_campaign_items_v88` during the actual source release sequence. Campaign draw submission also needs to consume `capture_source_campaign_item_visuals_v104` through the existing retained visual renderer. These are integration points outside lane 11's allowed paths (`model_renderer.cpp`, `native_app.cpp`, renderer producer and CMake are root-owned).

`renderer_canonical_loot_connection_v44.inc`, V31 actor adapters, V23 gameplay adapter, V47/V49 renderer root providers, V57 Item candidate, and V4 Item graph are earlier non-campaign composition adapters; none is textually included in current `model_renderer.cpp`. Do not instantiate them alongside the campaign owner for the same Item/World. Current `model_renderer.cpp` includes the V49 loot GPU publisher and V27 retained loot GPU support; these are not the source Item pool.

## Verification and limit

This is a source/interface handoff only. No compile, native fixture, APK, emulator or gameplay result was produced in this lane. Loot/pickup source is implemented; integrated Swamp drops, visible animation, pickup, inventory mutation, and save/reload remain unverified until root connects and exercises the actual source lifecycle. No new original reverse-engineering inference was introduced in this pass.

## Read-only PyData descriptor cross-check (lane01 support)

Cross-checked IDA pseudocode and ARM assembly against the existing specialized readers. The lane01 working descriptor changes are supported: `ItemBonusAttrMonopoly` is `[i]` (IDA `Structs::ItemBonusAttrList::read` `0x4ea954`: count then int32 values; V7 monopoly reader agrees); `LootTable` is `ii[iiiiiiii][iiiiiiii][i]` (IDA `Structs::Loot::read` `0x4fe794`: two scalars, two arrays of 8-int `LootEntry` records, then an int32 subloot array; `LootEntry::read` `0x4fec08` reads eight ints; existing `LootTablesV2` agrees); `SkillTable` is `ib[i]ibiiSbiiSiii` (IDA `Structs::Skill::read` `0x4ebeb0` and existing `SkillTables` agree on byte booleans, integer-vector `DisplayProps`, and length-prefixed strings).

Remaining sibling mismatch reported to lane01/root: `SkillListTable` is registered as `ii`, but IDA `Structs::SkillList::read` `0x4ea830` reads a count followed by that many int32 values; existing `SkillTables` parses the same count+array. Descriptor should be `[i]`. This pass made no C++ edits and ran no tests or proof pipelines.
