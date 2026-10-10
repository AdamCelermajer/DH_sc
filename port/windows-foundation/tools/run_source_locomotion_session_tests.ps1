param([string]$AssetRoot)
$ErrorActionPreference='Stop'
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
if (!$AssetRoot) { $AssetRoot=Join-Path $repo '.local-inputs/windows-main-frontend-v1/assets' }
$compiler=Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output=Join-Path $repo '.local-inputs/source-locomotion-session-tests'
New-Item -ItemType Directory -Path $output -Force | Out-Null
$includes=@('port/windows-foundation','port/level-world','port/level-loader','port/engine-ui/vendor/freetype-2.3.7-hud/include','port/physics-backend/box2d-2.0.1/Include') | ForEach-Object { '-I'+(Join-Path $repo $_) }
$sources=@('port/windows-foundation/tests/source_locomotion_session_tests.cpp','port/windows-foundation/features/equipment/runtime_player_locomotion_v1.cpp','port/windows-foundation/features/equipment/runtime_player_locomotion_program_v1.cpp','port/level-world/player_equipment_queries_v1.cpp','port/level-world/character_stance.cpp','port/script-runtime/script_constants.cpp') | ForEach-Object { Join-Path $repo $_ }
$archives=@('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','libdh2_freetype237.a') | ForEach-Object { Join-Path $repo ('.local-inputs/windows-foundation-build/'+$_) }
$native=Join-Path $repo 'port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a'
$exe=Join-Path $output 'source-locomotion-session-tests.exe'
& $compiler '-std=c++17' '-O1' '-Wall' '-Wextra' '-Werror' '-Wno-missing-field-initializers' '-static' '-ffunction-sections' '-fdata-sections' '-Wl,--gc-sections' @includes @sources $native @archives $native '-o' $exe
if ($LASTEXITCODE -ne 0) { throw 'Source locomotion session compile/link failed' }
& $exe $AssetRoot
if ($LASTEXITCODE -ne 0) { throw 'Source locomotion session test failed' }
