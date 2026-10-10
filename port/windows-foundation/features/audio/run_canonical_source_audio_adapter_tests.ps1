$ErrorActionPreference = 'Stop'
$workspace = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$compiler = Resolve-Path (Join-Path $workspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe')
$output = Join-Path $workspace '.local-inputs/windows-toolchain/canonical_source_audio_adapter_tests.exe'
$source = @(
    'port/windows-foundation/features/audio/canonical_source_audio_adapter_tests.cpp',
    'port/windows-foundation/features/audio/canonical_source_audio_adapter.cpp',
    'port/windows-foundation/features/audio/platform_source_audio.cpp',
    'port/windows-foundation/features/audio/winmm_output.cpp',
    'port/windows-foundation/features/audio/feature_audio.cpp',
    'port/engine-audio/audio_gameplay_runtime_v42.cpp',
    'port/engine-audio/audio_clock_v40.cpp',
    'port/engine-audio/audio_source_bindings_v38.cpp',
    'port/engine-audio/audio_sample_v34.cpp',
    'port/engine-audio/audio_catalog_v34.cpp',
    'port/engine-audio/audio_bank_v34.cpp',
    'port/engine-audio/audio_native_envelope_v34.cpp',
    'port/engine-audio/audio_mixer_v34.cpp',
    'port/level-world/vox_play3d_owner_v2.cpp'
)
$absoluteSource = $source | ForEach-Object { Join-Path $workspace $_ }
& $compiler -std=c++17 -O2 -Wall -Wextra -Werror -static -I (Join-Path $workspace 'port/windows-foundation') -I (Join-Path $workspace 'port/engine-audio') -I (Join-Path $workspace 'port/level-world') @absoluteSource -lwinmm -o $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $output (Join-Path $PSScriptRoot 'assets')
exit $LASTEXITCODE
