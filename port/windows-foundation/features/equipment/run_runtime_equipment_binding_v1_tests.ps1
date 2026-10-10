[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
}
if (-not (Test-Path -LiteralPath $Compiler)) { throw "Required LLVM-MinGW compiler not found: $Compiler" }
$build = Join-Path $repo '.local-inputs/windows-foundation-build/runtime-equipment-binding-v1'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$includes = @(
    'port/windows-foundation','port','port/game-data','port/scene-materials',
    'port/engine-skinning','port/engine-ui','port/engine-physics','port/level-world',
    'port/level-loader','port/physics-backend/box2d-2.0.1/Include'
) | ForEach-Object { '-I' + (Join-Path $repo $_) }
$sources = @(
    'port/windows-foundation/features/equipment/runtime_equipment_binding_v1_tests.cpp',
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
$exe = Join-Path $build 'runtime_equipment_binding_v1_tests.exe'
& $Compiler '-std=c++17' '-Wall' '-Wextra' '-Werror' '-Wno-missing-field-initializers' '-O1' '-static' `
    '-ffunction-sections' '-fdata-sections' '-Wl,--gc-sections' @includes @sources $native @archives $native '-o' $exe
if ($LASTEXITCODE -ne 0) { throw 'Strict LLVM-MinGW runtime equipment binding build failed' }
$runtimeOutput = & $exe $repo
if ($LASTEXITCODE -ne 0) { throw 'Actual CombatSession/source-table runtime equipment binding test failed' }
$runtimeOutput | ForEach-Object { Write-Output $_ }
$reportPath = Join-Path $repo 'port/windows-foundation/reports/runtime-equipment-page-composition-v1.json'
$report = [ordered]@{
    validation = 'PASS'
    test = 'runtime_equipment_binding_v1_tests.cpp'
    runner = 'run_runtime_equipment_binding_v1_tests.ps1'
    native_output = ($runtimeOutput -join "`n")
    coverage = @(
        'actual CombatSession player, source ItemTable and same CharacterState',
        'SourceCompositionV1 register_page, install_content, readiness-gated select, composed content and release',
        'typed equip/unequip render receipts preserve same visual, Scene and attachment identity',
        'SourceComposition request_drop/request_auto_equip/request_transmute preserve selected source instance and slot as one-shot typed commands',
        'selected request_auto_equip consumes the existing dh2_equipment_auto_v3 EquipmentAdapter kernel and returns same-Scene render receipt',
        'SaveStore profile save/reload after Auto-equip preserves the exact item instance ID and rebinds it to the same live preview owner',
        'composition provider retains RuntimeEquipmentPageV1 until teardown and releases it afterward',
        'selected bare-item ValueBox amount packet uses actual ItemTable value words, same-player resolved property197, actual design_pycst TransmuteMultiplier, and original HudText integer formatter; stale and unsupported generated instances reject',
        'original ItemTable/HudText bare-definition name/details, empty-slot symbols, and potion quantity formatter',
        'unsupported powered/generated definitions fail explicitly instead of receiving fabricated descriptors'
    )
}
$report | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $reportPath -Encoding utf8
$previewReportPath = Join-Path $repo 'port/windows-foundation/features/equipment/preview/runtime_equipment_preview_v1_report.json'
$previewReport = [ordered]@{
    status = 'PASS_LINKED_SOURCE_SESSION_PREVIEW'
    test = 'runtime_equipment_binding_v1_tests.cpp'
    runner = 'run_runtime_equipment_binding_v1_tests.ps1'
    native_output = ($runtimeOutput -join "`n")
    source_camera = @{
        authored_viewport_480x320 = @(155.05, 87.70, 321.10, 282.75)
        eye = @(0, -800, 200)
        target = @(0, 0, 200)
        up = @(0, 0, 1)
        fov_raw_bits = '0x3f0efb1a'
        fov_degrees = 32.00078
        near = 10
        far = 1000
        aspect = 0.85132017
        source_root = 'CharacterVisual::source_motion_root_name(false) resolved in the same retained Scene::graph'
        rebase = 'Exact RenderCharacterPane rule: preserve source root Euler X/Y, set source literal Z=-0.5 radians, preserve scale and remove old source root transform'
    }
    coverage = @(
        'actual CombatSession player, exact retained CharacterVisual and same Scene source_skin owner',
        'synchronous preview borrow checks the weak actor lease before Session access and rejects an expired destroyed Session without invoking the callback',
        'source_skin draw_views (including actual weapon rows) and the current EquipmentAttachmentSet are borrowed with actor/class/revision and consumed synchronously',
        'actual source ItemTable StartingSuit equip changes source modular body draw state and unequip restores the exact original module set',
        'injected second source Debug query failure preserves committed gear plus the first modular setter prefix, stops exactly at the reached callback, and later source refresh restores naked modules',
    'rebinding the same Session with its equipped sword recreates source weapon draw views and the attachment receipt without advancing the pose clock',
    'profile SaveStore round trip preserves the exact auto-equipped inventory instance ID; rebinding the reloaded CharacterState recreates its same-Session weapon view',
    'source appearance plan setter prefix uses per-category Debug Load readiness followed by the real query adapter; native SourceRootScopes query bundles genuine Debug.Load/GetSwitch',
    'with_preview_packets uses the same pinned source body BRES and SkinOwner as modular refresh, then produces exact source geometry/material/pose packets via SourceEquipmentRenderBridgeV1 and original COMMON material bindings',
    'packet provenance is checked against every current draw-view primitive, including real weapon BRES leases; source packet world transforms remain unchanged for the main renderer to apply the authored preview rebase',
        'preview refresh and successful equip/unequip preserve exact same Scene and do not advance CharacterVisual.animation_elapsed_seconds',
        'render receipts increment revisions for equipment and attachment pose changes'
    )
    debug_service_contract = 'RuntimeEquipmentOptionsV1::source_appearance_debug receives the main-owned retained SourceRootScopes adapter. load checks owner readiness; query calls the existing combined debug_switch(name,bool,error), which performs the genuine Debug.Load/GetSwitch in order. It does not claim a separate native Load API.'
    test_debug_service = 'The linked source table/session test uses a deterministic callback seam to verify source order and prefix delivery; production must inject the actual combined SourceRootScopes Debug adapter described above.'
    packet_material_evidence = 'The linked packet test passes SourceEquipmentOriginalBindingsV1 callbacks, which reach OriginalEffectMaterialBinding using the exact body/weapon BRES and selected COMMON source pass while decoding real texture pixels. Its upload callback assigns CPU-only test texture handles; this test makes no live GL/pixel claim.'
    existing_wgl_evidence = 'The separate actual-source SourceEquipmentRenderBridgeV1 WGL smoke is recorded in port/windows-foundation/reports/feature-equipment.json: source_equipment_renderer_smoke reports four body packets, one weapon packet, two original decoded/uploaded textures, 1288 changed pixels, GL_NO_ERROR, and unchanged source Scene/clock. That is bridge/Renderer smoke evidence, not proof of this main Equipment page callsite.'
    limitation = 'The current main.cpp contains the Details Auto-equip receipt dispatch and calls with_preview_packets in the Equipment pane, applies source_inventory_rebase, and submits through Renderer.withViewport. These tests exercise the real CombatSession/item providers and isolated renderer; they do not establish a frozen normal-executable capture, cross-class visual matrix, or production-window pixel result. Source weapon draw rows are included in packets and must not also be drawn from attachments.'
}
$previewReport | ConvertTo-Json -Depth 7 | Set-Content -LiteralPath $previewReportPath -Encoding utf8
