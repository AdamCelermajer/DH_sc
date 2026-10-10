param([string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath($RepositoryRoot)
$compiler = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $root '.local-inputs/runtime_death_rewards_v1_tests.exe'
$sources = @(
    'port/windows-foundation/features/loot/runtime_death_rewards_v1_tests.cpp',
    'port/windows-foundation/features/loot/runtime_death_rewards_v1.cpp',
    'port/windows-foundation/features/loot/runtime_world_item_adapter_v1.cpp',
    'port/windows-foundation/save_store.cpp',
    'port/windows-foundation/features/inventory/inventory_feature.cpp',
    'port/level-world/player_progression_v1.cpp',
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
if ($LASTEXITCODE -ne 0) { throw "Native source death reward compile failed ($LASTEXITCODE)" }
$assetRoot = Join-Path $root '.local-inputs/windows-shared-assets'
$powerRoot = Join-Path $root '.local-inputs/player-loot-v7/cache'
$designRoot = Join-Path $root '.local-inputs/design-settings'
$resultText = & $output $assetRoot $powerRoot $designRoot
if ($LASTEXITCODE -ne 0) { throw "Native source death reward test failed ($LASTEXITCODE): $resultText" }
$result = $resultText | ConvertFrom-Json
if ($result.validation -ne 'PASS') { throw 'Native source death reward audit did not pass' }
$hash = { param($path) (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $root $path)).Hash.ToLowerInvariant() }
$report = [ordered]@{
    validation = 'PASS'
    version = 1
    test = $result
    source_sha256 = [ordered]@{}
    original_binary_sha256 = & $hash '.local-inputs/libDungeonHunter2.so'
    actual_loot_cache_sha256 = & $hash '.local-inputs/player-loot-v7/cache/loot_table_pyarray.bin'
    actual_loot_names_sha256 = & $hash '.local-inputs/player-loot-v7/cache/loot_table_pyarraynames.bin'
    power_cache_sha256 = & $hash '.local-inputs/player-loot-v7/cache/num_prob_records_v7.bin'
    design_settings_cache_sha256 = & $hash '.local-inputs/design-settings/design_pyarray.bin'
    scope = 'Same-session generic death event delivery with actual Character property sheets, original loot/probability/item/power and DesignSettings snapshots, caller-owned LootRandom8, the existing XP formula kernels, canonical shared CharacterState, and the generic WorldItemStore pickup adapter. Gold value bonus is sourced from the actual same-world killer property195, or the source 0/0 baseline when the killer is absent/deleted. No native GS Level+150, Save, Gear, Item145 pool, or production host hook is claimed. Shared CharacterState stores whole-number XP; source property 33 retains its fixed-point exact value in the world property sheet, so current SaveStore projection does not preserve fractional XP remainder.'
    source_evidence = [ordered]@{
        library_sha256 = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
        get_loot = 'Character::GetLoot 0x3a2fcc returns Character+0x101c / resolved property word 9.'
        drop_loot = 'Character::DropLoot 0x3a5ae4 passes the getter result, dying Character and attacker into ItemObject::DropLootTable 0x3ecba0; the original Loot/Item selection kernels implement the authored probability/subloot rules.'
        xp = 'Character::DistributeXP 0x3bf828 uses victim XP property 35 and level property 19; player distribution and fixed-point XP award reuse progression_scaled_xp_v1, progression_award_raw_v1 and progression_modified_xp_v1. Source LevelUp increments property 19 by 256, resets 33, restores the CharacterTable row and recalculates with ClassTables.'
        xp_vitals = 'Ordinary progression_give_xp_v1 adds only resolved property33 and reaches LevelUp only when the new XP reaches property34 (port/level-world/player_progression_v1.cpp). Before progression, the feature now adds the difference between live ActorState HP/MP and candidate source resolved properties36/41 using loaded PropertyRules/dh2_property_add, after validating finite nonnegative Q8 range and exact source maximum matches at38/43. Native Character::_SetLevel 0x3b73a4 recalculates and explicitly calls RegenHP(-1)/RegenMP(-1), as recorded in port/level-world/reference/character-level/NOTES.md; the actual threshold-crossing test confirms that source refill remains authoritative.'
        xp_vitals_test = 'Same-world RoguePlayerBase tests first damage the live ActorState to81 HP and spend MP to12.25 while the source sheet and canonical CharacterState retain their initial vitals; actual Swamp_LizadMan_Type1 XP then publishes exact Q8-matched sheet36/41, ActorState and CharacterState. A second ordinary XP award repeats the check at70 HP/5.25 MP. An actual authored property34 crossing confirms the source level-up refills HP/MP to new resolved maxima and mirrors the published vitals. Strict SaveStore encode/decode retains the resulting level, XP, points and vitals.'
        gold_value_bonus = 'Character::DropLoot 0x3a5ae4 passes the actual killer into ItemObject::DropLootTable 0x3ecba0. CharacterLootDropV8 re-resolves that same killer and obtains LootCreationV8 value_bonus256 from resolved property195 (and power_bonus256 from word196). RuntimeDeathRewards mirrors word195 from `PlayableActorWorld::combat_properties(killer).sheets.resolved[195]`; an invalid/deleted killer follows the recovered CharacterLootDropV8 0/0 initialization baseline. If a killer ActorState exists but its source properties are unavailable, the outcome carries `killer_properties_unavailable_for_gold_value`; type13 consumes its original ItemTable range draw but no value is fabricated or published. Property196 is not consumed by this generic path because it does not synthesize ItemInstance powers; original LootPowerCreationV7 consumes it only when AddPowers is reached.'
        gold_test = 'The focused runtime test explicitly chooses the authored Barrel_Level_01 loot row on a bound actual Character property sheet to exercise source GoldStack selection. A bound killer sheet word195=12800 yields a source value of9 after the actual seed-selected drop; an independent original selector plus dh2_loot_item_value_v7 using the same seed, word195, and call order matches the published record and final RNG state. The world-store pickup credits9 and leaves LootRandom8 unchanged. A second real ActorState death with an invalid/deleted attacker verifies the recovered 0/0 baseline, source value6 and RNG-free pickup. The current PlayableActorWorld bind API installs ActorState and OriginalCombatProperties atomically, so the typed missing-properties diagnostic is fail-closed defensive coverage, not an independently constructible test fixture.'
        money = 'GoldStack type13 is valued at creation with original `dh2_loot_item_value_v7`, retaining its ItemInstance value on the world item. Original `_AddItemInstance` 0x3ff5d4 with convertGold=true reads ItemInstance+0x54, calls AddGold 0x3fe164 and destroys the token. The generic store carries the resolved value into an atomic, RNG-free CharacterState.gold pickup. The native gold notification/UI owner is not implemented here.'
        dedupe = 'Receipt key is ActorId plus caller-provided nonzero binding_lifecycle; source death dispatch marks attempted before selection/spawn, terminally retains partial failures, and permits a new lifetime only when the caller advances that token.'
    }
    limitations = @(
        'Production must pass the exact live CombatSession event batch, loaded source table/config snapshots, same Application LootRandom8V2, and the caller-provided binding lifecycle/shared CharacterState resolver.',
        'The world-item receiver remains explicit; this component does not build or publish a second item pool and is not yet enrolled by root main/session code.',
        'Original player roster order, remote-update/current-level policy, XP presentation/save tails, and source pickup gear conversion remain outside this generic headless component.'
        'The XP vitals projection uses the current PlayableActorWorld ActorState for ordinary non-level awards and the recalculated source sheet on level-up. The main/session caller must continue to pass the same live world whose ActorState receives combat damage; this feature adds no second vitals owner or main callback.'
    )
}
foreach ($path in @($sources + 'port/windows-foundation/features/loot/runtime_world_item_adapter_v1.hpp' + 'port/windows-foundation/features/loot/runtime_world_item_interaction_v1.hpp' + 'port/game-data/loot_tables_v2.hpp' + 'port/game-data/loot_tables_v2.cpp' + 'port/game-data/design_settings.hpp' + 'port/game-data/design_settings.cpp' + 'port/windows-foundation/reports/act1-loot-reward-integration.json')) {
    $report.source_sha256[$path] = & $hash $path
}
$reportPath = Join-Path $root 'port/windows-foundation/reports/runtime-death-rewards-v1.json'
$report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $reportPath -Encoding utf8
$resultText
