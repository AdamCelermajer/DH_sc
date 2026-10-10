# B040 level ambience asset tests: standalone compile of level_music_v1_tests.cpp + audio_sample_v34.cpp,
# then SHA-256 check of the staged swamp VXN files against source-subset-manifest.json.
$ErrorActionPreference = 'Stop'
$root = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$toolchain = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin'
$clang = Join-Path $toolchain 'clang++.exe'
$build = Join-Path $root '.local-inputs/level-music-v1-build'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$exe = Join-Path $build 'level_music_v1_tests.exe'
$test = Join-Path $PSScriptRoot 'level_music_v1_tests.cpp'
$sample = Join-Path $root 'port/engine-audio/audio_sample_v34.cpp'
& $clang -std=gnu++17 -O1 -Wall -Wextra -Werror "-I$(Join-Path $root 'port/engine-audio')" $test $sample -o $exe
if ($LASTEXITCODE -ne 0) { throw "compile failed: $LASTEXITCODE" }
$assets = Join-Path $PSScriptRoot 'assets'
$env:PATH = "$toolchain;$env:PATH"
& $exe $assets
if ($LASTEXITCODE -ne 0) { throw "native test failed: $LASTEXITCODE" }
# Hash check: manifest entries for the staged VXN files must match the actual SHA-256.
$manifest = Get-Content -Raw (Join-Path $PSScriptRoot 'source-subset-manifest.json') | ConvertFrom-Json
$hashFail = 0
foreach ($entry in $manifest.entries) {
    if ($entry.path -notlike 'data/sounds/m_level_swamp_sfx_*.vxn') { continue }
    $file = Join-Path $assets $entry.path
    $actual = (Get-FileHash -Algorithm SHA256 -LiteralPath $file).Hash.ToLowerInvariant()
    $size = (Get-Item -LiteralPath $file).Length
    $ok = ($actual -eq $entry.sha256) -and ($size -eq $entry.size)
    if (-not $ok) { $hashFail++ }
    Write-Output ("{0} {1} sha256={2} bytes={3}" -f $(if ($ok) { 'HASH-PASS' } else { 'HASH-FAIL' }), $entry.path, $actual, $size)
}
if ($hashFail -ne 0) { throw "hash failures: $hashFail" }
Write-Output 'HASH ALL PASS'
