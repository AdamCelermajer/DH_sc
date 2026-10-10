param([string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath($RepositoryRoot)
$compiler = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$build = Join-Path $root '.local-inputs/profile-persistence-matrix-v1'
New-Item -ItemType Directory -Force -Path $build | Out-Null
if (-not (Test-Path $compiler)) { throw "Pinned native compiler is missing: $compiler" }
$sources = @(
    'port/windows-foundation/features/campaign_save/profile_persistence_matrix_v1_tests.cpp',
    'port/windows-foundation/save_store.cpp',
    'port/windows-foundation/game_save.cpp',
    'port/windows-foundation/world.cpp',
    'port/windows-foundation/playable_actor_world.cpp',
    'port/windows-foundation/original_combat_properties.cpp',
    'port/windows-foundation/original_actor_properties.cpp'
) | ForEach-Object { Join-Path $root $_ }
$libraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a',
    'librecovered_trigger_contacts.a','librecovered_content.a',
    'physics-backend/libdh2_box2d_201.a') | ForEach-Object { Join-Path $root ('.local-inputs/windows-foundation-build/' + $_) }
foreach ($library in $libraries) { if (-not (Test-Path $library)) { throw "Required stable archive is missing: $library" } }
$exe = Join-Path $build 'profile_persistence_matrix_v1_tests.exe'
& $compiler -std=c++17 -O2 -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    -I(Join-Path $root 'port/windows-foundation') `
    -I(Join-Path $root 'port/game-data') `
    @sources @libraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $exe
if ($LASTEXITCODE -ne 0) { throw "Profile persistence matrix compile failed ($LASTEXITCODE)" }
$assets = Join-Path $root '.local-inputs/windows-shared-assets'
$character = Join-Path $build 'profile.dhsave'
$game = Join-Path $build 'profile.game'
$write = & $exe write $assets $character $game
if ($LASTEXITCODE -ne 0) { throw "Persistence matrix write phase failed: $write" }
# A separate executable process decodes both files and exercises restore.
$verify = & $exe verify $assets $character $game
if ($LASTEXITCODE -ne 0) { throw "Persistence matrix fresh-process verify failed: $verify" }
$result = $verify | ConvertFrom-Json
if ($result.validation -ne 'PASS') { throw 'Profile persistence matrix did not pass' }
$hash = { param($path) (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $root $path)).Hash.ToLowerInvariant() }
$report = [ordered]@{
    validation = 'PASS'
    profile = 'RoguePlayerBase'
    source_profile = 'Original CharacterTable row; actual CharacterTable/ClassTables resolution supplies baseline HP/MP and FaeryList ID.'
    behavior = 'Separate write/verify process invocations exercise SaveStore schema3 and GameSave v1 file encode/decode. GameSave restore uses an exact compatible Rogue actor binding.'
    investigation = 'This is an internal persistence invariant with no direct visible action. Its visible consumer invariant is that character stats, currency, equipment, skill mapping, and Faery selection after restore match the saved state; no gameplay screenshot/video or GUI runtime verification is claimed.'
    fixture_values = 'Rogue HP/MP and maxima come from current Original CharacterTable/ClassTables. XP=123456, gold=7890, point counts, inventory/gear instance labels, rank values, slots, and selected Faery slot/state/level are explicit codec test specimens; they are not asserted as native starter/reward values. The Rogue FaeryList ID alone is source-row-derived.'
    matrix = $result
    persisted_fields = @('HP/MP and maxima','integer XP and gold','inventory instance/definition/quantity','gear slot/item instance/equipment set/source slot','skill IDs/ranks and saved-row hotbar slots','source-known flags','FaeryList/current selection/per-slot state and level')
    legacy = 'Handcrafted SaveStore schema1 decode confirms omitted endurance/energy, points, skill slots and Faery metadata remain unknown/default; no values are inferred. Existing readers promote schema1 to current in-memory CharacterState and do not retain an original-version tag.'
    failure_atomicity = 'Truncated reads leave SaveStore CharacterState and GameSave output sentinels unchanged. Invalid writes leave existing file bytes unchanged. Rejected GameSave level route leaves CharacterState, actor and RNG unchanged.'
    source_evidence = [ordered]@{
        save_store = 'port/windows-foundation/save_store.cpp encode writes DH SAVE format1 + CharacterState schema; schema2 adds source points/skill slots/Faery, schema3 adds CQPG. decode builds a local candidate and moves it into caller output only after validation; save writes/flushed temporary then atomically replaces the destination.'
        game_save = 'port/windows-foundation/game_save.cpp snapshot includes CharacterState, level URI, controlled ActorId, caller RNG, actors and four property sheets; v2 adds objects/components, v3 adds optional physical presence. load_game uses a detached candidate and validates before assignment. restore_game_save checks exact level/roster/actor identity/equipment/binding before world replace, then assigns CharacterState.'
        original_profile = 'OriginalCharacterTable/ClassTables load through load_original_property_tables; RoguePlayerBase is resolved by build_original_combat_properties and its resolved properties36/38/41/43 supply test profile HP/MP and maxima. CharacterTable FaeryList is read by name and retained as an opaque source list ID.'
        skills = 'Existing Rogue source skill tests identify SkillList position0 as JumpKick and source saved slots index CharacterState::skills rows. This codec matrix uses explicit rank values as serialization specimens and does not claim rank2 is a native starter rank. Original SkillList/native skill unlock queries remain a caller-side producer concern.'
        faery = 'Faery list ID is source row-derived; the test sets a known current slot and state/level values to verify exact persistence. It does not claim a native starter unlock or resolve localized Faery content.'
    }
    integration_gates = @(
        'The production caller must save the exact current canonical CharacterState and, for live checkpoints, capture the same PlayableActorWorld and level URI; character-only SaveStore load does not rebuild actors, combat bindings or gear source records.',
        'GameSave restore only admits the same level, actor IDs/definitions, class/faction/persistent identity, attacks, equipped bindings, combat facts and world-object visuals; changed equipment/profile needs a future source-aware reconstruction path.',
        'The test proves codec and restore component behavior only. It does not prove native PlayerSavegame/GS save ownership, filesystem profile selection, source native gear-instance reconstruction, or GUI integration.'
    )
    source_sha256 = [ordered]@{}
    original_binary_sha256 = & $hash '.local-inputs/libDungeonHunter2.so'
    shared_source_assets = & $hash '.local-inputs/windows-shared-assets/original-cache/data/pydata/character_templates_pyarray.bin'
}
foreach ($path in @(
    'port/windows-foundation/features/campaign_save/profile_persistence_matrix_v1_tests.cpp',
    'port/windows-foundation/save_store.cpp','port/windows-foundation/save_store.hpp',
    'port/windows-foundation/game_save.cpp','port/windows-foundation/game_save.hpp',
    'port/windows-foundation/character_state.cpp','port/windows-foundation/character_state.hpp',
    'port/windows-foundation/original_combat_properties.cpp','port/windows-foundation/playable_actor_world.cpp',
    'port/windows-foundation/reports/runtime-skill-animation-bank-v1.json'
)) { $report.source_sha256[$path] = & $hash $path }
$reportPath = Join-Path $root 'port/windows-foundation/reports/profile-persistence-matrix-v1.json'
$report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $reportPath -Encoding utf8
Write-Output $write
Write-Output $verify
Write-Output "Report: $reportPath"
