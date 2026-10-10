$ErrorActionPreference = 'Stop'
$workspace = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$compiler = Join-Path $workspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $workspace '.local-inputs/windows-toolchain/audio_source_asset_provenance_v1_tests.exe'
$sourceCache = Join-Path $workspace 'port/level-world/reference/character-visual-v6/cache'
$quickness = Join-Path $sourceCache 'data/3d/characters/prince/animations/skill_dh2_prince_rogue_quickness.bdae'
$roundhouse = Join-Path $sourceCache 'data/3d/characters/prince/animations/skill_dh2_prince_rogue_roundhouse.bdae'
$expected = @(
    @{ Path = $quickness; Bytes = 11764; Hash = '261f8c8215f8e19407517f4cdbf9e4929a5f84ba69a55d2cb5ea18601c0d4d00' },
    @{ Path = $roundhouse; Bytes = 20660; Hash = '2d49b491e980e2d4f11828edf88c8e803931389a5289b9903a07f9a259a52950' }
)
foreach ($asset in $expected) {
    $file = Get-Item -LiteralPath $asset.Path
    $hash = (Get-FileHash -LiteralPath $asset.Path -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($file.Length -ne $asset.Bytes -or $hash -ne $asset.Hash) {
        throw "Source manifest mismatch for $($asset.Path): bytes=$($file.Length) sha256=$hash"
    }
}
$libraries = @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a') |
    ForEach-Object { Join-Path $workspace ('.local-inputs/windows-foundation-build/' + $_) }
$testSource = Join-Path $PSScriptRoot 'audio_source_asset_provenance_v1_tests.cpp'
& $compiler -std=c++17 -O2 -Wall -Wextra -Werror -static `
    -I (Join-Path $workspace 'port/windows-foundation') `
    -I (Join-Path $workspace 'port/engine-animation') `
    -I (Join-Path $workspace 'port/scene-materials') `
    -I (Join-Path $workspace 'port/engine-resources') `
    $testSource @libraries -o $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output $sourceCache
exit $LASTEXITCODE
