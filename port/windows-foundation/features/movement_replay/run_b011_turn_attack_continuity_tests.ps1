param([string]$AssetRoot,[string]$BuildRoot)
$ErrorActionPreference='Stop'
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if(!$AssetRoot){$AssetRoot=Join-Path $repo '.local-inputs/windows-source-clock-v19-preview-9/assets'}
if(!$BuildRoot){$BuildRoot=Join-Path $repo '.local-inputs/b011-turn-attack-build'}
$build=[IO.Path]::GetFullPath($BuildRoot)
$shared=[IO.Path]::GetFullPath((Join-Path $repo '.local-inputs/windows-foundation-build'))
if($build -eq $shared){throw 'B011 runner requires a private build directory'}
$null=New-Item -ItemType Directory -Path $build -Force
$archives=@('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a')
foreach($archive in $archives){$from=Join-Path $shared $archive;if(!(Test-Path -LiteralPath $from -PathType Leaf)){throw "Required archive absent: $from"};$to=Join-Path $build $archive;$null=New-Item -ItemType Directory -Path (Split-Path -Parent $to) -Force;Copy-Item -LiteralPath $from -Destination $to -Force}
$compiler=Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$includes=@('port/windows-foundation','port/game-data','port/level-world','port/level-loader','port/engine-ui/vendor/freetype-2.3.7-hud/include','port/physics-backend/box2d-2.0.1/Include')|ForEach-Object{'-I'+(Join-Path $repo $_)}
$sources=@('port/windows-foundation/features/movement_replay/b011_turn_attack_continuity_tests.cpp','port/windows-foundation/features/platform_input/semantic_input.cpp','port/windows-foundation/features/combat/runtime_player_combo_chain_v1.cpp','port/windows-foundation/features/equipment/runtime_player_locomotion_v1.cpp','port/windows-foundation/combat_session.cpp','port/level-world/player_equipment_queries_v1.cpp','port/level-world/character_stance.cpp','port/script-runtime/script_constants.cpp')|ForEach-Object{Join-Path $repo $_}
$libs=$archives|ForEach-Object{Join-Path $build $_}
$exe=Join-Path $build 'b011_turn_attack_continuity_tests.exe'
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' '-Wno-missing-field-initializers' '-static' @includes @sources @libs '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' '-lole32' '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $exe
if($LASTEXITCODE -ne 0){exit $LASTEXITCODE}
& $exe $AssetRoot
exit $LASTEXITCODE
