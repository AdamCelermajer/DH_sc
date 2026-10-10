[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$cache = Join-Path $repoRoot '.local-inputs\windows-main-frontend-v1\assets\original-cache\data'
$faery = Join-Path $repoRoot '.local-inputs\character-skill-session-v2\cache\data\pydata'
$hotty = Join-Path $repoRoot '.local-inputs\character-skill-session-v2\cache\data\scripts\skills\faerie_hotty.luac'
$effects = Join-Path $repoRoot '.local-inputs\loot-fx-v32-assets'
$effectHashes = @{
    'effects_pyarray.bin' = '9A743E51E4BA098A63FD274EE890EFEAF0DC6F750D51FC53FD9EF2C01695F71B'
    'effects_pyarraynames.bin' = 'F91B3EF914E6840C4339C1AF32446C5F3D24605AAA4A9403A797CC941EDBC043'
    'effects_pystructnames.bin' = 'D4296263D954714D560083E058DA3FF4D87D4B35FA7C796C4E88618F4CB2CC2B'
    'effects_dictionary_pyarray.bin' = 'E0162462ED31315A637B3F784E6957949054855357CF09431E5249DB5C42D294'
    'effects_dictionary_pyarraynames.bin' = '8227757B4E18E10F898E0693EEB1B728B024E8B4675162F97C5FBB6647B910F2'
}
foreach ($name in $effectHashes.Keys) {
    $path = Join-Path $effects $name
    if (-not (Test-Path -LiteralPath $path)) { throw "Missing source EffectsTables input: $name" }
    if ((Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash -ne $effectHashes[$name]) {
        throw "Source EffectsTables hash changed: $name"
    }
}
$exe = Join-Path $PSScriptRoot 'hotty_cast_v1_tests.exe'
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $Compiler -Parent) + ';' + $env:PATH
    $sources = @(
        (Join-Path $PSScriptRoot 'hotty_cast_v1.cpp'),
        (Join-Path $PSScriptRoot 'hotty_effects_tables_v1.cpp'),
        (Join-Path $PSScriptRoot 'tests\hotty_cast_v1_tests.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\playable_actor_world.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\world.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\actor_state.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\asset_catalog.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\original_actor_properties.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\original_combat_properties.cpp'),
        (Join-Path $repoRoot 'port\game-data\data.cpp'),
        (Join-Path $repoRoot 'port\game-data\class_tables.cpp'),
        (Join-Path $repoRoot 'port\game-data\properties.cpp'),
        (Join-Path $repoRoot 'port\game-data\ai.cpp'),
        (Join-Path $repoRoot 'port\game-data\faery_tables.cpp'),
        (Join-Path $repoRoot 'port\game-data\combat.cpp'),
        (Join-Path $repoRoot 'port\game-data\combat_result.cpp'),
        (Join-Path $repoRoot 'port\game-data\item_gear_properties_v5.cpp'),
        (Join-Path $repoRoot 'port\game-data\effects_tables.cpp'),
        (Join-Path $repoRoot 'port\level-world\character_attack_geometry.cpp')
    )
    & $Compiler -std=c++17 -Wall -Wextra -Werror -Wno-missing-field-initializers -O2 `
        -ffunction-sections -fdata-sections @sources '-Wl,--gc-sections' -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'Hotty source cast tests did not compile' }
    $output = & $exe $cache $faery $hotty $effects
    if ($LASTEXITCODE -ne 0) { throw 'Hotty source cast tests failed' }
    $output
    foreach ($source in @('hotty_source_use_v1.cpp', 'hotty_effects_v1.cpp',
                          'hotty_cast_session_v1.cpp', 'hotty_character_cast_v1.cpp')) {
        & $Compiler -std=c++17 -Wall -Wextra -Werror -Wno-missing-field-initializers -fsyntax-only `
            (Join-Path $PSScriptRoot $source)
        if ($LASTEXITCODE -ne 0) { throw "Hotty source cast TU did not compile: $source" }
    }
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $exe) { Remove-Item -LiteralPath $exe }
}
