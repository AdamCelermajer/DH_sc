$ErrorActionPreference = 'Stop'
$workspace = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$compiler = Join-Path $workspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $workspace '.local-inputs/windows-toolchain/audio_source_cue_submission_v1_tests.exe'
$samplePath = Join-Path $workspace '.local-inputs/audio-v34/cache/sfx_menu_select.wav'
$sample = Get-Item -LiteralPath $samplePath
$sampleHash = (Get-FileHash -LiteralPath $samplePath -Algorithm SHA256).Hash.ToLowerInvariant()
if ($sample.Length -ne 10784 -or $sampleHash -ne '8afb7665dec542af18f914295adb0ea7ecfb498ba5973d573d755ae485e6799d') {
    throw "MenuSelect source WAV provenance mismatch: bytes=$($sample.Length) sha256=$sampleHash"
}
$sources = @(
    'port/windows-foundation/features/audio/audio_source_cue_submission_v1.cpp',
    'port/windows-foundation/features/audio/audio_source_cue_submission_v1_tests.cpp',
    'port/engine-audio/audio_gameplay_runtime_v42.cpp',
    'port/engine-audio/audio_source_bindings_v38.cpp',
    'port/engine-audio/audio_catalog_v34.cpp',
    'port/engine-audio/audio_sample_v34.cpp',
    'port/engine-audio/audio_bank_v34.cpp',
    'port/engine-audio/audio_mixer_v34.cpp',
    'port/engine-audio/audio_native_envelope_v34.cpp',
    'port/engine-audio/audio_clock_v40.cpp',
    'port/engine-audio/audio_source_command_v40.cpp',
    'port/engine-audio/vox_source_fields_v38.cpp',
    'port/engine-audio/audio_spatial_v34.cpp',
    'port/level-world/vox_play3d_owner_v2.cpp',
    'port/level-world/character_combat_sound_v1.cpp',
    'port/level-world/character_combat_sound_tables_v2.cpp'
) | ForEach-Object { Join-Path $workspace $_ }
$libraries = @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a') |
    ForEach-Object { Join-Path $workspace ('.local-inputs/windows-foundation-build/' + $_) }
& $compiler -std=c++17 -O2 -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    -I (Join-Path $workspace 'port/windows-foundation') `
    -I (Join-Path $workspace 'port/engine-audio') `
    -I (Join-Path $workspace 'port/level-world') `
    @sources @libraries -o $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output $workspace
exit $LASTEXITCODE
