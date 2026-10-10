param(
    [string]$Root = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..\..')).Path
)
$ErrorActionPreference = 'Stop'
$asset = Join-Path $Root '.local-inputs\windows-source-clock-v19-preview-10-candidate\assets\data\3d\textures\fx_spell_lightning_ground_flash.tga'
$bdae = Join-Path $Root '.local-inputs\windows-source-clock-v19-preview-10-candidate\assets\data\3d\interface\spell_dh2_faery_lightning.bdae'
$capture = Join-Path $Root '.local-inputs\v19-frontend-hotfix\profile-projection\rogue-celest-fx-root-review27.png'
$expected = @{
    $asset = 'EBC5CFAACC129976AED397434D5B67A33A276C6F7CE75780E39A0C12F1E28CBA'
    $bdae = '24DE71141C46541F930841055880000446EB77A0C100DC09CABAD92550012B3E'
    $capture = 'D12A2D2A5F426EC214632D3077A9AFC984D4D8149F7F3482391FD1CAD322779A'
}
foreach ($path in @($asset, $bdae, $capture)) {
    if (!(Test-Path -LiteralPath $path)) { throw "Missing pinned source/evidence file: $path" }
    $actual = (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash
    if ($actual -ne $expected[$path]) { throw "Pinned SHA-256 changed for $path : $actual" }
}
$exe = Join-Path $Root '.local-inputs\celest-ground-flash-pixel-regression'
$rootPosix = '/mnt/' + $Root.Substring(0, 1).ToLowerInvariant() + '/' + $Root.Substring(3).Replace('\', '/')
$source = 'port/windows-foundation/features/effects/celest_ground_flash_pixel_regression_v1.cpp'
$compileArgs = @('--cd', $rootPosix, '--exec', 'g++', '-Iport/engine-textures', '-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror', $source, 'port/engine-textures/textures.cpp', 'port/engine-textures/pvrtc.cpp', '-o', '.local-inputs/celest-ground-flash-pixel-regression')
& wsl.exe @compileArgs
if ($LASTEXITCODE -ne 0) { throw "Strict focused regression compile failed ($LASTEXITCODE)" }
$assetPosix = '/mnt/' + $asset.Substring(0, 1).ToLowerInvariant() + '/' + $asset.Substring(3).Replace('\', '/')
& wsl.exe --cd $rootPosix --exec ./.local-inputs/celest-ground-flash-pixel-regression $assetPosix
if ($LASTEXITCODE -ne 0) { throw "Focused pixel/pass regression failed ($LASTEXITCODE)" }
