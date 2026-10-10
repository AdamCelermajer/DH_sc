$ErrorActionPreference = 'Stop'
$workspace = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$compiler = Join-Path $workspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $workspace '.local-inputs/windows-toolchain/frontend_menu_audio_v1_tests.exe'
$assets = Join-Path $workspace '.local-inputs/frontend-menu-audio-v1/assets'
$sourceAssets = Join-Path $PSScriptRoot 'assets'
$selectSample = Join-Path $workspace '.local-inputs/audio-v34/cache/sfx_menu_select.wav'
$confirmSample = Join-Path $workspace '.local-inputs/audio-v34/cache/sfx_menu_confirm.wav'
if (Test-Path -LiteralPath $assets) { Remove-Item -LiteralPath $assets -Recurse -Force }
New-Item -ItemType Directory -Path $assets | Out-Null
Copy-Item -Path (Join-Path $sourceAssets '*') -Destination $assets -Recurse -Force
New-Item -ItemType Directory -Path (Join-Path $assets 'data/sounds') -Force | Out-Null
Copy-Item -LiteralPath $selectSample -Destination (Join-Path $assets 'data/sounds/sfx_menu_select.wav') -Force
Copy-Item -LiteralPath $confirmSample -Destination (Join-Path $assets 'data/sounds/sfx_menu_confirm.wav') -Force
$selectHash = (Get-FileHash -LiteralPath (Join-Path $assets 'data/sounds/sfx_menu_select.wav') -Algorithm SHA256).Hash.ToLowerInvariant()
if ($selectHash -ne '8afb7665dec542af18f914295adb0ea7ecfb498ba5973d573d755ae485e6799d') { throw "MenuSelect sample hash changed: $selectHash" }
$confirmHash = (Get-FileHash -LiteralPath (Join-Path $assets 'data/sounds/sfx_menu_confirm.wav') -Algorithm SHA256).Hash.ToLowerInvariant()
if ($confirmHash -ne 'b8bd71d85b23c9a6f24b4650d98fc533b5a6e11034bbfb6277294fc1c9705ae5') { throw "MenuConfirm sample hash changed: $confirmHash" }
$sources = @(
    'port/windows-foundation/features/audio/frontend_menu_audio_v1_tests.cpp',
    'port/windows-foundation/features/audio/frontend_menu_audio_v1.cpp',
    'port/windows-foundation/features/audio/windows_source_session_control_v1.cpp',
    'port/windows-foundation/features/audio/winmm_output.cpp',
    'port/windows-foundation/features/audio/feature_audio.cpp',
    'port/engine-audio/integration-v42/audio_native_session_v42.cpp',
    'port/engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.cpp',
    'port/engine-audio/audio_gameplay_runtime_v42.cpp',
    'port/engine-audio/audio_clock_v40.cpp',
    'port/engine-audio/audio_source_bindings_v38.cpp',
    'port/engine-audio/audio_sample_v34.cpp',
    'port/engine-audio/audio_catalog_v34.cpp',
    'port/engine-audio/audio_bank_v34.cpp',
    'port/engine-audio/audio_native_envelope_v34.cpp',
    'port/engine-audio/audio_mixer_v34.cpp',
    'port/level-world/vox_play3d_owner_v2.cpp',
    'port/level-world/character_combat_sound_v1.cpp',
    'port/level-world/character_combat_sound_tables_v2.cpp'
) | ForEach-Object { Join-Path $workspace $_ }
$libraries = @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','librecovered_trigger_contacts.a','physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $workspace ('.local-inputs/windows-foundation-build/' + $_) }
& $compiler -std=c++17 -O2 -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    -I (Join-Path $workspace 'port/windows-foundation') `
    -I (Join-Path $workspace 'port/engine-audio') `
    -I (Join-Path $workspace 'port/level-world') `
    @sources @libraries -lwinmm -o $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output $assets
exit $LASTEXITCODE
