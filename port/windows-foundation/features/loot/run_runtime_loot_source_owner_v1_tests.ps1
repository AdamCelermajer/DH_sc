param([string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath($RepositoryRoot)
$compiler = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $root '.local-inputs/runtime_loot_source_owner_v1_tests.exe'
$sources = @(
    'port/windows-foundation/features/loot/runtime_loot_source_owner_v1_tests.cpp',
    'port/windows-foundation/features/loot/runtime_loot_source_owner_v1.cpp',
    'port/game-data/item_power_tables_v5.cpp',
    'port/game-data/loot_power_resources_v7.cpp',
    'port/game-data/loot_audiovisual_v8.cpp'
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
$arguments += @('-static','-lfoundation_frontend','-lfoundation_data','-lcontent_xml','-ldh2_freetype237',
    '-lrecovered_trigger_contacts','-lrecovered_content','-ldh2_box2d_201',
    '-lkernel32','-luser32','-lgdi32','-lwinspool','-lshell32','-lole32',
    '-loleaut32','-luuid','-lcomdlg32','-ladvapi32','-o',$output)
& $compiler @arguments
if ($LASTEXITCODE -ne 0) { throw "Runtime loot source owner compile failed ($LASTEXITCODE)" }
$assetRoot = Join-Path $root '.local-inputs/windows-source-clock-v19-preview-9/assets'
$resultText = & $output $assetRoot
if ($LASTEXITCODE -ne 0) { throw "Runtime loot source owner test failed ($LASTEXITCODE): $resultText" }
$result = $resultText | ConvertFrom-Json
if ($result.validation -ne 'PASS') { throw 'Runtime loot source owner audit did not pass' }
$hash = { param($path) (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $root $path)).Hash.ToLowerInvariant() }
$assetHash = { param($path) (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $assetRoot $path)).Hash.ToLowerInvariant() }
$report = [ordered]@{
    validation = 'PASS'
    version = 1
    test = $result
    source_sha256 = [ordered]@{}
    actual_assets_sha256 = [ordered]@{}
    api = 'RuntimeLootSourceOwnerV1::load(AssetCatalog, exact RuntimeCreationSourceOwnerV1, error) yields a borrow retaining the same OriginalPropertyDatabase and six-table LootTablesV2 snapshot, while pinning original ItemPowerTablesV5, LootPowerResourcesV7 quantities from bounded LootTables suffix after loot.consumed(), LootAudioVisualV8, DesignSettingsOwner and CharacterDesign MaxLevelBNormal/CHard/DVeryHard caps.'
    source_semantics = 'The V88 suffix reader is bounded to 8 MiB resources, 65536 counts/strings, exact record end, the actual merchant rows/schema/names, and the original NumProbArray payload. No fixture sidecar, native App/GameDesign graph, localization, private RNG or second LootTables owner is created.'
    player_class_query = 'source_player_class_count_v1 counts same-world PlayableActorWorld actors with traits.is_player, resolves ActorState.definition_id to the pinned OriginalPropertyDatabase CharacterTable row, and compares that actual row index with the recovered source PlayerManager IDs 263/290/325. Original 0x36ea50 compares Character::InitPre cached base-id +0x13c8; 0x36eab8/0x36eac0/0x36eac8 select Warrior/Rogue/Mage IDs. ActorState.class_id and ClassTables selectable names are deliberately not substituted, so KnightPlayerClass and KnightPlayerBase remain distinct namespaces.'
    lifecycle = 'The output is transactionally replaced only after all source bytes, table owners, caps and 25 hash receipts validate. Borrowed snapshots pin the decoded table owners. Caller-owned LootRandom8 remains untouched. Consumers must retain the RuntimeLootSourceOwnerV1 for each use of the borrowed prefix.'
    limitations = @('This data owner does not construct or publish ItemInstance, Loot outcome, native Application, GameDesign, localization or an RNG stream.','DesignSettings is pinned as the complete decoded first source row table; callers select rows according to their own source context.','No global startup/main/CMake hookup is claimed; this is a feature-owned source provider with a linked source-asset test.')
}
foreach ($path in $sources + 'port/windows-foundation/features/loot/runtime_loot_source_owner_v1.hpp' + 'port/windows-foundation/features/loot/run_runtime_loot_source_owner_v1_tests.ps1') {
    $report.source_sha256[$path] = & $hash $path
}
$assetPaths = @(
    'original-cache/data/pydata/item_powers_pyarray.bin',
    'original-cache/data/pydata/item_powers_pyarraynames.bin',
    'original-cache/data/pydata/item_powers_pystructnames.bin',
    'original-cache/data/pydata/item_powers_monopoly_pyarray.bin',
    'original-cache/data/pydata/item_powers_monopoly_pyarraynames.bin',
    'original-cache/data/pydata/item_powers_monopoly_pystructnames.bin',
    'data/loot_audiovisual_pyarray.bin',
    'data/loot_audiovisual_pyarraynames.bin',
    'data/loot_audiovisual_pystructnames.bin',
    'original-cache/data/pydata/design_pyarray.bin',
    'original-cache/data/pydata/design_pyarraynames.bin',
    'original-cache/data/pydata/design_pystructnames.bin',
    'original-cache/data/pydata/loot_table_pyarray.bin',
    'original-cache/data/pydata/loot_table_pyarraynames.bin',
    'original-cache/data/pydata/loot_table_pystructnames.bin',
    'original-cache/data/pydata/design_pycst.bin'
)
foreach ($path in $assetPaths) {
    if (Test-Path -LiteralPath (Join-Path $assetRoot $path)) { $report.actual_assets_sha256[$path] = & $assetHash $path }
}
$referencePaths = @(
    'port/level-world/reference/player-manager-owner-v1/original-source.asm',
    'port/level-world/reference/player-manager-owner-v1/original-functions.json',
    'port/level-world/player_manager_loot_queries_v8.hpp',
    'port/level-world/player_manager_loot_queries_v8.cpp'
)
$report.reference_sha256 = [ordered]@{}
foreach ($path in $referencePaths) { $report.reference_sha256[$path] = & $hash $path }
$reportPath = Join-Path $root 'port/windows-foundation/reports/runtime-loot-source-owner-v1.json'
$report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $reportPath -Encoding utf8
$resultText
