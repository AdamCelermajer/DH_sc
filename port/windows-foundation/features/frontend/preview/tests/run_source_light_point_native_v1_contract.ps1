$ErrorActionPreference = 'Stop'
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../../../../')).Path
$compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
if (-not (Test-Path -LiteralPath $compiler)) { throw 'Configured clang++ toolchain not found' }
$outputDir = Join-Path $repoRoot '.local-inputs\frontend-light-point-native-contract'
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
$exe = Join-Path $outputDir 'source_light_point_native_v1_contract.exe'
$test = Join-Path $PSScriptRoot 'source_light_point_native_v1_contract.cpp'
$owner = Join-Path $repoRoot 'port\level-world\native_scene_lights_v113.cpp'
$resources = Join-Path $repoRoot 'port\engine-resources\resources.cpp'
$lightNames = Join-Path $repoRoot 'port\level-world\light_set_name_owner_v3.cpp'
& $compiler -std=c++17 -O0 -ffunction-sections -fdata-sections '-include' exception `
    '-I' (Join-Path $repoRoot 'port\level-world') `
    '-I' (Join-Path $repoRoot 'port\scene-materials') `
    '-I' (Join-Path $repoRoot 'port\engine-resources') `
    $test $owner $resources $lightNames (Join-Path $PSScriptRoot '..\source_light_point_native_v1.cpp') `
    '-Wl,--gc-sections' -static -o $exe
if ($LASTEXITCODE -ne 0) { throw "clang++ failed with exit code $LASTEXITCODE" }
& $exe
if ($LASTEXITCODE -ne 0) { throw "LightPoint contract test failed with exit code $LASTEXITCODE" }
