param([string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath($RepositoryRoot)
$compiler = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $root '.local-inputs/runtime_world_item_adapter_v1_tests.exe'
$sources = @(
    'port/windows-foundation/features/loot/runtime_world_item_adapter_v1_tests.cpp',
    'port/windows-foundation/features/loot/runtime_death_rewards_v1.cpp',
    'port/level-world/player_progression_v1.cpp',
    'port/windows-foundation/features/loot/runtime_world_item_adapter_v1.cpp',
    'port/windows-foundation/features/loot/runtime_world_item_interaction_v1.cpp',
    'port/windows-foundation/features/inventory/inventory_feature.cpp',
    'port/game-data/loot_table_selection_v8.cpp',
    'port/game-data/loot_item_selection_v8.cpp',
    'port/game-data/loot_entry_selection_v8.cpp',
    'port/game-data/loot_power_creation_v7.cpp',
    'port/game-data/loot_power_resources_v7.cpp',
    'port/game-data/item_power_tables_v5.cpp',
    'port/game-data/item_presentation_v5.cpp'
)
$includes = @(
    'port/windows-foundation', 'port/game-data', 'port/level-world',
    'port/engine-animation', 'port/engine-skinning', 'port/scene-materials',
    'port/script-runtime', 'port/physics-backend/box2d-2.0.1/Include'
)
$arguments = @('-std=c++17','-O1','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-Wno-missing-field-initializers')
$arguments += $includes | ForEach-Object { '-I' + (Join-Path $root $_) }
$arguments += $sources | ForEach-Object { Join-Path $root $_ }
$arguments += ('-L' + (Join-Path $root '.local-inputs/windows-foundation-build'))
$arguments += ('-L' + (Join-Path $root '.local-inputs/windows-foundation-build/physics-backend'))
$arguments += @('-static','-lfoundation_data','-lcontent_xml','-ldh2_freetype237',
    '-lrecovered_trigger_contacts','-lrecovered_content','-ldh2_box2d_201',
    '-lkernel32','-luser32','-lgdi32','-lwinspool','-lshell32','-lole32',
    '-loleaut32','-luuid','-lcomdlg32','-ladvapi32','-o',$output)
& $compiler @arguments
if ($LASTEXITCODE -ne 0) { throw "Runtime world-item adapter compile failed ($LASTEXITCODE)" }
$assetRoot = Join-Path $root '.local-inputs/windows-shared-assets'
$powerRoot = Join-Path $root '.local-inputs/player-loot-v7/cache'
$resultText = & $output $assetRoot $powerRoot
if ($LASTEXITCODE -ne 0) { throw "Runtime world-item adapter test failed ($LASTEXITCODE): $resultText" }
$result = $resultText | ConvertFrom-Json
if ($result.validation -ne 'PASS') { throw 'Runtime world-item adapter audit did not pass' }
$hash = { param($path) (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $root $path)).Hash.ToLowerInvariant() }
$report = [ordered]@{
    validation = 'PASS'
    version = 1
    test = $result
    source_sha256 = [ordered]@{}
    original_binary_sha256 = & $hash '.local-inputs/libDungeonHunter2.so'
    actual_loot_cache_sha256 = & $hash '.local-inputs/windows-shared-assets/original-cache/data/pydata/loot_table_pyarray.bin'
    actual_loot_names_sha256 = & $hash '.local-inputs/windows-shared-assets/original-cache/data/pydata/loot_table_pyarraynames.bin'
    actual_power_cache_sha256 = & $hash '.local-inputs/player-loot-v7/cache/num_prob_records_v7.bin'
    scope = 'Same-gameplay-instance world-item map over actual selected source Loot outcomes, the retained original LootTables/ItemTable snapshot, exact victim ActorState position and the same CharacterState. The explicit-interaction dispatcher accepts only an exact source Interact item ID after caller-side eligibility checks. Type13 GoldStack value is calculated once at source loot creation using the caller LootRandom8 and actual AddLoot bonus, retained on the world record, then credited atomically without pickup-time RNG. It never chooses nearest items or invents a distance gate. No native ObjectManager/C1, duplicate item registry, fixed rewards or persisted world map is claimed.'
    lifecycle = 'Runtime IDs are monotonic per adapter/gameplay-instance lifetime. A consumed ID is absent and cannot be picked up again. The store must live as long as its gameplay world and is cleared when that world is destroyed; persistent world-drop recovery is outside this adapter.'
    source_evidence = [ordered]@{
        loot = 'Original LootTableSelectionV8/LootItemSelectionV8 produced the tested item descriptors from authored Swamp_Basic_Loot and Barrel_Level_01 rows using a test-owned stream; runtime publication receives the caller-selected item ID, quantity, Loot row/entry pointers and victim position without rerolling.'
        item = 'Every world record pins its exact ItemTable row pointer via the same LootTablesV2::Borrow and preserves the source quantity.'
        pickup = 'Ordinary CharacterState inventory mutation uses existing features/inventory/Presenter::pickup; stack arithmetic and validation run on a detached copy, then the original owner is committed only by no-throw swap after drop erasure. Gold uses source AddGold value/limit arithmetic on the staged CharacterState and the same no-throw erase/swap commit; it consumes no RNG.'
        interaction = 'Original `ItemObject::Interact` at 0x3ed144 is the explicit transfer receiver; `OnCollisionBegins` at 0x3ec048 updates tooltip/current-target state only. `ItemObject::_DoAutoPickupHack` at 0x3ec474 calls the virtual Interact receiver only for exact Automatic pickup type. `ItemInstance::GetPickUpType` at 0x3f9e34 uses instance +0x58 override or ItemTable word3. Interact contains no distance/radius comparison. The dispatcher therefore requires an explicit, already-admitted source-interaction request and does no proximity search.'
        interaction_test = 'The linked test selects an actual Swamp_Basic_Loot outcome with original LootTableSelectionV8/LootItemSelectionV8 (seed 4, 4 draws), publishes exact ItemTable row/quantity/position into the store, and sends the exact current-player/item source-interaction request. It also rejects mismatched current player and item IDs, verifies one transfer/retire, and rejects duplicate delivery without changing inventory. The source receiver and store test share no extra RNG or inventory owner.'
        gold = 'The actual Barrel_Level_01 outcome resolves to ItemTable item418 GoldStack01, type13, with value range words27..28 = 1..10. After original LootTableSelectionV8/LootItemSelectionV8, source loot_item_value_v7 draws the same caller-owned LootRandom8 stream over that range and applies source AddLoot value_bonus256 from same-world killer property195; absent/deleted killer uses the recovered 0/0 baseline. The adapter-specific fixture supplies explicit bonus0 only as a test input: seed1 has 3 selection calls, then the value kernel yields6 and advances to call4. Runtime death rewards with an existing killer and unavailable properties emit a typed diagnostic, consume the exact range draw and skip publishing only that gold record. Native _AddItemInstance 0x3ff5d4 with convertGold=true reads ItemInstance+0x54, calls AddGold 0x3fe164, then destroys the item. The store retains the resolved value and pickup performs no random draw. ItemInventory C1 at 0x3ff224 stores `mvn r2,#0x80000000` to +0x28 at 0x3ff230, which yields INT32_MAX; constructor default is corroborated by ItemInventoryV1/FreshInventoryOwnedV4 header35. Load clamps serialized gold against this limit; owned APIs expose only explicit projection/test setters, not a recovered production limit setter. An optional same-owner limit override is supported; absent override uses the proven constructor baseline.'
        presentation = 'Render enumeration returns the exact authored IconName only when ItemTable IconName is nonempty and carries source word17 text OID; it does not infer a display string or substitute an asset.'
    }
    limitations = @(
        'This generic world store is feature-owned and is not yet hooked by root Session/main or a native ObjectManager/Item145 publisher.'
        'World item IDs and the map are runtime-only; save/restart persistence and world-drop serialization are not added.'
        'The source ItemTable in the current original data has no authored IconName mappings, so the exercised rows are enumerable but not icon-renderable.'
        'The generic death-reward bridge requires the source caller to supply its exact AddLoot value_bonus256 input for type13 creation; when absent it consumes the source value draw but does not publish that gold item. The adapter does not create ItemInstance or own a second RNG. The generic world store has no native AddGold notification/effects owner; it reproduces staged value/cap arithmetic only. Potion capacity and native equipment/power/transmute paths remain fail-closed; callers must preserve those gates before submitting the explicit transfer-branch request.'
    )
}
foreach ($path in @($sources + 'port/windows-foundation/features/loot/runtime_death_rewards_v1.hpp' + 'port/windows-foundation/features/loot/runtime_world_item_interaction_v1.hpp' + 'port/windows-foundation/features/loot/runtime_world_item_adapter_v1.hpp' + 'port/game-data/loot_tables_v2.hpp' + 'port/game-data/loot_tables_v2.cpp' + 'port/game-data/items.hpp' + 'port/game-data/item_inventory_v1.hpp' + 'port/game-data/item_inventory_v1.cpp' + 'port/game-data/fresh_inventory_owned_v4.hpp' + 'port/game-data/fresh_inventory_owned_v4.cpp' + 'port/windows-foundation/character_state.hpp' + 'port/windows-foundation/features/inventory/inventory_feature.hpp' + 'port/windows-foundation/features/inventory/inventory_feature.cpp' + 'port/level-world/reference/character-loot-world-v8/original-source.asm' + 'port/level-world/reference/character-loot-world-v8/original-functions.json' + 'port/game-data/reference/player-inventory-v1/captures/core/reference/original-functions.asm' + 'port/game-data/reference/player-inventory-v1/captures/core/original-functions.json')) {
    $report.source_sha256[$path] = & $hash $path
}
$reportPath = Join-Path $root 'port/windows-foundation/reports/runtime-world-item-adapter-v1.json'
$report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $reportPath -Encoding utf8
$resultText
