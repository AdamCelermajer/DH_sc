[CmdletBinding()]
param(
    [string]$AssetRoot = ".local-inputs/windows-shared-assets",
    [string]$OutputDirectory = ".local-inputs/frontend-profile-regression-v1"
)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..\..'))
$compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
$output = Join-Path $repoRoot $OutputDirectory
$exe = Join-Path $output 'profile_create_cancel_remove_regression_v1_tests.exe'
$assetRootPath = [IO.Path]::GetFullPath((Join-Path $repoRoot $AssetRoot))
$frontendArchive = Join-Path $repoRoot '.local-inputs\frontend-feature-build\libfrontend_components.a'
$foundation = Join-Path $repoRoot '.local-inputs\windows-foundation-build'
if (-not (Test-Path -LiteralPath $compiler)) { throw "Pinned LLVM-MinGW compiler missing: $compiler" }
if (-not (Test-Path -LiteralPath $frontendArchive)) { throw "Existing frontend feature archive missing: $frontendArchive" }
if (-not (Test-Path -LiteralPath $assetRootPath)) { throw "Actual source asset root missing: $assetRootPath" }
New-Item -ItemType Directory -Force -Path $output | Out-Null
$includes = @(
    '-Iport/windows-foundation', '-Iport/level-world', '-Iport/level-loader',
    '-Iport/game-data', '-Iport/engine-animation', '-Iport/engine-skinning',
    '-Iport/scene-materials', '-Iport/physics-backend/box2d-2.0.1/Include'
)
$libraries = @(
    $frontendArchive,
    (Join-Path $foundation 'libfoundation_data.a'),
    (Join-Path $foundation 'librecovered_content.a'),
    (Join-Path $foundation 'libcontent_xml.a'),
    (Join-Path $foundation 'libdh2_freetype237.a'),
    (Join-Path $foundation 'librecovered_trigger_contacts.a'),
    (Join-Path $foundation 'physics-backend\libdh2_box2d_201.a')
)
foreach ($library in $libraries) {
    if (-not (Test-Path -LiteralPath $library)) { throw "Verified Foundation archive missing: $library" }
}
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $compiler -Parent) + ';' + $env:PATH
    Push-Location $repoRoot
    try {
        & $compiler -std=c++17 -O0 -static -Wall -Wextra -Werror -Wno-unused-value `
            -Wno-missing-field-initializers `
            -Wno-misleading-indentation -Dfinite=_finite -ffunction-sections -fdata-sections `
            -fuse-ld=lld '-Wl,--gc-sections' @includes `
            'port/windows-foundation/features/frontend/creation/profile_create_cancel_remove_regression_v1_tests.cpp' `
            'port/windows-foundation/features/pause_ui/source_pause_ui_v1.cpp' `
            'port/windows-foundation/features/pause_ui/source_pause_ui_render_v1.cpp' `
            'port/windows-foundation/features/pause_ui/source_pause_ui_art_v1.cpp' `
            @libraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 `
            -luuid -lcomdlg32 -ladvapi32 -o $exe
        if ($LASTEXITCODE -ne 0) { throw "Profile create/cancel/remove regression compile failed: $LASTEXITCODE" }
        & $exe $assetRootPath
        if ($LASTEXITCODE -ne 0) { throw "Profile create/cancel/remove regression failed: $LASTEXITCODE" }
    } finally { Pop-Location }
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $exe) { Remove-Item -LiteralPath $exe }
}
