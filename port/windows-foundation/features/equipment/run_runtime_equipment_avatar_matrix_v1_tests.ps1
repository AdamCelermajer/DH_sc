[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
}
if (-not (Test-Path -LiteralPath $Compiler)) { throw "Required LLVM-MinGW compiler not found: $Compiler" }
$build = Join-Path $repo '.local-inputs/windows-foundation-build/runtime-equipment-avatar-matrix-v1'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$assetOverlay = Join-Path $build 'assets'
if (-not (Test-Path -LiteralPath (Join-Path $assetOverlay 'original-cache/data/pydata/ai_pyarray.bin'))) {
    New-Item -ItemType Directory -Force -Path $assetOverlay | Out-Null
    $sharedAssets = Join-Path $repo '.local-inputs/windows-shared-assets'
    Copy-Item -Path (Join-Path $sharedAssets '*') -Destination $assetOverlay -Recurse -Force
    $androidAssets = Join-Path $repo 'port/android-native/app/src/main/assets'
    foreach ($folder in @('animations','models','textures')) {
        $sourceFolder = Join-Path $androidAssets $folder
        if (Test-Path -LiteralPath $sourceFolder) {
            Copy-Item -Path (Join-Path $sourceFolder '*') -Destination (Join-Path $assetOverlay $folder) -Recurse -Force
        }
    }
}
$includes = @(
    'port/windows-foundation','port','port/game-data','port/scene-materials',
    'port/engine-skinning','port/engine-ui','port/engine-physics','port/level-world',
    'port/level-loader','port/physics-backend/box2d-2.0.1/Include'
) | ForEach-Object { '-I' + (Join-Path $repo $_) }
$sources = @(
    'port/windows-foundation/features/equipment/runtime_equipment_avatar_matrix_v1_tests.cpp',
    'port/windows-foundation/features/equipment/runtime_equipment_binding_v1.cpp',
    'port/windows-foundation/features/equipment/preview/runtime_equipment_preview_v1.cpp',
    'port/windows-foundation/features/equipment/preview/runtime_equipment_preview_frame_v1.cpp',
    'port/windows-foundation/features/equipment/runtime_equipment_page_v1.cpp',
    'port/windows-foundation/features/equipment/runtime_equipment_text_v1.cpp',
    'port/windows-foundation/features/inventory/source_item_descriptors.cpp',
    'port/windows-foundation/features/equipment/equipment_main_page.cpp',
    'port/windows-foundation/features/equipment/equipment_adapter.cpp',
    'port/windows-foundation/features/equipment/equipment_menu.cpp',
    'port/windows-foundation/features/equipment/original_equipment_art.cpp',
    'port/windows-foundation/features/equipment/source_equipment_appearance.cpp',
    'port/windows-foundation/features/equipment/source_equipment_render_bridge.cpp',
    'port/windows-foundation/features/equipment/source_equipment_material_binding.cpp',
    'port/windows-foundation/features/effects/effects_material_binding.cpp',
    'port/windows-foundation/asset_catalog.cpp',
    'port/windows-foundation/content_paths.cpp',
    'port/windows-foundation/save_store.cpp',
    'port/windows-foundation/texture_loader.cpp',
    'port/windows-foundation/source_material_pass.cpp',
    'port/windows-foundation/equipment_visual.cpp',
    'port/windows-foundation/original_character.cpp',
    'port/windows-foundation/original_actor_properties.cpp',
    'port/windows-foundation/features/character_menu/menu_stats.cpp',
    'port/engine-ui/character_menu_actions_owner_v1.cpp',
    'port/engine-ui/character_menu_stats_owner_v1.cpp',
    'port/engine-ui/character_menu_potion_text_v4.cpp',
    'port/windows-foundation/features/inventory/inventory_feature.cpp',
    'port/windows-foundation/features/inventory/inventory_menu.cpp',
    'port/windows-foundation/features/inventory/inventory_details.cpp',
    'port/windows-foundation/features/inventory/original_inventory_art.cpp',
    'port/game-data/player_equipment_v3.cpp',
    'port/level-world/player_equipment_render_owner_v1.cpp',
    'port/engine-skinning/visual_skin_owner_v6.cpp',
    'port/engine-skinning/visual_skin_selection_v6.cpp',
    'port/engine-skinning/skin_pose_cache_v32.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$archives = @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','libdh2_freetype237.a') |
    ForEach-Object { Join-Path $repo ('.local-inputs/windows-foundation-build/' + $_) }
