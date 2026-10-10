$ErrorActionPreference = 'Stop'
$taskWorkspace = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$taskCompiler = Join-Path $taskWorkspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskOutput = Join-Path $taskWorkspace '.local-inputs/windows-foundation-build/combat_session_audio_hardware_tests.exe'
$taskSources = @(
    'port/windows-foundation/tests/combat_session_audio_hardware_tests.cpp',
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
    'port/level-world/vox_play3d_owner_v2.cpp'
) | ForEach-Object { Join-Path $taskWorkspace $_ }
$taskLibraries = @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','librecovered_trigger_contacts.a','physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $taskWorkspace ('.local-inputs/windows-foundation-build/' + $_) }
& $taskCompiler -std=c++17 -O2 -Wall -Wextra -Werror -Wno-missing-field-initializers -static @taskSources @taskLibraries -lwinmm -o $taskOutput
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $taskOutput (Join-Path $taskWorkspace '.local-inputs/windows-shared-assets') (Join-Path $taskWorkspace 'port/windows-foundation/features/audio/assets')
exit $LASTEXITCODE
