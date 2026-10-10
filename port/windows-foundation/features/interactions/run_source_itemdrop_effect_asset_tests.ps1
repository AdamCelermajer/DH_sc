param([string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath($RepositoryRoot)
$compiler = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $root 'port/windows-foundation/features/interactions/test-output/source_itemdrop_effect_asset_tests.exe'
$itemdrops = Join-Path $root '.local-inputs/item-visual-cache-v2/itemdrops.bdae'
$canonicalEffect = Join-Path $root '.local-inputs/interactions-source-material-cache-v1/candidate-data-gfx-effects/gl_diffuse_l1_vc_iphone.bdae'
$rootDuplicate = Join-Path $root '.local-inputs/interactions-source-material-cache-v1/candidate-root/gl_diffuse_l1_vc_iphone.bdae'
$canonicalHash = (Get-FileHash -Algorithm SHA256 -LiteralPath $canonicalEffect).Hash.ToLowerInvariant()
$rootHash = (Get-FileHash -Algorithm SHA256 -LiteralPath $rootDuplicate).Hash.ToLowerInvariant()
if ($canonicalHash -ne '10c64054906caf1683f3669becf415fe20196491ac5f182881cb69705d080bdc') {
    throw "Canonical source-cache effect BDAE hash changed: $canonicalHash"
}
if ($rootHash -ne 'af9518292b54a0bcd78db7682ecd7cfbd48cb50347dd495718446f42166a9d2d') {
    throw "Root duplicate source-cache effect BDAE hash changed: $rootHash"
}
$arguments = @(
    '-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror',
    '-ffunction-sections', '-fdata-sections', '-Wl,--gc-sections',
    ('-I' + (Join-Path $root 'port/engine-resources')),
    (Join-Path $root 'port/windows-foundation/features/interactions/source_itemdrop_effect_asset_tests.cpp'),
    (Join-Path $root 'port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a'),
    (Join-Path $root '.local-inputs/windows-foundation-build/libfoundation_data.a'),
    '-static', '-lkernel32', '-o', $output
)
New-Item -ItemType Directory -Force (Split-Path $output) | Out-Null
& $compiler @arguments
if ($LASTEXITCODE -ne 0) { throw "Source itemdrop effect asset test compile failed ($LASTEXITCODE)" }
$resultText = & $output $itemdrops $canonicalEffect $rootDuplicate
if ($LASTEXITCODE -ne 0) { throw "Source itemdrop effect asset test failed ($LASTEXITCODE): $resultText" }
$result = $resultText | ConvertFrom-Json
if ($result.validation -ne 'PASS') { throw 'Source itemdrop effect asset test did not pass' }
$result | Add-Member -NotePropertyName canonical_effect_sha256 -NotePropertyValue $canonicalHash
$result | Add-Member -NotePropertyName root_duplicate_effect_sha256 -NotePropertyValue $rootHash
$result | ConvertTo-Json -Depth 12 -Compress