$native = Join-Path $repo 'port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a'
if (-not (Test-Path -LiteralPath $native)) { throw "Current native source archive not found: $native" }
foreach ($archive in $archives) {
    if (-not (Test-Path -LiteralPath $archive)) { throw "Required native archive not found: $archive" }
}
$exe = Join-Path $build 'runtime_equipment_avatar_matrix_v1_tests.exe'
& $Compiler '-std=c++17' '-Wall' '-Wextra' '-Werror' '-Wno-missing-field-initializers' '-O1' '-static' `
    '-ffunction-sections' '-fdata-sections' '-Wl,--gc-sections' @includes @sources $native @archives $native '-o' $exe
if ($LASTEXITCODE -ne 0) { throw 'Strict LLVM-MinGW B019 avatar matrix build failed' }
$output = & $exe $repo
if ($LASTEXITCODE -ne 0) { throw 'Actual source-class same-Session equipment avatar matrix failed' }
$output | ForEach-Object { Write-Output $_ }
$reportPath = Join-Path $repo 'port/windows-foundation/features/equipment/runtime-equipment-avatar-matrix-v1-report.json'
$report = [ordered]@{
    validation = 'PASS_FEATURE_SAME_SESSION_CLASS_MATRIX'
    test = 'runtime_equipment_avatar_matrix_v1_tests.cpp'
    runner = 'run_runtime_equipment_avatar_matrix_v1_tests.ps1'
    native_output = ($output -join "`n")
    original_visual_evidence = 'Reuses B019-avatar-equipment-evidence-20261010.md: v1.0.3 reference at 8:30.5/8:31.0/8:32.5 and source RenderCharacterPane 0x452468 live-avatar path; sparse frames prove page composition, while same-Scene feature checks prove the tested runtime binding only.'
    matrix = @(
        @{ profile = 'KnightPlayerBase'; CharacterTable_row = 263; starter_items = 'StartingSuit(1079), StartingBoots(1073), StartingGloves(1076), Longsword01(664)' },
        @{ profile = 'RoguePlayerBase'; CharacterTable_row = 325; starter_items = 'StartingSuitRogue(1081), StartingBootsRogue(1075), StartingGlovesRogue(1078), Dagger01(370)' },
        @{ profile = 'MagePlayerBase'; CharacterTable_row = 290; starter_items = 'StartingSuitMage(1080), StartingBootsMage(1074), StartingGlovesMage(1077), Staff01(1025)' }
    )
    coverage = @(
        'exact CharacterTable row and ItemTable row identities checked against the original cache',
        'each profile has a distinct ActorId and its own CombatSession-owned CharacterVisual and retained Scene',
        'each class equips/unequips the exact source starter torso through RuntimeEquipmentBindingV1 and verifies source material refresh/restoration without advancing the source pose clock',
        'each class equips the exact source starter weapon and checks same-actor render receipt, source weapon geometry/material identity and exact instance identity',
        'SaveStore save/load preserves class, starter inventory rows and equipped stable instance, then a fresh feature binding rebinds to the same Session and source avatar before unequip restoration'
    )
    limitations = @(
        'Feature/runtime matrix only; no frozen normal executable visual capture or production Renderer viewport acceptance.',
        'SaveStore CharacterState roundtrip is covered. GameSave/native V60 save-load is outside this compatibility-session fixture.',
        'The original three sampled v1.0.3 frames prove avatar-pane composition but not an equip transaction animation; transaction acceptance here is source-backed and same-Session focused, not original-frame parity.'
    )
}
$report | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $reportPath -Encoding utf8
