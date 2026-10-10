# Original loot bridges

`OriginalLootFlow` uses the original immutable loot/power authorities and a
caller-owned `LootRandom8V2`. `generate` runs `LootCreationV8`, including source
table selection, difficulty variants, authored quantities, powers, item value,
localized construction, and insertion into its actual NULL-character inventory.
The caller supplies live Debug/player/difficulty/power/text services. Missing
services fail explicitly; no default balance or successful no-op is supplied.

For the clean runtime, `transfer_drop` takes the caller-owned world-item
`LootTemporaryInventoryV8` and destination services. The eligibility provider
must implement original inventory-full, potion and transmute admission. The
actual item instance then transfers using source `force=false/convertGold=true`,
followed by the captured item-ID quest tail and `AddGold(0)` tail. Fullness must
be decided before transfer: a reached effect failure deliberately retains the
original partial prefix and forbids destructive retry. Empty duplicate pickups
return `already_empty` without duplicate effects.

`original_loot_world.cpp` is an optional adapter for a host that already owns the
recovered canonical world graph. `drop` uses its existing pool and `pickup` uses
its existing world-item interaction services, keeping ownership and published
identities in that graph. It requires the original level-world/scene/physics
dependencies; it is not required for the clean transfer bridge.

For the production Character death path, use the existing level-world
`CharacterLootLiveV22` owner instead of constructing an `OriginalLootFlow`.
Bind it to the same initialized `WorldLootItemRuntimeV1` pool once; it borrows
the existing `CharacterWorldRuntimeV1`, `PlayerManagerOwnerV1`, source loot
tables/power/text providers and the same Application `LootRandom8V2`. Its
`route` handles the original `kill_drop_loot` service only. The source producer
is `Character::GetLoot` (`0x3a2fcc`), which reads Character+`0x101c` (resolved
property word 9); `Character::DropLoot` (`0x3a5ae4`) calls
`ItemObject::DropLootTable` (`0x3ecba0`). `Character::Kill` (`0x3a5b18`)
reaches that call only when current Level+`0x150` is zero, before contributor
XP. The native XP caller is `Character::DistributeXP` (`0x3bf828`), routed by
`CharacterKillRewardsLiveV31` into `CharacterProgressionWorldV23` and
`progression_give_xp_v1`; source XP is property index 33 and level is index 19.

The foundation `SourceDeathRewardBindingV1` is a thin lifetime/attachment
owner for that existing path. Call `bind` only after
`WorldLootGameplayV23::initialize` has prepared and precached its actual Item145
graph. It retains `CharacterKillRewardsLiveV31`, which dispatches the native
`kill_drop_loot` and `kill_distribute_xp` service requests in their source Kill
positions. The binding constructs no loot tables, temporary inventory, item
pool, player record, RNG or Level. Root must give the binding the strong lease
for the same application owners and keep the normal
`KillLevelProviderV23`/`CharacterKillProductionV23` on the same published
`CanonicalLevelContextV1`; that Level's C1-backed `KillLevel16` supplies the
actual `+0x150` word and its `GameEventManager` handles immediate quest events.

The complete runtime still requires root to bind the actual live Level gate,
Character/property graph, Item145 InitAgain/DropAndAwardLoot receivers, source
pickup effects and same Gear/save owner. Pickup must mutate that current Gear
through `WorldItemPickupInventoryV10`; refresh
`SourceInventoryInstanceResolver` after transfer so its projected stable ID
continues to resolve the same native item pointer and current source index.
The native-table and bridge tests below do not establish the full live
death-to-save round trip. See
`reports/act1-loot-reward-integration.json` for the exact remaining bindings.

`SelectedLoot` pins the original tables while selected entry/item pointers are
borrowed. Selection alone does not produce item stats, powers, or value. Keep
the actual generated `ItemInstanceV1` in authoritative inventory storage; a UI
projection containing only definition/quantity must not discard those fields.

Native tests accept an explicit original `data/pydata` directory containing the
three `loot_table_py*.bin` files. They check all fixed list identities/quantities,
fullness retention, same-instance pickup, duplicate pickup, required-provider
failure, and a reached quest-failure prefix. Destination fixture behavior in
these tests is explicitly supplied; it does not establish powered generation
parity or complete inventory/quest gameplay integration.
