param([string]$AssetRoot)
$ErrorActionPreference='Stop'
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
if (!$AssetRoot) { $AssetRoot=Join-Path $repo '.local-inputs/windows-main-frontend-v1/assets' }
$compiler=Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output=Join-Path $repo '.local-inputs/source-itemdrop-scene-tests'
New-Item -ItemType Directory -Path $output -Force | Out-Null
$archives=@('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','libdh2_freetype237.a') | ForEach-Object { Join-Path $repo ('.local-inputs/windows-foundation-build/'+$_) }
$sources=@('port/windows-foundation/tests/source_itemdrop_scene_tests.cpp','port/game-data/loot_audiovisual_v8.cpp') | ForEach-Object { Join-Path $repo $_ }
$exe=Join-Path $output 'source-itemdrop-scene-tests.exe'
$native=Join-Path $repo 'port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a'
& $compiler '-std=c++17' '-O1' '-Wall' '-Wextra' '-Werror' '-static' '-ffunction-sections' '-fdata-sections' '-Wl,--gc-sections' @sources $native @archives $native '-o' $exe
if ($LASTEXITCODE -ne 0) { throw 'Original itemdrop scene compile/link failed' }
& $exe $AssetRoot
if ($LASTEXITCODE -ne 0) { throw 'Original itemdrop scene decode failed' }
