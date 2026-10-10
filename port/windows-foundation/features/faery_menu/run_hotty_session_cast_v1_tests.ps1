[CmdletBinding()]
param([string]$Compiler, [switch]$CelestFxOnly)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $root '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$build = Join-Path $root '.local-inputs\windows-hotty-session-test'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$foundation = Join-Path $root '.local-inputs\windows-foundation-build'
$output = Join-Path $build 'hotty-session-cast-tests.exe'
$testAssets = Join-Path $root '.local-inputs\hotty-session-assets-v1'
$sourceAssets = Join-Path $root '.local-inputs\windows-shared-assets'
$fxOverlay = Join-Path $root '.local-inputs\hotty-fx-overlay-v1\com.gameloft.android.GAND.GloftD2SS\files'
$archive = Join-Path $env:USERPROFILE 'Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip'
$cachePrefix = 'com.gameloft.android.GAND.GloftD2SS/files/'
$sourceUris = @(
    'data/3d/interface/spell_dh2_faery_fire.bdae',
    'data/3d/interface/skill_dh2_faery_fire.bdae',
    'data/3d/interface/spell_dh2_faery_lightning.bdae',
    'data/3d/interface/skill_dh2_faery_lightning.bdae',
    'data/3d/characters/prince/animations/prince_spell_pre_hotty.bdae',
    'data/3d/characters/prince/animations/prince_spell_hotty.bdae',
    'data/3d/characters/prince/animations/prince_spell_channeling.bdae'
)
if (-not (Test-Path -LiteralPath (Join-Path $testAssets 'original-cache/data/pydata'))) {
    New-Item -ItemType Directory -Force -Path $testAssets | Out-Null
    Get-ChildItem -LiteralPath $sourceAssets -File -Recurse | ForEach-Object {
        $relative = [IO.Path]::GetRelativePath($sourceAssets, $_.FullName)
        $destination = Join-Path $testAssets $relative
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $destination) | Out-Null
        New-Item -ItemType HardLink -Path $destination -Target $_.FullName | Out-Null
    }
}
Add-Type -AssemblyName System.IO.Compression.FileSystem
$archiveReader = [IO.Compression.ZipFile]::OpenRead($archive)
try {
    foreach ($uri in $sourceUris) {
        $entryName = $cachePrefix + $uri
        $entry = $archiveReader.GetEntry($entryName)
        if (-not $entry) { throw "Missing canonical source cache entry $entryName" }
        $destinations = @(
            (Join-Path $testAssets ($uri -replace '/', '\')),
            (Join-Path $fxOverlay ($uri -replace '/', '\'))
        )
        foreach ($destination in $destinations) {
            New-Item -ItemType Directory -Force -Path (Split-Path -Parent $destination) | Out-Null
            $sourceStream = $entry.Open()
            try {
                $targetStream = [IO.File]::Create($destination)
                try { $sourceStream.CopyTo($targetStream) } finally { $targetStream.Dispose() }
            } finally { $sourceStream.Dispose() }
        }
    }
} finally { $archiveReader.Dispose() }
$expected = @{
    'data\3d\interface\spell_dh2_faery_fire.bdae' = 'E000AE3169DB48BF7F726C945EFC989F69FC152CB26827FA588ECA64B8F88135'
    'data\3d\interface\skill_dh2_faery_fire.bdae' = '91FB8FA21F71ACB38279FDB03A441115D9DB7CDF26A425F2B08E60C80422D66A'
    'data\3d\interface\spell_dh2_faery_lightning.bdae' = '24DE71141C46541F930841055880000446EB77A0C100DC09CABAD92550012B3E'
    'data\3d\interface\skill_dh2_faery_lightning.bdae' = '441444EC34AC2D0A6A06917B4FF27E26691C8466B7DE29D4DF369274D9A968A0'
    'data\3d\characters\prince\animations\prince_spell_pre_hotty.bdae' = '20F24C13DA52EECCD65E5C1D9E1622EAE02EB17C8ADD83893C7470DD2E5CE59D'
    'data\3d\characters\prince\animations\prince_spell_hotty.bdae' = 'E80741DB58A592071F0FE0F4683A2FB4CEC3D5E6C0872A32251EF7266D742FDA'
    'data\3d\characters\prince\animations\prince_spell_channeling.bdae' = '1350178E7A87B8868FB6E4D34ACFCE79E27A77674058AA09DDD8625BB6F596EC'
}
foreach ($relative in $expected.Keys) {
    $staged = Join-Path $testAssets $relative
    if ((Get-FileHash -Algorithm SHA256 -LiteralPath $staged).Hash -ne $expected[$relative]) {
        throw "Private Hotty original resource hash mismatch: $relative"
    }
}
$hottySpell = Join-Path $fxOverlay 'data\3d\interface\spell_dh2_faery_fire.bdae'
$hottyPlayerPre = Join-Path $fxOverlay 'data\3d\interface\skill_dh2_faery_fire.bdae'
if ((Get-FileHash -Algorithm SHA256 -LiteralPath $hottySpell).Hash -ne
    'E000AE3169DB48BF7F726C945EFC989F69FC152CB26827FA588ECA64B8F88135') {
    throw 'Staged original Hotty target BDAE hash differs from the archive receipt'
}
if ((Get-FileHash -Algorithm SHA256 -LiteralPath $hottyPlayerPre).Hash -ne
    '91FB8FA21F71ACB38279FDB03A441115D9DB7CDF26A425F2B08E60C80422D66A') {
    throw 'Staged original Hotty Player_Pre BDAE hash differs from the archive receipt'
}
$sources = @(
    (Join-Path $PSScriptRoot 'tests\hotty_session_cast_v1_tests.cpp'),
    (Join-Path $PSScriptRoot 'hotty_cast_v1.cpp'),
    (Join-Path $PSScriptRoot 'hotty_cast_session_v1.cpp'),
    (Join-Path $PSScriptRoot 'hotty_character_cast_v1.cpp'),
    (Join-Path $PSScriptRoot 'hotty_source_use_v1.cpp'),
    (Join-Path $PSScriptRoot 'celest_cast_v1.cpp'),
    (Join-Path $PSScriptRoot 'celest_source_use_v1.cpp'),
    (Join-Path $PSScriptRoot 'session_faery_page_v1.cpp'),
    (Join-Path $PSScriptRoot 'character_state_page_v1.cpp'),
    (Join-Path $PSScriptRoot 'original_art.cpp'),
    (Join-Path $PSScriptRoot 'source_text_v1.cpp'),
    (Join-Path $PSScriptRoot 'hotty_effects_v1.cpp'),
    (Join-Path $PSScriptRoot 'hotty_effects_tables_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\effects\runtime_effects_factory_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\effects\celest_target_fx_dispatch_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\effects\runtime_combat_effects_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\effects\effects_executor.cpp'),
    (Join-Path $root 'port\windows-foundation\features\effects\effects_render_bridge.cpp'),
    (Join-Path $root 'port\windows-foundation\features\effects\effects_material_binding.cpp'),
    (Join-Path $root 'port\level-world\character_mesh_fx_owner_v4.cpp'),
    (Join-Path $root 'port\level-world\character_authored_resource_v32.cpp'),
    (Join-Path $root 'port\level-world\character_authored_fx_forces_v4.cpp'),
    (Join-Path $root 'port\level-world\character_fx_floor_query_v3.cpp'),
    (Join-Path $root 'port\level-world\character_animation_step_fx_v2.cpp'),
    (Join-Path $root 'port\level-world\authored_fx_mesh_graph_v32.cpp'),
    (Join-Path $root 'port\level-world\authored_fx_nonrender_geometry_v32.cpp'),
    (Join-Path $root 'port\level-world\authored_fx_transform_v5.cpp'),
    (Join-Path $root 'port\scene-materials\particle_scene_v1.cpp'),
    (Join-Path $root 'port\engine-skinning\skinning.cpp'),
    (Join-Path $root 'port\engine-animation\animation.cpp'),
    (Join-Path $root 'port\engine-animation\component_applicator.cpp'),
    (Join-Path $root 'port\game-data\faery_tables.cpp'),
    (Join-Path $root 'port\game-data\effects_tables.cpp'),
    (Join-Path $root 'port\game-data\animation_tables.cpp'),
    (Join-Path $root 'port\level-world\character_animation_ai.cpp'),
    (Join-Path $root 'port\windows-foundation\features\generic_skills\runtime_skill_activation_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\generic_skills\runtime_skill_mana_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\generic_skills\runtime_skill_cast_prepare_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\generic_skills\runtime_skill_cast_coordinator_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\generic_skills\pc_cooldown_frame_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\generic_skills\runtime_skill_animation_bank_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\generic_skills\runtime_skill_target_query_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\generic_skills\runtime_skill_progression_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\generic_skills\generic_skills_page_v1.cpp'),
    (Join-Path $root 'port\windows-foundation\features\skills_animation\skill_animation_program.cpp'),
    (Join-Path $root 'port\windows-foundation\combat_session.cpp'),
    (Join-Path $root 'port\windows-foundation\playable_actor_world.cpp')
)
$sources += @(
    'particle_scalar_animation_v6', 'particle_cloud_runtime_v1',
    'particle_cloud_runtime_v3', 'particle_deflector_v1',
    'particle_force_scene_v2', 'particle_bound_forces_v4',
    'particle_resource_init_v2', 'particle_cloud_models_v1',
    'particle_billboard_v1', 'particle_force_scene_v1',
    'particle_scene_color_v1', 'particle_factory', 'particle_random_v1',
    'particle_emission', 'particle_box_v2', 'material_color',
    'material_color_v3', 'particle_parameter', 'particle_resource_init_v32',
    'particle_billboard_v32'
) | ForEach-Object { Join-Path $root ("port\engine-animation\$_.cpp") }
$sources += @(
    'asset_catalog', 'content_paths', 'texture_loader', 'source_material_pass',
    'actor_lighting'
) | ForEach-Object { Join-Path $root ("port\windows-foundation\$_.cpp") }
$sources += @(
    'textures', 'pvrtc'
) | ForEach-Object { Join-Path $root ("port\engine-textures\$_.cpp") }
$sources += @(
    'scene', 'effect_render_pass_v4'
) | ForEach-Object { Join-Path $root ("port\scene-materials\$_.cpp") }
$sources += @(
    'tinyxml', 'tinyxmlerror', 'tinyxmlparser', 'tinystr'
) | ForEach-Object { Join-Path $root ("port\level-loader\vendor\tinyxml\$_.cpp") }
$libraries = @(
    (Join-Path $root 'port\windows-foundation\features\actor_frame\source_character_owner_factory_native_build\native.a'),
    (Join-Path $foundation 'libfoundation_data.a'),
    (Join-Path $foundation 'libcontent_xml.a'),
    (Join-Path $foundation 'libdh2_freetype237.a'),
    (Join-Path $foundation 'librecovered_trigger_contacts.a'),
    (Join-Path $foundation 'librecovered_content.a'),
    (Join-Path $foundation 'physics-backend\libdh2_box2d_201.a')
)
$includes = @(
    (Join-Path $root 'port\windows-foundation'),
    (Join-Path $root 'port\level-world'),
    (Join-Path $root 'port\game-data'),
    (Join-Path $root 'port\engine-animation'),
    (Join-Path $root 'port\scene-materials'),
    (Join-Path $root 'port\engine-skinning'),
    (Join-Path $root 'port\engine-resources'),
    (Join-Path $root 'port\engine-math'),
    (Join-Path $root 'port\engine-textures'),
    (Join-Path $root 'port\asset-payloads'),
    (Join-Path $root 'port\physics-backend'),
    (Join-Path $root 'port\level-loader\vendor\tinyxml')
) | ForEach-Object { '-I' + $_ }
$includes += @('-isystem', (Join-Path $root 'port\physics-backend\box2d-2.0.1\Include'))
& $Compiler -std=c++17 -O1 -g -static -Wall -Wextra -Werror `
    -Dfinite=_finite -Wno-missing-field-initializers -Wno-misleading-indentation -Wno-unused-function -Wno-unused-value @includes @sources `
    '-Wl,--start-group' @libraries '-Wl,--end-group' `
    -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 `
    -luuid -lcomdlg32 -ladvapi32 -o $output
if ($LASTEXITCODE -ne 0) { throw 'Connected Hotty session test did not compile' }
$testArguments = @(
    $testAssets,
    (Join-Path $root '.local-inputs\character-skill-session-v2\cache\data\pydata'),
    (Join-Path $root '.local-inputs\loot-fx-v32-assets'),
    $fxOverlay
)
if ($CelestFxOnly) { $testArguments += '--celest-fx-only' }
& $output @testArguments
if ($LASTEXITCODE -ne 0) { throw 'Connected Hotty session test failed' }
