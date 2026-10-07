# Item text, powers and live gear effects V5

This batch uses the authoritative `FreshInventoryOwnedV4` vector, stable item/slot identities, both equipment sets, and its same shared `PropertyState`. It adds effects to that graph; it creates no detached equipment mirror and changes no frozen V2/V3/V4 source. The actual source library is bound by `original-manifest.json` (ELF SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`). Captures are static instruction evidence; the separate original/O2 reports identify which complete functions actually executed.

## Source effects and ownership

`CharProperties.LoadGearsProperties` `0x3df480` visits nine live slots. `ItemInventory.GetEquippedItem` `0x3ffe3c` calls `GetCurrentEquipSet` `0x3fc6a8`: only weapon slots 1/2 use the selected set; other slots use set 0. Gear reset copies all 224 genuine default words. `AddGearsProperty` `0x3df140` replaces a default sentinel on first addition, then performs wrapping signed32 addition. It does not perform a property-type gate or resolve after each addition.

`item_gear_properties_v5` preserves the original stats/type switch, the 49 Power attribute branches, left-hand offsets, multi-destination order and type9 direct assignment. `PlayerGearEffectsV5.update_properties` then calls the real existing class/property recalculation, including live buffs. `ValidateHPMP` `0x3bd140` clamps HP by signed minimum and completes Set/Resolve before freshly reading MP and clamping it. The adapter rejects a PropertyView that does not project the inventory's same shared state.

`ItemPowerTablesV5` owns actual original Power array/name/schema bytes: 121 leading lists, 937 definitions, 1,224 ordered property entries. Palette is one serialized byte, SpecialEffect is signed32, every property is three full signed32 words, and five scalar words follow the property list. Immutable Borrow retains all decoded backing after owner destruction. Reload while borrowed and malformed/truncated input reject atomically.

## Genuine text and Power instances

`ItemInstance.UpdateName` `0x3fb754`, `UpdateStatsDesc` `0x3fb290`, `UpdateReqDesc` `0x3facdc`, and `AddPower` `0x3fbc60` are reconstructed caller bodies. NameOID/MaterialOID are Item record fields 17/18. `Item.name` is the modular mesh name, not a localized display name. Source marker/segment grammar and real material placement are preserved; ArmorRating and BlockRating are separated by a literal space. Requirements read raw authored words and source class-name rows, without replacing them with resolved Character stats.

`ItemPresentationOwnerV5` owns complete additional Power records keyed by the actual item identity; the actual item's ID vector is mutated alongside them. Difficulty suffixes are `_Hard`/`_VeryHard`. Source `PowerInfo.swap` `0x3fb570` swaps the ID and description only: each positional SortingOrder word remains at its old position. Sorting uses those words on subsequent comparisons. This source quirk is preserved, rather than applying a conventional stable sort to complete records.

The caller must invoke `forget` before destroying an item, including failure cleanup. Destructive/reentrant AddPower is explicitly unsupported. The V4 owned creation path still rejects unrecovered powered-loot valuation; this batch does not silently turn that rejection into successful generic powered creation or cover arbitrary failed split ownership.

`ItemTextOwnerV5` borrows the genuine Item/Character table, owned `HudTextV1` and persistent Localization services. It calls real constant lookup, integer-string loading, current pack and complete existing parseEx. The new typed `item_text_varargs_v5` models the distinct `StringManager.parse` `0x508ef4` integer/string domain used by actual starter gear: signed32 `^d`, signed division `^k`, wrapping multiplication `^p`, strings/null `<null>`, escapes, pipe and source UTF spacing. Float and dollar varargs require a separate provider and explicitly fail; they are not routed through parseEx as though it were the same ABI. Application version/title providers remain required when reached. A localized null pointer is an explicit unsupported continuation, not an empty-string success.

All borrowed table, text, localization, class-row and PropertyView owners must remain stable through every call. These adapters cannot be moved into an invalidated vector while providers retain their context pointers.

## Skin caller and remaining scene boundary

`Character.INV_UpdateSkin` `0x3a999c` is fully modeled at its service boundary. Source null VisualObject genuinely skips. Nonnull source paths deliver category lookup, module lookup, placeholder fallback, Debug Load/GetSwitch, SetModular and SetWeapon in source order. Every visual call freshly reads the live VisualObject field, including after synchronous replacement. The source mappings include Torso/Feet/Hands/Head, separate weapon paths, shield mode 0, bow mode 2, and left-claw name mutation.

Required VisualObject category/module factories and actual Skin/controller graph mutation remain external genuine services. The differential fixtures expose those boundaries and compare all calls, order, arguments and live identity reloads. They do not claim original modular scene cloning, equipment mesh rendering, GPU parity or a full source Skin graph.

## Evidence and scope

| Original/O2 proof | Cases | Genuine executed data/body |
| --- | ---: | --- |
| `item-gear-properties-v5-arm64-differential.json` | 4,037 | all 1,322 Item stats both hands, Power branch projections, reset and HP/MP |
| `item-power-tables-v5-arm64-differential.json` | 937 | actual original Power reader and all 1,224 entries |
| `item-presentation-v5-arm64-differential.json` | 4,031 | actual Item callers, all metadata rows and marker/class cases; declared text fixtures |
| `item-power-instance-v5-arm64-differential.json` | 1,119 | actual append/value/sort bodies, 1,395 appends and 12 chains; declared localized fixtures |
| UI `item-text-varargs-v5-arm64-differential.json` | 355 | actual source formatter, signed32 edges and null strings; explicit localized/libc services |
| `player-skin-v5-arm64-differential.json` | 24 | 200 actual Skin invocations, all three starter classes, fallback and synchronous visual replacement; explicit factories |

`player-item-effects-v5-host-audit-v3.json` separately rebuilds and runs all six host tests with ASan/UBSan/LSan. It binds their exact sources, executables, gold, actual input bytes and seven coherent copied dependency DSOs. The same V4 starter graph compares six original cases/112 operations (Knight row263/Loot165, Mage row290/Loot174, Rogue row325/Loot213). The cache composition replaces the earlier text fixture with genuine localization, real filesystem leases, real DebugSwitches ENOENT behavior and real class/vitals effects. It formats 936 actual Power descriptions; definition 0's null Description remains explicit unsupported. Controlled nonplayer/current-player-null source projection is not a real live Application/player manager.

This is native source/caller/owned-data proof. It makes no APK, emulator, physical ARM64, whole campaign Player creation, full VisualObject factory or complete powered-loot valuation claim.

## Main integration handoff

Add four game-data TUs: `item_gear_properties_v5.cpp`, `item_power_tables_v5.cpp`, `item_presentation_v5.cpp`, `player_gear_effects_v5.cpp`. Add two engine-ui TUs: `item_text_varargs_v5.cpp`, `item_text_owner_v5.cpp`. Existing game-data/UI/world/runtime dependencies already provide genuine frozen properties/class, inventory, localization, HudText, constants and Debug modules. Keep `-fno-fast-math -ffp-contract=off`.

Host targets and arguments are recorded verbatim in the host report's `commands`/`host_audits`; `tools/audit_player_item_effects_v5_host.py` rebuilds them without touching central CMake. The cache target links all six new TUs plus genuine existing DSOs; the other targets deliberately distinguish fixture callers from real cache composition. Central integration should rerun the same gold against its newly rebuilt DSOs and issue a new receipt instead of reattributing this copied-dependency proof.
