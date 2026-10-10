param([string]$AssetRoot,[string]$BuildRoot)
$ErrorActionPreference='Stop'
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
if(!$AssetRoot){$AssetRoot=Join-Path $repo '.local-inputs/windows-main-frontend-v1/assets'}
if(!$BuildRoot){$BuildRoot=Join-Path $repo '.local-inputs/session-actor-transition-v1-private-build'}
$build=[IO.Path]::GetFullPath($BuildRoot)
$compiler=Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$shared=Join-Path $repo '.local-inputs/windows-foundation-build'
$null=New-Item -ItemType Directory -Path $build -Force
$libraries=@('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a',
              'librecovered_trigger_contacts.a','librecovered_content.a',
              'physics-backend/libdh2_box2d_201.a') | ForEach-Object {
    $source=Join-Path $shared $_
    if(!(Test-Path -LiteralPath $source)){throw "Required coherent input archive is missing: $source"}
    $destination=Join-Path $build $_
    $null=New-Item -ItemType Directory -Path (Split-Path -Parent $destination) -Force
    $before=(Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash
    Copy-Item -LiteralPath $source -Destination $destination -Force
    if($before -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash){throw "Shared archive changed during private snapshot: $source"}
    $destination
}
$includes=@('port/windows-foundation','port/game-data','port/level-world',
            'port/level-loader','port/scene-materials','port/engine-animation',
            'port/engine-skinning','port/engine-resources',
            'port/engine-ui/vendor/freetype-2.3.7-hud/include',
            'port/physics-backend/box2d-2.0.1/Include') | ForEach-Object {'-I'+(Join-Path $repo $_)}
$sources=@(
    'port/windows-foundation/features/physics/session_actor_transition_v1_tests.cpp',
    'port/windows-foundation/features/physics/session_actor_transition_v1.cpp',
    'port/windows-foundation/features/enemy_ai/runtime_enemy_navigation_v1.cpp',
    'port/windows-foundation/combat_session.cpp',
    'port/windows-foundation/actor_combat_runtime.cpp',
    'port/windows-foundation/combat_system.cpp',
    'port/windows-foundation/playable_actor_world.cpp',
    'port/windows-foundation/playable_actor_bodies.cpp',
    'port/windows-foundation/original_actor_physical.cpp',
    'port/windows-foundation/world.cpp',
    'port/windows-foundation/actor_state.cpp',
    'port/windows-foundation/original_actor_motion_flags.cpp',
    'port/windows-foundation/features/equipment/runtime_player_locomotion_v1.cpp',
    'port/windows-foundation/features/combat/runtime_player_profile_attack_bank_v1.cpp',
    'port/windows-foundation/features/combat/runtime_player_combo_chain_v1.cpp',
    'port/level-world/player_equipment_queries_v1.cpp',
    'port/level-world/character_stance.cpp'
) | ForEach-Object {Join-Path $repo $_}
$output=Join-Path $build 'session_actor_transition_v1_tests.exe'
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-Wno-unused-value' '-Dfinite=_finite' '-static' `
    @includes @sources @libraries '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' `
    '-lshell32' '-lole32' '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $output
if($LASTEXITCODE -ne 0){throw "Private Session transition compile failed with $LASTEXITCODE"}
& $output $AssetRoot 2>&1 | Tee-Object -FilePath (Join-Path $build 'run.log')
if($LASTEXITCODE -ne 0){throw "Session actor transition regression failed; see $build/run.log"}

$filterSources=@(
    'port/windows-foundation/tests/source_physical_filter_ops_tests.cpp',
    'port/windows-foundation/playable_actor_bodies.cpp',
    'port/windows-foundation/original_actor_physical.cpp'
) | ForEach-Object {Join-Path $repo $_}
$filterOutput=Join-Path $build 'source_physical_filter_ops_tests.exe'
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-Wno-unused-value' '-Dfinite=_finite' '-static' `
    @includes @filterSources @libraries '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' `
    '-lshell32' '-lole32' '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $filterOutput
if($LASTEXITCODE -ne 0){throw "Private source physical-filter compile failed with $LASTEXITCODE"}
& $filterOutput $AssetRoot 2>&1 | Tee-Object -FilePath (Join-Path $build 'source_physical_filter_ops_run.log')
if($LASTEXITCODE -ne 0){throw "Source physical-filter regression failed; see $build/source_physical_filter_ops_run.log"}
