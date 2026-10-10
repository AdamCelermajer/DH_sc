# B039 harness runner. Builds into its own folder and runs the real mixer + WinMM output on this machine.
# Pass -Stress to add the control-thread starvation sweep (informational margin measurement).
param([switch]$Stress)
$ErrorActionPreference = 'Stop'
$workspace = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$compiler = Join-Path $workspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$buildDir = Join-Path $workspace '.local-inputs/b039-audio-pump-build'
New-Item -ItemType Directory -Force -Path $buildDir | Out-Null
$output = Join-Path $buildDir 'winmm_pump_underrun_tests.exe'
$sources = @(
    'port/windows-foundation/features/audio/winmm_pump_underrun_tests.cpp',
    'port/windows-foundation/features/audio/windows_source_session_control_v1.cpp',
    'port/windows-foundation/features/audio/winmm_output.cpp',
    'port/windows-foundation/features/audio/feature_audio.cpp',
    'port/engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.cpp',
    'port/engine-audio/audio_clock_v40.cpp',
    'port/engine-audio/audio_sample_v34.cpp',
    'port/engine-audio/audio_bank_v34.cpp',
    'port/engine-audio/audio_catalog_v34.cpp',
    'port/engine-audio/audio_native_envelope_v34.cpp',
    'port/engine-audio/audio_mixer_v34.cpp'
) | ForEach-Object { Join-Path $workspace $_ }
& $compiler -std=c++17 -O2 -Wall -Wextra -Werror -static `
    -I (Join-Path $workspace 'port/windows-foundation') `
    -I (Join-Path $workspace 'port/engine-audio') `
    @sources -lwinmm -o $output
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
if ($Stress) { & $output --stress } else { & $output }
exit $LASTEXITCODE
