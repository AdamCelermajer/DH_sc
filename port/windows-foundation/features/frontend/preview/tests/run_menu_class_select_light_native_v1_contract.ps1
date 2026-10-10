$ErrorActionPreference = 'Stop'
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../../../../')).Path
$compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
if (-not (Test-Path -LiteralPath $compiler)) { throw 'Configured clang++ toolchain not found' }
$outputDir = Join-Path $repoRoot '.local-inputs\menu-class-select-light-native-contract'
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
$exe = Join-Path $outputDir 'menu_class_select_light_native_v1_contract.exe'
$test = Join-Path $PSScriptRoot 'menu_class_select_light_native_v1_contract.cpp'
$owner = Join-Path $repoRoot 'port\level-world\native_scene_lights_v113.cpp'
$resources = Join-Path $repoRoot 'port\engine-resources\resources.cpp'
$lightNames = Join-Path $repoRoot 'port\level-world\light_set_name_owner_v3.cpp'
$menuOwner = Join-Path $repoRoot 'port\level-world\menu_class_select_light_owner_v1.cpp'
& $compiler -std=c++17 -O0 -ffunction-sections -fdata-sections '-include' exception `
    '-I' (Join-Path $repoRoot 'port\level-world') `
    '-I' (Join-Path $repoRoot 'port\scene-materials') `
    '-I' (Join-Path $repoRoot 'port\engine-resources') `
    $test $menuOwner $owner $resources $lightNames `
    '-Wl,--gc-sections' -static -o $exe
if ($LASTEXITCODE -ne 0) { throw "clang++ failed with exit code $LASTEXITCODE" }
& $exe
if ($LASTEXITCODE -ne 0) { throw "Class-select light contract test failed with exit code $LASTEXITCODE" }
