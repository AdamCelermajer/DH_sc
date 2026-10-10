$ErrorActionPreference = 'Stop'
$workspace = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$compiler = Join-Path $workspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $workspace '.local-inputs/windows-toolchain/runtime_audio_host_v1_tests.exe'
$sources = @(
    'port/windows-foundation/features/audio/runtime_audio_host_v1_tests.cpp',
    'port/windows-foundation/features/audio/runtime_audio_host_v1.cpp',
    'port/windows-foundation/features/audio/runtime_combat_audio_v1.cpp',
    'port/windows-foundation/features/audio/windows_source_session_control_v1.cpp',
    'port/windows-foundation/features/audio/winmm_output.cpp',
    'port/windows-foundation/features/audio/feature_audio.cpp',
    'port/engine-audio/audio_source_command_v40.cpp',
    'port/engine-audio/vox_source_fields_v38.cpp',
    'port/engine-audio/audio_spatial_v34.cpp',
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
& $output `
    (Join-Path $PSScriptRoot 'assets') `
    (Join-Path $workspace '.local-inputs/combat-sound-v2/sounds_pyarray.bin')
exit $LASTEXITCODE
